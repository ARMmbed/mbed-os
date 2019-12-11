/** 
 *******************************************************************************
 * @file    txz_adc_include.h
 * @brief   This file provides internal common definition.
 * @version V1.0.0.0
 * $Date:: 2017-09-07 13:52:12 #$
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
#ifndef __TXZ_ADC_INCLUDE_H
#define __TXZ_ADC_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_driver_def.h"
#include "txz_hal.h"

/**
 *  @addtogroup Periph_Driver Peripheral Driver
 *  @{
 */

/** 
 *  @defgroup ADC ADC
 *  @brief    ADC Driver.
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup ADC_Private_define ADC Private Define
 *  @{
 */

/** 
 *  @defgroup NULL_Pointer NULL Pointer
 *  @brief    NULL Pointer.
 *  @{
 */
#define ADC_NULL        ((void *)0)                     /*!< NULL Pointer. */
/**
 *  @}
 */ /* End of group NULL_Pointer */

/** 
 *  @defgroup Parameter_Result Parameter Result
 *  @brief    Whether the parameter is specified or not.
 *  @{
 */
#define ADC_PARAM_OK             ((int32_t)1)              /*!< Parameter is valid(specified).         */
#define ADC_PARAM_NG             ((int32_t)0)              /*!< Parameter is invalid(not specified).   */
/**
 *  @}
 */ /* End of group Parameter_Result */

/** 
 *  @defgroup ADxCR0 ADxCR0
 *  @brief    ADxCR0 Register.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31-8  | -          |
 *  | 7     | ADEN       |
 *  | 6-3   | -          |
 *  | 2     | HPSGL      |
 *  | 1     | SGL        |
 *  | 0     | CNT        |
 *  @{
 */
/* ADEN  */
#define ADxCR0_ADEN_DISABLE         ((uint32_t)0x00000000)    /*!< ADEN   :Disable       */
#define ADxCR0_ADEN_ENABLE          ((uint32_t)0x00000080)    /*!< ADEN   :Enable        */
/* HPSGL */
#define ADxCR0_HPSGL_ENABLE         ((uint32_t)0x00000004)    /*!< HPSGL  :Enable        */
/* SGL   */
#define ADxCR0_SGL_ENABLE           ((uint32_t)0x00000002)    /*!< SGL    :Enable        */
/* CNT   */
#define ADxCR0_CNT_MASK             ((uint32_t)0x00000001)    /*!< CNT    :Mask          */
#define ADxCR0_CNT_DISABLE          ((uint32_t)0x00000000)    /*!< CNT    :Disable       */
#define ADxCR0_CNT_ENABLE           ((uint32_t)0x00000001)    /*!< CNT    :Enable        */
/**
 *  @}
 */ /* End of group ADxCR0 */

/** 
 *  @defgroup ADxCR1 ADxCR1
 *  @brief    ADxCR1 Register.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31-8  | -          |
 *  | 7     | HPDMEN     |
 *  | 6     | CNTDMEN    |
 *  | 5     | SGLDMEN    |
 *  | 4     | TRGDMEN    |
 *  | 3:2   | -          |
 *  | 1     | HPTRGEN    |
 *  | 0     | TRGEN      |
 *  @{
 */
/* HPDMEN  */
#define ADxCR1_HPDMEN_DISABLE       ((uint32_t)0x00000000)    /*!< HPDMEN :Disable      */
#define ADxCR1_HPDMEN_ENABLE        ((uint32_t)0x00000080)    /*!< HPDMEN :Enable       */
/* CNTDMEN  */
#define ADxCR1_CNTDMEN_DISABLE      ((uint32_t)0x00000000)    /*!< CNTDMEN :Disable      */
#define ADxCR1_CNTDMEN_ENABLE       ((uint32_t)0x00000040)    /*!< CNTDMEN :Enable       */
/* SGLDMEN  */
#define ADxCR1_SGLDMEN_DISABLE      ((uint32_t)0x00000000)    /*!< SGLDMEN :Disable      */
#define ADxCR1_SGLDMEN_ENABLE       ((uint32_t)0x00000020)    /*!< SGLDMEN :Enable       */
/* TRGDMEN  */
#define ADxCR1_TRGDMEN_DISABLE      ((uint32_t)0x00000000)    /*!< TRGDMEN :Disable      */
#define ADxCR1_TRGDMEN_ENABLE       ((uint32_t)0x00000010)    /*!< TRGDMEN :Enable       */
/* HPTRGEN    */
#define ADxCR1_HPTRGEN_DISABLE      ((uint32_t)0x00000000)    /*!< HPTRGEN :Disable      */
#define ADxCR1_HPTRGEN_ENABLE       ((uint32_t)0x00000002)    /*!< HPTRGEN :Enable       */
/* TRGEN    */
#define ADxCR1_TRGEN_DISABLE        ((uint32_t)0x00000000)    /*!< TRGEN   :Disable      */
#define ADxCR1_TRGEN_ENABLE         ((uint32_t)0x00000001)    /*!< TRGEN   :Enable       */
/**
 *  @}
 */ /* End of group ADxCR1 */

