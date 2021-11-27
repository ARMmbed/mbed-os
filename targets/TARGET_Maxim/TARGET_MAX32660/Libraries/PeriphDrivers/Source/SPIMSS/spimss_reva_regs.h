/**
 * @file    spimss_reva_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the SPIMSS Peripheral Module.
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

#ifndef _SPIMSS_REVA_REGS_H_
#define _SPIMSS_REVA_REGS_H_

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
 * @ingroup     spimss
 * @defgroup    spimss_registers SPIMSS_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the SPIMSS Peripheral Module.
 * @details Serial Peripheral Interface.
 */

/**
 * @ingroup spimss_registers
 * Structure type to access the SPIMSS Registers.
 */
typedef struct {
    __IO uint16_t data;                 /**< <tt>\b 0x00:</tt> SPIMSS DATA Register */
    __R  uint16_t rsv_0x2;
    __IO uint32_t ctrl;                 /**< <tt>\b 0x04:</tt> SPIMSS CTRL Register */
    __IO uint32_t int_fl;               /**< <tt>\b 0x08:</tt> SPIMSS INT_FL Register */
    __IO uint32_t mode;                 /**< <tt>\b 0x0C:</tt> SPIMSS MODE Register */
    __R  uint32_t rsv_0x10;
    __IO uint32_t brg;                  /**< <tt>\b 0x14:</tt> SPIMSS BRG Register */
    __IO uint32_t dma;                  /**< <tt>\b 0x18:</tt> SPIMSS DMA Register */
    __IO uint32_t i2s_ctrl;             /**< <tt>\b 0x1C:</tt> SPIMSS I2S_CTRL Register */
} mxc_spimss_reva_regs_t;

/* Register offsets for module SPIMSS */
/**
 * @ingroup    spimss_registers
 * @defgroup   SPIMSS_Register_Offsets Register Offsets
 * @brief      SPIMSS Peripheral Register Offsets from the SPIMSS Base Peripheral Address. 
 * @{
 */
 #define MXC_R_SPIMSS_REVA_DATA                  ((uint32_t)0x00000000UL) /**< Offset from SPIMSS Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_SPIMSS_REVA_CTRL                  ((uint32_t)0x00000004UL) /**< Offset from SPIMSS Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_SPIMSS_REVA_INT_FL                ((uint32_t)0x00000008UL) /**< Offset from SPIMSS Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_SPIMSS_REVA_MODE                  ((uint32_t)0x0000000CUL) /**< Offset from SPIMSS Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_SPIMSS_REVA_BRG                   ((uint32_t)0x00000014UL) /**< Offset from SPIMSS Base Address: <tt> 0x0014</tt> */ 
 #define MXC_R_SPIMSS_REVA_DMA                   ((uint32_t)0x00000018UL) /**< Offset from SPIMSS Base Address: <tt> 0x0018</tt> */ 
 #define MXC_R_SPIMSS_REVA_I2S_CTRL              ((uint32_t)0x0000001CUL) /**< Offset from SPIMSS Base Address: <tt> 0x001C</tt> */ 
/**@} end of group spimss_registers */

/**
 * @ingroup  spimss_registers
 * @defgroup SPIMSS_DATA SPIMSS_DATA
 * @brief    SPI 16-bit Data Access
 * @{
 */
 #define MXC_F_SPIMSS_REVA_DATA_DATA_POS                     0 /**< DATA_DATA Position */
 #define MXC_F_SPIMSS_REVA_DATA_DATA                         ((uint16_t)(0xFFFFUL << MXC_F_SPIMSS_REVA_DATA_DATA_POS)) /**< DATA_DATA Mask */

/**@} end of group SPIMSS_DATA_Register */

