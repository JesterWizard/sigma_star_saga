#include "global.h"
#include "runtime.h"
#include "ram_map.h"
#include "level_up.h"

/*
 * Status overlay LEVEL digits @ 0x080542B6 (SELECT status panel only).
 * Vanilla only places tens @ x=0xAD and ones @ x=0xB5 (sprites 0x7E / 0x7F),
 * so display values >= 100 show as a single visible ones digit against the
 * right-hand yellow chrome. With level_cap_255, draw hundreds @ x=0xA5
 * (sprite 0x7D) like ATK/DEF — the digit cells cover the yellow.
 *
 * Does not touch the top flight HUD (HP / EXP / bombs bar).
 *
 * Display value matches vanilla: gPlayerLevel + 1 (start 0 → "1", max 99 → "100").
 */

typedef void (*DrawHudSpriteFn)(
    u32 id, u32 digit, u32 r2, u32 r3,
    u32 s0, u32 s1, u32 s2, u32 s3, u32 s4, u32 s5,
    u32 x, u32 y);

#define DRAW_HUD_SPRITE ((DrawHudSpriteFn)0x08006DF1)

#define LEVEL_DIGIT_Y      0x20
#define LEVEL_HUNDREDS_X   0xA5
#define LEVEL_TENS_X       0xAD
#define LEVEL_ONES_X       0xB5
#define LEVEL_SPRITE_HUNDREDS 0x7D
#define LEVEL_SPRITE_TENS     0x7E
#define LEVEL_SPRITE_ONES     0x7F

#define STATUS_LEVEL_CONTINUE 0x08054315

static void DrawLevelDigit(u32 spriteId, u32 digit, u32 x)
{
    DRAW_HUD_SPRITE(
        spriteId, digit, 0, 0,
        0, 0, 0, 0, 0, 0,
        x, LEVEL_DIGIT_Y);
}

APPEND_TEXT static void DrawStatusLevelDigits_Body(void)
{
    u32 disp = (u32)gPlayerLevel + 1;
    u32 hundreds;
    u32 tens;
    u32 ones;

    if (gRuntimeConfig.level_cap_255)
    {
        hundreds = disp / 100;
        tens = (disp / 10) % 10;
        ones = disp % 10;
        DrawLevelDigit(LEVEL_SPRITE_HUNDREDS, hundreds, LEVEL_HUNDREDS_X);
        DrawLevelDigit(LEVEL_SPRITE_TENS, tens, LEVEL_TENS_X);
        DrawLevelDigit(LEVEL_SPRITE_ONES, ones, LEVEL_ONES_X);
    }
    else
    {
        /* Vanilla: two digits only (breaks for display >= 100). */
        tens = (disp / 10) % 10;
        ones = disp % 10;
        DrawLevelDigit(LEVEL_SPRITE_TENS, tens, LEVEL_TENS_X);
        DrawLevelDigit(LEVEL_SPRITE_ONES, ones, LEVEL_ONES_X);
    }
}

/* Veneered over 0x542B6 (Thumb ldr/bx; site is 2-mod-4). Branch to epilogue. */
APPEND_TEXT __attribute__((naked)) void DrawStatusLevelDigits__Replacement(void)
{
    /* Thumb cannot pop lr; discard the saved lr with add sp,#4 so the status
     * function's add sp,#32 still lines up. */
    __asm__ volatile(
        "push {r4, r5, r6, r7, lr}\n"
        "bl DrawStatusLevelDigits_Body\n"
        "pop {r4, r5, r6, r7}\n"
        "add sp, #4\n"
        "ldr r0, =%0\n"
        "bx r0\n"
        :
        : "i"(STATUS_LEVEL_CONTINUE)
        : "memory");
}
