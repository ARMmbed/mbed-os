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

#ifndef _UART_REVA_REGS_H_
#define _UART_REVA_REGS_H_

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
    __IO uint32_t ctrl;                 /**< <tt>\b 0x00:</tt> UART CTRL Register */
    __IO uint32_t thresh_ctrl;          /**< <tt>\b 0x04:</tt> UART THRESH_CTRL Register */
    __I  uint32_t status;               /**< <tt>\b 0x08:</tt> UART STATUS Register */
    __IO uint32_t int_en;               /**< <tt>\b 0x0C:</tt> UART INT_EN Register */
    __IO uint32_t int_fl;               /**< <tt>\b 0x10:</tt> UART INT_FL Register */
    __IO uint32_t baud0;                /**< <tt>\b 0x14:</tt> UART BAUD0 Register */
    __IO uint32_t baud1;                /**< <tt>\b 0x18:</tt> UART BAUD1 Register */
    __IO uint32_t fifo;                 /**< <tt>\b 0x1C:</tt> UART FIFO Register */
    __IO uint32_t dma;                  /**< <tt>\b 0x20:</tt> UART DMA Register */
    __IO uint32_t tx_fifo;              /**< <tt>\b 0x24:</tt> UART TX_FIFO Register */
} mxc_uart_reva_regs_t;

/* Register offsets for module UART */
/**
 * @ingroup    uart_registers
 * @defgroup   UART_Register_Offsets Register Offsets
 * @brief      UART Peripheral Register Offsets from the UART Base Peripheral Address. 
 * @{
 */
 #define MXC_R_UART_REVA_CTRL                    ((uint32_t)0x00000000UL) /**< Offset from UART Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_UART_REVA_THRESH_CTRL             ((uint32_t)0x00000004UL) /**< Offset from UART Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_UART_REVA_STATUS                  ((uint32_t)0x00000008UL) /**< Offset from UART Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_UART_REVA_INT_EN                  ((uint32_t)0x0000000CUL) /**< Offset from UART Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_UART_REVA_INT_FL                  ((uint32_t)0x00000010UL) /**< Offset from UART Base Address: <tt> 0x0010</tt> */ 
 #define MXC_R_UART_REVA_BAUD0                   ((uint32_t)0x00000014UL) /**< Offset from UART Base Address: <tt> 0x0014</tt> */ 
 #define MXC_R_UART_REVA_BAUD1                   ((uint32_t)0x00000018UL) /**< Offset from UART Base Address: <tt> 0x0018</tt> */ 
 #define MXC_R_UART_REVA_FIFO                    ((uint32_t)0x0000001CUL) /**< Offset from UART Base Address: <tt> 0x001C</tt> */ 
 #define MXC_R_UART_REVA_DMA                     ((uint32_t)0x00000020UL) /**< Offset from UART Base Address: <tt> 0x0020</tt> */ 
 #define MXC_R_UART_REVA_TX_FIFO                 ((uint32_t)0x00000024UL) /**< Offset from UART Base Address: <tt> 0x0024</tt> */ 
/**@} end of group uart_registers */

