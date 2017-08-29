/**
 *******************************************************************************
 * @file    tmpm066_tmrb.h
 * @brief   This file provides all the functions prototypes for TMRB driver.
 * @version V2.0.2.1
 * @date    2015/10/09
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM066_TMRB_H
#define __TMPM066_TMRB_H

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "TMPM066.h"
#include "tx00_common.h"

#if defined(__TMPM066_TMRB_H)
/** @addtogroup TX00_Periph_Driver
  * @{
  */
/** @addtogroup TMRB
  * @{
  */
/** @defgroup TMRB_Exported_Types
  * @{
  */
/**
  * @brief TMRB Init Structure definition
  */
    typedef struct {
        uint32_t Mode;          /*!< Select TMRB mode between internal interval
                                   timer mode and external event counter */
        uint32_t ClkDiv;        /*!< Select the division for TMRB source clock */
        uint32_t TrailingTiming;        /*!< Specify the trailingtiming value to be written 
                                           into TBnRG1 */
        uint32_t UpCntCtrl;     /*!< Select up-counter work mode between 
                                   freerun and auto-reload */
        uint32_t LeadingTiming; /*!< Specify the leadingtiming value to be written 
                                   into TBnRG0 */
    } TMRB_InitTypeDef;

/**
  * @brief TMRB Flip-flop Structure definition
  */

    typedef struct {
        uint32_t FlipflopCtrl;  /*!< Select TMRB flip-flop output level */
        uint32_t FlipflopReverseTrg;    /*!< Specify TMRB flip-flop reverse trigger */
    } TMRB_FFOutputTypeDef;

/**
  * @brief TMRB Interrupt factor Union definition
  */
    typedef union {
        uint32_t All;
        struct {
            uint32_t MatchLeadingTiming:1;
            uint32_t MatchTrailingTiming:1;
            uint32_t OverFlow:1;
            uint32_t Reserverd:29;
        } Bit;
    } TMRB_INTFactor;

/**                                              
  * @brief TMRB Interrupt masked Union definition
  */
    typedef union {
        uint32_t All;
        struct {
            uint32_t MatchLeadingTimingMask:1;
            uint32_t MatchTrailingTimingMask:1;
            uint32_t OverFlowMask:1;
            uint32_t Reserverd:29;
        } Bit;
    } TMRB_INTMask;

/** @} */
/* End of group TMRB_Exported_Types */

/** @defgroup TMRB_Exported_Constants
  * @{
  */
#define IS_TMRB_ALL_PERIPH(param)    (((param) == TSB_TB0) || \
                                      ((param) == TSB_TB1) || \
                                      ((param) == TSB_TB2) || \
                                      ((param) == TSB_TB3) || \
                                      ((param) == TSB_TB4) || \
                                      ((param) == TSB_TB5) || \
                                      ((param) == TSB_TB6) || \
                                      ((param) == TSB_TB7))

#define IS_TMRB_SYNC_PERIPH(param)   (((param) == TSB_TB1) || \
                                      ((param) == TSB_TB2) || \
                                      ((param) == TSB_TB3) || \
                                      ((param) == TSB_TB5) || \
                                      ((param) == TSB_TB6) || \
                                      ((param) == TSB_TB7))

#define IS_TMRB_CAP_PERIPH(param)    (((param) == TSB_TB0) || \
                                      ((param) == TSB_TB1) || \
                                      ((param) == TSB_TB2) || \
                                      ((param) == TSB_TB3) || \
                                      ((param) == TSB_TB4) || \
                                      ((param) == TSB_TB5))

#define TMRB_INTERVAL_TIMER          ((uint32_t)0x00000001)
#define TMRB_EVENT_CNT               ((uint32_t)0x00000000)
#define IS_TMRB_MODE(param)          (((param) == TMRB_INTERVAL_TIMER) || \
                                      ((param) == TMRB_EVENT_CNT))

