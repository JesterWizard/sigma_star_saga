@ Unmatched ROM between level_up and the shooter debug input function.

	.section .rodata
	.global gBaseromAfterA
gBaseromAfterA:
	.incbin "baserom.gba", 0xFE20, 0x1749C - 0xFE20
