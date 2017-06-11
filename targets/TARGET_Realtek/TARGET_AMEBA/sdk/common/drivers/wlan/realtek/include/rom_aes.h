/******************************************************************************
 *
 * mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