/** 
 *  @defgroup ADxST ADxST
 *  @brief    ADxST Register.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31-8  | -          |
 *  | 7     | ADBF       |
 *  | 6-4   | -          |
 *  | 3     | CNTF       |
 *  | 2     | SNGF       |
 *  | 1     | TRGF       |
 *  | 0     | HPF        |
 *  @{
 */
/* ADBF  */
#define ADxST_ADBF_MASK             ((uint32_t)0x00000080)    /*!< ADBF   :Mask.       */
#define ADxST_ADBF_IDLE             ((uint32_t)0x00000000)    /*!< ADBF   :Idle.      Can stop ADCLK. */
#define ADxST_ADBF_RUN              ((uint32_t)0x00000080)    /*!< ADBF   :Running. Can't stop ADCLK. */
/* CNTF  */
#define ADxST_CNTF_MASK             ((uint32_t)0x00000008)    /*!< CNTF   :Mask.       */
#define ADxST_CNTF_IDLE             ((uint32_t)0x00000000)    /*!< CNTF   :Idle.       */
#define ADxST_CNTF_RUN              ((uint32_t)0x00000008)    /*!< CNTF   :Running.    */
/* SNGF  */
#define ADxST_SNGF_MASK             ((uint32_t)0x00000004)    /*!< SNGF   :Mask.       */
#define ADxST_SNGF_IDLE             ((uint32_t)0x00000000)    /*!< SNGF   :Idle.       */
#define ADxST_SNGF_RUN              ((uint32_t)0x00000004)    /*!< SNGF   :Running.    */
/* TRGF  */
#define ADxST_TRGF_MASK             ((uint32_t)0x00000002)    /*!< TRGF   :Mask.       */
#define ADxST_TRGF_IDLE             ((uint32_t)0x00000000)    /*!< TRGF   :Idle.       */
#define ADxST_TRGF_RUN              ((uint32_t)0x00000002)    /*!< TRGF   :Running.    */
/* PMDF  */
#define ADxST_HPF_MASK              ((uint32_t)0x00000001)    /*!< HPF   :Mask.       */
#define ADxST_HPF_IDLE              ((uint32_t)0x00000000)    /*!< HPF   :Idle.       */
#define ADxST_HPF_RUN               ((uint32_t)0x00000001)    /*!< HPF   :Running.    */
/**
 *  @}
 */ /* End of group ADxST */

/** 
 *  @defgroup ADxMOD0 ADxMOD0.
 *  @brief    ADxMOD0 Register.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31-2  | -          |
 *  | 1     | RCUT       |
 *  | 0     | DACON      |
 *  @{
 */
/* RCUT  */
#define ADxMOD0_RCUT_NORMAL         ((uint32_t)0x00000000)    /*!< RCUT   :Normal        */
#define ADxMOD0_RCUT_IREF_CUT       ((uint32_t)0x00000002)    /*!< RCUT   :Iref cut      */
/* DACON */
#define ADxMOD0_DACON_OFF           ((uint32_t)0x00000000)    /*!< DACON  :DAC off       */
#define ADxMOD0_DACON_ON            ((uint32_t)0x00000001)    /*!< DACON  :DAC on        */
/**
 *  @}
 */ /* End of group ADxMOD0 */

/** 
 *  @name  ADxCMPEN Macro Definition.
 *  @brief ADxCMPEN Register Macro Definition.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31-4  | -          |
 *  | 3     | CMP3EN     |
 *  | 2     | CMP2EN     |
 *  | 1     | CMP1EN     |
 *  | 0     | CMP0EN     |
 *  @{
 */
/* CMP3EN  */
#define ADxCMPEN_CMP3EN_DISABLE     ((uint32_t)0x00000000)    /*!< CMP3EN :Disable       */
#define ADxCMPEN_CMP3EN_ENABLE      ((uint32_t)0x00000008)    /*!< CMP3EN :Enable        */
/* CMP2EN  */
#define ADxCMPEN_CMP2EN_DISABLE     ((uint32_t)0x00000000)    /*!< CMP2EN :Disable       */
#define ADxCMPEN_CMP2EN_ENABLE      ((uint32_t)0x00000004)    /*!< CMP2EN :Enable        */
/* CMP1EN  */
#define ADxCMPEN_CMP1EN_DISABLE     ((uint32_t)0x00000000)    /*!< CMP1EN :Disable       */
#define ADxCMPEN_CMP1EN_ENABLE      ((uint32_t)0x00000002)    /*!< CMP1EN :Enable        */
/* CMP0EN  */
#define ADxCMPEN_CMP0EN_DISABLE     ((uint32_t)0x00000000)    /*!< CMP0EN :Disable       */
#define ADxCMPEN_CMP0EN_ENABLE      ((uint32_t)0x00000001)    /*!< CMP0EN :Enable        */
/**
 *  @}
 */ /* End of name ADxCMPEN Macro Definition */

