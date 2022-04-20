/**
 * @file    i2s_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the I2S Peripheral Module.
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

#ifndef _I2S_REGS_H_
#define _I2S_REGS_H_

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
 * @ingroup     i2s
 * @defgroup    i2s_registers I2S_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the I2S Peripheral Module.
 * @details Inter-IC Sound Interface.
 */

/**
 * @ingroup i2s_registers
 * Structure type to access the I2S Registers.
 */
typedef struct {
    __IO uint32_t ctrl0ch0;             /**< <tt>\b 0x00:</tt> I2S CTRL0CH0 Register */
    __I  uint32_t rsv_0x4_0xf[3];
    __IO uint32_t ctrl1ch0;             /**< <tt>\b 0x10:</tt> I2S CTRL1CH0 Register */
    __I  uint32_t rsv_0x14_0x2f[7];
    __IO uint32_t dmach0;               /**< <tt>\b 0x30:</tt> I2S DMACH0 Register */
    __I  uint32_t rsv_0x34_0x3f[3];
    __IO uint32_t fifoch0;              /**< <tt>\b 0x40:</tt> I2S FIFOCH0 Register */
    __I  uint32_t rsv_0x44_0x4f[3];
    __IO uint32_t intfl;                /**< <tt>\b 0x50:</tt> I2S INTFL Register */
    __IO uint32_t inten;                /**< <tt>\b 0x54:</tt> I2S INTEN Register */
    __IO uint32_t extsetup;             /**< <tt>\b 0x58:</tt> I2S EXTSETUP Register */
} mxc_i2s_regs_t;

/* Register offsets for module I2S */
/**
 * @ingroup    i2s_registers
 * @defgroup   I2S_Register_Offsets Register Offsets
 * @brief      I2S Peripheral Register Offsets from the I2S Base Peripheral Address. 
 * @{
 */
 #define MXC_R_I2S_CTRL0CH0                 ((uint32_t)0x00000000UL) /**< Offset from I2S Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_I2S_CTRL1CH0                 ((uint32_t)0x00000010UL) /**< Offset from I2S Base Address: <tt> 0x0010</tt> */ 
 #define MXC_R_I2S_DMACH0                   ((uint32_t)0x00000030UL) /**< Offset from I2S Base Address: <tt> 0x0030</tt> */ 
 #define MXC_R_I2S_FIFOCH0                  ((uint32_t)0x00000040UL) /**< Offset from I2S Base Address: <tt> 0x0040</tt> */ 
 #define MXC_R_I2S_INTFL                    ((uint32_t)0x00000050UL) /**< Offset from I2S Base Address: <tt> 0x0050</tt> */ 
 #define MXC_R_I2S_INTEN                    ((uint32_t)0x00000054UL) /**< Offset from I2S Base Address: <tt> 0x0054</tt> */ 
 #define MXC_R_I2S_EXTSETUP                 ((uint32_t)0x00000058UL) /**< Offset from I2S Base Address: <tt> 0x0058</tt> */ 
/**@} end of group i2s_registers */

