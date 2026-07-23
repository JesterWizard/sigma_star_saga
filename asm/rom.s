@ Unmatched ROM image head (before peeled level-up core).
@ As functions/data are decompiled, shrink this .incbin and move ranges into
@ real asm/C objects listed in ld_script.ld.

	.section .rodata
	.align 2
	.global gBaserom
gBaserom:
	.incbin "baserom.gba", 0x0, 0xFD84
