/**
 *******************************************************************************
 * @file    tmpm066_intifao.h
 * @brief   This file provides all the functions prototypes for INTIFAO driver.
 * @version V2.0.2.1
 * @date    2015/10/19
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM066_INTIFAO_H
#define __TMPM066_INTIFAO_H

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "TMPM066.h"
#include "tx00_common.h"

/** @addtogroup TX00_Periph_Driver
  * @{
  */

/** @addtogroup INTIFAO
  * @{
  */

/** @addtogroup INTIFAO_Exported_types
  * @{
  */
    typedef struct {
        uint32_t All;
    } INTIFAO_IntFlag1Factor;

    typedef enum {
        INTIFAO_INT_SRC_0 = 0U,
        INTIFAO_INT_SRC_1 = 1U,
        INTIFAO_INT_SRC_2 = 2U,
        INTIFAO_INT_SRC_3 = 3U,
        INTIFAO_INT_SRC_4 = 4U,
        INTIFAO_INT_SRC_5 = 5U,
        INTIFAO_INT_SRC_I2CS = 6U,
        INTIFAO_INT_SRC_USBWKUP = 7U
     } INTIFAO_INTSrc;
     
#define IS_INTIFAO_INT_SRC(param)    (((param) == INTIFAO_INT_SRC_0) || \
                                 ((param) == INTIFAO_INT_SRC_1) || \
                                 ((param) == INTIFAO_INT_SRC_2) || \
                                 ((param) == INTIFAO_INT_SRC_3) || \
                                 ((param) == INTIFAO_INT_SRC_4) || \
                                 ((param) == INTIFAO_INT_SRC_5) || \
                                 ((param) == INTIFAO_INT_SRC_I2CS) || \
                                 ((param) == INTIFAO_INT_SRC_USBWKUP))

    typedef enum {
        INTIFAO_INT_ACTIVE_STATE_L = 0x0U,
        INTIFAO_INT_ACTIVE_STATE_H = 0x1U,
        INTIFAO_INT_ACTIVE_STATE_FALLING = 0x2U,
        INTIFAO_INT_ACTIVE_STATE_RISING = 0x3U,
        INTIFAO_INT_ACTIVE_STATE_BOTH_EDGES = 0x4U,
        INTIFAO_INT_ACTIVE_STATE_INVALID = 0x5U
    } INTIFAO_INTActiveState;
#define IS_INTIFAO_INT_ACTIVE_STATE(param)        (((param) == INTIFAO_INT_ACTIVE_STATE_L) || \
                                              ((param) == INTIFAO_INT_ACTIVE_STATE_H) || \
                                              ((param) == INTIFAO_INT_ACTIVE_STATE_FALLING) || \
                                              ((param) == INTIFAO_INT_ACTIVE_STATE_RISING) || \
                                              ((param) == INTIFAO_INT_ACTIVE_STATE_BOTH_EDGES))

#define IS_INTIFAO_INT_I2CS_USBWKUP_ACTIVE_STATE(param)  ((param) == INTIFAO_INT_ACTIVE_STATE_RISING)
               
/** @} */
/* End of group INTIFAO_Exported_types */

/** @defgroup INTIFAO_Exported_FunctionPrototypes
  * @{
  */
    void INTIFAO_SetSTBYReleaseINTSrc(INTIFAO_INTSrc INTSource,                                     
                                 INTIFAO_INTActiveState ActiveState, FunctionalState NewState);
    INTIFAO_INTActiveState INTIFAO_GetSTBYReleaseINTState(INTIFAO_INTSrc INTSource);                     
    void INTIFAO_ClearINTReq(INTIFAO_INTSrc INTSource);   

    INTIFAO_IntFlag1Factor INTIFAO_GetIntFlag1(void);

/** @} */
/* End of group INTIFAO_Exported_FunctionPrototype */

/** @} */
/* End of group INTIFAO */

/** @} */
/* End of group TX00_Periph_Driver */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM066_INTIFAO_H */
