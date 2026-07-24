@ ScanEncounters @ 0x080146B4 — 8-byte veneer; resume at push {r5-r7} (0x080146BC).
	.section .append_text, "ax", %progbits
	.thumb
	.align 2

	.global ScanEncounters__Continue
	.thumb_func
ScanEncounters__Continue:
	push {r4-r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	ldr r3, =0x080146BD
	bx r3
	.pool