#define TMRB_CLK_DIV_2               ((uint32_t)0x00000001)
#define TMRB_CLK_DIV_8               ((uint32_t)0x00000002)
#define TMRB_CLK_DIV_32              ((uint32_t)0x00000003)
#define TMRB_CLK_DIV_64              ((uint32_t)0x00000004)
#define TMRB_CLK_DIV_128             ((uint32_t)0x00000005)
#define TMRB_CLK_DIV_256             ((uint32_t)0x00000006)
#define TMRB_CLK_DIV_512             ((uint32_t)0x00000007)
#define IS_TMRB_CLK_DIV(param)       (((param) == TMRB_CLK_DIV_2) || \
                                      ((param) == TMRB_CLK_DIV_8) || \
                                      ((param) == TMRB_CLK_DIV_32) || \
                                      ((param) == TMRB_CLK_DIV_64) || \
                                      ((param) == TMRB_CLK_DIV_128) || \
                                      ((param) == TMRB_CLK_DIV_256) || \
                                      ((param) == TMRB_CLK_DIV_512))

#define TMRB_FREE_RUN                ((uint32_t)0x00000000)
#define TMRB_AUTO_CLEAR              ((uint32_t)0x00000008)
#define IS_TMRB_UC_CTRL(param)       (((param) == TMRB_FREE_RUN) || \
                                      ((param) == TMRB_AUTO_CLEAR))

#define TMRB_FLIPFLOP_INVERT         ((uint32_t)0x00000000)
#define TMRB_FLIPFLOP_SET            ((uint32_t)0x00000001)
#define TMRB_FLIPFLOP_CLEAR          ((uint32_t)0x00000002)
#define IS_TMRB_FLIPFLOP_CTRL(param) (((param) == TMRB_FLIPFLOP_INVERT) || \
                                      ((param) == TMRB_FLIPFLOP_SET) || \
                                      ((param) == TMRB_FLIPFLOP_CLEAR))

#define TMRB_DISABLE_FLIPFLOP        ((uint32_t)0x00000000)
#define TMRB_FLIPFLOP_TAKE_CAPTURE_0 ((uint32_t)0x00000010)
#define TMRB_FLIPFLOP_TAKE_CAPTURE_1 ((uint32_t)0x00000020)
#define TMRB_FLIPFLOP_MATCH_TRAILINGTIMING    ((uint32_t)0x00000008)
#define TMRB_FLIPFLOP_MATCH_LEADINGTIMING     ((uint32_t)0x00000004)
#define IS_TMRB_FLIPFLOP_TRG(param)  (((param) == TMRB_DISABLE_FLIPFLOP) || \
                                      ((param) == TMRB_FLIPFLOP_TAKE_CAPTURE_0) || \
                                      ((param) == TMRB_FLIPFLOP_TAKE_CAPTURE_1) || \
                                      ((param) == TMRB_FLIPFLOP_MATCH_TRAILINGTIMING) || \
                                      ((param) == TMRB_FLIPFLOP_MATCH_LEADINGTIMING) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_0 | TMRB_FLIPFLOP_TAKE_CAPTURE_1)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_0 | TMRB_FLIPFLOP_MATCH_TRAILINGTIMING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_0 | TMRB_FLIPFLOP_MATCH_LEADINGTIMING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_1 | TMRB_FLIPFLOP_MATCH_TRAILINGTIMING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_1 | TMRB_FLIPFLOP_MATCH_LEADINGTIMING)) || \
                                      ((param) == (TMRB_FLIPFLOP_MATCH_TRAILINGTIMING | TMRB_FLIPFLOP_MATCH_LEADINGTIMING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_0 | TMRB_FLIPFLOP_TAKE_CAPTURE_1 | TMRB_FLIPFLOP_MATCH_TRAILINGTIMING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_0 | TMRB_FLIPFLOP_MATCH_TRAILINGTIMING | TMRB_FLIPFLOP_MATCH_LEADINGTIMING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_1 | TMRB_FLIPFLOP_MATCH_TRAILINGTIMING | TMRB_FLIPFLOP_MATCH_LEADINGTIMING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_0 | TMRB_FLIPFLOP_TAKE_CAPTURE_1 | TMRB_FLIPFLOP_MATCH_LEADINGTIMING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_0 | TMRB_FLIPFLOP_TAKE_CAPTURE_1 | TMRB_FLIPFLOP_MATCH_TRAILINGTIMING | TMRB_FLIPFLOP_MATCH_LEADINGTIMING)))

