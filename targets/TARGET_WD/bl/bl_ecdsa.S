/*
 * bl_ecdsa.S
 *
 * Created: 26.09.2017 21:37:55
 *  Author: Guenter.Prossliner
*/ 

#include "bl_asmoptions.inc"


.section .bl_text,"ax",%progbits
.global bl_ecdsa_validate_image

/*
	int bl_ecdsa_validate(start, len, pk)
	hash:	ptr to a 0x20 blob of the hashed image
	pk:		ptr to a 0x40 blob of the public key, first 0x20 bytes are X, next Y
*/
.type bl_ecdsa_validate_image, %function
bl_ecdsa_validate_image:
push {r4, r5, r6, lr}
sub sp, #32

	#define r_start r4
	#define r_len	r5
	#define r_pk	r6
	#define r_hash	sp

	mov r_start, r0	
	mov r_len, r1	
	mov r_pk, r2	

	// hash the image
	// r0 = start and r1 = len already
	mov r2, r_hash
	bl bl_sha256_digest	

	mov r0, r_hash
	mov r1, r_pk
	add r2, r_start, r_len
	add r2, #0x24	// skip magic and thumbprint
	bl bl_ecdsa_validate_hash

add sp, #32
pop {r4, r5, r6, pc}

/*

	int bl_ecdsa_validate_hash(hash, pk, sig);
	hash:	ptr to a 0x20 blob of the hashed image
	pk:		ptr to a 0x40 blob of the public key, first 0x20 bytes are X, next Y
	sig:	ptr to a 0x40 blob of the signature, first 0x20 bytes are R, next S
*/

