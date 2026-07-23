@ Replaces UpdateShooterDebugInput at ROM 0x1749C (exactly 0x5E bytes).
@ Select / L / Start (raw KEYINPUT) → set gStageClearFlag and call TriggerStageClear.
@ .align 2 only via .short pad — do not use .align 2 on the section.

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
	ldr	r0, .Lmask
	ands	r0, r1
	beq	.Ldone
	ldr	r0, .Lflag
	ldrb	r1, [r0]
	cmp	r1, #0
	bne	.Ldone
	@ Set clear flag first (same as TriggerStageClear prologue)
	movs	r1, #1
	strb	r1, [r0]
	@ Also invoke TriggerStageClear with empty event {0,0}
	movs	r1, #0
	push	{r1}
	push	{r1}
	mov	r1, sp
	movs	r0, #0
	@ bl 0x08017FA4 from linked site 0x080174C0
	.short	0xF000
	.short	0xFD70
	add	sp, #8
.Ldone:
	pop	{r0}
	bx	r0
	.short	0
.Lkeyinput:
	.word	0x04000130
.Lmask:
	.word	0x020C			@ SELECT | L | START
.Lflag:
	.word	gStageClearFlag
	.fill	0x5E - (. - UpdateShooterDebugInput), 1, 0
