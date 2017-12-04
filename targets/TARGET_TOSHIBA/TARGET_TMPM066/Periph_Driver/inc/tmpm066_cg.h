/**
 *******************************************************************************
 * @file    tmpm066_cg.h
 * @brief   This file provides all the functions prototypes for CG driver.
 * @version V2.0.2.1
 * @date    2015/09/29
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM066_CG_H
#define __TMPM066_CG_H

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "TMPM066.h"
#include "tx00_common.h"

/** @addtogroup TX00_Periph_Driver
  * @{
  */

/** @addtogroup CG
  * @{
  */

/** @addtogroup CG_Exported_types
  * @{
  */

    typedef enum {
        CG_DIVIDE_1 = 0U,
        CG_DIVIDE_2 = 1U,
        CG_DIVIDE_4 = 2U,
        CG_DIVIDE_8 = 3U,
        CG_DIVIDE_16 = 4U,
        CG_DIVIDE_32 = 5U,
        CG_DIVIDE_64 = 6U,
        CG_DIVIDE_128 = 7U,
        CG_DIVIDE_256 = 8U,
        CG_DIVIDE_512 = 9U,
        CG_DIVIDE_UNKNOWN = 10U,
        CG_DIVIDE_MAX = 16U
    } CG_DivideLevel;
#define IS_CG_GEAR_DIVIDE_LEVEL(param)  (((param) == CG_DIVIDE_1) || \
                                         ((param) == CG_DIVIDE_2) || \
                                         ((param) == CG_DIVIDE_4) || \
                                         ((param) == CG_DIVIDE_8) || \
                                         ((param) == CG_DIVIDE_16))

#define IS_CG_DIVIDE_FC_LEVEL(param)    (((param) == CG_DIVIDE_1) || \
                                         ((param) == CG_DIVIDE_2) || \
                                         ((param) == CG_DIVIDE_4) || \
                                         ((param) == CG_DIVIDE_8) || \
                                         ((param) == CG_DIVIDE_16) || \
                                         ((param) == CG_DIVIDE_32) || \
                                         ((param) == CG_DIVIDE_64)|| \
                                         ((param) == CG_DIVIDE_128) || \
                                         ((param) == CG_DIVIDE_256) || \
                                         ((param) == CG_DIVIDE_512))

    typedef enum {
        CG_FOSC_OSC_INT = 0U,
        CG_FOSC_OSC_EXT = 1U,
        CG_FOSC_CLKIN_EXT = 2U
    } CG_FoscSrc;
#define IS_CG_FOSC_SRC(param)           (((param) == CG_FOSC_OSC_EXT) || \
                                         ((param) == CG_FOSC_OSC_INT) || \
                                         ((param) == CG_FOSC_CLKIN_EXT))

#define IS_CG_FOSC_STATE(param)         (((param) == CG_FOSC_OSC_EXT) || \
                                         ((param) == CG_FOSC_OSC_INT) )

    typedef enum {
        CG_WARM_UP_SRC_OSC_INT_HIGH = 0U,
        CG_WARM_UP_SRC_OSC_EXT_HIGH = 1U,
    } CG_WarmUpSrc;
#define IS_CG_WARM_UP_SRC(param)        (((param) == CG_WARM_UP_SRC_OSC_INT_HIGH) || \
                                         ((param) == CG_WARM_UP_SRC_OSC_EXT_HIGH))

    typedef enum {
        CG_FC_SRC_FOSC = 0U,
        CG_FC_SRC_FPLL = 1U
    } CG_FcSrc;
#define IS_CG_FC_SRC(param)    (((param) == CG_FC_SRC_FOSC) || \
                                ((param) == CG_FC_SRC_FPLL))

#define CG_8M_MUL_12_FPLL       (0x00C60B00UL<<8U)      /* 12 fold,  input 8MHz, output 96MHz */
#define CG_10M_MUL_8_FPLL       (0x00C60700UL<<8U)      /* 10 fold,  input 10MHz, output 80MHz */
#define CG_12M_MUL_8_FPLL       (0x00C60700UL<<8U)      /* 12 fold,  input 12MHz, output 96MHz */
#define CG_16M_MUL_6_FPLL       (0x00C60500UL<<8U)      /* 16 fold,  input 16MHz, output 96MHz */

#define IS_CG_FPLL_VALUE(param)        (((param) == CG_8M_MUL_12_FPLL) || \
                                        ((param) == CG_10M_MUL_8_FPLL) || \
                                        ((param) == CG_12M_MUL_8_FPLL) || \
                                        ((param) == CG_16M_MUL_6_FPLL))
    typedef enum {
        CG_STBY_MODE_IDLE = 0U,
        CG_STBY_MODE_STOP1 = 1U,
        CG_STBY_MODE_UNKNOWN = 2U,
        CG_STBY_MODE_MAX = 4U
    } CG_STBYMode;
