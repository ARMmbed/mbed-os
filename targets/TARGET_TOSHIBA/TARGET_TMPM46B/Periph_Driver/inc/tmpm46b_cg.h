/**
 *******************************************************************************
 * @file    tmpm46b_cg.h
 * @brief   This file provides all the functions prototypes for CG driver.
 * @version V2.0.2.1
 * @date    2015/02/26
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM46B_CG_H
#define __TMPM46B_CG_H

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "TMPM46B.h"
#include "tx04_common.h"

/** @addtogroup TX04_Periph_Driver
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
        CG_DIVIDE_MAX = 11U
    } CG_DivideLevel;
#define IS_CG_GEAR_DIVIDE_LEVEL(param)  (((param) == CG_DIVIDE_1) || \
                                         ((param) == CG_DIVIDE_2) || \
                                         ((param) == CG_DIVIDE_4) || \
                                         ((param) == CG_DIVIDE_8) || \
                                         ((param) == CG_DIVIDE_16))

#define IS_CG_FC_DIVIDE_LEVEL(param)    (((param) == CG_DIVIDE_1) || \
                                         ((param) == CG_DIVIDE_2) || \
                                         ((param) == CG_DIVIDE_4) || \
                                         ((param) == CG_DIVIDE_8) || \
                                         ((param) == CG_DIVIDE_16)|| \
                                         ((param) == CG_DIVIDE_32))

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
        CG_FOSC_OSC_EXT = 0U,
        CG_FOSC_OSC_INT = 1U,
        CG_FOSC_CLKIN_EXT = 2U
    } CG_FoscSrc;
#define IS_CG_FOSC_SRC(param)           (((param) == CG_FOSC_OSC_EXT) || \
                                         ((param) == CG_FOSC_OSC_INT) || \
                                         ((param) == CG_FOSC_CLKIN_EXT))

#define IS_CG_FOSC_STATE(param)         (((param) == CG_FOSC_OSC_EXT) || \
                                         ((param) == CG_FOSC_OSC_INT) )

    typedef enum {
        CG_SCOUT_SRC_FS = 0U,
        CG_SCOUT_SRC_FSYS_DIVIDE_8 = 1U,
        CG_SCOUT_SRC_FSYS_DIVIDE_4 = 2U,
        CG_SCOUT_SRC_FOSC = 3U
    } CG_SCOUTSrc;
#define IS_CG_SCOUT_SRC(param)          (((param) == CG_SCOUT_SRC_FS)            || \
                                         ((param) == CG_SCOUT_SRC_FSYS_DIVIDE_8) || \
                                         ((param) == CG_SCOUT_SRC_FSYS_DIVIDE_4) || \
                                         ((param) == CG_SCOUT_SRC_FOSC))

    typedef enum {
        CG_WARM_UP_SRC_OSC_INT_HIGH = 0U,
        CG_WARM_UP_SRC_OSC_EXT_HIGH = 1U,
        CG_WARM_UP_SRC_OSC_EXT_LOW = 2U
    } CG_WarmUpSrc;
#define IS_CG_WARM_UP_SRC(param)        (((param) == CG_WARM_UP_SRC_OSC_INT_HIGH) || \
                                         ((param) == CG_WARM_UP_SRC_OSC_EXT_HIGH) || \
                                         ((param) == CG_WARM_UP_SRC_OSC_EXT_LOW))

    typedef enum {
        CG_FC_SRC_FOSC = 0U,
        CG_FC_SRC_FPLL = 1U
    } CG_FcSrc;
#define IS_CG_FC_SRC(param)    (((param) == CG_FC_SRC_FOSC) || \
                                ((param) == CG_FC_SRC_FPLL))

#define CG_8M_MUL_4_FPLL        (0x00006A0FUL<<1U)      /* 4 fold,  input 8MHz,  output 32MHz  */
#define CG_8M_MUL_5_FPLL        (0x00006A13UL<<1U)      /* 5 fold,  input 8MHz,  output 40MHz  */
#define CG_8M_MUL_6_FPLL        (0x00006917UL<<1U)      /* 6 fold,  input 8MHz,  output 48MHz  */
#define CG_8M_MUL_8_FPLL        (0x0000691FUL<<1U)      /* 8 fold,  input 8MHz,  output 64MHz  */
#define CG_8M_MUL_10_FPLL       (0x00006A26UL<<1U)      /* 10 fold, input 8MHz,  output 80MHz  */
#define CG_8M_MUL_12_FPLL       (0x0000692EUL<<1U)      /* 12 fold, input 8MHz,  output 96MHz  */
#define CG_10M_MUL_4_FPLL       (0x00006A0FUL<<1U)      /* 4 fold,  input 10MHz, output 40MHz  */
#define CG_10M_MUL_5_FPLL       (0x00006A13UL<<1U)      /* 5 fold,  input 10MHz, output 50MHz  */
#define CG_10M_MUL_6_FPLL       (0x00006917UL<<1U)      /* 6 fold,  input 10MHz, output 60MHz  */
#define CG_10M_MUL_8_FPLL       (0x0000691FUL<<1U)      /* 8 fold,  input 10MHz, output 80MHz  */
#define CG_10M_MUL_10_FPLL      (0x00006A26UL<<1U)      /* 10 fold, input 10MHz, output 100MHz */
#define CG_10M_MUL_12_FPLL      (0x0000692EUL<<1U)      /* 12 fold, input 10MHz, output 120MHz */
#define CG_12M_MUL_4_FPLL       (0x00006A0FUL<<1U)      /* 4 fold,  input 12MHz, output 48MHz  */
#define CG_12M_MUL_5_FPLL       (0x00006A13UL<<1U)      /* 5 fold,  input 12MHz, output 60MHz  */
#define CG_12M_MUL_6_FPLL       (0x00006917UL<<1U)      /* 6 fold,  input 12MHz, output 72MHz  */
#define CG_12M_MUL_8_FPLL       (0x0000691FUL<<1U)      /* 8 fold,  input 12MHz, output 96MHz  */
#define CG_12M_MUL_10_FPLL      (0x00006A26UL<<1U)      /* 10 fold, input 12MHz, output 120MHz */
#define CG_16M_MUL_4_FPLL       (0x00006A0FUL<<1U)      /* 4 fold,  input 16MHz, output 64MHz  */
#define CG_16M_MUL_5_FPLL       (0x00006A13UL<<1U)      /* 5 fold,  input 16MHz, output 80MHz  */
#define IS_CG_FPLL_VALUE(param)        (((param) == CG_8M_MUL_4_FPLL)   || \
                                        ((param) == CG_8M_MUL_5_FPLL)   || \
                                        ((param) == CG_8M_MUL_6_FPLL)   || \
                                        ((param) == CG_8M_MUL_8_FPLL)   || \
                                        ((param) == CG_8M_MUL_10_FPLL)  || \
                                        ((param) == CG_8M_MUL_12_FPLL)  || \
                                        ((param) == CG_10M_MUL_4_FPLL)  || \
                                        ((param) == CG_10M_MUL_5_FPLL)  || \
                                        ((param) == CG_10M_MUL_6_FPLL)  || \
                                        ((param) == CG_10M_MUL_8_FPLL)  || \
                                        ((param) == CG_10M_MUL_10_FPLL) || \
                                        ((param) == CG_10M_MUL_12_FPLL) || \
                                        ((param) == CG_12M_MUL_4_FPLL)  || \
                                        ((param) == CG_12M_MUL_5_FPLL)  || \
                                        ((param) == CG_12M_MUL_6_FPLL)  || \
                                        ((param) == CG_12M_MUL_8_FPLL)  || \
                                        ((param) == CG_12M_MUL_10_FPLL) || \
                                        ((param) == CG_16M_MUL_4_FPLL)  || \
                                        ((param) == CG_16M_MUL_5_FPLL))

    typedef enum {
        CG_STBY_MODE_UNKNOWN = 0U,
        CG_STBY_MODE_STOP1 = 1U,
        CG_STBY_MODE_IDLE = 3U,
        CG_STBY_MODE_STOP2 = 5U,
        CG_STBY_MODE_MAX = 8U
    } CG_STBYMode;
