/*
 * bl_image.S
 *
 * Created: 29.10.2017 21:08:03
 *  Author: Guenter.Prossliner


Implements the staged image loading and validation.
If a stage is completed with != 0, no further stages are executed.
*/

#include "bl_asmoptions.inc"
#include "WD_ABI.h"

.align 4
//////////////////////////////////////////////////////////////////////////
.L_validation_stages_tbl:
//////////////////////////////////////////////////////////////////////////
//	Validation method table
//
//		Every method is called with the following convention:
//			r0: void * state_struct
//		Returns
//			r0: 0 if valid, else invalid
//		Values can be loaded and stored in the state_struct (IMAGE_STATE_FLDOFST_XXX) offsets

	// 0:												==> LEVEL -2: "EMPTY
	// 1: checks if there is any valid header			==> LEVEL -1: "INCOMPATIBLE"
	.word bl_read1_header_exisits

	// 2: checks if the masked CPUID matches			==> LEVEL 0: "COMPATIBLE"
	.word bl_read2_cpuid_valid							

	// 3: checks if there is a header postprocessed 
	// by the elf-tool (has valid size)					==> LEVEL 1: "IMAGE VALID"
	.word bl_read3_image_valid

	// checks if the digital signature is valid			==> LEVEL 2: "IMAGE DSA VALID"
	.word bl_read4_dsa_valid

	// Mandatory end of table marker
	.word 0
/*

The primary return-value is the index of the last executed stage (stage_index)
Additional information about the image is returned by a image_state structure, passed as a pointer to bl_read_image.

	struct {

		// offset: 0x00
		// setby: bl_read_image entry method, updated by each successful stage
		// number of last check succeeded. Also returned in r0 by bl_read_image.
		int stage_index;

		// offset: 0x04
		// setby: bl_read_image entry method
		// start of image to read
		void * image_start;		

		// offset: 0x08
		// setby: bl_read1_header_exisits
		// content header-word (lower start-word: contains image-kind, crc padding and dsa flags)
		int header_word;

		// offset: 0x0C
		// setby: bl_read3_image_valid
		// size of image, including CRC, Keystore and signature
		int image_size;		

		// offset: 0x10
		// setby: bl_read_image entry method
		// ptr to keystore
		void * keystore;		

	} image_state;

 */ 

