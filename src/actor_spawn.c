#include "global.h"
#include "actor.h"
#include "actor_spawn.h"
#include "ram_map.h"

/*
 * Actor spawn / despawn / visibility / walk — decompiled from baserom helpers
 * used heavily by Chapter 1 cutscenes (SpawnActor @ 0x08006178 ×14 in the
 * 0x08053Bxx setup, DeleteActor @ 0x08006310).
 *
 * Linked into append (.text) as named reference peels. Vanilla ROM still owns
 * the original bytes; call sites keep absolute Thumb until a LynJump redirects.
 */

typedef void (*CpuSetFunc)(const void *src, void *dst, u32 control);
typedef void *(*LookupAnmFunc)(u16 typeId);
typedef u8 (*AllocAnmSlotFunc)(u8 bank);
typedef u16 (*AllocTilesFunc)(u16 size);
typedef void (*FreeAnmSlotFunc)(u8 slot);
typedef void (*FreeTilesFunc)(u16 a, u16 b);

#define CpuSetRom     ((CpuSetFunc)0x08054445) /* SVC 11 wrapper */
#define LookupAnm     ((LookupAnmFunc)0x080037B5)
#define AllocAnmSlot  ((AllocAnmSlotFunc)0x08005C81)
#define AllocTiles    ((AllocTilesFunc)0x08005CC9)
#define FreeAnmSlot   ((FreeAnmSlotFunc)0x08005CA9)
#define FreeTiles     ((FreeTilesFunc)0x08005D95)

#define ACTOR_CLEAR_CONTROL 0x01000030 /* CpuSet: fill, 0x30 words */
#define SPAWN_FLAG_OR       SPAWN_ACTOR_DEFAULT_FLAGS
#define FIELD1E_NONE        SPAWN_ACTOR_FIELD1E_NONE
#define TILE_ALLOC_FAIL     0xFFFF

/* SpawnActor scratch tables (type-id cache). */
#define gAnmTypeCount (*(vu8 *)0x03000C74)
#define gAnmTypeIds   ((vu16 *)0x030038C0)
#define gAnmTypeAux   ((vu16 *)0x030038D0)

u8 SpawnActor(u8 camera, u16 anim, u16 typeId, u16 x, u16 y, u16 unusedArg,
              u16 flagBits, u16 field1e)
{
    u8 index;
    u8 *actor;
    u8 *anm;
    u8 i;
    u16 tileId;
    u16 flags;
    s32 posX;
    s32 posY;
    u8 activeBit;
    u32 zero;

    (void)unusedArg;

    index = gActorAllocIndex;
    if (index > 0x27)
        return 0xFF;

    for (;;)
    {
        actor = &gActorPool[(u32)index * ACTOR_STRIDE];
        if ((*(u16 *)(actor + ACTOR_OFF_FLAGS) & ACTOR_FLAG_ACTIVE) == 0)
            break;
        index++;
        if (index > 0x27)
            return 0xFF;
    }

    zero = 0;
    CpuSetRom(&zero, actor, ACTOR_CLEAR_CONTROL);

    actor[2] = (u8)(flagBits & 0xF);
    actor[3] = (u8)(flagBits & 0xF);
    actor[ACTOR_OFF_KIND] = 0x12;
    *(u16 *)(actor + ACTOR_OFF_FLAGS) = (u16)(flagBits & 0xFFF0);

    if (typeId != 0)
    {
        anm = LookupAnm(typeId);
        if (AllocAnmSlot(anm[2]) == 0)
            return 0xFF;

        i = 0;
        while (i < gAnmTypeCount)
        {
            if (gAnmTypeIds[i] == typeId)
                break;
            i++;
        }

        if (i < gAnmTypeCount)
        {
            flags = *(u16 *)(actor + ACTOR_OFF_FLAGS);
            *(u16 *)(actor + ACTOR_OFF_FLAGS) = (u16)(flags | 4);
            tileId = gAnmTypeAux[i];
        }
        else
        {
            tileId = AllocTiles(*(u16 *)(anm + 4));
            if (tileId == TILE_ALLOC_FAIL)
                return 0xFF;
        }

        actor[13] = (u8)*(u16 *)(anm + 2);
        actor[6] = (u8)*(u16 *)(anm + 2);
        *(u16 *)(actor + 0x24) = *(u16 *)(anm + 4);
        *(u16 *)(actor + 0x26) = tileId;
    }

    activeBit = 0;
    if (gCutsceneCounter == 0)
        activeBit = 1;
    actor[0] = activeBit;
    actor[ACTOR_OFF_CAMERA] = camera;

    flags = *(u16 *)(actor + ACTOR_OFF_FLAGS);
    *(u16 *)(actor + ACTOR_OFF_FLAGS) = (u16)(flags | SPAWN_FLAG_OR);

    *(u16 *)(actor + 0x22) = anim;
    *(u16 *)(actor + ACTOR_OFF_ANIM) = typeId;

    posX = (s32)x << 16;
    posY = (s32)y << 16;
    *(s32 *)(actor + ACTOR_OFF_X) = posX;
    *(s32 *)(actor + ACTOR_OFF_Y) = posY;
    *(s32 *)(actor + 0x44) = posX;
    *(s32 *)(actor + 0x50) = posY;

    actor[8] = 1;
    *(u16 *)(actor + 0x1E) = field1e;
    *(u16 *)(actor + 0x2A) = 0xFFFF;
    *(u16 *)(actor + 0x1A) = 0x100;
    *(u16 *)(actor + 0x1C) = 0x100;

    return index;
}

