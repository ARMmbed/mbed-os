/**
 *******************************************************************************
 * @file    tmpm46b_tmrb.h
 * @brief   This file provides all the functions prototypes for TMRB driver.
 * @version V2.0.2.1
 * @date    2015/02/27
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM46B_TMRB_H
#define __TMPM46B_TMRB_H

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "TMPM46B.h"
#include "tx04_common.h"

/** @addtogroup TX04_Periph_Driver
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
        uint32_t Mode;                /*!< Select TMRB mode between internal interval
                                           timer mode and external event counter */
        uint32_t ClkDiv;              /*!< Select the division for TMRB source clock */
        uint32_t TrailingTiming;      /*!< Specify the trailingTiming value to be written 
                                           into TBnRG1 */
        uint32_t UpCntCtrl;           /*!< Select up-counter work mode between 
                                           freerun and auto-reload */
        uint32_t LeadingTiming;       /*!< Specify the LeadingTiming value to be written 
                                           into TBnRG0 */
    } TMRB_InitTypeDef;

/**
  * @brief TMRB Flip-flop Structure definition
  */

    typedef struct {
        uint32_t FlipflopCtrl;        /*!< Select TMRB flip-flop output level */
        uint32_t FlipflopReverseTrg;  /*!< Specify TMRB flip-flop reverse trigger */
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

/** @} */
/* End of group TMRB_Exported_Types */

/** @defgroup TMRB_Exported_Constants
  * @{
  */
#define TSB_TB_MPT0                  ((TSB_TB_TypeDef *)TSB_MT0)
#define TSB_TB_MPT1                  ((TSB_TB_TypeDef *)TSB_MT1)
#define TSB_TB_MPT2                  ((TSB_TB_TypeDef *)TSB_MT2)
#define TSB_TB_MPT3                  ((TSB_TB_TypeDef *)TSB_MT3)
#define IS_TMRB_ALL_PERIPH(param)    (((param) == TSB_TB0) || \
                                      ((param) == TSB_TB1) || \
                                      ((param) == TSB_TB2) || \
                                      ((param) == TSB_TB3) || \
                                      ((param) == TSB_TB4) || \
                                      ((param) == TSB_TB5) || \
                                      ((param) == TSB_TB6) || \
                                      ((param) == TSB_TB7) || \
                                      ((param) == TSB_TB_MPT0) || \
                                      ((param) == TSB_TB_MPT1) || \
                                      ((param) == TSB_TB_MPT2) || \
                                      ((param) == TSB_TB_MPT3))

#define IS_TMRB_TMRB_PERIPH(param)   (((param) == TSB_TB0) || \
                                      ((param) == TSB_TB1) || \
                                      ((param) == TSB_TB2) || \
                                      ((param) == TSB_TB3) || \
                                      ((param) == TSB_TB4) || \
                                      ((param) == TSB_TB5) || \
                                      ((param) == TSB_TB6) || \
                                      ((param) == TSB_TB7))

#define IS_TMRB_MPT_PERIPH(param)    (((param) == TSB_TB_MPT0) || \
                                      ((param) == TSB_TB_MPT1) || \
                                      ((param) == TSB_TB_MPT2) || \
                                      ((param) == TSB_TB_MPT3))

#define IS_TMRB_SYNC_PERIPH(param)   (((param) == TSB_TB1) || \
                                      ((param) == TSB_TB2) || \
                                      ((param) == TSB_TB3) || \
                                      ((param) == TSB_TB5) || \
                                      ((param) == TSB_TB6) || \
                                      ((param) == TSB_TB7))

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

#define IS_MPT_CLK_DIV(param)        (((param) == TMRB_CLK_DIV_2) || \
                                      ((param) == TMRB_CLK_DIV_8) || \
                                      ((param) == TMRB_CLK_DIV_32))

#define TMRB_FREE_RUN                ((uint32_t)0x00000000)
#define TMRB_AUTO_CLEAR              ((uint32_t)0x00000008)
#define IS_TMRB_UC_CTRL(param)       (((param) == TMRB_FREE_RUN) || \
                                      ((param) == TMRB_AUTO_CLEAR))

#define MPT_FREE_RUN                 ((uint32_t)0x00000000)
#define MPT_AUTO_CLEAR               ((uint32_t)0x00000004)
#define IS_MPT_UC_CTRL(param)        (((param) == MPT_FREE_RUN) || \
                                      ((param) == MPT_AUTO_CLEAR))

#define TMRB_FLIPFLOP_INVERT         ((uint32_t)0x00000000)
#define TMRB_FLIPFLOP_SET            ((uint32_t)0x00000001)
#define TMRB_FLIPFLOP_CLEAR          ((uint32_t)0x00000002)
#define IS_TMRB_FLIPFLOP_CTRL(param) (((param) == TMRB_FLIPFLOP_INVERT) || \
                                      ((param) == TMRB_FLIPFLOP_SET) || \
                                      ((param) == TMRB_FLIPFLOP_CLEAR))