#define IS_CG_STBY_MODE(param)         (((param) == CG_STBY_MODE_STOP1) || \
                                        ((param) == CG_STBY_MODE_IDLE)|| \
                                        ((param) == CG_STBY_MODE_STOP2))

    typedef enum {
        CG_PHIT0_SRC_FGEAR = 0U,
        CG_PHIT0_SRC_FC = 1U,
        CG_PHIT0_SRC_MAX = 2U
    } CG_PhiT0Src;
#define IS_CG_PHIT0_SRC(param)  (((param) == CG_PHIT0_SRC_FGEAR) || \
                                 ((param) == CG_PHIT0_SRC_FC))

    typedef enum {
        CG_INT_SRC_1 = 0U,
        CG_INT_SRC_2 = 1U,
        CG_INT_SRC_7 = 2U,
        CG_INT_SRC_8 = 3U,
        CG_INT_SRC_D = 4U,
        CG_INT_SRC_E = 5U,
        CG_INT_SRC_F = 6U,
        CG_INT_SRC_RTC = 7U
    } CG_INTSrc;

#define IS_CG_INT_SRC(param)    (((param) == CG_INT_SRC_1) || \
                                 ((param) == CG_INT_SRC_2) || \
                                 ((param) == CG_INT_SRC_7) || \
                                 ((param) == CG_INT_SRC_8) || \
                                 ((param) == CG_INT_SRC_D) || \
                                 ((param) == CG_INT_SRC_E) || \
                                 ((param) == CG_INT_SRC_F) || \
                                 ((param) == CG_INT_SRC_RTC))

    typedef enum {
        CG_INT_ACTIVE_STATE_L = 0x00U,
        CG_INT_ACTIVE_STATE_H = 0x10U,
        CG_INT_ACTIVE_STATE_FALLING = 0x20U,
        CG_INT_ACTIVE_STATE_RISING = 0x30U,
        CG_INT_ACTIVE_STATE_BOTH_EDGES = 0x40U,
        CG_INT_ACTIVE_STATE_INVALID = 0x50U
    } CG_INTActiveState;
