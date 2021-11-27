/**
 * @file    rtc_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the RTC Peripheral Module.
 */

/* ****************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *
 *************************************************************************** */

#ifndef _RTC_REVA_REGS_H_
#define _RTC_REVA_REGS_H_

/* **** Includes **** */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
 
#if defined (__ICCARM__)
  #pragma system_include
#endif
 
#if defined (__CC_ARM)
  #pragma anon_unions
#endif
/// @cond
/*
    If types are not defined elsewhere (CMSIS) define them here
*/
#ifndef __IO
#define __IO volatile
#endif
#ifndef __I
#define __I  volatile const
#endif
#ifndef __O
#define __O  volatile
#endif
#ifndef __R
#define __R  volatile const
#endif
/// @endcond

/* **** Definitions **** */

/**
 * @ingroup     rtc
 * @defgroup    rtc_registers RTC_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the RTC Peripheral Module.
 * @details Real Time Clock and Alarm.
 */

/**
 * @ingroup rtc_registers
 * Structure type to access the RTC Registers.
 */
typedef struct {
    __IO uint32_t sec;                  /**< <tt>\b 0x00:</tt> RTC SEC Register */
    __IO uint32_t ssec;                 /**< <tt>\b 0x04:</tt> RTC SSEC Register */
    __IO uint32_t toda;                 /**< <tt>\b 0x08:</tt> RTC TODA Register */
    __IO uint32_t sseca;                /**< <tt>\b 0x0C:</tt> RTC SSECA Register */
    __IO uint32_t ctrl;                 /**< <tt>\b 0x10:</tt> RTC CTRL Register */
    __IO uint32_t trim;                 /**< <tt>\b 0x14:</tt> RTC TRIM Register */
    __IO uint32_t oscctrl;              /**< <tt>\b 0x18:</tt> RTC OSCCTRL Register */
} mxc_rtc_reva_regs_t;

/* Register offsets for module RTC */
/**
 * @ingroup    rtc_registers
 * @defgroup   RTC_Register_Offsets Register Offsets
 * @brief      RTC Peripheral Register Offsets from the RTC Base Peripheral Address. 
 * @{
 */
 #define MXC_R_RTC_REVA_SEC                      ((uint32_t)0x00000000UL) /**< Offset from RTC Base Address: <tt> 0x0000</tt> */
 #define MXC_R_RTC_REVA_SSEC                     ((uint32_t)0x00000004UL) /**< Offset from RTC Base Address: <tt> 0x0004</tt> */
 #define MXC_R_RTC_REVA_TODA                     ((uint32_t)0x00000008UL) /**< Offset from RTC Base Address: <tt> 0x0008</tt> */
 #define MXC_R_RTC_REVA_SSECA                    ((uint32_t)0x0000000CUL) /**< Offset from RTC Base Address: <tt> 0x000C</tt> */
 #define MXC_R_RTC_REVA_CTRL                     ((uint32_t)0x00000010UL) /**< Offset from RTC Base Address: <tt> 0x0010</tt> */
 #define MXC_R_RTC_REVA_TRIM                     ((uint32_t)0x00000014UL) /**< Offset from RTC Base Address: <tt> 0x0014</tt> */
 #define MXC_R_RTC_REVA_OSCCTRL                  ((uint32_t)0x00000018UL) /**< Offset from RTC Base Address: <tt> 0x0018</tt> */
/**@} end of group rtc_registers */

/**
 * @ingroup  rtc_registers
 * @defgroup RTC_SEC RTC_SEC
 * @brief    RTC Second Counter. This register contains the 32-bit second counter.
 * @{
 */
 #define MXC_F_RTC_REVA_SEC_SEC_POS                          0 /**< SEC_SEC Position */
 #define MXC_F_RTC_REVA_SEC_SEC                              ((uint32_t)(0xFFUL << MXC_F_RTC_REVA_SEC_SEC_POS)) /**< SEC_SEC Mask */

/**@} end of group RTC_SEC_Register */

/**
 * @ingroup  rtc_registers
 * @defgroup RTC_SSEC RTC_SSEC
 * @brief    RTC Sub-second Counter. This counter increments at 256Hz. RTC_SEC is incremented
 *           when this register rolls over from 0xFF to 0x00.
 * @{
 */
 #define MXC_F_RTC_REVA_SSEC_SSEC_POS                        0 /**< SSEC_SSEC Position */
 #define MXC_F_RTC_REVA_SSEC_SSEC                            ((uint32_t)(0xFFUL << MXC_F_RTC_REVA_SSEC_SSEC_POS)) /**< SSEC_SSEC Mask */

/**@} end of group RTC_SSEC_Register */

/**
 * @ingroup  rtc_registers
 * @defgroup RTC_TODA RTC_TODA
 * @brief    Time-of-day Alarm.
 * @{
 */
 #define MXC_F_RTC_REVA_TODA_TOD_ALARM_POS                   0 /**< TODA_TOD_ALARM Position */
 #define MXC_F_RTC_REVA_TODA_TOD_ALARM                       ((uint32_t)(0xFFFFFUL << MXC_F_RTC_REVA_TODA_TOD_ALARM_POS)) /**< TODA_TOD_ALARM Mask */

