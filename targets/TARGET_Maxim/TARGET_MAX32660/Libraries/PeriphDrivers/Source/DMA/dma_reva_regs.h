/**
 * @file    dma_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the DMA Peripheral Module.
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

#ifndef _DMA_REVA_REGS_H_
#define _DMA_REVA_REGS_H_

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
 * @ingroup     dma
 * @defgroup    dma_registers DMA_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the DMA Peripheral Module.
 * @details DMA Controller Fully programmable, chaining capable DMA channels.
 */

/**
 * @ingroup dma_registers
 * Structure type to access the DMA Registers.
 */
typedef struct {
    __IO uint32_t ctrl;                 /**< <tt>\b 0x100:</tt> DMA CTRL Register */
    __IO uint32_t status;               /**< <tt>\b 0x104:</tt> DMA STATUS Register */
    __IO uint32_t src;                  /**< <tt>\b 0x108:</tt> DMA SRC Register */
    __IO uint32_t dst;                  /**< <tt>\b 0x10C:</tt> DMA DST Register */
    __IO uint32_t cnt;                  /**< <tt>\b 0x110:</tt> DMA CNT Register */
    __IO uint32_t srcrld;               /**< <tt>\b 0x114:</tt> DMA SRCRLD Register */
    __IO uint32_t dstrld;               /**< <tt>\b 0x118:</tt> DMA DSTRLD Register */
    __IO uint32_t cntrld;               /**< <tt>\b 0x11C:</tt> DMA CNTRLD Register */
} mxc_dma_reva_ch_regs_t;

typedef struct {
    __IO uint32_t inten;                /**< <tt>\b 0x000:</tt> DMA INTEN Register */
    __I  uint32_t intfl;                /**< <tt>\b 0x004:</tt> DMA INTFL Register */
    __R  uint32_t rsv_0x8_0xff[62];
    __IO mxc_dma_reva_ch_regs_t    ch[8];    /**< <tt>\b 0x100:</tt> DMA CH Register */
} mxc_dma_reva_regs_t;

/* Register offsets for module DMA */
/**
 * @ingroup    dma_registers
 * @defgroup   DMA_Register_Offsets Register Offsets
 * @brief      DMA Peripheral Register Offsets from the DMA Base Peripheral Address. 
 * @{
 */
 #define MXC_R_DMA_REVA_CTRL                     ((uint32_t)0x00000100UL) /**< Offset from DMA Base Address: <tt> 0x0100</tt> */
 #define MXC_R_DMA_REVA_STATUS                   ((uint32_t)0x00000104UL) /**< Offset from DMA Base Address: <tt> 0x0104</tt> */
 #define MXC_R_DMA_REVA_SRC                      ((uint32_t)0x00000108UL) /**< Offset from DMA Base Address: <tt> 0x0108</tt> */
 #define MXC_R_DMA_REVA_DST                      ((uint32_t)0x0000010CUL) /**< Offset from DMA Base Address: <tt> 0x010C</tt> */
 #define MXC_R_DMA_REVA_CNT                      ((uint32_t)0x00000110UL) /**< Offset from DMA Base Address: <tt> 0x0110</tt> */
 #define MXC_R_DMA_REVA_SRCRLD                   ((uint32_t)0x00000114UL) /**< Offset from DMA Base Address: <tt> 0x0114</tt> */
 #define MXC_R_DMA_REVA_DSTRLD                   ((uint32_t)0x00000118UL) /**< Offset from DMA Base Address: <tt> 0x0118</tt> */
 #define MXC_R_DMA_REVA_CNTRLD                   ((uint32_t)0x0000011CUL) /**< Offset from DMA Base Address: <tt> 0x011C</tt> */
 #define MXC_R_DMA_REVA_INTEN                    ((uint32_t)0x00000000UL) /**< Offset from DMA Base Address: <tt> 0x0000</tt> */
 #define MXC_R_DMA_REVA_INTFL                    ((uint32_t)0x00000004UL) /**< Offset from DMA Base Address: <tt> 0x0004</tt> */
 #define MXC_R_DMA_REVA_CH                       ((uint32_t)0x00000100UL) /**< Offset from DMA Base Address: <tt> 0x0100</tt> */
/**@} end of group dma_registers */