//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_read_image):
//////////////////////////////////////////////////////////////////////////
//	Evaluates the status of an WD-ABI image in flash memory
//		r0: void * image_base	// base address (booloader, app or update)
//		r1: void * state_struct	// pointer to a IMAGE_STATE_STRUCT_SIZE region of memory, to store image details
//		r2: void * keystore		// the address of the keystore to validate ECDSA, if 0 no ECDSA is checked
//
//	Returns
//		r0: the stage_index of the last successfully executed read-method
//		details can be loader from the state_struct, using IMAGE_STATE_FLDOFST_XXX offsets
PUSH {r4, r5, r6, lr}

	MOV r4, r1			// r4: state

	// init state
	STR r0, [r4, #IMAGE_STATE_FLDOFST_IMAGE_START]	// state->image_start = image_start

	MOV r0, #-2
	STR r0, [r4, #IMAGE_STATE_FLDOFST_STAGE_INDEX]	// state->stage_index = -2

	MOV r0, #0
	STR r0, [r4, #IMAGE_STATE_FLDOFST_HEADER_WORD]	// state->header_word = 0
	STR r0, [r4, #IMAGE_STATE_FLDOFST_IMAGE_SIZE]	// state->image_size = 0

	STR r2, [r4, #IMAGE_STATE_FLDOFST_KEYSTORE]		// state->keystore = keystore

	// call validation methods
	// r5 will hold the address of the method in the table
	// r1 will deref the address, and so contain the pointer to the function
	// r0 will hold the state arg passed to the function
	ADR r5, .L_validation_stages_tbl

	1:	// next
		LDR r6, [r5], #4	// post-incr r5
		CMP r6, #0			// if(!r6)
		BEQ 0f				// return

		MOV r0, r4
		BLX r6				// call r6(state)
		CMP r0, #0			// if(!retvalue)
		BNE 0f				// return

		LDR r0, [r4]		// state->validation_result++
		ADD r0, #1
		STR r0, [r4, #0]

	B 1b	// goto next
	
0:	// done 
LDR r0, [r4]	// return stage_index in r0
POP {r4, r5, r6, pc}


//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_read1_header_exisits):
//////////////////////////////////////////////////////////////////////////
//	Checks for a valid WD-ABI Header at HDR_OFFSET
PUSH {lr}

	LDR r1, [r0, #IMAGE_STATE_FLDOFST_IMAGE_START]	// r1: image_start
	ADD r1, #WD_ABI_HDR_OFFSET						// r1: &abi-header

	LDR r2, [r1]									// r2: hdr-magic
	STR r2, [r0, #IMAGE_STATE_FLDOFST_HEADER_WORD]	// store into state->header_flags

	// validate flags for WD_ABI_HDR_MAGIC
	LSR r1, r2, #16					// r1: high word of magic
	LDR r2, =#WD_ABI_HDR_MAGIC		// r2: magic expected
	SUB r0, r1, r2					// r0: 0 == MAGIC VALID, everything else: MAGIC INVALID

POP {pc}

//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_read2_cpuid_valid):
//////////////////////////////////////////////////////////////////////////
//	Checks CPUID compatibility
PUSH {lr}
								// r0: state
	LDR r0, [r0, #IMAGE_STATE_FLDOFST_IMAGE_START]	// r0: image_start
	ADD r0, WD_ABI_HDR_OFFSET						// r0: image_header
	LDR r1, [r0, #WD_ABI_HDR_OFFSET_CPUMASK]		// r1: CPUID-mask
	LDR r2, [r0, #WD_ABI_HDR_OFFSET_CPUID]			// r2: CPUID-comparant

	// Load CPUID into r0
	LDR r0, bl_data_cpuid_address						// truth table example
	LDR r0, [r0]										// r0: running cpuid		11110000	r0: running
														// r2: cpuid compare		10101010	r2: compare
														// r1: cpuid-mask			11001100	r1: mask
	EOR r0, r2											//							01011010	r0 = r0 xor r2 (all 1 bits are different)
	AND r0, r1											//							01001000	r0 = r0 and r1 (two bits dont match, so this would be incompatible)

POP {pc}

//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_read3_image_valid):
//////////////////////////////////////////////////////////////////////////
//	Checks if the header was post-processed (by testing the SIZE field)
//	And performs the CRC validation of the image
PUSH {r4, r5, r6, lr}

	MOV r4, r0										// r4: state

	LDR r1, [r4, #IMAGE_STATE_FLDOFST_IMAGE_START]	// r1: image-base
	MOV r2, r1
	ADD r2, #WD_ABI_HDR_OFFSET						// r2: &abi-header
	LDR r2, [r2, #4]								// r2: image-size loader from header
	LDR r0, =__application_max_size					// r0: maximumn app size
	CMP r2, r0										// if (size>max) return 1
		ITT HI
		MOVHI r0, 1
		BHI 0f

	// check header_flags if we have crc padding
	LDR r0, [r4, #IMAGE_STATE_FLDOFST_HEADER_WORD]	// r1: header_word
	TST r0, WD_ABI_HDR_FLAG_CRC						// if(!header & FLAG_CRC) return 0
		ITT EQ
		MOVEQ r0, 0
		BEQ 0f

	STR r2, [r4, #IMAGE_STATE_FLDOFST_IMAGE_SIZE]	// state->image_size = r2

	// perform crc validation
	MOV r5, r1										// r5: ptr, post-increment
	MOV r6, r2										// r6: size, post-decrement

	BL bl_hal_crc_init

	3: // while(size) {

		LDR r0, [r5], #4							// r0 = *r5+=4 // r0: loaded word

		// LDR reads not the pysical order, it read LittleEndian, so we get 0x01020304, but in the file we have 0x4030201! 
		// to allow that we can form the CRC on the file (it seems better IMO), to REV to file-order here
		REV r0, r0	
		BL bl_hal_crc_update	

		SUBS r6, #4
	BNE 3b // while(size)

	// r0 contains the crc from the last call to bl_hal_crc_update, which should be 0!
	// we can return it directly

0: 
POP {r4, r5, r6, pc}

//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_read4_dsa_valid):
//////////////////////////////////////////////////////////////////////////
//	Performs ECDSA validation
PUSH {r4, lr}

	MOV r4, r0										// r4: state

	// fail if no keystore in state
	LDR r2, [r4, #0x10]								// r2: keystore
	CMP r2, #0
	ITT EQ
	MOVEQ r0, #1
	BEQ 0f

	// load args
	LDR r0, [r4, #IMAGE_STATE_FLDOFST_IMAGE_START]	// r0: image_start
	LDR r1, [r4, #IMAGE_STATE_FLDOFST_IMAGE_SIZE]	// r1: size total
	SUB r1, #WD_ABI_SIZE_SIGNATURE+WD_ABI_SIZE_CRC	// r1: size without signature
													// r2: keystore
	BL bl_signature_validate	//(void * image, size_t size, void * keystore);
		// r0: signature_state (0: VALID; -1: INVALID)
	
0:
POP {r4, pc}

.align 4
bl_data_cpuid_address: .word 0xE000ED00
