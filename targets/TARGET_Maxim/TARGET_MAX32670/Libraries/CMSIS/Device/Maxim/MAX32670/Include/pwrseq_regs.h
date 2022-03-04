/**
 * @file    pwrseq_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the PWRSEQ Peripheral Module.
 */

/* ****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
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

#ifndef _PWRSEQ_REGS_H_
#define _PWRSEQ_REGS_H_

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
/// @endcond

/* **** Definitions **** */

/**
 * @ingroup     pwrseq
 * @defgroup    pwrseq_registers PWRSEQ_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the PWRSEQ Peripheral Module.
 * @details Power Sequencer / Low Power Control Register.
 */

/**
 * @ingroup pwrseq_registers
 * Structure type to access the PWRSEQ Registers.
 */
typedef struct {
    __IO uint32_t lpcn;                 /**< <tt>\b 0x00:</tt> PWRSEQ LPCN Register */
    __IO uint32_t lpwkst0;              /**< <tt>\b 0x04:</tt> PWRSEQ LPWKST0 Register */
    __IO uint32_t lpwken0;              /**< <tt>\b 0x08:</tt> PWRSEQ LPWKEN0 Register */
    __IO uint32_t lpwkst1;              /**< <tt>\b 0x0C:</tt> PWRSEQ LPWKST1 Register */
    __IO uint32_t lpwken1;              /**< <tt>\b 0x10:</tt> PWRSEQ LPWKEN1 Register */
    __I  uint32_t rsv_0x14_0x2f[7];
    __IO uint32_t lppwkst;              /**< <tt>\b 0x30:</tt> PWRSEQ LPPWKST Register */
    __IO uint32_t lppwken;              /**< <tt>\b 0x34:</tt> PWRSEQ LPPWKEN Register */
    __I  uint32_t rsv_0x38_0x3f[2];
    __IO uint32_t lpmemsd;              /**< <tt>\b 0x40:</tt> PWRSEQ LPMEMSD Register */
} mxc_pwrseq_regs_t;

/* Register offsets for module PWRSEQ */
/**
 * @ingroup    pwrseq_registers
 * @defgroup   PWRSEQ_Register_Offsets Register Offsets
 * @brief      PWRSEQ Peripheral Register Offsets from the PWRSEQ Base Peripheral Address. 
 * @{
 */
 #define MXC_R_PWRSEQ_LPCN                  ((uint32_t)0x00000000UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_PWRSEQ_LPWKST0               ((uint32_t)0x00000004UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_PWRSEQ_LPWKEN0               ((uint32_t)0x00000008UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_PWRSEQ_LPWKST1               ((uint32_t)0x0000000CUL) /**< Offset from PWRSEQ Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_PWRSEQ_LPWKEN1               ((uint32_t)0x00000010UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0010</tt> */ 
 #define MXC_R_PWRSEQ_LPPWKST               ((uint32_t)0x00000030UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0030</tt> */ 
 #define MXC_R_PWRSEQ_LPPWKEN               ((uint32_t)0x00000034UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0034</tt> */ 
 #define MXC_R_PWRSEQ_LPMEMSD               ((uint32_t)0x00000040UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0040</tt> */ 
