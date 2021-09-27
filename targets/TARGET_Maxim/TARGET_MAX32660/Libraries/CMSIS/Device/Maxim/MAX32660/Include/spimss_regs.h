/**
 * @file    spimss_regs.h
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

#ifndef _SPIMSS_REGS_H_
#define _SPIMSS_REGS_H_

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
    __I  uint16_t rsv_0x2;
    __IO uint32_t ctrl;                 /**< <tt>\b 0x04:</tt> SPIMSS CTRL Register */
    __IO uint32_t int_fl;               /**< <tt>\b 0x08:</tt> SPIMSS INT_FL Register */
    __IO uint32_t mode;                 /**< <tt>\b 0x0C:</tt> SPIMSS MODE Register */
    __I  uint32_t rsv_0x10;
    __IO uint32_t brg;                  /**< <tt>\b 0x14:</tt> SPIMSS BRG Register */
    __IO uint32_t dma;                  /**< <tt>\b 0x18:</tt> SPIMSS DMA Register */
    __IO uint32_t i2s_ctrl;             /**< <tt>\b 0x1C:</tt> SPIMSS I2S_CTRL Register */
} mxc_spimss_regs_t;

/* Register offsets for module SPIMSS */
/**
 * @ingroup    spimss_registers
 * @defgroup   SPIMSS_Register_Offsets Register Offsets
 * @brief      SPIMSS Peripheral Register Offsets from the SPIMSS Base Peripheral Address. 
 * @{
 */
 #define MXC_R_SPIMSS_DATA                  ((uint32_t)0x00000000UL) /**< Offset from SPIMSS Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_SPIMSS_CTRL                  ((uint32_t)0x00000004UL) /**< Offset from SPIMSS Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_SPIMSS_INT_FL                ((uint32_t)0x00000008UL) /**< Offset from SPIMSS Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_SPIMSS_MODE                  ((uint32_t)0x0000000CUL) /**< Offset from SPIMSS Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_SPIMSS_BRG                   ((uint32_t)0x00000014UL) /**< Offset from SPIMSS Base Address: <tt> 0x0014</tt> */ 
 #define MXC_R_SPIMSS_DMA                   ((uint32_t)0x00000018UL) /**< Offset from SPIMSS Base Address: <tt> 0x0018</tt> */ 
 #define MXC_R_SPIMSS_I2S_CTRL              ((uint32_t)0x0000001CUL) /**< Offset from SPIMSS Base Address: <tt> 0x001C</tt> */ 
/**@} end of group spimss_registers */

/**
 * @ingroup  spimss_registers
 * @defgroup SPIMSS_DATA SPIMSS_DATA
 * @brief    SPI 16-bit Data Access
 * @{
 */
 #define MXC_F_SPIMSS_DATA_DATA_POS                     0 /**< DATA_DATA Position */
 #define MXC_F_SPIMSS_DATA_DATA                         ((uint16_t)(0xFFFFUL << MXC_F_SPIMSS_DATA_DATA_POS)) /**< DATA_DATA Mask */

/**@} end of group SPIMSS_DATA_Register */