/**
 * @ingroup  uart_registers
 * @defgroup UART_CTRL UART_CTRL
 * @brief    Control Register.
 * @{
 */
 #define MXC_F_UART_REVA_CTRL_ENABLE_POS                     0 /**< CTRL_ENABLE Position */
 #define MXC_F_UART_REVA_CTRL_ENABLE                         ((uint32_t)(0x1UL << MXC_F_UART_REVA_CTRL_ENABLE_POS)) /**< CTRL_ENABLE Mask */

 #define MXC_F_UART_REVA_CTRL_PARITY_EN_POS                  1 /**< CTRL_PARITY_EN Position */
 #define MXC_F_UART_REVA_CTRL_PARITY_EN                      ((uint32_t)(0x1UL << MXC_F_UART_REVA_CTRL_PARITY_EN_POS)) /**< CTRL_PARITY_EN Mask */

 #define MXC_F_UART_REVA_CTRL_PARITY_POS                     2 /**< CTRL_PARITY Position */
 #define MXC_F_UART_REVA_CTRL_PARITY                         ((uint32_t)(0x3UL << MXC_F_UART_REVA_CTRL_PARITY_POS)) /**< CTRL_PARITY Mask */
 #define MXC_V_UART_REVA_CTRL_PARITY_EVEN                    ((uint32_t)0x0UL) /**< CTRL_PARITY_EVEN Value */
 #define MXC_S_UART_REVA_CTRL_PARITY_EVEN                    (MXC_V_UART_REVA_CTRL_PARITY_EVEN << MXC_F_UART_REVA_CTRL_PARITY_POS) /**< CTRL_PARITY_EVEN Setting */
 #define MXC_V_UART_REVA_CTRL_PARITY_ODD                     ((uint32_t)0x1UL) /**< CTRL_PARITY_ODD Value */
 #define MXC_S_UART_REVA_CTRL_PARITY_ODD                     (MXC_V_UART_REVA_CTRL_PARITY_ODD << MXC_F_UART_REVA_CTRL_PARITY_POS) /**< CTRL_PARITY_ODD Setting */
 #define MXC_V_UART_REVA_CTRL_PARITY_MARK                    ((uint32_t)0x2UL) /**< CTRL_PARITY_MARK Value */
 #define MXC_S_UART_REVA_CTRL_PARITY_MARK                    (MXC_V_UART_REVA_CTRL_PARITY_MARK << MXC_F_UART_REVA_CTRL_PARITY_POS) /**< CTRL_PARITY_MARK Setting */
 #define MXC_V_UART_REVA_CTRL_PARITY_SPACE                   ((uint32_t)0x3UL) /**< CTRL_PARITY_SPACE Value */
 #define MXC_S_UART_REVA_CTRL_PARITY_SPACE                   (MXC_V_UART_REVA_CTRL_PARITY_SPACE << MXC_F_UART_REVA_CTRL_PARITY_POS) /**< CTRL_PARITY_SPACE Setting */

 #define MXC_F_UART_REVA_CTRL_PARMD_POS                      4 /**< CTRL_PARMD Position */
 #define MXC_F_UART_REVA_CTRL_PARMD                          ((uint32_t)(0x1UL << MXC_F_UART_REVA_CTRL_PARMD_POS)) /**< CTRL_PARMD Mask */

 #define MXC_F_UART_REVA_CTRL_TX_FLUSH_POS                   5 /**< CTRL_TX_FLUSH Position */
 #define MXC_F_UART_REVA_CTRL_TX_FLUSH                       ((uint32_t)(0x1UL << MXC_F_UART_REVA_CTRL_TX_FLUSH_POS)) /**< CTRL_TX_FLUSH Mask */

 #define MXC_F_UART_REVA_CTRL_RX_FLUSH_POS                   6 /**< CTRL_RX_FLUSH Position */
 #define MXC_F_UART_REVA_CTRL_RX_FLUSH                       ((uint32_t)(0x1UL << MXC_F_UART_REVA_CTRL_RX_FLUSH_POS)) /**< CTRL_RX_FLUSH Mask */

 #define MXC_F_UART_REVA_CTRL_BITACC_POS                     7 /**< CTRL_BITACC Position */
 #define MXC_F_UART_REVA_CTRL_BITACC                         ((uint32_t)(0x1UL << MXC_F_UART_REVA_CTRL_BITACC_POS)) /**< CTRL_BITACC Mask */

 #define MXC_F_UART_REVA_CTRL_CHAR_SIZE_POS                  8 /**< CTRL_CHAR_SIZE Position */
 #define MXC_F_UART_REVA_CTRL_CHAR_SIZE                      ((uint32_t)(0x3UL << MXC_F_UART_REVA_CTRL_CHAR_SIZE_POS)) /**< CTRL_CHAR_SIZE Mask */
 #define MXC_V_UART_REVA_CTRL_CHAR_SIZE_5                    ((uint32_t)0x0UL) /**< CTRL_CHAR_SIZE_5 Value */
 #define MXC_S_UART_REVA_CTRL_CHAR_SIZE_5                    (MXC_V_UART_REVA_CTRL_CHAR_SIZE_5 << MXC_F_UART_REVA_CTRL_CHAR_SIZE_POS) /**< CTRL_CHAR_SIZE_5 Setting */
 #define MXC_V_UART_REVA_CTRL_CHAR_SIZE_6                    ((uint32_t)0x1UL) /**< CTRL_CHAR_SIZE_6 Value */
 #define MXC_S_UART_REVA_CTRL_CHAR_SIZE_6                    (MXC_V_UART_REVA_CTRL_CHAR_SIZE_6 << MXC_F_UART_REVA_CTRL_CHAR_SIZE_POS) /**< CTRL_CHAR_SIZE_6 Setting */
 #define MXC_V_UART_REVA_CTRL_CHAR_SIZE_7                    ((uint32_t)0x2UL) /**< CTRL_CHAR_SIZE_7 Value */
 #define MXC_S_UART_REVA_CTRL_CHAR_SIZE_7                    (MXC_V_UART_REVA_CTRL_CHAR_SIZE_7 << MXC_F_UART_REVA_CTRL_CHAR_SIZE_POS) /**< CTRL_CHAR_SIZE_7 Setting */
 #define MXC_V_UART_REVA_CTRL_CHAR_SIZE_8                    ((uint32_t)0x3UL) /**< CTRL_CHAR_SIZE_8 Value */
 #define MXC_S_UART_REVA_CTRL_CHAR_SIZE_8                    (MXC_V_UART_REVA_CTRL_CHAR_SIZE_8 << MXC_F_UART_REVA_CTRL_CHAR_SIZE_POS) /**< CTRL_CHAR_SIZE_8 Setting */

 #define MXC_F_UART_REVA_CTRL_STOPBITS_POS                   10 /**< CTRL_STOPBITS Position */
 #define MXC_F_UART_REVA_CTRL_STOPBITS                       ((uint32_t)(0x1UL << MXC_F_UART_REVA_CTRL_STOPBITS_POS)) /**< CTRL_STOPBITS Mask */

 #define MXC_F_UART_REVA_CTRL_FLOW_CTRL_POS                  11 /**< CTRL_FLOW_CTRL Position */
 #define MXC_F_UART_REVA_CTRL_FLOW_CTRL                      ((uint32_t)(0x1UL << MXC_F_UART_REVA_CTRL_FLOW_CTRL_POS)) /**< CTRL_FLOW_CTRL Mask */

 #define MXC_F_UART_REVA_CTRL_FLOW_POL_POS                   12 /**< CTRL_FLOW_POL Position */
 #define MXC_F_UART_REVA_CTRL_FLOW_POL                       ((uint32_t)(0x1UL << MXC_F_UART_REVA_CTRL_FLOW_POL_POS)) /**< CTRL_FLOW_POL Mask */

 #define MXC_F_UART_REVA_CTRL_NULL_MODEM_POS                 13 /**< CTRL_NULL_MODEM Position */
 #define MXC_F_UART_REVA_CTRL_NULL_MODEM                     ((uint32_t)(0x1UL << MXC_F_UART_REVA_CTRL_NULL_MODEM_POS)) /**< CTRL_NULL_MODEM Mask */

 #define MXC_F_UART_REVA_CTRL_BREAK_POS                      14 /**< CTRL_BREAK Position */
 #define MXC_F_UART_REVA_CTRL_BREAK                          ((uint32_t)(0x1UL << MXC_F_UART_REVA_CTRL_BREAK_POS)) /**< CTRL_BREAK Mask */

 #define MXC_F_UART_REVA_CTRL_CLKSEL_POS                     15 /**< CTRL_CLKSEL Position */
 #define MXC_F_UART_REVA_CTRL_CLKSEL                         ((uint32_t)(0x1UL << MXC_F_UART_REVA_CTRL_CLKSEL_POS)) /**< CTRL_CLKSEL Mask */

 #define MXC_F_UART_REVA_CTRL_RX_TO_POS                      16 /**< CTRL_RX_TO Position */
 #define MXC_F_UART_REVA_CTRL_RX_TO                          ((uint32_t)(0xFFUL << MXC_F_UART_REVA_CTRL_RX_TO_POS)) /**< CTRL_RX_TO Mask */

