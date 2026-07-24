@ DrawActors @ 0x08006C0C — 8-byte veneer; resume at push {r5-r7} (0x08006C14).
	.section .append_text, "ax", %progbits
	.thumb
	.align 2

	.global DrawActors__Continue
	.thumb_func
DrawActors__Continue:
	push {r4-r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	ldr r3, =0x08006C15
	bx r3
	.pool
