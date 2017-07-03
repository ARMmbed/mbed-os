/***************************************************************************//**
* \file cy_crypto_common.h
* \version 1.0
*
* \brief
*  This file provides common constants and parameters
*  for the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_CRYPTO_COMMON_H)
#define CY_CRYPTO_COMMON_H

#include <stddef.h>
#include "cy_device_headers.h"
#include "syslib/cy_syslib.h"
#include "cy_crypto_config.h"

#if (CPUSS_CRYPTO_PRESENT == 1)

/**
* \addtogroup group_crypto_macro
* \{
*/

/** Driver major version */
#define CY_CRYPTO_DRV_VERSION_MAJOR       (1)

/** Driver minor version */
#define CY_CRYPTO_DRV_VERSION_MINOR       (0)

/** Defines Crypto_Sync blocking execution type parameter */
#define CY_CRYPTO_SYNC_BLOCKING           (true)

/** Defines Crypto_Sync non-blocking execution type parameter */
#define CY_CRYPTO_SYNC_NON_BLOCKING       (false)

/** Defines the Crypto DES key size (in bytes) */
#define CY_CRYPTO_DES_KEY_SIZE            (8u)

/** Defines the Crypto TDES key size (in bytes) */
#define CY_CRYPTO_TDES_KEY_SIZE           (24u)

/** Defines the Crypto AES block size (in bytes) */
#define CY_CRYPTO_AES_BLOCK_SIZE          (16u)

/** Defines the Crypto AES key maximum size (in bytes) */
#define CY_CRYPTO_AES_128_KEY_SIZE        (16u)

/** Defines the Crypto AES key maximum size (in bytes) */
#define CY_CRYPTO_AES_192_KEY_SIZE        (24u)

/** Defines the Crypto AES key maximum size (in bytes) */
#define CY_CRYPTO_AES_256_KEY_SIZE        (32u)

/** Defines size of the AES block, in four byte words */
#define CY_CRYPTO_AES_BLOCK_SIZE_U32      (4UL)

/** Defines the Crypto notify interrupt number */
#define CY_CRYPTO_NOTIFY_INTR     CY_IPC_INTR_CRYPTO_M0

/* Overall AES enable */
#define CY_CRYPTO_USER_AES_ENABLE (CY_CRYPTO_USER_AES_ECB | \
                                   CY_CRYPTO_USER_AES_CBC | \
                                   CY_CRYPTO_USER_AES_CFB | \
                                   CY_CRYPTO_USER_AES_CTR | \
                                   CY_CRYPTO_USER_CMAC)

/* Overall SHA enable */
#define CY_CRYPTO_USER_SHA_ENABLE (CY_CRYPTO_USER_SHA1   | \
                                   CY_CRYPTO_USER_SHA256 | \
                                   CY_CRYPTO_USER_SHA512 | \
                                   CY_CRYPTO_USER_HMAC)

#if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1))

/** Hash size for the SHA1 mode, in Bytes   */
#define CY_CRYPTO_SHA1_DIGEST_SIZE          (20u)
/** Hash size for the SHA224 mode, in Bytes */
#define CY_CRYPTO_SHA224_DIGEST_SIZE        (28u)
/** Hash size for the SHA256 mode, in Bytes */
#define CY_CRYPTO_SHA256_DIGEST_SIZE        (32u)
/** Hash size for the SHA384 mode, in Bytes */
#define CY_CRYPTO_SHA384_DIGEST_SIZE        (48u)
/** Hash size for the SHA512 mode, in Bytes */
#define CY_CRYPTO_SHA512_DIGEST_SIZE        (64u)
/** Hash size for the SHA512_224 mode, in Bytes */
#define CY_CRYPTO_SHA512_224_DIGEST_SIZE    (28u)
/** Hash size for the SHA512_256 mode, in Bytes */
#define CY_CRYPTO_SHA512_256_DIGEST_SIZE    (32u)

#endif /* #if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1)) */


#if ((CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1))

/** Processed message size for the RSA 1024Bit mode, in Bytes */
#define CY_CRYPTO_RSA1024_MESSAGE_SIZE      (128)
/** Processed message size for the RSA 1536Bit mode, in Bytes */
#define CY_CRYPTO_RSA1536_MESSAGE_SIZE      (192)
/** Processed message size for the RSA 2048Bit mode, in Bytes */
#define CY_CRYPTO_RSA2048_MESSAGE_SIZE      (256)

#endif /* #if ((CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1)) */

/** Defines the Crypto release interrupt number */
#define CY_CRYPTO_RELEASE_INTR    CY_IPC_INTR_CRYPTO_M4

/** Crypto Driver PDL ID */
#define CY_CRYPTO_ID              CY_PDL_DRV_ID(0x0Cu)

/** \} group_crypto_macro */

/**
* \addtogroup group_crypto_data_structures
* \{
*/

/** The Crypto user callback function type called at the end of Crypto calculation. */
typedef void (*cy_crypto_callback_ptr_t)(void);

/** The Crypto configuration structure. */
typedef struct
{
    uint8_t cm0NotifyIntrNum;                /**<  Specifies the notify interrupt number for the CM0 core */
    uint8_t notifyIntrPrior;                 /**<  Specifies the notify interrupt priority */
    uint8_t cm0ReleaseIntrNum;               /**<  Specifies the release interrupt number for the CM0 core */
    uint8_t releaseIntrPrior;                /**<  Specifies the release interrupt priority */
    uint8_t cm0CryptoErrorIntrNum;           /**<  Specifies the interrupt number for the Crypto error handler */
    uint8_t cryptoErrorIntrPrior;            /**<  Specifies the interrupt priority for the Crypto error handler */
    cy_crypto_callback_ptr_t userCompleteCallback;   /**<  User callback, called after Crypto HW completes calculation */
    cy_israddress userGetDataHandler;        /**<  User IPC process data IRQ handler */
    cy_israddress userErrorHandler;          /**<  User error IRQ handler */
} cy_stc_crypto_config_t;

#if ((CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1))
/** The structure for storing the RSA public key. */
typedef struct
{
    uint8_t *moduloPtr;         /**< The pointer to the modulus for both public and private keys. */
    uint32_t moduloLength;      /**< The modulus length, in bits, maximum supported size is 2048Bit */
    uint8_t *pubExpPtr;         /**< The pointer to the RSA key public exponent */
    uint32_t pubExpLength;      /**< The RSA key public exponent length, in bits, maximum supported size is 256Bit */
    uint8_t *barretCoefPtr;     /**< The pointer to the Barrett coefficient of size (moduloLength + 1) */
    uint8_t *inverseModuloPtr;  /**< The pointer to the binary inverse of the modulo of size moduloLength */
    uint8_t *rBarPtr;           /**< The pointer to the (2^moduloLength mod modulo) of size moduloLength */
} cy_stc_crypto_rsa_pub_key_t;

typedef enum
{
    CY_CRYPTO_RSA_VERIFY_SUCCESS = 0x00u,   /**< RSA verify SUCCESS */
    CY_CRYPTO_RSA_VERIFY_FAIL    = 0x01u    /**< RSA verify FAILED */
} cy_en_crypto_rsa_ver_result_t;

#endif /* #if ((CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1)) */

/** Structure for storing a description of a Crypto hardware error  */
typedef struct
{
    uint32_t errorStatus0;   /**< Captures error description information:
                                  for INSTR_OPC_ERROR: - violating the instruction.
                                  for INSTR_CC_ERROR : - violating the instruction condition code.
                                  for BUS_ERROR      : - violating the transfer address. */

    uint32_t errorStatus1;   /**< [31]     - Specifies if ERROR_STATUS0 and ERROR_STATUS1 capture is valid error-information.
                                  [26..24] - The error source:
                                             "0": INSTR_OPC_ERROR - an instruction decoder error.
                                             "1": INSTR_CC_ERROR - an instruction condition code-error.
                                             "2": BUS_ERROR - a bus master interface AHB-Lite bus-error.
                                             "3": TR_AP_DETECT_ERROR.
                                  [23..0]  - Captures error description information.
                                             For BUS_ERROR: - violating the transfer, read the attribute (DATA23[0]).
                                                            - violating the transfer, the size attribute (DATA23[5:4]).
                                            "0": an 8-bit transfer; "1": 16 bits transfer; "2": 32-bit transfer. */
} cy_stc_crypto_hw_error_t;

/** \} group_crypto_data_structures */


/**
* \addtogroup group_crypto_enums
* \{
*/


/** The key length options for the AES method. */
#if ((CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1))
typedef enum
{
    CY_CRYPTO_KEY_AES_128   = 0x00u,   /**< The AES key size is 128 bits */
    CY_CRYPTO_KEY_AES_192   = 0x01u,   /**< The AES key size is 192 bits */
    CY_CRYPTO_KEY_AES_256   = 0x02u    /**< The AES key size is 256 bits */
} cy_en_crypto_aes_key_length_t;
#endif /* #if ((CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1)) */

/** Defines the direction of the Crypto methods*/
typedef enum
{
    CY_CRYPTO_ENCRYPT   = 0x00u,   /**< The forward mode, plain text will be encrypted into cipher text */
    CY_CRYPTO_DECRYPT   = 0x01u    /**< The reverse mode, cipher text will be decrypted into plain text */
} cy_en_crypto_dir_mode_t;

/** Define modes of SHA method */
#if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1))
typedef enum
{
#if ((CPUSS_CRYPTO_SHA1 == 1) && (CY_CRYPTO_USER_SHA1 == 1))
    CY_CRYPTO_MODE_SHA1        = 0x00u,   /**< Sets the SHA1 mode */
#endif /* #if ((CPUSS_CRYPTO_SHA1 == 1) && (CY_CRYPTO_USER_SHA1 == 1)) */

#if ((CPUSS_CRYPTO_SHA256 == 1) && (CY_CRYPTO_USER_SHA256 == 1))
    CY_CRYPTO_MODE_SHA224      = 0x11u,   /**< Sets the SHA224 mode */
    CY_CRYPTO_MODE_SHA256      = 0x01u,   /**< Sets the SHA256 mode */
#endif /* #if ((CPUSS_CRYPTO_SHA256 == 1) && (CY_CRYPTO_USER_SHA256 == 1)) */

#if ((CPUSS_CRYPTO_SHA512 == 1) && (CY_CRYPTO_USER_SHA512 == 1))
    CY_CRYPTO_MODE_SHA384      = 0x12u,   /**< Sets the SHA384 mode */
    CY_CRYPTO_MODE_SHA512      = 0x02u,   /**< Sets the SHA512 mode */
    CY_CRYPTO_MODE_SHA512_256  = 0x22u,   /**< Sets the SHA512/256 mode */
    CY_CRYPTO_MODE_SHA512_224  = 0x32u    /**< Sets the SHA512/224 mode */
#endif /* #if ((CPUSS_CRYPTO_SHA512 == 1) && (CY_CRYPTO_USER_SHA512 == 1)) */
} cy_en_crypto_sha_mode_t;
#endif /* #if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1)) */

/** Errors of the Crypto block */
typedef enum
{
    /** Operation completed successfully. */
    CY_CRYPTO_SUCCESS             = 0x00u,

    /** A hardware error occurred, detailed information is in stc_crypto_hw_error_t. */
    CY_CRYPTO_HW_ERROR            = CY_CRYPTO_ID | CY_PDL_STATUS_ERROR   | 0x01u,

    /** The size of input data is not multiple of 16. */
    CY_CRYPTO_SIZE_NOT_X16        = CY_CRYPTO_ID | CY_PDL_STATUS_ERROR   | 0x02u,

    /** The key for the DES method is weak. */
    CY_CRYPTO_DES_WEAK_KEY        = CY_CRYPTO_ID | CY_PDL_STATUS_WARNING | 0x03u,

    /** Communication between the client-server via IPC is broken. */
    CY_CRYPTO_COMM_FAIL           = CY_CRYPTO_ID | CY_PDL_STATUS_ERROR   | 0x04u,

    /** The Crypto server is not started. */
    CY_CRYPTO_SERVER_NOT_STARTED  = CY_CRYPTO_ID | CY_PDL_STATUS_ERROR   | 0x06u,

    /** Communication between the client-server via IPC is broken. */
    CY_CRYPTO_SERVER_BUSY         = CY_CRYPTO_ID | CY_PDL_STATUS_INFO    | 0x07u,

    /** The Crypto driver is not initialized. */
    CY_CRYPTO_NOT_INITIALIZED     = CY_CRYPTO_ID | CY_PDL_STATUS_ERROR   | 0x08u,

    /** The Crypto hardware is not enabled. */
    CY_CRYPTO_HW_NOT_ENABLED      = CY_CRYPTO_ID | CY_PDL_STATUS_ERROR   | 0x09u,

    /** The Crypto hardware is not enabled. */
    CY_CRYPTO_NOT_SUPPORTED       = CY_CRYPTO_ID | CY_PDL_STATUS_ERROR   | 0x0Au

} cy_en_crypto_status_t;

/** \} group_crypto_enums */

/** Instruction to communicate between Client and Server */
typedef enum
{
    CY_CRYPTO_INSTR_UNKNOWN      = 0x00u,
    CY_CRYPTO_INSTR_ENABLE       = 0x01u,
    CY_CRYPTO_INSTR_DISABLE      = 0x02u,

#if ((CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_PR == 1))
    CY_CRYPTO_INSTR_PRNG_INIT    = 0x03u,
    CY_CRYPTO_INSTR_PRNG         = 0x04u,
#endif /* #if ((CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_PR == 1)) */

#if ((CPUSS_CRYPTO_TR == 1) && (CY_CRYPTO_USER_TR == 1))
    CY_CRYPTO_INSTR_TRNG_INIT    = 0x05u,
    CY_CRYPTO_INSTR_TRNG         = 0x06u,
#endif /* #if ((CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_TR == 1)) */

#if ((CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1))
    CY_CRYPTO_INSTR_AES_INIT     = 0x07u,

#if (CY_CRYPTO_USER_AES_ECB == 1)
    CY_CRYPTO_INSTR_AES_ECB      = 0x08u,
#endif /* #if (CRYPTO_USER_AES_ECB == 1) */

#if (CY_CRYPTO_USER_AES_CBC == 1)
    CY_CRYPTO_INSTR_AES_CBC      = 0x09u,
#endif /* #if (CRYPTO_USER_AES_CBC == 1) */

#if (CY_CRYPTO_USER_AES_CFB == 1)
    CY_CRYPTO_INSTR_AES_CFB      = 0x0Au,
#endif /* #if (CRYPTO_USER_AES_CFB == 1) */

#if (CY_CRYPTO_USER_AES_CTR == 1)
    CY_CRYPTO_INSTR_AES_CTR      = 0x0Bu,
#endif /* #if (CRYPTO_USER_AES_CTR == 1) */

#if (CY_CRYPTO_USER_CMAC == 1)
    CY_CRYPTO_INSTR_CMAC         = 0x0Cu,
#endif /* #if (CY_CRYPTO_USER_CMAC == 1) */
#endif /* #if ((CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1)) */

#if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1))
    CY_CRYPTO_INSTR_SHA          = 0x0Du,
#endif /* #if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1)) */

#if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_HMAC == 1))
    CY_CRYPTO_INSTR_HMAC         = 0x0Eu,
#endif /* #if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_HMAC == 1)) */

#if ((CPUSS_CRYPTO_STR == 1) && (CY_CRYPTO_USER_STR == 1))
    CY_CRYPTO_INSTR_MEM_CPY      = 0x0Fu,
    CY_CRYPTO_INSTR_MEM_SET      = 0x10u,
    CY_CRYPTO_INSTR_MEM_CMP      = 0x11u,
    CY_CRYPTO_INSTR_MEM_XOR      = 0x12u,
#endif /* #if ((CPUSS_CRYPTO_STR == 1) && (CY_CRYPTO_USER_STR == 1)) */

#if ((CPUSS_CRYPTO_CRC == 1) && (CY_CRYPTO_USER_CRC == 1))
    CY_CRYPTO_INSTR_CRC_INIT     = 0x13u,
    CY_CRYPTO_INSTR_CRC          = 0x14u,
#endif /* #if ((CPUSS_CRYPTO_CRC == 1) && (CY_CRYPTO_USER_CRC == 1)) */

#if ((CPUSS_CRYPTO_DES == 1) && (CY_CRYPTO_USER_DES == 1))
    CY_CRYPTO_INSTR_DES          = 0x15u,
    CY_CRYPTO_INSTR_3DES         = 0x16u,
#endif /* #if ((CPUSS_CRYPTO_DES == 1) && (CY_CRYPTO_USER_DES == 1)) */

#if ((CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1))
    CY_CRYPTO_INSTR_RSA_PROC     = 0x17u,
    CY_CRYPTO_INSTR_RSA_COEF     = 0x18u,
#endif /* #if ((CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1)) */

#if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1))
    CY_CRYPTO_INSTR_RSA_VER      = 0x19u
#endif /* #if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_HMAC == 1)) */

} cy_en_crypto_comm_instr_t;


