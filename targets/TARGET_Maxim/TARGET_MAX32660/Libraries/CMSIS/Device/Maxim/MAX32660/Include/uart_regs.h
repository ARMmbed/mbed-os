/**
 * @file    uart_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the UART Peripheral Module.
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

#ifndef _UART_REGS_H_
#define _UART_REGS_H_

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
 * @ingroup     uart
 * @defgroup    uart_registers UART_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the UART Peripheral Module.
 * @details UART
 */

/**
 * @ingroup uart_registers
 * Structure type to access the UART Registers.
 */
typedef struct {
    __IO uint32_t ctrl0;                /**< <tt>\b 0x00:</tt> UART CTRL0 Register */
    __IO uint32_t ctrl1;                /**< <tt>\b 0x04:</tt> UART CTRL1 Register */
    __I  uint32_t stat;                 /**< <tt>\b 0x08:</tt> UART STAT Register */
    __IO uint32_t int_en;               /**< <tt>\b 0x0C:</tt> UART INT_EN Register */
    __IO uint32_t int_fl;               /**< <tt>\b 0x10:</tt> UART INT_FL Register */
    __IO uint32_t baud0;                /**< <tt>\b 0x14:</tt> UART BAUD0 Register */
    __IO uint32_t baud1;                /**< <tt>\b 0x18:</tt> UART BAUD1 Register */
    __IO uint32_t fifo;                 /**< <tt>\b 0x1C:</tt> UART FIFO Register */
    __IO uint32_t dma;                  /**< <tt>\b 0x20:</tt> UART DMA Register */
    __IO uint32_t txfifo;               /**< <tt>\b 0x24:</tt> UART TXFIFO Register */
} mxc_uart_regs_t;

/* Register offsets for module UART */
/**
 * @ingroup    uart_registers
 * @defgroup   UART_Register_Offsets Register Offsets
 * @brief      UART Peripheral Register Offsets from the UART Base Peripheral Address. 
 * @{
 */
 #define MXC_R_UART_CTRL0                   ((uint32_t)0x00000000UL) /**< Offset from UART Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_UART_CTRL1                   ((uint32_t)0x00000004UL) /**< Offset from UART Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_UART_STAT                    ((uint32_t)0x00000008UL) /**< Offset from UART Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_UART_INT_EN                  ((uint32_t)0x0000000CUL) /**< Offset from UART Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_UART_INT_FL                  ((uint32_t)0x00000010UL) /**< Offset from UART Base Address: <tt> 0x0010</tt> */ 
 #define MXC_R_UART_BAUD0                   ((uint32_t)0x00000014UL) /**< Offset from UART Base Address: <tt> 0x0014</tt> */ 
 #define MXC_R_UART_BAUD1                   ((uint32_t)0x00000018UL) /**< Offset from UART Base Address: <tt> 0x0018</tt> */ 
 #define MXC_R_UART_FIFO                    ((uint32_t)0x0000001CUL) /**< Offset from UART Base Address: <tt> 0x001C</tt> */ 
 #define MXC_R_UART_DMA                     ((uint32_t)0x00000020UL) /**< Offset from UART Base Address: <tt> 0x0020</tt> */ 
 #define MXC_R_UART_TXFIFO                  ((uint32_t)0x00000024UL) /**< Offset from UART Base Address: <tt> 0x0024</tt> */ 
/**@} end of group uart_registers */