/**
 * @ingroup  i2s_registers
 * @defgroup I2S_CTRL0CH0 I2S_CTRL0CH0
 * @brief    Global mode channel.
 * @{
 */
 #define MXC_F_I2S_CTRL0CH0_LSB_FIRST_POS               1 /**< CTRL0CH0_LSB_FIRST Position */
 #define MXC_F_I2S_CTRL0CH0_LSB_FIRST                   ((uint32_t)(0x1UL << MXC_F_I2S_CTRL0CH0_LSB_FIRST_POS)) /**< CTRL0CH0_LSB_FIRST Mask */

 #define MXC_F_I2S_CTRL0CH0_CH_MODE_POS                 6 /**< CTRL0CH0_CH_MODE Position */
 #define MXC_F_I2S_CTRL0CH0_CH_MODE                     ((uint32_t)(0x3UL << MXC_F_I2S_CTRL0CH0_CH_MODE_POS)) /**< CTRL0CH0_CH_MODE Mask */

 #define MXC_F_I2S_CTRL0CH0_WS_POL_POS                  8 /**< CTRL0CH0_WS_POL Position */
 #define MXC_F_I2S_CTRL0CH0_WS_POL                      ((uint32_t)(0x1UL << MXC_F_I2S_CTRL0CH0_WS_POL_POS)) /**< CTRL0CH0_WS_POL Mask */

 #define MXC_F_I2S_CTRL0CH0_MSB_LOC_POS                 9 /**< CTRL0CH0_MSB_LOC Position */
 #define MXC_F_I2S_CTRL0CH0_MSB_LOC                     ((uint32_t)(0x1UL << MXC_F_I2S_CTRL0CH0_MSB_LOC_POS)) /**< CTRL0CH0_MSB_LOC Mask */

 #define MXC_F_I2S_CTRL0CH0_ALIGN_POS                   10 /**< CTRL0CH0_ALIGN Position */
 #define MXC_F_I2S_CTRL0CH0_ALIGN                       ((uint32_t)(0x1UL << MXC_F_I2S_CTRL0CH0_ALIGN_POS)) /**< CTRL0CH0_ALIGN Mask */

 #define MXC_F_I2S_CTRL0CH0_EXT_SEL_POS                 11 /**< CTRL0CH0_EXT_SEL Position */
 #define MXC_F_I2S_CTRL0CH0_EXT_SEL                     ((uint32_t)(0x1UL << MXC_F_I2S_CTRL0CH0_EXT_SEL_POS)) /**< CTRL0CH0_EXT_SEL Mask */

 #define MXC_F_I2S_CTRL0CH0_STEREO_POS                  12 /**< CTRL0CH0_STEREO Position */
 #define MXC_F_I2S_CTRL0CH0_STEREO                      ((uint32_t)(0x3UL << MXC_F_I2S_CTRL0CH0_STEREO_POS)) /**< CTRL0CH0_STEREO Mask */

 #define MXC_F_I2S_CTRL0CH0_WSIZE_POS                   14 /**< CTRL0CH0_WSIZE Position */
 #define MXC_F_I2S_CTRL0CH0_WSIZE                       ((uint32_t)(0x3UL << MXC_F_I2S_CTRL0CH0_WSIZE_POS)) /**< CTRL0CH0_WSIZE Mask */

 #define MXC_F_I2S_CTRL0CH0_TX_EN_POS                   16 /**< CTRL0CH0_TX_EN Position */
 #define MXC_F_I2S_CTRL0CH0_TX_EN                       ((uint32_t)(0x1UL << MXC_F_I2S_CTRL0CH0_TX_EN_POS)) /**< CTRL0CH0_TX_EN Mask */

 #define MXC_F_I2S_CTRL0CH0_RX_EN_POS                   17 /**< CTRL0CH0_RX_EN Position */
 #define MXC_F_I2S_CTRL0CH0_RX_EN                       ((uint32_t)(0x1UL << MXC_F_I2S_CTRL0CH0_RX_EN_POS)) /**< CTRL0CH0_RX_EN Mask */

 #define MXC_F_I2S_CTRL0CH0_FLUSH_POS                   18 /**< CTRL0CH0_FLUSH Position */
 #define MXC_F_I2S_CTRL0CH0_FLUSH                       ((uint32_t)(0x1UL << MXC_F_I2S_CTRL0CH0_FLUSH_POS)) /**< CTRL0CH0_FLUSH Mask */

 #define MXC_F_I2S_CTRL0CH0_RST_POS                     19 /**< CTRL0CH0_RST Position */
 #define MXC_F_I2S_CTRL0CH0_RST                         ((uint32_t)(0x1UL << MXC_F_I2S_CTRL0CH0_RST_POS)) /**< CTRL0CH0_RST Mask */

 #define MXC_F_I2S_CTRL0CH0_FIFO_LSB_POS                19 /**< CTRL0CH0_FIFO_LSB Position */
 #define MXC_F_I2S_CTRL0CH0_FIFO_LSB                    ((uint32_t)(0x1UL << MXC_F_I2S_CTRL0CH0_FIFO_LSB_POS)) /**< CTRL0CH0_FIFO_LSB Mask */

 #define MXC_F_I2S_CTRL0CH0_RX_THD_VAL_POS              24 /**< CTRL0CH0_RX_THD_VAL Position */
 #define MXC_F_I2S_CTRL0CH0_RX_THD_VAL                  ((uint32_t)(0xFFUL << MXC_F_I2S_CTRL0CH0_RX_THD_VAL_POS)) /**< CTRL0CH0_RX_THD_VAL Mask */

/**@} end of group I2S_CTRL0CH0_Register */

