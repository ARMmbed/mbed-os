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
#ifndef ROM_AES_H
#define ROM_AES_H

typedef struct
{
    u32 erk[64];     				/* encryption round keys */
    u32 drk[64];     				/* decryption round keys */
    int nr;             					/* number of rounds */
}aes_context;


#define AES_BLOCKSIZE8	8
#define AES_BLK_SIZE    	16     	// # octets in an AES block
typedef union _aes_block  		// AES cipher block
{
    unsigned long  x[AES_BLK_SIZE/4];    	// access as 8-bit octets or 32-bit words
    unsigned char  b[AES_BLK_SIZE];
}aes_block;


void AES_WRAP(unsigned char * plain, int plain_len,
			  		unsigned char * iv,	int iv_len,
					unsigned char * kek,	int kek_len,
			  		unsigned char *cipher, unsigned short *cipher_len);

void AES_UnWRAP(unsigned char * cipher, int cipher_len,
			   		 unsigned char * kek,	int kek_len,
			    		unsigned char * plain);

#endif