#define TMRB_DISABLE_CAPTURE                ((uint32_t)0x00000000)
#define TMRB_CAPTURE_IN_RISING_FALLING      ((uint32_t)0x00000020)
#define TMRB_CAPTURE_FF_RISING_FALLING      ((uint32_t)0x00000030)
#define IS_TMRB_CAPTURE_TIMING(param)       (((param) == TMRB_DISABLE_CAPTURE) || \
                                             ((param) == TMRB_CAPTURE_IN_RISING_FALLING) || \
                                             ((param) == TMRB_CAPTURE_FF_RISING_FALLING))

#define TMRB_RUN                      ((uint32_t)0x00000005)
#define TMRB_STOP                     ((uint32_t)0x00000000)
#define IS_TMRB_CMD(param)            (((param) == TMRB_RUN) || ((param) == TMRB_STOP))

#define TMRB_REG_0                ((uint8_t)0x00)
#define TMRB_REG_1                ((uint8_t)0x01)
#define IS_TMRB_REG(param)    (((param) == TMRB_REG_0) || ((param) == TMRB_REG_1))

#define TMRB_CAPTURE_0                ((uint8_t)0x00)
#define TMRB_CAPTURE_1                ((uint8_t)0x01)
#define IS_TMRB_CAPTURE_REG(param)    (((param) == TMRB_CAPTURE_0) || ((param) == TMRB_CAPTURE_1))

#define TMRB_NO_INT_MASK              ((uint32_t)0x00000000)
#define TMRB_MASK_MATCH_LEADINGTIMING_INT      ((uint32_t)0x00000001)
#define TMRB_MASK_MATCH_TRAILINGTIMING_INT     ((uint32_t)0x00000002)
#define TMRB_MASK_OVERFLOW_INT        ((uint32_t)0x00000004)
#define IS_TMRB_INT_MASK(param)       (((param) == TMRB_NO_INT_MASK) || \
                                       ((param) == TMRB_MASK_MATCH_LEADINGTIMING_INT) || \
                                       ((param) == TMRB_MASK_MATCH_TRAILINGTIMING_INT) || \
                                       ((param) == TMRB_MASK_OVERFLOW_INT) || \
                                       ((param) == (TMRB_MASK_MATCH_LEADINGTIMING_INT | TMRB_MASK_MATCH_TRAILINGTIMING_INT)) || \
                                       ((param) == (TMRB_MASK_MATCH_LEADINGTIMING_INT | TMRB_MASK_OVERFLOW_INT)) || \
                                       ((param) == (TMRB_MASK_MATCH_TRAILINGTIMING_INT | TMRB_MASK_OVERFLOW_INT)) || \
                                       ((param) == (TMRB_MASK_MATCH_LEADINGTIMING_INT | TMRB_MASK_MATCH_TRAILINGTIMING_INT | TMRB_MASK_OVERFLOW_INT)))

#define TMRB_TRG_EDGE_RISING          ((uint8_t)0x00)
#define TMRB_TRG_EDGE_FALLING         ((uint8_t)0x02)
#define IS_TMRB_TRG_EDGE(param)       (((param) == TMRB_TRG_EDGE_RISING) || \
                                       ((param) == TMRB_TRG_EDGE_FALLING))

