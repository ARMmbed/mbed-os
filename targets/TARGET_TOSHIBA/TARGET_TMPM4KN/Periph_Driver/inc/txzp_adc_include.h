/**
 *******************************************************************************
 * @file    txzp_adc_include.h
 * @brief   This file provides internal common definition.
 * @version V1.0.0
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __ADC_INCLUDE_H
#define __ADC_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txzp_driver_def.h"

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
#define ADC_NULL        ((void *)0)
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
 *  | 6-2   | -          |
 *  | 1     | SGL        |
 *  | 0     | CNT        |
 *  @{
 */
/* ADEN  */
#define ADxCR0_ADEN_MASK            ((uint32_t)0x00000080)    /*!< ADEN   :Mask          */
#define ADxCR0_ADEN_DISABLE         ((uint32_t)0x00000000)    /*!< ADEN   :Disable       */
#define ADxCR0_ADEN_ENABLE          ((uint32_t)0x00000080)    /*!< ADEN   :Enable        */
/* SGL   */
#define ADxCR0_SGL_MASK             ((uint32_t)0x00000002)    /*!< SGL    :Mask          */
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
 *  | 31-7  | -          |
 *  | 6     | CNTDMEN    |
 *  | 5     | SGLDMEN    |
 *  | 4     | TRGDMEN    |
 *  | 3:1   | -          |
 *  | 0     | TRGEN      |
 *  @{
 */
/* CNTDMEN  */
#define ADxCR1_CNTDMEN_MASK         ((uint32_t)0x00000040)    /*!< CNTDMEN :Mask         */
#define ADxCR1_CNTDMEN_DISABLE      ((uint32_t)0x00000000)    /*!< CNTDMEN :Disable      */
#define ADxCR1_CNTDMEN_ENABLE       ((uint32_t)0x00000040)    /*!< CNTDMEN :Enable       */
/* SGLDMEN  */
#define ADxCR1_SGLDMEN_MASK         ((uint32_t)0x00000020)    /*!< SGLDMEN :Mask         */
#define ADxCR1_SGLDMEN_DISABLE      ((uint32_t)0x00000000)    /*!< SGLDMEN :Disable      */
#define ADxCR1_SGLDMEN_ENABLE       ((uint32_t)0x00000020)    /*!< SGLDMEN :Enable       */
/* TRGDMEN  */
#define ADxCR1_TRGDMEN_MASK         ((uint32_t)0x00000010)    /*!< TRGDMEN :Mask         */
#define ADxCR1_TRGDMEN_DISABLE      ((uint32_t)0x00000000)    /*!< TRGDMEN :Disable      */
#define ADxCR1_TRGDMEN_ENABLE       ((uint32_t)0x00000010)    /*!< TRGDMEN :Enable       */
/* TRGEN    */
#define ADxCR1_TRGEN_MASK           ((uint32_t)0x00000001)    /*!< TRGEN   :Mask         */
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
 *  | 0     | PMDF       |
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
#define ADxST_PMDF_MASK             ((uint32_t)0x00000001)    /*!< PMDF   :Mask.       */
#define ADxST_PMDF_IDLE             ((uint32_t)0x00000000)    /*!< PMDF   :Idle.       */
#define ADxST_PMDF_RUN              ((uint32_t)0x00000001)    /*!< PMDF   :Running.    */
/**
 *  @}
 */ /* End of group ADxST */

/**
 *  @defgroup ADxCLK ADxCLK.
 *  @brief    ADxCLK Register.
 *  @details  Detail.
 *  | Bit    | Bit Symbol  |
 *  | :---   | :---        |
 *  | 31-12  | -           |
 *  | 11-8   | EXAZ1[3:0]  |
 *  | 7      | -           |
 *  | 6-3    | EXAZ0[3:0]  |
 *  | 2-0    | VADCLK[2:0] |
 *  @{
 */
/* EXAZ1[3:0]  */
#define ADxCLK_EXAZ1_MASK           ((uint32_t)0x00000F00)    /*!< EXAZ1   :Mask        */
#define ADxCLK_EXAZ1_0_96           ((uint32_t)0x00000100)    /*!< EXAZ1   :Set 0001    */
#define ADxCLK_EXAZ1_0_91_1_09      ((uint32_t)0x00000000)    /*!< EXAZ1   :Set 0000    */
/* EXAZ0[3:0]  */
#define ADxCLK_EXAZ0_MASK           ((uint32_t)0x00000078)    /*!< EXAZ1   :Mask        */
#define ADxCLK_EXAZ0_0_96           ((uint32_t)0x00000008)    /*!< EXAZ1   :Set 0001    */
#define ADxCLK_EXAZ0_0_91_1_09      ((uint32_t)0x00000000)    /*!< EXAZ1   :Set 0000    */
/* VADCLK[2:0] */
#define ADxCLK_VADCLK_MASK          ((uint32_t)0x00000007)    /*!< VADCLK :Mask        */
#define ADxCLK_VADCLK_4             ((uint32_t)0x00000000)    /*!< VADCLK :ADCLK/4     */
#define ADxCLK_VADCLK_8             ((uint32_t)0x00000001)    /*!< VADCLK :ADCLK/8     */
/**
 *  @}
 */ /* End of group ADxCLK */

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
/* RCUT    */
#define ADxMOD0_RCUT_MASK           ((uint32_t)0x00000002)    /*!< RCUT   :Mask          */
#define ADxMOD0_RCUT_NORMAL         ((uint32_t)0x00000000)    /*!< RCUT   :Normal        */
#define ADxMOD0_RCUT_IREF_CUT       ((uint32_t)0x00000002)    /*!< RCUT   :Iref cut      */
/* DACON   */
#define ADxMOD0_DACON_MASK          ((uint32_t)0x00000001)    /*!< DACON  :Mask          */
#define ADxMOD0_DACON_OFF           ((uint32_t)0x00000000)    /*!< DACON  :DAC off       */
#define ADxMOD0_DACON_ON            ((uint32_t)0x00000001)    /*!< DACON  :DAC on        */
/**
 *  @}
 */ /* End of group ADxMOD0 */


/**
 *  @defgroup ADxMOD1 ADxMOD1.
 *  @brief    ADxMOD1 Register.
 *  @{
 */
#define ADxMOD1_TIME_0_96_AVDD_4_5      ((uint32_t)0x00306122)    /*!< TIME 0.96u AVDD 4.5V  */
#define ADxMOD1_TIME_0_91_AVDD_4_5      ((uint32_t)0x00308012)    /*!< TIME 0.91u AVDD 4.5V  */
#define ADxMOD1_TIME_1_09_AVDD_4_5      ((uint32_t)0x00104011)    /*!< TIME 1.09u AVDD 4.5V  */
/**
 *  @}
 */ /* End of group ADxMOD1 */

/**
 *  @defgroup ADxMOD2 ADxMOD2.
 *  @brief    ADxMOD2 Register.
 *  @{
 */
#define ADxMOD2                         ((uint32_t)0x00000000)    /*Fixed Register Value     */
/**
 *  @}
 */ /* End of group ADxMOD2 */

/**
 *  @name  ADxCMPEN Macro Definition.
 *  @brief ADxCMPEN Register Macro Definition.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31-2  | -          |
 *  | 1     | CMP1EN     |
 *  | 0     | CMP0EN     |
 *  @{
 */
/* CMP1EN  */
#define ADxCMPEN_CMP1EN_MASK        ((uint32_t)0x00000002)    /*!< CMP1EN :Mask          */
#define ADxCMPEN_CMP1EN_DISABLE     ((uint32_t)0x00000000)    /*!< CMP1EN :Disable       */
#define ADxCMPEN_CMP1EN_ENABLE      ((uint32_t)0x00000002)    /*!< CMP1EN :Enable        */
/* CMP0EN  */
#define ADxCMPEN_CMP0EN_MASK        ((uint32_t)0x00000001)    /*!< CMP0EN :Mask          */
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
/* ENINT0       */
#define ADxTSETn_ENINT_MASK         ((uint32_t)0x00000080)    /*!< ENINT :Mask              */
#define ADxTSETn_ENINT_DISABLE      ((uint32_t)0x00000000)    /*!< ENINT :Disable           */
#define ADxTSETn_ENINT_ENABLE       ((uint32_t)0x00000080)    /*!< ENINT :Enable            */
/* TRGS0[1:0]   */
#define ADxTSETn_TRGS_MASK          ((uint32_t)0x00000060)    /*!< TRGS  :Mask              */
#define ADxTSETn_TRGS_DISABLE       ((uint32_t)0x00000000)    /*!< TRGS  :Disable           */
#define ADxTSETn_TRGS_CNT           ((uint32_t)0x00000020)    /*!< TRGS  :Continuation      */
#define ADxTSETn_TRGS_SGL           ((uint32_t)0x00000040)    /*!< TRGS  :Single            */
#define ADxTSETn_TRGS_TRG           ((uint32_t)0x00000060)    /*!< TRGS  :Universal Trigger */
/* AINST0[4:0]  */
#define ADxTSETn_AINST_MASK         ((uint32_t)0x0000001F)    /*!< AINST :Mask              */
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
#ifdef DEBUG
__STATIC_INLINE int32_t check_param_channel(uint32_t ch, uint32_t max);
__STATIC_INLINE int32_t check_param_ain(uint32_t ain, uint32_t min, uint32_t max);
__STATIC_INLINE int32_t check_param_continuity_num(uint32_t max, uint32_t num);
#endif

#ifdef DEBUG
/*--------------------------------------------------*/
/**
  * @brief  Check the Channel Number parameter.
  * @param  ch     :Channel Number
  * @param  max    :Channel Max
  * @retval ADC_PARAM_OK :Valid
  * @retval ADC_PARAM_NG :Invalid
  * @note   -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_channel(uint32_t ch, uint32_t max)
{
    int32_t result = ADC_PARAM_NG;

    if (ch < max) {
        result = ADC_PARAM_OK;
    }

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Check the AIN Range's parameter.
  * @param  ain      :AIN Range's parameter
  * @param  min      :Range Min.
  * @param  max      :Range Max.
  * @retval ADC_PARAM_OK :Valid
  * @retval ADC_PARAM_NG :Invalid
  * @note   -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_ain(uint32_t ain, uint32_t min, uint32_t max)
{
    int32_t result = ADC_PARAM_NG;

    if (min == 0) {
        if (ain <= max) {
            result = ADC_PARAM_OK;
        }
    } else {
        if ((min <= ain) && (ain <= max)) {
            result = ADC_PARAM_OK;
        }
    }

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Check the Max Num of continuity conversion's parameter.
  * @param  max    :Max Num of continuity conversion's parameter
  * @param  num    :Num of continuity conversion's parameter
  * @retval ADC_PARAM_OK :Valid
  * @retval ADC_PARAM_NG :Invalid
  * @note   -
  */
/*--------------------------------------------------*/
__STATIC_INLINE check_param_continuity_num(uint32_t max, uint32_t num)
{
    int32_t result = ADC_PARAM_NG;

    if (max >= num) {
        result = ADC_PARAM_OK;
    }

    return (result);
}
#endif
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
#endif /* __ADC_INCLUDE_H */