#define IS_CG_STBY_MODE(param)         (((param) == CG_STBY_MODE_STOP1) || \
                                        ((param) == CG_STBY_MODE_IDLE))
#define CG_FC_PERIPH_PORTH          (0x00000001U << 7U)
#define CG_FC_PERIPH_PORTJ          (0x00000001U << 8U)
#define CG_FC_PERIPH_TMRB0_3        (0x00000001U << 9U)
#define CG_FC_PERIPH_TMRB4_6        (0x00000001U << 10U)
#define CG_FC_PERIPH_TMR16A         (0x00000001U << 11U)
#define CG_FC_PERIPH_I2C0           (0x00000001U << 12U)
#define CG_FC_PERIPH_SIO0           (0x00000001U << 13U)
#define CG_FC_PERIPH_TSPI           (0x00000001U << 14U)
#define CG_FC_PERIPH_DMAC           (0x00000001U << 15U)
#define CG_FC_PERIPH_ADC            (0x00000001U << 16U)
#define CG_FC_PERIPH_USBD           (0x00000001U << 17U)
#define CG_FC_PERIPH_TMRD           (0x00000001U << 18U)
#define CG_FC_PERIPHA_ALL           (0xFFFFFFFFU)
#define IS_CG_FC_PERIPHA(param)     ( ((param) >= CG_FC_PERIPH_PORTH) && ((param) <= CG_FC_PERIPH_TMRD ))

#define CG_FC_PERIPH_TMRB7          (0x00000001U << 28U)
#define CG_FC_PERIPH_SIO1           (0x00000001U << 29U)
#define CG_FC_PERIPH_WDT            (0x00000001U << 30U)
#define CG_FC_PERIPH_I2C1           (0x00000001U << 31U)
#define CG_FC_PERIPHB_ALL           (0xFFFFFFFFU)
#define IS_CG_FC_PERIPHB(param)     (((param) >= CG_FC_PERIPH_TMRB7))

#define CG_FPLL_PERIPH_TMRD          (0x00000001U << 5U)
#define CG_EHCLKSEL_8_24_48MHZ       (0x00000001U << 4U)
#define CG_USBSEL_PLL_CLOCKIN        (0x00000001U << 1U)
#define CG_USBENA_USB                (0x00000001U)
#define CG_FPLL_OPTIONAL_ALL         (0x0FFFFFFFU)
#define IS_CG_FPLL_OPTIONAL(param)   (((param) > 0U) && ((param) <= CG_FPLL_OPTIONAL_ALL))

/** @} */
/* End of group CG_Exported_types */

/** @defgroup CG_Exported_FunctionPrototypes
  * @{
  */
    void CG_SetFgearLevel(CG_DivideLevel DivideFgearFromFc);
    CG_DivideLevel CG_GetFgearLevel(void);
    Result CG_SetPhiT0Level(CG_DivideLevel DividePhiT0FromFc);
    CG_DivideLevel CG_GetPhiT0Level(void);
    void CG_SetWarmUpTime(CG_WarmUpSrc Source, uint16_t Time);
    void CG_StartWarmUp(void);
    WorkState CG_GetWarmUpState(void);
    Result CG_SetFPLLValue(uint32_t NewValue);
    uint32_t CG_GetFPLLValue(void);
    Result CG_SetPLL(FunctionalState NewState);
    FunctionalState CG_GetPLLState(void);
    void CG_SetFoscSrc(CG_FoscSrc Source);
    CG_FoscSrc CG_GetFoscSrc(void);
    void CG_SetSTBYMode(CG_STBYMode Mode);
    CG_STBYMode CG_GetSTBYMode(void);
    void CG_SetFcSrc(CG_FcSrc Source);
    CG_FcSrc CG_GetFcSrc(void);
    void CG_SetProtectCtrl(FunctionalState NewState);
    void CG_SetFcPeriphA(uint32_t Periph, FunctionalState NewState);
    void CG_SetFcPeriphB(uint32_t Periph, FunctionalState NewState);
    void CG_SetFcOptional(uint32_t Periph, FunctionalState NewState);
    void CG_SetADCClkSupply(FunctionalState NewState);

/** @} */
/* End of group CG_Exported_FunctionPrototype */

/** @} */
/* End of group CG */

/** @} */
/* End of group TX00_Periph_Driver */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM066_CG_H */