/**
 * @ingroup  dma_registers
 * @defgroup DMA_INTEN DMA_INTEN
 * @brief    DMA Control Register.
 * @{
 */
 #define MXC_F_DMA_REVA_INTEN_CH0_POS                        0 /**< INTEN_CH0 Position */
 #define MXC_F_DMA_REVA_INTEN_CH0                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH0_POS)) /**< INTEN_CH0 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH1_POS                        1 /**< INTEN_CH1 Position */
 #define MXC_F_DMA_REVA_INTEN_CH1                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH1_POS)) /**< INTEN_CH1 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH2_POS                        2 /**< INTEN_CH2 Position */
 #define MXC_F_DMA_REVA_INTEN_CH2                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH2_POS)) /**< INTEN_CH2 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH3_POS                        3 /**< INTEN_CH3 Position */
 #define MXC_F_DMA_REVA_INTEN_CH3                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH3_POS)) /**< INTEN_CH3 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH4_POS                        4 /**< INTEN_CH4 Position */
 #define MXC_F_DMA_REVA_INTEN_CH4                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH4_POS)) /**< INTEN_CH4 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH5_POS                        5 /**< INTEN_CH5 Position */
 #define MXC_F_DMA_REVA_INTEN_CH5                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH5_POS)) /**< INTEN_CH5 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH6_POS                        6 /**< INTEN_CH6 Position */
 #define MXC_F_DMA_REVA_INTEN_CH6                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH6_POS)) /**< INTEN_CH6 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH7_POS                        7 /**< INTEN_CH7 Position */
 #define MXC_F_DMA_REVA_INTEN_CH7                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH7_POS)) /**< INTEN_CH7 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH8_POS                        8 /**< INTEN_CH8 Position */
 #define MXC_F_DMA_REVA_INTEN_CH8                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH8_POS)) /**< INTEN_CH8 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH9_POS                        9 /**< INTEN_CH9 Position */
 #define MXC_F_DMA_REVA_INTEN_CH9                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH9_POS)) /**< INTEN_CH9 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH10_POS                       10 /**< INTEN_CH10 Position */
 #define MXC_F_DMA_REVA_INTEN_CH10                           ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH10_POS)) /**< INTEN_CH10 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH11_POS                       11 /**< INTEN_CH11 Position */
 #define MXC_F_DMA_REVA_INTEN_CH11                           ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH11_POS)) /**< INTEN_CH11 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH12_POS                       12 /**< INTEN_CH12 Position */
 #define MXC_F_DMA_REVA_INTEN_CH12                           ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH12_POS)) /**< INTEN_CH12 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH13_POS                       13 /**< INTEN_CH13 Position */
 #define MXC_F_DMA_REVA_INTEN_CH13                           ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH13_POS)) /**< INTEN_CH13 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH14_POS                       14 /**< INTEN_CH14 Position */
 #define MXC_F_DMA_REVA_INTEN_CH14                           ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH14_POS)) /**< INTEN_CH14 Mask */

 #define MXC_F_DMA_REVA_INTEN_CH15_POS                       15 /**< INTEN_CH15 Position */
 #define MXC_F_DMA_REVA_INTEN_CH15                           ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTEN_CH15_POS)) /**< INTEN_CH15 Mask */

/**@} end of group DMA_INTEN_Register */

/**
 * @ingroup  dma_registers
 * @defgroup DMA_INTFL DMA_INTFL
 * @brief    DMA Interrupt Register.
 * @{
 */
 #define MXC_F_DMA_REVA_INTFL_CH0_POS                        0 /**< INTFL_CH0 Position */
 #define MXC_F_DMA_REVA_INTFL_CH0                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH0_POS)) /**< INTFL_CH0 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH1_POS                        1 /**< INTFL_CH1 Position */
 #define MXC_F_DMA_REVA_INTFL_CH1                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH1_POS)) /**< INTFL_CH1 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH2_POS                        2 /**< INTFL_CH2 Position */
 #define MXC_F_DMA_REVA_INTFL_CH2                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH2_POS)) /**< INTFL_CH2 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH3_POS                        3 /**< INTFL_CH3 Position */
 #define MXC_F_DMA_REVA_INTFL_CH3                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH3_POS)) /**< INTFL_CH3 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH4_POS                        4 /**< INTFL_CH4 Position */
 #define MXC_F_DMA_REVA_INTFL_CH4                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH4_POS)) /**< INTFL_CH4 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH5_POS                        5 /**< INTFL_CH5 Position */
 #define MXC_F_DMA_REVA_INTFL_CH5                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH5_POS)) /**< INTFL_CH5 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH6_POS                        6 /**< INTFL_CH6 Position */
 #define MXC_F_DMA_REVA_INTFL_CH6                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH6_POS)) /**< INTFL_CH6 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH7_POS                        7 /**< INTFL_CH7 Position */
 #define MXC_F_DMA_REVA_INTFL_CH7                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH7_POS)) /**< INTFL_CH7 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH8_POS                        8 /**< INTFL_CH8 Position */
 #define MXC_F_DMA_REVA_INTFL_CH8                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH8_POS)) /**< INTFL_CH8 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH9_POS                        9 /**< INTFL_CH9 Position */
 #define MXC_F_DMA_REVA_INTFL_CH9                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH9_POS)) /**< INTFL_CH9 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH10_POS                       10 /**< INTFL_CH10 Position */
 #define MXC_F_DMA_REVA_INTFL_CH10                           ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH10_POS)) /**< INTFL_CH10 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH11_POS                       11 /**< INTFL_CH11 Position */
 #define MXC_F_DMA_REVA_INTFL_CH11                           ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH11_POS)) /**< INTFL_CH11 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH12_POS                       12 /**< INTFL_CH12 Position */
 #define MXC_F_DMA_REVA_INTFL_CH12                           ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH12_POS)) /**< INTFL_CH12 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH13_POS                       13 /**< INTFL_CH13 Position */
 #define MXC_F_DMA_REVA_INTFL_CH13                           ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH13_POS)) /**< INTFL_CH13 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH14_POS                       14 /**< INTFL_CH14 Position */
 #define MXC_F_DMA_REVA_INTFL_CH14                           ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH14_POS)) /**< INTFL_CH14 Mask */

 #define MXC_F_DMA_REVA_INTFL_CH15_POS                       15 /**< INTFL_CH15 Position */
 #define MXC_F_DMA_REVA_INTFL_CH15                           ((uint32_t)(0x1UL << MXC_F_DMA_REVA_INTFL_CH15_POS)) /**< INTFL_CH15 Mask */

