/**
 *******************************************************************************
 * @file    tmpm066_intifsd.h
 * @brief   This file provides all the functions prototypes for INTIFSD driver.
 * @version V2.0.2.2
 * @date    2016/02/09
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM066_INTIFSD_H
#define __TMPM066_INTIFSD_H

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "TMPM066.h"
#include "tx00_common.h"

/** @addtogroup TX00_Periph_Driver
  * @{
  */

/** @addtogroup INTIFSD
  * @{
  */

/** @addtogroup INTIFSD_Exported_types
  * @{
  */                                      
    typedef union {
        uint32_t All;
        struct {
            uint32_t Reserved1:16;
            uint32_t DetectLowVoltage:1;           
            uint32_t DetectOverVoltage:1;  
            uint32_t WDT:1;
            uint32_t Reserved2:13;
        } Bit;
    } INTIFSD_NMIFactor;   

    typedef struct {
        uint32_t All;
    } INTIFSD_IntFlag3Factor;

    typedef struct {
        uint32_t All;
    } INTIFSD_IntFlag4Factor;

    typedef struct {
        uint32_t All;
    } INTIFSD_IntFlag5Factor;

    typedef enum {
        INTIFSD_INT_SRC_LVD_PSFD = 0U,
        INTIFSD_INT_SRC_LVD_PRD = 1U,
        INTIFSD_INT_SRC_WDT = 2U,
        INTIFSD_INT_SRC_DMAC_0 = 3U,
        INTIFSD_INT_SRC_DMAC_1 = 4U,
        INTIFSD_INT_SRC_DMAC_2 = 5U,
        INTIFSD_INT_SRC_DMAC_3 = 6U,
        INTIFSD_INT_SRC_DMAC_4 = 7U,
        INTIFSD_INT_SRC_DMAC_5 = 8U,
        INTIFSD_INT_SRC_DMAC_6 = 9U,
        INTIFSD_INT_SRC_DMAC_7 = 10U,
        INTIFSD_INT_SRC_DMAC_8 = 11U,
        INTIFSD_INT_SRC_DMAC_9 = 12U,
        INTIFSD_INT_SRC_DMAC_10 = 13U,
        INTIFSD_INT_SRC_DMAC_11 = 14U,
        INTIFSD_INT_SRC_DMAC_12 = 15U,
        INTIFSD_INT_SRC_DMAC_13 = 16U,
        INTIFSD_INT_SRC_DMAC_14 = 17U,
        INTIFSD_INT_SRC_DMAC_15 = 18U,
        INTIFSD_INT_SRC_DMAC_16 = 19U,
        INTIFSD_INT_SRC_DMAC_17 = 20U,
        INTIFSD_INT_SRC_DMAC_18 = 21U,
        INTIFSD_INT_SRC_DMAC_19 = 22U,
        INTIFSD_INT_SRC_DMAC_20 = 23U,
        INTIFSD_INT_SRC_DMAC_21 = 24U,
        INTIFSD_INT_SRC_DMAC_22 = 25U,
        INTIFSD_INT_SRC_DMAC_23 = 26U,
        INTIFSD_INT_SRC_DMAC_24 = 27U,
        INTIFSD_INT_SRC_DMAC_25 = 28U,
        INTIFSD_INT_SRC_DMAC_26 = 29U,
        INTIFSD_INT_SRC_DMAC_27 = 30U,
        INTIFSD_INT_SRC_DMAC_28 = 31U,
        INTIFSD_INT_SRC_DMAC_29 = 32U,
        INTIFSD_INT_SRC_DMAC_30 = 33U,
        INTIFSD_INT_SRC_DMAC_31 = 34U,
        INTIFSD_INT_SRC_DMAC_ERR = 35U,
        INTIFSD_INT_SRC_TMRB_0_MDOVF = 36U,
        INTIFSD_INT_SRC_TMRB_0_CAP0 = 37U,
        INTIFSD_INT_SRC_TMRB_0_CAP1 = 38U,
        INTIFSD_INT_SRC_TMRB_1_MDOVF = 39U,
        INTIFSD_INT_SRC_TMRB_1_CAP0 = 40U,
        INTIFSD_INT_SRC_TMRB_1_CAP1 = 41U,
        INTIFSD_INT_SRC_TMRB_2_MDOVF = 42U,
        INTIFSD_INT_SRC_TMRB_2_CAP0 = 43U,
        INTIFSD_INT_SRC_TMRB_2_CAP1 = 44U,
        INTIFSD_INT_SRC_TMRB_3_MDOVF = 45U,
        INTIFSD_INT_SRC_TMRB_3_CAP0 = 46U,
        INTIFSD_INT_SRC_TMRB_3_CAP1 = 47U,
        INTIFSD_INT_SRC_TMRB_4_MDOVF = 48U,
        INTIFSD_INT_SRC_TMRB_4_CAP0 = 49U,
        INTIFSD_INT_SRC_TMRB_4_CAP1 = 50U,
        INTIFSD_INT_SRC_TMRB_5_MDOVF = 51U,
        INTIFSD_INT_SRC_TMRB_5_CAP0 = 52U,
        INTIFSD_INT_SRC_TMRB_5_CAP1 = 53U,
        INTIFSD_INT_SRC_TMRB_6_MDOVF = 54U,
        INTIFSD_INT_SRC_TMRB_6_CAP0 = 55U,
        INTIFSD_INT_SRC_TMRB_6_CAP1 = 56U,
        INTIFSD_INT_SRC_TMRB_7_MDOVF = 57U,
        INTIFSD_INT_SRC_TMRB_7_CAP0 = 58U,
        INTIFSD_INT_SRC_TMRB_7_CAP1 = 59U,
        INTIFSD_INT_SRC_TMRD_00 = 60U,
        INTIFSD_INT_SRC_TMRD_01 = 61U,
        INTIFSD_INT_SRC_TMRD_02 = 62U,
        INTIFSD_INT_SRC_TMRD_03 = 63U,
        INTIFSD_INT_SRC_TMRD_04 = 64U,
        INTIFSD_INT_SRC_TMRD_10 = 65U,
        INTIFSD_INT_SRC_TMRD_11 = 66U,
        INTIFSD_INT_SRC_TMRD_12 = 67U,
        INTIFSD_INT_SRC_TMRD_13 = 68U,
        INTIFSD_INT_SRC_TMRD_14 = 69U
     } INTIFSD_INTSrc;

