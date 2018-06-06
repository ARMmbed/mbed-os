/**
 *******************************************************************************
 * @file    tmpm46b_ssp.h
 * @brief   This file provides all the functions prototypes for SSP driver.
 * @version V2.0.2.1
 * @date    2015/02/05
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM46B_SSP_H
#define __TMPM46B_SSP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "TMPM46B.h"
#include "tx04_common.h"

/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @addtogroup SSP
  * @{
  */

/** @addtogroup SSP_Exported_types
  * @{
  */

#define IS_SSP_PERIPH(param)     (((param) == TSB_SSP0)  || \
                                  ((param) == TSB_SSP1)  || \
                                  ((param) == TSB_SSP2) )

    typedef enum {
        SSP_FORMAT_SPI = 0U,
        SSP_FORMAT_SSI = 1U,
        SSP_FORMAT_MICROWIRE = 2U
    } SSP_FrameFormat;
#define IS_SSP_FRAME_FORMAT(param)  (((param) == SSP_FORMAT_SPI) || \
                                     ((param) == SSP_FORMAT_SSI) || \
                                     ((param) == SSP_FORMAT_MICROWIRE))

    typedef enum {
        SSP_POLARITY_LOW = 0U,
        SSP_POLARITY_HIGH = 1U
    } SSP_ClkPolarity;
#define IS_SSP_CLK_POLARITY(param)  (((param) == SSP_POLARITY_LOW) || \
                                     ((param) == SSP_POLARITY_HIGH))

    typedef enum {
        SSP_PHASE_FIRST_EDGE = 0U,
        SSP_PHASE_SECOND_EDGE = 1U
    } SSP_ClkPhase;
#define IS_SSP_CLK_PHASE(param)  (((param) == SSP_PHASE_FIRST_EDGE) || \
                                  ((param) == SSP_PHASE_SECOND_EDGE))

    typedef enum {
        SSP_MASTER = 0U,
        SSP_SLAVE = 1U
    } SSP_MS_Mode;
#define IS_SSP_MS_MODE(param)  (((param) == SSP_MASTER) || \
                                ((param) == SSP_SLAVE))

    typedef enum {
        SSP_FIFO_EMPTY = 0U,
        SSP_FIFO_NORMAL = 1U,
        SSP_FIFO_INVALID = 2U,
        SSP_FIFO_FULL = 3U
    } SSP_FIFOState;

#define IS_SSP_PRE_SCALE(param)          (((param)>=2U)&&((param)<=254U)&&(((param)%2U)==0U))
#define IS_SSP_DATA_BIT_SIZE(param)      (((param)>=4U)&&((param)<=16U))

    typedef enum {
        SSP_RX = 0U,
        SSP_TX = 1U
    } SSP_Direction;
#define IS_SSP_DIRECTION(param)  (((param) == SSP_RX) || \
                                  ((param) == SSP_TX))

    typedef struct {
        SSP_FrameFormat FrameFormat;
        uint8_t PreScale;
        uint8_t ClkRate;
        SSP_ClkPolarity ClkPolarity;
        SSP_ClkPhase ClkPhase;
        uint8_t DataSize;
        SSP_MS_Mode Mode;
    } SSP_InitTypeDef;

/* Parameter to configure SSP interrupt enable/disable Register */
#define SSP_INTCFG_NONE         ((uint32_t)0x00000000)
#define SSP_INTCFG_RX_OVERRUN   ((uint32_t)0x00000001)
#define SSP_INTCFG_RX_TIMEOUT   ((uint32_t)0x00000002)
#define SSP_INTCFG_RX           ((uint32_t)0x00000004)
#define SSP_INTCFG_TX           ((uint32_t)0x00000008)
#define SSP_INTCFG_ALL          ((uint32_t)0x0000000F)

#define IS_SSP_INT_SRC(param)  ((param) <= SSP_INTCFG_ALL )

#define IS_SSP_CLEAR_INT_SRC(param)  (((param) == SSP_INTCFG_RX_OVERRUN) || \
                                      ((param) == SSP_INTCFG_RX_TIMEOUT) || \
                                      ((param) == SSP_INTCFG_ALL) )

    typedef union {
        uint32_t All;
        struct {
            uint32_t OverRun:1;    /* Bit 0 , for TSB-M4 in IAR EWARM */
            uint32_t TimeOut:1;
            uint32_t Rx:1;
            uint32_t Tx:1;
            uint32_t Reserved:28;  /* Bit 4 to 31 */
        } Bit;
    } SSP_INTState;

/** @} */
/* End of group SSP_Exported_types */

/** @defgroup SSP_Exported_FunctionPrototypes
  * @{
  */
    void SSP_Enable(TSB_SSP_TypeDef * SSPx);
    void SSP_Disable(TSB_SSP_TypeDef * SSPx);
    void SSP_Init(TSB_SSP_TypeDef * SSPx, SSP_InitTypeDef * InitStruct);
    void SSP_SetClkPreScale(TSB_SSP_TypeDef * SSPx, uint8_t PreScale, uint8_t ClkRate);
    void SSP_SetFrameFormat(TSB_SSP_TypeDef * SSPx, SSP_FrameFormat FrameFormat);
    void SSP_SetClkPolarity(TSB_SSP_TypeDef * SSPx, SSP_ClkPolarity ClkPolarity);
    void SSP_SetClkPhase(TSB_SSP_TypeDef * SSPx, SSP_ClkPhase ClkPhase);
    void SSP_SetDataSize(TSB_SSP_TypeDef * SSPx, uint8_t DataSize);
    void SSP_SetSlaveOutputCtrl(TSB_SSP_TypeDef * SSPx, FunctionalState NewState);
    void SSP_SetMSMode(TSB_SSP_TypeDef * SSPx, SSP_MS_Mode Mode);
    void SSP_SetLoopBackMode(TSB_SSP_TypeDef * SSPx, FunctionalState NewState);
    void SSP_SetTxData(TSB_SSP_TypeDef * SSPx, uint16_t Data);
    uint16_t SSP_GetRxData(TSB_SSP_TypeDef * SSPx);
    WorkState SSP_GetWorkState(TSB_SSP_TypeDef * SSPx);
    SSP_FIFOState SSP_GetFIFOState(TSB_SSP_TypeDef * SSPx, SSP_Direction Direction);
    void SSP_SetINTConfig(TSB_SSP_TypeDef * SSPx, uint32_t IntSrc);
    SSP_INTState SSP_GetINTConfig(TSB_SSP_TypeDef * SSPx);
    SSP_INTState SSP_GetPreEnableINTState(TSB_SSP_TypeDef * SSPx);
    SSP_INTState SSP_GetPostEnableINTState(TSB_SSP_TypeDef * SSPx);
    void SSP_ClearINTFlag(TSB_SSP_TypeDef * SSPx, uint32_t IntSrc);
    void SSP_SetDMACtrl(TSB_SSP_TypeDef * SSPx, SSP_Direction Direction, FunctionalState NewState);
/** @} */
/* End of group SSP_Exported_FunctionPrototypes */

/** @} */
/* End of group SSP */

/** @} */
/* End of group TX04_Periph_Driver */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /*__TMPM46B_SSP_H */
