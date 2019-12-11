/** 
 *******************************************************************************
 * @file    txz_t32a.h
 * @brief   This file provides all the functions prototypes for T32A driver.
 * @version V1.0.0.7
 * $Date:: 2017-11-30 17:34:52 #$
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright(C) 2019, Toshiba Electronic Device Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Nuvoton Technology Corp. nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __T32A_H
#define __T32A_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_driver_def.h"
/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/** 
 *  @defgroup T32A T32A
 *  @brief    T32A Driver.
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup T32A_Exported_define T32A Exported Define
 *  @{
 */

/** 
 *  @defgroup T32A_Result  Result
 *  @brief    T32A Result Macro Definition.
 *  @{
 */
#define T32A_RESULT_SUCCESS     (0)          /*!< Success       */
#define T32A_RESULT_FAILURE     (-1)         /*!< Failure       */
#define T32A_READ_FAILURE       (0xFFFFFFFF) /*!< Failure       */
/**
 *  @}
 */ /* End of group T32A_Result */

/** 
 *  @defgroup T32A_NullPointer  Null Pointer
 *  @brief    Null Pointer.
 *  @{
 */
#define T32A_NULL        ((void *)0) /*!< NULL Pointer For T32A */
/**
 *  @}
 */ /* End of group T32A_NullPointer */

 /** 
 *  @defgroup T32A_HALT  T32A Debug HALT Control
 *  @brief    Debug HALT Control Run/Stop HALT Macro Definition.
 *  @{
 */
#define T32A_DBG_HALT_RUN               ((uint32_t)0x00000000)    /*!< Run */
#define T32A_DBG_HALT_STOP              ((uint32_t)0x00000002)    /*!< Stop  */
/**
 *  @}
 */ /* End of group T32A_HALT */

 /** 
 *  @defgroup T32A_MODE32  T32A 16bit/32bit MODE 
 *  @brief    T32A 16bit/32bit MODE MODE32 Macro Definition.
 *  @{
 */ 
#define T32A_MODE_16                    ((uint32_t)0x00000000)    /*!< 16bit Mode */
#define T32A_MODE_32                    ((uint32_t)0x00000001)    /*!< 32bit Mode  */
/**
 *  @}
 */ /* End of group T32A_MODE32 */

 /** 
 *  @defgroup T32A_RUNFLGx  T32A RUNFLG Control
 *  @brief    Run/Stop RUNFLGx Macro Definition.
 *  @{
 */
#define T32A_RUNFLG_RUN                 ((uint32_t)0x00000010)    /*!< Run */
#define T32A_RUNFLG_STOP                ((uint32_t)0x00000000)    /*!< Stop  */
/**
 *  @}
 */ /* End of group T32A_RUNFLGx */
 
/** 
 *  @defgroup T32A_SFTSTPx  T32A SW STOP Control
 *  @brief    T32A SW STOPx SFTSTPx Macro Definition.
 *  @{
 */
#define T32A_COUNT_DONT_STOP            ((uint32_t)0x0000000)    /*!< No effect */
#define T32A_COUNT_STOP                 ((uint32_t)0x0000004)    /*!< Counter Stop */
/**
 *  @}
 */ /* End of group T32A_SFTSTPx */

/** 
 *  @defgroup T32A_SFTSTAx  T32A SW START Control
 *  @brief    T32A SW STARTx SFTSTAx Macro Definition.
 *  @{
 */
#define T32A_COUNT_DONT_START           ((uint32_t)0x0000000)    /*!< No effect */
#define T32A_COUNT_START                ((uint32_t)0x0000002)    /*!< Counter Start */
/**
 *  @}
 */ /* End of group T32A_SFTSTAx */

/** 
 *  @defgroup T32A_RUNx     T32A RUN Disable/Enable Control
 *  @brief    RUN Disable/Enable RUNx Macro Definition.
 *  @{
 */
#define T32A_RUN_DISABLE                ((uint32_t)0x00000000)    /*!< Disable */
#define T32A_RUN_ENABLE                 ((uint32_t)0x00000001)    /*!< Enable  */
/**
 *  @}
 */ /* End of group T32A_RUNx */
 
 
/** 
 *  @defgroup T32A_PRSCLx  T32A PRESCALER Control
 *  @brief    PRESCALER Control PRSCLx Macro Definition.
 *  @{
 */
#define T32A_PRSCLx_1                  ((uint32_t)0x00000000)    /*!< 1/1 */
#define T32A_PRSCLx_2                  ((uint32_t)0x10000000)    /*!< 1/2 */
#define T32A_PRSCLx_8                  ((uint32_t)0x20000000)    /*!< 1/8 */
#define T32A_PRSCLx_32                 ((uint32_t)0x30000000)    /*!< 1/32 */
#define T32A_PRSCLx_128                ((uint32_t)0x40000000)    /*!< 1/128 */
#define T32A_PRSCLx_256                ((uint32_t)0x50000000)    /*!< 1/256 */
#define T32A_PRSCLx_512                ((uint32_t)0x60000000)    /*!< 1/512 */
#define T32A_PRSCLx_1024               ((uint32_t)0x70000000)    /*!< 1/1024 */
/**
 *  @}
 */ /* End of group T32A_PRSCLx */

/** 
 *  @defgroup T32A_CLKx  T32A COLCK Control
 *  @brief    CLOCK Control CLKA Macro Definition.
 *  @{
 */
