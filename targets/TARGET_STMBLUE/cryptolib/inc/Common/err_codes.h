/**
  ******************************************************************************
  * @file    err_codes.h
  * @author  MCD Application Team
  * @version V2.2
  * @date    24-April-2015
  * @brief   Provides defines for error codes
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http:www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STACKLIB__CRL_ERR_CODES_H__
#define STACKLIB__CRL_ERR_CODES_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "types.h"
#include "err_codes.h"


/** @addtogroup UserAPI User Level API
  * @{
  */

/** @addtogroup ErrCodes Error Codes Definitions
  * @{
  */
  
/** @defgroup GenericError Generic Error Codes 
  * @{
  */  
  
#define AUTHENTICATION_SUCCESSFUL (int32_t) (1003)          /*!<  Authentication successful */
#define AUTHENTICATION_FAILED     (int32_t) (1004)          /*!<  Authentication failed */
#define SIGNATURE_VALID           AUTHENTICATION_SUCCESSFUL /*!<  Signature is valid */
#define SIGNATURE_INVALID         AUTHENTICATION_FAILED     /*!<  Signature is NOT valid */
#define ERR_MEMORY_FAIL (int32_t) (1005)      /*!<  Problems with dynamic allocation (there's no more available memory) */
#define DMA_BAD_ADDRESS     (int32_t) (2101)                /*!<  Used when pointer passed to DMA is not a multiple of word size as it should */
#define DMA_ERR_TRANSFER    (int32_t) (2102)                /*!<  Generic error during DMA transfer */
/**
  * @}
  */
  
/** @defgroup HWError Hardware Error Codes 
  * @{
  */ 
#define HW_SUCCESS        (int32_t) (0)      /*!<  CRYPTO HW ENGINES Success */
#define HW_BAD_INPUT_SIZE (int32_t) (2001)   /*!<  CRYPTO HW ENGINES Input Size is not multiple of AES block size */
/**
  * @}
  */
  
/** @defgroup AESError AES Error Codes 
  * @{
  */
#define AES_SUCCESS             (int32_t) (0)    /*!< AES of PRIVKEY Success */
#define AES_ERR_BAD_INPUT_SIZE  (int32_t) (3101) /*!< AES of PRIVKEY Invalid input size */
#define AES_ERR_BAD_OPERATION   (int32_t) (3102) /*!< AES of PRIVKEY Invalid operation */
#define AES_ERR_BAD_CONTEXT     (int32_t) (3103) /*!< AES of PRIVKEY The AES context contains some invalid or uninitialized values */
#define AES_ERR_BAD_PARAMETER   (int32_t) (3104) /*!< AES of PRIVKEY One of the expected function parameters is invalid */
/**
  * @}
  */
  
/** @defgroup ARC4Error ARC4 Error Codes
  * @{
  */
#define ARC4_SUCCESS             (int32_t) (0)    /*!< ARC4 of PRIVKEY Success*/
#define ARC4_ERR_BAD_OPERATION   (int32_t) (3202) /*!< ARC4 of PRIVKEY Invalid operation */
#define ARC4_ERR_BAD_CONTEXT     (int32_t) (3203) /*!< ARC4 of PRIVKEY The ARC4 context contains some invalid or uninitialized values */
#define ARC4_ERR_BAD_PARAMETER   (int32_t) (3204) /*!< ARC4 of PRIVKEY One of the expected function parameters is invalid */
/**
  * @}
  */
  
/** @defgroup DESError DES Error Codes
  * @{
  */
#define DES_SUCCESS             (int32_t) (0)    /*!<  DES of PRIVKEY Success */
#define DES_ERR_BAD_INPUT_SIZE  (int32_t) (3301) /*!<  DES of PRIVKEY Invalid input size, it must be multiple of 8 */
#define DES_ERR_BAD_OPERATION   (int32_t) (3302) /*!<  DES of PRIVKEY Invalid operation */
#define DES_ERR_BAD_CONTEXT     (int32_t) (3303) /*!<  DES of PRIVKEY The DES context contains some invalid or uninitialized values */
#define DES_ERR_BAD_PARAMETER   (int32_t) (3304) /*!<  DES of PRIVKEY One of the expected function parameters is invalid*/
/**
  * @}
  */
  
/** @defgroup TDESError TDES Error Codes
  * @{
  */
#define TDES_SUCCESS             (int32_t) (0)    /*!<  TDES of PRIVKEY Success */
#define TDES_ERR_BAD_INPUT_SIZE  (int32_t) (3311) /*!<  TDES of PRIVKEY Invalid input size, it must be multiple of 8 */
#define TDES_ERR_BAD_OPERATION   (int32_t) (3312) /*!<  TDES of PRIVKEY Invalid operation */
#define TDES_ERR_BAD_CONTEXT     (int32_t) (3313) /*!<  TDES of PRIVKEY The TDES context contains some invalid or uninitialized values */
#define TDES_ERR_BAD_PARAMETER   (int32_t) (3314) /*!<  TDES of PRIVKEY One of the expected function parameters is invalid */
/**
  * @}
  */
  
/** @defgroup HASHError HASH Error Codes
  * @{
  */
