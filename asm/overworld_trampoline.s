@ OverworldPlayerUpdate @ 0x0801DC84 — 8-byte veneer; resume at movs r0,#0 path.
	.section .append_text, "ax", %progbits
	.thumb
	.align 2

	.global OverworldPlayerUpdate__Continue
	.thumb_func
OverworldPlayerUpdate__Continue:
	push {r4-r7, lr}
	ldr r3, =0x08001539
	bl _ow_blx_r3
	movs r0, #0
	ldr r3, =0x0801DC8D
	bx r3
	.pool

	.thumb_func
_ow_blx_r3:
	bx r3
