/**
  ******************************************************************************
  * @file    types.h
  * @author  MCD Application Team
  * @version V2.2
  * @date    24-April-2015
  * @brief   Define the various basic types needed by the Library
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
#ifndef STACKLIB__CRL_TYPES_H__
#define STACKLIB__CRL_TYPES_H__



#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/** @addtogroup UserAPI User Level API
  * @{
  */

/** @addtogroup TYPES Data Types Definitions
  * @{
  */

#if !defined(_STDINT_H) && !defined(__stdint_h) && !defined(_STDINT)

#ifndef E__uint8_t_defined

typedef unsigned int uint32_t;       /*!< stdint.h defintion of uint32_t */
typedef unsigned char uint8_t;       /*!< stdint.h defintion of uint8_t */
typedef unsigned short int uint16_t; /*!< stdint.h defintion of uint16_t */
typedef unsigned long long uint64_t; /*!< stdint.h defintion of uint64_t */

#define E__uint8_t_defined
#define E__uint16_t_defined
#define E__uint32_t_defined
#define E__uint64_t_defined
#endif /*!<  __uint8_t_defined */

#ifndef E__int8_t_defined

typedef signed long long int64_t;  /*!< stdint.h defintion of int64_t */
typedef signed int int32_t;        /*!< stdint.h defintion of int32_t */
typedef signed short int int16_t;  /*!< stdint.h defintion of int16_t */
typedef signed char int8_t;        /*!< stdint.h defintion of int8_t */
#define E__int8_t_defined
#define E__int16_t_defined
#define E__int32_t_defined
#define E__int64_t_defined
#endif /* __int8_t_defined */

#endif /* !defined(_STDINT_H) && !defined(__stdint_h) && !defined(_STDINT) */

/** @brief Type definitation for a pre-allocated memory buffer that is required by some functions */
typedef struct 
{
  uint8_t *pmBuf;  /*!< Pointer to the pre-allocated memory buffer, this must be set by the user*/
  uint16_t  mSize; /*!< Total size of the pre-allocated memory buffer */
  uint16_t  mUsed; /*!< Currently used portion of the buffer, should be inititalized by user to zero */
}
membuf_stt;

/**
  * @}
  */

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif


#endif /* STACKLIB__CRL_TYPES_H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