/**
 * @ingroup  spimss_registers
 * @defgroup SPIMSS_CTRL SPIMSS_CTRL
 * @brief    SPI Control Register.
 * @{
 */
 #define MXC_F_SPIMSS_CTRL_ENABLE_POS                   0 /**< CTRL_ENABLE Position */
 #define MXC_F_SPIMSS_CTRL_ENABLE                       ((uint32_t)(0x1UL << MXC_F_SPIMSS_CTRL_ENABLE_POS)) /**< CTRL_ENABLE Mask */

 #define MXC_F_SPIMSS_CTRL_MMEN_POS                     1 /**< CTRL_MMEN Position */
 #define MXC_F_SPIMSS_CTRL_MMEN                         ((uint32_t)(0x1UL << MXC_F_SPIMSS_CTRL_MMEN_POS)) /**< CTRL_MMEN Mask */

 #define MXC_F_SPIMSS_CTRL_WOR_POS                      2 /**< CTRL_WOR Position */
 #define MXC_F_SPIMSS_CTRL_WOR                          ((uint32_t)(0x1UL << MXC_F_SPIMSS_CTRL_WOR_POS)) /**< CTRL_WOR Mask */

 #define MXC_F_SPIMSS_CTRL_CLKPOL_POS                   3 /**< CTRL_CLKPOL Position */
 #define MXC_F_SPIMSS_CTRL_CLKPOL                       ((uint32_t)(0x1UL << MXC_F_SPIMSS_CTRL_CLKPOL_POS)) /**< CTRL_CLKPOL Mask */

 #define MXC_F_SPIMSS_CTRL_PHASE_POS                    4 /**< CTRL_PHASE Position */
 #define MXC_F_SPIMSS_CTRL_PHASE                        ((uint32_t)(0x1UL << MXC_F_SPIMSS_CTRL_PHASE_POS)) /**< CTRL_PHASE Mask */

 #define MXC_F_SPIMSS_CTRL_BIRQ_POS                     5 /**< CTRL_BIRQ Position */
 #define MXC_F_SPIMSS_CTRL_BIRQ                         ((uint32_t)(0x1UL << MXC_F_SPIMSS_CTRL_BIRQ_POS)) /**< CTRL_BIRQ Mask */

 #define MXC_F_SPIMSS_CTRL_STR_POS                      6 /**< CTRL_STR Position */
 #define MXC_F_SPIMSS_CTRL_STR                          ((uint32_t)(0x1UL << MXC_F_SPIMSS_CTRL_STR_POS)) /**< CTRL_STR Mask */

 #define MXC_F_SPIMSS_CTRL_IRQE_POS                     7 /**< CTRL_IRQE Position */
 #define MXC_F_SPIMSS_CTRL_IRQE                         ((uint32_t)(0x1UL << MXC_F_SPIMSS_CTRL_IRQE_POS)) /**< CTRL_IRQE Mask */

/**@} end of group SPIMSS_CTRL_Register */

/**
 * @ingroup  spimss_registers
 * @defgroup SPIMSS_INT_FL SPIMSS_INT_FL
 * @brief    SPI Interrupt Flag Register.
 * @{
 */
 #define MXC_F_SPIMSS_INT_FL_SLAS_POS                   0 /**< INT_FL_SLAS Position */
 #define MXC_F_SPIMSS_INT_FL_SLAS                       ((uint32_t)(0x1UL << MXC_F_SPIMSS_INT_FL_SLAS_POS)) /**< INT_FL_SLAS Mask */

 #define MXC_F_SPIMSS_INT_FL_TXST_POS                   1 /**< INT_FL_TXST Position */
 #define MXC_F_SPIMSS_INT_FL_TXST                       ((uint32_t)(0x1UL << MXC_F_SPIMSS_INT_FL_TXST_POS)) /**< INT_FL_TXST Mask */

 #define MXC_F_SPIMSS_INT_FL_TUND_POS                   2 /**< INT_FL_TUND Position */
 #define MXC_F_SPIMSS_INT_FL_TUND                       ((uint32_t)(0x1UL << MXC_F_SPIMSS_INT_FL_TUND_POS)) /**< INT_FL_TUND Mask */

 #define MXC_F_SPIMSS_INT_FL_ROVR_POS                   3 /**< INT_FL_ROVR Position */
 #define MXC_F_SPIMSS_INT_FL_ROVR                       ((uint32_t)(0x1UL << MXC_F_SPIMSS_INT_FL_ROVR_POS)) /**< INT_FL_ROVR Mask */

 #define MXC_F_SPIMSS_INT_FL_ABT_POS                    4 /**< INT_FL_ABT Position */
 #define MXC_F_SPIMSS_INT_FL_ABT                        ((uint32_t)(0x1UL << MXC_F_SPIMSS_INT_FL_ABT_POS)) /**< INT_FL_ABT Mask */

 #define MXC_F_SPIMSS_INT_FL_COL_POS                    5 /**< INT_FL_COL Position */
 #define MXC_F_SPIMSS_INT_FL_COL                        ((uint32_t)(0x1UL << MXC_F_SPIMSS_INT_FL_COL_POS)) /**< INT_FL_COL Mask */

 #define MXC_F_SPIMSS_INT_FL_TOVR_POS                   6 /**< INT_FL_TOVR Position */
 #define MXC_F_SPIMSS_INT_FL_TOVR                       ((uint32_t)(0x1UL << MXC_F_SPIMSS_INT_FL_TOVR_POS)) /**< INT_FL_TOVR Mask */

 #define MXC_F_SPIMSS_INT_FL_IRQ_POS                    7 /**< INT_FL_IRQ Position */
 #define MXC_F_SPIMSS_INT_FL_IRQ                        ((uint32_t)(0x1UL << MXC_F_SPIMSS_INT_FL_IRQ_POS)) /**< INT_FL_IRQ Mask */

