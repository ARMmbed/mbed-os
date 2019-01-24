/**
  ******************************************************************************
  * @file    aes_common.h
  * @author  MCD Application Team
  * @version V2.1
  * @date    22-June-2012
  * @brief   AES common functions and definitions
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
  *******************************************************************************/
#ifndef STACKLIB__AES_COMMON_H__
#define STACKLIB__AES_COMMON_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "sk.h"
#include "err_codes.h"
/*----------------- Types declaration -------------*/
/** @addtogroup AES
  * @{
  */

#define CRL_AES128_KEY   16 /*!< Number of bytes (uint8_t) necessary to store an AES key of 128 bits. */
#define CRL_AES128_EXPANDED_KEY  44 /*!< Number of ints (uint32_t) necessary to store an expanded AES key of 128 bits. */
#define CRL_AES192_KEY   24 /*!< Number of bytes (uint8_t) necessary to store an AES key of 192 bits. */
#define CRL_AES192_EXPANDED_KEY  52 /*!< Number of ints (uint32_t) necessary to store an expanded AES key of 192 bits. */
#define CRL_AES256_KEY   32 /*!< Number of bytes (uint8_t) necessary to store an AES key of 256 bits. */
#define CRL_AES256_EXPANDED_KEY  60 /*!< Number of ints (uint32_t) necessary to store an expanded AES key of 256 bits. */


#define CRL_AES_BLOCK     16 /*!< Number of bytes (uint8_t) necessary to store an AES block. */

#ifdef INCLUDE_AES256
# define CRL_AES_MAX_EXPKEY_SIZE 60 /*!< The max size of the AES expanded key (in uint32_t) according to the INCLUDE OPTIONS */
# else
#   ifdef INCLUDE_AES192
#     define CRL_AES_MAX_EXPKEY_SIZE 52 /*!< The max size of the AES expanded key (in uint32_t) according to the INCLUDE OPTIONS */
#   else
#     define CRL_AES_MAX_EXPKEY_SIZE 44 /*!< The max size of the AES expanded key (in uint32_t) according to the INCLUDE OPTIONS */
#   endif
#endif /*include aes 256 */

 /**
  * @}
  */

  
/** @addtogroup AESCBC
  * @{
  */  
typedef struct
{  
  uint32_t   mContextId; /*!< Unique ID of this context. \b Not \b used in current implementation. */  
  SKflags_et mFlags; /*!< 32 bit mFlags, used to perform keyschedule, choose betwen hw/sw/hw+dma and future use */  
  const uint8_t *pmKey; /*!< Pointer to original Key buffer */  
  const uint8_t *pmIv; /*!< Pointer to original Initialization Vector buffer */  
  int32_t   mIvSize; /*!< Size of the Initialization Vector in bytes */  
  uint32_t   amIv[4]; /*!< Temporary result/IV */  
  int32_t   mKeySize; /*!< Key length in bytes */
  uint32_t   amExpKey[CRL_AES_MAX_EXPKEY_SIZE]; /*!< Expanded AES key */
} AESCBCctx_stt; /*!< AES context structure for CBC mode */

 /**
  * @}
  */


/** @addtogroup AESlowlevel AES
  * @{
  */
  
#ifdef USE_HW_AES

typedef struct
{  
  uint32_t   mContextId;  /*!< Unique ID of this context. \b Not \b used in current implementation. */ 
  SKflags_et mFlags;  /*!< 32 bit mFlags, used to perform keyschedule, choose betwen hw/sw/hw+dma and future use */
  const uint8_t *pmKey; /*!< Pointer to original Key buffer */
  const uint8_t *pmIv;   /*!< Pointer to original Initialization Vector buffer */
  int32_t   mIvSize; /*!< Size of the Initialization Vector in bytes */
  uint32_t   amIv[4];   /*!< This is the current IV value. Attention should be made to not overflow this, as in DES it
                             has only 2 words */
} SKctx_stt;   /*!< AES and DES context structure can be safely casted to SKctx_stt for usage with hw engines
                    This features allows to not duplicate hw code for different algorithms */

#endif

 /**
  * @}
  */

  
#ifdef __cplusplus
}
#endif


/* MISRA 2012 Rule 8.4 */
void AES_keyschedule_enc(const uint8_t *P_pKey, uint32_t *P_pExpKey, int32_t P_keySize);
void AES_general_SW_enc(const uint32_t* P_pInputBuffer, uint32_t* P_pOutputBuffer, const uint32_t* P_pExpKey, int32_t P_keySize);

#endif  /*__AES_COMMON_H__*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
