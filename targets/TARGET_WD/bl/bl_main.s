/*
 * bl_main.s
 *
 * Created: 10.08.2017 11:40:20
 *  Author: Guenter.Prossliner

# Implementation note:

The bootloader is not in a part of the application that is performance-critical.
For sure we should not spend time in doing unnesserary things, but we will focus on maintainability instead of reducing some CPU cycles

# Organization:

The bootloader is seperated into multiple files:

## bootloader.s (this file) 
Contains the entry-point and implements the bootloader workflow.
The ISR-Vectors go to section .bl_vectors, all other objects to section .bl.
All functions are prefixed with bl_

## bootloader_BOARD.s (like bootloader_cb.s)
Implements function prototypes called by bootloader.s to access the hardware.
There will be more than one bootloader_BARDS.s file in the project, but only one should be linked.
All objects need to be placed in the .bl section.
All functions are prefixed with bl_hal_

# Calling convention

We will use the standard ARM calling convention for the implementation:
- Arguments will be passed from R0 to R3
- Return value will be passed in R0
- Registers R4-R11 must be perserved by the callee (PUSHM {r4-r11, lr} .... POPM {r4-r11, pc})

# Document conventions

To document the signatures of the bl_hal_ functions we will write a pseudo C signature as a comment in the sourcefile.
The use of constants (like HW-Registers or there values need to be documented in a way that allows them to be found in the datasheets.

# Update state

The process of applying an update needs to maintain some state, like if we can an valid update, the bootloader should apply an update,
or the result of this process.

First we wanted to use Option-Bits for this, but there seem be no user-defined option bits available.
We don't want to use RAM, because of it's volatile nature, and also no external memory.

The current concept don't explicitly stores this state in a single memory location.
It is reconstructed every time the bootloader runs. Changing this state is done by earse or write operations to 
one of the memory banks.

1. Valid Metadata in Boot-Image? =no=> unkown, try reapply
2. Boot bank valid (see validation procedure)? =no=> app invalid, try reapply

=> app is valid an can be launched, continue to read update status

3. Valid Metadata in Update-Image? =no=> unkown update status (factory default)
4. Is Update == Boot =no=> update already done
4. Update bank valid (see validation procedure)? =no=> update invalid, append error-code to Update bank
5. Read Update command word from flash
6. If == APPLY apply update, write Update Status word, and restart

## Metadata Validation

1. Check Metadata signurature word. Must be 0x01020304, or MD_NOTFOUND
2. Read Image size. Must be < sizeof(imageregion), or INVALID_SIZE

## Bank Validation

Depends on successfull metadata validation.
Runs a CRC (we will use the STM32 hardware CRC driver, which uses the CRC_MPEG2 implementation, must change Build-tool).
If final CRC == 0 => IMAGE_VALUE
if CRC word == 0 => IMAGE_UNVERIFYABLE (no crc signature found)
=> IMAGE_INVALID

- Default: Unkown
	- Factory default. Bootloader starts app directly
	- Update area erased

- UpdateRequested
	- enabled by user request (sw_apply_update)

- Default: UpdateSuccess
	- Update has been applied
	- Update area not erased

- UpdateValidationError
	- Update did not pass validation
	- Retry will not be successfull

- UpdateProgrammingError
	- The Flash hardware controller reported an error while programming
	- additional state may be stored in RAM

- UpdateSystemError
	- An unexpected error or interrupt occured on running the update
	- bootloader may be corrupt, or there is an programming bug
	- retry most likely to fail, but you may try ;-)

 */ 

#include "bl_asmoptions.inc"
#include "WD_ABI.h"

