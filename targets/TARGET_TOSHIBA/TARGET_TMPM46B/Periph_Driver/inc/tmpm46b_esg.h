/**
 *******************************************************************************
 * @file    tmpm46b_esg.h
 * @brief   This file provides all the functions prototypes for ESG driver.
 * @version V2.0.2.1
 * @date    2015/02/04
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM46B_ESG_H
#define __TMPM46B_ESG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "TMPM46B.h"
#include "tx04_common.h"     

/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @addtogroup ESG
  * @{
  */

/** @defgroup ESG_Exported_Constants
  * @{
  */

/** @} */
/* End of ESG_Exported_Constants */

/** @defgroup ESG_Exported_Types 
  * @{
  */
    typedef enum {
        ESG_CALCULATION_COMPLETE = 0U,
        ESG_CALCULATION_PROCESS = 1U
    } ESG_CalculationStatus;

    typedef enum {
        ESG_LATCH_TIMING_1 = 0U,
        ESG_LATCH_TIMING_2 = 1U,
        ESG_LATCH_TIMING_3 = 2U,
        ESG_LATCH_TIMING_4 = 3U,
        ESG_LATCH_TIMING_5 = 4U,
        ESG_LATCH_TIMING_6 = 5U,
        ESG_LATCH_TIMING_7 = 6U,
        ESG_LATCH_TIMING_8 = 7U,
        ESG_LATCH_TIMING_9 = 8U,
        ESG_LATCH_TIMING_10 = 9U,
        ESG_LATCH_TIMING_11 = 10U,
        ESG_LATCH_TIMING_12 = 11U,
        ESG_LATCH_TIMING_13 = 12U,
        ESG_LATCH_TIMING_14 = 13U,
        ESG_LATCH_TIMING_15 = 14U,
        ESG_LATCH_TIMING_16 = 15U
    } ESG_LatchTiming;
#define IS_ESG_LATCH_TIMING(param)       ((param) <= ESG_LATCH_TIMING_16)

/** @} */
/* End of ESG_Exported_types */

/** @addtogroup ESG_Exported_Types
  * @{
  */

/**
  * @brief ESG Init Structure definition
  */
/** @} */
/* End of group ESG_Exported_Types */

/** @defgroup ESG_Exported_FunctionPrototypes
  * @{
  */
  
    Result ESG_Startup(void);
    Result ESG_SetLatchTiming(ESG_LatchTiming Value);
    uint32_t ESG_GetLatchTiming(void);
    Result ESG_SetFintiming(uint16_t Fintming);
    uint16_t ESG_GetFintiming(void);
    Result ESG_ClrInt(void);
    FunctionalState ESG_GetIntStatus(void);
    void ESG_IPReset(void);
    ESG_CalculationStatus ESG_GetCalculationStatus(void);
    void ESG_GetResult(uint32_t Seed[16U]);

/** @} */
/* End of ESG_Exported_FunctionPrototypes */

/** @} */
/* End of group ESG */

/** @} */
/* End of group TX04_Periph_Driver */

#ifdef __cplusplus
}
#endif
#endif                          /* __TMPM46B_ESG_H */
