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
