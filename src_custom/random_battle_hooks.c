#include "global.h"
#include "runtime.h"
#include "ram_map.h"
#include "overworld_encounters.h"

/*
 * Disable / gate overworld step random battles (TryStartRandomBattle @ 0x0801DA5C).
 *
 * Debug-menu gate: gDebugMenuToggleRandomBattlesOff in EWRAM (never touches vanilla
 * gRandomBattleCooldown). Seeded from .disable_random_battles.
 *
 * Vanilla encounter flow (same walk pass, in order):
 *   1F030  trap when gOwEncActive == 0  → player state 0x39
 *   1DBB8  consume armed prep → battle transition
 *   1DA5C  roll new prep for a later pass
 *
 * Prep must survive from 1DA5C until 1DBB8 runs. If the transition fails we are
 * still on the overworld field with armed / latch / 0x36 / 0x39 — clear then.
 * State 0x39 alone is cleared before the walk trap on the next frame.
 */

#define MODE_JT_BASE 0x0800BC04
#define OVERWORLD_FRAME_ADDR 0x0800D610

#define OW_ENC_ACTIVE  ((vu8 *)0x030076E0)
#define OW_ENC_STEP    ((vu8 *)0x030076B0)
#define OW_ENC_ARMED   ((vu8 *)0x03007688)

/*
 * Engine-owned, deliberately NOT written here (see RandomBattle_RestoreWalk):
 *   0x03007680  encounter/arena selector, chosen from gMode by
 *               TryStartRandomBattle @ 0x1DB20-0x1DB8E and consumed by the
 *               battle setup @ 0x1234A. 0 is FIRE's value, not "none".
 *   0x030076C0  encounter slot, written alongside it @ 0x1DB1A
 *   0x03007670  pending-encounter scratch
 */

#define OW_PLAYER_STATE_ENCOUNTER 0x36
#define OW_PLAYER_STATE_TRAP      0x39
#define OW_PLAYER_FLAG_ENCOUNTER    0x802

void TryStartRandomBattle__Continue(void);
void OwEncFollowUp__Continue(void);

/* Walkable field modes only (see include/overworld_frame.h). The JT check on
 * its own matches 129 modes — the save/load screen (0x8F) and the flight stage
 * (0x84) among them — and the recovery writes below must never fire there. */
#define FIELD_MODE_LO_FIRST 0x04
#define FIELD_MODE_LO_LAST 0x09
#define FIELD_MODE_HI_FIRST 0x0F
#define FIELD_MODE_HI_LAST 0x17

static bool8 RandomBattle_IsOverworldField(void)
{
    u8 mode = gMode;
    const u32 *jt;

    if (mode == 0)
        return FALSE;

    if (!((mode >= FIELD_MODE_LO_FIRST && mode <= FIELD_MODE_LO_LAST)
          || (mode >= FIELD_MODE_HI_FIRST && mode <= FIELD_MODE_HI_LAST)))
        return FALSE;

    jt = (const u32 *)MODE_JT_BASE;
    return jt[mode - 1] == OVERWORLD_FRAME_ADDR;
}

/*
 * Undo a *failed* encounter transition and hand the walk back.
 *
 * Only the latch/arm state belongs to us. In particular OW_ENC_MODE
 * (0x03007680) must be left alone: TryStartRandomBattle @ 0x1DB20-0x1DB8E
 * selects it from gMode (4->1, 5->0, 6->4, 7->3, 17->5, else 0) and the battle
 * setup @ 0x1234A dispatches on it to pick the encounter/arena set. Forcing it
 * to 0 does not "clear" it — 0 is a *valid* selector (FIRE's) — so every later
 * battle loaded the wrong planet's set, which is why the Startbase-1 intro
 * sequence replayed over and over after toggling random battles from the debug
 * menu. OW_ENC_SLOT / OW_ENC_PENDING are written by the same engine paths and
 * are likewise not ours to reset.
 */
static void RandomBattle_RestoreWalk(void)
{
    u8 *player = gPlayerPtr;

    gTalkUiLatch = 0;
    *OW_ENC_ARMED = 0;
    *OW_ENC_STEP = 1;
    *OW_ENC_ACTIVE = 1;

    if (player == NULL)
        return;

    *(u32 *)(player + 0x48) = 0;
    *(u32 *)(player + 0x54) = 0;
    *(u16 *)(player + 0x18) &= (u16)~OW_PLAYER_FLAG_ENCOUNTER;

    if (player[8] == OW_PLAYER_STATE_ENCOUNTER
        || player[8] == OW_PLAYER_STATE_TRAP)
        player[8] = 7;
}

static bool8 RandomBattle_IsTrapStuck(void)
{
    u8 *player = gPlayerPtr;

    if (player != NULL && player[8] == OW_PLAYER_STATE_TRAP)
        return TRUE;
    return FALSE;
}

static bool8 RandomBattle_FailedTransition(void)
{
    u8 *player = gPlayerPtr;

    if (!RandomBattle_IsOverworldField())
        return FALSE;

    if (*OW_ENC_ARMED != 0 || gTalkUiLatch != 0)
        return TRUE;

    if (player == NULL)
        return FALSE;

    if (player[8] == OW_PLAYER_STATE_ENCOUNTER)
        return TRUE;
    if ((*(u16 *)(player + 0x18) & OW_PLAYER_FLAG_ENCOUNTER) != 0)
        return TRUE;

    return FALSE;
}

static void RandomBattle_ClearFailedTransition(void)
{
    if (!RandomBattle_IsOverworldField())
        return;
    if (!RandomBattle_FailedTransition() && !RandomBattle_IsTrapStuck())
        return;
    RandomBattle_RestoreWalk();
}

void RandomBattle_EnsureInit(void)
{
    if (gRandomBattleInit)
        return;
    gRandomBattleInit = 1;
    if (gRuntimeConfig.disable_random_battles)
        gDebugMenuToggleRandomBattlesOff = 1;
}

bool8 RandomBattlesDisabled(void)
{
    RandomBattle_EnsureInit();
    return gDebugMenuToggleRandomBattlesOff != 0;
}

void RandomBattlesSetDisabled(bool8 disabled)
{
    RandomBattle_EnsureInit();
    gDebugMenuToggleRandomBattlesOff = disabled ? 1 : 0;
    RandomBattle_RestoreWalk();
}

void RandomBattle_RecoverTrapBeforeWalk(void)
{
    if (RandomBattlesDisabled())
        return;
    if (!RandomBattle_IsOverworldField())
        return;
    if (!RandomBattle_IsTrapStuck())
        return;
    RandomBattle_RestoreWalk();
}

void RandomBattle_RecoverOverworldIfStuck(void)
{
    if (RandomBattlesDisabled())
        return;
    RandomBattle_ClearFailedTransition();
}

APPEND_TEXT void TryStartRandomBattle__Replacement(void)
{
    RandomBattle_EnsureInit();
    if (gDebugMenuToggleRandomBattlesOff != 0)
        return;

    /* Leave prep latched for OwEncFollowUp @ 0x1DBB8 later this frame / next. */
    TryStartRandomBattle__Continue();
}

APPEND_TEXT void OwEncFollowUp__Replacement(void)
{
    RandomBattle_EnsureInit();
    if (gDebugMenuToggleRandomBattlesOff != 0)
    {
        OwEncFollowUp__Continue();
        return;
    }

    OwEncFollowUp__Continue();
    RandomBattle_ClearFailedTransition();
}
