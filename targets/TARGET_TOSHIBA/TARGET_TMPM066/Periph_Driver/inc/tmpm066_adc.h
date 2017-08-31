/**
 *******************************************************************************
 * @file    tmpm066_adc.h
 * @brief   This file provides all the functions prototypes for ADC driver.
 * @version V2.0.2.1
 * @date    2015/10/09
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM066_ADC_H
#define __TMPM066_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "TMPM066.h"
#include "tx00_common.h"

#if defined(__TMPM066_ADC_H)

/** @addtogroup TX00_Periph_Driver
  * @{
  */

/** @addtogroup ADC
  * @{
  */

/** @defgroup ADC_Exported_Types
  * @{
  */

#define ADC_CONVERSION_35_CLOCK         ((uint32_t)0x00000000)  /* 35.5 conversion clock */
#define ADC_CONVERSION_42_CLOCK         ((uint32_t)0x00000040)  /* 42 conversion clock */
#define ADC_CONVERSION_68_CLOCK         ((uint32_t)0x00000080)  /* 68 conversion clock */
#define ADC_CONVERSION_81_CLOCK         ((uint32_t)0x000000C0)  /* 81 conversion clock */
#define IS_ADC_CONVERSION_TIME(param)   (((param) == ADC_CONVERSION_35_CLOCK) || \
                                         ((param) == ADC_CONVERSION_42_CLOCK) || \
                                         ((param) == ADC_CONVERSION_68_CLOCK) || \
                                         ((param) == ADC_CONVERSION_81_CLOCK))

#define ADC_FC_DIVIDE_LEVEL_1           ((uint32_t)0x00000000)
#define ADC_FC_DIVIDE_LEVEL_2           ((uint32_t)0x00000001)
#define ADC_FC_DIVIDE_LEVEL_4           ((uint32_t)0x00000002)
#define ADC_FC_DIVIDE_LEVEL_6           ((uint32_t)0x00000008)
#define ADC_FC_DIVIDE_LEVEL_8           ((uint32_t)0x00000003)
#define ADC_FC_DIVIDE_LEVEL_12          ((uint32_t)0x00000009)
#define ADC_FC_DIVIDE_LEVEL_16          ((uint32_t)0x00000004)
#define ADC_FC_DIVIDE_LEVEL_24          ((uint32_t)0x0000000A)
#define ADC_FC_DIVIDE_LEVEL_48          ((uint32_t)0x0000000B)
#define ADC_FC_DIVIDE_LEVEL_96          ((uint32_t)0x0000000C)
#define IS_ADC_PRESCALER(param)         (((param) == ADC_FC_DIVIDE_LEVEL_1) || \
                                         ((param) == ADC_FC_DIVIDE_LEVEL_2) || \
                                         ((param) == ADC_FC_DIVIDE_LEVEL_4) || \
                                         ((param) == ADC_FC_DIVIDE_LEVEL_6) || \
                                         ((param) == ADC_FC_DIVIDE_LEVEL_8) || \
                                         ((param) == ADC_FC_DIVIDE_LEVEL_12) || \
                                         ((param) == ADC_FC_DIVIDE_LEVEL_16) || \
                                         ((param) == ADC_FC_DIVIDE_LEVEL_24) || \
                                         ((param) == ADC_FC_DIVIDE_LEVEL_48) || \
                                         ((param) == ADC_FC_DIVIDE_LEVEL_96))

#define ADC_INT_SINGLE                  ((uint32_t)0x00000000)
#define ADC_INT_CONVERSION_4            ((uint32_t)0x00000008)
#define ADC_INT_CONVERSION_8            ((uint32_t)0x00000010)
#define IS_ADC_INT_MODE(param)          (((param) == ADC_INT_SINGLE) || \
                                         ((param) == ADC_INT_CONVERSION_4) || \
                                         ((param) == ADC_INT_CONVERSION_8))

#define ADC_AN_0                        ((uint32_t)0x00000000)
#define ADC_AN_1                        ((uint32_t)0x00000001)
#define ADC_AN_2                        ((uint32_t)0x00000002)
#define ADC_AN_3                        ((uint32_t)0x00000003)
#define ADC_AN_4                        ((uint32_t)0x00000004)
#define ADC_AN_5                        ((uint32_t)0x00000005)
#define ADC_AN_6                        ((uint32_t)0x00000006)
#define ADC_AN_7                        ((uint32_t)0x00000007)

#define IS_ADC_INPUT_CH(param)          (((param) == ADC_AN_0) || \
                                         ((param) == ADC_AN_1) || \
                                         ((param) == ADC_AN_2) || \
                                         ((param) == ADC_AN_3) || \
                                         ((param) == ADC_AN_4) || \
                                         ((param) == ADC_AN_5) || \
                                         ((param) == ADC_AN_6) || \
                                         ((param) == ADC_AN_7))


#define ADC_CMP_0                       ((uint8_t)0x00)
#define ADC_CMP_1                       ((uint8_t)0x01)
#define IS_ADC_CMP(param)               (((param) == ADC_CMP_0) || \
                                         ((param) == ADC_CMP_1))