/**@} end of group UART_CTRL_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_THRESH_CTRL UART_THRESH_CTRL
 * @brief    Threshold Control register.
 * @{
 */
 #define MXC_F_UART_REVA_THRESH_CTRL_RX_FIFO_THRESH_POS      0 /**< THRESH_CTRL_RX_FIFO_THRESH Position */
 #define MXC_F_UART_REVA_THRESH_CTRL_RX_FIFO_THRESH          ((uint32_t)(0x3FUL << MXC_F_UART_REVA_THRESH_CTRL_RX_FIFO_THRESH_POS)) /**< THRESH_CTRL_RX_FIFO_THRESH Mask */

 #define MXC_F_UART_REVA_THRESH_CTRL_TX_FIFO_THRESH_POS      8 /**< THRESH_CTRL_TX_FIFO_THRESH Position */
 #define MXC_F_UART_REVA_THRESH_CTRL_TX_FIFO_THRESH          ((uint32_t)(0x3FUL << MXC_F_UART_REVA_THRESH_CTRL_TX_FIFO_THRESH_POS)) /**< THRESH_CTRL_TX_FIFO_THRESH Mask */

 #define MXC_F_UART_REVA_THRESH_CTRL_RTS_FIFO_THRESH_POS     16 /**< THRESH_CTRL_RTS_FIFO_THRESH Position */
 #define MXC_F_UART_REVA_THRESH_CTRL_RTS_FIFO_THRESH         ((uint32_t)(0x3FUL << MXC_F_UART_REVA_THRESH_CTRL_RTS_FIFO_THRESH_POS)) /**< THRESH_CTRL_RTS_FIFO_THRESH Mask */

