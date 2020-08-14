/**************************************************************************************
* Copyright (c) 2016-2017, ARM Limited or its affiliates. All rights reserved         *
*                                                                                     *
* This file and the related binary are licensed under the following license:          *
*                                                                                     *
* ARM Object Code and Header Files License, v1.0 Redistribution.                      *
*                                                                                     *
* Redistribution and use of object code, header files, and documentation, without     *
* modification, are permitted provided that the following conditions are met:         *
*                                                                                     *
* 1) Redistributions must reproduce the above copyright notice and the                *
*    following disclaimer in the documentation and/or other materials                 *
*    provided with the distribution.                                                  *
*                                                                                     *
* 2) Unless to the extent explicitly permitted by law, no reverse                     *
*    engineering, decompilation, or disassembly of is permitted.                      *
*                                                                                     *
* 3) Redistribution and use is permitted solely for the purpose of                    *
*    developing or executing applications that are targeted for use                   *
*    on an ARM-based product.                                                         *
*                                                                                     *
* DISCLAIMER. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND                  *
* CONTRIBUTORS "AS IS." ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT             *
* NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT,        *
* AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE          *
* COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   *
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED            *
* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR              *
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF              *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING                *
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS                  *
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                        *
**************************************************************************************/



#ifndef _DRIVER_DEFS_H_
#define _DRIVER_DEFS_H_

#ifdef __KERNEL__
#include <linux/types.h>
#define INT32_MAX 0x7FFFFFFFL
#else
#include <stdint.h>
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

/******************************************************************************
*				TYPE DEFINITIONS
******************************************************************************/
typedef uint32_t drvError_t;

typedef enum aesMode {
	CIPHER_NULL_MODE = -1,
	CIPHER_ECB = 0,
	CIPHER_CBC = 1,
	CIPHER_CTR = 2,
	CIPHER_CBC_MAC = 3,
	CIPHER_CMAC = 7,
	CIPHER_RESERVE32B = INT32_MAX
}aesMode_t;

typedef enum hashMode {
	HASH_NULL_MODE = -1,
	HASH_SHA1 = 0,
	HASH_SHA256 = 1,
	HASH_SHA224 = 2,
	HASH_SHA512 = 3,
	HASH_SHA384 = 4,
	HASH_RESERVE32B = INT32_MAX
}hashMode_t;

typedef enum DataBlockType {
	FIRST_BLOCK,
	MIDDLE_BLOCK,
	LAST_BLOCK,
	RESERVE32B_BLOCK = INT32_MAX
}DataBlockType_t;

typedef enum dataAddrType {
	SRAM_ADDR = 0,
	DLLI_ADDR = 1,
	ADDR_RESERVE32B = INT32_MAX
}dataAddrType_t;

typedef enum cryptoDirection {
	CRYPTO_DIRECTION_ENCRYPT = 0,
	CRYPTO_DIRECTION_DECRYPT = 1,

	CRYPTO_DIRECTION_NUM_OF_ENC_MODES,
	CRYPTO_DIRECTION_RESERVE32B = INT32_MAX
}cryptoDirection_t;

typedef enum cryptoKeyType {
	RKEK_KEY = 0,
	USER_KEY = 1,
	PROVISIONING_KEY = 2,
	SESSION_KEY = 3,
	END_OF_KEYS = INT32_MAX,
}cryptoKeyType_t;

typedef enum cryptoPaddingType {
	CRYPTO_PADDING_NONE = 0,
	CRYPTO_PADDING_PKCS7 = 1,
	CRYPTO_PADDING_RESERVE32B = INT32_MAX
}cryptoPaddingType_t;

typedef enum chachaNonceSize {
        NONCE_SIZE_64 = 0,
        NONCE_SIZE_96 = 1,
        NONCE_SIZE_RESERVE32B = INT32_MAX
}chachaNonceSize_t;

/* The IOT drviers base address */
#define DRV_MODULE_ERROR_BASE				0x00F00000
#define AES_DRV_MODULE_ERROR_BASE			(DRV_MODULE_ERROR_BASE + 0x10000UL)
#define HASH_DRV_MODULE_ERROR_BASE			(DRV_MODULE_ERROR_BASE + 0x20000UL)
#define HMAC_DRV_MODULE_ERROR_BASE			(DRV_MODULE_ERROR_BASE + 0x30000UL)
#define BYPASS_DRV_MODULE_ERROR_BASE			(DRV_MODULE_ERROR_BASE + 0x40000UL)
#define CHACHA_DRV_MODULE_ERROR_BASE			(DRV_MODULE_ERROR_BASE + 0x50000UL)