/**
 * @ingroup  spimss_registers
 * @defgroup SPIMSS_CTRL SPIMSS_CTRL
 * @brief    SPI Control Register.
 * @{
 */
 #define MXC_F_SPIMSS_REVA_CTRL_ENABLE_POS                   0 /**< CTRL_ENABLE Position */
 #define MXC_F_SPIMSS_REVA_CTRL_ENABLE                       ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_CTRL_ENABLE_POS)) /**< CTRL_ENABLE Mask */
 #define MXC_V_SPIMSS_REVA_CTRL_ENABLE_DISABLE               ((uint32_t)0x0UL) /**< CTRL_ENABLE_DISABLE Value */
 #define MXC_S_SPIMSS_REVA_CTRL_ENABLE_DISABLE               (MXC_V_SPIMSS_REVA_CTRL_ENABLE_DISABLE << MXC_F_SPIMSS_REVA_CTRL_ENABLE_POS) /**< CTRL_ENABLE_DISABLE Setting */
 #define MXC_V_SPIMSS_REVA_CTRL_ENABLE_ENABLE                ((uint32_t)0x1UL) /**< CTRL_ENABLE_ENABLE Value */
 #define MXC_S_SPIMSS_REVA_CTRL_ENABLE_ENABLE                (MXC_V_SPIMSS_REVA_CTRL_ENABLE_ENABLE << MXC_F_SPIMSS_REVA_CTRL_ENABLE_POS) /**< CTRL_ENABLE_ENABLE Setting */

 #define MXC_F_SPIMSS_REVA_CTRL_MMEN_POS                     1 /**< CTRL_MMEN Position */
 #define MXC_F_SPIMSS_REVA_CTRL_MMEN                         ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_CTRL_MMEN_POS)) /**< CTRL_MMEN Mask */
 #define MXC_V_SPIMSS_REVA_CTRL_MMEN_SLAVE                   ((uint32_t)0x0UL) /**< CTRL_MMEN_SLAVE Value */
 #define MXC_S_SPIMSS_REVA_CTRL_MMEN_SLAVE                   (MXC_V_SPIMSS_REVA_CTRL_MMEN_SLAVE << MXC_F_SPIMSS_REVA_CTRL_MMEN_POS) /**< CTRL_MMEN_SLAVE Setting */
 #define MXC_V_SPIMSS_REVA_CTRL_MMEN_MASTER                  ((uint32_t)0x1UL) /**< CTRL_MMEN_MASTER Value */
 #define MXC_S_SPIMSS_REVA_CTRL_MMEN_MASTER                  (MXC_V_SPIMSS_REVA_CTRL_MMEN_MASTER << MXC_F_SPIMSS_REVA_CTRL_MMEN_POS) /**< CTRL_MMEN_MASTER Setting */

 #define MXC_F_SPIMSS_REVA_CTRL_WOR_POS                      2 /**< CTRL_WOR Position */
 #define MXC_F_SPIMSS_REVA_CTRL_WOR                          ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_CTRL_WOR_POS)) /**< CTRL_WOR Mask */
 #define MXC_V_SPIMSS_REVA_CTRL_WOR_DISABLE                  ((uint32_t)0x0UL) /**< CTRL_WOR_DISABLE Value */
 #define MXC_S_SPIMSS_REVA_CTRL_WOR_DISABLE                  (MXC_V_SPIMSS_REVA_CTRL_WOR_DISABLE << MXC_F_SPIMSS_REVA_CTRL_WOR_POS) /**< CTRL_WOR_DISABLE Setting */
 #define MXC_V_SPIMSS_REVA_CTRL_WOR_ENABLE                   ((uint32_t)0x1UL) /**< CTRL_WOR_ENABLE Value */
 #define MXC_S_SPIMSS_REVA_CTRL_WOR_ENABLE                   (MXC_V_SPIMSS_REVA_CTRL_WOR_ENABLE << MXC_F_SPIMSS_REVA_CTRL_WOR_POS) /**< CTRL_WOR_ENABLE Setting */

 #define MXC_F_SPIMSS_REVA_CTRL_CLKPOL_POS                   3 /**< CTRL_CLKPOL Position */
 #define MXC_F_SPIMSS_REVA_CTRL_CLKPOL                       ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_CTRL_CLKPOL_POS)) /**< CTRL_CLKPOL Mask */
 #define MXC_V_SPIMSS_REVA_CTRL_CLKPOL_IDLELO                ((uint32_t)0x0UL) /**< CTRL_CLKPOL_IDLELO Value */
 #define MXC_S_SPIMSS_REVA_CTRL_CLKPOL_IDLELO                (MXC_V_SPIMSS_REVA_CTRL_CLKPOL_IDLELO << MXC_F_SPIMSS_REVA_CTRL_CLKPOL_POS) /**< CTRL_CLKPOL_IDLELO Setting */
 #define MXC_V_SPIMSS_REVA_CTRL_CLKPOL_IDLEHI                ((uint32_t)0x1UL) /**< CTRL_CLKPOL_IDLEHI Value */
 #define MXC_S_SPIMSS_REVA_CTRL_CLKPOL_IDLEHI                (MXC_V_SPIMSS_REVA_CTRL_CLKPOL_IDLEHI << MXC_F_SPIMSS_REVA_CTRL_CLKPOL_POS) /**< CTRL_CLKPOL_IDLEHI Setting */

 #define MXC_F_SPIMSS_REVA_CTRL_PHASE_POS                    4 /**< CTRL_PHASE Position */
 #define MXC_F_SPIMSS_REVA_CTRL_PHASE                        ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_CTRL_PHASE_POS)) /**< CTRL_PHASE Mask */
 #define MXC_V_SPIMSS_REVA_CTRL_PHASE_ACTIVEEDGE             ((uint32_t)0x0UL) /**< CTRL_PHASE_ACTIVEEDGE Value */
 #define MXC_S_SPIMSS_REVA_CTRL_PHASE_ACTIVEEDGE             (MXC_V_SPIMSS_REVA_CTRL_PHASE_ACTIVEEDGE << MXC_F_SPIMSS_REVA_CTRL_PHASE_POS) /**< CTRL_PHASE_ACTIVEEDGE Setting */
 #define MXC_V_SPIMSS_REVA_CTRL_PHASE_INACTIVEEDGE           ((uint32_t)0x1UL) /**< CTRL_PHASE_INACTIVEEDGE Value */
 #define MXC_S_SPIMSS_REVA_CTRL_PHASE_INACTIVEEDGE           (MXC_V_SPIMSS_REVA_CTRL_PHASE_INACTIVEEDGE << MXC_F_SPIMSS_REVA_CTRL_PHASE_POS) /**< CTRL_PHASE_INACTIVEEDGE Setting */

 #define MXC_F_SPIMSS_REVA_CTRL_BIRQ_POS                     5 /**< CTRL_BIRQ Position */
 #define MXC_F_SPIMSS_REVA_CTRL_BIRQ                         ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_CTRL_BIRQ_POS)) /**< CTRL_BIRQ Mask */
 #define MXC_V_SPIMSS_REVA_CTRL_BIRQ_DISABLE                 ((uint32_t)0x0UL) /**< CTRL_BIRQ_DISABLE Value */
 #define MXC_S_SPIMSS_REVA_CTRL_BIRQ_DISABLE                 (MXC_V_SPIMSS_REVA_CTRL_BIRQ_DISABLE << MXC_F_SPIMSS_REVA_CTRL_BIRQ_POS) /**< CTRL_BIRQ_DISABLE Setting */
 #define MXC_V_SPIMSS_REVA_CTRL_BIRQ_ENABLE                  ((uint32_t)0x1UL) /**< CTRL_BIRQ_ENABLE Value */
 #define MXC_S_SPIMSS_REVA_CTRL_BIRQ_ENABLE                  (MXC_V_SPIMSS_REVA_CTRL_BIRQ_ENABLE << MXC_F_SPIMSS_REVA_CTRL_BIRQ_POS) /**< CTRL_BIRQ_ENABLE Setting */

 #define MXC_F_SPIMSS_REVA_CTRL_STR_POS                      6 /**< CTRL_STR Position */
 #define MXC_F_SPIMSS_REVA_CTRL_STR                          ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_CTRL_STR_POS)) /**< CTRL_STR Mask */
 #define MXC_V_SPIMSS_REVA_CTRL_STR_COMPLETE                 ((uint32_t)0x0UL) /**< CTRL_STR_COMPLETE Value */
 #define MXC_S_SPIMSS_REVA_CTRL_STR_COMPLETE                 (MXC_V_SPIMSS_REVA_CTRL_STR_COMPLETE << MXC_F_SPIMSS_REVA_CTRL_STR_POS) /**< CTRL_STR_COMPLETE Setting */
 #define MXC_V_SPIMSS_REVA_CTRL_STR_START                    ((uint32_t)0x1UL) /**< CTRL_STR_START Value */
 #define MXC_S_SPIMSS_REVA_CTRL_STR_START                    (MXC_V_SPIMSS_REVA_CTRL_STR_START << MXC_F_SPIMSS_REVA_CTRL_STR_POS) /**< CTRL_STR_START Setting */

 #define MXC_F_SPIMSS_REVA_CTRL_IRQE_POS                     7 /**< CTRL_IRQE Position */
 #define MXC_F_SPIMSS_REVA_CTRL_IRQE                         ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_CTRL_IRQE_POS)) /**< CTRL_IRQE Mask */
 #define MXC_V_SPIMSS_REVA_CTRL_IRQE_DISABLE                 ((uint32_t)0x0UL) /**< CTRL_IRQE_DISABLE Value */
 #define MXC_S_SPIMSS_REVA_CTRL_IRQE_DISABLE                 (MXC_V_SPIMSS_REVA_CTRL_IRQE_DISABLE << MXC_F_SPIMSS_REVA_CTRL_IRQE_POS) /**< CTRL_IRQE_DISABLE Setting */
 #define MXC_V_SPIMSS_REVA_CTRL_IRQE_ENABLE                  ((uint32_t)0x1UL) /**< CTRL_IRQE_ENABLE Value */
 #define MXC_S_SPIMSS_REVA_CTRL_IRQE_ENABLE                  (MXC_V_SPIMSS_REVA_CTRL_IRQE_ENABLE << MXC_F_SPIMSS_REVA_CTRL_IRQE_POS) /**< CTRL_IRQE_ENABLE Setting */