/**@} end of group DMA_INTFL_Register */

/**
 * @ingroup  dma_registers
 * @defgroup DMA_CTRL DMA_CTRL
 * @brief    DMA Channel Control Register.
 * @{
 */
 #define MXC_F_DMA_REVA_CTRL_EN_POS                          0 /**< CTRL_EN Position */
 #define MXC_F_DMA_REVA_CTRL_EN                              ((uint32_t)(0x1UL << MXC_F_DMA_REVA_CTRL_EN_POS)) /**< CTRL_EN Mask */

 #define MXC_F_DMA_REVA_CTRL_RLDEN_POS                       1 /**< CTRL_RLDEN Position */
 #define MXC_F_DMA_REVA_CTRL_RLDEN                           ((uint32_t)(0x1UL << MXC_F_DMA_REVA_CTRL_RLDEN_POS)) /**< CTRL_RLDEN Mask */

 #define MXC_F_DMA_REVA_CTRL_PRI_POS                         2 /**< CTRL_PRI Position */
 #define MXC_F_DMA_REVA_CTRL_PRI                             ((uint32_t)(0x3UL << MXC_F_DMA_REVA_CTRL_PRI_POS)) /**< CTRL_PRI Mask */
 #define MXC_V_DMA_REVA_CTRL_PRI_HIGH                        ((uint32_t)0x0UL) /**< CTRL_PRI_HIGH Value */
 #define MXC_S_DMA_REVA_CTRL_PRI_HIGH                        (MXC_V_DMA_REVA_CTRL_PRI_HIGH << MXC_F_DMA_REVA_CTRL_PRI_POS) /**< CTRL_PRI_HIGH Setting */
 #define MXC_V_DMA_REVA_CTRL_PRI_MEDHIGH                     ((uint32_t)0x1UL) /**< CTRL_PRI_MEDHIGH Value */
 #define MXC_S_DMA_REVA_CTRL_PRI_MEDHIGH                     (MXC_V_DMA_REVA_CTRL_PRI_MEDHIGH << MXC_F_DMA_REVA_CTRL_PRI_POS) /**< CTRL_PRI_MEDHIGH Setting */
 #define MXC_V_DMA_REVA_CTRL_PRI_MEDLOW                      ((uint32_t)0x2UL) /**< CTRL_PRI_MEDLOW Value */
 #define MXC_S_DMA_REVA_CTRL_PRI_MEDLOW                      (MXC_V_DMA_REVA_CTRL_PRI_MEDLOW << MXC_F_DMA_REVA_CTRL_PRI_POS) /**< CTRL_PRI_MEDLOW Setting */
 #define MXC_V_DMA_REVA_CTRL_PRI_LOW                         ((uint32_t)0x3UL) /**< CTRL_PRI_LOW Value */
 #define MXC_S_DMA_REVA_CTRL_PRI_LOW                         (MXC_V_DMA_REVA_CTRL_PRI_LOW << MXC_F_DMA_REVA_CTRL_PRI_POS) /**< CTRL_PRI_LOW Setting */

 #define MXC_F_DMA_REVA_CTRL_REQUEST_POS                     4 /**< CTRL_REQUEST Position */
 #define MXC_F_DMA_REVA_CTRL_REQUEST                         ((uint32_t)(0x3FUL << MXC_F_DMA_REVA_CTRL_REQUEST_POS)) /**< CTRL_REQUEST Mask */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_MEMTOMEM                ((uint32_t)0x0UL) /**< CTRL_REQUEST_MEMTOMEM Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_MEMTOMEM                (MXC_V_DMA_REVA_CTRL_REQUEST_MEMTOMEM << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_MEMTOMEM Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_SPI0RX                  ((uint32_t)0x1UL) /**< CTRL_REQUEST_SPI0RX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_SPI0RX                  (MXC_V_DMA_REVA_CTRL_REQUEST_SPI0RX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_SPI0RX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_SPI1RX                  ((uint32_t)0x2UL) /**< CTRL_REQUEST_SPI1RX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_SPI1RX                  (MXC_V_DMA_REVA_CTRL_REQUEST_SPI1RX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_SPI1RX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_SPI2RX                  ((uint32_t)0x3UL) /**< CTRL_REQUEST_SPI2RX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_SPI2RX                  (MXC_V_DMA_REVA_CTRL_REQUEST_SPI2RX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_SPI2RX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_UART0RX                 ((uint32_t)0x4UL) /**< CTRL_REQUEST_UART0RX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_UART0RX                 (MXC_V_DMA_REVA_CTRL_REQUEST_UART0RX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_UART0RX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_UART1RX                 ((uint32_t)0x5UL) /**< CTRL_REQUEST_UART1RX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_UART1RX                 (MXC_V_DMA_REVA_CTRL_REQUEST_UART1RX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_UART1RX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_I2C0RX                  ((uint32_t)0x7UL) /**< CTRL_REQUEST_I2C0RX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_I2C0RX                  (MXC_V_DMA_REVA_CTRL_REQUEST_I2C0RX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_I2C0RX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_I2C1RX                  ((uint32_t)0x8UL) /**< CTRL_REQUEST_I2C1RX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_I2C1RX                  (MXC_V_DMA_REVA_CTRL_REQUEST_I2C1RX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_I2C1RX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_ADC                     ((uint32_t)0x9UL) /**< CTRL_REQUEST_ADC Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_ADC                     (MXC_V_DMA_REVA_CTRL_REQUEST_ADC << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_ADC Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_UART2RX                 ((uint32_t)0xEUL) /**< CTRL_REQUEST_UART2RX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_UART2RX                 (MXC_V_DMA_REVA_CTRL_REQUEST_UART2RX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_UART2RX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_SPI3RX                  ((uint32_t)0xFUL) /**< CTRL_REQUEST_SPI3RX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_SPI3RX                  (MXC_V_DMA_REVA_CTRL_REQUEST_SPI3RX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_SPI3RX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_SPI_MSS0RX              ((uint32_t)0x10UL) /**< CTRL_REQUEST_SPI_MSS0RX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_SPI_MSS0RX              (MXC_V_DMA_REVA_CTRL_REQUEST_SPI_MSS0RX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_SPI_MSS0RX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP1                ((uint32_t)0x11UL) /**< CTRL_REQUEST_USBRXEP1 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBRXEP1                (MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP1 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBRXEP1 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP2                ((uint32_t)0x12UL) /**< CTRL_REQUEST_USBRXEP2 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBRXEP2                (MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP2 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBRXEP2 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP3                ((uint32_t)0x13UL) /**< CTRL_REQUEST_USBRXEP3 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBRXEP3                (MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP3 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBRXEP3 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP4                ((uint32_t)0x14UL) /**< CTRL_REQUEST_USBRXEP4 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBRXEP4                (MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP4 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBRXEP4 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP5                ((uint32_t)0x15UL) /**< CTRL_REQUEST_USBRXEP5 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBRXEP5                (MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP5 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBRXEP5 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP6                ((uint32_t)0x16UL) /**< CTRL_REQUEST_USBRXEP6 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBRXEP6                (MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP6 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBRXEP6 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP7                ((uint32_t)0x17UL) /**< CTRL_REQUEST_USBRXEP7 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBRXEP7                (MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP7 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBRXEP7 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP8                ((uint32_t)0x18UL) /**< CTRL_REQUEST_USBRXEP8 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBRXEP8                (MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP8 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBRXEP8 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP9                ((uint32_t)0x19UL) /**< CTRL_REQUEST_USBRXEP9 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBRXEP9                (MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP9 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBRXEP9 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP10               ((uint32_t)0x1AUL) /**< CTRL_REQUEST_USBRXEP10 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBRXEP10               (MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP10 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBRXEP10 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP11               ((uint32_t)0x1BUL) /**< CTRL_REQUEST_USBRXEP11 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBRXEP11               (MXC_V_DMA_REVA_CTRL_REQUEST_USBRXEP11 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBRXEP11 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_SPI0TX                  ((uint32_t)0x21UL) /**< CTRL_REQUEST_SPI0TX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_SPI0TX                  (MXC_V_DMA_REVA_CTRL_REQUEST_SPI0TX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_SPI0TX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_SPI1TX                  ((uint32_t)0x22UL) /**< CTRL_REQUEST_SPI1TX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_SPI1TX                  (MXC_V_DMA_REVA_CTRL_REQUEST_SPI1TX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_SPI1TX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_SPI2TX                  ((uint32_t)0x23UL) /**< CTRL_REQUEST_SPI2TX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_SPI2TX                  (MXC_V_DMA_REVA_CTRL_REQUEST_SPI2TX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_SPI2TX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_UART0TX                 ((uint32_t)0x24UL) /**< CTRL_REQUEST_UART0TX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_UART0TX                 (MXC_V_DMA_REVA_CTRL_REQUEST_UART0TX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_UART0TX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_UART1TX                 ((uint32_t)0x25UL) /**< CTRL_REQUEST_UART1TX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_UART1TX                 (MXC_V_DMA_REVA_CTRL_REQUEST_UART1TX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_UART1TX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_I2C0TX                  ((uint32_t)0x27UL) /**< CTRL_REQUEST_I2C0TX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_I2C0TX                  (MXC_V_DMA_REVA_CTRL_REQUEST_I2C0TX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_I2C0TX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_I2C1TX                  ((uint32_t)0x28UL) /**< CTRL_REQUEST_I2C1TX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_I2C1TX                  (MXC_V_DMA_REVA_CTRL_REQUEST_I2C1TX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_I2C1TX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_UART2TX                 ((uint32_t)0x2EUL) /**< CTRL_REQUEST_UART2TX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_UART2TX                 (MXC_V_DMA_REVA_CTRL_REQUEST_UART2TX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_UART2TX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_SPI3TX                  ((uint32_t)0x2FUL) /**< CTRL_REQUEST_SPI3TX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_SPI3TX                  (MXC_V_DMA_REVA_CTRL_REQUEST_SPI3TX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_SPI3TX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_SPI_MSS0TX              ((uint32_t)0x30UL) /**< CTRL_REQUEST_SPI_MSS0TX Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_SPI_MSS0TX              (MXC_V_DMA_REVA_CTRL_REQUEST_SPI_MSS0TX << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_SPI_MSS0TX Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP1                ((uint32_t)0x31UL) /**< CTRL_REQUEST_USBTXEP1 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBTXEP1                (MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP1 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBTXEP1 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP2                ((uint32_t)0x32UL) /**< CTRL_REQUEST_USBTXEP2 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBTXEP2                (MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP2 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBTXEP2 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP3                ((uint32_t)0x33UL) /**< CTRL_REQUEST_USBTXEP3 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBTXEP3                (MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP3 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBTXEP3 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP4                ((uint32_t)0x34UL) /**< CTRL_REQUEST_USBTXEP4 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBTXEP4                (MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP4 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBTXEP4 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP5                ((uint32_t)0x35UL) /**< CTRL_REQUEST_USBTXEP5 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBTXEP5                (MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP5 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBTXEP5 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP6                ((uint32_t)0x36UL) /**< CTRL_REQUEST_USBTXEP6 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBTXEP6                (MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP6 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBTXEP6 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP7                ((uint32_t)0x37UL) /**< CTRL_REQUEST_USBTXEP7 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBTXEP7                (MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP7 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBTXEP7 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP8                ((uint32_t)0x38UL) /**< CTRL_REQUEST_USBTXEP8 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBTXEP8                (MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP8 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBTXEP8 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP9                ((uint32_t)0x39UL) /**< CTRL_REQUEST_USBTXEP9 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBTXEP9                (MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP9 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBTXEP9 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP10               ((uint32_t)0x3AUL) /**< CTRL_REQUEST_USBTXEP10 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBTXEP10               (MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP10 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBTXEP10 Setting */
 #define MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP11               ((uint32_t)0x3BUL) /**< CTRL_REQUEST_USBTXEP11 Value */
 #define MXC_S_DMA_REVA_CTRL_REQUEST_USBTXEP11               (MXC_V_DMA_REVA_CTRL_REQUEST_USBTXEP11 << MXC_F_DMA_REVA_CTRL_REQUEST_POS) /**< CTRL_REQUEST_USBTXEP11 Setting */

 #define MXC_F_DMA_REVA_CTRL_TO_WAIT_POS                     10 /**< CTRL_TO_WAIT Position */
 #define MXC_F_DMA_REVA_CTRL_TO_WAIT                         ((uint32_t)(0x1UL << MXC_F_DMA_REVA_CTRL_TO_WAIT_POS)) /**< CTRL_TO_WAIT Mask */

 #define MXC_F_DMA_REVA_CTRL_TO_PER_POS                      11 /**< CTRL_TO_PER Position */
 #define MXC_F_DMA_REVA_CTRL_TO_PER                          ((uint32_t)(0x7UL << MXC_F_DMA_REVA_CTRL_TO_PER_POS)) /**< CTRL_TO_PER Mask */
 #define MXC_V_DMA_REVA_CTRL_TO_PER_TO4                      ((uint32_t)0x0UL) /**< CTRL_TO_PER_TO4 Value */
 #define MXC_S_DMA_REVA_CTRL_TO_PER_TO4                      (MXC_V_DMA_REVA_CTRL_TO_PER_TO4 << MXC_F_DMA_REVA_CTRL_TO_PER_POS) /**< CTRL_TO_PER_TO4 Setting */
 #define MXC_V_DMA_REVA_CTRL_TO_PER_TO8                      ((uint32_t)0x1UL) /**< CTRL_TO_PER_TO8 Value */
 #define MXC_S_DMA_REVA_CTRL_TO_PER_TO8                      (MXC_V_DMA_REVA_CTRL_TO_PER_TO8 << MXC_F_DMA_REVA_CTRL_TO_PER_POS) /**< CTRL_TO_PER_TO8 Setting */
 #define MXC_V_DMA_REVA_CTRL_TO_PER_TO16                     ((uint32_t)0x2UL) /**< CTRL_TO_PER_TO16 Value */
 #define MXC_S_DMA_REVA_CTRL_TO_PER_TO16                     (MXC_V_DMA_REVA_CTRL_TO_PER_TO16 << MXC_F_DMA_REVA_CTRL_TO_PER_POS) /**< CTRL_TO_PER_TO16 Setting */
 #define MXC_V_DMA_REVA_CTRL_TO_PER_TO32                     ((uint32_t)0x3UL) /**< CTRL_TO_PER_TO32 Value */
 #define MXC_S_DMA_REVA_CTRL_TO_PER_TO32                     (MXC_V_DMA_REVA_CTRL_TO_PER_TO32 << MXC_F_DMA_REVA_CTRL_TO_PER_POS) /**< CTRL_TO_PER_TO32 Setting */
 #define MXC_V_DMA_REVA_CTRL_TO_PER_TO64                     ((uint32_t)0x4UL) /**< CTRL_TO_PER_TO64 Value */
 #define MXC_S_DMA_REVA_CTRL_TO_PER_TO64                     (MXC_V_DMA_REVA_CTRL_TO_PER_TO64 << MXC_F_DMA_REVA_CTRL_TO_PER_POS) /**< CTRL_TO_PER_TO64 Setting */
 #define MXC_V_DMA_REVA_CTRL_TO_PER_TO128                    ((uint32_t)0x5UL) /**< CTRL_TO_PER_TO128 Value */
 #define MXC_S_DMA_REVA_CTRL_TO_PER_TO128                    (MXC_V_DMA_REVA_CTRL_TO_PER_TO128 << MXC_F_DMA_REVA_CTRL_TO_PER_POS) /**< CTRL_TO_PER_TO128 Setting */
 #define MXC_V_DMA_REVA_CTRL_TO_PER_TO256                    ((uint32_t)0x6UL) /**< CTRL_TO_PER_TO256 Value */
 #define MXC_S_DMA_REVA_CTRL_TO_PER_TO256                    (MXC_V_DMA_REVA_CTRL_TO_PER_TO256 << MXC_F_DMA_REVA_CTRL_TO_PER_POS) /**< CTRL_TO_PER_TO256 Setting */
 #define MXC_V_DMA_REVA_CTRL_TO_PER_TO512                    ((uint32_t)0x7UL) /**< CTRL_TO_PER_TO512 Value */
 #define MXC_S_DMA_REVA_CTRL_TO_PER_TO512                    (MXC_V_DMA_REVA_CTRL_TO_PER_TO512 << MXC_F_DMA_REVA_CTRL_TO_PER_POS) /**< CTRL_TO_PER_TO512 Setting */

 #define MXC_F_DMA_REVA_CTRL_TO_CLKDIV_POS                   14 /**< CTRL_TO_CLKDIV Position */
 #define MXC_F_DMA_REVA_CTRL_TO_CLKDIV                       ((uint32_t)(0x3UL << MXC_F_DMA_REVA_CTRL_TO_CLKDIV_POS)) /**< CTRL_TO_CLKDIV Mask */
 #define MXC_V_DMA_REVA_CTRL_TO_CLKDIV_DIS                   ((uint32_t)0x0UL) /**< CTRL_TO_CLKDIV_DIS Value */
 #define MXC_S_DMA_REVA_CTRL_TO_CLKDIV_DIS                   (MXC_V_DMA_REVA_CTRL_TO_CLKDIV_DIS << MXC_F_DMA_REVA_CTRL_TO_CLKDIV_POS) /**< CTRL_TO_CLKDIV_DIS Setting */
 #define MXC_V_DMA_REVA_CTRL_TO_CLKDIV_DIV256                ((uint32_t)0x1UL) /**< CTRL_TO_CLKDIV_DIV256 Value */
 #define MXC_S_DMA_REVA_CTRL_TO_CLKDIV_DIV256                (MXC_V_DMA_REVA_CTRL_TO_CLKDIV_DIV256 << MXC_F_DMA_REVA_CTRL_TO_CLKDIV_POS) /**< CTRL_TO_CLKDIV_DIV256 Setting */
 #define MXC_V_DMA_REVA_CTRL_TO_CLKDIV_DIV64K                ((uint32_t)0x2UL) /**< CTRL_TO_CLKDIV_DIV64K Value */
 #define MXC_S_DMA_REVA_CTRL_TO_CLKDIV_DIV64K                (MXC_V_DMA_REVA_CTRL_TO_CLKDIV_DIV64K << MXC_F_DMA_REVA_CTRL_TO_CLKDIV_POS) /**< CTRL_TO_CLKDIV_DIV64K Setting */
 #define MXC_V_DMA_REVA_CTRL_TO_CLKDIV_DIV16M                ((uint32_t)0x3UL) /**< CTRL_TO_CLKDIV_DIV16M Value */
 #define MXC_S_DMA_REVA_CTRL_TO_CLKDIV_DIV16M                (MXC_V_DMA_REVA_CTRL_TO_CLKDIV_DIV16M << MXC_F_DMA_REVA_CTRL_TO_CLKDIV_POS) /**< CTRL_TO_CLKDIV_DIV16M Setting */

 #define MXC_F_DMA_REVA_CTRL_SRCWD_POS                       16 /**< CTRL_SRCWD Position */
 #define MXC_F_DMA_REVA_CTRL_SRCWD                           ((uint32_t)(0x3UL << MXC_F_DMA_REVA_CTRL_SRCWD_POS)) /**< CTRL_SRCWD Mask */
 #define MXC_V_DMA_REVA_CTRL_SRCWD_BYTE                      ((uint32_t)0x0UL) /**< CTRL_SRCWD_BYTE Value */
 #define MXC_S_DMA_REVA_CTRL_SRCWD_BYTE                      (MXC_V_DMA_REVA_CTRL_SRCWD_BYTE << MXC_F_DMA_REVA_CTRL_SRCWD_POS) /**< CTRL_SRCWD_BYTE Setting */
 #define MXC_V_DMA_REVA_CTRL_SRCWD_HALFWORD                  ((uint32_t)0x1UL) /**< CTRL_SRCWD_HALFWORD Value */
 #define MXC_S_DMA_REVA_CTRL_SRCWD_HALFWORD                  (MXC_V_DMA_REVA_CTRL_SRCWD_HALFWORD << MXC_F_DMA_REVA_CTRL_SRCWD_POS) /**< CTRL_SRCWD_HALFWORD Setting */
 #define MXC_V_DMA_REVA_CTRL_SRCWD_WORD                      ((uint32_t)0x2UL) /**< CTRL_SRCWD_WORD Value */
 #define MXC_S_DMA_REVA_CTRL_SRCWD_WORD                      (MXC_V_DMA_REVA_CTRL_SRCWD_WORD << MXC_F_DMA_REVA_CTRL_SRCWD_POS) /**< CTRL_SRCWD_WORD Setting */

 #define MXC_F_DMA_REVA_CTRL_SRCINC_POS                      18 /**< CTRL_SRCINC Position */
 #define MXC_F_DMA_REVA_CTRL_SRCINC                          ((uint32_t)(0x1UL << MXC_F_DMA_REVA_CTRL_SRCINC_POS)) /**< CTRL_SRCINC Mask */

 #define MXC_F_DMA_REVA_CTRL_DSTWD_POS                       20 /**< CTRL_DSTWD Position */
 #define MXC_F_DMA_REVA_CTRL_DSTWD                           ((uint32_t)(0x3UL << MXC_F_DMA_REVA_CTRL_DSTWD_POS)) /**< CTRL_DSTWD Mask */
 #define MXC_V_DMA_REVA_CTRL_DSTWD_BYTE                      ((uint32_t)0x0UL) /**< CTRL_DSTWD_BYTE Value */
 #define MXC_S_DMA_REVA_CTRL_DSTWD_BYTE                      (MXC_V_DMA_REVA_CTRL_DSTWD_BYTE << MXC_F_DMA_REVA_CTRL_DSTWD_POS) /**< CTRL_DSTWD_BYTE Setting */
 #define MXC_V_DMA_REVA_CTRL_DSTWD_HALFWORD                  ((uint32_t)0x1UL) /**< CTRL_DSTWD_HALFWORD Value */
 #define MXC_S_DMA_REVA_CTRL_DSTWD_HALFWORD                  (MXC_V_DMA_REVA_CTRL_DSTWD_HALFWORD << MXC_F_DMA_REVA_CTRL_DSTWD_POS) /**< CTRL_DSTWD_HALFWORD Setting */
 #define MXC_V_DMA_REVA_CTRL_DSTWD_WORD                      ((uint32_t)0x2UL) /**< CTRL_DSTWD_WORD Value */
 #define MXC_S_DMA_REVA_CTRL_DSTWD_WORD                      (MXC_V_DMA_REVA_CTRL_DSTWD_WORD << MXC_F_DMA_REVA_CTRL_DSTWD_POS) /**< CTRL_DSTWD_WORD Setting */

 #define MXC_F_DMA_REVA_CTRL_DSTINC_POS                      22 /**< CTRL_DSTINC Position */
 #define MXC_F_DMA_REVA_CTRL_DSTINC                          ((uint32_t)(0x1UL << MXC_F_DMA_REVA_CTRL_DSTINC_POS)) /**< CTRL_DSTINC Mask */

 #define MXC_F_DMA_REVA_CTRL_BURST_SIZE_POS                  24 /**< CTRL_BURST_SIZE Position */
 #define MXC_F_DMA_REVA_CTRL_BURST_SIZE                      ((uint32_t)(0x1FUL << MXC_F_DMA_REVA_CTRL_BURST_SIZE_POS)) /**< CTRL_BURST_SIZE Mask */

 #define MXC_F_DMA_REVA_CTRL_DIS_IE_POS                      30 /**< CTRL_DIS_IE Position */
 #define MXC_F_DMA_REVA_CTRL_DIS_IE                          ((uint32_t)(0x1UL << MXC_F_DMA_REVA_CTRL_DIS_IE_POS)) /**< CTRL_DIS_IE Mask */

 #define MXC_F_DMA_REVA_CTRL_CTZ_IE_POS                      31 /**< CTRL_CTZ_IE Position */
 #define MXC_F_DMA_REVA_CTRL_CTZ_IE                          ((uint32_t)(0x1UL << MXC_F_DMA_REVA_CTRL_CTZ_IE_POS)) /**< CTRL_CTZ_IE Mask */

