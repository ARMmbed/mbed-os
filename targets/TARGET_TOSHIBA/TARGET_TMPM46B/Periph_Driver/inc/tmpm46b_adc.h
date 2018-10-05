/**
 *******************************************************************************
 * @file    tmpm46b_adc.h
 * @brief   This file provides all the functions prototypes for ADC driver.
 * @version V2.0.2.1
 * @date    2015/02/11
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM46B_ADC_H
#define __TMPM46B_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "TMPM46B.h"
#include "tx04_common.h"

/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @addtogroup ADC
  * @{
  */

/** @defgroup ADC_Exported_Types
  * @{
  */
#define IS_ADC_UNIT(param)          (((param) == TSB_AD))

#define ADC_CONVERSION_CLK_10           ((uint32_t)0x00000000)
#define ADC_CONVERSION_CLK_20           ((uint32_t)0x00000010)
#define ADC_CONVERSION_CLK_30           ((uint32_t)0x00000020)
#define ADC_CONVERSION_CLK_40           ((uint32_t)0x00000030)
#define ADC_CONVERSION_CLK_80           ((uint32_t)0x00000040)
#define ADC_CONVERSION_CLK_160          ((uint32_t)0x00000050)
#define ADC_CONVERSION_CLK_320          ((uint32_t)0x00000060)
#define IS_ADC_HOLD_TIME(param)         (((param) == ADC_CONVERSION_CLK_10) || \
                                         ((param) == ADC_CONVERSION_CLK_20) || \
                                         ((param) == ADC_CONVERSION_CLK_30) || \
                                         ((param) == ADC_CONVERSION_CLK_40) || \
                                         ((param) == ADC_CONVERSION_CLK_80) || \
                                         ((param) == ADC_CONVERSION_CLK_160)|| \
                                         ((param) == ADC_CONVERSION_CLK_320))

#define ADC_FC_DIVIDE_LEVEL_1           ((uint32_t)0x00000000)
#define ADC_FC_DIVIDE_LEVEL_2           ((uint32_t)0x00000001)
#define ADC_FC_DIVIDE_LEVEL_4           ((uint32_t)0x00000002)
#define ADC_FC_DIVIDE_LEVEL_8           ((uint32_t)0x00000003)
#define ADC_FC_DIVIDE_LEVEL_16          ((uint32_t)0x00000004)
#define IS_ADC_PRESCALER(param)         ((param) <= ADC_FC_DIVIDE_LEVEL_16)

/* Interrupt generation timing in fixed channel mode */
#define ADC_INT_SINGLE                  ((uint32_t)0x00000000)
#define ADC_INT_CONVERSION_2            ((uint32_t)0x00000010)
#define ADC_INT_CONVERSION_3            ((uint32_t)0x00000020)
#define ADC_INT_CONVERSION_4            ((uint32_t)0x00000030)
#define ADC_INT_CONVERSION_5            ((uint32_t)0x00000040)
#define ADC_INT_CONVERSION_6            ((uint32_t)0x00000050)
#define ADC_INT_CONVERSION_7            ((uint32_t)0x00000060)
#define ADC_INT_CONVERSION_8            ((uint32_t)0x00000070)
#define IS_ADC_INT_MODE(param)          (((param) == ADC_INT_SINGLE)       || \
                                         ((param) == ADC_INT_CONVERSION_2) || \
                                         ((param) == ADC_INT_CONVERSION_3) || \
                                         ((param) == ADC_INT_CONVERSION_4) || \
                                         ((param) == ADC_INT_CONVERSION_5) || \
                                         ((param) == ADC_INT_CONVERSION_6) || \
                                         ((param) == ADC_INT_CONVERSION_7) || \
                                         ((param) == ADC_INT_CONVERSION_8))

    typedef enum {
        ADC_AN_00 = 0U,         /*!< define for Analog Input channel */
        ADC_AN_01 = 1U,
        ADC_AN_02 = 2U,
        ADC_AN_03 = 3U,
        ADC_AN_04 = 4U,
        ADC_AN_05 = 5U,
        ADC_AN_06 = 6U,
        ADC_AN_07 = 7U,
    } ADC_AINx;