#define T32A_CLKx_PRSCLx                ((uint32_t)0x00000000)    /*!< prescaler */
#define T32A_CLKx_INTRG                 ((uint32_t)0x01000000)    /*!< internal triger */
#define T32A_CLKx_TIM_RISING_EDGE       ((uint32_t)0x02000000)    /*!< other timer rising edge */
#define T32A_CLKx_TIM_TRAILING_EDGE     ((uint32_t)0x03000000)    /*!< other timer trailing edge */
#define T32A_CLKx_EXTTRG_RISING_EDGE    ((uint32_t)0x04000000)    /*!< external triger rising edge */
#define T32A_CLKx_EXTTRG_TRAILING_EDGE  ((uint32_t)0x05000000)    /*!< external triger trailing edge */
/**
 *  @}
 */ /* End of group T32A_CLKx */

/** 
 *  @defgroup T32A_WBFx  T32A Double Buffer Disable/Enable Control
 *  @brief    Double Buffer Disable/Enable WBFx Macro Definition.
 *  @{
 */
#define T32A_WBF_DISABLE                ((uint32_t)0x00000000)    /*!< Disable */
#define T32A_WBF_ENABLE                 ((uint32_t)0x00100000)    /*!< Enable  */
/**
 *  @}
 */ /* End of group T32A_WBFx */
 
 /** 
 *  @defgroup T32A_UPDNx  T32A Counter Up/Down Control
 *  @brief    Counter Up/Down Control UPDNx Macro Definition.
 *  @{
 */
#define T32A_COUNT_UP                   ((uint32_t)0x00000000)    /*!< count up */
#define T32A_COUNT_DOWN                 ((uint32_t)0x00010000)    /*!< count down  */
#define T32A_COUNT_UPDOWN               ((uint32_t)0x00020000)    /*!< count updown  */
#define T32A_COUNT_PLS                 ((uint32_t)0x00030000)    /*!< count pulse  */
/**
 *  @}
 */ /* End of group T32A_UPDNx */

 /** 
 *  @defgroup T32A_RELDx  T32A Counter Reload Control
 *  @brief    Counter Reload Control RELDx Macro Definition.
 *  @{
 */
#define T32A_RELOAD_NON                  ((uint32_t)0x00000000)    /*!< Nothing(Free run) */
#define T32A_RELOAD_INTRG                ((uint32_t)0x00000100)    /*!< internal trigger  */
#define T32A_RELOAD_EXTTRG_RISING_EDGE   ((uint32_t)0x00000200)    /*!< external trigger rising edge  */
#define T32A_RELOAD_EXTTRG_TRAILING_EDGE ((uint32_t)0x00000300)    /*!< external trigger trailing edge  */
#define T32A_RELOAD_TIM_RISING_EDGE      ((uint32_t)0x00000400)    /*!< other timer rising edge */
#define T32A_RELOAD_TIM_TRAILING_EDGE    ((uint32_t)0x00000500)    /*!< other timer trailing edge */
#define T32A_RELOAD_SYNC                 ((uint32_t)0x00000600)    /*!< sync(slave channel) */
#define T32A_RELOAD_TREGx                ((uint32_t)0x00000700)    /*!< match up Timer Register */
/**
 *  @}
 */ /* End of group T32A_RELDx */

 /** 
 *  @defgroup T32A_STOPx  T32A Counter Stop Control
 *  @brief    Counter Stop Control STOPx Macro Definition.
 *  @{
 */
#define T32A_STOP_NON                   ((uint32_t)0x00000000)    /*!< No use trigger */
#define T32A_STOP_INTRG                 ((uint32_t)0x00000010)    /*!< internal trigger  */
#define T32A_STOP_EXTTRG_RISING_EDGE    ((uint32_t)0x00000020)    /*!< external trigger rising edge  */
#define T32A_STOP_EXTTRG_TRAILING_EDGE  ((uint32_t)0x00000030)    /*!< external trigger trailing edge  */
#define T32A_STOP_TIM_RISING_EDGE       ((uint32_t)0x00000040)    /*!< other timer rising edge */
#define T32A_STOP_TIM_TRAILING_EDGE     ((uint32_t)0x00000050)    /*!< other timer trailing edge */
#define T32A_STOP_SYNC                  ((uint32_t)0x00000060)    /*!< sync(slave channel) */
#define T32A_STOP_TREGx                 ((uint32_t)0x00000070)    /*!< match up Timer Register A */
/**
 *  @}
 */ /* End of group T32A_STOPx */


 /** 
 *  @defgroup T32A_STARTx  T32A Counter Start Control
 *  @brief    Counter Start Control STARTx Macro Definition.
 *  @{
 */
#define T32A_START_NON                  ((uint32_t)0x00000000)    /*!< No use trigger */
#define T32A_START_INTRG                ((uint32_t)0x00000001)    /*!< internal trigger  */
#define T32A_START_EXTTRG_RISING_EDGE   ((uint32_t)0x00000002)    /*!< external trigger rising edge  */
#define T32A_START_EXTTRG_TRAILING_EDGE ((uint32_t)0x00000003)    /*!< external trigger trailing edge  */
#define T32A_START_TIM_RISING_EDGE      ((uint32_t)0x00000004)    /*!< other timer rising edge */
#define T32A_START_TIM_TRAILING_EDGE    ((uint32_t)0x00000005)    /*!< other timer trailing edge */
#define T32A_START_SYNC                 ((uint32_t)0x00000006)    /*!< sync(slave channel) */
#define T32A_START_Rsvd                 ((uint32_t)0x00000007)    /*!< Reserved */
/**
 *  @}
 */ /* End of group T32A_STARTx */

/** 
 *  @defgroup T32A_OCRx  T32AxOUTA Control
 *  @brief    T32AxOUTA Control OCRx Macro Definition.
 *  @{
 */
#define T32A_OCR_DISABLE                    ((uint32_t)0x00000000)    /*!< Nothing */
#define T32A_OCR_SET                    ((uint32_t)0x00000001)    /*!< Hi */
#define T32A_OCR_CLR                    ((uint32_t)0x00000002)    /*!< Low */
#define T32A_OCR_INVERSION              ((uint32_t)0x00000003)    /*!< inversion */
/**
 *  @}
 */ /* End of group T32A_OCRx */
 
