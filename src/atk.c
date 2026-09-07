#include "global.h"
#include "atk.h"
#include "ram_map.h"

/*
 * CalcAtk — decompiled from baserom @ 0x080304D0 (span 0x88, ends at the
 * literal pool before the next helper at 0x0803055C).
 *
 * Status-screen ATK: gPlayerLevel plus a small bonus from the equipped
 * bullet type. Vanilla-only; Training Weights halves the result in
 * src_custom via LynJump (CalcAtk__Replacement).
 *
 * Linked into append (.text). The 8-byte veneer at 0x080304D0 jumps to
 * the replacement, which calls this copy so the veneer is not re-entered.
 */

u32 CalcAtk(void)
{
    u32 bonus;

    switch (gGunLoadoutBullet)
    {
    case 9:
    case 14:
        bonus = 4;
        break;
    case 17:
        bonus = 2;
        break;
    case 18:
        bonus = 6;
        break;
    case 19:
        bonus = 11;
        break;
    default:
        bonus = 1;
        break;
    }

    return gPlayerLevel + bonus;
}