/**
 * @ingroup  uart_registers
 * @defgroup UART_CTRL0 UART_CTRL0
 * @brief    Control Register.
 * @{
 */
 #define MXC_F_UART_CTRL0_ENABLE_POS                    0 /**< CTRL0_ENABLE Position */
 #define MXC_F_UART_CTRL0_ENABLE                        ((uint32_t)(0x1UL << MXC_F_UART_CTRL0_ENABLE_POS)) /**< CTRL0_ENABLE Mask */

 #define MXC_F_UART_CTRL0_PARITY_EN_POS                 1 /**< CTRL0_PARITY_EN Position */
 #define MXC_F_UART_CTRL0_PARITY_EN                     ((uint32_t)(0x1UL << MXC_F_UART_CTRL0_PARITY_EN_POS)) /**< CTRL0_PARITY_EN Mask */

 #define MXC_F_UART_CTRL0_PARITY_MODE_POS               2 /**< CTRL0_PARITY_MODE Position */
 #define MXC_F_UART_CTRL0_PARITY_MODE                   ((uint32_t)(0x3UL << MXC_F_UART_CTRL0_PARITY_MODE_POS)) /**< CTRL0_PARITY_MODE Mask */
 #define MXC_V_UART_CTRL0_PARITY_MODE_EVEN              ((uint32_t)0x0UL) /**< CTRL0_PARITY_MODE_EVEN Value */
 #define MXC_S_UART_CTRL0_PARITY_MODE_EVEN              (MXC_V_UART_CTRL0_PARITY_MODE_EVEN << MXC_F_UART_CTRL0_PARITY_MODE_POS) /**< CTRL0_PARITY_MODE_EVEN Setting */
 #define MXC_V_UART_CTRL0_PARITY_MODE_ODD               ((uint32_t)0x1UL) /**< CTRL0_PARITY_MODE_ODD Value */
 #define MXC_S_UART_CTRL0_PARITY_MODE_ODD               (MXC_V_UART_CTRL0_PARITY_MODE_ODD << MXC_F_UART_CTRL0_PARITY_MODE_POS) /**< CTRL0_PARITY_MODE_ODD Setting */
 #define MXC_V_UART_CTRL0_PARITY_MODE_MARK              ((uint32_t)0x2UL) /**< CTRL0_PARITY_MODE_MARK Value */
 #define MXC_S_UART_CTRL0_PARITY_MODE_MARK              (MXC_V_UART_CTRL0_PARITY_MODE_MARK << MXC_F_UART_CTRL0_PARITY_MODE_POS) /**< CTRL0_PARITY_MODE_MARK Setting */
 #define MXC_V_UART_CTRL0_PARITY_MODE_SPACE             ((uint32_t)0x3UL) /**< CTRL0_PARITY_MODE_SPACE Value */
 #define MXC_S_UART_CTRL0_PARITY_MODE_SPACE             (MXC_V_UART_CTRL0_PARITY_MODE_SPACE << MXC_F_UART_CTRL0_PARITY_MODE_POS) /**< CTRL0_PARITY_MODE_SPACE Setting */

 #define MXC_F_UART_CTRL0_PARITY_LVL_POS                4 /**< CTRL0_PARITY_LVL Position */
 #define MXC_F_UART_CTRL0_PARITY_LVL                    ((uint32_t)(0x1UL << MXC_F_UART_CTRL0_PARITY_LVL_POS)) /**< CTRL0_PARITY_LVL Mask */

 #define MXC_F_UART_CTRL0_TXFLUSH_POS                   5 /**< CTRL0_TXFLUSH Position */
 #define MXC_F_UART_CTRL0_TXFLUSH                       ((uint32_t)(0x1UL << MXC_F_UART_CTRL0_TXFLUSH_POS)) /**< CTRL0_TXFLUSH Mask */

 #define MXC_F_UART_CTRL0_RXFLUSH_POS                   6 /**< CTRL0_RXFLUSH Position */
 #define MXC_F_UART_CTRL0_RXFLUSH                       ((uint32_t)(0x1UL << MXC_F_UART_CTRL0_RXFLUSH_POS)) /**< CTRL0_RXFLUSH Mask */

 #define MXC_F_UART_CTRL0_BITACC_POS                    7 /**< CTRL0_BITACC Position */
 #define MXC_F_UART_CTRL0_BITACC                        ((uint32_t)(0x1UL << MXC_F_UART_CTRL0_BITACC_POS)) /**< CTRL0_BITACC Mask */

 #define MXC_F_UART_CTRL0_SIZE_POS                      8 /**< CTRL0_SIZE Position */
 #define MXC_F_UART_CTRL0_SIZE                          ((uint32_t)(0x3UL << MXC_F_UART_CTRL0_SIZE_POS)) /**< CTRL0_SIZE Mask */
 #define MXC_V_UART_CTRL0_SIZE_5                        ((uint32_t)0x0UL) /**< CTRL0_SIZE_5 Value */
 #define MXC_S_UART_CTRL0_SIZE_5                        (MXC_V_UART_CTRL0_SIZE_5 << MXC_F_UART_CTRL0_SIZE_POS) /**< CTRL0_SIZE_5 Setting */
 #define MXC_V_UART_CTRL0_SIZE_6                        ((uint32_t)0x1UL) /**< CTRL0_SIZE_6 Value */
 #define MXC_S_UART_CTRL0_SIZE_6                        (MXC_V_UART_CTRL0_SIZE_6 << MXC_F_UART_CTRL0_SIZE_POS) /**< CTRL0_SIZE_6 Setting */
 #define MXC_V_UART_CTRL0_SIZE_7                        ((uint32_t)0x2UL) /**< CTRL0_SIZE_7 Value */
 #define MXC_S_UART_CTRL0_SIZE_7                        (MXC_V_UART_CTRL0_SIZE_7 << MXC_F_UART_CTRL0_SIZE_POS) /**< CTRL0_SIZE_7 Setting */
 #define MXC_V_UART_CTRL0_SIZE_8                        ((uint32_t)0x3UL) /**< CTRL0_SIZE_8 Value */
 #define MXC_S_UART_CTRL0_SIZE_8                        (MXC_V_UART_CTRL0_SIZE_8 << MXC_F_UART_CTRL0_SIZE_POS) /**< CTRL0_SIZE_8 Setting */

 #define MXC_F_UART_CTRL0_STOP_POS                      10 /**< CTRL0_STOP Position */
 #define MXC_F_UART_CTRL0_STOP                          ((uint32_t)(0x1UL << MXC_F_UART_CTRL0_STOP_POS)) /**< CTRL0_STOP Mask */

 #define MXC_F_UART_CTRL0_FLOW_POS                      11 /**< CTRL0_FLOW Position */
 #define MXC_F_UART_CTRL0_FLOW                          ((uint32_t)(0x1UL << MXC_F_UART_CTRL0_FLOW_POS)) /**< CTRL0_FLOW Mask */

 #define MXC_F_UART_CTRL0_FLOWPOL_POS                   12 /**< CTRL0_FLOWPOL Position */
 #define MXC_F_UART_CTRL0_FLOWPOL                       ((uint32_t)(0x1UL << MXC_F_UART_CTRL0_FLOWPOL_POS)) /**< CTRL0_FLOWPOL Mask */

 #define MXC_F_UART_CTRL0_NULLMOD_POS                   13 /**< CTRL0_NULLMOD Position */
 #define MXC_F_UART_CTRL0_NULLMOD                       ((uint32_t)(0x1UL << MXC_F_UART_CTRL0_NULLMOD_POS)) /**< CTRL0_NULLMOD Mask */

 #define MXC_F_UART_CTRL0_BREAK_POS                     14 /**< CTRL0_BREAK Position */
 #define MXC_F_UART_CTRL0_BREAK                         ((uint32_t)(0x1UL << MXC_F_UART_CTRL0_BREAK_POS)) /**< CTRL0_BREAK Mask */

 #define MXC_F_UART_CTRL0_CLK_SEL_POS                   15 /**< CTRL0_CLK_SEL Position */
 #define MXC_F_UART_CTRL0_CLK_SEL                       ((uint32_t)(0x1UL << MXC_F_UART_CTRL0_CLK_SEL_POS)) /**< CTRL0_CLK_SEL Mask */

 #define MXC_F_UART_CTRL0_TO_CNT_POS                    16 /**< CTRL0_TO_CNT Position */
 #define MXC_F_UART_CTRL0_TO_CNT                        ((uint32_t)(0xFFUL << MXC_F_UART_CTRL0_TO_CNT_POS)) /**< CTRL0_TO_CNT Mask */