#define TMRB_RUNNING_IN_CORE_HALT        ((uint8_t)0x00)
#define TMRB_STOP_IN_CORE_HALT           ((uint8_t)0x40)
#define IS_TMRB_CLK_IN_CORE_HALT(param)  (((param) == TMRB_RUNNING_IN_CORE_HALT) || \
                                          ((param) == TMRB_STOP_IN_CORE_HALT))

#define TMRB_NO_INT                   ((uint32_t)0x00000000)
#define IS_TMRB_VALUE(param)          ((param) <= 0x0000FFFFU)
#define IS_VALID_LEADINGTIMING(param1, param2) ((param1) <= (param2))

#define TMRB_DMA_REQ_CMP_MATCH                   ((uint32_t)0x000000004)
#define TMRB_DMA_REQ_CAPTURE_1                   ((uint32_t)0x000000002)
#define TMRB_DMA_REQ_CAPTURE_0                   ((uint32_t)0x000000001)
#define IS_TMRB_DMA_REQ(param)                   (((param) == TMRB_DMA_REQ_CMP_MATCH) || \
                                                  ((param) == TMRB_DMA_REQ_CAPTURE_1) || \
                                                  ((param) == TMRB_DMA_REQ_CAPTURE_0))

/** @} */
/* End of group TMRB_Exported_Constants */
/** @defgroup TMRB_Exported_FunctionPrototypes
  * @{
  */
    void TMRB_Enable(TSB_TB_TypeDef * TBx);
    void TMRB_Disable(TSB_TB_TypeDef * TBx);
    void TMRB_SetRunState(TSB_TB_TypeDef * TBx, uint32_t Cmd);
    void TMRB_Init(TSB_TB_TypeDef * TBx, TMRB_InitTypeDef * InitStruct);
    void TMRB_SetCaptureTiming(TSB_TB_TypeDef * TBx, uint32_t CaptureTiming);
    void TMRB_SetFlipFlop(TSB_TB_TypeDef * TBx, TMRB_FFOutputTypeDef * FFStruct);
    TMRB_INTFactor TMRB_GetINTFactor(TSB_TB_TypeDef * TBx);
    TMRB_INTMask TMRB_GetINTMask(TSB_TB_TypeDef * TBx);
    void TMRB_SetINTMask(TSB_TB_TypeDef * TBx, uint32_t INTMask);
    void TMRB_ChangeLeadingTiming(TSB_TB_TypeDef * TBx, uint32_t LeadingTiming);
    void TMRB_ChangeTrailingTiming(TSB_TB_TypeDef * TBx, uint32_t TrailingTiming);
    uint16_t TMRB_GetRegisterValue(TSB_TB_TypeDef * TBx, uint8_t Reg);
    uint16_t TMRB_GetUpCntValue(TSB_TB_TypeDef * TBx);
    uint16_t TMRB_GetCaptureValue(TSB_TB_TypeDef * TBx, uint8_t CapReg);
    void TMRB_ExecuteSWCapture(TSB_TB_TypeDef * TBx);
    void TMRB_SetSyncMode(TSB_TB_TypeDef * TBx, FunctionalState NewState);
    void TMRB_SetDoubleBuf(TSB_TB_TypeDef * TBx, FunctionalState NewState);
    void TMRB_SetExtStartTrg(TSB_TB_TypeDef * TBx, FunctionalState NewState, uint8_t TrgMode);
    void TMRB_SetClkInCoreHalt(TSB_TB_TypeDef * TBx, uint8_t ClkState);
    void TMRB_SetDMAReq(TSB_TB_TypeDef * TBx, FunctionalState NewState, uint8_t DMAReq);

/** @} */
/* End of group TMRB_Exported_FunctionPrototypes */

/** @} */
/* End of group TMRB */

/** @} */
/* End of group TX00_Periph_Driver */
#endif                          /* defined(__TMPM066_TMRB_H) */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM066_TMRB_H */