/** 
 *  @defgroup T32A_OCRCAPx1  T32AxOUTA Control of T32AxCAPx1 T32AxRGx1
 *  @brief    T32AxOUTA Control of T32AxCAPx1 T32AxRGx1 OCRCAPx1 Macro Definition.
 *  @{
 */
#define T32A_OCRCAPx1_DISABLE               ((uint32_t)0x00000000)    /*!< No effect */
#define T32A_OCRCAPx1_SET               ((uint32_t)0x00000040)    /*!< Hi */
#define T32A_OCRCAPx1_CLR               ((uint32_t)0x00000080)    /*!< Low */
#define T32A_OCRCAPx1_INVERSION         ((uint32_t)0x000000C0)    /*!< inversion */
/**
 *  @}
 */ /* End of group T32A_OCRCAPx1 */

/** 
 *  @defgroup T32A_OCRCAPx0  T32AxOUTA Control of T32AxCAPx0 counter value
 *  @brief    T32AxOUTA Control of T32AxCAPx0 T32AxRGx1 OCRCAPx0 Macro Definition.
 *  @{
 */
#define T32A_OCRCAPx0_DISABLE               ((uint32_t)0x00000000)    /*!< No effect */
#define T32A_OCRCAPx0_SET               ((uint32_t)0x00000010)    /*!< Hi */
#define T32A_OCRCAPx0_CLR               ((uint32_t)0x00000020)    /*!< Low */
#define T32A_OCRCAPx0_INVERSION         ((uint32_t)0x00000030)    /*!< inversion */
/**
 *  @}
 */ /* End of group T32A_OCRCAPx0 */

/** 
 *  @defgroup T32A_OCRCMPx1  T32AxOUTA Control of T32AxRGx1 Counter Value
 *  @brief    T32AxOUTA Control of T32AxRGx1 Counter Value OCRCMPx1 Macro Definition.
 *  @{
 */
#define T32A_OCRCMPx1_DISABLE               ((uint32_t)0x00000000)    /*!< No effect */
#define T32A_OCRCMPx1_SET               ((uint32_t)0x00000004)    /*!< Hi */
#define T32A_OCRCMPx1_CLR               ((uint32_t)0x00000008)    /*!< Low */
#define T32A_OCRCMPx1_INVERSION         ((uint32_t)0x0000000C)    /*!< inversion */
/**
 *  @}
 */ /* End of group T32A_OCRCMPx1 */

/** 
 *  @defgroup T32A_OCRCMPx0  T32AxOUTA Control of T32AxRGx0 Counter Value
 *  @brief    T32AxOUTA Control of T32AxRGx0 Counter Value OCRCMPx0 Macro Definition.
 *  @{
 */
#define T32A_OCRCMPx0_DISABLE               ((uint32_t)0x00000000)    /*!< No effect */
#define T32A_OCRCMPx0_SET               ((uint32_t)0x00000001)    /*!< Hi */
#define T32A_OCRCMPx0_CLR               ((uint32_t)0x00000002)    /*!< Low */
#define T32A_OCRCMPx0_INVERSION         ((uint32_t)0x00000003)    /*!< inversion */
/**
 *  @}
 */ /* End of group T32A_OCRCMPx0 */

/** 
 *  @defgroup T32A_RGx0  T32A Timer Register x0 MASK
 *  @brief    T32A Timer Register A0 MASK RGx0 Macro Definition.
 *  @{
 */
#define T32A_RGx0_MASK                  ((uint32_t)0x0000FFFF)    /*!< register value mask */
#define T32A_RGC0_MASK                  ((uint32_t)0xFFFFFFFF)    /*!< register value mask */
/**
 *  @}
 */ /* End of group T32A_RGx0 */

/** 
 *  @defgroup T32A_RGx1  T32A Timer Register x1 MASK
 *  @brief    T32A Timer Register A1 MASK RGx1 Macro Definition.
 *  @{
 */
#define T32A_RGx1_MASK                  ((uint32_t)0x0000FFFF)    /*!< register value mask */
#define T32A_RGC1_MASK                  ((uint32_t)0xFFFFFFFF)    /*!< register value mask */
/**
 *  @}
 */ /* End of group T32A_RGx0 */

/** 
 *  @defgroup T32A_TMRx  T32A Counter Capture Register x MASK
 *  @brief    T32A Counter Capture Register x MASK TMRx Macro Definition.
 *  @{
 */
#define T32A_TMRx_MASK                  ((uint32_t)0x0000FFFF)    /*!< register value mask */
#define T32A_TMRC_MASK                  ((uint32_t)0xFFFFFFFF)    /*!< register value mask */
/**
 *  @}
 */ /* End of group T32A_TMRx */

/** 
 *  @defgroup T32A_RELD  T32A Counter Reload Register x MASK
 *  @brief    T32A Counter Reload Register x MASK TMRx Macro Definition.
 *  @{
 */
#define T32A_RELDx_MASK                 ((uint32_t)0x0000FFFF)    /*!< register value mask */
#define T32A_RELDC_MASK                 ((uint32_t)0xFFFFFFFF)    /*!< register value mask */
/**
 *  @}
 */ /* End of group T32A_RELD */

 /** 
 *  @defgroup T32A_CAPMx1  T32A Capture Control Register x1
 *  @brief    Capture Control Register A1 CAPMx1 Macro Definition.
 *  @{
 */
