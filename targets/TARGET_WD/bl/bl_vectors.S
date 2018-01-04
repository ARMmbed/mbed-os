/*
 * bl_vectors.S
 *
 * Created: 02.11.2017 21:21:47
 *  Author: Guenter.Prossliner
 */ 

#include "bl_asmoptions.inc"

#include "WD_ABI.h"

.section  .bl_vectors,"a",%progbits
.type  g_bl_vectors, %object

g_bl_vectors:

	.word 0x20001000		// SP for bootloader
	.word bl_start	// reset vector
	.word bl_Other_Handler
	.word bl_HardFault_Handler
	.word bl_Other_Handler
	.word bl_Other_Handler
	.word bl_Other_Handler
	.word 0
	.word 0
	.word 0
	.word 0
	.word bl_Other_Handler
	.word bl_Other_Handler
	.word 0
	.word bl_Other_Handler
	.word bl_Other_Handler
	.word 0
	.word 0
	.word 0
	.word 0
	.word bl_flash_handler

.size  g_bl_vectors, .-g_bl_vectors


.section .bl_text,"ax",%progbits


bl_HardFault_Handler:
	BL bl_die

bl_Other_Handler:
	BL bl_die

bl_flash_handler:
	BL bl_die