/**@} end of group UART_CTRL0_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_CTRL1 UART_CTRL1
 * @brief    Threshold Control register.
 * @{
 */
 #define MXC_F_UART_CTRL1_RX_FIFO_LVL_POS               0 /**< CTRL1_RX_FIFO_LVL Position */
 #define MXC_F_UART_CTRL1_RX_FIFO_LVL                   ((uint32_t)(0x3FUL << MXC_F_UART_CTRL1_RX_FIFO_LVL_POS)) /**< CTRL1_RX_FIFO_LVL Mask */

 #define MXC_F_UART_CTRL1_TX_FIFO_LVL_POS               8 /**< CTRL1_TX_FIFO_LVL Position */
 #define MXC_F_UART_CTRL1_TX_FIFO_LVL                   ((uint32_t)(0x3FUL << MXC_F_UART_CTRL1_TX_FIFO_LVL_POS)) /**< CTRL1_TX_FIFO_LVL Mask */

 #define MXC_F_UART_CTRL1_RTS_FIFO_LVL_POS              16 /**< CTRL1_RTS_FIFO_LVL Position */
 #define MXC_F_UART_CTRL1_RTS_FIFO_LVL                  ((uint32_t)(0x3FUL << MXC_F_UART_CTRL1_RTS_FIFO_LVL_POS)) /**< CTRL1_RTS_FIFO_LVL Mask */

