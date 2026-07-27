@ Resume AgbMain after custom GAX boot (stolen span 0x080038D8..0x080038F9).
@ Vanilla next insn @ 0x080038FA: strh r5, [r0] (IE clear path).

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