#define T32A_CAPMx1_DISABLE                 ((uint32_t)0x00000000)    /*!< No use trigger */
#define T32A_CAPMx1_INTRG               ((uint32_t)0x00000010)    /*!< internal trigger  */
#define T32A_CAPMx1_INx0_RISING_EDGE    ((uint32_t)0x00000020)    /*!< INx0 rising edge  */
#define T32A_CAPMx1_INx0_TRAILING_EDGE  ((uint32_t)0x00000030)    /*!< INx0 trailing edge */
#define T32A_CAPMx1_INx1_RISING_EDGE    ((uint32_t)0x00000040)    /*!< INx1 rising edge  */
#define T32A_CAPMx1_INx1_TRAILING_EDGE  ((uint32_t)0x00000050)    /*!< INx1 trailing edge  */
#define T32A_CAPMx1_TIM_RISING_EDGE     ((uint32_t)0x00000060)    /*!< other timer rising edge */
#define T32A_CAPMx1_TIM_TRAILING_EDGE   ((uint32_t)0x00000070)    /*!< other timer trailing edge */
/**
 *  @}
 */ /* End of group T32A_CAPMx1 */

 /** 
 *  @defgroup T32A_CAPMx0  T32A Capture Control Register x0
 *  @brief    Capture Control Register x0 CAPMx0 Macro Definition.
 *  @{
 */
#define T32A_CAPMx0_DISABLE                 ((uint32_t)0x00000000)    /*!< No use trigger */
#define T32A_CAPMx0_INTRG               ((uint32_t)0x00000001)    /*!< internal trigger  */
#define T32A_CAPMx0_INx0_RISING_EDGE    ((uint32_t)0x00000002)    /*!< INx0 rising edge  */
#define T32A_CAPMx0_INx0_TRAILING_EDGE  ((uint32_t)0x00000003)    /*!< INx0 trailing edge  */
#define T32A_CAPMx0_INx1_RISING_EDGE    ((uint32_t)0x00000004)    /*!< INx1 rising edge  */
#define T32A_CAPMx0_INx1_TRAILING_EDGE  ((uint32_t)0x00000005)    /*!< INx1 trailing edge  */
#define T32A_CAPMx0_TIM_RISING_EDGE     ((uint32_t)0x00000006)    /*!< other timer rising edge */
#define T32A_CAPMx0_TIM_TRAILING_EDGE   ((uint32_t)0x00000007)    /*!< other timer trailing edge */
/**
 *  @}
 */ /* End of group T32A_CAPMx0 */

/** 
 *  @defgroup T32A_CAPx0  T32A Capture Register x0 MASK
 *  @brief    T32A Capture Register x0 MASK CAPx0 Macro Definition.
 *  @{
 */
#define T32A_CAPx0_MASK                 ((uint32_t)0x0000FFFF)    /*!< register value mask */
#define T32A_CAPC0_MASK                 ((uint32_t)0xFFFFFFFF)    /*!< register value mask */
/**
 *  @}
 */ /* End of group T32A_CAPx0 */

/** 
 *  @defgroup T32A_CAPx1  T32A Capture Register x1 MASK
 *  @brief    T32A Capture Register x1 MASK CAPx1 Macro Definition.
 *  @{
 */
#define T32A_CAPx1_MASK                 ((uint32_t)0x0000FFFF)    /*!< register value mask */
#define T32A_CAPC1_MASK                 ((uint32_t)0xFFFFFFFF)    /*!< register value mask */
/**
 *  @}
 */ /* End of group T32A_CAPx1 */

/** 
 *  @defgroup T32A_IMSTERR T32A Statuserr Interrupt Request MASK
 *  @brief    T32A Statuserr Interrupt Request MASK IMSTERR Macro Definition.
 *  @{
 */
#define T32A_IMSTERR_MASK_NOREQ          ((uint32_t)0x00000000)
#define T32A_IMSTERR_MASK_REQ            ((uint32_t)0x00000010)
/**
 *  @}
 */ /* End of group T32A_IMSTERR */
 
/** 
 *  @defgroup T32A_IMUFx  T32A Underflow Interrupt Request MASK
 *  @brief    T32A Underflow Interrupt Request MASK IMUFx Macro Definition.
 *  @{
 */
#define T32A_IMUFx_MASK_NOREQ           ((uint32_t)0x00000000)    /*!< don't request */
#define T32A_IMUFx_MASK_REQ             ((uint32_t)0x00000008)    /*!< request */
/**
 *  @}
 */ /* End of group T32A_IMUFx */

/** 
 *  @defgroup T32A_IMOFx  T32A Overflow Interrupt Request MASK
 *  @brief    T32A Overflow Interrupt Request MASK IMOFx Macro Definition.
 *  @{
 */
#define T32A_IMOFx_MASK_NOREQ           ((uint32_t)0x00000000)    /*!< don't request */
#define T32A_IMOFx_MASK_REQ             ((uint32_t)0x00000004)    /*!< request */
/**
 *  @}
 */ /* End of group T32A_IMOFx */

/** 
 *  @defgroup T32A_IMx1  T32A Match Up T32AxRGx1 Interrupt Request MASK
 *  @brief    T32A Match Up T32AxRGx1 Interrupt Request MASK IMx1 Macro Definition.
 *  @{
 */
#define T32A_IMx1_MASK_NOREQ            ((uint32_t)0x00000000)    /*!< don't request */
#define T32A_IMx1_MASK_REQ              ((uint32_t)0x00000002)    /*!< request */
/**
 *  @}
 */ /* End of group T32A_IMx1 */

/** 
 *  @defgroup T32A_IMx0  T32A Match Up T32AxRGx0 Interrupt Request MASK
 *  @brief    T32A Match Up T32AxRGx0 Interrupt Request MASK IMx0 Macro Definition.
 *  @{
 */
#define T32A_IMx0_MASK_NOREQ            ((uint32_t)0x00000000)    /*!< don't request */
#define T32A_IMx0_MASK_REQ              ((uint32_t)0x00000001)    /*!< request */
/**
 *  @}
 */ /* End of group T32A_IMx0 */