/******************************************************************************
*				AES DEFINITIONS
******************************************************************************/

#define AES_BLOCK_SIZE                  16
#define AES_BLOCK_SIZE_WORDS            (AES_BLOCK_SIZE >> 2)
#define AES_IV_SIZE                     16
#define AES_IV_SIZE_WORDS               (AES_IV_SIZE >> 2)
#define AES_128_BIT_KEY_SIZE            16
#define AES_128_BIT_KEY_SIZE_WORDS	(AES_128_BIT_KEY_SIZE >> 2)


#define ENABLE_AES_CLOCK		0x1UL
#define DISABLE_AES_CLOCK		0x0UL

#define CONFIG_DIN_AES_DOUT_VAL	        0x1UL

/* The CRYS AES module errors */
#define AES_DRV_OK						0
#define AES_DRV_INVALID_USER_CONTEXT_POINTER_ERROR     		(AES_DRV_MODULE_ERROR_BASE + 0x00UL)
#define AES_DRV_ILLEGAL_OPERATION_MODE_ERROR           		(AES_DRV_MODULE_ERROR_BASE + 0x01UL)
#define AES_DRV_ILLEGAL_OPERATION_DIRECTION_ERROR          	(AES_DRV_MODULE_ERROR_BASE + 0x02UL)
#define AES_DRV_ILLEGAL_INPUT_ADDR_MEM_ERROR   			(AES_DRV_MODULE_ERROR_BASE + 0x03UL)
#define AES_DRV_ILLEGAL_OUTPUT_ADDR_MEM_ERROR   		(AES_DRV_MODULE_ERROR_BASE + 0x04UL)
#define AES_DRV_ILLEGAL_MEM_SIZE_ERROR   		   	(AES_DRV_MODULE_ERROR_BASE + 0x05UL)


/******************************************************************************
*				HASH & HMAC DEFINITIONS
******************************************************************************/

/************************ Typedefs  ****************************/
typedef drvError_t (*llf_hash_init_operation_func)(void *);
typedef drvError_t (*llf_hash_update_operation_func)(void *, uint32_t inputDataAddr, uint32_t dataInSize);
typedef drvError_t (*llf_hash_finish_operation_func)(void *);


/* The SHA-1 digest result size */
#define SHA1_DIGEST_SIZE_IN_WORDS 5
#define SHA1_DIGEST_SIZE_IN_BYTES (SHA1_DIGEST_SIZE_IN_WORDS * sizeof(uint32_t))

/* The SHA-256 digest result size*/
#define SHA224_DIGEST_SIZE_IN_WORDS 7
#define SHA224_DIGEST_SIZE_IN_BYTES (SHA224_DIGEST_SIZE_IN_WORDS * sizeof(uint32_t))

/* The SHA-256 digest result size */
#define SHA256_DIGEST_SIZE_IN_WORDS 8
#define SHA256_DIGEST_SIZE_IN_BYTES (SHA256_DIGEST_SIZE_IN_WORDS * sizeof(uint32_t))

/* The SHA-384 digest result size*/
#define SHA384_DIGEST_SIZE_IN_WORDS 12
#define SHA384_DIGEST_SIZE_IN_BYTES (SHA384_DIGEST_SIZE_IN_WORDS * sizeof(uint32_t))

/* The SHA-512 digest result size in bytes */
#define SHA512_DIGEST_SIZE_IN_WORDS 16
#define SHA512_DIGEST_SIZE_IN_BYTES (SHA512_DIGEST_SIZE_IN_WORDS * sizeof(uint32_t))


#define MAX_DIGEST_SIZE_WORDS 		SHA512_DIGEST_SIZE_IN_WORDS

/* Hash driver registers configurations */
#define ENABLE_HASH_CLOCK		0x1UL
#define DISABLE_HASH_CLOCK		0x0UL

#define HW_HASH_CTL_SHA1_VAL           	0x0001UL
#define HW_HASH_CTL_SHA256_VAL	        0x0002UL
#define HW_HASH_LE_MODE_VAL	       	0x0001UL
#define HW_HASH_PAD_EN_VAL	        0x1UL

/* The SHA1 hash block size in words */
#define HASH_BLOCK_SIZE_IN_WORDS 16
#define HASH_BLOCK_SIZE_IN_BYTES (HASH_BLOCK_SIZE_IN_WORDS * sizeof(uint32_t))