#define IS_CG_INT_ACTIVE_STATE(param)        (((param) == CG_INT_ACTIVE_STATE_L) || \
                                              ((param) == CG_INT_ACTIVE_STATE_H) || \
                                              ((param) == CG_INT_ACTIVE_STATE_FALLING) || \
                                              ((param) == CG_INT_ACTIVE_STATE_RISING) || \
                                              ((param) == CG_INT_ACTIVE_STATE_BOTH_EDGES))

#define IS_CG_INT_RTC_ACTIVE_STATE(param)    ((param) == CG_INT_ACTIVE_STATE_FALLING)

#define CG_FC_PERIPH_PORTA          (0x00000001U << 0U)
#define CG_FC_PERIPH_PORTB          (0x00000001U << 1U)
#define CG_FC_PERIPH_PORTC          (0x00000001U << 2U)
#define CG_FC_PERIPH_PORTD          (0x00000001U << 3U)
#define CG_FC_PERIPH_PORTE          (0x00000001U << 4U)
#define CG_FC_PERIPH_PORTF          (0x00000001U << 5U)
#define CG_FC_PERIPH_PORTG          (0x00000001U << 6U)
#define CG_FC_PERIPH_PORTH          (0x00000001U << 7U)
#define CG_FC_PERIPH_PORTJ          (0x00000001U << 8U)
#define CG_FC_PERIPH_PORTK          (0x00000001U << 9U)
#define CG_FC_PERIPH_PORTL          (0x00000001U << 10U)
#define CG_FC_PERIPH_TMRB0          (0x00000001U << 13U)
#define CG_FC_PERIPH_TMRB1          (0x00000001U << 14U)
#define CG_FC_PERIPH_TMRB2          (0x00000001U << 15U)
#define CG_FC_PERIPH_TMRB3          (0x00000001U << 16U)
#define CG_FC_PERIPH_TMRB4          (0x00000001U << 17U)
#define CG_FC_PERIPH_TMRB5          (0x00000001U << 18U)
#define CG_FC_PERIPH_TMRB6          (0x00000001U << 19U)
#define CG_FC_PERIPH_TMRB7          (0x00000001U << 20U)
#define CG_FC_PERIPH_MPT0           (0x00000001U << 27U)
#define CG_FC_PERIPH_MPT1           (0x00000001U << 28U)
#define CG_FC_PERIPH_MPT2           (0x00000001U << 29U)
#define CG_FC_PERIPH_MPT3           (0x00000001U << 30U)
#define CG_FC_PERIPH_TRACE          (0x00000001U << 31U)
#define CG_FC_PERIPHA_ALL           (0xF81FE7FFU)
#define IS_CG_FC_PERIPHA(param)     ((param) > 0U)