//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_start):
//////////////////////////////////////////////////////////////////////////
//	this is the entrypoint of the bootloader

	// B bl_start_app

	/////////////////////////////////////////////////////
	// perform hw initialization
	BL bl_hal_init

	// turn on BUS_LED to signal start of bootloader
	// MED will turn it of on initialization (somewhere in software_init_hook), so you will just see it flashing!
	/////////////////////////////////////////////////////
	MOV r0, #1
	BL bl_hal_ui


	// get system state without forcing DSA
	/////////////////////////////////////////////////////

	SUB sp, #SYSTEM_STATE_STRUCT_SIZE	// sp: system state struct

	MOV r0, sp							// r0: system state struct
	MOV r1, #0							// r1: perform dsa-validation = 0
	BL bl_get_system_state				// r0: ?, r1: ?, r2: ?, r3: ?

	LDR r0, [sp, #SYSTEM_STATE_FLDOFST_BOOTLOADER_IMAGE_STATE]	// r0: bootloader image state
	#ifndef WD_BL_ENABLE_DEVELOPMENT
		// if development mode is not enabled, bootloader-image-state must be > 0!
		BL_ASSERT(r0, HI, #0)
	#else
		// even in development, bootloader-image-state must be >= 0!
		BL_ASSERT(r0, GE, #-1)		
	#endif

	// if update validation state < 1, we can directly start the app
	LDR r0, [sp, #SYSTEM_STATE_FLDOFST_UPDATE_IMAGE_STATE]	// r0: update-image-state
	CMP r0, #1
	BLT bl_start_app

	// evaluate command word
	/////////////////////////////////////////////////////
	LDR r3, [sp, #SYSTEM_STATE_FLDOFST_COMMAND_WORD_PTR]	// r3: &command-word
	LDR r1, [r3]											// r1: command-word
	LDR r2, bl_data_commandword_apply						// r2: command-word comparant to apply update
	CMP r1, r2
	BNE bl_start_app										// skip update, because command-word not in apply-state

/*

	`Skip this, as this has already been performed when apply was performed

	// if we reach here, we have a request for update, check DSA
	/////////////////////////////////////////////////////

	MOV r0, sp							// r0: system state struct
	MOV r1, #1							// r1: perform dsa-validation = true
	BL bl_get_system_state				// r0: ?, r1: ?, r2: ?, r3: ?
*/

	LDR r0, [sp, #SYSTEM_STATE_FLDOFST_BOOTLOADER_IMAGE_STATE]	// r0: bootloader-image-state
	LDR r1, [sp, #SYSTEM_STATE_FLDOFST_UPDATE_IMAGE_STATE]	// r1: update-image-state

	CMP r0, r1
	BHI bl_start_app										// skip update, because update-image-state < bootloader-image-state (bootloader signed, but app is not)
	
	
	// if we reach here we can finally run the update
	/////////////////////////////////////////////////////

	LDR r0, [sp, #SYSTEM_STATE_FLDOFST_UPDATE_SIZE]
	BL bl_update	// bl_update(size)

	// update command-word to signal ok
	/////////////////////////////////////////////////////
	LDR r0, [sp, #SYSTEM_STATE_FLDOFST_UPDATE_SIZE]
	ADR r1, bl_data_commandword_success	
	BL bl_set_command_word

	// and run the app
	B bl_start_app

//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_get_system_state):
//////////////////////////////////////////////////////////////////////////
//	this method implements the system state discovery.
//	it is called at system startup, and if requested by swmanagement_get_status
//
//	Checking for DSA is defered to the last possible stage, because it
//	is very heavy for runtime execution.
//
//	To check for DSA, the bootloader have to be self-checked, and because
//	we have no way for persistent storage, this needs to be done every time.
//
//		r0: state-struct with the following fields (see constants SYSTEM_STATE_*):
//			0x00: bootloader image state
//			0x04: application image state
//			0x08: update image state
//			0x0C: &command-word
//			0x10: update image size
//
//		r1: int perform-dsa-validation bit mask
//			0: no validation
//			1: validate application region
//			2: validate update region
//			3: validate both
//			remark: bootloader self-sign validation is performed if any validation is requesed (!=0)
//		returns: void
PUSH {r4, r5, r6, lr}

	MOV r4, r0							// r4: system-state-struct
	MOV r5, r1							// r5: perform-dsa-validation
	MOV r6, #0							// r6: keystore

	SUB sp, #IMAGE_STATE_STRUCT_SIZE	// sp: image-load state struct

	// read bootloader state without dsa
	MOV r0, WD_FLASH_BASE	// r0: image base
	MOV r1, sp				// r1: image-state
	MOV r2, 0				// r2: to locate keystore, we first run image-validation without DSA
	BL bl_read_image
							// r0: bootloader image state pass 1
	STR r0, [r4, #SYSTEM_STATE_FLDOFST_BOOTLOADER_IMAGE_STATE]	// state[SYSTEM_STATE_FLDOFST_BOOTLOADER_IMAGE_STATE] = r0

	CMP r5, #0				// need to perform dsa?
	BEQ 1f					// continue with loading app-state

	MOV r0, sp				// r0: image-state struct
	BL bl_get_keystore		// r0: keystore
	MOV r6, r0				// r6: keystore
	LDR r0, [sp, #IMAGE_STATE_FLDOFST_STAGE_INDEX]	// r0: bootloader final state
	STR r0, [r4, #SYSTEM_STATE_FLDOFST_BOOTLOADER_IMAGE_STATE]	// state[SYSTEM_STATE_FLDOFST_BOOTLOADER_IMAGE_STATE] = r0

	1:

	// get application image state
	LDR r0, bl_data_image_start	// r0: image start
	MOV r1, sp					// r1: state
	TST r5, #1					// need to validate app-region?
	ITE EQ
	MOVEQ r2, #0				// r2: keystore or NULL
	MOVNE r2, r6
	BL bl_read_image			// r0: application-image-state
	STR r0, [r4, #SYSTEM_STATE_FLDOFST_APPLICATION_IMAGE_STATE]	// state[SYSTEM_STATE_FLDOFST_APPLICATION_IMAGE_STATE] = r0

	// get update image state
	LDR r0, bl_data_update_image_start	// r0: update image start
	MOV r1, sp					// r1: state
	TST r5, #2					// need to validate update-region?
	ITE EQ
	MOVEQ r2, #0				// r2: keystore or NULL
	MOVNE r2, r6
	BL bl_read_image			// r0: update-image-state
	STR r0, [r4, #SYSTEM_STATE_FLDOFST_UPDATE_IMAGE_STATE]	// state[SYSTEM_STATE_FLDOFST_APPLICATION_IMAGE_STATE] = r0
	LDR r0, [sp, #IMAGE_STATE_FLDOFST_IMAGE_SIZE]	// r0: update-image-size
	STR r0, [r4, #SYSTEM_STATE_FLDOFST_UPDATE_SIZE]	// state[SYSTEM_STATE_FLDOFST_UPDATE_SIZE] = r0
	
	// get command-word address
	LDR r1, bl_data_update_image_start	// r1: update image start
	ADD r0, r1							// r0: &comand-word
	STR r0, [r4, #SYSTEM_STATE_FLDOFST_COMMAND_WORD_PTR]	// state[SYSTEM_STATE_FLDOFST_COMMAND_WORD_PTR] = r0

0:
ADD sp, #IMAGE_STATE_STRUCT_SIZE
POP {r4, r5, r6, pc}

//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_start_app):
//	This function starts the application. It will not return.
//////////////////////////////////////////////////////////////////////////

	LDR r0, bl_data_image_start

	// load SP
	LDR r1, [r0, #0]
	MOV sp, r1

	// load PC
	LDR r1, [r0, #4]
	MOV pc, r1


//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_get_keystore):
//////////////////////////////////////////////////////////////////////////
//	Gets the pointer to the keystore in bootload, and checks bootloader self-signature
//
//	void * bl_get_keystore(void * bootloader_image_struct);
//
//		r0: bootloader image state
//
//////////////////////////////////////////////////////////////////////////
PUSH {r4, r5, r6, lr}

	MOV r4, r0						// r4: image-state
	MOV r0, #0						// r0: return NULL by default

	// locate keystore
	LDR r3, [r4, #IMAGE_STATE_FLDOFST_HEADER_WORD]	// r3: header-word
	TST r3, WD_ABI_HDR_FLAG_KEYSTR
	BEQ 0f							// return if no keystore flag

	// calculate offset in r2 from end based on flags
	MOV r2, #WD_ABI_SIZE_KEYSTR
	TST r3, WD_ABI_HDR_FLAG_CRC
	IT NE
	ADDNE r2, #WD_ABI_SIZE_CRC

	TST r3, WD_ABI_HDR_FLAG_DSA
	IT NE
	ADDNE r2, #WD_ABI_SIZE_SIGNATURE

	// get keystore address
	LDR r0, [r4, #IMAGE_STATE_FLDOFST_IMAGE_START]		// r0: image_start
	LDR r1, [r4, #IMAGE_STATE_FLDOFST_IMAGE_SIZE]		// r1: image_size total
	ADD r0, r1					// r0: image-end
	SUB r0, r2					// r0: &keystore-header

	// test keystore version
	LDR r2, [r0]					// r2: keystore-header
	LDR r1, =#WD_ABI_KEYSTR_MAGIC	// r1: expected header
	CMP r1, r2
	BNE bl_die

	ADD r0, #0x04				// r0: &keystore
	MOV r6, r0					// r6: &keystore

	// re-test with keystore for DSA self-validation
	MOV r0, WD_FLASH_BASE		// r0: image base
	MOV r1, r4					// r1: state
	MOV r2, r6					// r2: keystore
	BL bl_read_image

	// r0: MUST BE 2 (valid DSA, otherwise we have an invalid self-signature!)
	BL_ASSERT(r0, EQ, #2)

	MOV r0, r6		// r1: keystore
0:
POP {r4, r5, r6, pc}


//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_set_command_word):
//////////////////////////////////////////////////////////////////////////
//	void bl_set_command_word(int image-size, int * command_word)
//	updates the command-word in flash.
//
//	NOTE: There is no erase here, so after erase (after factory programming or sw-update), you can only update bits from 1 to 0.
//	the command-word is: 
//	- 0xFFFFFFFF on erase, no further action
//	- 0x0000FFFF if the bootloader should  load the new image, set by bootloader after validation
//	- 0x00000000 if bootloader applied the image, can only be reset by a new image
//
PUSH {r4, lr}

	LDR r2, bl_data_update_image_start
	ADD r0, r2		// r0 (dest): &command_word_in_flash
	
	MOV r4, r1		// command_word

	// local buffer (we uses stack) needed for flash_memcpy
	MOV r1, r4
	MOV r2, #4		// r2: (size): always 4
	BL bl_hal_flash_memcpy	// perform the write

POP {r4, pc}

/*************************************************************************
	void bl_update(int size)

	this is the entypoint for erase and write of the image
	size is passed form the validation procedure
*/
.global bl_update
.type bl_update, %function
bl_update:
PUSH {r4, lr}

	// variable regisers
	// r4: size
	MOV r4, r0

	// signal bl-flash operation by blinking two times
	MOV r0, #0
	BL bl_hal_ui
	BL bl_hal_sleep

	MOV r0, #1
	BL bl_hal_ui
	BL bl_hal_sleep

	MOV r0, #0
	BL bl_hal_ui
	BL bl_hal_sleep

	MOV r0, #1
	BL bl_hal_ui
	BL bl_hal_sleep


	// erase app bank
	BL bl_hal_erase_boot_image

	// flash progamming
	LDR r0, bl_data_image_start
	LDR r1, bl_data_update_image_start
	MOV r2, r4
	BL bl_hal_flash_memcpy
	
0:
POP {r4, pc}



//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_die):
//////////////////////////////////////////////////////////////////////////
//	Notifies about a fatal condition in bootloader.
//	Will not return, need to reset manually.
	0:

		MOV r0, #0
		BL bl_hal_ui
		BL bl_hal_sleep

		MOV r0, #1
		BL bl_hal_ui
		BL bl_hal_sleep

		B 0b



bl_data_image_start: .word __image_start
bl_data_update_image_start : .word __update_image_start
bl_data_metadata_offset : .word WD_ABI_METADATA_SECTION_OFFSET
bl_data_metadata_magic: .word WD_ABI_METADATA_MAGIC
bl_data_application_max_size: .word __application_max_size
bl_data_commandword_apply: .word 0x000000FF
bl_data_commandword_success: .word 0x00000000
// TODO //
bl_data_unverifiable_crc_value: .word 0
bl_data_cpuid_address: .word 0xE000ED00


// the assembler will emit it's data from the LDR r2, =<constant> expressions at the end of the file
// so this label is to clean up the dissassembly.
bl_misc_data: