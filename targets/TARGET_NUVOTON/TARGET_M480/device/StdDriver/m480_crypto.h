/**************************************************************************//**
 * @file     crypto.h
 * @version  V1.10
 * @brief    Cryptographic Accelerator driver header file
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup M480_Device_Driver M480 Device Driver
  @{
*/

/** @addtogroup M480_CRYPTO_Driver CRYPTO Driver
  @{
*/


/** @addtogroup M480_CRYPTO_EXPORTED_CONSTANTS CRYPTO Exported Constants
  @{
*/

#define PRNG_KEY_SIZE_64        0       /*!< Select to generate 64-bit random key    \hideinitializer */
#define PRNG_KEY_SIZE_128       1       /*!< Select to generate 128-bit random key   \hideinitializer */
#define PRNG_KEY_SIZE_192       2       /*!< Select to generate 192-bit random key   \hideinitializer */
#define PRNG_KEY_SIZE_256       3       /*!< Select to generate 256-bit random key   \hideinitializer */

#define PRNG_SEED_CONT          0       /*!< PRNG using current seed                 \hideinitializer */
#define PRNG_SEED_RELOAD        1       /*!< PRNG reload new seed                    \hideinitializer */

#define AES_KEY_SIZE_128        0       /*!< AES select 128-bit key length           \hideinitializer */
#define AES_KEY_SIZE_192        1       /*!< AES select 192-bit key length           \hideinitializer */
#define AES_KEY_SIZE_256        2       /*!< AES select 256-bit key length           \hideinitializer */

#define AES_MODE_ECB            0       /*!< AES select ECB mode                     \hideinitializer */
#define AES_MODE_CBC            1       /*!< AES select CBC mode                     \hideinitializer */
#define AES_MODE_CFB            2       /*!< AES select CFB mode                     \hideinitializer */
#define AES_MODE_OFB            3       /*!< AES select OFB mode                     \hideinitializer */
#define AES_MODE_CTR            4       /*!< AES select CTR mode                     \hideinitializer */
#define AES_MODE_CBC_CS1        0x10    /*!< AES select CBC CS1 mode                 \hideinitializer */
#define AES_MODE_CBC_CS2        0x11    /*!< AES select CBC CS2 mode                 \hideinitializer */
#define AES_MODE_CBC_CS3        0x12    /*!< AES select CBC CS3 mode                 \hideinitializer */

#define AES_NO_SWAP             0       /*!< AES do not swap input and output data   \hideinitializer */
#define AES_OUT_SWAP            1       /*!< AES swap output data                    \hideinitializer */
#define AES_IN_SWAP             2       /*!< AES swap input data                     \hideinitializer */
#define AES_IN_OUT_SWAP         3       /*!< AES swap both input and output data     \hideinitializer */

#define DES_MODE_ECB            0x000   /*!< DES select ECB mode                     \hideinitializer */
#define DES_MODE_CBC            0x100   /*!< DES select CBC mode                     \hideinitializer */
#define DES_MODE_CFB            0x200   /*!< DES select CFB mode                     \hideinitializer */
#define DES_MODE_OFB            0x300   /*!< DES select OFB mode                     \hideinitializer */
#define DES_MODE_CTR            0x400   /*!< DES select CTR mode                     \hideinitializer */
#define TDES_MODE_ECB           0x004   /*!< TDES select ECB mode                    \hideinitializer */
#define TDES_MODE_CBC           0x104   /*!< TDES select CBC mode                    \hideinitializer */
#define TDES_MODE_CFB           0x204   /*!< TDES select CFB mode                    \hideinitializer */
#define TDES_MODE_OFB           0x304   /*!< TDES select OFB mode                    \hideinitializer */
#define TDES_MODE_CTR           0x404   /*!< TDES select CTR mode                    \hideinitializer */

#define TDES_NO_SWAP            0       /*!< TDES do not swap data                       \hideinitializer */
#define TDES_WHL_SWAP           1       /*!< TDES swap high-low word                     \hideinitializer */
#define TDES_OUT_SWAP           2       /*!< TDES swap output data                       \hideinitializer */
#define TDES_OUT_WHL_SWAP       3       /*!< TDES swap output data and high-low word     \hideinitializer */
#define TDES_IN_SWAP            4       /*!< TDES swap input data                        \hideinitializer */
#define TDES_IN_WHL_SWAP        5       /*!< TDES swap input data and high-low word      \hideinitializer */
#define TDES_IN_OUT_SWAP        6       /*!< TDES swap both input and output data        \hideinitializer */
#define TDES_IN_OUT_WHL_SWAP    7       /*!< TDES swap input, output and high-low word   \hideinitializer */

#define SHA_MODE_SHA1           0       /*!< SHA select SHA-1 160-bit                \hideinitializer */
#define SHA_MODE_SHA224         5       /*!< SHA select SHA-224 224-bit              \hideinitializer */
#define SHA_MODE_SHA256         4       /*!< SHA select SHA-256 256-bit              \hideinitializer */
#define SHA_MODE_SHA384         7       /*!< SHA select SHA-384 384-bit              \hideinitializer */
#define SHA_MODE_SHA512         6       /*!< SHA select SHA-512 512-bit              \hideinitializer */

#define SHA_NO_SWAP             0       /*!< SHA do not swap input and output data   \hideinitializer */
#define SHA_OUT_SWAP            1       /*!< SHA swap output data                    \hideinitializer */
#define SHA_IN_SWAP             2       /*!< SHA swap input data                     \hideinitializer */
#define SHA_IN_OUT_SWAP         3       /*!< SHA swap both input and output data     \hideinitializer */

#define CRYPTO_DMA_FIRST        0x4     /*!< Do first encrypt/decrypt in DMA cascade \hideinitializer */
#define CRYPTO_DMA_ONE_SHOT     0x5     /*!< Do one shot encrypt/decrypt with DMA      \hideinitializer */
#define CRYPTO_DMA_CONTINUE     0x6     /*!< Do continuous encrypt/decrypt in DMA cascade \hideinitializer */
#define CRYPTO_DMA_LAST         0x7     /*!< Do last encrypt/decrypt in DMA cascade          \hideinitializer */

typedef enum {
    CURVE_P_192,
    CURVE_P_224,
    CURVE_P_256,
    CURVE_P_384,
    CURVE_P_521,
    CURVE_K_163,
    CURVE_K_233,
    CURVE_K_283,
    CURVE_K_409,
    CURVE_K_571,
    CURVE_B_163,
    CURVE_B_233,
    CURVE_B_283,
    CURVE_B_409,
    CURVE_B_571
}
E_ECC_CURVE;


/*@}*/ /* end of group M480_CRYPTO_EXPORTED_CONSTANTS */


/** @addtogroup M480_CRYPTO_EXPORTED_MACROS CRYPTO Exported Macros
  @{
*/

/*----------------------------------------------------------------------------------------------*/
/*  Macros                                                                                      */
/*----------------------------------------------------------------------------------------------*/

/**
  * @brief This macro enables PRNG interrupt.
  * @param None
  * @return None
  * \hideinitializer
  */
#define PRNG_ENABLE_INT()       (CRPT->INTEN |= CRPT_INTEN_PRNGIEN_Msk)

/**
  * @brief This macro disables PRNG interrupt.
  * @param None
  * @return None
  * \hideinitializer
  */
#define PRNG_DISABLE_INT()      (CRPT->INTEN &= ~CRPT_INTEN_PRNGIEN_Msk)

/**
  * @brief This macro gets PRNG interrupt flag.
  * @param None
  * @return PRNG interrupt flag.
  * \hideinitializer
  */
#define PRNG_GET_INT_FLAG()     (CRPT->INTSTS & CRPT_INTSTS_PRNGIF_Msk)

/**
  * @brief This macro clears PRNG interrupt flag.
  * @param None
  * @return None
  * \hideinitializer
  */
#define PRNG_CLR_INT_FLAG()     (CRPT->INTSTS = CRPT_INTSTS_PRNGIF_Msk)

/**
  * @brief This macro enables AES interrupt.
  * @param None
  * @return None
  * \hideinitializer
  */
#define AES_ENABLE_INT()        (CRPT->INTEN |= (CRPT_INTEN_AESIEN_Msk|CRPT_INTEN_AESEIEN_Msk))

/**
  * @brief This macro disables AES interrupt.
  * @param None
  * @return None
  * \hideinitializer
  */
#define AES_DISABLE_INT()       (CRPT->INTEN &= ~(CRPT_INTEN_AESIEN_Msk|CRPT_INTEN_AESEIEN_Msk))

/**
  * @brief This macro gets AES interrupt flag.
  * @param None
  * @return AES interrupt flag.
  * \hideinitializer
  */
#define AES_GET_INT_FLAG()      (CRPT->INTSTS & (CRPT_INTSTS_AESIF_Msk|CRPT_INTSTS_AESEIF_Msk))

/**
  * @brief This macro clears AES interrupt flag.
  * @param None
  * @return None
  * \hideinitializer
  */
#define AES_CLR_INT_FLAG()      (CRPT->INTSTS = (CRPT_INTSTS_AESIF_Msk|CRPT_INTSTS_AESEIF_Msk))

/**
  * @brief This macro enables AES key protection.
  * @param None
  * @return None
  * \hideinitializer
  */
#define AES_ENABLE_KEY_PROTECT()  (CRPT->AES_CTL |= CRPT_AES_CTL_KEYPRT_Msk)

/**
  * @brief This macro disables AES key protection.
  * @param None
  * @return None
  * \hideinitializer
  */
#define AES_DISABLE_KEY_PROTECT() (CRPT->AES_CTL = (CRPT->AES_CTL & ~CRPT_AES_CTL_KEYPRT_Msk) | (0x16<<CRPT_AES_CTL_KEYUNPRT_Pos)); \
                                  (CRPT->AES_CTL &= ~CRPT_AES_CTL_KEYPRT_Msk)

/**
  * @brief This macro enables TDES interrupt.
  * @param None
  * @return None
  * \hideinitializer
  */
#define TDES_ENABLE_INT()       (CRPT->INTEN |= (CRPT_INTEN_TDESIEN_Msk|CRPT_INTEN_TDESEIEN_Msk))

/**
  * @brief This macro disables TDES interrupt.
  * @param None
  * @return None
  * \hideinitializer
  */
#define TDES_DISABLE_INT()      (CRPT->INTEN &= ~(CRPT_INTEN_TDESIEN_Msk|CRPT_INTEN_TDESEIEN_Msk))

/**
  * @brief This macro gets TDES interrupt flag.
  * @param None
  * @return TDES interrupt flag.
  * \hideinitializer
  */
#define TDES_GET_INT_FLAG()     (CRPT->INTSTS & (CRPT_INTSTS_TDESIF_Msk|CRPT_INTSTS_TDESEIF_Msk))

/**
  * @brief This macro clears TDES interrupt flag.
  * @param None
  * @return None
  * \hideinitializer
  */
#define TDES_CLR_INT_FLAG()     (CRPT->INTSTS = (CRPT_INTSTS_TDESIF_Msk|CRPT_INTSTS_TDESEIF_Msk))

/**
  * @brief This macro enables TDES key protection.
  * @param None
  * @return None
  * \hideinitializer
  */
#define TDES_ENABLE_KEY_PROTECT()  (CRPT->TDES_CTL |= CRPT_TDES_CTL_KEYPRT_Msk)

/**
  * @brief This macro disables TDES key protection.
  * @param None
  * @return None
  * \hideinitializer
  */
#define TDES_DISABLE_KEY_PROTECT() (CRPT->TDES_CTL = (CRPT->TDES_CTL & ~CRPT_TDES_CTL_KEYPRT_Msk) | (0x16<<CRPT_TDES_CTL_KEYUNPRT_Pos)); \
                                   (CRPT->TDES_CTL &= ~CRPT_TDES_CTL_KEYPRT_Msk)

/**
  * @brief This macro enables SHA interrupt.
  * @param None
  * @return None
  * \hideinitializer
  */
#define SHA_ENABLE_INT()        (CRPT->INTEN |= (CRPT_INTEN_HMACIEN_Msk|CRPT_INTEN_HMACEIEN_Msk))

/**
  * @brief This macro disables SHA interrupt.
  * @param None
  * @return None
  * \hideinitializer
  */
#define SHA_DISABLE_INT()       (CRPT->INTEN &= ~(CRPT_INTEN_HMACIEN_Msk|CRPT_INTEN_HMACEIEN_Msk))

/**
  * @brief This macro gets SHA interrupt flag.
  * @param None
  * @return SHA interrupt flag.
  * \hideinitializer
  */
#define SHA_GET_INT_FLAG()      (CRPT->INTSTS & (CRPT_INTSTS_HMACIF_Msk|CRPT_INTSTS_HMACEIF_Msk))

/**
  * @brief This macro clears SHA interrupt flag.
  * @param None
  * @return None
  * \hideinitializer
  */
#define SHA_CLR_INT_FLAG()      (CRPT->INTSTS = (CRPT_INTSTS_HMACIF_Msk|CRPT_INTSTS_HMACEIF_Msk))

/**
  * @brief This macro enables ECC interrupt.
  * @param None
  * @return None
  * \hideinitializer
  */
#define ECC_ENABLE_INT()        (CRPT->INTEN |= (CRPT_INTEN_ECCIEN_Msk|CRPT_INTEN_ECCEIEN_Msk))

/**
  * @brief This macro disables ECC interrupt.
  * @param None
  * @return None
  * \hideinitializer
  */
#define ECC_DISABLE_INT()       (CRPT->INTEN &= ~(CRPT_INTEN_ECCIEN_Msk|CRPT_INTEN_ECCEIEN_Msk))

/**
  * @brief This macro gets ECC interrupt flag.
  * @param None
  * @return ECC interrupt flag.
  * \hideinitializer
  */
#define ECC_GET_INT_FLAG()      (CRPT->INTSTS & (CRPT_INTSTS_ECCIF_Msk|CRPT_INTSTS_ECCEIF_Msk))

/**
  * @brief This macro clears ECC interrupt flag.
  * @param None
  * @return None
  * \hideinitializer
  */
#define ECC_CLR_INT_FLAG()      (CRPT->INTSTS = (CRPT_INTSTS_ECCIF_Msk|CRPT_INTSTS_ECCEIF_Msk))


/*@}*/ /* end of group M480_CRYPTO_EXPORTED_MACROS */



/** @addtogroup M480_CRYPTO_EXPORTED_FUNCTIONS CRYPTO Exported Functions
  @{
*/

/*---------------------------------------------------------------------------------------------------------*/
/*  Functions                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/

void PRNG_Open(uint32_t u32KeySize, uint32_t u32SeedReload, uint32_t u32Seed);
void PRNG_Start(void);
void PRNG_Read(uint32_t u32RandKey[]);
void AES_Open(uint32_t u32Channel, uint32_t u32EncDec, uint32_t u32OpMode, uint32_t u32KeySize, uint32_t u32SwapType);
void AES_Start(int32_t u32Channel, uint32_t u32DMAMode);
void AES_SetKey(uint32_t u32Channel, uint32_t au32Keys[], uint32_t u32KeySize);
void AES_SetInitVect(uint32_t u32Channel, uint32_t au32IV[]);
void AES_SetDMATransfer(uint32_t u32Channel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32TransCnt);
void TDES_Open(uint32_t u32Channel, uint32_t u32EncDec, int Is3DES, int Is3Key, uint32_t u32OpMode, uint32_t u32SwapType);
void TDES_Start(int32_t u32Channel, uint32_t u32DMAMode);
void TDES_SetKey(uint32_t u32Channel, uint8_t au8Keys[3][8]);
void TDES_SetInitVect(uint32_t u32Channel, uint32_t u32IVH, uint32_t u32IVL);
void TDES_SetDMATransfer(uint32_t u32Channel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32TransCnt);
void SHA_Open(uint32_t u32OpMode, uint32_t u32SwapType, int hmac_key_len);
void SHA_Start(uint32_t u32DMAMode);
void SHA_SetDMATransfer(uint32_t u32SrcAddr, uint32_t u32TransCnt);
void SHA_Read(uint32_t u32Digest[]);
void ECC_DriverISR(void);
int  ECC_GeneratePublicKey(E_ECC_CURVE ecc_curve, char *private_k, char *public_k1, char *public_k2);
int  ECC_GenerateSignature(E_ECC_CURVE ecc_curve, char *message, char *d, char *k, char *R, char *S);
int  ECC_VerifySignature(E_ECC_CURVE ecc_curve, char *message, char *public_k1, char *public_k2, char *R, char *S);


/*@}*/ /* end of group M480_CRYPTO_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group M480_CRYPTO_Driver */

/*@}*/ /* end of group M480_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif  // __CRYPTO_H__

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/