/* The SHA2 hash block size in words */
#define HASH_SHA512_BLOCK_SIZE_IN_WORDS	32
#define HASH_SHA512_BLOCK_SIZE_IN_BYTES	(HASH_SHA512_BLOCK_SIZE_IN_WORDS * sizeof(uint32_t))

#define CONFIG_HASH_MODE_VAL	        0x7UL

/* the MAC key IPAD and OPAD bytes */
#define MAC_KEY_IPAD_BYTE 0x36
#define MAC_KEY_OPAD_BYTE 0x5C

#define HMAC_CONTEXT_VALIDATION_TAG 0x23456789

/* The CRYS HASH module errors */
#define HASH_DRV_OK						0
#define HASH_DRV_INVALID_USER_CONTEXT_POINTER_ERROR     	(HASH_DRV_MODULE_ERROR_BASE + 0x00UL)
#define HASH_DRV_ILLEGAL_OPERATION_MODE_ERROR			(HASH_DRV_MODULE_ERROR_BASE + 0x01UL)
#define HASH_DRV_USER_CONTEXT_CORRUPTED_ERROR			(HASH_DRV_MODULE_ERROR_BASE + 0x02UL)

/* The CRYS HMAC module errors */
#define HMAC_DRV_OK						0
#define HMAC_DRV_INVALID_USER_CONTEXT_POINTER_ERROR     	(HMAC_DRV_MODULE_ERROR_BASE + 0x00UL)


/* SHA512 soft driver */

/* The first padding byte */
#define LLF_HASH_FIRST_PADDING_BYTE 0x80
/* The size at the end of the padding for SHA384 and SHA512 */
#define LLF_HASH_SHA2_COUNTER_SIZE_ON_END_OF_PADDING_IN_BYTES (4 * sizeof(uint32_t))
#define LLF_HASH_SHA2_COUNTER_SIZE_ON_END_OF_PADDING_IN_WORDS 4

/* the HASH user context validity TAG */
#define HASH_CONTEXT_VALIDATION_TAG 0x12345678

/******************************************************************************
*				BYPASS DEFINITIONS
******************************************************************************/

#define CONFIG_DIN_BYPASS_DOUT_VAL		                0

/* The CRYS BYPASS module errors */
#define BYPASS_DRV_OK						0
#define BYPASS_DRV_ILLEGAL_BLOCK_SIZE_ERROR			(BYPASS_DRV_MODULE_ERROR_BASE + 0x01UL)
#define BYPASS_DRV_ILLEGAL_INPUT_ADDR_MEM_ERROR   		(BYPASS_DRV_MODULE_ERROR_BASE + 0x02UL)
#define BYPASS_DRV_ILLEGAL_OUTPUT_ADDR_MEM_ERROR   		(BYPASS_DRV_MODULE_ERROR_BASE + 0x03UL)

/******************************************************************************
*				CHACHA DEFINITIONS
******************************************************************************/

#define CHACHA_BLOCK_SIZE_BYTES            	64
#define CHACHA_BLOCK_SIZE_WORDS            	(CHACHA_BLOCK_SIZE_BYTES >> 2)
#define CHACHA_NONCE_64_SIZE_BYTES           	8
#define CHACHA_NONCE_64_SIZE_WORDS            	(CHACHA_NONCE_64_SIZE_BYTES >> 2)
#define CHACHA_NONCE_96_SIZE_BYTES            	12
#define CHACHA_NONCE_96_SIZE_WORDS            	(CHACHA_NONCE_96_SIZE_BYTES >> 2)
#define CHACHA_256_BIT_KEY_SIZE                	32
#define CHACHA_256_BIT_KEY_SIZE_WORDS		(CHACHA_256_BIT_KEY_SIZE >> 2)

#define ENABLE_CHACHA_CLOCK		0x1UL
#define DISABLE_CHACHA_CLOCK		0x0UL

#define CONFIG_DIN_CHACHA_DOUT_VAL	        0x10UL