/**@} end of group SPIMSS_INT_FL_Register */

/**
 * @ingroup  spimss_registers
 * @defgroup SPIMSS_MODE SPIMSS_MODE
 * @brief    SPI Mode Register.
 * @{
 */
 #define MXC_F_SPIMSS_MODE_SSV_POS                      0 /**< MODE_SSV Position */
 #define MXC_F_SPIMSS_MODE_SSV                          ((uint32_t)(0x1UL << MXC_F_SPIMSS_MODE_SSV_POS)) /**< MODE_SSV Mask */

 #define MXC_F_SPIMSS_MODE_SS_IO_POS                    1 /**< MODE_SS_IO Position */
 #define MXC_F_SPIMSS_MODE_SS_IO                        ((uint32_t)(0x1UL << MXC_F_SPIMSS_MODE_SS_IO_POS)) /**< MODE_SS_IO Mask */

 #define MXC_F_SPIMSS_MODE_NUMBITS_POS                  2 /**< MODE_NUMBITS Position */
 #define MXC_F_SPIMSS_MODE_NUMBITS                      ((uint32_t)(0xFUL << MXC_F_SPIMSS_MODE_NUMBITS_POS)) /**< MODE_NUMBITS Mask */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS16               ((uint32_t)0x0UL) /**< MODE_NUMBITS_BITS16 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS16               (MXC_V_SPIMSS_MODE_NUMBITS_BITS16 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS16 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS1                ((uint32_t)0x1UL) /**< MODE_NUMBITS_BITS1 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS1                (MXC_V_SPIMSS_MODE_NUMBITS_BITS1 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS1 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS2                ((uint32_t)0x2UL) /**< MODE_NUMBITS_BITS2 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS2                (MXC_V_SPIMSS_MODE_NUMBITS_BITS2 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS2 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS3                ((uint32_t)0x3UL) /**< MODE_NUMBITS_BITS3 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS3                (MXC_V_SPIMSS_MODE_NUMBITS_BITS3 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS3 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS4                ((uint32_t)0x4UL) /**< MODE_NUMBITS_BITS4 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS4                (MXC_V_SPIMSS_MODE_NUMBITS_BITS4 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS4 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS5                ((uint32_t)0x5UL) /**< MODE_NUMBITS_BITS5 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS5                (MXC_V_SPIMSS_MODE_NUMBITS_BITS5 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS5 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS6                ((uint32_t)0x6UL) /**< MODE_NUMBITS_BITS6 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS6                (MXC_V_SPIMSS_MODE_NUMBITS_BITS6 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS6 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS7                ((uint32_t)0x7UL) /**< MODE_NUMBITS_BITS7 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS7                (MXC_V_SPIMSS_MODE_NUMBITS_BITS7 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS7 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS8                ((uint32_t)0x8UL) /**< MODE_NUMBITS_BITS8 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS8                (MXC_V_SPIMSS_MODE_NUMBITS_BITS8 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS8 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS9                ((uint32_t)0x9UL) /**< MODE_NUMBITS_BITS9 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS9                (MXC_V_SPIMSS_MODE_NUMBITS_BITS9 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS9 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS10               ((uint32_t)0xAUL) /**< MODE_NUMBITS_BITS10 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS10               (MXC_V_SPIMSS_MODE_NUMBITS_BITS10 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS10 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS11               ((uint32_t)0xBUL) /**< MODE_NUMBITS_BITS11 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS11               (MXC_V_SPIMSS_MODE_NUMBITS_BITS11 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS11 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS12               ((uint32_t)0xCUL) /**< MODE_NUMBITS_BITS12 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS12               (MXC_V_SPIMSS_MODE_NUMBITS_BITS12 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS12 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS13               ((uint32_t)0xDUL) /**< MODE_NUMBITS_BITS13 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS13               (MXC_V_SPIMSS_MODE_NUMBITS_BITS13 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS13 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS14               ((uint32_t)0xEUL) /**< MODE_NUMBITS_BITS14 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS14               (MXC_V_SPIMSS_MODE_NUMBITS_BITS14 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS14 Setting */
 #define MXC_V_SPIMSS_MODE_NUMBITS_BITS15               ((uint32_t)0xFUL) /**< MODE_NUMBITS_BITS15 Value */
 #define MXC_S_SPIMSS_MODE_NUMBITS_BITS15               (MXC_V_SPIMSS_MODE_NUMBITS_BITS15 << MXC_F_SPIMSS_MODE_NUMBITS_POS) /**< MODE_NUMBITS_BITS15 Setting */

 #define MXC_F_SPIMSS_MODE_TX_LJ_POS                    7 /**< MODE_TX_LJ Position */
 #define MXC_F_SPIMSS_MODE_TX_LJ                        ((uint32_t)(0x1UL << MXC_F_SPIMSS_MODE_TX_LJ_POS)) /**< MODE_TX_LJ Mask */

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
 #define MXC_F_SPIMSS_BRG_DIV_POS                       0 /**< BRG_DIV Position */
 #define MXC_F_SPIMSS_BRG_DIV                           ((uint32_t)(0xFFFFUL << MXC_F_SPIMSS_BRG_DIV_POS)) /**< BRG_DIV Mask */