#define ADC_REG_0                       ((uint32_t)0x00000000)
#define ADC_REG_1                       ((uint32_t)0x00000001)
#define ADC_REG_2                       ((uint32_t)0x00000002)
#define ADC_REG_3                       ((uint32_t)0x00000003)
#define ADC_REG_4                       ((uint32_t)0x00000004)
#define ADC_REG_5                       ((uint32_t)0x00000005)
#define ADC_REG_6                       ((uint32_t)0x00000006)
#define ADC_REG_7                       ((uint32_t)0x00000007)
#define ADC_REG_SP                      ((uint32_t)0x00000008)
#define IS_ADC_REG(param)               ((param) <= ADC_REG_SP)

#define IS_ADC_RESULT_CMP_REG(param)    ((param) <= ADC_REG_SP)

#define ADC_EXT_TRG                     ((uint32_t)0x00000000)
#define ADC_MATCH_TB_0                  ((uint32_t)0x00000020)
#define IS_ADC_HW_TRG_NORMAL(param)     (((param) == ADC_EXT_TRG) || \
                                         ((param) == ADC_MATCH_TB_0))

#define ADC_MATCH_TB_1                  ((uint32_t)0x00000080)
#define IS_ADC_HW_TRG_TOP(param)        (((param) == ADC_EXT_TRG) || \
                                         ((param) == ADC_MATCH_TB_1))

#define IS_ADC_CMP_VALUE(param)         ((param) <= (uint16_t)0x03ff)

    typedef enum {
        ADC_NO_OVERRUN = 0U,
        ADC_OVERRUN = 1U
    } ADC_OverrunState;

    typedef enum {
        ADC_SCAN_4CH = 0U,
        ADC_SCAN_8CH = 1U
    } ADC_ChannelScanMode;
#define IS_ADC_CH_SCAN_MODE(param)      (((param) == ADC_SCAN_4CH) || \
                                         ((param) == ADC_SCAN_8CH))

    typedef enum {
        ADC_COMPARISON_SMALLER = 0U,
        ADC_COMPARISON_LARGER = 1U
    } ADC_ComparisonState;
#define IS_ADC_CMP_INT(param)           (((param) == ADC_COMPARISON_SMALLER) || \
                                         ((param) == ADC_COMPARISON_LARGER))

    typedef struct {
        WorkState ADCResultStored;      /*!< ADC result storage flag    */
        ADC_OverrunState ADCOverrunState;       /*!< ADC overrun flag           */
        uint16_t ADCResultValue;        /*!< ADC result value           */
    } ADC_ResultTypeDef;

#define ADC_DMA_REQ_NORMAL              ((uint8_t)0x00)
#define ADC_DMA_REQ_TOP                 ((uint8_t)0x01)
#define ADC_DMA_REQ_MONITOR1            ((uint8_t)0x02)
#define ADC_DMA_REQ_MONITOR2            ((uint8_t)0x03)
#define IS_ADC_DMA_REQ(param)           ((param) <= ADC_DMA_REQ_MONITOR2)

    typedef union {
        uint32_t All;
        struct {
            uint32_t NormalBusy:1;      /*!< bit0, Normal A/D conversion busy flag (MOD0<ADBFN>) */
            uint32_t NormalComplete:1;  /*!< bit1, Normal AD conversion complete flag (MOD0<EOCFN>) */
            uint32_t TopBusy:1; /*!< bit2, Top-priority A/D conversion busy flag (MOD2<ADBFHP>) */
            uint32_t TopComplete:1;     /*!< bit3, Top-priority AD conversion complete flag (MOD2<EOCFHP>) */
            uint32_t Reserved:28;       /*!< bit4 to bit 31, reserved */
        } Bit;
    } ADC_State;

/** @} */
/* End of group ADC_Exported_Types */

/** @defgroup ADC_Exported_FunctionPrototypes
  * @{
  */
    void ADC_SWReset(void);
    void ADC_SetClk(uint32_t Conversion_Time, uint32_t Prescaler_Output);
    void ADC_Start(void);
    void ADC_SetScanMode(FunctionalState NewState);
    void ADC_SetRepeatMode(FunctionalState NewState);
    void ADC_SetINTMode(uint32_t INTMode);
    ADC_State ADC_GetConvertState(void);
    void ADC_SetInputChannel(uint32_t InputChannel);
    void ADC_SetChannelScanMode(ADC_ChannelScanMode ScanMode);
    void ADC_SetIdleMode(FunctionalState NewState);
    void ADC_SetVref(FunctionalState NewState);
    void ADC_SetInputChannelTop(uint32_t TopInputChannel);
    void ADC_StartTopConvert(void);
    void ADC_SetMonitor(uint8_t ADCMPx, FunctionalState NewState);
    void ADC_SetResultCmpReg(uint8_t ADCMPx, uint32_t ResultComparison);
    void ADC_SetMonitorINT(uint8_t ADCMPx, ADC_ComparisonState NewState);
    void ADC_SetHWTrg(uint32_t HwSource, FunctionalState NewState);
    void ADC_SetHWTrgTop(uint32_t HwSource, FunctionalState NewState);
    ADC_ResultTypeDef ADC_GetConvertResult(uint32_t ADREGx);
    void ADC_SetCmpValue(uint8_t ADCMPx, uint16_t value);
    void ADC_SetDMAReq(uint8_t DMAReq, FunctionalState NewState);

/** @} */
/* End of group ADC_Exported_FunctionPrototypes */

/** @} */
/* End of group ADC */

/** @} */
/* End of group TX00_Periph_Driver */

#endif                          /* (__TMPM066_ADC_H) */

#ifdef __cplusplus
}
#endif
#endif                          /* __TMPM066_ADC_H */
