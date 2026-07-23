@ Replaces UpdateShooterDebugInput at ROM 0x1749C (exactly 0x5E bytes).
@ SELECT + L (both, raw KEYINPUT) → force stage clear.
@ Sets gStageClearFlag, zeros gEnemyRemaining, and clears gStageClearGate
@ (0x03007730 — same write as the Code Breaker "Clear Stage" cheat).
@ Do NOT call TriggerStageClear: a fake event pointer corrupts story flags.

	.syntax unified
	.section .text, "ax", %progbits
	.thumb
	.global UpdateShooterDebugInput
	.thumb_func
UpdateShooterDebugInput:
	push	{lr}
	@ Raw KEYINPUT is active-low
	ldr	r0, .Lkeyinput
	ldrh	r1, [r0]
	mvns	r1, r1
	ldr	r0, .Lmask		@ SELECT | L
	ands	r1, r0
	cmp	r1, r0			@ both must be held
	bne	.Ldone
	ldr	r0, .Lflag
	ldrb	r1, [r0]
	cmp	r1, #0
	bne	.Ldone
	movs	r1, #1
	strb	r1, [r0]		@ gStageClearFlag = 1
	movs	r1, #0
	ldr	r0, .Lremain
	strb	r1, [r0]		@ gEnemyRemaining = 0
	ldr	r0, .Lgate
	strb	r1, [r0]		@ gStageClearGate = 0
.Ldone:
	pop	{r0}
	bx	r0
	.short	0
.Lkeyinput:
	.word	0x04000130
.Lmask:
	.word	0x0204			@ SELECT | L
.Lflag:
	.word	gStageClearFlag
.Lremain:
	.word	gEnemyRemaining
.Lgate:
	.word	gStageClearGate
	.fill	0x5E - (. - UpdateShooterDebugInput), 1, 0
