/*
 * bootloader_sign.S
 *
 * Created: 19.09.2017 12:05:11
 *  Author: Guenter.Prossliner
 */ 

#include "bl_asmoptions.inc"

//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_sha256_digest):
//////////////////////////////////////////////////////////////////////////
//	Calculates a SHA256 hash of a speicified memory-section
//
//		r0: void * start
//		r1: int len
//		r2: void * buffer (uint32_t[8] output will be written)
push	{r4, r5, r6, r7, lr}
sub	sp, #104	// 0x68

	mov	r6, r0		// r6: start
	mov	r5, r1		// r5: len
	mov	r4, r2		// r4: digest
	mov	r7, sp		// r7: ctx

	mov r0, r7
	bl	bl_sha256_init

	mov	r0, r7
	mov	r1, r6
	mov	r2, r5
	bl	bl_sha256_update

	mov r0, #0	// arg 0 is unused
	mov	r1, r7
	bl	bl_sha256_final

	// we have the correct sha256 in sp, 
	// let's copy it to the output buffer digest to and reverse byte order (needed for ECDSA validation)
	mov r6, #32
	add r4, #28
	0:
		ldr r0, [r7], #4
		str r0, [r4], #-4
		subs r6, #4
		bne 0b


add	sp, #104	// 0x68
pop	{r4, r5, r6, r7, pc}


//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_sha256_init):
//////////////////////////////////////////////////////////////////////////
push {r4, lr}

	mov		r4, r0	// r4: ctx

	// fill initial value
	adr		r1, bl_sha256_initial_hash_value
	movs	r2, #32
	bl		bl_memcpy

	// clear rest of ctx
	add		r0, r4, #32	// 0x20 == offsetof(bitcount)
	movs	r1, #0
	movs	r2, #72		// 0x48 == sizeof(ctx)-offsetof(bitcount)
	bl		bl_memset

pop	{r4, pc}