#define IS_INTIFSD_INT_SRC(param)   (((param) == INTIFSD_INT_SRC_LVD_PSFD) || \
                                    ((param) == INTIFSD_INT_SRC_LVD_PRD) || \
                                    ((param) == INTIFSD_INT_SRC_WDT) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_0) ||  \
                                    ((param) == INTIFSD_INT_SRC_DMAC_1) ||  \
                                    ((param) == INTIFSD_INT_SRC_DMAC_2) ||  \
                                    ((param) == INTIFSD_INT_SRC_DMAC_3) ||  \
                                    ((param) == INTIFSD_INT_SRC_DMAC_4) ||  \
                                    ((param) == INTIFSD_INT_SRC_DMAC_5) ||  \
                                    ((param) == INTIFSD_INT_SRC_DMAC_6) ||  \
                                    ((param) == INTIFSD_INT_SRC_DMAC_7) ||  \
                                    ((param) == INTIFSD_INT_SRC_DMAC_8) ||  \
                                    ((param) == INTIFSD_INT_SRC_DMAC_9) ||  \
                                    ((param) == INTIFSD_INT_SRC_DMAC_10) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_11) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_12) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_13) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_14) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_15) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_16) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_17) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_18) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_19) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_20) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_21) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_22) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_23) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_24) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_25) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_26) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_27) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_28) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_29) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_30) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_31) || \
                                    ((param) == INTIFSD_INT_SRC_DMAC_ERR) || \
                                    ((param) == INTIFSD_INT_SRC_TMRB_0_MDOVF) || \
                                    ((param) == INTIFSD_INT_SRC_TMRB_0_CAP0) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_0_CAP1) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_1_MDOVF) || \
                                    ((param) == INTIFSD_INT_SRC_TMRB_1_CAP0) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_1_CAP1) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_2_MDOVF) || \
                                    ((param) == INTIFSD_INT_SRC_TMRB_2_CAP0) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_2_CAP1) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_3_MDOVF) || \
                                    ((param) == INTIFSD_INT_SRC_TMRB_3_CAP0) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_3_CAP1) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_4_MDOVF) || \
                                    ((param) == INTIFSD_INT_SRC_TMRB_4_CAP0) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_4_CAP1) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_5_MDOVF) || \
                                    ((param) == INTIFSD_INT_SRC_TMRB_5_CAP0) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_5_CAP1) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_6_MDOVF) || \
                                    ((param) == INTIFSD_INT_SRC_TMRB_6_CAP0) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_6_CAP1) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_7_MDOVF) || \
                                    ((param) == INTIFSD_INT_SRC_TMRB_7_CAP0) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRB_7_CAP1) ||  \
                                    ((param) == INTIFSD_INT_SRC_TMRD_00) || \
                                    ((param) == INTIFSD_INT_SRC_TMRD_01) || \
                                    ((param) == INTIFSD_INT_SRC_TMRD_02) || \
                                    ((param) == INTIFSD_INT_SRC_TMRD_03) || \
                                    ((param) == INTIFSD_INT_SRC_TMRD_04) || \
                                    ((param) == INTIFSD_INT_SRC_TMRD_10) || \
                                    ((param) == INTIFSD_INT_SRC_TMRD_11) || \
                                    ((param) == INTIFSD_INT_SRC_TMRD_12) || \
                                    ((param) == INTIFSD_INT_SRC_TMRD_13) || \
                                    ((param) == INTIFSD_INT_SRC_TMRD_14))

/** @} */
/* End of group INTIFSD_Exported_types */

/** @defgroup INTIFSD_Exported_FunctionPrototypes
  * @{
  */
    INTIFSD_NMIFactor INTIFSD_GetNMIFlag(void);

    void INTIFSD_ClearINTReq(INTIFSD_INTSrc INTSource);

    INTIFSD_IntFlag3Factor INTIFSD_GetIntFlag3(void);
    INTIFSD_IntFlag4Factor INTIFSD_GetIntFlag4(void);
    INTIFSD_IntFlag5Factor INTIFSD_GetIntFlag5(void);

/** @} */
/* End of group INTIFSD_Exported_FunctionPrototype */

/** @} */
/* End of group INTIFSD */

/** @} */
/* End of group TX00_Periph_Driver */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM066_INTIFSD_H */
