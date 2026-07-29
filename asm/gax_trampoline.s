@ Resume AgbMain after custom GAX boot (stolen span 0x080038D8..0x080038F9).
@ Vanilla next insn @ 0x080038FA: strh r5, [r0] (IE clear path).
@
@ Speech consumer probe: 8-byte veneer at 0x08056A30 steals the prologue;
@ replay it here and continue at 0x08056A38 (push {r5,r6,r7}).

	.syntax unified
	.text
	.thumb

	.global GaxBootInit__Continue
	.type GaxBootInit__Continue, %function
	.thumb_func
GaxBootInit__Continue:
	ldr	r3, =0x080038FB
	bx	r3
	.pool

	.global GaxSpeechConsumer__Continue
	.type GaxSpeechConsumer__Continue, %function
	.thumb_func
GaxSpeechConsumer__Continue:
	push	{r4, r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, sb
	mov	r5, r8
	ldr	r3, =0x08056A39
	bx	r3
	.pool