/**@} end of group UART_THRESH_CTRL_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_STATUS UART_STATUS
 * @brief    Status Register.
 * @{
 */
 #define MXC_F_UART_REVA_STATUS_TX_BUSY_POS                  0 /**< STATUS_TX_BUSY Position */
 #define MXC_F_UART_REVA_STATUS_TX_BUSY                      ((uint32_t)(0x1UL << MXC_F_UART_REVA_STATUS_TX_BUSY_POS)) /**< STATUS_TX_BUSY Mask */

 #define MXC_F_UART_REVA_STATUS_RX_BUSY_POS                  1 /**< STATUS_RX_BUSY Position */
 #define MXC_F_UART_REVA_STATUS_RX_BUSY                      ((uint32_t)(0x1UL << MXC_F_UART_REVA_STATUS_RX_BUSY_POS)) /**< STATUS_RX_BUSY Mask */

 #define MXC_F_UART_REVA_STATUS_PARITY_POS                   2 /**< STATUS_PARITY Position */
 #define MXC_F_UART_REVA_STATUS_PARITY                       ((uint32_t)(0x1UL << MXC_F_UART_REVA_STATUS_PARITY_POS)) /**< STATUS_PARITY Mask */

 #define MXC_F_UART_REVA_STATUS_BREAK_POS                    3 /**< STATUS_BREAK Position */
 #define MXC_F_UART_REVA_STATUS_BREAK                        ((uint32_t)(0x1UL << MXC_F_UART_REVA_STATUS_BREAK_POS)) /**< STATUS_BREAK Mask */

 #define MXC_F_UART_REVA_STATUS_RX_EMPTY_POS                 4 /**< STATUS_RX_EMPTY Position */
 #define MXC_F_UART_REVA_STATUS_RX_EMPTY                     ((uint32_t)(0x1UL << MXC_F_UART_REVA_STATUS_RX_EMPTY_POS)) /**< STATUS_RX_EMPTY Mask */

 #define MXC_F_UART_REVA_STATUS_RX_FULL_POS                  5 /**< STATUS_RX_FULL Position */
 #define MXC_F_UART_REVA_STATUS_RX_FULL                      ((uint32_t)(0x1UL << MXC_F_UART_REVA_STATUS_RX_FULL_POS)) /**< STATUS_RX_FULL Mask */

 #define MXC_F_UART_REVA_STATUS_TX_EMPTY_POS                 6 /**< STATUS_TX_EMPTY Position */
 #define MXC_F_UART_REVA_STATUS_TX_EMPTY                     ((uint32_t)(0x1UL << MXC_F_UART_REVA_STATUS_TX_EMPTY_POS)) /**< STATUS_TX_EMPTY Mask */

 #define MXC_F_UART_REVA_STATUS_TX_FULL_POS                  7 /**< STATUS_TX_FULL Position */
 #define MXC_F_UART_REVA_STATUS_TX_FULL                      ((uint32_t)(0x1UL << MXC_F_UART_REVA_STATUS_TX_FULL_POS)) /**< STATUS_TX_FULL Mask */

 #define MXC_F_UART_REVA_STATUS_RX_FIFO_CNT_POS              8 /**< STATUS_RX_FIFO_CNT Position */
 #define MXC_F_UART_REVA_STATUS_RX_FIFO_CNT                  ((uint32_t)(0x3FUL << MXC_F_UART_REVA_STATUS_RX_FIFO_CNT_POS)) /**< STATUS_RX_FIFO_CNT Mask */

 #define MXC_F_UART_REVA_STATUS_TX_FIFO_CNT_POS              16 /**< STATUS_TX_FIFO_CNT Position */
 #define MXC_F_UART_REVA_STATUS_TX_FIFO_CNT                  ((uint32_t)(0x3FUL << MXC_F_UART_REVA_STATUS_TX_FIFO_CNT_POS)) /**< STATUS_TX_FIFO_CNT Mask */