/**
 * @ingroup  i2s_registers
 * @defgroup I2S_CTRL1CH0 I2S_CTRL1CH0
 * @brief    Local channel Setup.
 * @{
 */
 #define MXC_F_I2S_CTRL1CH0_BITS_WORD_POS               0 /**< CTRL1CH0_BITS_WORD Position */
 #define MXC_F_I2S_CTRL1CH0_BITS_WORD                   ((uint32_t)(0x1FUL << MXC_F_I2S_CTRL1CH0_BITS_WORD_POS)) /**< CTRL1CH0_BITS_WORD Mask */

 #define MXC_F_I2S_CTRL1CH0_EN_POS                      8 /**< CTRL1CH0_EN Position */
 #define MXC_F_I2S_CTRL1CH0_EN                          ((uint32_t)(0x1UL << MXC_F_I2S_CTRL1CH0_EN_POS)) /**< CTRL1CH0_EN Mask */

 #define MXC_F_I2S_CTRL1CH0_SMP_SIZE_POS                9 /**< CTRL1CH0_SMP_SIZE Position */
 #define MXC_F_I2S_CTRL1CH0_SMP_SIZE                    ((uint32_t)(0x1FUL << MXC_F_I2S_CTRL1CH0_SMP_SIZE_POS)) /**< CTRL1CH0_SMP_SIZE Mask */

 #define MXC_F_I2S_CTRL1CH0_ADJST_POS                   15 /**< CTRL1CH0_ADJST Position */
 #define MXC_F_I2S_CTRL1CH0_ADJST                       ((uint32_t)(0x1UL << MXC_F_I2S_CTRL1CH0_ADJST_POS)) /**< CTRL1CH0_ADJST Mask */

 #define MXC_F_I2S_CTRL1CH0_CLKDIV_POS                  16 /**< CTRL1CH0_CLKDIV Position */
 #define MXC_F_I2S_CTRL1CH0_CLKDIV                      ((uint32_t)(0xFFFFUL << MXC_F_I2S_CTRL1CH0_CLKDIV_POS)) /**< CTRL1CH0_CLKDIV Mask */

/**@} end of group I2S_CTRL1CH0_Register */

/**
 * @ingroup  i2s_registers
 * @defgroup I2S_DMACH0 I2S_DMACH0
 * @brief    DMA Control.
 * @{
 */
 #define MXC_F_I2S_DMACH0_DMA_TX_THD_VAL_POS            0 /**< DMACH0_DMA_TX_THD_VAL Position */
 #define MXC_F_I2S_DMACH0_DMA_TX_THD_VAL                ((uint32_t)(0x7FUL << MXC_F_I2S_DMACH0_DMA_TX_THD_VAL_POS)) /**< DMACH0_DMA_TX_THD_VAL Mask */

 #define MXC_F_I2S_DMACH0_DMA_TX_EN_POS                 7 /**< DMACH0_DMA_TX_EN Position */
 #define MXC_F_I2S_DMACH0_DMA_TX_EN                     ((uint32_t)(0x1UL << MXC_F_I2S_DMACH0_DMA_TX_EN_POS)) /**< DMACH0_DMA_TX_EN Mask */

 #define MXC_F_I2S_DMACH0_DMA_RX_THD_VAL_POS            8 /**< DMACH0_DMA_RX_THD_VAL Position */
 #define MXC_F_I2S_DMACH0_DMA_RX_THD_VAL                ((uint32_t)(0x7FUL << MXC_F_I2S_DMACH0_DMA_RX_THD_VAL_POS)) /**< DMACH0_DMA_RX_THD_VAL Mask */

 #define MXC_F_I2S_DMACH0_DMA_RX_EN_POS                 15 /**< DMACH0_DMA_RX_EN Position */
 #define MXC_F_I2S_DMACH0_DMA_RX_EN                     ((uint32_t)(0x1UL << MXC_F_I2S_DMACH0_DMA_RX_EN_POS)) /**< DMACH0_DMA_RX_EN Mask */

 #define MXC_F_I2S_DMACH0_TX_LVL_POS                    16 /**< DMACH0_TX_LVL Position */
 #define MXC_F_I2S_DMACH0_TX_LVL                        ((uint32_t)(0xFFUL << MXC_F_I2S_DMACH0_TX_LVL_POS)) /**< DMACH0_TX_LVL Mask */

 #define MXC_F_I2S_DMACH0_RX_LVL_POS                    24 /**< DMACH0_RX_LVL Position */
 #define MXC_F_I2S_DMACH0_RX_LVL                        ((uint32_t)(0xFFUL << MXC_F_I2S_DMACH0_RX_LVL_POS)) /**< DMACH0_RX_LVL Mask */

/**@} end of group I2S_DMACH0_Register */

/**
 * @ingroup  i2s_registers
 * @defgroup I2S_FIFOCH0 I2S_FIFOCH0
 * @brief    I2S Fifo.
 * @{
 */
 #define MXC_F_I2S_FIFOCH0_DATA_POS                     0 /**< FIFOCH0_DATA Position */
 #define MXC_F_I2S_FIFOCH0_DATA                         ((uint32_t)(0xFFFFFFFFUL << MXC_F_I2S_FIFOCH0_DATA_POS)) /**< FIFOCH0_DATA Mask */