/**@} end of group UART_CTRL1_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_STAT UART_STAT
 * @brief    Status Register.
 * @{
 */
 #define MXC_F_UART_STAT_TX_BUSY_POS                    0 /**< STAT_TX_BUSY Position */
 #define MXC_F_UART_STAT_TX_BUSY                        ((uint32_t)(0x1UL << MXC_F_UART_STAT_TX_BUSY_POS)) /**< STAT_TX_BUSY Mask */

 #define MXC_F_UART_STAT_RX_BUSY_POS                    1 /**< STAT_RX_BUSY Position */
 #define MXC_F_UART_STAT_RX_BUSY                        ((uint32_t)(0x1UL << MXC_F_UART_STAT_RX_BUSY_POS)) /**< STAT_RX_BUSY Mask */

 #define MXC_F_UART_STAT_PARITY_POS                     2 /**< STAT_PARITY Position */
 #define MXC_F_UART_STAT_PARITY                         ((uint32_t)(0x1UL << MXC_F_UART_STAT_PARITY_POS)) /**< STAT_PARITY Mask */

 #define MXC_F_UART_STAT_BREAK_POS                      3 /**< STAT_BREAK Position */
 #define MXC_F_UART_STAT_BREAK                          ((uint32_t)(0x1UL << MXC_F_UART_STAT_BREAK_POS)) /**< STAT_BREAK Mask */

 #define MXC_F_UART_STAT_RX_EMPTY_POS                   4 /**< STAT_RX_EMPTY Position */
 #define MXC_F_UART_STAT_RX_EMPTY                       ((uint32_t)(0x1UL << MXC_F_UART_STAT_RX_EMPTY_POS)) /**< STAT_RX_EMPTY Mask */

 #define MXC_F_UART_STAT_RX_FULL_POS                    5 /**< STAT_RX_FULL Position */
 #define MXC_F_UART_STAT_RX_FULL                        ((uint32_t)(0x1UL << MXC_F_UART_STAT_RX_FULL_POS)) /**< STAT_RX_FULL Mask */

 #define MXC_F_UART_STAT_TX_EMPTY_POS                   6 /**< STAT_TX_EMPTY Position */
 #define MXC_F_UART_STAT_TX_EMPTY                       ((uint32_t)(0x1UL << MXC_F_UART_STAT_TX_EMPTY_POS)) /**< STAT_TX_EMPTY Mask */

 #define MXC_F_UART_STAT_TX_FULL_POS                    7 /**< STAT_TX_FULL Position */
 #define MXC_F_UART_STAT_TX_FULL                        ((uint32_t)(0x1UL << MXC_F_UART_STAT_TX_FULL_POS)) /**< STAT_TX_FULL Mask */

 #define MXC_F_UART_STAT_RX_NUM_POS                     8 /**< STAT_RX_NUM Position */
 #define MXC_F_UART_STAT_RX_NUM                         ((uint32_t)(0x3FUL << MXC_F_UART_STAT_RX_NUM_POS)) /**< STAT_RX_NUM Mask */

 #define MXC_F_UART_STAT_TX_NUM_POS                     16 /**< STAT_TX_NUM Position */
 #define MXC_F_UART_STAT_TX_NUM                         ((uint32_t)(0x3FUL << MXC_F_UART_STAT_TX_NUM_POS)) /**< STAT_TX_NUM Mask */

 #define MXC_F_UART_STAT_RX_TO_POS                      24 /**< STAT_RX_TO Position */
 #define MXC_F_UART_STAT_RX_TO                          ((uint32_t)(0x1UL << MXC_F_UART_STAT_RX_TO_POS)) /**< STAT_RX_TO Mask */