/**@} end of group pwrseq_registers */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPCN PWRSEQ_LPCN
 * @brief    Low Power Control Register.
 * @{
 */
 #define MXC_F_PWRSEQ_LPCN_RAM0RET_EN_POS               0 /**< LPCN_RAM0RET_EN Position */
 #define MXC_F_PWRSEQ_LPCN_RAM0RET_EN                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_RAM0RET_EN_POS)) /**< LPCN_RAM0RET_EN Mask */

 #define MXC_F_PWRSEQ_LPCN_RAM1RET_EN_POS               1 /**< LPCN_RAM1RET_EN Position */
 #define MXC_F_PWRSEQ_LPCN_RAM1RET_EN                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_RAM1RET_EN_POS)) /**< LPCN_RAM1RET_EN Mask */

 #define MXC_F_PWRSEQ_LPCN_RAM2RET_EN_POS               2 /**< LPCN_RAM2RET_EN Position */
 #define MXC_F_PWRSEQ_LPCN_RAM2RET_EN                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_RAM2RET_EN_POS)) /**< LPCN_RAM2RET_EN Mask */

 #define MXC_F_PWRSEQ_LPCN_RAM3RET_EN_POS               3 /**< LPCN_RAM3RET_EN Position */
 #define MXC_F_PWRSEQ_LPCN_RAM3RET_EN                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_RAM3RET_EN_POS)) /**< LPCN_RAM3RET_EN Mask */

 #define MXC_F_PWRSEQ_LPCN_OVR_POS                      4 /**< LPCN_OVR Position */
 #define MXC_F_PWRSEQ_LPCN_OVR                          ((uint32_t)(0x3UL << MXC_F_PWRSEQ_LPCN_OVR_POS)) /**< LPCN_OVR Mask */
 #define MXC_V_PWRSEQ_LPCN_OVR_0_9V                     ((uint32_t)0x0UL) /**< LPCN_OVR_0_9V Value */
 #define MXC_S_PWRSEQ_LPCN_OVR_0_9V                     (MXC_V_PWRSEQ_LPCN_OVR_0_9V << MXC_F_PWRSEQ_LPCN_OVR_POS) /**< LPCN_OVR_0_9V Setting */
 #define MXC_V_PWRSEQ_LPCN_OVR_1_0V                     ((uint32_t)0x1UL) /**< LPCN_OVR_1_0V Value */
 #define MXC_S_PWRSEQ_LPCN_OVR_1_0V                     (MXC_V_PWRSEQ_LPCN_OVR_1_0V << MXC_F_PWRSEQ_LPCN_OVR_POS) /**< LPCN_OVR_1_0V Setting */
 #define MXC_V_PWRSEQ_LPCN_OVR_1_1V                     ((uint32_t)0x2UL) /**< LPCN_OVR_1_1V Value */
 #define MXC_S_PWRSEQ_LPCN_OVR_1_1V                     (MXC_V_PWRSEQ_LPCN_OVR_1_1V << MXC_F_PWRSEQ_LPCN_OVR_POS) /**< LPCN_OVR_1_1V Setting */

 #define MXC_F_PWRSEQ_LPCN_VCORE_DET_BYPASS_POS         6 /**< LPCN_VCORE_DET_BYPASS Position */
 #define MXC_F_PWRSEQ_LPCN_VCORE_DET_BYPASS             ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VCORE_DET_BYPASS_POS)) /**< LPCN_VCORE_DET_BYPASS Mask */

 #define MXC_F_PWRSEQ_LPCN_RETREG_EN_POS                8 /**< LPCN_RETREG_EN Position */
 #define MXC_F_PWRSEQ_LPCN_RETREG_EN                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_RETREG_EN_POS)) /**< LPCN_RETREG_EN Mask */

 #define MXC_F_PWRSEQ_LPCN_STORAGE_EN_POS               9 /**< LPCN_STORAGE_EN Position */
 #define MXC_F_PWRSEQ_LPCN_STORAGE_EN                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_STORAGE_EN_POS)) /**< LPCN_STORAGE_EN Mask */

 #define MXC_F_PWRSEQ_LPCN_FASTWK_EN_POS                10 /**< LPCN_FASTWK_EN Position */
 #define MXC_F_PWRSEQ_LPCN_FASTWK_EN                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_FASTWK_EN_POS)) /**< LPCN_FASTWK_EN Mask */

 #define MXC_F_PWRSEQ_LPCN_BG_DIS_POS                   11 /**< LPCN_BG_DIS Position */
 #define MXC_F_PWRSEQ_LPCN_BG_DIS                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_BG_DIS_POS)) /**< LPCN_BG_DIS Mask */

 #define MXC_F_PWRSEQ_LPCN_VCOREPOR_DIS_POS             12 /**< LPCN_VCOREPOR_DIS Position */
 #define MXC_F_PWRSEQ_LPCN_VCOREPOR_DIS                 ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VCOREPOR_DIS_POS)) /**< LPCN_VCOREPOR_DIS Mask */

 #define MXC_F_PWRSEQ_LPCN_LDO_DIS_POS                  16 /**< LPCN_LDO_DIS Position */
 #define MXC_F_PWRSEQ_LPCN_LDO_DIS                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_LDO_DIS_POS)) /**< LPCN_LDO_DIS Mask */

 #define MXC_F_PWRSEQ_LPCN_VCORE_EXT_POS                17 /**< LPCN_VCORE_EXT Position */
 #define MXC_F_PWRSEQ_LPCN_VCORE_EXT                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VCORE_EXT_POS)) /**< LPCN_VCORE_EXT Mask */

 #define MXC_F_PWRSEQ_LPCN_VCOREMON_DIS_POS             20 /**< LPCN_VCOREMON_DIS Position */
 #define MXC_F_PWRSEQ_LPCN_VCOREMON_DIS                 ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VCOREMON_DIS_POS)) /**< LPCN_VCOREMON_DIS Mask */

 #define MXC_F_PWRSEQ_LPCN_VDDAMON_DIS_POS              22 /**< LPCN_VDDAMON_DIS Position */
 #define MXC_F_PWRSEQ_LPCN_VDDAMON_DIS                  ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VDDAMON_DIS_POS)) /**< LPCN_VDDAMON_DIS Mask */

 #define MXC_F_PWRSEQ_LPCN_PORVDDMON_DIS_POS            25 /**< LPCN_PORVDDMON_DIS Position */
 #define MXC_F_PWRSEQ_LPCN_PORVDDMON_DIS                ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_PORVDDMON_DIS_POS)) /**< LPCN_PORVDDMON_DIS Mask */

 #define MXC_F_PWRSEQ_LPCN_INRO_EN_POS                  28 /**< LPCN_INRO_EN Position */
 #define MXC_F_PWRSEQ_LPCN_INRO_EN                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_INRO_EN_POS)) /**< LPCN_INRO_EN Mask */

 #define MXC_F_PWRSEQ_LPCN_ERTCO_EN_POS                 29 /**< LPCN_ERTCO_EN Position */
 #define MXC_F_PWRSEQ_LPCN_ERTCO_EN                     ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_ERTCO_EN_POS)) /**< LPCN_ERTCO_EN Mask */

 #define MXC_F_PWRSEQ_LPCN_TM_LPMODE_POS                30 /**< LPCN_TM_LPMODE Position */
 #define MXC_F_PWRSEQ_LPCN_TM_LPMODE                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_TM_LPMODE_POS)) /**< LPCN_TM_LPMODE Mask */

 #define MXC_F_PWRSEQ_LPCN_TM_PWRSEQ_POS                31 /**< LPCN_TM_PWRSEQ Position */
 #define MXC_F_PWRSEQ_LPCN_TM_PWRSEQ                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_TM_PWRSEQ_POS)) /**< LPCN_TM_PWRSEQ Mask */

/**@} end of group PWRSEQ_LPCN_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPWKST0 PWRSEQ_LPWKST0
 * @brief    Low Power I/O Wakeup Status Register 0. This register indicates the low power
 *           wakeup status for GPIO0.
 * @{
 */
 #define MXC_F_PWRSEQ_LPWKST0_ST_POS                    0 /**< LPWKST0_ST Position */
 #define MXC_F_PWRSEQ_LPWKST0_ST                        ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPWKST0_ST_POS)) /**< LPWKST0_ST Mask */

/**@} end of group PWRSEQ_LPWKST0_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPWKEN0 PWRSEQ_LPWKEN0
 * @brief    Low Power I/O Wakeup Enable Register 0. This register enables low power wakeup
 *           functionality for GPIO0.
 * @{
 */
 #define MXC_F_PWRSEQ_LPWKEN0_EN_POS                    0 /**< LPWKEN0_EN Position */
 #define MXC_F_PWRSEQ_LPWKEN0_EN                        ((uint32_t)(0x7FFFFFFFUL << MXC_F_PWRSEQ_LPWKEN0_EN_POS)) /**< LPWKEN0_EN Mask */

/**@} end of group PWRSEQ_LPWKEN0_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPPWKST PWRSEQ_LPPWKST
 * @brief    Low Power Peripheral Wakeup Status Register.
 * @{
 */
 #define MXC_F_PWRSEQ_LPPWKST_LPTMR0_POS                0 /**< LPPWKST_LPTMR0 Position */
 #define MXC_F_PWRSEQ_LPPWKST_LPTMR0                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWKST_LPTMR0_POS)) /**< LPPWKST_LPTMR0 Mask */

 #define MXC_F_PWRSEQ_LPPWKST_LPTMR1_POS                1 /**< LPPWKST_LPTMR1 Position */
 #define MXC_F_PWRSEQ_LPPWKST_LPTMR1                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWKST_LPTMR1_POS)) /**< LPPWKST_LPTMR1 Mask */

 #define MXC_F_PWRSEQ_LPPWKST_LPUART0_POS               2 /**< LPPWKST_LPUART0 Position */
 #define MXC_F_PWRSEQ_LPPWKST_LPUART0                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWKST_LPUART0_POS)) /**< LPPWKST_LPUART0 Mask */

/**@} end of group PWRSEQ_LPPWKST_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPPWKEN PWRSEQ_LPPWKEN
 * @brief    Low Power Peripheral Wakeup Enable Register.
 * @{
 */
 #define MXC_F_PWRSEQ_LPPWKEN_LPTMR0_POS                0 /**< LPPWKEN_LPTMR0 Position */
 #define MXC_F_PWRSEQ_LPPWKEN_LPTMR0                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWKEN_LPTMR0_POS)) /**< LPPWKEN_LPTMR0 Mask */

 #define MXC_F_PWRSEQ_LPPWKEN_LPTMR1_POS                1 /**< LPPWKEN_LPTMR1 Position */
 #define MXC_F_PWRSEQ_LPPWKEN_LPTMR1                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWKEN_LPTMR1_POS)) /**< LPPWKEN_LPTMR1 Mask */

 #define MXC_F_PWRSEQ_LPPWKEN_LPUART0_POS               2 /**< LPPWKEN_LPUART0 Position */
 #define MXC_F_PWRSEQ_LPPWKEN_LPUART0                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWKEN_LPUART0_POS)) /**< LPPWKEN_LPUART0 Mask */

/**@} end of group PWRSEQ_LPPWKEN_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPMEMSD PWRSEQ_LPMEMSD
 * @brief    Low Power Memory Shutdown Control.
 * @{
 */
 #define MXC_F_PWRSEQ_LPMEMSD_RAM0_POS                  0 /**< LPMEMSD_RAM0 Position */
 #define MXC_F_PWRSEQ_LPMEMSD_RAM0                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_RAM0_POS)) /**< LPMEMSD_RAM0 Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_RAM1_POS                  1 /**< LPMEMSD_RAM1 Position */
 #define MXC_F_PWRSEQ_LPMEMSD_RAM1                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_RAM1_POS)) /**< LPMEMSD_RAM1 Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_RAM2_POS                  2 /**< LPMEMSD_RAM2 Position */
 #define MXC_F_PWRSEQ_LPMEMSD_RAM2                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_RAM2_POS)) /**< LPMEMSD_RAM2 Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_RAM3_POS                  3 /**< LPMEMSD_RAM3 Position */
 #define MXC_F_PWRSEQ_LPMEMSD_RAM3                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_RAM3_POS)) /**< LPMEMSD_RAM3 Mask */

/**@} end of group PWRSEQ_LPMEMSD_Register */

#ifdef __cplusplus
}
#endif

#endif /* _PWRSEQ_REGS_H_ */