/** 
 *  @defgroup T32A_INTSTERR T32A_Statuerr Flag Status
 *  @brief    T32A Statuserr Flag Status INTSTERR Macro Definition.
 *  @{
 */
#define T32A_INTSTERR_FLG_MASK           ((uint32_t)0x00000010)
#define T32A_INTSTERR_FLG_CLR            ((uint32_t)0x00000010)
/**
 *  @}
 */ /* End of group T32A_INTSTERR */

/** 
 *  @defgroup T32A_INTUFA  T32A Underflow Flag Status
 *  @brief    T32A Underflow Flag Status INTUFA Macro Definition.
 *  @{
 */
#define T32A_INTUFx_FLG_MASK            ((uint32_t)0x00000008)    /*!< Underflow Flag Mask */
#define T32A_INTUFx_FLG_CLR             ((uint32_t)0x00000008)    /*!< Underflow Flag Clear */
/**
 *  @}
 */ /* End of group T32A_INTUFA */

/** 
 *  @defgroup T32A_INTOFA  T32A Overflow Flag Status
 *  @brief    T32A Overflow Flag Status INTOFA Macro Definition.
 *  @{
 */
#define T32A_INTOFx_FLG_MASK            ((uint32_t)0x00000004)    /*!< Overflow Flag Mask */
#define T32A_INTOFx_FLG_CLR             ((uint32_t)0x00000004)    /*!< Overflow Flag Clear */
/**
 *  @}
 */ /* End of group T32A_INTOFA */

/** 
 *  @defgroup T32A_INTA1  T32A Match Up T32AxRGx1 Flag Status
 *  @brief    T32A Match Up T32AxRGx1 Flag Status INTA1 Macro Definition.
 *  @{
 */
#define T32A_INTx1_FLG_MASK             ((uint32_t)0x00000002)    /*!< Match Up T32AxRGx1 Flag Mask */
#define T32A_INTx1_FLG_CLR              ((uint32_t)0x00000002)    /*!< Match Up T32AxRGx1 Flag Clear */
/**
 *  @}
 */ /* End of group T32A_INTA1 */

/** 
 *  @defgroup T32A_INTA0  T32A Match Up T32AxRGx0 Flag Status
 *  @brief    T32A Match Up T32AxRGx0 Flag Status INTA0 Macro Definition.
 *  @{
 */
#define T32A_INTx0_FLG_MASK             ((uint32_t)0x00000001)    /*!< Match Up T32AxRGx0 Flag Mask */
#define T32A_INTx0_FLG_CLR              ((uint32_t)0x00000001)    /*!< Match Up T32AxRGx0 Flag Clear */
/**
 *  @}
 */ /* End of group T32A_INTA0 */

/** 
 *  @defgroup T32A_DMAENx2  T32A DMA Converter1 Request control
 *  @brief    T32A DMA Converter1 Disable/Enable DMAENx2 Macro Definition.
 *  @{
 */
#define T32A_DMAENx2_DISABLE            ((uint32_t)0x00000000)    /*!< disable */
#define T32A_DMAENx2_ENABLE             ((uint32_t)0x00000004)    /*!< enable */
/**
 *  @}
 */ /* End of group T32A_DMAENx2 */

/** 
 *  @defgroup T32A_DMAENx1  T32A DMA InputCapture1 Request control
 *  @brief    T32A DMA InputCapture1 Disable/Enable DMAENx1 Macro Definition.
 *  @{
 */
#define T32A_DMAENx1_DISABLE            ((uint32_t)0x00000000)    /*!< disable */
#define T32A_DMAENx1_ENABLE             ((uint32_t)0x00000002)    /*!< enable */
/**
 *  @}
 */ /* End of group T32A_DMAENx1 */

/** 
 *  @defgroup T32A_DMAENx0  T32A DMA InputCapture0 Request control
 *  @brief    T32A DMA InputCapture0 Disable/Enable DMAENx0 Macro Definition.
 *  @{
 */
#define T32A_DMAENx0_DISABLE            ((uint32_t)0x00000000)    /*!< disable */
#define T32A_DMAENx0_ENABLE             ((uint32_t)0x00000001)    /*!< enable */
/**
 *  @}
 */ /* End of group T32A_DMAENx0 */

 /** 
 *  @defgroup T32A_PDN  T32A Pulse Mode Count Down Control
 *  @brief    Pulse Mode Count Down Control PDN Macro Definition.
 *  @{
 */
#define T32A_PDN_NON0                   ((uint32_t)0x00000000)    /*!< Do not count down */
#define T32A_PDN_NON1                   ((uint32_t)0x00001000)    /*!< Do not count down */
#define T32A_PDN_INC0_RISING_EDGE       ((uint32_t)0x00002000)    /*!< T32AxINC0 rising edge  */
#define T32A_PDN_INC0_TRAILING_EDGE     ((uint32_t)0x00003000)    /*!< T32AxINC0 trailing edge  */
#define T32A_PDN_INC1_RISING_EDGE       ((uint32_t)0x00004000)    /*!< T32AxINC1 rising edge  */
#define T32A_PDN_INC1_TRAILING_EDGE     ((uint32_t)0x00005000)    /*!< T32AxINC1 trailing edge  */
#define T32A_PDN_INC0_BOTH_EDGE         ((uint32_t)0x00006000)    /*!< T32AxINC0 rising edge/trailing edge */
#define T32A_PDN_INC1_BOTH_EDGE         ((uint32_t)0x00007000)    /*!< T32AxINC1 rising edge/trailing edge */
/**
 *  @}
 */ /* End of group T32A_PDN */

 /** 
 *  @defgroup T32A_PUP  T32A Pulse Mode Count UP Control
 *  @brief    Pulse Mode Count UP Control PUP Macro Definition.
 *  @{
 */