/* The CRYS CHACHA module errors */
#define CHACHA_DRV_OK						0
#define CHACHA_DRV_INVALID_USER_CONTEXT_POINTER_ERROR     	(CHACHA_DRV_MODULE_ERROR_BASE + 0x00UL)
#define CHACHA_DRV_ILLEGAL_OPERATION_DIRECTION_ERROR          	(CHACHA_DRV_MODULE_ERROR_BASE + 0x01UL)
#define CHACHA_DRV_ILLEGAL_INPUT_ADDR_MEM_ERROR   		(CHACHA_DRV_MODULE_ERROR_BASE + 0x02UL)
#define CHACHA_DRV_ILLEGAL_OUTPUT_ADDR_MEM_ERROR   		(CHACHA_DRV_MODULE_ERROR_BASE + 0x03UL)
#define CHACHA_DRV_ILLEGAL_MEM_SIZE_ERROR   		   	(CHACHA_DRV_MODULE_ERROR_BASE + 0x04UL)
#define CHACHA_DRV_ILLEGAL_NONCE_SIZE_ERROR   		   	(CHACHA_DRV_MODULE_ERROR_BASE + 0x05UL)


/******************************************************************************
*				MACROS
******************************************************************************/
/* This MACRO purpose is to switch from CryptoCell definitions to crypto driver definitions, the MACRO assumes that the value is legal (encrypt or decrypt only) */
#define SASI_2_DRIVER_DIRECTION(ssiDirection) ((ssiDirection == SASI_AES_ENCRYPT) ? (CRYPTO_DIRECTION_ENCRYPT) : (CRYPTO_DIRECTION_DECRYPT))

/* Poll on the DOUT MEM DMA (DLLI) busy till it is = 0 */
#define SASI_HAL_WAIT_ON_DOUT_MEM_DMA_BUSY()\
        do {\
	       uint32_t regVal=1;\
	       do {\
		            regVal = SASI_HAL_READ_REGISTER( SASI_REG_OFFSET(HOST_RGF, DOUT_MEM_DMA_BUSY));\
               }while( regVal ); \
        }while(0)

/* Poll on the DIN MEM DMA (DLLI) busy till it is = 0 */
#define SASI_HAL_WAIT_ON_DIN_MEM_DMA_BUSY()\
	do {\
	        uint32_t regVal=1;\
	        do {\
			regVal = SASI_HAL_READ_REGISTER( SASI_REG_OFFSET(HOST_RGF, DIN_MEM_DMA_BUSY));\
                }while( regVal );\
        }while(0)

/* Poll on the DOUT SRAM DMA busy till it is = 0 */
#define SASI_HAL_WAIT_ON_DOUT_SRAM_DMA_BUSY()\
	do {\
	        uint32_t regVal=1; \
	        do {\
			regVal = SASI_HAL_READ_REGISTER( SASI_REG_OFFSET(HOST_RGF, DOUT_SRAM_DMA_BUSY));\
                }while( regVal );\
        }while(0)

/* Poll on the DIN SRAM busy till it is = 0 */
#define SASI_HAL_WAIT_ON_DIN_SRAM_DMA_BUSY()\
	do {\
	        uint32_t regVal=1;\
	        do {\
			regVal = SASI_HAL_READ_REGISTER( SASI_REG_OFFSET(HOST_RGF, DIN_SRAM_DMA_BUSY));\
                }while( regVal );\
        }while(0)


/* Poll on the AES busy till it is = 0 */
#define SASI_HAL_WAIT_ON_AES_BUSY()\
	do {\
		uint32_t regVal=1;\
	        do {\
			regVal = SASI_HAL_READ_REGISTER( SASI_REG_OFFSET(HOST_RGF, AES_BUSY));\
                }while( regVal );\
        }while(0)

/* Poll on the HASH busy till it is = 0 */
#define SASI_HAL_WAIT_ON_HASH_BUSY()\
	do {\
		uint32_t regVal=1;\
	        do {\
			regVal = SASI_HAL_READ_REGISTER( SASI_REG_OFFSET(HOST_RGF, HASH_BUSY));\
                }while( regVal );\
        }while(0)

/* Poll on the CHACHA busy till it is = 0 */
#define SASI_HAL_WAIT_ON_CHACHA_BUSY() 																    \
	    do { 																						    \
		        uint32_t regVal=1; 																		\
		        do { 																					\
			            regVal = SASI_HAL_READ_REGISTER( SASI_REG_OFFSET(HOST_RGF, CHACHA_BUSY));		\
                }while( regVal ); 																		\
        }while(0)

/* Poll on the crypto busy till it is = 0 */
#define SASI_HAL_WAIT_ON_CRYPTO_BUSY()\
	do {\
		uint32_t regVal=1;\
		do {\
			regVal = SASI_HAL_READ_REGISTER( SASI_REG_OFFSET(HOST_RGF, CRYPTO_BUSY));\
                }while( regVal );\
        }while(0)


#endif /* _DRIVER_DEFS_H_ */