/**@} end of group UART_STAT_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_INT_EN UART_INT_EN
 * @brief    Interrupt Enable Register.
 * @{
 */
 #define MXC_F_UART_INT_EN_RX_FRAME_ERROR_POS           0 /**< INT_EN_RX_FRAME_ERROR Position */
 #define MXC_F_UART_INT_EN_RX_FRAME_ERROR               ((uint32_t)(0x1UL << MXC_F_UART_INT_EN_RX_FRAME_ERROR_POS)) /**< INT_EN_RX_FRAME_ERROR Mask */

 #define MXC_F_UART_INT_EN_RX_PARITY_ERROR_POS          1 /**< INT_EN_RX_PARITY_ERROR Position */
 #define MXC_F_UART_INT_EN_RX_PARITY_ERROR              ((uint32_t)(0x1UL << MXC_F_UART_INT_EN_RX_PARITY_ERROR_POS)) /**< INT_EN_RX_PARITY_ERROR Mask */

 #define MXC_F_UART_INT_EN_CTS_POS                      2 /**< INT_EN_CTS Position */
 #define MXC_F_UART_INT_EN_CTS                          ((uint32_t)(0x1UL << MXC_F_UART_INT_EN_CTS_POS)) /**< INT_EN_CTS Mask */

 #define MXC_F_UART_INT_EN_RX_OVERRUN_POS               3 /**< INT_EN_RX_OVERRUN Position */
 #define MXC_F_UART_INT_EN_RX_OVERRUN                   ((uint32_t)(0x1UL << MXC_F_UART_INT_EN_RX_OVERRUN_POS)) /**< INT_EN_RX_OVERRUN Mask */

 #define MXC_F_UART_INT_EN_RX_FIFO_LVL_POS              4 /**< INT_EN_RX_FIFO_LVL Position */
 #define MXC_F_UART_INT_EN_RX_FIFO_LVL                  ((uint32_t)(0x1UL << MXC_F_UART_INT_EN_RX_FIFO_LVL_POS)) /**< INT_EN_RX_FIFO_LVL Mask */

 #define MXC_F_UART_INT_EN_TX_FIFO_AE_POS               5 /**< INT_EN_TX_FIFO_AE Position */
 #define MXC_F_UART_INT_EN_TX_FIFO_AE                   ((uint32_t)(0x1UL << MXC_F_UART_INT_EN_TX_FIFO_AE_POS)) /**< INT_EN_TX_FIFO_AE Mask */

 #define MXC_F_UART_INT_EN_TX_FIFO_LVL_POS              6 /**< INT_EN_TX_FIFO_LVL Position */
 #define MXC_F_UART_INT_EN_TX_FIFO_LVL                  ((uint32_t)(0x1UL << MXC_F_UART_INT_EN_TX_FIFO_LVL_POS)) /**< INT_EN_TX_FIFO_LVL Mask */

 #define MXC_F_UART_INT_EN_BREAK_POS                    7 /**< INT_EN_BREAK Position */
 #define MXC_F_UART_INT_EN_BREAK                        ((uint32_t)(0x1UL << MXC_F_UART_INT_EN_BREAK_POS)) /**< INT_EN_BREAK Mask */

 #define MXC_F_UART_INT_EN_RX_TO_POS                    8 /**< INT_EN_RX_TO Position */
 #define MXC_F_UART_INT_EN_RX_TO                        ((uint32_t)(0x1UL << MXC_F_UART_INT_EN_RX_TO_POS)) /**< INT_EN_RX_TO Mask */

 #define MXC_F_UART_INT_EN_LAST_BREAK_POS               9 /**< INT_EN_LAST_BREAK Position */
 #define MXC_F_UART_INT_EN_LAST_BREAK                   ((uint32_t)(0x1UL << MXC_F_UART_INT_EN_LAST_BREAK_POS)) /**< INT_EN_LAST_BREAK Mask */