#define TMRB_DISABLE_FLIPFLOP        ((uint32_t)0x00000000)
#define TMRB_FLIPFLOP_TAKE_CAPTURE_0 ((uint32_t)0x00000010)
#define TMRB_FLIPFLOP_TAKE_CAPTURE_1 ((uint32_t)0x00000020)
#define TMRB_FLIPFLOP_MATCH_TRAILING ((uint32_t)0x00000008)
#define TMRB_FLIPFLOP_MATCH_LEADING  ((uint32_t)0x00000004)
#define IS_TMRB_FLIPFLOP_TRG(param)  (((param) == TMRB_DISABLE_FLIPFLOP) || \
                                      ((param) == TMRB_FLIPFLOP_TAKE_CAPTURE_0) || \
                                      ((param) == TMRB_FLIPFLOP_TAKE_CAPTURE_1) || \
                                      ((param) == TMRB_FLIPFLOP_MATCH_TRAILING) || \
                                      ((param) == TMRB_FLIPFLOP_MATCH_LEADING) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_0 | TMRB_FLIPFLOP_TAKE_CAPTURE_1)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_0 | TMRB_FLIPFLOP_MATCH_TRAILING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_0 | TMRB_FLIPFLOP_MATCH_LEADING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_1 | TMRB_FLIPFLOP_MATCH_TRAILING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_1 | TMRB_FLIPFLOP_MATCH_LEADING)) || \
                                      ((param) == (TMRB_FLIPFLOP_MATCH_TRAILING | TMRB_FLIPFLOP_MATCH_LEADING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_0 | TMRB_FLIPFLOP_TAKE_CAPTURE_1 | TMRB_FLIPFLOP_MATCH_TRAILING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_0 | TMRB_FLIPFLOP_MATCH_TRAILING | TMRB_FLIPFLOP_MATCH_LEADING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_1 | TMRB_FLIPFLOP_MATCH_TRAILING | TMRB_FLIPFLOP_MATCH_LEADING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_0 | TMRB_FLIPFLOP_TAKE_CAPTURE_1 | TMRB_FLIPFLOP_MATCH_LEADING)) || \
                                      ((param) == (TMRB_FLIPFLOP_TAKE_CAPTURE_0 | TMRB_FLIPFLOP_TAKE_CAPTURE_1 | TMRB_FLIPFLOP_MATCH_TRAILING | TMRB_FLIPFLOP_MATCH_LEADING)))

#define TMRB_DISABLE_CAPTURE                          ((uint32_t)0x00000000)
#define TMRB_CAPTURE_TBIN0_TBIN1_RISING               ((uint32_t)0x00000100)
#define TMRB_CAPTURE_TBIN0_RISING_FALLING             ((uint32_t)0x00000200)
#define TMRB_CAPTURE_TBFF0_EDGE                       ((uint32_t)0x00000300)
#define TMRB_CLEAR_TBIN1_RISING                       ((uint32_t)0x00000400)
#define TMRB_CAPTURE_TBIN0_RISING_CLEAR_TBIN1_RISING  ((uint32_t)0x00000500)
#define IS_TMRB_CAPTURE_TIMING_ALL(param)             (((param) == TMRB_DISABLE_CAPTURE) || \
                                                       ((param) == TMRB_CAPTURE_TBIN0_TBIN1_RISING) || \
                                                       ((param) == TMRB_CAPTURE_TBIN0_RISING_FALLING) || \
                                                       ((param) == TMRB_CAPTURE_TBFF0_EDGE) || \
                                                       ((param) == TMRB_CLEAR_TBIN1_RISING) || \
                                                       ((param) == TMRB_CAPTURE_TBIN0_RISING_CLEAR_TBIN1_RISING))

#define IS_TMRB_CAPTURE_TIMING_NONE_TBIN1(param)      (((param) == TMRB_DISABLE_CAPTURE) || \
                                                       ((param) == TMRB_CAPTURE_TBIN0_RISING_FALLING) || \
                                                       ((param) == TMRB_CAPTURE_TBFF0_EDGE))

#define MPT_DISABLE_CAPTURE              ((uint32_t)0x00000000)
#define MPT_CAPTURE_IN_RISING            ((uint32_t)0x00000008)
#define MPT_CAPTURE_IN_RISING_FALLING    ((uint32_t)0x00000010)
#define IS_MPT_CAPTURE_TIMING(param)     (((param) == MPT_DISABLE_CAPTURE) || \
                                          ((param) == MPT_CAPTURE_IN_RISING) || \
                                          ((param) == MPT_CAPTURE_IN_RISING_FALLING))

