/**
 * @file    crc.h
 * @brief   cyclic redundancy check driver.
 */

/* ****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 * 
 *************************************************************************** */

#ifndef _CRC_H_
#define _CRC_H_

/***** Includes *****/
#include "crc_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup crc CRC
 * @ingroup periphlibs
 * @{
 */

/***** CRC Definitions *****/
/**
  * @brief  Structure used to set up CRC request
  *
  */
typedef struct _mxc_crc_req_t {
    uint32_t* dataBuffer;     ///< Pointer to the data 
    uint32_t dataLen;         ///< Length of the data
    uint32_t resultCRC;       ///< Calculated CRC value
} mxc_crc_req_t;

/** 
 * @brief CRC data bit order
 *  
 */
typedef enum {
    CRC_LSB_FIRST,
    CRC_MSB_FIRST
} mxc_crc_bitorder_t;

/***** Function Prototypes *****/

/* ************************************************************************* */
/* Global Control/Configuration functions                                    */
/* ************************************************************************* */

/**
 * @brief   Enable portions of the CRC
 *
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_CRC_Init (void);

/**
 * @brief   Disable and reset portions of the CRC
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_CRC_Shutdown (void);

/**
 * @brief   This function should be called from the CRC ISR Handler
 *          when using Async functions
 * @param   ch      DMA channel
 * @param   error   error
 */
void MXC_CRC_Handler (int ch, int error);

/**
 * @brief   Set the bit-order of CRC calculation
 *
 * @param   bitOrder  The direction to perform CRC calculation in
 */
void MXC_CRC_SetDirection (mxc_crc_bitorder_t bitOrder);

/**
 * @brief   Set the bit-order of CRC calculation
 *
 * @return  The direction of calculation, 1 for MSB first, 0 for LSB first
 */
mxc_crc_bitorder_t MXC_CRC_GetDirection (void);

/**
 * @brief   Byte Swap CRC Data Input
 *
 * @param   bitOrder  The direction to perform CRC calculation in
 */
void MXC_CRC_SwapDataIn (mxc_crc_bitorder_t bitOrder);

/**
 * @brief   Byte Swap CRC Data output
 *
 * @param   bitOrder  The direction to perform CRC calculation in
 */
void MXC_CRC_SwapDataOut (mxc_crc_bitorder_t bitOrder);

/**
 * @brief   Set the Polynomial for CRC calculation
 *
 * @param   poly  The polynomial to use for CRC calculation
 */
void MXC_CRC_SetPoly (uint32_t poly);

/**
 * @brief   Get the polynomial for CRC calculation
 *
 * @return  The polynomial used in calculation
 */
uint32_t MXC_CRC_GetPoly (void);

/**
 * @brief   Get the result of a CRC calculation
 *
 * @return  The calculated CRC value
 */
uint32_t MXC_CRC_GetResult (void);

/*******************************/
/* High Level Functions        */
/*******************************/

/**
 * @brief   Perform a CRC computation
 * @note    The result of the CRC calculation will be placed in the
 *          mxc_crc_req_t structure
 *
 * @param   req   Structure containing the data for calculation
 *
 * @return  see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_CRC_Compute (mxc_crc_req_t* req);

/**
 * @brief   Perform a CRC computation using DMA
 * @note    The result of the CRC calculation will be placed in the
 *          mxc_crc_req_t structure. The user must call
 *          MXC_DMA_Handler() in the ISR
 *
 * @param   req   Structure containing the data for calculation
 * 
 * @return  see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_CRC_ComputeAsync (mxc_crc_req_t* req);

#ifdef __cplusplus
}
#endif
/**@} end of group crc */

#endif  /* _CRC_H_ */