/**@} end of group DMA_CTRL_Register */

/**
 * @ingroup  dma_registers
 * @defgroup DMA_STATUS DMA_STATUS
 * @brief    DMA Channel Status Register.
 * @{
 */
 #define MXC_F_DMA_REVA_STATUS_STATUS_POS                    0 /**< STATUS_STATUS Position */
 #define MXC_F_DMA_REVA_STATUS_STATUS                        ((uint32_t)(0x1UL << MXC_F_DMA_REVA_STATUS_STATUS_POS)) /**< STATUS_STATUS Mask */

 #define MXC_F_DMA_REVA_STATUS_IPEND_POS                     1 /**< STATUS_IPEND Position */
 #define MXC_F_DMA_REVA_STATUS_IPEND                         ((uint32_t)(0x1UL << MXC_F_DMA_REVA_STATUS_IPEND_POS)) /**< STATUS_IPEND Mask */

 #define MXC_F_DMA_REVA_STATUS_CTZ_IF_POS                    2 /**< STATUS_CTZ_IF Position */
 #define MXC_F_DMA_REVA_STATUS_CTZ_IF                        ((uint32_t)(0x1UL << MXC_F_DMA_REVA_STATUS_CTZ_IF_POS)) /**< STATUS_CTZ_IF Mask */

 #define MXC_F_DMA_REVA_STATUS_RLD_IF_POS                    3 /**< STATUS_RLD_IF Position */
 #define MXC_F_DMA_REVA_STATUS_RLD_IF                        ((uint32_t)(0x1UL << MXC_F_DMA_REVA_STATUS_RLD_IF_POS)) /**< STATUS_RLD_IF Mask */

 #define MXC_F_DMA_REVA_STATUS_BUS_ERR_POS                   4 /**< STATUS_BUS_ERR Position */
 #define MXC_F_DMA_REVA_STATUS_BUS_ERR                       ((uint32_t)(0x1UL << MXC_F_DMA_REVA_STATUS_BUS_ERR_POS)) /**< STATUS_BUS_ERR Mask */

 #define MXC_F_DMA_REVA_STATUS_TO_IF_POS                     6 /**< STATUS_TO_IF Position */
 #define MXC_F_DMA_REVA_STATUS_TO_IF                         ((uint32_t)(0x1UL << MXC_F_DMA_REVA_STATUS_TO_IF_POS)) /**< STATUS_TO_IF Mask */

