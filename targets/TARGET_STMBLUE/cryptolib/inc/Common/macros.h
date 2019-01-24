/**
  ******************************************************************************
  * @file    macros.h
  * @author  MCD Application Team
  * @version V2.2
  * @date    24-April-2015
  * @brief   Support Macros for functions of the Cryptolib
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http:/\/www.st.com/software_license_agreement_liberty_v2
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
#ifndef CRYPTOLIB__CRL_MACROS_H__
#define CRYPTOLIB__CRL_MACROS_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/** @addtogroup InternalAPI Internal functions
  * @{
  */

/** @addtogroup Macros Macros
  * @{
  */

/**
  * @brief  This macro returns the number of 32-bit words required
  *                    to store a variable whose byte size is passed in input
  * @param  b Number of bytes
  * @retval Number of words required to store a variable of size b
  */
#define WORD_COUNT(B) ((int32_t) ((uint32_t) (B) + 3u) / 4u)

/**
  * @brief  This macro returns the maximum value between two inputs,
  *                    the evaluation is done with the ">" operator
  * @param  a  First input
  * @param  b  Second input
  * @retval Max(a,b)
  */
//#define MAX(a,b)        ((((uint32_t)(a)) > ((uint32_t)(b)))  ? (a)  : (b))

/**
  * @brief  This macro returns the minumum value between two inputs,
  *                     the evaluation is done with the ">" operator
  * @param  a  First input
  * @param  b  Second input
  * @retval Min(a,b)
  */
//#define MIN(a,b)        ((((uint32_t)(a)) > ((uint32_t)(b)))  ? (b)  : (a))


/**
  * @brief  This macro is used to rotate left a 32bit variable by bits bits
  * @param  value The 32 bit value to rotate left
  * @param  bits  The number of bits to rotate left
  * @retval Roteted 32 bit value
  */
#ifdef CL_ON_STM32
#define ROL(value, bits) (uint32_t) __ROR((value),32 - (bits))
#else
#define ROL(VALUE, BITS) (uint32_t) (((VALUE) << (BITS)) | ((VALUE) >> (32 - (BITS))))
#endif

/**
  * @brief  This macro is used to rotate right a 32bit variable by bits bits
  * @param  value The 32 bit value to rotate right
  * @param  bits  The number of bits to rotate right
  * @retval Roteted 32 bit value
  */
#ifdef CL_ON_STM32
#define ROR(value, bits) (uint32_t) __ROR((value),(bits))
#else
#define ROR(VALUE, BITS) (uint32_t) (((VALUE) >> (BITS)) | ((VALUE) << (32 - (BITS))))
#endif

/**
  * @brief  This macro returns the least significant byte from an int32_t
  * @param  P_word The 32-bit from which the least significant byte is taken
  * @retval Least significant byte of P_word
  */
#define BYTE_3(P_word) (uint8_t) ((P_word) & 0xFFu)

/**
  * @brief  This macro returns the second least significant byte from an int32_t
  * @param  P_word The 32-bit from which the second least significant byte is taken
  * @retval Second least significant byte of P_word
  */
#define BYTE_2(P_word) (uint8_t) (((P_word) >> 8) & 0xFFu)

/**
  * @brief  This macro returns the second most significant byte from an int32_t
  * @param  P_word The 32-bit from which the second most significant byte is taken
  * @retval Second most significant byte of P_word
  */
#define BYTE_1(P_word) (uint8_t) (((P_word) >> 16) & 0xFFu)

/**
  * @brief  This macro returns the most significant byte from an int32_t
  * @param  P_word The 32-bit from which the most significant byte is taken
  * @retval Most significant byte of x
  */
#define BYTE_0(P_word) (uint8_t) (((P_word) >> 24) & 0xFFu)

/**
  * @brief  This macro returns a byte within an word
  * @param   P_word The 32-bit from which the byte
  * @param   P_n The index of the byte to be taken, 0 = MSB, 3 = LSB
  * @retval  Selected P_n byte from P_word
  */
#define BYTE_X(P_word, P_n) (uint8_t) (((P_word) >> (24 - (8 * (P_n)))) & 0xFFu)


/**
  * @brief   This macro outputs the 4 octects that form the input 32bit integer
  * @param   [in]    P_x The input 32bit integer
  * @param   [out]   P_a The least significant byte of P_x
  * @param   [out]   P_b The second least significant byte of P_x
  * @param   [out]   P_c The second most significant byte of P_x
  * @param   P_d The most significant byte of P_x
  * @retval  none
  */
#define WORD32_TO_WORD8(P_x,P_a,P_b,P_c,P_d) (P_a)=(uint8_t)BYTE_3(P_x),(P_b)=(uint8_t)BYTE_2(P_x),(P_c)=(uint8_t)BYTE_1(P_x),(P_d)=(uint8_t)BYTE_0(P_x)


/**
  * @brief   This macro returns an integer from 4 octects
  * @param   P_b0 The most significant byte of the resulting integer
  * @param   P_b1 The second most byte of the resulting integer
  * @param   P_b2 The second least byte of the resulting integer
  * @param   P_b3 The least byte of the resulting integer
  * @retval  The resulting 32bit integer formed by P_b0 || P_b1 || P_b2 || P_b3
  */
