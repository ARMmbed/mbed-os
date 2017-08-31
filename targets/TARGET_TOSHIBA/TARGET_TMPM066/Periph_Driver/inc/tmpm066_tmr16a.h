/**
 *******************************************************************************
 * @file    tmpm066_tmr16a.h
 * @brief   This file provides all the functions prototypes for TMR16A driver.
 * @version V2.0.2.1
 * @date    2015/10/09
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM066_TMR16A_H
#define __TMPM066_TMR16A_H

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "TMPM066.h"
#include "tx00_common.h"

#if defined(__TMPM066_TMR16A_H)
/** @addtogroup TX00_Periph_Driver
  * @{
  */

/** @addtogroup TMR16A
  * @{
  */

/** @defgroup TMR16A_Exported_Types
  * @{
  */

/**
  * @brief TMR16A Flip-flop Structure definition
  */

    typedef struct {
        uint32_t TMR16AFlipflopCtrl;    /*!< Select TMR16A flip-flop output level */
        uint32_t TMR16AFlipflopReverseTrg;      /*!< Specify TMR16A flip-flop reverse trigger */
    } TMR16A_FFOutputTypeDef;

/** @} */
/* End of group TMR16A_Exported_Types */

/** @defgroup TMR16A_Exported_Constants
  * @{
  */
#define IS_TMR16A_ALL_PERIPH(param)    (((param) == TSB_T16A0) || \
                                        ((param) == TSB_T16A1))



#define TMR16A_SYSCK                    ((uint32_t)0x00000000)
#define TMR16A_PRCK                     ((uint32_t)0x00000001)
#define IS_TMR16A_SRCCLK(param)         (((param) == TMR16A_SYSCK) || ((param) == TMR16A_PRCK))

#define TMR16A_RUN                      ((uint32_t)0x00000001)
#define TMR16A_STOP                     ((uint32_t)0x00000000)
#define IS_TMR16A_CMD(param)            (((param) == TMR16A_RUN) || ((param) == TMR16A_STOP))

#define TMR16A_RUNNING_IN_CORE_HALT        ((uint8_t)0x00)
#define TMR16A_STOP_IN_CORE_HALT           ((uint8_t)0x02)
#define IS_TMR16A_CLK_IN_CORE_HALT(param)  (((param) == TMR16A_RUNNING_IN_CORE_HALT) || \
                                           ((param) == TMR16A_STOP_IN_CORE_HALT))

#define TMR16A_FLIPFLOP_INVERT         ((uint32_t)0x00000000)
#define TMR16A_FLIPFLOP_SET            ((uint32_t)0x00000001)
#define TMR16A_FLIPFLOP_CLEAR          ((uint32_t)0x00000002)
#define IS_TMR16A_FLIPFLOP_CTRL(param) (((param) == TMR16A_FLIPFLOP_INVERT) || \
                                        ((param) == TMR16A_FLIPFLOP_SET) || \
                                        ((param) == TMR16A_FLIPFLOP_CLEAR))

#define TMR16A_DISABLE_FLIPFLOP        ((uint32_t)0x00000000)
#define TMR16A_FLIPFLOP_MATCH_CYCLE    ((uint32_t)0x00000080)
#define IS_TMR16A_FLIPFLOP_TRG(param)  (((param) == TMR16A_DISABLE_FLIPFLOP) || \
                                        ((param) == TMR16A_FLIPFLOP_MATCH_CYCLE))

#define IS_TMR16A_VALUE(param)          ((param) <= 0x0000FFFFU)
#define IS_TMR16A_VALID_DUTY(param1, param2) ((param1) <= (param2))

/* End of group TMR16A_Exported_Constants */
/** @defgroup TMR16A_Exported_FunctionPrototypes
  * @{
  */
    void TMR16A_SetClkInCoreHalt(TSB_T16A_TypeDef * T16Ax, uint8_t ClkState);
    void TMR16A_SetRunState(TSB_T16A_TypeDef * T16Ax, uint32_t Cmd);
    void TMR16A_SetSrcClk(TSB_T16A_TypeDef * T16Ax, uint32_t SrcClk);
    void TMR16A_SetFlipFlop(TSB_T16A_TypeDef * T16Ax, TMR16A_FFOutputTypeDef * FFStruct);
    void TMR16A_ChangeCycle(TSB_T16A_TypeDef * T16Ax, uint32_t Cycle);
    uint16_t TMR16A_GetCaptureValue(TSB_T16A_TypeDef * T16Ax);
/** @} */
/* End of group TMR16A_Exported_FunctionPrototypes */

/** @} */
/* End of group TMR16A */

/** @} */
/* End of group TX00_Periph_Driver */
#endif                          /* defined(__TMPM066_TMR16A_H) */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM066_TMR16A_H */