void DeleteActor(u8 index)
{
    u8 *actor;
    u16 flags;
    u16 field1e;
    u8 camera;
    u16 *maskSlot;

    if (index < gActorAllocIndex)
    {
        actor = &gActorPool[(u32)index * ACTOR_STRIDE];
        *(u16 *)(actor + ACTOR_OFF_FLAGS) = 2;
        return;
    }

    actor = &gActorPool[(u32)index * ACTOR_STRIDE];
    FreeAnmSlot(actor[13]);

    flags = *(u16 *)(actor + ACTOR_OFF_FLAGS);
    if ((flags & 4) == 0)
        FreeTiles(*(u16 *)(actor + 0x26), *(u16 *)(actor + 0x24));

    field1e = *(u16 *)(actor + 0x1E);
    if (field1e != FIELD1E_NONE)
    {
        camera = actor[ACTOR_OFF_CAMERA];
        /* *(u16 **)(gCameras + cam * 0x84 + 0x6C) is a halfword mask table. */
        maskSlot = *(u16 **)(gCameras + (u32)camera * CAMERA_STRIDE + 0x6C);
        maskSlot[field1e] &= 0x7FFF;
    }

    *(u16 *)(actor + ACTOR_OFF_FLAGS) = 0;
}

void MoveActor(u8 index, u16 x, u16 y)
{
    u8 *actor = &gActorPool[(u32)index * ACTOR_STRIDE];
    s32 posX = (s32)x << 16;
    s32 posY = (s32)y << 16;

    *(s32 *)(actor + ACTOR_OFF_X) = posX;
    *(s32 *)(actor + ACTOR_OFF_Y) = posY;
    *(s32 *)(actor + 0x44) = posX;
    *(s32 *)(actor + 0x50) = posY;
}

void ShowActor(u8 index, u16 x, u16 y)
{
    u8 *actor = &gActorPool[(u32)index * ACTOR_STRIDE];

    MoveActor(index, x, y);
    *(u16 *)(actor + ACTOR_OFF_FLAGS) |= ACTOR_FLAG_ACTIVE;
}

void HideActor(u8 index)
{
    u8 *actor = &gActorPool[(u32)index * ACTOR_STRIDE];

    *(u16 *)(actor + ACTOR_OFF_FLAGS) &= (u16)~ACTOR_FLAG_ACTIVE;
}

void SetActorVelX(u8 index, s32 vx)
{
    u8 *actor = &gActorPool[(u32)index * ACTOR_STRIDE];

    *(s32 *)(actor + ACTOR_OFF_VX) = vx;
}

void SetActorVelY(u8 index, s32 vy)
{
    u8 *actor = &gActorPool[(u32)index * ACTOR_STRIDE];

    *(s32 *)(actor + ACTOR_OFF_VY) = vy;
}

void WalkActor(u8 index, u16 x, u16 y, s32 speed)
{
    u8 *actor = &gActorPool[(u32)index * ACTOR_STRIDE];
    s32 curX = *(s32 *)(actor + ACTOR_OFF_X);
    s32 curY = *(s32 *)(actor + ACTOR_OFF_Y);
    s32 dx = ((s32)x << 16) - curX;
    s32 dy = ((s32)y << 16) - curY;
    s32 vx = 0;
    s32 vy = 0;

    if (speed < 0)
        speed = -speed;

    if (dx > 0)
        vx = speed;
    else if (dx < 0)
        vx = -speed;

    if (dy > 0)
        vy = speed;
    else if (dy < 0)
        vy = -speed;

    /* Axis-dominant when both nonzero — prefer the longer delta. */
    if (dx != 0 && dy != 0)
    {
        if ((dx < 0 ? -dx : dx) >= (dy < 0 ? -dy : dy))
            vy = 0;
        else
            vx = 0;
    }

    SetActorVelX(index, vx);
    SetActorVelY(index, vy);
}

void WalkActorDir(u8 index, u8 dir, s32 speed)
{
    s32 vx = 0;
    s32 vy = 0;

    switch (dir & 3)
    {
    case ACTOR_DIR_RIGHT:
        vx = speed;
        break;
    case ACTOR_DIR_DOWN:
        vy = speed;
        break;
    case ACTOR_DIR_LEFT:
        vx = -speed;
        break;
    case ACTOR_DIR_UP:
        vy = -speed;
        break;
    }

    SetActorVelX(index, vx);
    SetActorVelY(index, vy);
}