#define IS_ADC_INPUT_CHANNEL(param)           ((param) <= ADC_AN_07)
#define IS_ADC_SCAN_CHANNEL(start, range)     (((start) <= ADC_AN_07) && \
                                               ((range) >= 1U) && \
                                               (((start) + (range)) <= 8U))


    typedef enum {
        ADC_REG_00 = 0U,
        ADC_REG_01 = 1U,
        ADC_REG_02 = 2U,
        ADC_REG_03 = 3U,
        ADC_REG_04 = 4U,
        ADC_REG_05 = 5U,
        ADC_REG_06 = 6U,
        ADC_REG_07 = 7U,
        ADC_REG_SP = 8U
    } ADC_REGx;
#define IS_ADC_REG(param)               ((param) <= ADC_REG_SP)

#define ADC_APPLY_VREF_IN_CONVERSION    ((uint32_t)0x00000000)
#define ADC_APPLY_VREF_AT_ANY_TIME      ((uint32_t)0x00000001)
#define IS_ADC_VREF_CTRL(param)         (((param) == ADC_APPLY_VREF_IN_CONVERSION) || \
                                         ((param) == ADC_APPLY_VREF_AT_ANY_TIME))

    typedef enum {
        ADC_CMPCR_0 = 0U,
        ADC_CMPCR_1 = 1U
    } ADC_CMPCRx;
#define IS_ADC_CMPCRx(param)            ((param) <= ADC_CMPCR_1)

#define ADC_EXTERADTRG                  ((uint32_t)0x00000000)
#define ADC_INTERTRIGGER                ((uint32_t)0x00000001)
#define IS_ADC_EXTERADTRG(param)        (((param) == ADC_EXTERADTRG)|| \
                                         ((param) == ADC_INTERTRIGGER))

#define IS_ADC_EXTERADTRG_TOP(param)    (((param) == ADC_EXTERADTRG)|| \
                                         ((param) == ADC_INTERTRIGGER))


    typedef enum {
        ADC_LARGER_THAN_CMP_REG = 0U,
        ADC_SMALLER_THAN_CMP_REG = 1U
    } ADC_CmpCondition;
#define IS_ADC_CMPCONDITION(param)      ((param) <= ADC_SMALLER_THAN_CMP_REG)

    typedef enum {
        ADC_SEQUENCE_CMP_MODE = 0U,
        ADC_CUMULATION_CMP_MODE = 1U
    } ADC_CmpCntMode;
#define IS_ADC_CMPMODE(param)           ((param) <= ADC_CUMULATION_CMP_MODE)

    typedef enum {
        ADC_TRG_00 = 0U,
        ADC_TRG_01 = 1U,
        ADC_TRG_02 = 2U,
        ADC_TRG_03 = 3U,
        ADC_TRG_04 = 4U,
        ADC_TRG_05 = 5U,
        ADC_TRG_06 = 6U,
        ADC_TRG_07 = 7U,
        ADC_TRG_08 = 8U,
        ADC_TRG_09 = 9U
    } ADC_TRGx;
#define IS_ADC_TRG(param)              ((param) <= ADC_TRG_09)

/**
  * @brief  ADC Monitor Configuration Structure definition
  */
    typedef struct {
        ADC_AINx CmpChannel;            /*!< Select which ADC channel will be used */
        uint32_t CmpCnt;                /*!< How many times will valid comparisons be counted, range from 1 to 16 */
        ADC_CmpCondition Condition;     /*!< Condition to compare ADC channel with Compare Register */
        ADC_CmpCntMode CntMode;         /*!< Mode to compare ADC channel with Compare Register */
        uint32_t CmpValue;              /*!< Comparison value to be set in Compare Register, max value is 4095 */
    } ADC_MonitorTypeDef;

#define IS_ADC_CMPCNT(param)            (((param) >= 1U) && ((param) <= 16U))
#define IS_ADC_CMPVALUE_12BIT(param)    ((param) <= 4095U)