/**@} end of group DMA_STATUS_Register */

/**
 * @ingroup  dma_registers
 * @defgroup DMA_SRC DMA_SRC
 * @brief    Source Device Address. If SRCINC=1, the counter bits are incremented by 1,2, or
 *           4, depending on the data width of each AHB cycle. For peripheral transfers, some
 *           or all of the actual address bits are fixed. If SRCINC=0, this register remains
 *           constant. In the case where a count-to-zero condition occurs while RLDEN=1, the
 *           register is reloaded with the contents of DMA_SRC_RLD.
 * @{
 */
 #define MXC_F_DMA_REVA_SRC_ADDR_POS                         0 /**< SRC_ADDR Position */
 #define MXC_F_DMA_REVA_SRC_ADDR                             ((uint32_t)(0xFFFFFFFFUL << MXC_F_DMA_REVA_SRC_ADDR_POS)) /**< SRC_ADDR Mask */

/**@} end of group DMA_SRC_Register */

/**
 * @ingroup  dma_registers
 * @defgroup DMA_DST DMA_DST
 * @brief    Destination Device Address. For peripheral transfers, some or all of the actual
 *           address bits are fixed. If DSTINC=1, this register is incremented on every AHB
 *           write out of the DMA FIFO. They are incremented by 1, 2, or 4, depending on the
 *           data width of each AHB cycle. In the case where a count-to-zero condition occurs
 *           while RLDEN=1, the register is reloaded with DMA_DST_RLD.
 * @{
 */
 #define MXC_F_DMA_REVA_DST_ADDR_POS                         0 /**< DST_ADDR Position */
 #define MXC_F_DMA_REVA_DST_ADDR                             ((uint32_t)(0xFFFFFFFFUL << MXC_F_DMA_REVA_DST_ADDR_POS)) /**< DST_ADDR Mask */

