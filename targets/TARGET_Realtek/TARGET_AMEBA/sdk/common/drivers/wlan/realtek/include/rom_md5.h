/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************
 *                                        
 * This is ROM code section. 
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