/**@} end of group RTC_TODA_Register */

/**
 * @ingroup  rtc_registers
 * @defgroup RTC_SSECA RTC_SSECA
 * @brief    RTC sub-second alarm.  This register contains the reload value for the sub-
 *           second alarm.
 * @{
 */
 #define MXC_F_RTC_REVA_SSECA_SSEC_ALARM_POS                 0 /**< SSECA_SSEC_ALARM Position */
 #define MXC_F_RTC_REVA_SSECA_SSEC_ALARM                     ((uint32_t)(0xFFFFFFFFUL << MXC_F_RTC_REVA_SSECA_SSEC_ALARM_POS)) /**< SSECA_SSEC_ALARM Mask */

/**@} end of group RTC_SSECA_Register */

/**
 * @ingroup  rtc_registers
 * @defgroup RTC_CTRL RTC_CTRL
 * @brief    RTC Control Register.
 * @{
 */
 #define MXC_F_RTC_REVA_CTRL_EN_POS                          0 /**< CTRL_EN Position */
 #define MXC_F_RTC_REVA_CTRL_EN                              ((uint32_t)(0x1UL << MXC_F_RTC_REVA_CTRL_EN_POS)) /**< CTRL_EN Mask */

 #define MXC_F_RTC_REVA_CTRL_TOD_ALARM_IE_POS                1 /**< CTRL_TOD_ALARM_IE Position */
 #define MXC_F_RTC_REVA_CTRL_TOD_ALARM_IE                    ((uint32_t)(0x1UL << MXC_F_RTC_REVA_CTRL_TOD_ALARM_IE_POS)) /**< CTRL_TOD_ALARM_IE Mask */

 #define MXC_F_RTC_REVA_CTRL_SSEC_ALARM_IE_POS               2 /**< CTRL_SSEC_ALARM_IE Position */
 #define MXC_F_RTC_REVA_CTRL_SSEC_ALARM_IE                   ((uint32_t)(0x1UL << MXC_F_RTC_REVA_CTRL_SSEC_ALARM_IE_POS)) /**< CTRL_SSEC_ALARM_IE Mask */

 #define MXC_F_RTC_REVA_CTRL_BUSY_POS                        3 /**< CTRL_BUSY Position */
 #define MXC_F_RTC_REVA_CTRL_BUSY                            ((uint32_t)(0x1UL << MXC_F_RTC_REVA_CTRL_BUSY_POS)) /**< CTRL_BUSY Mask */

 #define MXC_F_RTC_REVA_CTRL_RDY_POS                         4 /**< CTRL_RDY Position */
 #define MXC_F_RTC_REVA_CTRL_RDY                             ((uint32_t)(0x1UL << MXC_F_RTC_REVA_CTRL_RDY_POS)) /**< CTRL_RDY Mask */

 #define MXC_F_RTC_REVA_CTRL_RDY_IE_POS                      5 /**< CTRL_RDY_IE Position */
 #define MXC_F_RTC_REVA_CTRL_RDY_IE                          ((uint32_t)(0x1UL << MXC_F_RTC_REVA_CTRL_RDY_IE_POS)) /**< CTRL_RDY_IE Mask */

 #define MXC_F_RTC_REVA_CTRL_TOD_ALARM_POS                   6 /**< CTRL_TOD_ALARM Position */
 #define MXC_F_RTC_REVA_CTRL_TOD_ALARM                       ((uint32_t)(0x1UL << MXC_F_RTC_REVA_CTRL_TOD_ALARM_POS)) /**< CTRL_TOD_ALARM Mask */

 #define MXC_F_RTC_REVA_CTRL_SSEC_ALARM_POS                  7 /**< CTRL_SSEC_ALARM Position */
 #define MXC_F_RTC_REVA_CTRL_SSEC_ALARM                      ((uint32_t)(0x1UL << MXC_F_RTC_REVA_CTRL_SSEC_ALARM_POS)) /**< CTRL_SSEC_ALARM Mask */

 #define MXC_F_RTC_REVA_CTRL_SQW_EN_POS                      8 /**< CTRL_SQW_EN Position */
 #define MXC_F_RTC_REVA_CTRL_SQW_EN                          ((uint32_t)(0x1UL << MXC_F_RTC_REVA_CTRL_SQW_EN_POS)) /**< CTRL_SQW_EN Mask */

 #define MXC_F_RTC_REVA_CTRL_SQW_SEL_POS                     9 /**< CTRL_SQW_SEL Position */
 #define MXC_F_RTC_REVA_CTRL_SQW_SEL                         ((uint32_t)(0x3UL << MXC_F_RTC_REVA_CTRL_SQW_SEL_POS)) /**< CTRL_SQW_SEL Mask */
 #define MXC_V_RTC_REVA_CTRL_SQW_SEL_FREQ1HZ                 ((uint32_t)0x0UL) /**< CTRL_SQW_SEL_FREQ1HZ Value */
 #define MXC_S_RTC_REVA_CTRL_SQW_SEL_FREQ1HZ                 (MXC_V_RTC_REVA_CTRL_SQW_SEL_FREQ1HZ << MXC_F_RTC_REVA_CTRL_SQW_SEL_POS) /**< CTRL_SQW_SEL_FREQ1HZ Setting */
 #define MXC_V_RTC_REVA_CTRL_SQW_SEL_FREQ512HZ               ((uint32_t)0x1UL) /**< CTRL_SQW_SEL_FREQ512HZ Value */
 #define MXC_S_RTC_REVA_CTRL_SQW_SEL_FREQ512HZ               (MXC_V_RTC_REVA_CTRL_SQW_SEL_FREQ512HZ << MXC_F_RTC_REVA_CTRL_SQW_SEL_POS) /**< CTRL_SQW_SEL_FREQ512HZ Setting */
 #define MXC_V_RTC_REVA_CTRL_SQW_SEL_FREQ4KHZ                ((uint32_t)0x2UL) /**< CTRL_SQW_SEL_FREQ4KHZ Value */
 #define MXC_S_RTC_REVA_CTRL_SQW_SEL_FREQ4KHZ                (MXC_V_RTC_REVA_CTRL_SQW_SEL_FREQ4KHZ << MXC_F_RTC_REVA_CTRL_SQW_SEL_POS) /**< CTRL_SQW_SEL_FREQ4KHZ Setting */
 #define MXC_V_RTC_REVA_CTRL_SQW_SEL_CLKDIV8                 ((uint32_t)0x3UL) /**< CTRL_SQW_SEL_CLKDIV8 Value */
 #define MXC_S_RTC_REVA_CTRL_SQW_SEL_CLKDIV8                 (MXC_V_RTC_REVA_CTRL_SQW_SEL_CLKDIV8 << MXC_F_RTC_REVA_CTRL_SQW_SEL_POS) /**< CTRL_SQW_SEL_CLKDIV8 Setting */

 #define MXC_F_RTC_REVA_CTRL_RD_EN_POS                       14 /**< CTRL_RD_EN Position */
 #define MXC_F_RTC_REVA_CTRL_RD_EN                           ((uint32_t)(0x1UL << MXC_F_RTC_REVA_CTRL_RD_EN_POS)) /**< CTRL_RD_EN Mask */

 #define MXC_F_RTC_REVA_CTRL_WR_EN_POS                       15 /**< CTRL_WR_EN Position */
 #define MXC_F_RTC_REVA_CTRL_WR_EN                           ((uint32_t)(0x1UL << MXC_F_RTC_REVA_CTRL_WR_EN_POS)) /**< CTRL_WR_EN Mask */