/**@} end of group UART_STATUS_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_INT_EN UART_INT_EN
 * @brief    Interrupt Enable Register.
 * @{
 */
 #define MXC_F_UART_REVA_INT_EN_RX_FRAME_ERROR_POS           0 /**< INT_EN_RX_FRAME_ERROR Position */
 #define MXC_F_UART_REVA_INT_EN_RX_FRAME_ERROR               ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_EN_RX_FRAME_ERROR_POS)) /**< INT_EN_RX_FRAME_ERROR Mask */

 #define MXC_F_UART_REVA_INT_EN_RX_PARITY_ERROR_POS          1 /**< INT_EN_RX_PARITY_ERROR Position */
 #define MXC_F_UART_REVA_INT_EN_RX_PARITY_ERROR              ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_EN_RX_PARITY_ERROR_POS)) /**< INT_EN_RX_PARITY_ERROR Mask */

 #define MXC_F_UART_REVA_INT_EN_CTS_CHANGE_POS               2 /**< INT_EN_CTS_CHANGE Position */
 #define MXC_F_UART_REVA_INT_EN_CTS_CHANGE                   ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_EN_CTS_CHANGE_POS)) /**< INT_EN_CTS_CHANGE Mask */

 #define MXC_F_UART_REVA_INT_EN_RX_OVERRUN_POS               3 /**< INT_EN_RX_OVERRUN Position */
 #define MXC_F_UART_REVA_INT_EN_RX_OVERRUN                   ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_EN_RX_OVERRUN_POS)) /**< INT_EN_RX_OVERRUN Mask */

 #define MXC_F_UART_REVA_INT_EN_RX_FIFO_THRESH_POS           4 /**< INT_EN_RX_FIFO_THRESH Position */
 #define MXC_F_UART_REVA_INT_EN_RX_FIFO_THRESH               ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_EN_RX_FIFO_THRESH_POS)) /**< INT_EN_RX_FIFO_THRESH Mask */

 #define MXC_F_UART_REVA_INT_EN_TX_FIFO_ALMOST_EMPTY_POS     5 /**< INT_EN_TX_FIFO_ALMOST_EMPTY Position */
 #define MXC_F_UART_REVA_INT_EN_TX_FIFO_ALMOST_EMPTY         ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_EN_TX_FIFO_ALMOST_EMPTY_POS)) /**< INT_EN_TX_FIFO_ALMOST_EMPTY Mask */

 #define MXC_F_UART_REVA_INT_EN_TX_FIFO_THRESH_POS           6 /**< INT_EN_TX_FIFO_THRESH Position */
 #define MXC_F_UART_REVA_INT_EN_TX_FIFO_THRESH               ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_EN_TX_FIFO_THRESH_POS)) /**< INT_EN_TX_FIFO_THRESH Mask */

 #define MXC_F_UART_REVA_INT_EN_BREAK_POS                    7 /**< INT_EN_BREAK Position */
 #define MXC_F_UART_REVA_INT_EN_BREAK                        ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_EN_BREAK_POS)) /**< INT_EN_BREAK Mask */

 #define MXC_F_UART_REVA_INT_EN_RX_TIMEOUT_POS               8 /**< INT_EN_RX_TIMEOUT Position */
 #define MXC_F_UART_REVA_INT_EN_RX_TIMEOUT                   ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_EN_RX_TIMEOUT_POS)) /**< INT_EN_RX_TIMEOUT Mask */

 #define MXC_F_UART_REVA_INT_EN_LAST_BREAK_POS               9 /**< INT_EN_LAST_BREAK Position */
 #define MXC_F_UART_REVA_INT_EN_LAST_BREAK                   ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_EN_LAST_BREAK_POS)) /**< INT_EN_LAST_BREAK Mask */

