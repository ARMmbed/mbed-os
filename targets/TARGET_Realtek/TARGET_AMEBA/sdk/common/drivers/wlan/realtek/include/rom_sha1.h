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
 ******************************************************************************/
#ifndef _ROM_SHA1_
#define _ROM_SHA1_


#ifndef _SHA_enum_
#define _SHA_enum_
enum
{
    shaSuccess = 0,
    shaNull,            /* Null pointer parameter */
    shaInputTooLong,    /* input data too long */
    shaStateError       /* called Input after Result */
};
#endif


#define SHA1HashSize 20

/*
 *  This structure will hold context information for the SHA-1
 *  hashing operation
 */
typedef struct SHA1Context
{
    u32 Intermediate_Hash[SHA1HashSize/4]; /* Message Digest  */

    u32 Length_Low;            /* Message length in bits      */
    u32 Length_High;           /* Message length in bits      */

                               /* Index into message block array   */
    u16 Message_Block_Index;
    u8 Message_Block[64];      /* 512-bit message blocks      */

    int Computed;               /* Is the digest computed?         */
    int Corrupted;             /* Is the message digest corrupted? */
} SHA1Context;


/*
 *  Function Prototypes
 */


int rt_sha1_init(  SHA1Context *);
int rt_sha1_update(  SHA1Context *, const u8 *, unsigned int);
int rt_sha1_finish( SHA1Context *, u8 Message_Digest[SHA1HashSize]);

void rt_hmac_sha1(unsigned char *text, int text_len, unsigned char *key,
		 int key_len, unsigned char *digest);


#endif //_ROM_SHA1_