/**@} end of group RTC_CTRL_Register */

/**
 * @ingroup  rtc_registers
 * @defgroup RTC_TRIM RTC_TRIM
 * @brief    RTC Trim Register.
 * @{
 */
 #define MXC_F_RTC_REVA_TRIM_TRIM_POS                        0 /**< TRIM_TRIM Position */
 #define MXC_F_RTC_REVA_TRIM_TRIM                            ((uint32_t)(0xFFUL << MXC_F_RTC_REVA_TRIM_TRIM_POS)) /**< TRIM_TRIM Mask */

 #define MXC_F_RTC_REVA_TRIM_VRTC_TMR_POS                    8 /**< TRIM_VRTC_TMR Position */
 #define MXC_F_RTC_REVA_TRIM_VRTC_TMR                        ((uint32_t)(0xFFFFFFUL << MXC_F_RTC_REVA_TRIM_VRTC_TMR_POS)) /**< TRIM_VRTC_TMR Mask */

/**@} end of group RTC_TRIM_Register */

/**
 * @ingroup  rtc_registers
 * @defgroup RTC_OSCCTRL RTC_OSCCTRL
 * @brief    RTC Oscillator Control Register.
 * @{
 */
 #define MXC_F_RTC_REVA_OSCCTRL_BYPASS_POS                   4 /**< OSCCTRL_BYPASS Position */
 #define MXC_F_RTC_REVA_OSCCTRL_BYPASS                       ((uint32_t)(0x1UL << MXC_F_RTC_REVA_OSCCTRL_BYPASS_POS)) /**< OSCCTRL_BYPASS Mask */

 #define MXC_F_RTC_REVA_OSCCTRL_SQW_32K_POS                  5 /**< OSCCTRL_SQW_32K Position */
 #define MXC_F_RTC_REVA_OSCCTRL_SQW_32K                      ((uint32_t)(0x1UL << MXC_F_RTC_REVA_OSCCTRL_SQW_32K_POS)) /**< OSCCTRL_SQW_32K Mask */

/**@} end of group RTC_OSCCTRL_Register */

#ifdef __cplusplus
}
#endif

#endif /* _RTC_REVA_REGS_H_ */