/**@} end of group UART_INT_EN_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_INT_FL UART_INT_FL
 * @brief    Interrupt Status Flags.
 * @{
 */
 #define MXC_F_UART_REVA_INT_FL_RX_FRAME_ERROR_POS           0 /**< INT_FL_RX_FRAME_ERROR Position */
 #define MXC_F_UART_REVA_INT_FL_RX_FRAME_ERROR               ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_FL_RX_FRAME_ERROR_POS)) /**< INT_FL_RX_FRAME_ERROR Mask */

 #define MXC_F_UART_REVA_INT_FL_RX_PARITY_ERROR_POS          1 /**< INT_FL_RX_PARITY_ERROR Position */
 #define MXC_F_UART_REVA_INT_FL_RX_PARITY_ERROR              ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_FL_RX_PARITY_ERROR_POS)) /**< INT_FL_RX_PARITY_ERROR Mask */

 #define MXC_F_UART_REVA_INT_FL_CTS_CHANGE_POS               2 /**< INT_FL_CTS_CHANGE Position */
 #define MXC_F_UART_REVA_INT_FL_CTS_CHANGE                   ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_FL_CTS_CHANGE_POS)) /**< INT_FL_CTS_CHANGE Mask */

 #define MXC_F_UART_REVA_INT_FL_RX_OVERRUN_POS               3 /**< INT_FL_RX_OVERRUN Position */
 #define MXC_F_UART_REVA_INT_FL_RX_OVERRUN                   ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_FL_RX_OVERRUN_POS)) /**< INT_FL_RX_OVERRUN Mask */

 #define MXC_F_UART_REVA_INT_FL_RX_FIFO_THRESH_POS           4 /**< INT_FL_RX_FIFO_THRESH Position */
 #define MXC_F_UART_REVA_INT_FL_RX_FIFO_THRESH               ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_FL_RX_FIFO_THRESH_POS)) /**< INT_FL_RX_FIFO_THRESH Mask */

 #define MXC_F_UART_REVA_INT_FL_TX_FIFO_ALMOST_EMPTY_POS     5 /**< INT_FL_TX_FIFO_ALMOST_EMPTY Position */
 #define MXC_F_UART_REVA_INT_FL_TX_FIFO_ALMOST_EMPTY         ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_FL_TX_FIFO_ALMOST_EMPTY_POS)) /**< INT_FL_TX_FIFO_ALMOST_EMPTY Mask */

 #define MXC_F_UART_REVA_INT_FL_TX_FIFO_THRESH_POS           6 /**< INT_FL_TX_FIFO_THRESH Position */
 #define MXC_F_UART_REVA_INT_FL_TX_FIFO_THRESH               ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_FL_TX_FIFO_THRESH_POS)) /**< INT_FL_TX_FIFO_THRESH Mask */

 #define MXC_F_UART_REVA_INT_FL_BREAK_POS                    7 /**< INT_FL_BREAK Position */
 #define MXC_F_UART_REVA_INT_FL_BREAK                        ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_FL_BREAK_POS)) /**< INT_FL_BREAK Mask */

 #define MXC_F_UART_REVA_INT_FL_RX_TIMEOUT_POS               8 /**< INT_FL_RX_TIMEOUT Position */
 #define MXC_F_UART_REVA_INT_FL_RX_TIMEOUT                   ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_FL_RX_TIMEOUT_POS)) /**< INT_FL_RX_TIMEOUT Mask */

 #define MXC_F_UART_REVA_INT_FL_LAST_BREAK_POS               9 /**< INT_FL_LAST_BREAK Position */
 #define MXC_F_UART_REVA_INT_FL_LAST_BREAK                   ((uint32_t)(0x1UL << MXC_F_UART_REVA_INT_FL_LAST_BREAK_POS)) /**< INT_FL_LAST_BREAK Mask */

