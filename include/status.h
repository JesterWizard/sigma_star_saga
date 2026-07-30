#ifndef GUARD_STATUS_H
#define GUARD_STATUS_H

#include "gba/types.h"

/*
 * Vanilla SELECT status/map path (not yet peeled into src/).
 *
 * StatusToggle @ 0x08019094 — SELECT edge; QueueMode(0x8F) open / 0x8E close
 * StatusPanel  @ 0x08053BDC — mode 0x8F body; SetMode(0x168) + UI actors
 * LeaveStatus  @ 0x0803AB40 — mode 0x8E cleanup; SetMode(0xE8)
 * SetMode      @ 0x080079F0 — wipe cameras, load mode BG/GFX, write gSoftDispCnt
 *
 * Soft display mirrors (VBlank @ 0x080039B8 copies to IO):
 *   gSoftDispCnt @ 0x03000BF4 → REG_DISPCNT
 *   gSoftBg2Pa   @ 0x03001E7C → REG_BG2PA (affine batch, gated on 0x03000C4C)
 *   gSoftBg2Pb   @ 0x03001F6C → REG_BG2PB
 *
 * BGxCNT has no soft mirror: the camera layer setup @ 0x080077D2 writes
 * 0x04000008 + layer*2 directly. The status/HUD module separately re-applies
 * gDisplayCtrlMirror @ 0x03007194 to REG_DISPCNT every frame, which overrides
 * gSoftDispCnt and decides which layers are actually visible.
 */

typedef void (*SetModeFn)(u32 modeId, u32 flag);
typedef void (*VoidFn)(void);
typedef void (*VoidU32Fn)(u32 a);
typedef void (*FadeSpeedFn)(u32 speed);

#define SetMode ((SetModeFn)0x080079F1)
#define SetModeCallback ((VoidU32Fn)0x0800379D)
#define LeaveStatusRestore ((VoidFn)0x0803AB41)
#define FadeStart ((FadeSpeedFn)0x080041B5) /* positive speed — fade to black */
#define FadeOut ((FadeSpeedFn)0x08004191)   /* negated speed — fade from black */
#define FadeStep ((VoidFn)0x080042E1)
#define VBlankIntrWait ((VoidFn)0x08054475)

#define STATUS_GFX_MODE 0x168
#define FIELD_RESUME_MODE 0xE8

/* Soft fade speeds written by FadeStart / FadeOut (StatusToggle path). */
#define FADE_SPEED_A (*(vu16 *)0x03000CE8)
#define FADE_SPEED_B (*(vu16 *)0x03000CC0)

/* Fixed-point 1.0 written into camera scroll slots by StatusPanel / LeaveStatus. */
#define CAMERA_SCROLL_ONE 0x10000

#endif /* GUARD_STATUS_H */