/** 
 *  @name  ADxTSETn Macro Definition.
 *  @brief ADxTSETn Register Macro Definition.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31-8  | -          |
 *  | 7     | ENINT0     |
 *  | 6-5   | TRGS0[1:0] |
 *  | 4-0   | AINST0[4:0]|
 *  @{
 */
/* ENINT0  */
#define ADxTSETn_ENINT_MASK         ((uint32_t)0x00000080)    /*!< ENINT :Mask              */
#define ADxTSETn_ENINT_DISABLE      ((uint32_t)0x00000000)    /*!< ENINT :Disable           */
#define ADxTSETn_ENINT_ENABLE       ((uint32_t)0x00000080)    /*!< ENINT :Enable            */
/* TRGS0[1:0]  */
#define ADxTSETn_TRGS_DISABLE       ((uint32_t)0x00000000)    /*!< TRGS  :Disable           */
#define ADxTSETn_TRGS_CNT           ((uint32_t)0x00000100)    /*!< TRGS  :Continuation      */
#define ADxTSETn_TRGS_SGL           ((uint32_t)0x00000200)    /*!< TRGS  :Single            */
#define ADxTSETn_TRGS_TRG           ((uint32_t)0x00000300)    /*!< TRGS  :Universal Trigger */
#define ADxTSETn_TRGS_PRI           ((uint32_t)0x00000400)    /*!< TRGS  :Priority Trigger */
/**
 *  @}
 */ /* End of name ADxTSETn Macro Definition */

/** 
 *  @name  ADxREGn Macro Definition.
 *  @brief ADxREGn Register Macro Definition.
 *  @details  Detail.
 *  | Bit   | Bit Symbol   |
 *  | :---  | :---         |
 *  | 31-30 | -            |
 *  | 29    | ADOVRF_Mn    |
 *  | 28    | ADRF_Mn      |
 *  | 27-16 | ADR_Mn[11:0] |
 *  | 15-4  | ADRn[11:0]   |
 *  | 3-2   | -            |
 *  | 1     | ADOVRFn      |
 *  | 0     | ADRFn        |
 *  @{
 */
/* ADOVRF_Mn */
#define ADxREGn_ADOVRF_Mn_MASK      ((uint32_t)0x20000000)    /*!< ADOVRF_Mn :Mask          */
#define ADxREGn_ADOVRF_Mn_OFF       ((uint32_t)0x00000000)    /*!< ADOVRF_Mn :Flag off.     */
#define ADxREGn_ADOVRF_Mn_ON        ((uint32_t)0x20000000)    /*!< ADOVRF_Mn :Flag on       */
/* ADRF_Mn   */
#define ADxREGn_ADRF_Mn_MASK        ((uint32_t)0x10000000)    /*!< ADRF_Mn   :Mask          */
#define ADxREGn_ADRF_Mn_OFF         ((uint32_t)0x00000000)    /*!< ADRF_Mn   :Flag off.     */
#define ADxREGn_ADRF_Mn_ON          ((uint32_t)0x10000000)    /*!< ADRF_Mn   :Flag on       */
/* ADR_Mn    */
#define ADxREGn_ADR_Mn_MASK         ((uint32_t)0x0FFF0000)    /*!< ADR_Mn    :Mask          */
/* ADRn      */
#define ADxREGn_ADRn_MASK           ((uint32_t)0x0000FFF0)    /*!< ADRn      :Mask          */
/* ADOVRFn   */
#define ADxREGn_ADOVRFn_MASK        ((uint32_t)0x00000002)    /*!< ADOVRF_Mn :Mask          */
#define ADxREGn_ADOVRFn_OFF         ((uint32_t)0x00000000)    /*!< ADOVRF_Mn :Flag off.     */
#define ADxREGn_ADOVRFn_ON          ((uint32_t)0x00000002)    /*!< ADOVRF_Mn :Flag on       */
/* ADRFn     */
#define ADxREGn_ADRFn_MASK          ((uint32_t)0x00000001)    /*!< ADRFn     :Mask          */
#define ADxREGn_ADRFn_OFF           ((uint32_t)0x00000000)    /*!< ADRFn     :Flag off.     */
#define ADxREGn_ADRFn_ON            ((uint32_t)0x00000001)    /*!< ADRFn     :Flag on       */
/**
 *  @}
 */ /* End of name ADxREGn Macro Definition */

/**
 *  @}
 */ /* End of group ADC_Private_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup ADC_Private_define ADC Private Define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group ADC_Private_define */


/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup ADC_Private_typedef ADC Private Typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group ADC_Private_typedef */

/*------------------------------------------------------------------------------*/
/*  Inline Functions                                                             */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup ADC_Private_fuctions ADC Private Fuctions
 *  @{
 */
/* no define */
/**
 *  @}
 */ /* End of group ADC_Private_functions */

/**
 *  @}
 */ /* End of group ADC */

/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __TXZ_ADC_INCLUDE_H */