#define T32A_PUP_NON0                   ((uint32_t)0x00000000)    /*!< Do not count up */
#define T32A_PUP_NON1                   ((uint32_t)0x00000100)    /*!< Do not count up */
#define T32A_PUP_INC0_RISING_EDGE       ((uint32_t)0x00000200)    /*!< T32AxINC0 rising edge  */
#define T32A_PUP_INC0_TRAILING_EDGE     ((uint32_t)0x00000300)    /*!< T32AxINC0 trailing edge  */
#define T32A_PUP_INC1_RISING_EDGE       ((uint32_t)0x00000400)    /*!< T32AxINC1 rising edge  */
#define T32A_PUP_INC1_TRAILING_EDGE     ((uint32_t)0x00000500)    /*!< T32AxINC1 trailing edge  */
#define T32A_PUP_INC0_BOTH_EDGE         ((uint32_t)0x00000600)    /*!< T32AxINC0 rising edge/trailing edge */
#define T32A_PUP_INC1_BOTH_EDGE         ((uint32_t)0x00000700)    /*!< T32AxINC1 rising edge/trailing edge */
/**
 *  @}
 */ /* End of group T32A_PUP */

 /** 
 *  @defgroup T32A_NF  T32A Noise Filter control
 *  @brief    Noise Filter control NF Macro Definition.
 *  @{
 */
#define T32A_NF_NON                     ((uint32_t)0x00000000)    /*!< Nothing */
#define T32A_NF_2                       ((uint32_t)0x00000010)    /*!< Noise Filter less than 2/ƒÓT0 */
#define T32A_NF_4                       ((uint32_t)0x00000020)    /*!< Noise Filter less than 4/ƒÓT0 */
#define T32A_NF_8                       ((uint32_t)0x00000030)    /*!< Noise Filter less than 8/ƒÓT0 */
/**
 *  @}
 */ /* End of group T32A_NF */
 
/** 
 *  @defgroup T32A_PDIR  T32A Phase 2 Pulse Direction control
 *  @brief    Phase 2 Pulse Direction control PDIR Macro Definition.
 *  @{
 */
#define T32A_PDIR_FORWARD               ((uint32_t)0x00000000)    /*!< forward */
#define T32A_PDIR_BACKWARD              ((uint32_t)0x00000002)    /*!< backward */
/**
 *  @}
 */ /* End of group T32A_PDIR */
 
/** 
 *  @defgroup T32A_PMODE  T32A Pulse Count Mode control
 *  @brief    Pulse Count Mode control PDIR Macro Definition.
 *  @{
 */
#define T32A_PMODE_PHASE_2              ((uint32_t)0x00000000)    /*!< Phase 2 Pulse Counter Mode */
#define T32A_PMODE_PHASE_1              ((uint32_t)0x00000001)    /*!< Phase 1 Pulse Counter Mode */
/**
 *  @}
 */ /* End of group T32A_PMODE */
 
/**
 *  @}
 */ /* End of group T32A_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** @defgroup T32A_Exported_Typedef T32A Exported Typedef
  * @{
  */

/** 
 *  @enum t32_type_t
 *  @brief    Use of Timer register.
 */
typedef enum
{
  T32A_TIMERA = 0,     /*!< 0: Timer A     */
  T32A_TIMERB,         /*!< 1: Timer B     */
  T32A_TIMERC,         /*!< 2: Timer C     */
  T32A_TIMERMAX,
}t32_type_t;

/** 
 *  @enum t32_regnum_t
 *  @brief    Use of Timer register number.
 */
typedef enum
{
  T32A_REG0 = 0,     /*!< 0: Register 0     */
  T32A_REG1,          /*!< 1: Register 1     */ 
  T32A_RELOAD,      /*!< 2: Reload Register     */
}t32_regnum_t;
/** 
 *  @enum t32_mode_t
 *  @brief    Use of Timer register.
 */
typedef enum
{
  T32A_MATCH = 0,     /*!< 0: compare match detection 0     */
  T32A_OVERFLOW,      /*!< 1: Overfloe detection     */
  T32A_UNDERFLOW,     /*!< 2: Underflow detection    */
  T32A_CAPTURE0,     /*!< 3: Capture 0              */
  T32A_CAPTURE1,     /*!< 4: Capture 0              */
}t32_mode_t;

/** 
 *  @enum t32_triger_t
 *  @brief    Use of Timer register.
 */
typedef enum
{
  T32A_INTRG = 0,                /*!< 0: internal triger     */
  T32A_TIM_RISING_EDGE,          /*!< 1: Same Channel other timer rising edge     */
  T32A_TIM_TRAILING_EDGE,        /*!< 2: Same Channel other timer trailing edge     */
  T32A_EXTTRG_RISING_EDGE,       /*!< 3: external triger rising edge              */
  T32A_EXTTRG_TRAILING_EDGE,     /*!< 4: external triger trailing edge              */
}t32_triger_t;
/**
 *  @} 
 */ /* End of group T32A_Exported_Typedef */
 
/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup T32A_Exported_Typedef T32A Exported Typedef
 *  @{
 */
/*----------------------------------*/
/** 
 * @struct t32a_mode_t
 * @brief  TimerA Mode Setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t halt;         /*!< T32A Debug HALT Control.
                                : Use @ref T32A_HALT */
    uint32_t mode;         /*!< T32A 16bit/32bit MODE .
                                : Use @ref T32A_MODE32 */
} t32a_mode_t;