/**@} end of group I2S_FIFOCH0_Register */

/**
 * @ingroup  i2s_registers
 * @defgroup I2S_INTFL I2S_INTFL
 * @brief    ISR Status.
 * @{
 */
 #define MXC_F_I2S_INTFL_RX_OV_CH0_POS                  0 /**< INTFL_RX_OV_CH0 Position */
 #define MXC_F_I2S_INTFL_RX_OV_CH0                      ((uint32_t)(0x1UL << MXC_F_I2S_INTFL_RX_OV_CH0_POS)) /**< INTFL_RX_OV_CH0 Mask */

 #define MXC_F_I2S_INTFL_RX_THD_CH0_POS                 1 /**< INTFL_RX_THD_CH0 Position */
 #define MXC_F_I2S_INTFL_RX_THD_CH0                     ((uint32_t)(0x1UL << MXC_F_I2S_INTFL_RX_THD_CH0_POS)) /**< INTFL_RX_THD_CH0 Mask */

 #define MXC_F_I2S_INTFL_TX_OB_CH0_POS                  2 /**< INTFL_TX_OB_CH0 Position */
 #define MXC_F_I2S_INTFL_TX_OB_CH0                      ((uint32_t)(0x1UL << MXC_F_I2S_INTFL_TX_OB_CH0_POS)) /**< INTFL_TX_OB_CH0 Mask */

 #define MXC_F_I2S_INTFL_TX_HE_CH0_POS                  3 /**< INTFL_TX_HE_CH0 Position */
 #define MXC_F_I2S_INTFL_TX_HE_CH0                      ((uint32_t)(0x1UL << MXC_F_I2S_INTFL_TX_HE_CH0_POS)) /**< INTFL_TX_HE_CH0 Mask */

/**@} end of group I2S_INTFL_Register */

/**
 * @ingroup  i2s_registers
 * @defgroup I2S_INTEN I2S_INTEN
 * @brief    Interrupt Enable.
 * @{
 */
 #define MXC_F_I2S_INTEN_RX_OV_CH0_POS                  0 /**< INTEN_RX_OV_CH0 Position */
 #define MXC_F_I2S_INTEN_RX_OV_CH0                      ((uint32_t)(0x1UL << MXC_F_I2S_INTEN_RX_OV_CH0_POS)) /**< INTEN_RX_OV_CH0 Mask */

 #define MXC_F_I2S_INTEN_RX_THD_CH0_POS                 1 /**< INTEN_RX_THD_CH0 Position */
 #define MXC_F_I2S_INTEN_RX_THD_CH0                     ((uint32_t)(0x1UL << MXC_F_I2S_INTEN_RX_THD_CH0_POS)) /**< INTEN_RX_THD_CH0 Mask */

 #define MXC_F_I2S_INTEN_TX_OB_CH0_POS                  2 /**< INTEN_TX_OB_CH0 Position */
 #define MXC_F_I2S_INTEN_TX_OB_CH0                      ((uint32_t)(0x1UL << MXC_F_I2S_INTEN_TX_OB_CH0_POS)) /**< INTEN_TX_OB_CH0 Mask */

 #define MXC_F_I2S_INTEN_TX_HE_CH0_POS                  3 /**< INTEN_TX_HE_CH0 Position */
 #define MXC_F_I2S_INTEN_TX_HE_CH0                      ((uint32_t)(0x1UL << MXC_F_I2S_INTEN_TX_HE_CH0_POS)) /**< INTEN_TX_HE_CH0 Mask */

/**@} end of group I2S_INTEN_Register */

/**
 * @ingroup  i2s_registers
 * @defgroup I2S_EXTSETUP I2S_EXTSETUP
 * @brief    Ext Control.
 * @{
 */
 #define MXC_F_I2S_EXTSETUP_EXT_BITS_WORD_POS           0 /**< EXTSETUP_EXT_BITS_WORD Position */
 #define MXC_F_I2S_EXTSETUP_EXT_BITS_WORD               ((uint32_t)(0x1FUL << MXC_F_I2S_EXTSETUP_EXT_BITS_WORD_POS)) /**< EXTSETUP_EXT_BITS_WORD Mask */

/**@} end of group I2S_EXTSETUP_Register */

#ifdef __cplusplus
}
#endif

#endif /* _I2S_REGS_H_ */