/**
* \addtogroup group_crypto_data_structures
* \{
*/

/** Structure for storing the AES state */
#if ((CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1))
typedef struct
{
    uint32_t *keyPtr;                                       /**< Pointer to AES key */
    uint32_t  invKey[CY_CRYPTO_AES_256_KEY_SIZE / 4u];      /**< Storage for inversed key */
    cy_en_crypto_aes_key_length_t keyLength;                /**< AES key length */
    uint32_t blockIdx;                                      /**< AES processed block index (for CMAC, SHA operations) */
} cy_stc_crypto_aes_state_t;
#endif /* #if ((CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1)) */

/*************************************************************
*  Structures used for communication between Client and Server
***************************************************************/

/** Structure for storing the global context */
typedef struct
{
    cy_en_crypto_comm_instr_t instr;                        /**< Server instruction code */
    cy_en_crypto_status_t resp;                             /**< Response from executed crypto function */
    cy_stc_crypto_hw_error_t hwErrorStatus;                 /**< Hardware processing errors */
    void *xdata;                                            /**< Pointer to the crypto function specific context data */
} cy_stc_crypto_context_t;


#if ((CPUSS_CRYPTO_DES == 1) && (CY_CRYPTO_USER_DES == 1))
/** Structure for storing the DES context */
typedef struct
{
    cy_en_crypto_dir_mode_t dirMode;    /**<  Operation direction (Encrypt / Decrypt) */
    uint32_t *keyPtr;                   /**<  Pointer to key data */
    uint32_t *dstPtr;                   /**<  Pointer to data destination block */
    uint32_t *srcPtr;                   /**<  Pointer to data source block */
} cy_stc_crypto_context_des_t;
#endif /* #if ((CPUSS_CRYPTO_DES == 1) && (CY_CRYPTO_USER_DES == 1)) */

#if ((CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1))
/** Structure for storing the AES context */
typedef struct
{
    cy_stc_crypto_aes_state_t aesState;                     /**< AES state data */
    cy_en_crypto_dir_mode_t dirMode;                        /**< Operation direction (Encrypt / Decrypt) */
    uint32_t srcSize;                                       /**< Operation data size */
    uint32_t *srcOffset;                                    /**< Size of the last non-complete block (for CTR mode only) */
    uint32_t *ivPtr;                                        /**< Initialization vector, in the CTR mode is used as nonceCounter */
    uint32_t *streamBlock;                                  /**< AES processed block pointer (for CTR mode only) */
    uint32_t *dstPtr;                                       /**< Pointer to data destination block */
    uint32_t *srcPtr;                                       /**< Pointer to data source block */
} cy_stc_crypto_context_aes_t;
#endif /* #if ((CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1)) */

#if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1))
/** Structure for storing the SHA context */
typedef struct
{
    uint32_t *messagePtr;                                   /**< Pointer to data source block */
    uint32_t  messageSize;                                  /**< Operation data size */
    uint32_t *dstPtr;                                       /**< Pointer to data destination block */
    cy_en_crypto_sha_mode_t mode;                           /**< SHA mode */
    uint32_t *keyPtr;                                       /**< Pointer to key data (for HMAC only) */
    uint32_t  keyLength;                                    /**< Key data length (for HMAC only) */
} cy_stc_crypto_context_sha_t;
#endif /* #if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1)) */

#if ((CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_PR == 1))
/** Structure for storing the PRNG context */
typedef struct
{
    uint32_t lfsr32InitState;                               /**< lfsr32 initialization data */
    uint32_t lfsr31InitState;                               /**< lfsr31 initialization data */
    uint32_t lfsr29InitState;                               /**< lfsr29 initialization data */
    uint32_t max;                                           /**< Maximum of the generated value */
    uint32_t *prngNumPtr;                                   /**< Pointer to generated value */
} cy_stc_crypto_context_prng_t;
#endif /* #if ((CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_PR == 1)) */