/**@} end of group UART_INT_FL_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_BAUD0 UART_BAUD0
 * @brief    Baud rate register. Integer portion.
 * @{
 */
 #define MXC_F_UART_REVA_BAUD0_IBAUD_POS                     0 /**< BAUD0_IBAUD Position */
 #define MXC_F_UART_REVA_BAUD0_IBAUD                         ((uint32_t)(0xFFFUL << MXC_F_UART_REVA_BAUD0_IBAUD_POS)) /**< BAUD0_IBAUD Mask */

 #define MXC_F_UART_REVA_BAUD0_FACTOR_POS                    16 /**< BAUD0_FACTOR Position */
 #define MXC_F_UART_REVA_BAUD0_FACTOR                        ((uint32_t)(0x3UL << MXC_F_UART_REVA_BAUD0_FACTOR_POS)) /**< BAUD0_FACTOR Mask */
 #define MXC_V_UART_REVA_BAUD0_FACTOR_128                    ((uint32_t)0x0UL) /**< BAUD0_FACTOR_128 Value */
 #define MXC_S_UART_REVA_BAUD0_FACTOR_128                    (MXC_V_UART_REVA_BAUD0_FACTOR_128 << MXC_F_UART_REVA_BAUD0_FACTOR_POS) /**< BAUD0_FACTOR_128 Setting */
 #define MXC_V_UART_REVA_BAUD0_FACTOR_64                     ((uint32_t)0x1UL) /**< BAUD0_FACTOR_64 Value */
 #define MXC_S_UART_REVA_BAUD0_FACTOR_64                     (MXC_V_UART_REVA_BAUD0_FACTOR_64 << MXC_F_UART_REVA_BAUD0_FACTOR_POS) /**< BAUD0_FACTOR_64 Setting */
 #define MXC_V_UART_REVA_BAUD0_FACTOR_32                     ((uint32_t)0x2UL) /**< BAUD0_FACTOR_32 Value */
 #define MXC_S_UART_REVA_BAUD0_FACTOR_32                     (MXC_V_UART_REVA_BAUD0_FACTOR_32 << MXC_F_UART_REVA_BAUD0_FACTOR_POS) /**< BAUD0_FACTOR_32 Setting */
 #define MXC_V_UART_REVA_BAUD0_FACTOR_16                     ((uint32_t)0x3UL) /**< BAUD0_FACTOR_16 Value */
 #define MXC_S_UART_REVA_BAUD0_FACTOR_16                     (MXC_V_UART_REVA_BAUD0_FACTOR_16 << MXC_F_UART_REVA_BAUD0_FACTOR_POS) /**< BAUD0_FACTOR_16 Setting */

