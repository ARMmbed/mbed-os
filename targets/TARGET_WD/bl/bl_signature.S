
/*
 * bl_signature.S
 *
 * Created: 27.09.2017 15:02:06
 *  Author: Guenter.Prossliner
 */ 

#include "bl_asmoptions.inc"

#include "WD_ABI.h"

/*************************************************************************
	
	(struct * key_vectors, struct * signature_vectors) bl_signature_findkey (void * signature, void * keystore);
	
	Finds the key specified by signature.
	Returns a pointer to the signature-struct, which can be directly passed to the ecdsa_validate_image fn:

	r0:
		struct {
			uint32_t x[0x08];
			uint32_t y[0x08];
		} * key_vectors;

	r1:
		struct {
			uint32_t r[0x08];
			uint32_t s[0x08];
		} * signature_vectors;

	
	
*************************************************************************/
BL_GLOBAL_FUNCTION(bl_signature_findkey):
push {r4, r5, r6, lr}

	mov r4, r0	// r5: &signature_header
	mov r6, r1	// r6: &keystore_data

	// compare signature version
	ldr r0, [r4], #4								// r0: signature magic, r4: &signature_data
	ldr r1, bl_signature_current_version			// r1: signature magic expected
	cmp r0, r1
	bne 1f	// return NULL
	
	// compare the thumbprint
	mov r0, r4		// r0: &thumbprint_signature
	mov r1, r6		// r1: &thumbprint_keystore
	mov r2, #0x20	// r2: sizeof(thumbprint)
	bl bl_memcmp
	cmp r0, #0
	bne 1f	// return NULL

	// set ptrs to vectors (skip thumbprint)
	add r1, r4, #0x20	// r1: &signature_vectors
	add r0, r6, #0x20	// r0: &key_vectors
	b 0f
1:
	mov r0, 0
0:
pop {r4, r5, r6, pc}


/*************************************************************************
	
	int bl_signature_validate (void * image, size_t size, void * keystore);
	
	Checks the image for a valid signature.
	Return-Values:
		0: validation ok!
		-1: nok
	
*************************************************************************/
BL_GLOBAL_FUNCTION(bl_signature_validate):
push {r4, r5, r6, lr}
sub sp, #32

							// r0: start
							// r1: len
							// r2: keystore
	/*
	#define r_start r4
	#define r_len	r5
	#define r_pk	r6
	#define r_hash	sp
	*/

	mov r4, r0				// r4: start	
	mov r5, r1				// r5: len
	mov r6, r2				// r6: keystore

	// hash the image
	// r0 = start and r1 = len already
	mov r2, sp				// r2: hash-buffer
	bl bl_sha256_digest	

	// find the key
	mov r0, r4				// r0: start
	add r0, r5				// r0: &signature_header
	mov r1, r6				// r1: keystore
	bl bl_signature_findkey
							// r0: &key_vectors, or NULL if nf
							// r1: &signature_vectors
	cmp r0, #0
	beq 1f	// return false

	// int bl_ecdsa_validate_hash(hash, pk, sig);
	mov r2, r1					// r2: &signature_vectors
	mov r1, r0					// r1: &key_vectors
	mov r0, sp					// r0: hash-buffer
	bl bl_ecdsa_validate_hash

	mov r0, #0
	b 0f
1:
	mov r0, #-1
0:
add sp, #32
pop {r4, r5, r6, pc}




.align 4
bl_header_adr:					.word WD_FLASH_BASE + WD_ABI_BL_HEADER_OFFSET
bl_signature_current_version:	.word WD_ABI_SIGNATURE_CURRENT_VERSION
bl_keystore_current_version:	.word WD_ABI_KEYSTORE_CURRENT_VERSION