#if ((CPUSS_CRYPTO_TR == 1) && (CY_CRYPTO_USER_TR == 1))
/** Structure for storing the TRNG context */
typedef struct
{
    uint32_t GAROPol;       /**< The polynomial for the programmable Galois ring oscillator (TR_GARO_CTL).
                                 The polynomial is represented WITHOUT the high order bit (this bit is always assumed '1').
                                 The polynomial should be aligned so that more significant bits (bit 30 and down)
                                 contain the polynomial and less significant bits (bit 0 and up) contain padding '0's. */

    uint32_t FIROPol;       /**< The polynomial for the programmable Fibonacci ring oscillator(TR_FIRO_CTL).
                                 The polynomial is represented WITHOUT the high order bit (this bit is always assumed '1').
                                 The polynomial should be aligned so that more significant bits (bit 30 and down)
                                 contain the polynomial and less significant bits (bit 0 and up) contain padding '0's. */
    uint32_t max;           /**< Maximum of the generated value */
    uint32_t *trngNumPtr;   /**< Pointer to generated value */
} cy_stc_crypto_context_trng_t;
#endif /* #if ((CPUSS_CRYPTO_TR == 1) && (CY_CRYPTO_USER_TR == 1)) */

#if ((CPUSS_CRYPTO_STR == 1) && (CY_CRYPTO_USER_STR == 1))
/** Structure for storing the STR context */
typedef struct
{
    void const *srcPtr0;            /**<  Pointer to 1-st string source */
    void const *srcPtr1;            /**<  Pointer to 2-nd string source */
    void *dstPtr;                   /**<  Pointer to string destination */
    uint32_t dataSize;              /**<  Operation data size */
    uint32_t data;                  /**<  Operation data value (for memory setting) */
} cy_stc_crypto_context_str_t;
#endif /* #if ((CPUSS_CRYPTO_STR == 1) && (CY_CRYPTO_USER_STR == 1)) */

#if ((CPUSS_CRYPTO_CRC == 1) && (CY_CRYPTO_USER_CRC == 1))
/** Structure for storing the CRC context */
typedef struct
{
    void* srcDataPtr;               /**<  Pointer to data source block */
    uint32_t dataSize;              /**<  operation data size */
    uint32_t *crcPtr;               /**<  Pointer to CRC destination variable */
    uint32_t polynomial;            /**<  Polynomial for CRC calculate */
    uint32_t lfsrInitState;         /**<  CRC calculation initial value */
    uint32_t dataReverse;           /**<  Input data reverse flag */
    uint32_t dataXor;               /**<  Input data  XOR flag */
    uint32_t remReverse;            /**<  Output data reverse flag */
    uint32_t remXor;                /**<  Output data XOR flag */
} cy_stc_crypto_context_crc_t;
#endif /* #if ((CPUSS_CRYPTO_CRC == 1) && (CY_CRYPTO_USER_CRC == 1)) */

#if (CY_CRYPTO_USER_PKCS1_5 == 1)
/** Structure for storing the RSA verify context */
typedef struct
{
    cy_en_crypto_rsa_ver_result_t *verResult;  /**< Pointer to verification result /ref cy_en_crypto_rsa_ver_result_t */
    cy_en_crypto_sha_mode_t digestType;        /**< SHA digess type, used with SHA calculation of the message */
    uint32_t const *hashPtr;                   /**< SHA digest of the message, calculated with digestType */
    uint32_t const *decryptedSignaturePtr;     /**< Previously decrypted RSA signature */
    uint32_t decryptedSignatureLength;         /**< Length of the decrypted RSA signature */
} cy_stc_crypto_context_rsa_ver_t;
#endif /* #if (CY_CRYPTO_USER_PKCS1_5 == 1) */

#if ((CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1))
/** Structure for storing the RSA process context */
typedef struct
{
    cy_stc_crypto_rsa_pub_key_t *keyPtr;     /**<  Pointer to key data */
    uint32_t const *messagePtr;              /**<  Pointer to data source block */
    uint32_t messageLength;                  /**<  Operation data size */
    uint32_t const *resultPtr;               /**<  Pointer to data destination block */
} cy_stc_crypto_context_rsa_t;
#endif /* #if ((CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1)) */

/** \} group_crypto_data_structures */


#endif  /* (CPUSS_CRYPTO_PRESENT == 1) */

#endif /* #if !defined(CY_CRYPTO_COMMON_H) */

/* [] END OF FILE */