/**@} end of group UART_BAUD0_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_BAUD1 UART_BAUD1
 * @brief    Baud rate register. Decimal Setting.
 * @{
 */
 #define MXC_F_UART_REVA_BAUD1_DBAUD_POS                     0 /**< BAUD1_DBAUD Position */
 #define MXC_F_UART_REVA_BAUD1_DBAUD                         ((uint32_t)(0xFFFUL << MXC_F_UART_REVA_BAUD1_DBAUD_POS)) /**< BAUD1_DBAUD Mask */

/**@} end of group UART_BAUD1_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_FIFO UART_FIFO
 * @brief    FIFO Data buffer.
 * @{
 */
 #define MXC_F_UART_REVA_FIFO_FIFO_POS                       0 /**< FIFO_FIFO Position */
 #define MXC_F_UART_REVA_FIFO_FIFO                           ((uint32_t)(0xFFUL << MXC_F_UART_REVA_FIFO_FIFO_POS)) /**< FIFO_FIFO Mask */

/**@} end of group UART_FIFO_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_DMA UART_DMA
 * @brief    DMA Configuration.
 * @{
 */
 #define MXC_F_UART_REVA_DMA_TXDMA_EN_POS                    0 /**< DMA_TXDMA_EN Position */
 #define MXC_F_UART_REVA_DMA_TXDMA_EN                        ((uint32_t)(0x1UL << MXC_F_UART_REVA_DMA_TXDMA_EN_POS)) /**< DMA_TXDMA_EN Mask */

 #define MXC_F_UART_REVA_DMA_RXDMA_EN_POS                    1 /**< DMA_RXDMA_EN Position */
 #define MXC_F_UART_REVA_DMA_RXDMA_EN                        ((uint32_t)(0x1UL << MXC_F_UART_REVA_DMA_RXDMA_EN_POS)) /**< DMA_RXDMA_EN Mask */

 #define MXC_F_UART_REVA_DMA_RXDMA_START_POS                 3 /**< DMA_RXDMA_START Position */
 #define MXC_F_UART_REVA_DMA_RXDMA_START                     ((uint32_t)(0x1UL << MXC_F_UART_REVA_DMA_RXDMA_START_POS)) /**< DMA_RXDMA_START Mask */

 #define MXC_F_UART_REVA_DMA_RXDMA_AUTO_TO_POS               5 /**< DMA_RXDMA_AUTO_TO Position */
 #define MXC_F_UART_REVA_DMA_RXDMA_AUTO_TO                   ((uint32_t)(0x1UL << MXC_F_UART_REVA_DMA_RXDMA_AUTO_TO_POS)) /**< DMA_RXDMA_AUTO_TO Mask */

 #define MXC_F_UART_REVA_DMA_TXDMA_LEVEL_POS                 8 /**< DMA_TXDMA_LEVEL Position */
 #define MXC_F_UART_REVA_DMA_TXDMA_LEVEL                     ((uint32_t)(0x3FUL << MXC_F_UART_REVA_DMA_TXDMA_LEVEL_POS)) /**< DMA_TXDMA_LEVEL Mask */

 #define MXC_F_UART_REVA_DMA_RXDMA_LEVEL_POS                 16 /**< DMA_RXDMA_LEVEL Position */
 #define MXC_F_UART_REVA_DMA_RXDMA_LEVEL                     ((uint32_t)(0x3FUL << MXC_F_UART_REVA_DMA_RXDMA_LEVEL_POS)) /**< DMA_RXDMA_LEVEL Mask */

/**@} end of group UART_DMA_Register */

/**
 * @ingroup  uart_registers
 * @defgroup UART_TX_FIFO UART_TX_FIFO
 * @brief    Transmit FIFO Status register.
 * @{
 */
 #define MXC_F_UART_REVA_TX_FIFO_DATA_POS                    0 /**< TX_FIFO_DATA Position */
 #define MXC_F_UART_REVA_TX_FIFO_DATA                        ((uint32_t)(0x7FUL << MXC_F_UART_REVA_TX_FIFO_DATA_POS)) /**< TX_FIFO_DATA Mask */

/**@} end of group UART_TX_FIFO_Register */

#ifdef __cplusplus
}
#endif

#endif /* _UART_REVA_REGS_H_ */