.global bl_ecdsa_validate_hash
.type bl_ecdsa_validate_hash, %function
bl_ecdsa_validate_hash:

	// return true
	/*mov r0, #0
	mov pc, lr
	*/
	push	{r4, r5, r6, r7, lr}
	sub	sp, #372

		mov	r5, r1	// key
		mov	r4, r2	// sig
		mov	r6, r0	// hash

	// Calculate w = s^{-1} \pmod{n}
		add	r3, sp, #8				// w
		add.w	r0, r2, #32
		adr	r1, bl_data_ecdsa_order_m
		adr	r2, bl_data_ecdsa_order_r
		bl bl_ec_fieldInv

	// Calculate u_1 = zw \pmod{n}
		add	r1, sp, #8
		add	r2, sp, #304	//; 0x130
		movs	r3, #8
		mov	r0, r6

		bl bl_ec_fieldMult
		add	r0, sp, #304	//; 0x130
		add	r1, sp, #232	//; 0xe8
		movs	r2, #16
		bl bl_ec_fieldModO

	// 4. Calculate u_2 = rw \pmod{n}
		add	r1, sp, #8
		add	r2, sp, #304	//; 0x130
		movs	r3, #8
		mov	r0, r4
		bl bl_ec_fieldMult
		add	r7, sp, #72	//; 0x48
		add	r0, sp, #304	//; 0x130
		add	r1, sp, #268	//; 0x10c
		movs	r2, #16
		bl bl_ec_fieldModO

	// Calculate the curve point (x_1, y_1) = u_1 * G + u_2 * Q_A.
	// tmp1 = u_1 * G
		add	r6, sp, #136	//; 0x88
		add	r2, sp, #232	//; 0xe8
		add	r3, sp, #40	//; 0x28
		str	r7, [sp, #0]
		adr	r0, bl_data_ecdsa_g_point_x
		adr	r1, bl_data_ecdsa_g_point_y
		bl bl_ec_mult

	// tmp2 = u_2 * Q_A
		mov	r0, r5
		add.w	r1, r5, #32
		add	r2, sp, #268	//; 0x10c
		add	r3, sp, #104	//; 0x68
		str	r6, [sp, #0]
		bl bl_ec_mult

	// tmp3 = tmp1 + tmp2
		add	r5, sp, #168	//; 0xa8
		add	r3, sp, #200	//; 0xc8
		str	r3, [sp, #4]
		add	r0, sp, #40	//; 0x28
		mov	r1, r7
		add	r2, sp, #104	//; 0x68
		mov	r3, r6
		str	r5, [sp, #0]
		bl bl_ec_ec_add

	// issame(tmp3_x, r)
		mov	r0, r5
		mov	r1, r4
		movs	r2, #32
		bl bl_memcmp
		adds	r0, #0
		it	ne
		movne	r0, #1
		negs	r0, r0

	add	sp, #372	//; 0x174
	pop	{r4, r5, r6, r7, pc}


// int bl_ecdsa_validate2(const uint32_t *x, const uint32_t *y, const uint32_t *e, const uint32_t *r, const uint32_t *s);

.global bl_ecdsa_validate2
.type bl_ecdsa_validate2, %function
bl_ecdsa_validate2:
		stmdb	sp!, {r4, r5, r6, r7, r8, lr}
		sub	sp, #368	// 0x170
		
		mov	r4, r3
		mov	r8, r0	// x
		mov	r5, r1	// y
		mov	r6, r2 // e

		// Calculate w = s^{-1} \pmod{n}
		ldr	r0, [sp, #392]	// 0x188
		adr	r1, bl_data_ecdsa_order_m
		adr	r2, bl_data_ecdsa_order_r
		add	r3, sp, #8
		bl	bl_ec_fieldInv

		// Calculate u_1 = zw \pmod{n}
		add	r1, sp, #8
		add	r2, sp, #304	// 0x130
		movs	r3, #8
		mov	r0, r6
		bl	bl_ec_fieldMult

		add	r0, sp, #304	// 0x130
		add	r1, sp, #232	// 0xe8
		movs	r2, #16
		bl	bl_ec_fieldModO

		// 4. Calculate u_2 = rw \pmod{n}
		add	r1, sp, #8
		add	r2, sp, #304	// 0x130
		movs	r3, #8
		mov	r0, r4
		bl	bl_ec_fieldMult

		add	r7, sp, #72	// 0x48
		add	r0, sp, #304	// 0x130
		add	r1, sp, #268	// 0x10c
		movs	r2, #16
		bl	bl_ec_fieldModO

		// Calculate the curve point (x_1, y_1) = u_1 * G + u_2 * Q_A.
		// tmp1 = u_1 * G
		add	r6, sp, #136	// 0x88
		add	r2, sp, #232	// 0xe8
		add	r3, sp, #40	// 0x28
		str	r7, [sp, #0]
		adr	r0, bl_data_ecdsa_g_point_x
		adr	r1, bl_data_ecdsa_g_point_y
		bl	bl_ec_mult

		// tmp2 = u_2 * Q_A
		mov	r0, r8
		mov	r1, r5
		add	r2, sp, #268	// 0x10c
		add	r3, sp, #104	// 0x68
		str	r6, [sp, #0]
		bl	bl_ec_mult

		// tmp3 = tmp1 + tmp2
		add	r5, sp, #168	// 0xa8
		add	r3, sp, #200	// 0xc8
		str	r3, [sp, #4]
		add	r0, sp, #40	// 0x28
		mov	r1, r7
		add	r2, sp, #104	// 0x68
		mov	r3, r6
		str	r5, [sp, #0]
		bl	bl_ec_ec_add

		// issame(tmp3_x, r)
		mov	r0, r5
		mov	r1, r4
		movs	r2, #32
		bl	bl_memcmp

		adds	r0, #0
		it	ne
		movne	r0, #1
		negs	r0, r0
		add	sp, #368	// 0x170
		ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}

.type bl_ec_add, %function
bl_ec_add:
		stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
		mov	lr, r3
		movs	r6, #0
		movs	r3, #0
		movs	r7, #0

	1:
		cmp	r3, lr
		bge.n	0f
		ldr.w	r4, [r1, r3, lsl #2]
		mov	r8, r4
		ldr.w	r4, [r0, r3, lsl #2]
		mov.w	r9, #0
		adds.w	r4, r8, r4
		adc.w	r5, r9, #0
		adds.w	r8, r4, r6
		adc.w	r9, r5, r7
		str.w	r8, [r2, r3, lsl #2]
		mov	r6, r9
		movs	r7, #0
		adds	r3, #1
		b.n	1b
	0:
		mov	r0, r6
		ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}

.type bl_ec_sub, %function
bl_ec_sub:
		stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
		mov	lr, r3
		mov.w	r8, #0
		movs	r3, #0
		mov.w	r9, #0
	1:
		cmp	r3, lr
		bge.n	0f
		ldr.w	r5, [r1, r3, lsl #2]
		ldr.w	r4, [r0, r3, lsl #2]
		subs	r4, r4, r5
		sbc.w	r5, r4, r4
		subs.w	r4, r4, r8
		sbc.w	r5, r5, r9
		str.w	r4, [r2, r3, lsl #2]
		and.w	r8, r5, #1
		mov.w	r9, #0
		adds	r3, #1
		b.n	1b

	0:
		mov	r0, r8
		ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}

.type bl_ec_rshiftby, %function 
bl_ec_rshiftby:
		push	{r4, r5, r6, lr}
		ldrb.w	r5, [sp, #16]
		movs	r4, #0
		add.w	r0, r0, r5, lsl #2
	2:
		subs	r6, r1, r5
		cmp	r4, r6
		bge.n	0f
		cmp	r4, r3
		bge.n	0f
		ldr.w	r6, [r0, r4, lsl #2]
		str.w	r6, [r2, r4, lsl #2]
		adds	r4, #1
		b.n	2b
	0:
		cmp	r4, r3
		bge.n	1f
		movs	r1, #0
		str.w	r1, [r2, r4, lsl #2]
		adds	r4, #1
		b.n	0b
	1:
		pop	{r4, r5, r6, pc}


.type bl_ec_isGreater, %function
bl_ec_isGreater:
		push	{r4, lr}
	3:
		adds.w	r2, r2, #4294967295
		bcc.n	1f
		ldr.w	r4, [r0, r2, lsl #2]
		ldr.w	r3, [r1, r2, lsl #2]
		cmp	r4, r3
		bhi.n	2f
		bcs.n	3b
		mov.w	r0, #4294967295
		pop	{r4, pc}
	1:
		movs	r0, #0
		pop	{r4, pc}
	2:
		movs	r0, #1
		pop	{r4, pc}

.type bl_ec_isOne, %function
bl_ec_isOne:
		mov	r3, r0
		add.w	r2, r0, #28
	1:
		ldr.w	r1, [r3, #4]!
		cbnz	r1, 0f
		cmp	r3, r2
		bne.n	1b
		ldr	r0, [r0, #0]
		subs	r3, r0, #1
		negs	r0, r3
		adcs	r0, r3
		bx	lr
	0:
		movs	r0, #0
		bx	lr

.type bl_ec_isZero, %function
bl_ec_isZero:
		subs	r2, r0, #4
		add.w	r3, r0, #28
		movs	r0, #0
	1:
		ldr.w	r1, [r2, #4]!
		cbnz	r1, 0f
		adds	r0, #1
		uxtb	r0, r0
	0:
			cmp	r2, r3
			bne.n	1b
			sub.w	r3, r0, #8
			negs	r0, r3
			adcs	r0, r3
			bx	lr

.type bl_ec_rshift, %function
bl_ec_rshift:
		add.w	r2, r0, #32
		movs	r3, #0
	1:
		cmp	r2, r0
		beq.n	0f
		ldr.w	r1, [r2, #-4]!
		lsls	r3, r3, #31
		orr.w	r3, r3, r1, lsr #1
		str	r3, [r2, #0]
		and.w	r3, r1, #1
		b.n	1b
	0:
		bx	lr

.type bl_ec_setZero, %function
bl_ec_setZero:
		lsls	r2, r1, #2
		movs	r1, #0
		b.w	bl_memset

.type bl_ec_fieldMult, %function
bl_ec_fieldMult:
		stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
		mov	sl, r3
		sxth	r3, r3
		sub	sp, #20
		mov.w	r9, r3, lsl #1
		lsls	r3, r3, #3
		adds	r3, #8
		add	r7, sp, #0
		sub.w	sp, sp, r3
		str	r0, [r7, #0]
		mov	fp, r1
		mov	r0, sp
		mov	r1, r9
		str	r2, [r7, #12]
		bl	bl_ec_setZero
		ldr	r0, [r7, #12]
		mov	r1, r9
		bl	bl_ec_setZero
		mov.w	r3, sl, lsl #1
		uxtb	r3, r3
		mov	r8, sp
		str	r3, [r7, #4]
		movs	r4, #0
	3:
		cmp	r4, sl
		uxtb	r3, r4
		beq.n	0f
		ldr	r2, [r7, #4]
		subs	r3, r2, r3
		uxtb	r3, r3
		str	r3, [r7, #8]
		add.w	r6, r8, r4, lsl #2
		movs	r5, #0
	2:
		cmp	r5, sl
		beq.n	1f
		ldr	r3, [r7, #0]
		ldr.w	r2, [fp, r4, lsl #2]
		ldr.w	r3, [r3, r5, lsl #2]
		umull	r2, r3, r3, r2
		stmia.w	r6, {r2, r3}
		ldr	r3, [r7, #12]

		// uxtab	r1, r5, r4
		// [Error] bl_ecdsa.S@443,0: selected processor does not support `uxtab r1,r5,r4' in Thumb mode
		// only on cortex-m3, not m4!

		// http://www.keil.com/support/man/docs/armasm/armasm_dom1361289923867.htm
		// r1 := (r4 & 0xFF) + r5

		and r1, r4, #0xFF
		add r1, r5


		add.w	r1, r3, r1, lsl #2
		ldr	r3, [r7, #8]
		subs	r3, r3, r5
		mov	r2, r1
		mov	r0, r6
		uxtb	r3, r3
		bl	bl_ec_add
		mov	r0, r8
		mov	r1, r9
		bl	bl_ec_setZero
		adds	r5, #1
		adds	r6, #4
		b.n 2b
	1:
		adds	r4, #1
		b.n	3b
	0:
		movs	r0, #0
		adds	r7, #20
		mov	sp, r7
		ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
			
.type bl_ec_fieldModO, %function
bl_ec_fieldModO:
		push	{r4, r5, r6, lr}
		cmp	r2, #8
		sub	sp, #120	// 0x78
		mov	r5, r0
		mov	r4, r1
		mov	r6, r2
		bne.n	0f
		adr	r1, bl_data_ecdsa_order_m
		bl	bl_ec_isGreater
		cmp	r0, #0
		bgt.n	0f
		cmp	r5, r4
		beq.n	1f
		mov	r0, r4
		mov	r1, r5
		movs	r2, #32
		bl	bl_memcpy
		b.n	1f
	0:
		movs	r3, #7
		mov	r1, r6
		str	r3, [sp, #0]
		mov	r0, r5
		add	r2, sp, #12
		movs	r3, #9
		bl	bl_ec_rshiftby
		movs	r6, #9
		add	r1, sp, #12
		add	r2, sp, #48	// 0x30
		movs	r3, #9
		adr	r0, bl_data_ecdsa_order_mu
		bl	bl_ec_fieldMult
		add	r0, sp, #48	// 0x30
		movs	r1, #18
		add	r2, sp, #12
		movs	r3, #8
		str	r6, [sp, #0]
		bl	bl_ec_rshiftby
		adr	r1, bl_data_ecdsa_order_m
		add	r2, sp, #48	// 0x30
		movs	r3, #8
		add	r0, sp, #12
		bl	bl_ec_fieldMult
		mov	r0, r5
		add	r1, sp, #48	// 0x30
		mov	r2, r4
		mov	r3, r6
	2:
		bl	bl_ec_sub
		mov	r0, r4
		adr	r1, bl_data_ecdsa_order_m
		movs	r2, #9
		bl	bl_ec_isGreater
		cmp	r0, #0
		blt.n	1f
		mov	r0, r4
		adr	r1, bl_data_ecdsa_order_m
		mov	r2, r4
		movs	r3, #9
		b.n	2b
	1:
		add	sp, #120	// 0x78
		pop	{r4, r5, r6, pc}

.type bl_ec_fieldSub, %function
bl_ec_fieldSub:
		push	{r4, r5, lr}
		mov	r5, r2
		sub	sp, #36	// 0x24
		mov	r4, r3
		mov	r2, r3
		movs	r3, #8
		bl	bl_ec_sub
		cbz	r0, 0f
		mov	r0, sp
		movs	r1, #8
		bl	bl_ec_setZero
		mov	r1, r5
		mov	r2, sp
		mov	r0, r4
		movs	r3, #8
		bl	bl_ec_add
		mov	r0, r4
		mov	r1, sp
		movs	r2, #32
		bl	bl_memcpy
	0:
		movs	r0, #0
		add	sp, #36	// 0x24
		pop	{r4, r5, pc}


.type bl_ec_fieldAdd, %function
bl_ec_fieldAdd:
		push	{r4, r5, lr}
		mov	r5, r2
		sub	sp, #36	// 0x24
		mov	r4, r3
		mov	r2, r3
		movs	r3, #8
		bl	bl_ec_add
		cbz	r0, 0f
		mov	r0, sp
		movs	r1, #8
		bl	bl_ec_setZero
		mov	r1, r5
		mov	r2, sp
		mov	r0, r4
		movs	r3, #8
		bl	bl_ec_add
		mov	r0, r4
		mov	r1, sp
		movs	r2, #32
		bl	bl_memcpy
	0:
		movs	r0, #0
		add	sp, #36	// 0x24
		pop	{r4, r5, pc}

.type bl_ec_fieldModP, %function
bl_ec_fieldModP:
		push	{r4, r5, r6, lr}
		sub	sp, #64	// 0x40
		mov	r4, r1
		mov	r6, r0
		movs	r1, #8
		mov	r0, sp
		bl	bl_ec_setZero
		add	r0, sp, #32
		movs	r1, #8
		bl	bl_ec_setZero
		mov	r1, r4
		movs	r2, #32
		mov	r0, r6
		bl	bl_memcpy
		add	r3, sp, #64	// 0x40
		movs	r2, #0
		str	r2, [sp, #0]
		str	r2, [sp, #4]
		str.w	r2, [r3, #-56]!
		add.w	r1, r4, #60	// 0x3c
		add.w	r2, r4, #40	// 0x28
	0:
		ldr.w	r0, [r2, #4]!
		str.w	r0, [r3, #4]!
		cmp	r2, r1
		bne.n	0b
		mov	r1, sp
		adr	r2, bl_data_ecdsa_prime_r
		add	r3, sp, #32
		mov	r0, r6
		bl	bl_ec_fieldAdd
		mov	r1, sp
		adr	r2, bl_data_ecdsa_prime_r
		mov	r3, r6
		add	r0, sp, #32
		bl	bl_ec_fieldAdd
		ldr	r3, [r4, #48]	// 0x30
		str	r3, [sp, #12]
		ldr	r3, [r4, #52]	// 0x34
		str	r3, [sp, #16]
		ldr	r3, [r4, #56]	// 0x38
		str	r3, [sp, #20]
		ldr	r3, [r4, #60]	// 0x3c
		str	r3, [sp, #24]
		movs	r5, #0
		mov	r1, sp
		adr	r2, bl_data_ecdsa_prime_r
		str	r5, [sp, #0]
		add	r3, sp, #32
		mov	r0, r6
		str	r5, [sp, #4]
		str	r5, [sp, #8]
		str	r5, [sp, #28]
		bl	bl_ec_fieldAdd
		mov	r1, sp
		adr	r2, bl_data_ecdsa_prime_r
		mov	r3, r6
		add	r0, sp, #32
		bl	bl_ec_fieldAdd
		ldr	r3, [r4, #32]
		str	r3, [sp, #0]
		ldr	r3, [r4, #36]	// 0x24
		str	r3, [sp, #4]
		ldr	r3, [r4, #40]	// 0x28
		str	r3, [sp, #8]
		ldr	r3, [r4, #56]	// 0x38
		str	r3, [sp, #24]
		ldr	r3, [r4, #60]	// 0x3c
		str	r3, [sp, #28]
		mov	r1, sp
		adr	r2, bl_data_ecdsa_prime_r
		str	r5, [sp, #12]
		add	r3, sp, #32
		mov	r0, r6
		str	r5, [sp, #16]
		str	r5, [sp, #20]
		bl	bl_ec_fieldAdd
		ldr	r3, [r4, #36]	// 0x24
		str	r3, [sp, #0]
		ldr	r3, [r4, #40]	// 0x28
		str	r3, [sp, #4]
		ldr	r3, [r4, #44]	// 0x2c
		ldr	r2, [r4, #56]	// 0x38
		str	r3, [sp, #8]
		ldr	r3, [r4, #52]	// 0x34
		str	r3, [sp, #12]
		str	r2, [sp, #16]
		str	r3, [sp, #24]
		ldr	r2, [r4, #60]	// 0x3c
		ldr	r3, [r4, #32]
		str	r2, [sp, #20]
		str	r3, [sp, #28]
		mov	r1, sp
		adr	r2, bl_data_ecdsa_prime_r
		mov	r3, r6
		add	r0, sp, #32
		bl	bl_ec_fieldAdd
		ldr	r3, [r4, #44]	// 0x2c
		str	r3, [sp, #0]
		ldr	r3, [r4, #48]	// 0x30
		str	r3, [sp, #4]
		ldr	r3, [r4, #52]	// 0x34
		str	r3, [sp, #8]
		ldr	r3, [r4, #32]
		str	r3, [sp, #24]
		ldr	r3, [r4, #40]	// 0x28
		str	r3, [sp, #28]
		mov	r1, sp
		adr	r2, bl_data_ecdsa_prime_m
		str	r5, [sp, #12]
		add	r3, sp, #32
		mov	r0, r6
		str	r5, [sp, #16]
		str	r5, [sp, #20]
		bl	bl_ec_fieldSub
		ldr	r3, [r4, #48]	// 0x30
		str	r3, [sp, #0]
		ldr	r3, [r4, #52]	// 0x34
		str	r3, [sp, #4]
		ldr	r3, [r4, #56]	// 0x38
		str	r3, [sp, #8]
		ldr	r3, [r4, #60]	// 0x3c
		str	r3, [sp, #12]
		ldr	r3, [r4, #36]	// 0x24
		str	r3, [sp, #24]
		ldr	r3, [r4, #44]	// 0x2c
		str	r3, [sp, #28]
		mov	r1, sp
		adr	r2, bl_data_ecdsa_prime_m
		str	r5, [sp, #16]
		mov	r3, r6
		add	r0, sp, #32
		str	r5, [sp, #20]
		bl	bl_ec_fieldSub
		ldr	r3, [r4, #52]	// 0x34
		str	r3, [sp, #0]
		ldr	r3, [r4, #56]	// 0x38
		str	r3, [sp, #4]
		ldr	r3, [r4, #60]	// 0x3c
		str	r3, [sp, #8]
		ldr	r3, [r4, #32]
		str	r3, [sp, #12]
		ldr	r3, [r4, #36]	// 0x24
		str	r3, [sp, #16]
		ldr	r3, [r4, #40]	// 0x28
		str	r3, [sp, #20]
		ldr	r3, [r4, #48]	// 0x30
		str	r3, [sp, #28]
		mov	r1, sp
		adr	r2, bl_data_ecdsa_prime_m
		str	r5, [sp, #24]
		add	r3, sp, #32
		mov	r0, r6
		bl	bl_ec_fieldSub
		ldr	r3, [r4, #56]	// 0x38
		str	r3, [sp, #0]
		ldr	r3, [r4, #60]	// 0x3c
		str	r3, [sp, #4]
		ldr	r3, [r4, #36]	// 0x24
		str	r3, [sp, #12]
		ldr	r3, [r4, #40]	// 0x28
		str	r3, [sp, #16]
		ldr	r3, [r4, #44]	// 0x2c
		str	r3, [sp, #20]
		ldr	r3, [r4, #52]	// 0x34
		str	r3, [sp, #28]
		mov	r1, sp
		adr	r2, bl_data_ecdsa_prime_m
		str	r5, [sp, #8]
		mov	r3, r6
		add	r0, sp, #32
		str	r5, [sp, #24]
		bl	bl_ec_fieldSub
		mov	r0, r6
		adr	r1, bl_data_ecdsa_prime_m
		movs	r2, #8
		bl	bl_ec_isGreater
		cmp	r0, r5
		blt.n	1f
		adr	r1, bl_data_ecdsa_prime_m
		mov	r0, r6
		mov	r2, r1
		mov	r3, sp
		bl	bl_ec_fieldSub
		mov	r0, r6
		mov	r1, sp
		movs	r2, #32
		bl	bl_memcpy
	1:
		add	sp, #64	// 0x40
		pop	{r4, r5, r6, pc}

.type bl_ec_fieldAddAndDivide, %function
bl_ec_fieldAddAndDivide:
		push	{r4, r5, r6, r7, lr}
		mov	r4, r3
		sub	sp, #36	// 0x24
		mov	r5, r2
		mov	r2, r3
		movs	r3, #8
		mov	r6, r1
		bl	bl_ec_add
		mov	r7, r0
		mov	r0, r4
		bl	bl_ec_rshift
		cbz	r7, 0f
		ldr	r3, [r4, #28]
		orr.w	r3, r3, #2147483648	// 0x80000000
		str	r3, [r4, #28]
		mov	r0, r4
		mov	r1, r6
		movs	r2, #8
		bl	bl_ec_isGreater
		cmp	r0, #1
		bne.n	0f
		mov	r0, sp
		movs	r1, #8
		bl	bl_ec_setZero
		mov	r1, r5
		mov	r2, sp
		mov	r0, r4
		movs	r3, #8
		bl	bl_ec_add
		mov	r0, r4
		mov	r1, sp
		movs	r2, #32
		bl	bl_memcpy
	0:
		movs	r0, #0
		add	sp, #36	// 0x24
		pop	{r4, r5, r6, r7, pc}

.type bl_ec_fieldInv, %function
bl_ec_fieldInv:
		push	{r4, r5, r6, r7, lr}
		sub	sp, #196	// 0xc4
		mov	r7, r0
		mov	r4, r1
		add	r0, sp, #128	// 0x80
		movs	r1, #8
		mov	r5, r3
		mov	r6, r2
		bl	bl_ec_setZero
		add	r0, sp, #160	// 0xa0
		movs	r1, #8
		bl	bl_ec_setZero
		mov	r0, sp
		movs	r1, #8
		bl	bl_ec_setZero
		add	r0, sp, #32
		movs	r1, #8
		bl	bl_ec_setZero
		mov	r1, r7
		movs	r2, #32
		mov	r0, sp
		bl	bl_memcpy
		movs	r2, #32
		mov	r1, r4
		add	r0, sp, #32
		bl	bl_memcpy
		add	r0, sp, #64	// 0x40
		movs	r1, #8
		bl	bl_ec_setZero
		add	r0, sp, #96	// 0x60
		movs	r1, #8
		bl	bl_ec_setZero
		movs	r3, #1
		str	r3, [sp, #64]	// 0x40
	10:
		mov	r0, sp
		bl	bl_ec_isOne
		cmp	r0, #0
		bne.n	0f
		add	r0, sp, #32
		bl	bl_ec_isOne
		cmp	r0, #0
		bne.n	1f
	4:
		ldr	r3, [sp, #0]
		lsls	r1, r3, #31
		bmi.n	2f
		mov	r0, sp
		bl	bl_ec_rshift
		ldr	r3, [sp, #64]	// 0x40
		lsls	r7, r3, #31
		add	r0, sp, #64	// 0x40
		bmi.n	3f
		bl	bl_ec_rshift
		b.n	4b
	3:
		mov	r1, r4
		mov	r2, r6
		add	r3, sp, #128	// 0x80
		bl	bl_ec_fieldAddAndDivide
		add	r0, sp, #64	// 0x40
		add	r1, sp, #128	// 0x80
		movs	r2, #32
		bl	bl_memcpy
		b.n	4b
	6:
		add	r0, sp, #32
		bl	bl_ec_rshift
		ldr	r3, [sp, #96]	// 0x60
		lsls	r2, r3, #31
		add	r0, sp, #96	// 0x60
		bmi.n	5f
		bl	bl_ec_rshift
	2:
		ldr	r3, [sp, #32]
		lsls	r3, r3, #31
		bpl.n	6b
		b.n	7f
	5:
		mov	r1, r4
		mov	r2, r6
		add	r3, sp, #128	// 0x80
		bl	bl_ec_fieldAddAndDivide
		add	r0, sp, #96	// 0x60
		add	r1, sp, #128	// 0x80
		movs	r2, #32
		bl	bl_memcpy
		b.n	2b
	7:
		mov	r0, sp
		add	r1, sp, #32
		add	r2, sp, #128	// 0x80
		movs	r3, #8
		bl	bl_ec_sub
		tst.w	r0, #255	// 0xff
		bne.n	8f
		add	r1, sp, #128	// 0x80
		movs	r2, #32
		mov	r0, sp
		bl	bl_memcpy
		add	r0, sp, #64	// 0x40
		add	r1, sp, #96	// 0x60
		mov	r2, r4
		add	r3, sp, #128	// 0x80
		bl	bl_ec_fieldSub
		add	r0, sp, #64	// 0x40
		b.n	9f
	8:
		movs	r3, #8
		mov	r1, sp
		add	r2, sp, #128	// 0x80
		add	r0, sp, #32
		bl	bl_ec_sub
		add	r1, sp, #128	// 0x80
		movs	r2, #32
		add	r0, sp, #32
		bl	bl_memcpy
		add	r0, sp, #96	// 0x60
		add	r1, sp, #64	// 0x40
		mov	r2, r4
		add	r3, sp, #128	// 0x80
		bl	bl_ec_fieldSub
		add	r0, sp, #96	// 0x60
	9:
		add	r1, sp, #128	// 0x80
		movs	r2, #32
		bl	bl_memcpy
		b.n	10b
	1:
		mov	r0, r5
		add	r1, sp, #96	// 0x60
		b.n	11f
	0:
		mov	r0, r5
		add	r1, sp, #64	// 0x40
	11:
		movs	r2, #32
		bl	bl_memcpy
		add	sp, #196	// 0xc4
		pop	{r4, r5, r6, r7, pc}


.type bl_ec_double, %function
bl_ec_double:
		push	{r4, r5, r6, r7, lr}
		sub	sp, #164	// 0xa4
		mov	r4, r0
		mov	r5, r1
		mov	r6, r2
		mov	r7, r3
		bl	bl_ec_isZero
		cbz	r0, 1f
		mov	r0, r5
		bl	bl_ec_isZero
		cbz	r0, 1f
		mov	r1, r4
		mov	r0, r6
		movs	r2, #32
		bl	bl_memcpy
		mov	r0, r7
		mov	r1, r5
		movs	r2, #32
		bl	bl_memcpy
		b.n	0f
	1:
		add	r2, sp, #96	// 0x60
		movs	r3, #8
		mov	r1, r4
		mov	r0, r4
		bl	bl_ec_fieldMult
		mov	r0, sp
		add	r1, sp, #96	// 0x60
		bl	bl_ec_fieldModP
		add	r0, sp, #32
		movs	r1, #8
		bl	bl_ec_setZero
		movs	r3, #1
		str	r3, [sp, #32]
		add	r1, sp, #32
		adr	r2, bl_data_ecdsa_prime_m
		add	r3, sp, #64	// 0x40
		mov	r0, sp
		bl	bl_ec_fieldSub
		movs	r3, #3
		add	r2, sp, #96	// 0x60
		str	r3, [sp, #32]
		add	r1, sp, #32
		movs	r3, #8
		add	r0, sp, #64	// 0x40
		bl	bl_ec_fieldMult
		mov	r0, sp
		add	r1, sp, #96	// 0x60
		bl	bl_ec_fieldModP
		mov	r1, r5
		adr	r2, bl_data_ecdsa_prime_r
		add	r3, sp, #32
		mov	r0, r5
		bl	bl_ec_fieldAdd
		add	r0, sp, #32
		adr	r1, bl_data_ecdsa_prime_m
		adr	r2, bl_data_ecdsa_prime_r
		add	r3, sp, #64	// 0x40
		bl	bl_ec_fieldInv
		add	r2, sp, #96	// 0x60
		movs	r3, #8
		add	r1, sp, #64	// 0x40
		mov	r0, sp
		bl	bl_ec_fieldMult
		add	r0, sp, #32
		add	r1, sp, #96	// 0x60
		bl	bl_ec_fieldModP
		add	r0, sp, #32
		add	r2, sp, #96	// 0x60
		movs	r3, #8
		mov	r1, r0
		bl	bl_ec_fieldMult
		add	r0, sp, #64	// 0x40
		add	r1, sp, #96	// 0x60
		bl	bl_ec_fieldModP
		mov	r1, r4
		adr	r2, bl_data_ecdsa_prime_m
		mov	r3, sp
		add	r0, sp, #64	// 0x40
		bl	bl_ec_fieldSub
		mov	r1, r4
		adr	r2, bl_data_ecdsa_prime_m
		mov	r3, r6
		mov	r0, sp
		bl	bl_ec_fieldSub
		mov	r1, r6
		adr	r2, bl_data_ecdsa_prime_m
		mov	r3, sp
		mov	r0, r4
		bl	bl_ec_fieldSub
		add	r2, sp, #96	// 0x60
		movs	r3, #8
		mov	r1, sp
		add	r0, sp, #32
		bl	bl_ec_fieldMult
		add	r0, sp, #64	// 0x40
		add	r1, sp, #96	// 0x60
		bl	bl_ec_fieldModP
		add	r0, sp, #64	// 0x40
		mov	r1, r5
		adr	r2, bl_data_ecdsa_prime_m
		mov	r3, r7
		bl	bl_ec_fieldSub
	0:
		add	sp, #164	// 0xa4
		pop	{r4, r5, r6, r7, pc}

.type bl_ec_ec_add, %function
bl_ec_ec_add:
		push	{r4, r5, r6, r7, lr}
		sub	sp, #164	// 0xa4
		mov	r6, r0
		mov	r7, r1
		mov	r4, r2
		mov	r5, r3
		bl	bl_ec_isZero
		cbz	r0, 0f
		mov	r0, r7
		bl	bl_ec_isZero
		cbz	r0, 0f
		mov	r1, r4
		ldr	r0, [sp, #184]	// 0xb8
		movs	r2, #32
		bl	bl_memcpy
		ldr	r0, [sp, #188]	// 0xbc
		mov	r1, r5
		b.n	1f
	0:
		mov	r0, r4
		bl	bl_ec_isZero
		cbz	r0, 2f
		mov	r0, r5
		bl	bl_ec_isZero
		cbz	r0, 2f
		mov	r1, r6
		ldr	r0, [sp, #184]	// 0xb8
		movs	r2, #32
		bl	bl_memcpy
		ldr	r0, [sp, #188]	// 0xbc
		mov	r1, r7
	1:
		movs	r2, #32
		bl	bl_memcpy
		b.n	3f
	2:
		mov	r0, r6
		mov	r1, r4
		movs	r2, #32
		bl	bl_memcmp
		cbnz	r0, 4f
		mov	r0, r7
		mov	r1, r5
		movs	r2, #32
		bl	bl_memcmp
		cbz	r0, 5f
		ldr	r0, [sp, #184]	// 0xb8
		movs	r1, #8
		bl	bl_ec_setZero
		ldr	r0, [sp, #188]	// 0xbc
		movs	r1, #8
		bl	bl_ec_setZero
		b.n	3f
	5:
		mov	r0, r6
		mov	r1, r7
		ldr	r2, [sp, #184]	// 0xb8
		ldr	r3, [sp, #188]	// 0xbc
		bl	bl_ec_double
		b.n	3f
	4:
		mov	r1, r5
		adr	r2, bl_data_ecdsa_prime_m
		mov	r3, sp
		mov	r0, r7
		bl	bl_ec_fieldSub
		mov	r1, r4
		adr	r2, bl_data_ecdsa_prime_m
		add	r3, sp, #32
		mov	r0, r6
		bl	bl_ec_fieldSub
		add	r0, sp, #32
		mov	r3, r0
		adr	r1, bl_data_ecdsa_prime_m
		adr	r2, bl_data_ecdsa_prime_r
		bl	bl_ec_fieldInv
		add	r2, sp, #96	// 0x60
		movs	r3, #8
		add	r1, sp, #32
		mov	r0, sp
		bl	bl_ec_fieldMult
		add	r0, sp, #64	// 0x40
		add	r1, sp, #96	// 0x60
		bl	bl_ec_fieldModP
		add	r0, sp, #64	// 0x40
		add	r2, sp, #96	// 0x60
		movs	r3, #8
		mov	r1, r0
		bl	bl_ec_fieldMult
		mov	r0, sp
		add	r1, sp, #96	// 0x60
		bl	bl_ec_fieldModP
		mov	r1, r6
		adr	r2, bl_data_ecdsa_prime_m
		add	r3, sp, #32
		mov	r0, sp
		bl	bl_ec_fieldSub
		mov	r1, r4
		adr	r2, bl_data_ecdsa_prime_m
		ldr	r3, [sp, #184]	// 0xb8
		add	r0, sp, #32
		bl	bl_ec_fieldSub
		ldr	r1, [sp, #184]	// 0xb8
		adr	r2, bl_data_ecdsa_prime_m
		add	r3, sp, #32
		mov	r0, r4
		bl	bl_ec_fieldSub
		add	r2, sp, #96	// 0x60
		movs	r3, #8
		add	r1, sp, #32
		add	r0, sp, #64	// 0x40
		bl	bl_ec_fieldMult
		add	r0, sp, #64	// 0x40
		add	r1, sp, #96	// 0x60
		bl	bl_ec_fieldModP
		add	r0, sp, #64	// 0x40
		mov	r1, r5
		adr	r2, bl_data_ecdsa_prime_m
		ldr	r3, [sp, #188]	// 0xbc
		bl	bl_ec_fieldSub
	3:
		add	sp, #164	// 0xa4
		pop	{r4, r5, r6, r7, pc}

.type bl_ec_mult, %function
bl_ec_mult:
		stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
		sub	sp, #136	// 0x88
		mov	r8, r0
		mov	r9, r1
		add	r0, sp, #8
		movs	r1, #8
		mov	sl, r2
		mov	r7, r3
		bl	bl_ec_setZero
		add	r0, sp, #40	// 0x28
		movs	r1, #8
		bl	bl_ec_setZero
		mov.w	r4, #256	// 0x100
	1:
		adds.w	r4, r4, #4294967295
		bcc.n	0f
		add	r6, sp, #72	// 0x48
		add	r5, sp, #104	// 0x68
		mov	r3, r5
		add	r0, sp, #8
		add	r1, sp, #40	// 0x28
		mov	r2, r6
		bl	bl_ec_double
		mov	r1, r6
		movs	r2, #32
		add	r0, sp, #8
		bl	bl_memcpy
		movs	r2, #32
		add	r0, sp, #40	// 0x28
		mov	r1, r5
		bl	bl_memcpy
		asrs	r3, r4, #5
		and.w	r2, r4, #31
		ldr.w	r3, [sl, r3, lsl #2]
		lsrs	r3, r2
		lsls	r3, r3, #31
		bpl.n	1b
		add	r0, sp, #8
		add	r1, sp, #40	// 0x28
		mov	r2, r8
		mov	r3, r9
		str	r6, [sp, #0]
		str	r5, [sp, #4]
		bl	bl_ec_ec_add
		mov	r1, r6
		add	r0, sp, #8
		movs	r2, #32
		bl	bl_memcpy
		add	r0, sp, #40	// 0x28
		mov	r1, r5
		movs	r2, #32
		bl	bl_memcpy
		b.n	1b
	0:
		add	r1, sp, #8
		mov	r0, r7
		movs	r2, #32
		bl	bl_memcpy
		ldr	r0, [sp, #168]	// 0xa8
		add	r1, sp, #40	// 0x28
		movs	r2, #32
		bl	bl_memcpy
		add	sp, #136	// 0x88
		ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}

.align 4

bl_data_ecdsa_prime_m: /*0x0806d310*/	.word 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xffffffff
bl_data_ecdsa_prime_r: /*0x0806d354*/	.word 0x00000001, 0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffe, 0x00000000
bl_data_ecdsa_order_mu: /*0x0806d2ec*/	.word 0xEEDF9BFE, 0x012FFD85, 0xDF1A6C21, 0x43190552, 0xFFFFFFFF, 0xFFFFFFFE, 0xFFFFFFFF, 0x00000000, 0x00000001
bl_data_ecdsa_order_m: /* 0x0806d330 */ .word 0xFC632551, 0xF3B9CAC2, 0xA7179E84, 0xBCE6FAAD, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000
bl_data_ecdsa_order_r: /*0x0806d374*/	.word 0x039CDAAF, 0x0C46353D, 0x58E8617B, 0x43190552, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000
bl_data_ecdsa_g_point_x: /*0x0806d394*/ .word  0xD898C296, 0xF4A13945, 0x2DEB33A0, 0x77037D81, 0x63A440F2, 0xF8BCE6E5, 0xE12C4247, 0x6B17D1F2
bl_data_ecdsa_g_point_y: /*0x0806d3b4*/ .word 0x37BF51F5, 0xCBB64068, 0x6B315ECE, 0x2BCE3357, 0x7C0F9E16, 0x8EE7EB4A, 0xFE1A7F9B, 0x4FE342E2