/**@} end of group SPIMSS_BRG_Register */

/**
 * @ingroup  spimss_registers
 * @defgroup SPIMSS_DMA SPIMSS_DMA
 * @brief    SPI DMA Register.
 * @{
 */
 #define MXC_F_SPIMSS_DMA_TX_FIFO_LVL_POS               0 /**< DMA_TX_FIFO_LVL Position */
 #define MXC_F_SPIMSS_DMA_TX_FIFO_LVL                   ((uint32_t)(0x7UL << MXC_F_SPIMSS_DMA_TX_FIFO_LVL_POS)) /**< DMA_TX_FIFO_LVL Mask */
 #define MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRY1            ((uint32_t)0x0UL) /**< DMA_TX_FIFO_LVL_ENTRY1 Value */
 #define MXC_S_SPIMSS_DMA_TX_FIFO_LVL_ENTRY1            (MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRY1 << MXC_F_SPIMSS_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRY1 Setting */
 #define MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES2          ((uint32_t)0x1UL) /**< DMA_TX_FIFO_LVL_ENTRIES2 Value */
 #define MXC_S_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES2          (MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES2 << MXC_F_SPIMSS_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRIES2 Setting */
 #define MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES3          ((uint32_t)0x2UL) /**< DMA_TX_FIFO_LVL_ENTRIES3 Value */
 #define MXC_S_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES3          (MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES3 << MXC_F_SPIMSS_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRIES3 Setting */
 #define MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES4          ((uint32_t)0x3UL) /**< DMA_TX_FIFO_LVL_ENTRIES4 Value */
 #define MXC_S_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES4          (MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES4 << MXC_F_SPIMSS_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRIES4 Setting */
 #define MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES5          ((uint32_t)0x4UL) /**< DMA_TX_FIFO_LVL_ENTRIES5 Value */
 #define MXC_S_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES5          (MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES5 << MXC_F_SPIMSS_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRIES5 Setting */
 #define MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES6          ((uint32_t)0x5UL) /**< DMA_TX_FIFO_LVL_ENTRIES6 Value */
 #define MXC_S_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES6          (MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES6 << MXC_F_SPIMSS_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRIES6 Setting */
 #define MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES7          ((uint32_t)0x6UL) /**< DMA_TX_FIFO_LVL_ENTRIES7 Value */
 #define MXC_S_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES7          (MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES7 << MXC_F_SPIMSS_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRIES7 Setting */
 #define MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES8          ((uint32_t)0x7UL) /**< DMA_TX_FIFO_LVL_ENTRIES8 Value */
 #define MXC_S_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES8          (MXC_V_SPIMSS_DMA_TX_FIFO_LVL_ENTRIES8 << MXC_F_SPIMSS_DMA_TX_FIFO_LVL_POS) /**< DMA_TX_FIFO_LVL_ENTRIES8 Setting */

 #define MXC_F_SPIMSS_DMA_TX_FIFO_CLR_POS               4 /**< DMA_TX_FIFO_CLR Position */
 #define MXC_F_SPIMSS_DMA_TX_FIFO_CLR                   ((uint32_t)(0x1UL << MXC_F_SPIMSS_DMA_TX_FIFO_CLR_POS)) /**< DMA_TX_FIFO_CLR Mask */

 #define MXC_F_SPIMSS_DMA_TX_FIFO_CNT_POS               8 /**< DMA_TX_FIFO_CNT Position */
 #define MXC_F_SPIMSS_DMA_TX_FIFO_CNT                   ((uint32_t)(0xFUL << MXC_F_SPIMSS_DMA_TX_FIFO_CNT_POS)) /**< DMA_TX_FIFO_CNT Mask */

 #define MXC_F_SPIMSS_DMA_TX_DMA_EN_POS                 15 /**< DMA_TX_DMA_EN Position */
 #define MXC_F_SPIMSS_DMA_TX_DMA_EN                     ((uint32_t)(0x1UL << MXC_F_SPIMSS_DMA_TX_DMA_EN_POS)) /**< DMA_TX_DMA_EN Mask */

 #define MXC_F_SPIMSS_DMA_RX_FIFO_LVL_POS               16 /**< DMA_RX_FIFO_LVL Position */
 #define MXC_F_SPIMSS_DMA_RX_FIFO_LVL                   ((uint32_t)(0x7UL << MXC_F_SPIMSS_DMA_RX_FIFO_LVL_POS)) /**< DMA_RX_FIFO_LVL Mask */
 #define MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRY1            ((uint32_t)0x0UL) /**< DMA_RX_FIFO_LVL_ENTRY1 Value */
 #define MXC_S_SPIMSS_DMA_RX_FIFO_LVL_ENTRY1            (MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRY1 << MXC_F_SPIMSS_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRY1 Setting */
 #define MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES2          ((uint32_t)0x1UL) /**< DMA_RX_FIFO_LVL_ENTRIES2 Value */
 #define MXC_S_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES2          (MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES2 << MXC_F_SPIMSS_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRIES2 Setting */
 #define MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES3          ((uint32_t)0x2UL) /**< DMA_RX_FIFO_LVL_ENTRIES3 Value */
 #define MXC_S_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES3          (MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES3 << MXC_F_SPIMSS_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRIES3 Setting */
 #define MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES4          ((uint32_t)0x3UL) /**< DMA_RX_FIFO_LVL_ENTRIES4 Value */
 #define MXC_S_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES4          (MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES4 << MXC_F_SPIMSS_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRIES4 Setting */
 #define MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES5          ((uint32_t)0x4UL) /**< DMA_RX_FIFO_LVL_ENTRIES5 Value */
 #define MXC_S_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES5          (MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES5 << MXC_F_SPIMSS_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRIES5 Setting */
 #define MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES6          ((uint32_t)0x5UL) /**< DMA_RX_FIFO_LVL_ENTRIES6 Value */
 #define MXC_S_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES6          (MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES6 << MXC_F_SPIMSS_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRIES6 Setting */
 #define MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES7          ((uint32_t)0x6UL) /**< DMA_RX_FIFO_LVL_ENTRIES7 Value */
 #define MXC_S_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES7          (MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES7 << MXC_F_SPIMSS_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRIES7 Setting */
 #define MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES8          ((uint32_t)0x7UL) /**< DMA_RX_FIFO_LVL_ENTRIES8 Value */
 #define MXC_S_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES8          (MXC_V_SPIMSS_DMA_RX_FIFO_LVL_ENTRIES8 << MXC_F_SPIMSS_DMA_RX_FIFO_LVL_POS) /**< DMA_RX_FIFO_LVL_ENTRIES8 Setting */

 #define MXC_F_SPIMSS_DMA_RX_FIFO_CLR_POS               20 /**< DMA_RX_FIFO_CLR Position */
 #define MXC_F_SPIMSS_DMA_RX_FIFO_CLR                   ((uint32_t)(0x1UL << MXC_F_SPIMSS_DMA_RX_FIFO_CLR_POS)) /**< DMA_RX_FIFO_CLR Mask */

 #define MXC_F_SPIMSS_DMA_RX_FIFO_CNT_POS               24 /**< DMA_RX_FIFO_CNT Position */
 #define MXC_F_SPIMSS_DMA_RX_FIFO_CNT                   ((uint32_t)(0xFUL << MXC_F_SPIMSS_DMA_RX_FIFO_CNT_POS)) /**< DMA_RX_FIFO_CNT Mask */

 #define MXC_F_SPIMSS_DMA_RX_DMA_EN_POS                 31 /**< DMA_RX_DMA_EN Position */
 #define MXC_F_SPIMSS_DMA_RX_DMA_EN                     ((uint32_t)(0x1UL << MXC_F_SPIMSS_DMA_RX_DMA_EN_POS)) /**< DMA_RX_DMA_EN Mask */