#define CG_FC_PERIPH_SIO_UART0      (0x00000001U << 0U)
#define CG_FC_PERIPH_SIO_UART1      (0x00000001U << 1U)
#define CG_FC_PERIPH_SIO_UART2      (0x00000001U << 2U)
#define CG_FC_PERIPH_SIO_UART3      (0x00000001U << 3U)
#define CG_FC_PERIPH_UART0          (0x00000001U << 10U)
#define CG_FC_PERIPH_UART1          (0x00000001U << 11U)
#define CG_FC_PERIPH_I2C0           (0x00000001U << 12U)
#define CG_FC_PERIPH_I2C1           (0x00000001U << 13U)
#define CG_FC_PERIPH_I2C2           (0x00000001U << 14U)
#define CG_FC_PERIPH_SSP0           (0x00000001U << 17U)
#define CG_FC_PERIPH_SSP1           (0x00000001U << 18U)
#define CG_FC_PERIPH_SSP2           (0x00000001U << 19U)
#define CG_FC_PERIPH_EBIF           (0x00000001U << 20U)
#define CG_FC_PERIPH_DMACA          (0x00000001U << 21U)
#define CG_FC_PERIPH_DMACB          (0x00000001U << 22U)
#define CG_FC_PERIPH_DMACC          (0x00000001U << 23U)
#define CG_FC_PERIPH_DMAIF          (0x00000001U << 24U)
#define CG_FC_PERIPH_ADC            (0x00000001U << 25U)
#define CG_FC_PERIPH_WDT            (0x00000001U << 26U)
#define CG_FC_PERIPH_MLA            (0x00000001U << 28U)
#define CG_FC_PERIPH_ESG            (0x00000001U << 29U)
#define CG_FC_PERIPH_SHA            (0x00000001U << 30U)
#define CG_FC_PERIPH_AES            (0x00000001U << 31U)
#define CG_FC_PERIPHB_ALL           (0xF7FE7C0FU)
#define IS_CG_FC_PERIPHB(param)     (((param) > 0U) && ((param) <= CG_FC_PERIPHB_ALL))

    typedef union {
        uint32_t All;
        struct {
            uint32_t WDT:1;
            uint32_t Reserved0:1;
            uint32_t DetectLowVoltage:1;
            uint32_t Reserved1:29;
        } Bit;
    } CG_NMIFactor;

    typedef union {
        uint32_t All;
        struct {
            uint32_t PinReset:1;
            uint32_t OSCFLF:1;
            uint32_t WDTReset:1;
            uint32_t STOP2Reset:1;
            uint32_t DebugReset:1;
            uint32_t Reserved0:1;
            uint32_t LVDReset:1;
            uint32_t Reserved1:25;
        } Bit;
    } CG_ResetFlag;

/** @} */
/* End of group CG_Exported_types */

/** @defgroup CG_Exported_FunctionPrototypes
  * @{
  */
    void CG_SetFgearLevel(CG_DivideLevel DivideFgearFromFc);
    CG_DivideLevel CG_GetFgearLevel(void);
    void CG_SetPhiT0Src(CG_PhiT0Src PhiT0Src);
    CG_PhiT0Src CG_GetPhiT0Src(void);
    Result CG_SetPhiT0Level(CG_DivideLevel DividePhiT0FromFc);
    CG_DivideLevel CG_GetPhiT0Level(void);
    void CG_SetSCOUTSrc(CG_SCOUTSrc Source);
    CG_SCOUTSrc CG_GetSCOUTSrc(void);
    void CG_SetWarmUpTime(CG_WarmUpSrc Source, uint16_t Time);
    void CG_StartWarmUp(void);
    WorkState CG_GetWarmUpState(void);
    Result CG_SetFPLLValue(uint32_t NewValue);
    uint32_t CG_GetFPLLValue(void);
    Result CG_SetPLL(FunctionalState NewState);
    FunctionalState CG_GetPLLState(void);
    Result CG_SetFosc(CG_FoscSrc Source, FunctionalState NewState);
    void CG_SetFoscSrc(CG_FoscSrc Source);
    CG_FoscSrc CG_GetFoscSrc(void);
    FunctionalState CG_GetFoscState(CG_FoscSrc Source);
    void CG_SetSTBYMode(CG_STBYMode Mode);
    CG_STBYMode CG_GetSTBYMode(void);
    void CG_SetPortKeepInStop2Mode(FunctionalState NewState);
    FunctionalState CG_GetPortKeepInStop2Mode(void);
    Result CG_SetFcSrc(CG_FcSrc Source);
    CG_FcSrc CG_GetFcSrc(void);
    void CG_SetProtectCtrl(FunctionalState NewState);
    void CG_SetSTBYReleaseINTSrc(CG_INTSrc INTSource,
                                 CG_INTActiveState ActiveState, FunctionalState NewState);
    CG_INTActiveState CG_GetSTBYReleaseINTState(CG_INTSrc INTSource);
    void CG_ClearINTReq(CG_INTSrc INTSource);
    CG_NMIFactor CG_GetNMIFlag(void);
    FunctionalState CG_GetIOSCFlashFlag(void);
    CG_ResetFlag CG_GetResetFlag(void);
    void CG_SetADCClkSupply(FunctionalState NewState);
    void CG_SetFcPeriphA(uint32_t Periph, FunctionalState NewState);
    void CG_SetFcPeriphB(uint32_t Periph, FunctionalState NewState);
    void CG_SetFs(FunctionalState NewState);

/** @} */
/* End of group CG_Exported_FunctionPrototype */

/** @} */
/* End of group CG */

/** @} */
/* End of group TX04_Periph_Driver */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM46B_CG_H */
