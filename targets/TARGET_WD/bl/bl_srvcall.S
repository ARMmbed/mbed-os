/*
 * bl_srvcall.S
 *
 * Created: 02.11.2017 14:20:46
 *  Author: Guenter.Prossliner
 */ 

#include "bl_asmoptions.inc"

//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_srv_call):
//////////////////////////////////////////////////////////////////////////
//	This is the entrypoint service calls from the application to the bootloader.
//	it's it called indirectly from the application by the address emitted to ABI Header Offset SRVCALL
//		r0: struct blsrv_desc * arg (see blsrv.h)
PUSH {r4, r5, r6, lr}

	MOV r4, r0					// r4: arg, will be post-incremented to read struct in stream
	LDR r1, [r4], #4			// r1: operation
	
	SUB r1, #1					// r1: operation table offset (commands start with 1, vs table with 0)
	CMP r1, #4					// 4 is is the highest accepted offset
	ITT HI						// if (r1>4) return false;
	MOVHI r0, 0
	BHI 0f

	ADR r0, bl_srv_call_jump_table	// r0: jump table address
	LDR r1, [r0, r1, LSL #2]		// r1: label address (pc=[r0+(r1<<2)])
	ORR r1, #1						// thumb bit :-(
	BX r1

	// r5 and r6 are alread pushed. They can be used to store vars in the case blocks (.L_blsrv_*)
	.L_blsrv_erase_update_region:
		BL bl_hal_erase_update_image
		B 1f

	.L_blsrv_write_update_region:
		// load args
		LDR r0, [r4], #4	// load offset
		LDR r1, bl_data_update_image_start	// load base address
		ADD r0, r1			// dest = offset + base address
		LDR r1, [r4], #4	// src
		LDR r2, [r4], #4	// size
		BL bl_hal_flash_memcpy
		B 1f

	.L_blsrv_get_status:

		// load args
		LDR r1, [r4], #4					// r1: force dsa
		SUB sp, #SYSTEM_STATE_STRUCT_SIZE	// sp: system state struct
		MOV r0, sp							// r0: system state struct
		BL bl_get_system_state				// r0: ?, r1: ?, r2: ?, r3: ?

		// set output args
		LDR r0, [sp, #SYSTEM_STATE_FLDOFST_BOOTLOADER_IMAGE_STATE]
		STR r0, [r4], #4
		LDR r0, [sp, #SYSTEM_STATE_FLDOFST_APPLICATION_IMAGE_STATE]
		STR r0, [r4], #4
		LDR r0, [sp, #SYSTEM_STATE_FLDOFST_UPDATE_IMAGE_STATE]
		STR r0, [r4], #4
		LDR r0, [sp, #SYSTEM_STATE_FLDOFST_COMMAND_WORD_PTR]
		LDR r0, [r0]
		STR r0, [r4], #4
		
		ADD sp, #SYSTEM_STATE_STRUCT_SIZE	// dealloc stack memory
		B 1f

	.L_blsrv_apply_update:

		SUB sp, #SYSTEM_STATE_STRUCT_SIZE	// sp: system state struct
		MOV r0, sp							// r0: system state struct
		MOV r1, #2							// r1: force dsa for update region
		BL bl_get_system_state				// r0: ?, r1: ?, r2: ?, r3: ?

		LDR r0, [sp, #SYSTEM_STATE_FLDOFST_BOOTLOADER_IMAGE_STATE]
		LDR r1, [sp, #SYSTEM_STATE_FLDOFST_UPDATE_IMAGE_STATE]
		LDR r5, [sp, #SYSTEM_STATE_FLDOFST_UPDATE_SIZE]

		ADD sp, #SYSTEM_STATE_STRUCT_SIZE	// release stack memory


		// store update-state in output-field
		STR r1, [r4], #4

		CMP r0, r1	
		BHI 1f		// if(bootloader_image_state > update_image_state) return;

		MOV r0, r5							// r0: update image size
		ADR r1, bl_data_commandword_apply	// r1: &command-word-apply (0x000000FF)
		BL bl_set_command_word

		B 1f

	.L_blsrv_write_config_data:
		// load args
		LDR r0, [r4], #4	// load offset
		MOV r1, #0x210	// BL ABI Header (BANK2OFF)
		LDR r1, [r1]	// load base address 
		ADD r0, r1			// dest = offset + base address
		LDR r1, [r4], #4	// src
		LDR r2, [r4], #4	// size
		BL bl_hal_flash_memcpy
		B 1f

1:
MOVS r0, 1
0:
POP {r4, r5, r6, pc}

.align 4

bl_srv_call_jump_table:
.word .L_blsrv_erase_update_region		// 1
.word .L_blsrv_write_update_region		// 2
.word .L_blsrv_get_status				// 3
.word .L_blsrv_apply_update				// 4
.word .L_blsrv_write_config_data		// 5


bl_data_image_start: .word __image_start
bl_data_update_image_start : .word __update_image_start
bl_data_commandword_apply: .word 0x000000FF