#define HASH_SUCCESS             (int32_t) (0)    /*!<  HASH Success */
#define HASH_ERR_BAD_OPERATION   (int32_t) (4001) /*!<  HASH Invalid operation */
#define HASH_ERR_BAD_CONTEXT     (int32_t) (4002) /*!<  HASH The HASH context contains some invalid or uninitialized values */
#define HASH_ERR_BAD_PARAMETER   (int32_t) (4003) /*!<  HASH One of the expected function parameters is invalid */
#define HASH_ERR_INTERNAL        (int32_t) (4011) /*!<  HASH Generic internal error */
/**
  * @}
  */
  
/** @defgroup RSAError RSA Error Codes
  * @{
  */
#define RSA_SUCCESS               (int32_t) (0)     /*!<  RSA Success */
#define RSA_ERR_BAD_OPERATION     (int32_t) (5102)  /*!<  RSA Invalid operation */
#define RSA_ERR_BAD_KEY           (int32_t) (5103)  /*!<  RSA Invalid Key */
#define RSA_ERR_BAD_PARAMETER     (int32_t) (5104)  /*!<  RSA One of the expected function parameters is invalid */
#define RSA_ERR_UNSUPPORTED_HASH  (int32_t) (5105)  /*!<  RSA The hash function is not supported */
#define RSA_ERR_MESSAGE_TOO_LONG  (int32_t) (5106)  /*!<  RSA Message too long */
#define RSA_ERR_MODULUS_TOO_SHORT (int32_t) (5107)  /*!<  RSA modulus too short */
#define RSA_ERR_GENERIC           (int32_t) (5108)  /*!<  RSA Generic Error */
/**
  * @}
  */
  
/** @defgroup ECCError ECC Error Codes
  * @{
  */
#define ECC_SUCCESS              (int32_t) (0)        /*!<  ECC Success */
#define ECC_ERR_BAD_OPERATION    (int32_t) (5202)     /*!<  ECC Invalid operation */
#define ECC_ERR_BAD_CONTEXT      (int32_t) (5203)     /*!<  ECC The ECC context contains some invalid or initialized parameters */
#define ECC_ERR_BAD_PARAMETER    (int32_t) (5204)     /*!<  ECC One of the expected function parameters is invalid */
#define ECC_ERR_BAD_PUBLIC_KEY   (int32_t) (5205)     /*!<  ECC Invalid Public Key */
#define ECC_ERR_BAD_PRIVATE_KEY  (int32_t) (5206)     /*!<  ECC Invalid Private Key */
#define ECC_ERR_MISSING_EC_PARAMETER (int32_t) (5207) /*!<  ECC The EC parameters structure miss some parameter required by the function */
#define ECC_WARN_POINT_AT_INFINITY   (int32_t) (5208) /*!<  ECC Returned Point is the point at infinity */
/**
  * @}
  */
    
/** @defgroup RNGError Random Number Error Codes
  * @{
  */
#define RNG_SUCCESS                  (int32_t) (0)    /*!<  RNG Success */
#define RNG_ERR_UNINIT_STATE         (int32_t) (6001) /*!<  RNG has not been correctly initialized */
#define RNG_ERR_BAD_OPERATION        (int32_t) (6002) /*!<  RNG Invalid operation */
#define RNG_ERR_RESEED_NEEDED        (int32_t) (6003) /*!<  RNG Reseed is needed */
#define RNG_ERR_BAD_PARAMETER        (int32_t) (6004) /*!<  RNG One of the expected function parameters is invalid */
#define RNG_ERR_BAD_ENTROPY_SIZE     (int32_t) (6006) /*!<  RNG Check the size of the entropy string */
#define RNG_ERR_BAD_PERS_STRING_SIZE (int32_t) (6007) /*!<  RNG Check the size of the personalization string */
#define RNG_ERR_BAD_ADD_INPUT_SIZE   (int32_t) (6008) /*!<  RNG Check the size of the additional input string */
#define RNG_ERR_BAD_REQUEST          (int32_t) (6009) /*!<  RNG Check the size of the random request */
#define RNG_ERR_BAD_NONCE_SIZE       (int32_t) (6010) /*!<  RNG Check the size of the nocne */
#define RNG_ERR_INTERNAL             (int32_t) (6011) /*!<  RNG Generic internal RNG error */

/** @defgroup MathError Mathemtical Error Codes
  * @{
  */
#define MATH_SUCCESS                (int32_t) (0)    /*!<  Math Success */
#define MATH_ERR_BIGNUM_OVERFLOW    (int32_t) (5301) /*!<  Math Overflow, the returned BigNum would be greater than its maximum size */                                                                 
#define MATH_ERR_EVEN_MODULUS       (int32_t) (5302) /*!<  Math This function can be used only with odd moduli */
#define MATH_ERR_BAD_PARAMETER      (int32_t) (5304) /*!<  Math One of the expected function parameters is invalid */
#define MATH_ERR_INTERNAL           (int32_t) (5311) /*!<  Math Generic internal error */
/**
  * @}
  */


 #ifdef __cplusplus
}
#endif

#endif /* __CRL_ERR_CODES_H__ */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