/**@} end of group UART_INT_EN_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_INT_FL UART_INT_FL
 * @brief    Interrupt Status Flags.
 * @{
 */
 #define MXC_F_UART_INT_FL_FRAME_POS                    0 /**< INT_FL_FRAME Position */
 #define MXC_F_UART_INT_FL_FRAME                        ((uint32_t)(0x1UL << MXC_F_UART_INT_FL_FRAME_POS)) /**< INT_FL_FRAME Mask */

 #define MXC_F_UART_INT_FL_PARITY_POS                   1 /**< INT_FL_PARITY Position */
 #define MXC_F_UART_INT_FL_PARITY                       ((uint32_t)(0x1UL << MXC_F_UART_INT_FL_PARITY_POS)) /**< INT_FL_PARITY Mask */

 #define MXC_F_UART_INT_FL_CTS_POS                      2 /**< INT_FL_CTS Position */
 #define MXC_F_UART_INT_FL_CTS                          ((uint32_t)(0x1UL << MXC_F_UART_INT_FL_CTS_POS)) /**< INT_FL_CTS Mask */

 #define MXC_F_UART_INT_FL_RX_OVR_POS                   3 /**< INT_FL_RX_OVR Position */
 #define MXC_F_UART_INT_FL_RX_OVR                       ((uint32_t)(0x1UL << MXC_F_UART_INT_FL_RX_OVR_POS)) /**< INT_FL_RX_OVR Mask */

 #define MXC_F_UART_INT_FL_RX_FIFO_LVL_POS              4 /**< INT_FL_RX_FIFO_LVL Position */
 #define MXC_F_UART_INT_FL_RX_FIFO_LVL                  ((uint32_t)(0x1UL << MXC_F_UART_INT_FL_RX_FIFO_LVL_POS)) /**< INT_FL_RX_FIFO_LVL Mask */

 #define MXC_F_UART_INT_FL_TX_FIFO_AE_POS               5 /**< INT_FL_TX_FIFO_AE Position */
 #define MXC_F_UART_INT_FL_TX_FIFO_AE                   ((uint32_t)(0x1UL << MXC_F_UART_INT_FL_TX_FIFO_AE_POS)) /**< INT_FL_TX_FIFO_AE Mask */

 #define MXC_F_UART_INT_FL_TX_FIFO_LVL_POS              6 /**< INT_FL_TX_FIFO_LVL Position */
 #define MXC_F_UART_INT_FL_TX_FIFO_LVL                  ((uint32_t)(0x1UL << MXC_F_UART_INT_FL_TX_FIFO_LVL_POS)) /**< INT_FL_TX_FIFO_LVL Mask */

 #define MXC_F_UART_INT_FL_BREAK_POS                    7 /**< INT_FL_BREAK Position */
 #define MXC_F_UART_INT_FL_BREAK                        ((uint32_t)(0x1UL << MXC_F_UART_INT_FL_BREAK_POS)) /**< INT_FL_BREAK Mask */

 #define MXC_F_UART_INT_FL_RX_TO_POS                    8 /**< INT_FL_RX_TO Position */
 #define MXC_F_UART_INT_FL_RX_TO                        ((uint32_t)(0x1UL << MXC_F_UART_INT_FL_RX_TO_POS)) /**< INT_FL_RX_TO Mask */

 #define MXC_F_UART_INT_FL_LAST_BREAK_POS               9 /**< INT_FL_LAST_BREAK Position */
 #define MXC_F_UART_INT_FL_LAST_BREAK                   ((uint32_t)(0x1UL << MXC_F_UART_INT_FL_LAST_BREAK_POS)) /**< INT_FL_LAST_BREAK Mask */

/**@} end of group UART_INT_FL_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_BAUD0 UART_BAUD0
 * @brief    Baud rate register. Integer portion.
 * @{
 */
 #define MXC_F_UART_BAUD0_IBAUD_POS                     0 /**< BAUD0_IBAUD Position */
 #define MXC_F_UART_BAUD0_IBAUD                         ((uint32_t)(0xFFFUL << MXC_F_UART_BAUD0_IBAUD_POS)) /**< BAUD0_IBAUD Mask */

 #define MXC_F_UART_BAUD0_CLKDIV_POS                    16 /**< BAUD0_CLKDIV Position */
 #define MXC_F_UART_BAUD0_CLKDIV                        ((uint32_t)(0x7UL << MXC_F_UART_BAUD0_CLKDIV_POS)) /**< BAUD0_CLKDIV Mask */
 #define MXC_V_UART_BAUD0_CLKDIV_128                    ((uint32_t)0x0UL) /**< BAUD0_CLKDIV_128 Value */
 #define MXC_S_UART_BAUD0_CLKDIV_128                    (MXC_V_UART_BAUD0_CLKDIV_128 << MXC_F_UART_BAUD0_CLKDIV_POS) /**< BAUD0_CLKDIV_128 Setting */
 #define MXC_V_UART_BAUD0_CLKDIV_64                     ((uint32_t)0x1UL) /**< BAUD0_CLKDIV_64 Value */
 #define MXC_S_UART_BAUD0_CLKDIV_64                     (MXC_V_UART_BAUD0_CLKDIV_64 << MXC_F_UART_BAUD0_CLKDIV_POS) /**< BAUD0_CLKDIV_64 Setting */
 #define MXC_V_UART_BAUD0_CLKDIV_32                     ((uint32_t)0x2UL) /**< BAUD0_CLKDIV_32 Value */
 #define MXC_S_UART_BAUD0_CLKDIV_32                     (MXC_V_UART_BAUD0_CLKDIV_32 << MXC_F_UART_BAUD0_CLKDIV_POS) /**< BAUD0_CLKDIV_32 Setting */
 #define MXC_V_UART_BAUD0_CLKDIV_16                     ((uint32_t)0x3UL) /**< BAUD0_CLKDIV_16 Value */
 #define MXC_S_UART_BAUD0_CLKDIV_16                     (MXC_V_UART_BAUD0_CLKDIV_16 << MXC_F_UART_BAUD0_CLKDIV_POS) /**< BAUD0_CLKDIV_16 Setting */
 #define MXC_V_UART_BAUD0_CLKDIV_8                      ((uint32_t)0x4UL) /**< BAUD0_CLKDIV_8 Value */
 #define MXC_S_UART_BAUD0_CLKDIV_8                      (MXC_V_UART_BAUD0_CLKDIV_8 << MXC_F_UART_BAUD0_CLKDIV_POS) /**< BAUD0_CLKDIV_8 Setting */