/**@} end of group SPIMSS_DMA_Register */

/**
 * @ingroup  spimss_registers
 * @defgroup SPIMSS_I2S_CTRL SPIMSS_I2S_CTRL
 * @brief    I2S Control Register.
 * @{
 */
 #define MXC_F_SPIMSS_I2S_CTRL_I2S_EN_POS               0 /**< I2S_CTRL_I2S_EN Position */
 #define MXC_F_SPIMSS_I2S_CTRL_I2S_EN                   ((uint32_t)(0x1UL << MXC_F_SPIMSS_I2S_CTRL_I2S_EN_POS)) /**< I2S_CTRL_I2S_EN Mask */

 #define MXC_F_SPIMSS_I2S_CTRL_I2S_MUTE_POS             1 /**< I2S_CTRL_I2S_MUTE Position */
 #define MXC_F_SPIMSS_I2S_CTRL_I2S_MUTE                 ((uint32_t)(0x1UL << MXC_F_SPIMSS_I2S_CTRL_I2S_MUTE_POS)) /**< I2S_CTRL_I2S_MUTE Mask */

 #define MXC_F_SPIMSS_I2S_CTRL_I2S_PAUSE_POS            2 /**< I2S_CTRL_I2S_PAUSE Position */
 #define MXC_F_SPIMSS_I2S_CTRL_I2S_PAUSE                ((uint32_t)(0x1UL << MXC_F_SPIMSS_I2S_CTRL_I2S_PAUSE_POS)) /**< I2S_CTRL_I2S_PAUSE Mask */

 #define MXC_F_SPIMSS_I2S_CTRL_I2S_MONO_POS             3 /**< I2S_CTRL_I2S_MONO Position */
 #define MXC_F_SPIMSS_I2S_CTRL_I2S_MONO                 ((uint32_t)(0x1UL << MXC_F_SPIMSS_I2S_CTRL_I2S_MONO_POS)) /**< I2S_CTRL_I2S_MONO Mask */

 #define MXC_F_SPIMSS_I2S_CTRL_I2S_LJ_POS               4 /**< I2S_CTRL_I2S_LJ Position */
 #define MXC_F_SPIMSS_I2S_CTRL_I2S_LJ                   ((uint32_t)(0x1UL << MXC_F_SPIMSS_I2S_CTRL_I2S_LJ_POS)) /**< I2S_CTRL_I2S_LJ Mask */

/**@} end of group SPIMSS_I2S_CTRL_Register */

#ifdef __cplusplus
}
#endif

#endif /* _SPIMSS_REGS_H_ */
