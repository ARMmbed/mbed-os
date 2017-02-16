/******************************************************************************
 *
 * Copyright(c) 2007 - 2014 Realtek Corporation. All rights reserved.
 *                                        
 * This is ROM code section. 
 *
 *
 ******************************************************************************/

#ifndef ROM_MD5_H
#define ROM_MD5_H

#if PSK_SUPPORT_TKIP

/* MD5 context. */
typedef struct {
  u32 state[4];                                   /* state (ABCD) */
  u32 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  u8 buffer[64];                         /* input buffer */
} md5_ctx;

void rt_md5_init(md5_ctx *context);
void rt_md5_append(md5_ctx *context, u8 *input, u32 inputLen);
void rt_md5_final(u8 digest[16], md5_ctx *context);
void rt_md5_hmac(unsigned char *text, int text_len, unsigned char *key,
		 int key_len, void * digest);


#endif	//#if PSK_SUPPORT_TKIP
#endif