/**@} end of group DMA_DST_Register */

/**
 * @ingroup  dma_registers
 * @defgroup DMA_CNT DMA_CNT
 * @brief    DMA Counter. The user loads this register with the number of bytes to transfer.
 *           This counter decreases on every AHB cycle into the DMA FIFO. The decrement will
 *           be 1, 2, or 4 depending on the data width of each AHB cycle. When the counter
 *           reaches 0, a count-to-zero condition is triggered.
 * @{
 */
 #define MXC_F_DMA_REVA_CNT_CNT_POS                          0 /**< CNT_CNT Position */
 #define MXC_F_DMA_REVA_CNT_CNT                              ((uint32_t)(0xFFFFFFUL << MXC_F_DMA_REVA_CNT_CNT_POS)) /**< CNT_CNT Mask */

/**@} end of group DMA_CNT_Register */

/**
 * @ingroup  dma_registers
 * @defgroup DMA_SRCRLD DMA_SRCRLD
 * @brief    Source Address Reload Value. The value of this register is loaded into DMA0_SRC
 *           upon a count-to-zero condition.
 * @{
 */
 #define MXC_F_DMA_REVA_SRCRLD_ADDR_POS                      0 /**< SRCRLD_ADDR Position */
 #define MXC_F_DMA_REVA_SRCRLD_ADDR                          ((uint32_t)(0x7FFFFFFFUL << MXC_F_DMA_REVA_SRCRLD_ADDR_POS)) /**< SRCRLD_ADDR Mask */