#define TMRB_RUN                         ((uint32_t)0x00000005)
#define TMRB_STOP                        ((uint32_t)0x00000000)
#define IS_TMRB_CMD(param)               (((param) == TMRB_RUN) || ((param) == TMRB_STOP))

#define TMRB_CAPTURE_0                   ((uint8_t)0x00)
#define TMRB_CAPTURE_1                   ((uint8_t)0x01)
#define IS_TMRB_CAPTURE_REG(param)       (((param) == TMRB_CAPTURE_0) || ((param) == TMRB_CAPTURE_1))

#define TMRB_NO_INT_MASK                 ((uint32_t)0x00000000)
#define TMRB_MASK_MATCH_LEADING_INT      ((uint32_t)0x00000001)
#define TMRB_MASK_MATCH_TRAILING_INT     ((uint32_t)0x00000002)
#define TMRB_MASK_OVERFLOW_INT           ((uint32_t)0x00000004)
#define IS_TMRB_INT_MASK(param)          (((param) == TMRB_NO_INT_MASK) || \
                                          ((param) == TMRB_MASK_MATCH_LEADING_INT) || \
                                          ((param) == TMRB_MASK_MATCH_TRAILING_INT) || \
                                          ((param) == TMRB_MASK_OVERFLOW_INT) || \
                                          ((param) == (TMRB_MASK_MATCH_LEADING_INT | TMRB_MASK_MATCH_TRAILING_INT)) || \
                                          ((param) == (TMRB_MASK_MATCH_LEADING_INT | TMRB_MASK_OVERFLOW_INT)) || \
                                          ((param) == (TMRB_MASK_MATCH_TRAILING_INT | TMRB_MASK_OVERFLOW_INT)) || \
                                          ((param) == (TMRB_MASK_MATCH_LEADING_INT | TMRB_MASK_MATCH_TRAILING_INT | TMRB_MASK_OVERFLOW_INT)))

#define TMRB_TRG_EDGE_RISING             ((uint8_t)0x00)
#define TMRB_TRG_EDGE_FALLING            ((uint8_t)0x02)
#define IS_TMRB_TRG_EDGE(param)          (((param) == TMRB_TRG_EDGE_RISING) || \
                                          ((param) == TMRB_TRG_EDGE_FALLING))

#define TMRB_WRITE_REG_SEPARATE          ((uint8_t)0x00)
#define TMRB_WRITE_REG_SIMULTANEOUS      ((uint8_t)0x40)
#define IS_TMRB_WRITE_REG_MODE(param)    (((param) == TMRB_WRITE_REG_SEPARATE) || \
                                          ((param) == TMRB_WRITE_REG_SIMULTANEOUS))

#define TMRB_RUNNING_IN_CORE_HALT        ((uint8_t)0x00)
#define TMRB_STOP_IN_CORE_HALT           ((uint8_t)0x40)
#define IS_TMRB_CLK_IN_CORE_HALT(param)  (((param) == TMRB_RUNNING_IN_CORE_HALT) || \
                                          ((param) == TMRB_STOP_IN_CORE_HALT))

#define TMRB_NO_INT                      ((uint32_t)0x00000000)
#define IS_TMRB_VALUE(param)             ((param) <= 0x0000FFFFU)
#define IS_VALID_LEADING(param1, param2) ((param1) <= (param2))

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
    void TMRB_SetINTMask(TSB_TB_TypeDef * TBx, uint32_t INTMask);
    void TMRB_ChangeLeadingTiming(TSB_TB_TypeDef * TBx, uint32_t LeadingTiming);
    void TMRB_ChangeTrailingTiming(TSB_TB_TypeDef * TBx, uint32_t TrailingTiming);
    uint16_t TMRB_GetUpCntValue(TSB_TB_TypeDef * TBx);
    uint16_t TMRB_GetCaptureValue(TSB_TB_TypeDef * TBx, uint8_t CapReg);
    void TMRB_ExecuteSWCapture(TSB_TB_TypeDef * TBx);
    void TMRB_SetIdleMode(TSB_TB_TypeDef * TBx, FunctionalState NewState);
    void TMRB_SetSyncMode(TSB_TB_TypeDef * TBx, FunctionalState NewState);
    void TMRB_SetDoubleBuf(TSB_TB_TypeDef * TBx, FunctionalState NewState, uint8_t WriteRegMode);
    void TMRB_SetExtStartTrg(TSB_TB_TypeDef * TBx, FunctionalState NewState, uint8_t TrgMode);
    void TMRB_SetClkInCoreHalt(TSB_TB_TypeDef * TBx, uint8_t ClkState);

/** @} */
/* End of group TMRB_Exported_FunctionPrototypes */

/** @} */
/* End of group TMRB */

/** @} */
/* End of group TX04_Periph_Driver */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM46B_TMRB_H */