/**@} end of group SPIMSS_CTRL_Register */

/**
 * @ingroup  spimss_registers
 * @defgroup SPIMSS_INT_FL SPIMSS_INT_FL
 * @brief    SPI Interrupt Flag Register.
 * @{
 */
 #define MXC_F_SPIMSS_REVA_INT_FL_SLAS_POS                   0 /**< INT_FL_SLAS Position */
 #define MXC_F_SPIMSS_REVA_INT_FL_SLAS                       ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_INT_FL_SLAS_POS)) /**< INT_FL_SLAS Mask */
 #define MXC_V_SPIMSS_REVA_INT_FL_SLAS_SELECTED              ((uint32_t)0x0UL) /**< INT_FL_SLAS_SELECTED Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_SLAS_SELECTED              (MXC_V_SPIMSS_REVA_INT_FL_SLAS_SELECTED << MXC_F_SPIMSS_REVA_INT_FL_SLAS_POS) /**< INT_FL_SLAS_SELECTED Setting */
 #define MXC_V_SPIMSS_REVA_INT_FL_SLAS_NOTSELECTED           ((uint32_t)0x1UL) /**< INT_FL_SLAS_NOTSELECTED Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_SLAS_NOTSELECTED           (MXC_V_SPIMSS_REVA_INT_FL_SLAS_NOTSELECTED << MXC_F_SPIMSS_REVA_INT_FL_SLAS_POS) /**< INT_FL_SLAS_NOTSELECTED Setting */

 #define MXC_F_SPIMSS_REVA_INT_FL_TXST_POS                   1 /**< INT_FL_TXST Position */
 #define MXC_F_SPIMSS_REVA_INT_FL_TXST                       ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_INT_FL_TXST_POS)) /**< INT_FL_TXST Mask */
 #define MXC_V_SPIMSS_REVA_INT_FL_TXST_IDLE                  ((uint32_t)0x0UL) /**< INT_FL_TXST_IDLE Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_TXST_IDLE                  (MXC_V_SPIMSS_REVA_INT_FL_TXST_IDLE << MXC_F_SPIMSS_REVA_INT_FL_TXST_POS) /**< INT_FL_TXST_IDLE Setting */
 #define MXC_V_SPIMSS_REVA_INT_FL_TXST_BUSY                  ((uint32_t)0x1UL) /**< INT_FL_TXST_BUSY Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_TXST_BUSY                  (MXC_V_SPIMSS_REVA_INT_FL_TXST_BUSY << MXC_F_SPIMSS_REVA_INT_FL_TXST_POS) /**< INT_FL_TXST_BUSY Setting */

 #define MXC_F_SPIMSS_REVA_INT_FL_TUND_POS                   2 /**< INT_FL_TUND Position */
 #define MXC_F_SPIMSS_REVA_INT_FL_TUND                       ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_INT_FL_TUND_POS)) /**< INT_FL_TUND Mask */
 #define MXC_V_SPIMSS_REVA_INT_FL_TUND_NOEVENT               ((uint32_t)0x0UL) /**< INT_FL_TUND_NOEVENT Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_TUND_NOEVENT               (MXC_V_SPIMSS_REVA_INT_FL_TUND_NOEVENT << MXC_F_SPIMSS_REVA_INT_FL_TUND_POS) /**< INT_FL_TUND_NOEVENT Setting */
 #define MXC_V_SPIMSS_REVA_INT_FL_TUND_OCCURRED              ((uint32_t)0x1UL) /**< INT_FL_TUND_OCCURRED Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_TUND_OCCURRED              (MXC_V_SPIMSS_REVA_INT_FL_TUND_OCCURRED << MXC_F_SPIMSS_REVA_INT_FL_TUND_POS) /**< INT_FL_TUND_OCCURRED Setting */

 #define MXC_F_SPIMSS_REVA_INT_FL_ROVR_POS                   3 /**< INT_FL_ROVR Position */
 #define MXC_F_SPIMSS_REVA_INT_FL_ROVR                       ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_INT_FL_ROVR_POS)) /**< INT_FL_ROVR Mask */
 #define MXC_V_SPIMSS_REVA_INT_FL_ROVR_NOEVENT               ((uint32_t)0x0UL) /**< INT_FL_ROVR_NOEVENT Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_ROVR_NOEVENT               (MXC_V_SPIMSS_REVA_INT_FL_ROVR_NOEVENT << MXC_F_SPIMSS_REVA_INT_FL_ROVR_POS) /**< INT_FL_ROVR_NOEVENT Setting */
 #define MXC_V_SPIMSS_REVA_INT_FL_ROVR_OCCURRED              ((uint32_t)0x1UL) /**< INT_FL_ROVR_OCCURRED Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_ROVR_OCCURRED              (MXC_V_SPIMSS_REVA_INT_FL_ROVR_OCCURRED << MXC_F_SPIMSS_REVA_INT_FL_ROVR_POS) /**< INT_FL_ROVR_OCCURRED Setting */

 #define MXC_F_SPIMSS_REVA_INT_FL_ABT_POS                    4 /**< INT_FL_ABT Position */
 #define MXC_F_SPIMSS_REVA_INT_FL_ABT                        ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_INT_FL_ABT_POS)) /**< INT_FL_ABT Mask */
 #define MXC_V_SPIMSS_REVA_INT_FL_ABT_NOEVENT                ((uint32_t)0x0UL) /**< INT_FL_ABT_NOEVENT Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_ABT_NOEVENT                (MXC_V_SPIMSS_REVA_INT_FL_ABT_NOEVENT << MXC_F_SPIMSS_REVA_INT_FL_ABT_POS) /**< INT_FL_ABT_NOEVENT Setting */
 #define MXC_V_SPIMSS_REVA_INT_FL_ABT_OCCURRED               ((uint32_t)0x1UL) /**< INT_FL_ABT_OCCURRED Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_ABT_OCCURRED               (MXC_V_SPIMSS_REVA_INT_FL_ABT_OCCURRED << MXC_F_SPIMSS_REVA_INT_FL_ABT_POS) /**< INT_FL_ABT_OCCURRED Setting */

 #define MXC_F_SPIMSS_REVA_INT_FL_COL_POS                    5 /**< INT_FL_COL Position */
 #define MXC_F_SPIMSS_REVA_INT_FL_COL                        ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_INT_FL_COL_POS)) /**< INT_FL_COL Mask */
 #define MXC_V_SPIMSS_REVA_INT_FL_COL_NOEVENT                ((uint32_t)0x0UL) /**< INT_FL_COL_NOEVENT Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_COL_NOEVENT                (MXC_V_SPIMSS_REVA_INT_FL_COL_NOEVENT << MXC_F_SPIMSS_REVA_INT_FL_COL_POS) /**< INT_FL_COL_NOEVENT Setting */
 #define MXC_V_SPIMSS_REVA_INT_FL_COL_OCCURRED               ((uint32_t)0x1UL) /**< INT_FL_COL_OCCURRED Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_COL_OCCURRED               (MXC_V_SPIMSS_REVA_INT_FL_COL_OCCURRED << MXC_F_SPIMSS_REVA_INT_FL_COL_POS) /**< INT_FL_COL_OCCURRED Setting */

 #define MXC_F_SPIMSS_REVA_INT_FL_TOVR_POS                   6 /**< INT_FL_TOVR Position */
 #define MXC_F_SPIMSS_REVA_INT_FL_TOVR                       ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_INT_FL_TOVR_POS)) /**< INT_FL_TOVR Mask */
 #define MXC_V_SPIMSS_REVA_INT_FL_TOVR_NOEVENT               ((uint32_t)0x0UL) /**< INT_FL_TOVR_NOEVENT Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_TOVR_NOEVENT               (MXC_V_SPIMSS_REVA_INT_FL_TOVR_NOEVENT << MXC_F_SPIMSS_REVA_INT_FL_TOVR_POS) /**< INT_FL_TOVR_NOEVENT Setting */
 #define MXC_V_SPIMSS_REVA_INT_FL_TOVR_OCCURRED              ((uint32_t)0x1UL) /**< INT_FL_TOVR_OCCURRED Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_TOVR_OCCURRED              (MXC_V_SPIMSS_REVA_INT_FL_TOVR_OCCURRED << MXC_F_SPIMSS_REVA_INT_FL_TOVR_POS) /**< INT_FL_TOVR_OCCURRED Setting */

 #define MXC_F_SPIMSS_REVA_INT_FL_IRQ_POS                    7 /**< INT_FL_IRQ Position */
 #define MXC_F_SPIMSS_REVA_INT_FL_IRQ                        ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_INT_FL_IRQ_POS)) /**< INT_FL_IRQ Mask */
 #define MXC_V_SPIMSS_REVA_INT_FL_IRQ_INACTIVE               ((uint32_t)0x0UL) /**< INT_FL_IRQ_INACTIVE Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_IRQ_INACTIVE               (MXC_V_SPIMSS_REVA_INT_FL_IRQ_INACTIVE << MXC_F_SPIMSS_REVA_INT_FL_IRQ_POS) /**< INT_FL_IRQ_INACTIVE Setting */
 #define MXC_V_SPIMSS_REVA_INT_FL_IRQ_PENDING                ((uint32_t)0x1UL) /**< INT_FL_IRQ_PENDING Value */
 #define MXC_S_SPIMSS_REVA_INT_FL_IRQ_PENDING                (MXC_V_SPIMSS_REVA_INT_FL_IRQ_PENDING << MXC_F_SPIMSS_REVA_INT_FL_IRQ_POS) /**< INT_FL_IRQ_PENDING Setting */