//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_sha256_update):
//////////////////////////////////////////////////////////////////////////
push {r3, r4, r5, r6, r7, lr}
	mov	r5, r0	// r5: ctx
	mov	r7, r1	// r7: data
	mov	r4, r2	// r8: len)

	// if (!len) return;
	cmp	r2, #0
	beq.n	0f

	// calculate usedspace
	ldr	r0, [r0, #32]
	ubfx	r0, r0, #3, #6

	// if(usedspace > 0) // this is if there is already data written, and could possiby be removed
	cbz	r0, 1f
		rsb	r6, r0, #64	// 0x40
		cmp	r2, r6
		add.w	r0, r0, #40	// 0x28
		add	r0, r5
		bcc.n	2f
		mov	r2, r6
		bl	bl_memcpy
		ldrd	r2, r3, [r5, #32]
		lsls	r1, r6, #3
		adds	r2, r2, r1
		adc.w	r3, r3, #0
		strd	r2, r3, [r5, #32]
		mov	r0, r5
		add.w	r1, r5, #40	// 0x28
		subs	r4, r4, r6
		add	r7, r6
		bl	bl_sha256_transform

	1:
	mov	r6, r4

4:
	subs	r1, r4, r6
	cmp	r6, #63	// 0x3f
	add	r1, r7
	bls.n	3f
	mov	r0, r5
	bl	bl_sha256_transform
	ldrd	r2, r3, [r5, #32]
	adds.w	r2, r2, #512	// 0x200
	adc.w	r3, r3, #0
	strd	r2, r3, [r5, #32]
	subs	r6, #64	// 0x40
	b.n	4b
3:
	bic.w	r1, r4, #63	// 0x3f
	ands.w	r4, r4, #63	// 0x3f
	add	r1, r7
	beq.n	0f
	add.w	r0, r5, #40	// 0x28
	mov	r2, r4
2:
	bl	bl_memcpy
	ldrd	r2, r3, [r5, #32]
	lsls	r4, r4, #3
	adds	r2, r2, r4
	adc.w	r3, r3, #0
	strd	r2, r3, [r5, #32]
0:
	pop	{r3, r4, r5, r6, r7, pc}


//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_sha256_transform):
//////////////////////////////////////////////////////////////////////////
stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	sub	sp, #28
	ldr	r3, [r0, #8]
	str	r3, [sp, #4]
	ldmia.w	r0, {r4, r5}
	ldr	r3, [r0, #24]
	ldr.w	sl, [r0, #12]
	ldr	r2, [r0, #16]
	ldr	r6, [r0, #20]
	ldr.w	r9, [r0, #28]
	str	r3, [sp, #8]
	add.w	lr, r0, #40	// 0x28
	movs	r3, #0
1:
	ldr.w	r7, [r1, r3, lsl #2]
	rev.w	fp, r7
	ldr	r7, [sp, #8]
	str.w	fp, [lr, r3, lsl #2]
	mov.w	ip, r2, ror #6
	bic.w	r8, r7, r2
	eor.w	ip, ip, r2, ror #11
	and.w	r7, r6, r2
	eor.w	ip, ip, r2, ror #25
	eor.w	r7, r8, r7
	add	r7, ip
	adr	ip, bl_sha256_K256
	add	r7, fp
	ldr.w	fp, [ip, r3, lsl #2]
	add	fp, r7
	ldr	r7, [sp, #4]
	eor.w	ip, r7, r5
	add	r9, fp
	and.w	ip, ip, r4
	and.w	fp, r7, r5
	eor.w	fp, ip, fp
	mov.w	ip, r4, ror #2
	eor.w	ip, ip, r4, ror #13
	eor.w	r7, ip, r4, ror #22
	adds	r3, #1
	add.w	ip, fp, r7
	cmp	r3, #16
	add.w	r8, sl, r9
	add.w	fp, ip, r9
	ldr.w	sl, [sp, #4]
	ldr.w	r9, [sp, #8]
	beq.n	0f
	str	r6, [sp, #8]
	str	r5, [sp, #4]
	mov	r6, r2
	mov	r5, r4
	mov	r2, r8
	mov	r4, fp
	b.n	1b
0:
	adds	r1, r3, #1
	str	r1, [sp, #16]
	and.w	r1, r1, #15
	ldr.w	r9, [lr, r1, lsl #2]
	mov.w	r1, r9, ror #7
	eor.w	r1, r1, r9, ror #18
	str	r1, [sp, #20]
	add.w	r1, r3, #14
	and.w	r1, r1, #15
	ldr.w	r1, [lr, r1, lsl #2]
	str	r1, [sp, #12]
	mov.w	sl, r1, ror #17
	eor.w	ip, sl, r1, ror #19
	add.w	r1, r3, #9
	and.w	sl, r3, #15
	and.w	r1, r1, #15
	ldr.w	r7, [lr, sl, lsl #2]
	ldr.w	r1, [lr, r1, lsl #2]
	add	r1, r7
	ldr	r7, [sp, #20]
	eor.w	r9, r7, r9, lsr #3
	ldr	r7, [sp, #12]
	add	r1, r9
	eor.w	r7, ip, r7, lsr #10
	mov.w	ip, r8, ror #6
	add	r1, r7
	eor.w	ip, ip, r8, ror #11
	bic.w	r9, r6, r8
	and.w	r7, r2, r8
	eor.w	r7, r9, r7
	eor.w	ip, ip, r8, ror #25
	add	ip, r7
	adr	r7, bl_sha256_K256
	str.w	r1, [lr, sl, lsl #2]
	ldr.w	r7, [r7, r3, lsl #2]
	ldr	r3, [sp, #8]
	str	r6, [sp, #8]
	add	ip, r7
	add	ip, r1
	add	r3, ip
	eor.w	ip, r5, r4
	mov	r7, r3
	and.w	ip, ip, fp
	and.w	r3, r5, r4
	eor.w	ip, ip, r3
	mov.w	r3, fp, ror #2
	eor.w	r3, r3, fp, ror #13
	eor.w	r3, r3, fp, ror #22
	add	ip, r3
	ldr	r3, [sp, #4]
	str	r5, [sp, #4]
	add	r3, r7
	mov	r1, r3
	ldr	r3, [sp, #16]
	cmp	r3, #64	// 0x40
	add	ip, r7
	beq.n	2f
	mov	r6, r2
	mov	r5, r4
	mov	r2, r8
	mov	r4, fp
	mov	r8, r1
	mov	fp, ip
	b.n	0b
2:
	ldr	r3, [r0, #0]
	add	r3, ip
	str	r3, [r0, #0]
	ldr	r3, [r0, #4]
	add	r3, fp
	str	r3, [r0, #4]
	ldr	r3, [r0, #8]
	add	r4, r3
	ldr	r3, [r0, #12]
	str	r4, [r0, #8]
	add	r5, r3
	ldr	r3, [r0, #16]
	str	r5, [r0, #12]
	add	r1, r3
	ldr	r3, [r0, #20]
	str	r1, [r0, #16]
	add	r3, r8
	str	r3, [r0, #20]
	ldr	r3, [r0, #24]
	add	r2, r3
	ldr	r3, [r0, #28]
	str	r2, [r0, #24]
	add	r6, r3
	str	r6, [r0, #28]
	add	sp, #28
ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}

//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_sha256_final):
//////////////////////////////////////////////////////////////////////////
push	{r3, r4, r5, lr}
	ldr	r3, [r1, #32]
	mov	r4, r1
	ldr	r1, [r1, #36]	// 0x24
	ubfx	r2, r3, #3, #6
	rev	r1, r1
	rev	r3, r3
	str	r1, [r4, #32]
	str	r3, [r4, #36]	// 0x24
	add.w	r5, r4, #40	// 0x28
	cbz	r2, 0f
	adds	r1, r2, #1
	adds	r3, r4, r2
	movs	r0, #128	// 0x80
	cmp	r1, #56	// 0x38
	strb.w	r0, [r3, #40]	// 0x28
	bhi.n	1f
	add.w	r0, r2, #41	// 0x29
	add	r0, r4
	movs	r1, #0
	rsb	r2, r2, #55	// 0x37
	b.n	2f
1:
	cmp	r1, #64	// 0x40
	beq.n	3f
	add.w	r0, r2, #41	// 0x29
	add	r0, r4
	movs	r1, #0
	rsb	r2, r2, #63	// 0x3f
	bl	bl_memset
3:
	mov	r0, r4
	mov	r1, r5
	bl	bl_sha256_transform
	mov	r0, r5
	movs	r1, #0
	movs	r2, #56	// 0x38
2:
	bl	bl_memset
	b.n	4f
0:
	mov	r1, r2
	mov	r0, r5
	movs	r2, #56	// 0x38
	bl	bl_memset
	movs	r3, #128	// 0x80
	strb.w	r3, [r4, #40]	// 0x28
4:
	ldrd	r2, r3, [r4, #32]
	mov	r0, r4
	strd	r2, r3, [r4, #96]	// 0x60
	mov	r1, r5
	ldmia.w	sp!, {r3, r4, r5, lr}
b bl_sha256_transform


//////////////////////////////////////////////////////////////////////////
// STATIC DATA
//////////////////////////////////////////////////////////////////////////

.align 4

bl_sha256_initial_hash_value:
	.word	0x6a09e667
	.word	0xbb67ae85
	.word	0x3c6ef372
	.word	0xa54ff53a
	.word	0x510e527f
	.word	0x9b05688c
	.word	0x1f83d9ab
	.word	0x5be0cd19


bl_sha256_K256:
	.word 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5
	.word 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5
	.word 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3
	.word 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174
	.word 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc
	.word 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da
	.word 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7
	.word 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967
	.word 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13
	.word 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85
	.word 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3
	.word 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070
	.word 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5
	.word 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3
	.word 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208
	.word 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2

