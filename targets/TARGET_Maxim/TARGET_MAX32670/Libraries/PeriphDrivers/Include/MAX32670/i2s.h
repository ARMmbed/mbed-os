/**
 * @file    i2s.h
 * @brief   I2S (Inter-Integrated Sound) driver function prototypes and data types.
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

#ifndef _I2S_H_
#define _I2S_H_

/* **** Includes **** */
#include "mxc_sys.h"
#include "dma.h"
#include "i2s_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup i2s Inter-Integrated Sound (I2S)
 * @ingroup periphlibs
 * @{
 */

/* **** Definitions **** */

/** @brief I2S stereo mode select */
typedef enum {
    MXC_I2S_MONO_LEFT_CH = 2,
    MXC_I2S_MONO_RIGHT_CH = 3
} mxc_i2s_stereo_t;

/** @brief I2S polarity configuration */
typedef enum {
    MXC_I2S_POL_NORMAL,
    MXC_I2S_POL_INVERSE
} mxc_i2s_polarity_t;

/** @brief I2S transaction bit order */
typedef enum {
    MXC_I2S_MSB_FIRST,
    MXC_I2S_LSB_FIRST
} mxc_i2s_bitorder_t;

/** @brief I2S transaction justify order */
typedef enum {
    MXC_I2S_MSB_JUSTIFY,
    MXC_I2S_LSB_JUSTIFY
} mxc_i2s_justify_t;

/** @brief  I2S transaction word size */
typedef enum {
    MXC_I2S_DATASIZE_BYTE,
    MXC_I2S_DATASIZE_HALFWORD,
    MXC_I2S_DATASIZE_WORD
} mxc_i2s_wsize_t;

/** @brief  I2S transaction sample size */
typedef enum {
    MXC_I2S_SAMPLESIZE_EIGHT,
    MXC_I2S_SAMPLESIZE_SIXTEEN,
    MXC_I2S_SAMPLESIZE_TWENTY,
    MXC_I2S_SAMPLESIZE_TWENTYFOUR,
    MXC_I2S_SAMPLESIZE_THIRTYTWO,
} mxc_i2s_samplesize_t;

/** @brief  I2S channel mode */
typedef enum {
    MXC_I2S_INTERNAL_SCK_WS_0,
    MXC_I2S_INTERNAL_SCK_WS_1,
    MXC_I2S_EXTERNAL_SCK_INTERNAL_WS,
    MXC_I2S_EXTERNAL_SCK_EXTERNAL_WS,
} mxc_i2s_ch_mode_t;

/** @brief I2S Configuration Struct */
typedef struct {
    mxc_i2s_ch_mode_t       channelMode;
    mxc_i2s_stereo_t        stereoMode;
    mxc_i2s_wsize_t         wordSize;
    mxc_i2s_justify_t       justify;
    mxc_i2s_bitorder_t      bitOrder;
    mxc_i2s_polarity_t      wsPolarity;
    mxc_i2s_samplesize_t    sampleSize;
    uint16_t                clkdiv;  
    void                    *rawData;
    void                    *txData;
    void                    *rxData;
    uint32_t                length;
} mxc_i2s_req_t;
    
/* **** Function Prototypes **** */

/**
 * @brief   Initialize I2S resources 
 * 
 * @param   req           see \ref mxc_i2s_req_t I2S Request Struct 
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2S_Init(mxc_i2s_req_t *req);

/**
 * @brief   Release I2S, clear configuration and flush FIFOs
 * 
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */  
int MXC_I2S_Shutdown(void);

/**
 * @brief   Configure data to be transmitted based on word and sample size
 * 
 * @param   req           see \ref mxc_i2s_req_t I2S Request Struct 
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.   
 */ 
int MXC_I2S_ConfigData(mxc_i2s_req_t *req);

/**
 * @brief   Enable TX channel
 */
void MXC_I2S_TXEnable(void);

/**
 * @brief   Disable TX channel
 */
void MXC_I2S_TXDisable(void);

/**
 * @brief   Enable RX channel
 */
void MXC_I2S_RXEnable(void);

/**
 * @brief   Disable RX channel
 */
void MXC_I2S_RXDisable(void);

/**
 * @brief   Set threshold for RX FIFO  
 * 
 * @param   threshold  RX FIFO interrupt threshold.
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2S_SetRXThreshold(uint8_t threshold);

/**
 * @brief   Set I2S Frequency, automatically called by I2S_Init
 * 
 * @param   mode    Channel mode to select clock
 * @param   clkdiv  clock divider to set baudrate
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */  
int MXC_I2S_SetFrequency(mxc_i2s_ch_mode_t mode, uint16_t clkdiv);

/**
 * @brief   Flush I2S FIFO
 * 
 */  
void MXC_I2S_Flush(void);

/**
 * @brief   Enable Interrupts
 * 
 * @param   flags   Interrupt mask 
 */
void MXC_I2S_EnableInt(uint32_t flags);

/**
 * @brief   Disable Interrupt
 * 
 * @param   flags   Interrupt mask
 */
void MXC_I2S_DisableInt(uint32_t flags);

/**
 * @brief   Get the set interrupt flags
 * 
 * @return  int     return the mask of the set interrupt flags     
 */
int MXC_I2S_GetFlags(void);

/**
 * @brief   Clears Interrupt Flags
 * 
 * @param   flags   Interrupt flags to be cleared
 */  
void MXC_I2S_ClearFlags(uint32_t flags);

/**
 * @brief   Configure TX DMA transaction
 * 
 * @param   src_addr    source address of data
 * @param   len         length od the data to be transmitted    
 */
void MXC_I2S_TXDMAConfig(void *src_addr, int len);

/**
 * @brief   Configure RX DMA transaction
 * 
 * @param   dest_addr   destination address
 * @param   len         length of the data to be received
 */
void MXC_I2S_RXDMAConfig(void *dest_addr, int len);

/**
 * @brief   Set the callback function pointer for I2S DMA transactions
 * 
 * @param   callback    Function pointer to the DMA callback function
 */
void MXC_I2S_RegisterDMACallback(void(*callback)(int, int));

#ifdef __cplusplus
}
#endif

#endif /* _I2S_H_ */
