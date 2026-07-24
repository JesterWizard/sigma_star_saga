@ Trampolines for Suction gem-update veneers (stolen 8-byte prologues).

	.syntax unified
	.text
	.thumb
	.align 2

@ ExpGemUpdate @ 0x0804ABEC — continue at 0x0804ABF4
	.global ExpGemUpdate__Continue
	.thumb_func
ExpGemUpdate__Continue:
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r3, =0x0804ABF5
	bx r3
	.pool

@ LeechGemUpdate @ 0x0804B188 — reissue bl 0x080050B0, continue at 0x0804B190
	.global LeechGemUpdate__Continue
	.thumb_func
LeechGemUpdate__Continue:
	push {r4, r5, r6, r7, lr}
	movs r7, #0
	ldr r3, =0x080050B1
	bl _suction_blx_r3
	ldr r3, =0x0804B191
	bx r3
	.pool

	.thumb_func
_suction_blx_r3:
	bx r3

@ GetGunDataIconFrame @ 0x08039F30 — 16-byte icon veneer; resume at 0x08039F40
	.global GetGunDataIconFrame__Continue
	.thumb_func
GetGunDataIconFrame__Continue:
	push {r4, lr}
	adds r4, r3, #0
	lsls r0, r0, #24
	lsrs r0, r0, #24
	adds r3, r0, #0
	lsls r1, r1, #24
	lsrs r1, r1, #24
	lsls r2, r2, #24
	push {r0, r1}
	ldr r0, =0x08039F41
	mov r12, r0
	pop {r0, r1}
	bx r12
	.pool

@ IsGunDataOwned @ 0x08039C4C — continue at 0x08039C54
	.global IsGunDataOwned__Continue
	.thumb_func
IsGunDataOwned__Continue:
	push {lr}
	lsls r0, r0, #24
	lsrs r0, r0, #24
	lsls r1, r1, #24
	ldr r3, =0x08039C55
	bx r3
	.pool

@ PlayerStateMachine @ 0x08025DB4 — 16-byte LynJump stub; resume at 0x08025DC4
	.global PlayerStateMachine__Continue
	.thumb_func
PlayerStateMachine__Continue:
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r0, =0x03000DB8
	ldr r0, [r0]
	ldr r3, =0x08025DC5
	bx r3
	.pool

@ PlayerHitUpdate @ 0x08024E24 — 16-byte LynJump stub; resume at 0x08024E34
	.global PlayerHitUpdate__Continue
	.thumb_func
PlayerHitUpdate__Continue:
	push {r4, r5, r6, r7, lr}
	sub sp, #16
	ldr r0, =0x03000DB8
	ldr r1, [r0]
	ldrb r1, [r1, #8]
	subs r1, #1
	adds r5, r0, #0
	cmp r1, #0x2F
	ldr r3, =0x08024E35
	bx r3
	.pool

@ PlayerDeathFx @ 0x0801BA4C — 8-byte veneer; reissue prologue, resume at 0x0801BA54
	.global PlayerDeathFx__Continue
	.thumb_func
PlayerDeathFx__Continue:
	push {lr}
	sub sp, #16
	ldr r3, =0x08007349
	bl _suction_blx_r3
	ldr r3, =0x0801BA55
	bx r3
	.pool

@ DeleteActor @ 0x08006310 — 8-byte veneer; resume at 0x08006318.
	.global DeleteActor__Continue
	.thumb_func
DeleteActor__Continue:
	push {r4, lr}
	lsls r0, r0, #24
	lsrs r2, r0, #24
	ldr r0, =0x0300368C
	ldr r3, =0x08006319
	bx r3
	.pool

	.section .append_text, "ax", %progbits
	.thumb
	.align 2

@ PlayerDeathFx replacement: revive, then skip the caller's death tail
@ (which includes DeleteActor for every known caller).
	.global PlayerDeathFx__Replacement
	.thumb_func
PlayerDeathFx__Replacement:
	push {lr}
	bl DoPhoenixRevive
	cmp r0, #0
	bne 1f
	bl PlayerDeathFx__Continue
	pop {r0}
	bx r0
1:
	pop {r0}
	ldr r3, =PhoenixDeathFxSkipReturn
	bl _suction_blx_r3
	bx r0
	.pool