/*----------------------------------*/
/** 
 * @struct t32a_runx_t
 * @brief  TimerA Run Control Setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t runflg;      /*!< TimerA Run Control Flag.
                                : Use @ref T32A_RUNFLGx */
    uint32_t sftstp;      /*!< SW Counter STOP Control.
                                : Use @ref T32A_SFTSTPx */
    uint32_t sftsta;      /*!< SW Counter START Control. 
                                : Use @ref T32A_SFTSTAx */
    uint32_t run;         /*!< TimerA Run Control. 
                                : Use @ref T32A_RUNx */
} t32a_runx_t;

/*----------------------------------*/
/** 
 * @struct t32a_crx_t
 * @brief  Counter Register Control Setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t prscl;       /*!< T32A PRESCALER Control.
                                : Use @ref T32A_PRSCLx */
    uint32_t clk;         /*!< T32A COLCK Control.
                                : Use @ref T32A_CLKx */
    uint32_t wbf;         /*!< T32A Double Buffer Disable/Enable Control. 
                                : Use @ref T32A_WBFx */
    uint32_t updn;        /*!< T32A Counter Up/Down Control. 
                                : Use @ref T32A_UPDNx */
    uint32_t reld;        /*!< T32A Counter Reload Control.
                                : Use @ref T32A_RELDx */
    uint32_t stop;        /*!< T32A Counter Stop Control.
                                : Use @ref T32A_STOPx */
    uint32_t start;       /*!< T32A Counter Start Controlc. 
                                : Use @ref T32A_STARTx */
} t32a_crx_t;

/*----------------------------------*/
/** 
 * @struct t32a_outcrx0_t
 * @brief  TimerA Output Control Setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t ocr;         /*!< T32AxOUTA Control.
                                : Use @ref T32A_OCRx */
} t32a_outcrx0_t;

/*----------------------------------*/
/** 
 * @struct t32a_outcrx1_t
 * @brief  T32AxOUTA Control Setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t ocrcap1;         /*!< T32AxOUTA Control of T32AxCAPx1 T32AxRGx1.
                                : Use @ref T32A_OCRCAPx1 */
    uint32_t ocrcap0;         /*!< T32AxOUTA Control of T32AxCAPx0 T32AxRGx1.
                                : Use @ref T32A_OCRCAPx0 */
    uint32_t ocrcmp1;         /*!< T32AxOUTA Control of T32AxRGx1 Counter Value
                                : Use @ref T32A_OCRCMPx1 */
    uint32_t ocrcmp0;         /*!< T32AxOUTA Control of T32AxRGx0 Counter Value
                                : Use @ref T32A_OCRCMPx0 */
} t32a_outcrx1_t;

/*----------------------------------*/
/** 
 * @struct t32a_capcrx_t
 * @brief  Capture Control Setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t capmx1;           /*!< T32A Capture Control Register x1.
                                : Use @ref T32A_CAPMx1 */
    uint32_t capmx0;           /*!< T32A Capture Control Register A0.
                                : Use @ref T32A_CAPMx0 */
} t32a_capcrx_t;

/*----------------------------------*/
/** 
 * @struct t32a_rgx0_t
 * @brief  T32A Timer Register x0 Setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t rgx0;             /*!< T32A Timer Register x0.
                                : Use @ref T32A_RGx0 */
} t32a_rgx0_t;

/*----------------------------------*/
/** 
 * @struct t32a_rgx1_t
 * @brief  T32A Timer Register x1 Setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t rgx1;             /*!< T32A Timer Register x1.
                                : Use @ref T32A_RGx1 */
} t32a_rgx1_t;

/*----------------------------------*/
/** 
 * @struct t32a_tmrx_t 
 * @brief  T32A Counter Capture Register A Setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t tmrx;             /*!< T32A Counter Capture Register x.
                                : Use @ref T32A_TMRx */
} t32a_tmrx_t;

/*----------------------------------*/
/** 
 * @struct t32a_reldx_t
 * @brief  T32A Counter Reload Register Setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t reld;             /*!< T32A Counter Reload Register.
                                : Use @ref T32A_RELD */
} t32a_reldx_t;

/*----------------------------------*/
/** 
 * @struct t32a_capx0_t
 * @brief  T32A Capture Register x0 Setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t capx0;             /*!< T32A Capture Register x0.
                                : Use @ref T32A_CAPx0 */
} t32a_capx0_t;

/*----------------------------------*/
/** 
 * @struct t32a_capx1_t
 * @brief  T32A Capture Register x0 Setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t capx1;             /*!< T32A Capture Register x1.
                                : Use @ref T32A_CAPx1 */
} t32a_capx1_t;

/*----------------------------------*/
/** 
 * @struct t32a_imx_t
 * @brief  Interrupt mask register Setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t imsterr;          /*!< T32A State Transition Err Interrupt Request MASK (Only use Timer C).
                                : Use @ref T32A_IMSTERR */
    uint32_t imuf;            /*!< T32A Underflow Interrupt Request MASK.
                                : Use @ref T32A_IMUFx */
    uint32_t imof;            /*!< T32A Underflow Interrupt Request MASK.
                                : Use @ref T32A_IMOFx */
    uint32_t imx1;             /*!< T32A Match Up T32AxRGx1 Interrupt Request MASK.
                                : Use @ref T32A_IMx1 */
    uint32_t imx0;             /*!< T32A Match Up T32AxRGx0 Interrupt Request MASK.
                                : Use @ref T32A_IMx0 */
} t32a_imx_t;

/*----------------------------------*/
/** 
 * @struct t32a_stx_t
 * @brief  Status register structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t intsterr;         /*!< T32A State Transition Err Flag Status (Only use Timer C).
                                : Use @ref T32A_INTSTERR */
    uint32_t intuf;            /*!< T32A Underflow Flag Status.
                                : Use @ref T32A_INTUFA */
    uint32_t intof;            /*!< T32A Overflow Flag Status.
                                : Use @ref T32A_INTOFA */
    uint32_t intx1;             /*!< T32A Match Up T32AxRGx1 Flag Status.
                                : Use @ref T32A_INTA1 */
    uint32_t intx0;             /*!< T32A Match Up T32AxRGx0 Flag Status.
                                : Use @ref T32A_INTA0 */
} t32a_stx_t;

