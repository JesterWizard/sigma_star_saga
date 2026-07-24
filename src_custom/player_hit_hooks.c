#include "global.h"
#include "runtime.h"
#include "ram_map.h"
#include "suction.h"
#include "nocash.h"

/*
 * PlayerHitUpdate @ 0x08024E24 — LynJump replacement (ygodm8 16-byte stub).
 * PlayerDeathFx @ 0x0801BA4C — common explosion/game-over helper used by stage
 * handlers when HP hits 0 (often without ever writing state 0x2F).
 */

void PlayerHitUpdate__Continue(void);
void PlayerStateMachine__Continue(void);
void PlayerDeathFx__Continue(void);
void DeleteActor__Continue(u32 actor);
u32 PhoenixDeathFxSkipReturn(u32 lr);

APPEND_TEXT void PlayerHitUpdate__Replacement(void)
{
    /* Stage handlers run earlier this frame; catch HP==0 they left. */
    ApplyPhoenixRevive();
    PlayerHitUpdate__Continue();
    ApplyPhoenixRevive();
}

APPEND_TEXT void PlayerStateMachine__Replacement(void)
{
    ApplyPhoenixRevive();
    PlayerStateMachine__Continue();
    ApplyPhoenixRevive();
}

APPEND_TEXT u32 PhoenixDeathFxSkipReturn(u32 lr)
{
    switch (lr)
    {
    case 0x080215DD:
        NoCashGBAPrint("PHX skip tail: 215DD");
        return 0x0802160D;
    case 0x08022737:
        NoCashGBAPrint("PHX skip tail: 22737");
        return 0x08022767;
    case 0x08023225:
        NoCashGBAPrint("PHX skip tail: 23225");
        return 0x080232AF;
    case 0x0802367B:
        NoCashGBAPrint("PHX skip tail: 2367B");
        return 0x080236A3;
    case 0x08023A89:
        NoCashGBAPrint("PHX skip tail: 23A89");
        return 0x08023B77;
    case 0x08023E99:
        NoCashGBAPrint("PHX skip tail: 23E99");
        return 0x08023EC1;
    case 0x08024105:
        NoCashGBAPrint("PHX skip tail: 24105");
        return 0x0802412D;
    case 0x08024631:
        NoCashGBAPrint("PHX skip tail: 24631");
        return 0x080246A7;
    case 0x08024927:
        NoCashGBAPrint("PHX skip tail: 24927");
        return 0x0802494F;
    case 0x08024D9D:
        NoCashGBAPrint("PHX skip tail: 24D9D");
        return 0x08024E15;
    case 0x080251D9:
        NoCashGBAPrint("PHX skip tail: 251D9");
        return 0x08025243;
    case 0x0802535D:
        NoCashGBAPrint("PHX skip tail: 2535D");
        return 0x08025385;
    default:
        NoCashGBAPrint("PHX skip tail: unknown LR");
        return lr;
    }
}

/* Pass-through: ship deletion is skipped via PhoenixDeathFxSkipReturn remaps.
 * Never suppress arbitrary actors — that softlocks overworld transitions. */
APPEND_TEXT void DeleteActor__Replacement(u32 actor)
{
    DeleteActor__Continue(actor);
}
