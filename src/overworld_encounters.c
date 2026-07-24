#include "global.h"
#include "overworld_encounters.h"
#include "ram_map.h"

/*
 * ScanEncounters — decompiled from baserom @ 0x080146B4 (span 0x160).
 *
 * Linked into append ROM (not peeled). This C is the readable reference for
 * lure-circle / map-object encounters (not step RNG random battles).
 *
 * Map objects are 16-byte records: hitbox at +0..+6, flags at +8, trigger
 * mode at +10, battle ID at +12. Walk-into lure battles are mode 0/1.
 */

typedef u8 (*CheckNearbyActorsFunc)(u8 a, u8 b);
typedef u8 (*RectsOverlapFunc)(const u16 *a, const u16 *b);
typedef u8 (*PointInObjFunc)(u16 x, u16 y, const u16 *obj);
typedef u8 (*TryStartBattleFunc)(u16 battleId);

#define CheckNearbyActors ((CheckNearbyActorsFunc)0x08014599)
#define RectsOverlap ((RectsOverlapFunc)0x0800582D)
#define PointInMapObj ((PointInObjFunc)0x0800585D)
#define TryStartBattleRom ((TryStartBattleFunc)0x08014829)

#define MAP_OBJ_STRIDE 16
#define BATTLE_ID_MIN 0x3E7 /* only IDs above 999 start battles */

u8 TryStartBattle(u16 battleId)
{
    return TryStartBattleRom(battleId);
}

/*
 * TryStartRandomBattle — baserom @ 0x0801DA5C.
 * Only caller: overworld walk @ 0x1F0C2. Gated by gRandomBattleCooldown;
 * when the word is non-zero the routine decrements and returns. Disable
 * patch: in-place `bx lr` via apply_lynjump when .disable_random_battles.
 */
void TryStartRandomBattle(void)
{
    ((void (*)(void))0x0801DA5D)();
}

u8 ScanEncounters(u8 mode)
{
    u8 started;
    u8 nearbyHit;
    u8 i;
    u8 useRectOverlap;
    u16 playerX;
    u16 playerY;
    u16 *playerObj;
    u8 *player;
    u16 *obj;
    u16 battleId;
    u16 mode0BattleId;
    u8 idx;

    started = 0;
    nearbyHit = 0;
    mode0BattleId = 0;

    if (mode == 2 && (gHeldKeys & 1) != 0)
    {
        /* Non-zero if CheckNearbyActors returned anything but 0xFF. */
        if ((u8)(CheckNearbyActors(1, 2) ^ 0xFF) != 0)
            nearbyHit = 1;
    }

    playerObj = (u16 *)(gMapObjBasePtr + (u32)gPlayerMapObjIndex * MAP_OBJ_STRIDE);
    player = gPlayerPtr;
    playerX = (u16)(*(u32 *)(player + 0x40) >> 16);
    playerY = (u16)(*(u32 *)(player + 0x4C) >> 16);

    /* Player states 1–2 use rectangle overlap vs the player map object. */
    useRectOverlap = 0;
    idx = player[8] - 1;
    if (idx <= 1)
        useRectOverlap = 1;

    for (i = 0; i < gEncounterObjCount; i++)
    {
        obj = (u16 *)(gMapObjBasePtr
                      + (u32)gEncounterObjIndices[i] * MAP_OBJ_STRIDE);

        if (obj[4] != 0) /* flags @ +8 */
            continue;
        if (obj[5] != mode) /* trigger mode @ +10 */
            continue;

        battleId = obj[6]; /* @ +12 */
        if (battleId <= BATTLE_ID_MIN)
            continue;

        if (useRectOverlap)
        {
            if (RectsOverlap(obj, playerObj))
                goto hit;
        }

        if (!PointInMapObj(playerX, playerY, obj))
            continue;

    hit:
        if (mode == 1)
        {
            TryStartBattle(battleId);
            started = 1;
        }
        else if (mode == 0)
        {
            mode0BattleId = battleId;
            if (gLastEncounterBattleId != battleId)
            {
                gLastEncounterBattleId = battleId;
                TryStartBattle(battleId);
                started = 1;
            }
        }
        else if (mode == 2)
        {
            if ((gHeldKeys & 1) != 0)
            {
                TryStartBattle(battleId);
                started = 1;
            }
        }
    }

    /* Mode 0 clears the latch when no zone matched (r7 stayed 0). */
    if (mode == 0)
        gLastEncounterBattleId = mode0BattleId;

    if (started || nearbyHit)
        return 1;
    return 0;
}