/**@} end of group DMA_SRCRLD_Register */

/**
 * @ingroup  dma_registers
 * @defgroup DMA_DSTRLD DMA_DSTRLD
 * @brief    Destination Address Reload Value. The value of this register is loaded into
 *           DMA0_DST upon a count-to-zero condition.
 * @{
 */
 #define MXC_F_DMA_REVA_DSTRLD_ADDR_POS                      0 /**< DSTRLD_ADDR Position */
 #define MXC_F_DMA_REVA_DSTRLD_ADDR                          ((uint32_t)(0x7FFFFFFFUL << MXC_F_DMA_REVA_DSTRLD_ADDR_POS)) /**< DSTRLD_ADDR Mask */

/**@} end of group DMA_DSTRLD_Register */

/**
 * @ingroup  dma_registers
 * @defgroup DMA_CNTRLD DMA_CNTRLD
 * @brief    DMA Channel Count Reload Register.
 * @{
 */
 #define MXC_F_DMA_REVA_CNTRLD_CNT_POS                       0 /**< CNTRLD_CNT Position */
 #define MXC_F_DMA_REVA_CNTRLD_CNT                           ((uint32_t)(0xFFFFFFUL << MXC_F_DMA_REVA_CNTRLD_CNT_POS)) /**< CNTRLD_CNT Mask */

 #define MXC_F_DMA_REVA_CNTRLD_EN_POS                        31 /**< CNTRLD_EN Position */
 #define MXC_F_DMA_REVA_CNTRLD_EN                            ((uint32_t)(0x1UL << MXC_F_DMA_REVA_CNTRLD_EN_POS)) /**< CNTRLD_EN Mask */

/**@} end of group DMA_CNTRLD_Register */

#ifdef __cplusplus
}
#endif

#endif /* _DMA_REVA_REGS_H_ */
