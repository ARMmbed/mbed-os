/******************************************************************************
 *
 * Copyright(c) 2007 - 2014 Realtek Corporation. All rights reserved.
 *                                        
 * This is ROM code section. 
 *
 *
 ******************************************************************************/
#ifndef ROM_ARC4_H
#define ROM_ARC4_H

struct arc4context
{
	u32 x;
	u32 y;
	u8 state[256];
};

u32 crc32_get(u8 *buf, sint len);
void rt_arc4_init(struct arc4context 	*parc4ctx, u8 * key,u32	key_len);
void rt_arc4_crypt(	struct arc4context	*parc4ctx, u8 * dest, u8 * src, u32 len);


#endif //ROM_ARC4_H