/**@} end of group SPIMSS_INT_FL_Register */

/**
 * @ingroup  spimss_registers
 * @defgroup SPIMSS_MODE SPIMSS_MODE
 * @brief    SPI Mode Register.
 * @{
 */
 #define MXC_F_SPIMSS_REVA_MODE_SSV_POS                      0 /**< MODE_SSV Position */
 #define MXC_F_SPIMSS_REVA_MODE_SSV                          ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_MODE_SSV_POS)) /**< MODE_SSV Mask */
 #define MXC_V_SPIMSS_REVA_MODE_SSV_LO                       ((uint32_t)0x0UL) /**< MODE_SSV_LO Value */
 #define MXC_S_SPIMSS_REVA_MODE_SSV_LO                       (MXC_V_SPIMSS_REVA_MODE_SSV_LO << MXC_F_SPIMSS_REVA_MODE_SSV_POS) /**< MODE_SSV_LO Setting */
 #define MXC_V_SPIMSS_REVA_MODE_SSV_HI                       ((uint32_t)0x1UL) /**< MODE_SSV_HI Value */
 #define MXC_S_SPIMSS_REVA_MODE_SSV_HI                       (MXC_V_SPIMSS_REVA_MODE_SSV_HI << MXC_F_SPIMSS_REVA_MODE_SSV_POS) /**< MODE_SSV_HI Setting */

 #define MXC_F_SPIMSS_REVA_MODE_SS_IO_POS                    1 /**< MODE_SS_IO Position */
 #define MXC_F_SPIMSS_REVA_MODE_SS_IO                        ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_MODE_SS_IO_POS)) /**< MODE_SS_IO Mask */
 #define MXC_V_SPIMSS_REVA_MODE_SS_IO_INPUT                  ((uint32_t)0x0UL) /**< MODE_SS_IO_INPUT Value */
 #define MXC_S_SPIMSS_REVA_MODE_SS_IO_INPUT                  (MXC_V_SPIMSS_REVA_MODE_SS_IO_INPUT << MXC_F_SPIMSS_REVA_MODE_SS_IO_POS) /**< MODE_SS_IO_INPUT Setting */
 #define MXC_V_SPIMSS_REVA_MODE_SS_IO_OUTPUT                 ((uint32_t)0x1UL) /**< MODE_SS_IO_OUTPUT Value */
 #define MXC_S_SPIMSS_REVA_MODE_SS_IO_OUTPUT                 (MXC_V_SPIMSS_REVA_MODE_SS_IO_OUTPUT << MXC_F_SPIMSS_REVA_MODE_SS_IO_POS) /**< MODE_SS_IO_OUTPUT Setting */

 #define MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS                  2 /**< MODE_NUMBITS Position */
 #define MXC_F_SPIMSS_REVA_MODE_NUMBITS                      ((uint32_t)(0xFUL << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS)) /**< MODE_NUMBITS Mask */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS16               ((uint32_t)0x0UL) /**< MODE_NUMBITS_BITS16 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS16               (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS16 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS16 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS1                ((uint32_t)0x1UL) /**< MODE_NUMBITS_BITS1 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS1                (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS1 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS1 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS2                ((uint32_t)0x2UL) /**< MODE_NUMBITS_BITS2 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS2                (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS2 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS2 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS3                ((uint32_t)0x3UL) /**< MODE_NUMBITS_BITS3 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS3                (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS3 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS3 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS4                ((uint32_t)0x4UL) /**< MODE_NUMBITS_BITS4 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS4                (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS4 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS4 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS5                ((uint32_t)0x5UL) /**< MODE_NUMBITS_BITS5 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS5                (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS5 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS5 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS6                ((uint32_t)0x6UL) /**< MODE_NUMBITS_BITS6 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS6                (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS6 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS6 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS7                ((uint32_t)0x7UL) /**< MODE_NUMBITS_BITS7 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS7                (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS7 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS7 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS8                ((uint32_t)0x8UL) /**< MODE_NUMBITS_BITS8 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS8                (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS8 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS8 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS9                ((uint32_t)0x9UL) /**< MODE_NUMBITS_BITS9 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS9                (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS9 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS9 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS10               ((uint32_t)0xAUL) /**< MODE_NUMBITS_BITS10 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS10               (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS10 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS10 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS11               ((uint32_t)0xBUL) /**< MODE_NUMBITS_BITS11 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS11               (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS11 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS11 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS12               ((uint32_t)0xCUL) /**< MODE_NUMBITS_BITS12 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS12               (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS12 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS12 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS13               ((uint32_t)0xDUL) /**< MODE_NUMBITS_BITS13 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS13               (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS13 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS13 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS14               ((uint32_t)0xEUL) /**< MODE_NUMBITS_BITS14 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS14               (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS14 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS14 Setting */
 #define MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS15               ((uint32_t)0xFUL) /**< MODE_NUMBITS_BITS15 Value */
 #define MXC_S_SPIMSS_REVA_MODE_NUMBITS_BITS15               (MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS15 << MXC_F_SPIMSS_REVA_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS15 Setting */

 #define MXC_F_SPIMSS_REVA_MODE_TX_LJ_POS                    7 /**< MODE_TX_LJ Position */
 #define MXC_F_SPIMSS_REVA_MODE_TX_LJ                        ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_MODE_TX_LJ_POS)) /**< MODE_TX_LJ Mask */
 #define MXC_V_SPIMSS_REVA_MODE_TX_LJ_DISABLE                ((uint32_t)0x0UL) /**< MODE_TX_LJ_DISABLE Value */
 #define MXC_S_SPIMSS_REVA_MODE_TX_LJ_DISABLE                (MXC_V_SPIMSS_REVA_MODE_TX_LJ_DISABLE << MXC_F_SPIMSS_REVA_MODE_TX_LJ_POS) /**< MODE_TX_LJ_DISABLE Setting */
 #define MXC_V_SPIMSS_REVA_MODE_TX_LJ_ENABLE                 ((uint32_t)0x1UL) /**< MODE_TX_LJ_ENABLE Value */
 #define MXC_S_SPIMSS_REVA_MODE_TX_LJ_ENABLE                 (MXC_V_SPIMSS_REVA_MODE_TX_LJ_ENABLE << MXC_F_SPIMSS_REVA_MODE_TX_LJ_POS) /**< MODE_TX_LJ_ENABLE Setting */

