#include "global.h"
#include "runtime.h"
#include "ram_map.h"
#include "actor.h"
#include "gba/defines.h"

/*
 * Enemy HP bars — LynJump DrawActors / InitActorParams.
 *
 * Safety rules (from the failed soft-OAM-at-camera attempt):
 *  - Soft OAM is gSoftOam @ 0x03001F70 (NOT 0x030009C0 — that is gCameras).
 *  - Only append disabled tail slots after vanilla DrawActors finishes.
 *  - Upload bar tiles once via QueueVramTransfer; never touch OBJ VRAM/pal
 *    on the per-frame path.
 */

#define RGB5(r, g, b) ((u16)((r) | ((g) << 5) | ((b) << 10)))

#define BAR_TILE_BASE 0x3E0
#define BAR_LEVELS 9
#define BAR_TILES_PER_LEVEL 2 /* 16×8 */
#define BAR_PALETTE 15
#define BAR_WIDTH_PX 16
#define BAR_Y_NUDGE 10

#define ATTR0_SHAPE_H 0x4000
#define ATTR1_SIZE_16x8 0x4000

typedef bool8 (*QueueVramTransferFunc)(const void *src, void *dst, u16 size, u16 prio);

void DrawActors__Continue(void);

APPEND_TEXT static void BuildHpBarTiles(void)
{
    u32 level;
    u32 *dst;
    u32 row;
    u32 col;
    u32 fillPx;
    u32 color;
    u32 shift;
    u32 left;
    u32 right;

    dst = gHpBarTileScratch;
    for (level = 0; level < BAR_LEVELS; level++)
    {
        fillPx = (level * BAR_WIDTH_PX) / (BAR_LEVELS - 1);
        for (row = 0; row < 8; row++)
        {
            left = 0;
            right = 0;
            if (row >= 2 && row <= 5)
            {
                for (col = 0; col < BAR_WIDTH_PX; col++)
                {
                    if (row == 2 || row == 5 || col == 0 || col == BAR_WIDTH_PX - 1)
                        color = 3;
                    else if (col < fillPx)
                        color = 2;
                    else
                        color = 1;

                    if (col < 8)
                    {
                        shift = col * 4;
                        left |= (color & 0xF) << shift;
                    }
                    else
                    {
                        shift = (col - 8) * 4;
                        right |= (color & 0xF) << shift;
                    }
                }
            }
            dst[row] = left;
            dst[8 + row] = right;
        }
        dst += 16;
    }
}

APPEND_TEXT static void EnsureHpBarGfx(void)
{
    u16 *pal;
    QueueVramTransferFunc queue;

    if (gHpBarTilesReady)
        return;

    BuildHpBarTiles();

    pal = (u16 *)(OBJ_PLTT + BAR_PALETTE * 32);
    pal[0] = 0;
    pal[1] = RGB5(4, 4, 6);
    pal[2] = RGB5(0, 28, 8);
    pal[3] = RGB5(28, 28, 28);

    queue = (QueueVramTransferFunc)0x08004811;
    if (queue(gHpBarTileScratch,
              (void *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(BAR_TILE_BASE)),
              BAR_LEVELS * BAR_TILES_PER_LEVEL * 32,
              0x10))
        gHpBarTilesReady = 1;
}

APPEND_TEXT static void DrawEnemyHpBars(void)
{
    u32 i;
    u8 cursor;
    u8 limit;

    EnsureHpBarGfx();
    if (!gHpBarTilesReady)
        return;

    cursor = gOamCursor;
    limit = gSoftOamCount;
    if (limit > 128)
        limit = 128;

    for (i = 1; i < ACTOR_COUNT && cursor < limit; i++)
    {
        u8 *actor = &gActorPool[i * ACTOR_STRIDE];
        u32 hp;
        u32 maxHp;
        s32 sx;
        s32 sy;
        u32 level;
        u16 *entry;

        if ((*(u16 *)(actor + ACTOR_OFF_FLAGS) & ACTOR_FLAG_ACTIVE) == 0
            || *(u16 *)(actor + ACTOR_OFF_ANIM) == 0
            || (actor[ACTOR_OFF_KIND] & ACTOR_KIND_YSORT_EXCL) != 0)
            continue;

        hp = *(u32 *)(actor + ACTOR_OFF_HP);
        maxHp = gActorMaxHp[i];
        if (maxHp == 0 || hp == 0 || hp > 0x10000u)
            continue;
        if (hp > maxHp)
        {
            gActorMaxHp[i] = hp;
            maxHp = hp;
        }

        GetActorScreenPos(actor, &sx, &sy);
        sx -= BAR_WIDTH_PX / 2;
        sy += BAR_Y_NUDGE;
        if (sx <= -BAR_WIDTH_PX || sx >= DISPLAY_WIDTH || sy <= -8 || sy >= DISPLAY_HEIGHT)
            continue;

        level = (hp * (BAR_LEVELS - 1)) / maxHp;
        if (level >= BAR_LEVELS)
            level = BAR_LEVELS - 1;
        if (hp > 0 && level == 0)
            level = 1;

        entry = &gSoftOam[cursor * 4];
        entry[0] = (u16)((sy & 0xFF) | ATTR0_SHAPE_H);
        entry[1] = (u16)((sx & 0x1FF) | ATTR1_SIZE_16x8);
        entry[2] = (u16)((BAR_TILE_BASE + level * BAR_TILES_PER_LEVEL)
                         | (BAR_PALETTE << 12));
        entry[3] = 0;
        cursor++;
    }

    gOamCursor = cursor;
}

APPEND_TEXT void DrawActors__Replacement(void)
{
    DrawActors__Continue();
    if (gRuntimeConfig.enemy_hp_bars)
        DrawEnemyHpBars();
}

APPEND_TEXT void InitActorParams__Replacement(u8 index, s32 hp, s32 param38, s32 expPool, u8 kind)
{
    /* Decompiled body lives in src/actor.c (append); baserom entry is veneered. */
    InitActorParams(index, hp, param38, expPool, kind);

    if (gRuntimeConfig.enemy_hp_bars && index < ACTOR_COUNT && hp > 0 && hp <= 0xFFFF)
        gActorMaxHp[index] = (u32)hp;
}