/*----------------------------------*/
/** 
 * @struct t32a_dma_req_t
 * @brief  DMA Request register setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t dmaenx2;           /*!< T32A DMA Converter1 Request control.
                                : Use @ref T32A_DMAENx2 */
    uint32_t dmaenx1;           /*!< T32A DMA InputCapture1 Request control.
                                : Use @ref T32A_DMAENx1 */
    uint32_t dmaenx0;           /*!< T32A DMA InputCapture0 Request control.
                                : Use @ref T32A_DMAENx0 */
} t32a_dma_req_t;

/*----------------------------------*/
/** 
 * @struct t32a_pulse_cr_t
 * @brief  Pulse Count Control register setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t pdn;               /*!< Pulse Mode Count Down Control.
                                : Use @ref T32A_PDN */
    uint32_t pup;               /*!< Pulse Mode Count UP Control.
                                : Use @ref T32A_PUP */
    uint32_t nf;                /*!< Noise Filter control.
                                : Use @ref T32A_NF */
    uint32_t pdir;              /*!< Phase 2 Pulse Direction control.
                                : Use @ref T32A_PDIR */
    uint32_t pmode;             /*!< Pulse Count Mode control.
                                : Use @ref T32A_PMODE */
} t32a_pulse_cr_t;

/** 
 * @struct t32a_initial_setting_t
 * @brief  Initial Timer setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t            id;       /*!< ID: User value.             */
    t32a_runx_t       runx;       /*!< Timer Run Control Setting */
    t32a_crx_t        crx;        /*!< Counter Register Control Setting */
    t32a_outcrx0_t    outcrx0;    /*!< Timer Output Control Setting */
    t32a_outcrx1_t    outcrx1;    /*!< T32AxOUTx Control Setting */
    t32a_capcrx_t     capcrx;     /*!< Capture Control Setting */
    t32a_rgx0_t       rgx0;       /*!< T32A Timer Register x0 Setting */
    t32a_rgx1_t       rgx1;       /*!< T32A Timer Register x1 Setting */
    t32a_tmrx_t       tmrx;       /*!< T32A Counter Capture Register Setting */
    t32a_reldx_t      reldx;      /*!< T32A Counter Reload Register Setting */
    t32a_capx0_t      capx0;      /*!< T32A Capture Register x0 Setting */
    t32a_capx1_t      capx1;      /*!< T32A Capture Register x1 Setting */
    t32a_imx_t        imx;        /*!< Interrupt mask register Setting */
    t32a_dma_req_t    dma_req;    /*!< DMA Request register Setting */
    t32a_pulse_cr_t   pls_cr;     /*!< Pulse Count Control Register Setting (Only use Timer C) */
    void (*handler_T)(uint32_t id, uint32_t status, TXZ_Result result); /*!< Timer Event handler.            */
    void (*handler_TC0)(uint32_t id, uint32_t status, TXZ_Result result); /*!< Timer Cap0 Event handler. */
    void (*handler_TC1)(uint32_t id, uint32_t status, TXZ_Result result); /*!< Timer Cap1 Event handler. */
} t32a_initial_setting_t;

/** 
 * @struct t32a_initial_mode_t
 * @brief  Initial Mode setting structure definition.
*/
/*----------------------------------*/
typedef struct
{
    t32a_mode_t       mode;       /*!< Timer Mode Setting */
} t32a_initial_mode_t;


/*----------------------------------*/
/** 
 * @brief  T32A handle structure definition.
*/
/*----------------------------------*/
typedef struct t32a_handle
{
    TSB_T32A_TypeDef       *p_instance;            /*!< Registers base address.           */
    t32a_initial_mode_t    init_mode;              /*!< Timer Mode Initial Setting */
    t32a_initial_setting_t init[T32A_TIMERMAX];    /*!< Initial setting.                  */
}t32a_t;

/** @} */
/* End of group T32A_Exported_Types */
/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup T32A_Exported_functions T32A Exported Functions
 *  @{
 */
TXZ_Result t32a_mode_init(t32a_t *p_obj);
TXZ_Result t32a_timer_init(t32a_t *p_obj, uint32_t type);
TXZ_Result t32a_deinit(t32a_t *p_obj, uint32_t type);
TXZ_Result t32a_timer_stopIT(t32a_t *p_obj, uint32_t type);
TXZ_Result t32a_timer_startIT(t32a_t *p_obj, uint32_t type);
TXZ_Result t32a_SWcounter_start(t32a_t *p_obj, uint32_t type);
TXZ_Result t32a_SWcounter_stop(t32a_t *p_obj, uint32_t type);
TXZ_Result t32a_reg_set(t32a_t *p_obj, uint32_t type, uint32_t num, uint32_t value);
TXZ_Result t32a_tmr_read(t32a_t *p_obj, uint32_t type, uint32_t *p_val);
TXZ_Result t32a_get_status(t32a_t *p_obj, uint32_t *p_status, uint32_t type);
void t32a_timer_IRQHandler(t32a_t *p_obj);
void t32a_timer_cap0_IRQHandler(t32a_t *p_obj);
void t32a_timer_cap1_IRQHandler(t32a_t *p_obj);
TXZ_Result t32a_Calculator(uint32_t *p_value, uint32_t time, uint32_t prescaler, uint32_t prscl);
/**
 *  @}
 */ /* End of group T32A_Exported_functions */
 
/**
 *  @}
 */ /* End of group T32A */
 
/**
 *  @}
 */ /* End of group Periph_Driver */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __T32A_H */