/**@} end of group UART_BAUD0_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_BAUD1 UART_BAUD1
 * @brief    Baud rate register. Decimal Setting.
 * @{
 */
 #define MXC_F_UART_BAUD1_DBAUD_POS                     0 /**< BAUD1_DBAUD Position */
 #define MXC_F_UART_BAUD1_DBAUD                         ((uint32_t)(0xFFFUL << MXC_F_UART_BAUD1_DBAUD_POS)) /**< BAUD1_DBAUD Mask */

/**@} end of group UART_BAUD1_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_FIFO UART_FIFO
 * @brief    FIFO Data buffer.
 * @{
 */
 #define MXC_F_UART_FIFO_FIFO_POS                       0 /**< FIFO_FIFO Position */
 #define MXC_F_UART_FIFO_FIFO                           ((uint32_t)(0xFFUL << MXC_F_UART_FIFO_FIFO_POS)) /**< FIFO_FIFO Mask */

/**@} end of group UART_FIFO_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_DMA UART_DMA
 * @brief    DMA Configuration.
 * @{
 */
 #define MXC_F_UART_DMA_TXDMA_EN_POS                    0 /**< DMA_TXDMA_EN Position */
 #define MXC_F_UART_DMA_TXDMA_EN                        ((uint32_t)(0x1UL << MXC_F_UART_DMA_TXDMA_EN_POS)) /**< DMA_TXDMA_EN Mask */

 #define MXC_F_UART_DMA_RXDMA_EN_POS                    1 /**< DMA_RXDMA_EN Position */
 #define MXC_F_UART_DMA_RXDMA_EN                        ((uint32_t)(0x1UL << MXC_F_UART_DMA_RXDMA_EN_POS)) /**< DMA_RXDMA_EN Mask */

 #define MXC_F_UART_DMA_TXDMA_LVL_POS                   8 /**< DMA_TXDMA_LVL Position */
 #define MXC_F_UART_DMA_TXDMA_LVL                       ((uint32_t)(0x3FUL << MXC_F_UART_DMA_TXDMA_LVL_POS)) /**< DMA_TXDMA_LVL Mask */

 #define MXC_F_UART_DMA_RXDMA_LVL_POS                   16 /**< DMA_RXDMA_LVL Position */
 #define MXC_F_UART_DMA_RXDMA_LVL                       ((uint32_t)(0x3FUL << MXC_F_UART_DMA_RXDMA_LVL_POS)) /**< DMA_RXDMA_LVL Mask */

/**@} end of group UART_DMA_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_TXFIFO UART_TXFIFO
 * @brief    Transmit FIFO Status register.
 * @{
 */
 #define MXC_F_UART_TXFIFO_DATA_POS                     0 /**< TXFIFO_DATA Position */
 #define MXC_F_UART_TXFIFO_DATA                         ((uint32_t)(0x7FUL << MXC_F_UART_TXFIFO_DATA_POS)) /**< TXFIFO_DATA Mask */

/**@} end of group UART_TXFIFO_Register */

#ifdef __cplusplus
}
#endif

#endif /* _UART_REGS_H_ */