/**
  * @brief Union to store ADC state
  */
    typedef union {
        uint32_t All;
        struct {
            uint32_t NormalBusy:1;      /*!< bit0, Normal A/D conversion busy flag (ADBF) */
            uint32_t NormalComplete:1;  /*!< bit1, Normal AD conversion complete flag (EOCF) */
            uint32_t TopBusy:1;         /*!< bit2, Top-priority A/D conversion busy flag (HPADBF) */
            uint32_t TopComplete:1;     /*!< bit3, Top-priority AD conversion complete flag (HPEOCF) */
            uint32_t Reserved:28;       /*!< bit4 to bit 31, reserved */
        } Bit;
    } ADC_State;

/**
  * @brief Union to store ADC result
  */
    typedef union {
        uint32_t All;
        struct {
            uint32_t ADResult:12;       /*!< bit0 to bit11, store AD result */
            uint32_t Stored:1;          /*!< bit12, AD result has been stored */
            uint32_t OverRun:1;         /*!< bit13, new AD result is stored before the old one is read */
            uint32_t Reserved:18;       /*!< bit14 to bit31, reserved */
        } Bit;
    } ADC_Result;


/** @} */
/* End of group ADC_Exported_Types */

/** @defgroup ADC_Exported_FunctionPrototypes
  * @{
  */

    void ADC_SWReset(TSB_AD_TypeDef * ADx);
    void ADC_SetClk(TSB_AD_TypeDef * ADx, uint32_t Sample_HoldTime, uint32_t Prescaler_Output);
    void ADC_Start(TSB_AD_TypeDef * ADx);
    void ADC_SetScanMode(TSB_AD_TypeDef * ADx, FunctionalState NewState);
    void ADC_SetRepeatMode(TSB_AD_TypeDef * ADx, FunctionalState NewState);
    void ADC_SetINTMode(TSB_AD_TypeDef * ADx, uint32_t INTMode);
    void ADC_SetInputChannel(TSB_AD_TypeDef * ADx, ADC_AINx InputChannel);
    void ADC_SetScanChannel(TSB_AD_TypeDef * ADx, ADC_AINx StartChannel, uint32_t Range);
    void ADC_SetVrefCut(TSB_AD_TypeDef * ADx, uint32_t VrefCtrl);
    void ADC_SetIdleMode(TSB_AD_TypeDef * ADx, FunctionalState NewState);
    void ADC_SetVref(TSB_AD_TypeDef * ADx, FunctionalState NewState);
    void ADC_SetInputChannelTop(TSB_AD_TypeDef * ADx, ADC_AINx TopInputChannel);
    void ADC_StartTopConvert(TSB_AD_TypeDef * ADx);
    void ADC_SetMonitor(TSB_AD_TypeDef * ADx, ADC_CMPCRx ADCMPx, FunctionalState NewState);
    void ADC_ConfigMonitor(TSB_AD_TypeDef * ADx, ADC_CMPCRx ADCMPx, ADC_MonitorTypeDef * Monitor);
    void ADC_SetHWTrg(TSB_AD_TypeDef * ADx, uint32_t HWSrc, FunctionalState NewState);
    void ADC_SetHWTrgTop(TSB_AD_TypeDef * ADx, uint32_t HWSrc, FunctionalState NewState);
    ADC_State ADC_GetConvertState(TSB_AD_TypeDef * ADx);
    ADC_Result ADC_GetConvertResult(TSB_AD_TypeDef * ADx, ADC_REGx ADREGx);
    void ADC_EnableTrigger(void);
    void ADC_DisableTrigger(void);
    void ADC_SetTriggerStartup(ADC_TRGx TriggerStartup);
    void ADC_SetTriggerStartupTop(ADC_TRGx TopTriggerStartup);

/** @} */
/* End of group ADC_Exported_FunctionPrototypes */
/** @} */
/* End of group ADC */
/** @} */
/* End of group TX04_Periph_Driver */
#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM46B_ADC_H */