/**@} end of group SPIMSS_MODE_Register */

/**
 * @ingroup  spimss_registers
 * @defgroup SPIMSS_BRG SPIMSS_BRG
 * @brief    Baud Rate Reload Value. The SPI Baud Rate register is a 16-bit reload value for
 *           the SPI Baud Rate Generator. The reload value must be greater than or equal to
 *           0002H for proper SPI operation (maximum baud rate is PCLK frequency divided by
 *           4).
 * @{
 */
 #define MXC_F_SPIMSS_REVA_BRG_DIV_POS                       0 /**< BRG_DIV Position */
 #define MXC_F_SPIMSS_REVA_BRG_DIV                           ((uint32_t)(0xFFFFUL << MXC_F_SPIMSS_REVA_BRG_DIV_POS)) /**< BRG_DIV Mask */

/**@} end of group SPIMSS_BRG_Register */

/**
 * @ingroup  spimss_registers
 * @defgroup SPIMSS_DMA SPIMSS_DMA
 * @brief    SPI DMA Register.
 * @{
 */
 #define MXC_F_SPIMSS_REVA_DMA_TX_FIFO_LVL_POS               0 /**< DMA_TX_FIFO_LVL Position */
 #define MXC_F_SPIMSS_REVA_DMA_TX_FIFO_LVL                   ((uint32_t)(0x7UL << MXC_F_SPIMSS_REVA_DMA_TX_FIFO_LVL_POS)) /**< DMA_TX_FIFO_LVL Mask */
 #define MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRY1            ((uint32_t)0x0UL) /**< DMA_TX_FIFO_LVL_ENTRY1 Value */
 #define MXC_S_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRY1            (MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRY1 << MXC_F_SPIMSS_REVA_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRY1 Setting */
 #define MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES2          ((uint32_t)0x1UL) /**< DMA_TX_FIFO_LVL_ENTRIES2 Value */
 #define MXC_S_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES2          (MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES2 << MXC_F_SPIMSS_REVA_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRIES2 Setting */
 #define MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES3          ((uint32_t)0x2UL) /**< DMA_TX_FIFO_LVL_ENTRIES3 Value */
 #define MXC_S_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES3          (MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES3 << MXC_F_SPIMSS_REVA_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRIES3 Setting */
 #define MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES4          ((uint32_t)0x3UL) /**< DMA_TX_FIFO_LVL_ENTRIES4 Value */
 #define MXC_S_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES4          (MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES4 << MXC_F_SPIMSS_REVA_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRIES4 Setting */
 #define MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES5          ((uint32_t)0x4UL) /**< DMA_TX_FIFO_LVL_ENTRIES5 Value */
 #define MXC_S_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES5          (MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES5 << MXC_F_SPIMSS_REVA_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRIES5 Setting */
 #define MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES6          ((uint32_t)0x5UL) /**< DMA_TX_FIFO_LVL_ENTRIES6 Value */
 #define MXC_S_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES6          (MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES6 << MXC_F_SPIMSS_REVA_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRIES6 Setting */
 #define MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES7          ((uint32_t)0x6UL) /**< DMA_TX_FIFO_LVL_ENTRIES7 Value */
 #define MXC_S_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES7          (MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES7 << MXC_F_SPIMSS_REVA_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRIES7 Setting */
 #define MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES8          ((uint32_t)0x7UL) /**< DMA_TX_FIFO_LVL_ENTRIES8 Value */
 #define MXC_S_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES8          (MXC_V_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES8 << MXC_F_SPIMSS_REVA_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRIES8 Setting */

 #define MXC_F_SPIMSS_REVA_DMA_TX_FIFO_CLR_POS               4 /**< DMA_TX_FIFO_CLR Position */
 #define MXC_F_SPIMSS_REVA_DMA_TX_FIFO_CLR                   ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_DMA_TX_FIFO_CLR_POS)) /**< DMA_TX_FIFO_CLR Mask */
 #define MXC_V_SPIMSS_REVA_DMA_TX_FIFO_CLR_COMPLETE          ((uint32_t)0x0UL) /**< DMA_TX_FIFO_CLR_COMPLETE Value */
 #define MXC_S_SPIMSS_REVA_DMA_TX_FIFO_CLR_COMPLETE          (MXC_V_SPIMSS_REVA_DMA_TX_FIFO_CLR_COMPLETE << MXC_F_SPIMSS_REVA_DMA_TX_FIFO_CLR_POS) /**< DMA_TX_FIFO_CLR_COMPLETE Setting */
 #define MXC_V_SPIMSS_REVA_DMA_TX_FIFO_CLR_START             ((uint32_t)0x1UL) /**< DMA_TX_FIFO_CLR_START Value */
 #define MXC_S_SPIMSS_REVA_DMA_TX_FIFO_CLR_START             (MXC_V_SPIMSS_REVA_DMA_TX_FIFO_CLR_START << MXC_F_SPIMSS_REVA_DMA_TX_FIFO_CLR_POS) /**< DMA_TX_FIFO_CLR_START Setting */

 #define MXC_F_SPIMSS_REVA_DMA_TX_FIFO_CNT_POS               8 /**< DMA_TX_FIFO_CNT Position */
 #define MXC_F_SPIMSS_REVA_DMA_TX_FIFO_CNT                   ((uint32_t)(0xFUL << MXC_F_SPIMSS_REVA_DMA_TX_FIFO_CNT_POS)) /**< DMA_TX_FIFO_CNT Mask */

 #define MXC_F_SPIMSS_REVA_DMA_TX_DMA_EN_POS                 15 /**< DMA_TX_DMA_EN Position */
 #define MXC_F_SPIMSS_REVA_DMA_TX_DMA_EN                     ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_DMA_TX_DMA_EN_POS)) /**< DMA_TX_DMA_EN Mask */
 #define MXC_V_SPIMSS_REVA_DMA_TX_DMA_EN_DISABLE             ((uint32_t)0x0UL) /**< DMA_TX_DMA_EN_DISABLE Value */
 #define MXC_S_SPIMSS_REVA_DMA_TX_DMA_EN_DISABLE             (MXC_V_SPIMSS_REVA_DMA_TX_DMA_EN_DISABLE << MXC_F_SPIMSS_REVA_DMA_TX_DMA_EN_POS) /**< DMA_TX_DMA_EN_DISABLE Setting */
 #define MXC_V_SPIMSS_REVA_DMA_TX_DMA_EN_ENABLE              ((uint32_t)0x1UL) /**< DMA_TX_DMA_EN_ENABLE Value */
 #define MXC_S_SPIMSS_REVA_DMA_TX_DMA_EN_ENABLE              (MXC_V_SPIMSS_REVA_DMA_TX_DMA_EN_ENABLE << MXC_F_SPIMSS_REVA_DMA_TX_DMA_EN_POS) /**< DMA_TX_DMA_EN_ENABLE Setting */

 #define MXC_F_SPIMSS_REVA_DMA_RX_FIFO_LVL_POS               16 /**< DMA_RX_FIFO_LVL Position */
 #define MXC_F_SPIMSS_REVA_DMA_RX_FIFO_LVL                   ((uint32_t)(0x7UL << MXC_F_SPIMSS_REVA_DMA_RX_FIFO_LVL_POS)) /**< DMA_RX_FIFO_LVL Mask */
 #define MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRY1            ((uint32_t)0x0UL) /**< DMA_RX_FIFO_LVL_ENTRY1 Value */
 #define MXC_S_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRY1            (MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRY1 << MXC_F_SPIMSS_REVA_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRY1 Setting */
 #define MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES2          ((uint32_t)0x1UL) /**< DMA_RX_FIFO_LVL_ENTRIES2 Value */
 #define MXC_S_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES2          (MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES2 << MXC_F_SPIMSS_REVA_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRIES2 Setting */
 #define MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES3          ((uint32_t)0x2UL) /**< DMA_RX_FIFO_LVL_ENTRIES3 Value */
 #define MXC_S_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES3          (MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES3 << MXC_F_SPIMSS_REVA_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRIES3 Setting */
 #define MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES4          ((uint32_t)0x3UL) /**< DMA_RX_FIFO_LVL_ENTRIES4 Value */
 #define MXC_S_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES4          (MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES4 << MXC_F_SPIMSS_REVA_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRIES4 Setting */
 #define MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES5          ((uint32_t)0x4UL) /**< DMA_RX_FIFO_LVL_ENTRIES5 Value */
 #define MXC_S_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES5          (MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES5 << MXC_F_SPIMSS_REVA_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRIES5 Setting */
 #define MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES6          ((uint32_t)0x5UL) /**< DMA_RX_FIFO_LVL_ENTRIES6 Value */
 #define MXC_S_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES6          (MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES6 << MXC_F_SPIMSS_REVA_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRIES6 Setting */
 #define MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES7          ((uint32_t)0x6UL) /**< DMA_RX_FIFO_LVL_ENTRIES7 Value */
 #define MXC_S_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES7          (MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES7 << MXC_F_SPIMSS_REVA_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRIES7 Setting */
 #define MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES8          ((uint32_t)0x7UL) /**< DMA_RX_FIFO_LVL_ENTRIES8 Value */
 #define MXC_S_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES8          (MXC_V_SPIMSS_REVA_DMA_RX_FIFO_LVL_ENTRIES8 << MXC_F_SPIMSS_REVA_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRIES8 Setting */

 #define MXC_F_SPIMSS_REVA_DMA_RX_FIFO_CLR_POS               20 /**< DMA_RX_FIFO_CLR Position */
 #define MXC_F_SPIMSS_REVA_DMA_RX_FIFO_CLR                   ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_DMA_RX_FIFO_CLR_POS)) /**< DMA_RX_FIFO_CLR Mask */
 #define MXC_V_SPIMSS_REVA_DMA_RX_FIFO_CLR_COMPLETE          ((uint32_t)0x0UL) /**< DMA_RX_FIFO_CLR_COMPLETE Value */
 #define MXC_S_SPIMSS_REVA_DMA_RX_FIFO_CLR_COMPLETE          (MXC_V_SPIMSS_REVA_DMA_RX_FIFO_CLR_COMPLETE << MXC_F_SPIMSS_REVA_DMA_RX_FIFO_CLR_POS) /**< DMA_RX_FIFO_CLR_COMPLETE Setting */
 #define MXC_V_SPIMSS_REVA_DMA_RX_FIFO_CLR_START             ((uint32_t)0x1UL) /**< DMA_RX_FIFO_CLR_START Value */
 #define MXC_S_SPIMSS_REVA_DMA_RX_FIFO_CLR_START             (MXC_V_SPIMSS_REVA_DMA_RX_FIFO_CLR_START << MXC_F_SPIMSS_REVA_DMA_RX_FIFO_CLR_POS) /**< DMA_RX_FIFO_CLR_START Setting */

 #define MXC_F_SPIMSS_REVA_DMA_RX_FIFO_CNT_POS               24 /**< DMA_RX_FIFO_CNT Position */
 #define MXC_F_SPIMSS_REVA_DMA_RX_FIFO_CNT                   ((uint32_t)(0xFUL << MXC_F_SPIMSS_REVA_DMA_RX_FIFO_CNT_POS)) /**< DMA_RX_FIFO_CNT Mask */

 #define MXC_F_SPIMSS_REVA_DMA_RX_DMA_EN_POS                 31 /**< DMA_RX_DMA_EN Position */
 #define MXC_F_SPIMSS_REVA_DMA_RX_DMA_EN                     ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_DMA_RX_DMA_EN_POS)) /**< DMA_RX_DMA_EN Mask */
 #define MXC_V_SPIMSS_REVA_DMA_RX_DMA_EN_DISABLE             ((uint32_t)0x0UL) /**< DMA_RX_DMA_EN_DISABLE Value */
 #define MXC_S_SPIMSS_REVA_DMA_RX_DMA_EN_DISABLE             (MXC_V_SPIMSS_REVA_DMA_RX_DMA_EN_DISABLE << MXC_F_SPIMSS_REVA_DMA_RX_DMA_EN_POS) /**< DMA_RX_DMA_EN_DISABLE Setting */
 #define MXC_V_SPIMSS_REVA_DMA_RX_DMA_EN_ENABLE              ((uint32_t)0x1UL) /**< DMA_RX_DMA_EN_ENABLE Value */
 #define MXC_S_SPIMSS_REVA_DMA_RX_DMA_EN_ENABLE              (MXC_V_SPIMSS_REVA_DMA_RX_DMA_EN_ENABLE << MXC_F_SPIMSS_REVA_DMA_RX_DMA_EN_POS) /**< DMA_RX_DMA_EN_ENABLE Setting */

