/*
 * bl_lib.S
 *
 * Created: 27.09.2017 14:43:53
 *  Author: Guenter.Prossliner
 */ 

#include "bl_asmoptions.inc"


/*************************************************************************
	
	void * bl_memcpy ( void * destination, const void * source, size_t num );
	
	<string.h> compatible memcpy
*************************************************************************/
BL_GLOBAL_FUNCTION(bl_memcpy):
	push	{r4, lr}
	subs	r3, r0, #1
	add	r2, r1
1:	
	cmp	r1, r2
	beq.n	0f
	ldrb.w	r4, [r1], #1
	strb.w	r4, [r3, #1]!
	b.n	1b
0:	
	pop	{r4, pc}


/*************************************************************************
	
	void * bl_memset ( void * ptr, int value, size_t num 
	
	<string.h> compatible memset
*************************************************************************/
BL_GLOBAL_FUNCTION(bl_memset):
	add	r2, r0
1:	
	cmp	r0, r2
	beq.n	0f
	strb.w	r1, [r0], #1
	b.n	1b
0:	
	bx	lr


/*************************************************************************
	
	int bl_memcmp ( const void * ptr1, const void * ptr2, size_t num );
	
	<string.h> compatible memcmp
*************************************************************************/
BL_GLOBAL_FUNCTION(bl_memcmp):
	push {r4, r5, lr}

		movs	r3, #0
	3:	
			cmp	r3, r2
			bge.n	1f	// return 0

			ldrb	r4, [r0, #0]
			ldrb	r5, [r1, #0]
			subs	r4, r4, r5
			cmp	r4, #0
			beq.n	2f	// continue
			blt.n	3f	// return -1
			b 0f	// return 0
		2:	
			adds	r3, #1
			b.n	3b
	1:	
		movs	r0, #0
		b 0f
	3:	
		mov.w	r0, #-1

	0:
	pop	{r4, r5, pc}