#define WORD8_TO_WORD32(P_b0, P_b1, P_b2, P_b3) (uint32_t) ((uint32_t)(P_b0) << 24 | (uint32_t)(P_b1) << 16 | (uint32_t)(P_b2) << 8 | (P_b3))

#if CRL_ENDIANNESS==1

/**
  * @brief   This macro is used on little endian cpus to convert a
  *                        4 byte array into a 32 bit integer or viceversa.
  * @param   VariableToConvert The 32 bits value to be converted
  * @retval  The converted 32 bits value
  */
#ifdef CL_ON_STM32
#define LE_CONVERT_W32(VariableToConvert) (uint32_t) (__REV(VariableToConvert))
#else
#define LE_CONVERT_W32(VariableToConvert) (uint32_t) ((ROL( (uint32_t) (VariableToConvert),24) & 0xFF00FF00u ) | (ROL( (uint32_t) (VariableToConvert),8) & 0x00FF00FFu ))
#endif

#elif CRL_ENDIANNESS==2

/**
  * @brief   This macro doesn't do anything. It exist for compatibility with
  *                        little endian cpus.
  * @param   VariableToConvert The 32 bit value to be converted
  * @retval  VariableToConvert as it was passed
  * @note This macro simply returns its argument as it is.
  */
#define LE_CONVERT_W32(VariableToConvert) (VariableToConvert)
#else  /* CRL_ENDIANNESS NOT SET */
#error "Please select endianness in config.h"
#endif /* #if CRL_ENDIANNESS */


/* Support macro for rand.c for big endian cpus */
#if CRL_ENDIANNESS==2

/**
  * @brief   This macro is used on big endian cpus to convert a
  *                        a 32 bit integer into a little endian array of 4 bytes.
  * @param   VariableToConvert The 32 bits value to be converted
  * @retval  The converted 32 bits value
  */
#define BE_CONVERT_W32(VariableToConvert) ((ROL(VariableToConvert,24)&0xFF00FF00) | (ROL(VariableToConvert,8)&0x00FF00FF))
#endif

#ifdef CRL_CPU_SUPPORT_MISALIGNED

/**
  * @brief   This macro is used to read a 4 bytes from a byte array and return
  *                        it as a 32-bit word, taking care of system endianness
  * @param   P_pInputBuffer Pointer to the input byte buffer
  * @param   P_index        Index of P_pInputBuffer where the reading should start
  * @retval  The 32 bits value
  * @note    This macro is for systems where unaligned reading is allowed
  */
#define BUFFER_2_W32(P_pInputBuffer,P_index)   (LE_CONVERT_W32(*((const uint32_t *) ((P_pInputBuffer)+(P_index)))))

/**
  * @brief   This macro is used to write a 32bit integer into a byte buffer,
  *                        taking care of system endianness
  * @param   [out]   P_pOutputBuffer Pointer to the output byte buffer
  * @param   [in]    P_index         Index of P_pOutputBuffer where the writing should start
  * @param   [in]    P_word          The 32-bit word that will be written
  * @retval  none
  * @note    This macro is for systems where unaligned writing is allowed
  */
#define W32_2_BUFFER(P_pOutputBuffer,P_index,P_word) *((uint32_t*)(P_pOutputBuffer+(P_index)))=LE_CONVERT_W32(P_word)
#else

/**
  * @brief   This macro is used to read a 4 bytes from a byte array and return
  *                        it as a 32-bit word
  * @param   P_pInputBuffer Pointer to the input byte buffer
  * @param   P_index        Index of P_pInputBuffer where the reading should start
  * @retval  The 32 bits value
  * @note    This macro is for systems where unaligned reading is \b not allowed
  */
#define BUFFER_2_W32(P_pInputBuffer,P_index)   (WORD8_TO_WORD32((P_pInputBuffer)[(P_index)],(P_pInputBuffer)[(P_index)+1],(P_pInputBuffer)[(P_index)+2],(P_pInputBuffer)[(P_index)+3]))

#define BUFFER_LE_2_W32_BE(P_pInputBuffer,P_index)   (WORD8_TO_WORD32((P_pInputBuffer)[(P_index)+3],(P_pInputBuffer)[(P_index)+2],(P_pInputBuffer)[(P_index)+1],(P_pInputBuffer)[(P_index)+0]))


/**
  * @brief   This macro is used to write a 32bit integer into a byte buffer,
  *                        taking care of system endianness
  * @param   [out]   P_pOutputBuffer Pointer to the output byte buffer
  * @param   [in]    P_index         Index of P_pOutputBuffer where the writing should start
  * @param   [in]    P_word          The 32-bit word that will be written
  * @retval  none
  * @note    This macro is for systems where unaligned writing is \b  not allowed
  */
#define W32_2_BUFFER(P_pOutputBuffer,P_index,P_word) WORD32_TO_WORD8((P_word),(P_pOutputBuffer)[(P_index)+3],(P_pOutputBuffer)[(P_index)+2],(P_pOutputBuffer)[(P_index)+1],(P_pOutputBuffer)[(P_index)])

#define W32_BE_2_BUFFER_LE(P_pOutputBuffer,P_index,P_word) WORD32_TO_WORD8((P_word),(P_pOutputBuffer)[(P_index)+0],(P_pOutputBuffer)[(P_index)+1],(P_pOutputBuffer)[(P_index)+2],(P_pOutputBuffer)[(P_index)+3])

#endif

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*CRYPTOLIB__CRL_MACROS_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