/**@} end of group SPIMSS_DMA_Register */

/**
 * @ingroup  spimss_registers
 * @defgroup SPIMSS_I2S_CTRL SPIMSS_I2S_CTRL
 * @brief    I2S Control Register.
 * @{
 */
 #define MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_EN_POS               0 /**< I2S_CTRL_I2S_EN Position */
 #define MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_EN                   ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_EN_POS)) /**< I2S_CTRL_I2S_EN Mask */
 #define MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_EN_DISABLE           ((uint32_t)0x0UL) /**< I2S_CTRL_I2S_EN_DISABLE Value */
 #define MXC_S_SPIMSS_REVA_I2S_CTRL_I2S_EN_DISABLE           (MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_EN_DISABLE << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_EN_POS) /**< I2S_CTRL_I2S_EN_DISABLE Setting */
 #define MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_EN_ENABLE            ((uint32_t)0x1UL) /**< I2S_CTRL_I2S_EN_ENABLE Value */
 #define MXC_S_SPIMSS_REVA_I2S_CTRL_I2S_EN_ENABLE            (MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_EN_ENABLE << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_EN_POS) /**< I2S_CTRL_I2S_EN_ENABLE Setting */

 #define MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_MUTE_POS             1 /**< I2S_CTRL_I2S_MUTE Position */
 #define MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_MUTE                 ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_MUTE_POS)) /**< I2S_CTRL_I2S_MUTE Mask */
 #define MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_MUTE_NORMAL          ((uint32_t)0x0UL) /**< I2S_CTRL_I2S_MUTE_NORMAL Value */
 #define MXC_S_SPIMSS_REVA_I2S_CTRL_I2S_MUTE_NORMAL          (MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_MUTE_NORMAL << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_MUTE_POS) /**< I2S_CTRL_I2S_MUTE_NORMAL Setting */
 #define MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_MUTE_REPLACED        ((uint32_t)0x1UL) /**< I2S_CTRL_I2S_MUTE_REPLACED Value */
 #define MXC_S_SPIMSS_REVA_I2S_CTRL_I2S_MUTE_REPLACED        (MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_MUTE_REPLACED << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_MUTE_POS) /**< I2S_CTRL_I2S_MUTE_REPLACED Setting */

 #define MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_PAUSE_POS            2 /**< I2S_CTRL_I2S_PAUSE Position */
 #define MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_PAUSE                ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_PAUSE_POS)) /**< I2S_CTRL_I2S_PAUSE Mask */
 #define MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_PAUSE_NORMAL         ((uint32_t)0x0UL) /**< I2S_CTRL_I2S_PAUSE_NORMAL Value */
 #define MXC_S_SPIMSS_REVA_I2S_CTRL_I2S_PAUSE_NORMAL         (MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_PAUSE_NORMAL << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_PAUSE_POS) /**< I2S_CTRL_I2S_PAUSE_NORMAL Setting */
 #define MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_PAUSE_HALT           ((uint32_t)0x1UL) /**< I2S_CTRL_I2S_PAUSE_HALT Value */
 #define MXC_S_SPIMSS_REVA_I2S_CTRL_I2S_PAUSE_HALT           (MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_PAUSE_HALT << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_PAUSE_POS) /**< I2S_CTRL_I2S_PAUSE_HALT Setting */

 #define MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_MONO_POS             3 /**< I2S_CTRL_I2S_MONO Position */
 #define MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_MONO                 ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_MONO_POS)) /**< I2S_CTRL_I2S_MONO Mask */
 #define MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_MONO_STEREOPHONIC    ((uint32_t)0x0UL) /**< I2S_CTRL_I2S_MONO_STEREOPHONIC Value */
 #define MXC_S_SPIMSS_REVA_I2S_CTRL_I2S_MONO_STEREOPHONIC    (MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_MONO_STEREOPHONIC << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_MONO_POS) /**< I2S_CTRL_I2S_MONO_STEREOPHONIC Setting */
 #define MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_MONO_MONOPHONIC      ((uint32_t)0x1UL) /**< I2S_CTRL_I2S_MONO_MONOPHONIC Value */
 #define MXC_S_SPIMSS_REVA_I2S_CTRL_I2S_MONO_MONOPHONIC      (MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_MONO_MONOPHONIC << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_MONO_POS) /**< I2S_CTRL_I2S_MONO_MONOPHONIC Setting */

 #define MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_LJ_POS               4 /**< I2S_CTRL_I2S_LJ Position */
 #define MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_LJ                   ((uint32_t)(0x1UL << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_LJ_POS)) /**< I2S_CTRL_I2S_LJ Mask */
 #define MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_LJ_NORMAL            ((uint32_t)0x0UL) /**< I2S_CTRL_I2S_LJ_NORMAL Value */
 #define MXC_S_SPIMSS_REVA_I2S_CTRL_I2S_LJ_NORMAL            (MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_LJ_NORMAL << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_LJ_POS) /**< I2S_CTRL_I2S_LJ_NORMAL Setting */
 #define MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_LJ_REPLACED          ((uint32_t)0x1UL) /**< I2S_CTRL_I2S_LJ_REPLACED Value */
 #define MXC_S_SPIMSS_REVA_I2S_CTRL_I2S_LJ_REPLACED          (MXC_V_SPIMSS_REVA_I2S_CTRL_I2S_LJ_REPLACED << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_LJ_POS) /**< I2S_CTRL_I2S_LJ_REPLACED Setting */

/**@} end of group SPIMSS_I2S_CTRL_Register */

#ifdef __cplusplus
}
#endif

#endif /* _SPIMSS_REVA_REGS_H_ */
