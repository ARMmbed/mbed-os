/**
 * @file    spi_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the SPI Peripheral Module.
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

#ifndef _SPI_REGS_H_
#define _SPI_REGS_H_

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
 * @ingroup     spi
 * @defgroup    spi_registers SPI_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the SPI Peripheral Module.
 * @details SPI peripheral.
 */

/**
 * @ingroup spi_registers
 * Structure type to access the SPI Registers.
 */
typedef struct {
    __IO uint32_t data;                 /**< <tt>\b 0x00:</tt> SPI DATA Register */
    __IO uint32_t ctrl0;                /**< <tt>\b 0x04:</tt> SPI CTRL0 Register */
    __IO uint32_t ctrl1;                /**< <tt>\b 0x08:</tt> SPI CTRL1 Register */
    __IO uint32_t ctrl2;                /**< <tt>\b 0x0C:</tt> SPI CTRL2 Register */
    __IO uint32_t ss_time;              /**< <tt>\b 0x10:</tt> SPI SS_TIME Register */
    __IO uint32_t clk_cfg;              /**< <tt>\b 0x14:</tt> SPI CLK_CFG Register */
    __I  uint32_t rsv_0x18;
    __IO uint32_t dma;                  /**< <tt>\b 0x1C:</tt> SPI DMA Register */
    __IO uint32_t int_fl;               /**< <tt>\b 0x20:</tt> SPI INT_FL Register */
    __IO uint32_t int_en;               /**< <tt>\b 0x24:</tt> SPI INT_EN Register */
    __IO uint32_t wake_fl;              /**< <tt>\b 0x28:</tt> SPI WAKE_FL Register */
    __IO uint32_t wake_en;              /**< <tt>\b 0x2C:</tt> SPI WAKE_EN Register */
    __I  uint32_t stat;                 /**< <tt>\b 0x30:</tt> SPI STAT Register */
} mxc_spi_regs_t;

/* Register offsets for module SPI */
/**
 * @ingroup    spi_registers
 * @defgroup   SPI_Register_Offsets Register Offsets
 * @brief      SPI Peripheral Register Offsets from the SPI Base Peripheral Address. 
 * @{
 */
 #define MXC_R_SPI_DATA                     ((uint32_t)0x00000000UL) /**< Offset from SPI Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_SPI_CTRL0                    ((uint32_t)0x00000004UL) /**< Offset from SPI Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_SPI_CTRL1                    ((uint32_t)0x00000008UL) /**< Offset from SPI Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_SPI_CTRL2                    ((uint32_t)0x0000000CUL) /**< Offset from SPI Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_SPI_SS_TIME                  ((uint32_t)0x00000010UL) /**< Offset from SPI Base Address: <tt> 0x0010</tt> */ 
 #define MXC_R_SPI_CLK_CFG                  ((uint32_t)0x00000014UL) /**< Offset from SPI Base Address: <tt> 0x0014</tt> */ 
 #define MXC_R_SPI_DMA                      ((uint32_t)0x0000001CUL) /**< Offset from SPI Base Address: <tt> 0x001C</tt> */ 
 #define MXC_R_SPI_INT_FL                   ((uint32_t)0x00000020UL) /**< Offset from SPI Base Address: <tt> 0x0020</tt> */ 
 #define MXC_R_SPI_INT_EN                   ((uint32_t)0x00000024UL) /**< Offset from SPI Base Address: <tt> 0x0024</tt> */ 
 #define MXC_R_SPI_WAKE_FL                  ((uint32_t)0x00000028UL) /**< Offset from SPI Base Address: <tt> 0x0028</tt> */ 
 #define MXC_R_SPI_WAKE_EN                  ((uint32_t)0x0000002CUL) /**< Offset from SPI Base Address: <tt> 0x002C</tt> */ 
 #define MXC_R_SPI_STAT                     ((uint32_t)0x00000030UL) /**< Offset from SPI Base Address: <tt> 0x0030</tt> */ 
/**@} end of group spi_registers */

/**
 * @ingroup  spi_registers
 * @defgroup SPI_DATA SPI_DATA
 * @brief    Register for reading and writing the FIFO.
 * @{
 */
/**@} end of group SPI_DATA_Register */

/**
 * @ingroup  spi_registers
 * @defgroup SPI_CTRL0 SPI_CTRL0
 * @brief    Register for controlling SPI peripheral.
 * @{
 */
 #define MXC_F_SPI_CTRL0_SPI_EN_POS                     0 /**< CTRL0_SPI_EN Position */
 #define MXC_F_SPI_CTRL0_SPI_EN                         ((uint32_t)(0x1UL << MXC_F_SPI_CTRL0_SPI_EN_POS)) /**< CTRL0_SPI_EN Mask */

 #define MXC_F_SPI_CTRL0_MM_EN_POS                      1 /**< CTRL0_MM_EN Position */
 #define MXC_F_SPI_CTRL0_MM_EN                          ((uint32_t)(0x1UL << MXC_F_SPI_CTRL0_MM_EN_POS)) /**< CTRL0_MM_EN Mask */

 #define MXC_F_SPI_CTRL0_SS_IO_POS                      4 /**< CTRL0_SS_IO Position */
 #define MXC_F_SPI_CTRL0_SS_IO                          ((uint32_t)(0x1UL << MXC_F_SPI_CTRL0_SS_IO_POS)) /**< CTRL0_SS_IO Mask */

 #define MXC_F_SPI_CTRL0_START_POS                      5 /**< CTRL0_START Position */
 #define MXC_F_SPI_CTRL0_START                          ((uint32_t)(0x1UL << MXC_F_SPI_CTRL0_START_POS)) /**< CTRL0_START Mask */

 #define MXC_F_SPI_CTRL0_SS_CTRL_POS                    8 /**< CTRL0_SS_CTRL Position */
 #define MXC_F_SPI_CTRL0_SS_CTRL                        ((uint32_t)(0x1UL << MXC_F_SPI_CTRL0_SS_CTRL_POS)) /**< CTRL0_SS_CTRL Mask */

 #define MXC_F_SPI_CTRL0_SS_SEL_POS                     16 /**< CTRL0_SS_SEL Position */
 #define MXC_F_SPI_CTRL0_SS_SEL                         ((uint32_t)(0xFUL << MXC_F_SPI_CTRL0_SS_SEL_POS)) /**< CTRL0_SS_SEL Mask */
 #define MXC_V_SPI_CTRL0_SS_SEL_SS0                     ((uint32_t)0x1UL) /**< CTRL0_SS_SEL_SS0 Value */
 #define MXC_S_SPI_CTRL0_SS_SEL_SS0                     (MXC_V_SPI_CTRL0_SS_SEL_SS0 << MXC_F_SPI_CTRL0_SS_SEL_POS) /**< CTRL0_SS_SEL_SS0 Setting */
 #define MXC_V_SPI_CTRL0_SS_SEL_SS1                     ((uint32_t)0x2UL) /**< CTRL0_SS_SEL_SS1 Value */
 #define MXC_S_SPI_CTRL0_SS_SEL_SS1                     (MXC_V_SPI_CTRL0_SS_SEL_SS1 << MXC_F_SPI_CTRL0_SS_SEL_POS) /**< CTRL0_SS_SEL_SS1 Setting */
 #define MXC_V_SPI_CTRL0_SS_SEL_SS2                     ((uint32_t)0x4UL) /**< CTRL0_SS_SEL_SS2 Value */
 #define MXC_S_SPI_CTRL0_SS_SEL_SS2                     (MXC_V_SPI_CTRL0_SS_SEL_SS2 << MXC_F_SPI_CTRL0_SS_SEL_POS) /**< CTRL0_SS_SEL_SS2 Setting */
 #define MXC_V_SPI_CTRL0_SS_SEL_SS3                     ((uint32_t)0x8UL) /**< CTRL0_SS_SEL_SS3 Value */
 #define MXC_S_SPI_CTRL0_SS_SEL_SS3                     (MXC_V_SPI_CTRL0_SS_SEL_SS3 << MXC_F_SPI_CTRL0_SS_SEL_POS) /**< CTRL0_SS_SEL_SS3 Setting */

/**@} end of group SPI_CTRL0_Register */

/**
 * @ingroup  spi_registers
 * @defgroup SPI_CTRL1 SPI_CTRL1
 * @brief    Register for controlling SPI peripheral.
 * @{
 */
 #define MXC_F_SPI_CTRL1_TX_NUM_CHAR_POS                0 /**< CTRL1_TX_NUM_CHAR Position */
 #define MXC_F_SPI_CTRL1_TX_NUM_CHAR                    ((uint32_t)(0xFFFFUL << MXC_F_SPI_CTRL1_TX_NUM_CHAR_POS)) /**< CTRL1_TX_NUM_CHAR Mask */

 #define MXC_F_SPI_CTRL1_RX_NUM_CHAR_POS                16 /**< CTRL1_RX_NUM_CHAR Position */
 #define MXC_F_SPI_CTRL1_RX_NUM_CHAR                    ((uint32_t)(0xFFFFUL << MXC_F_SPI_CTRL1_RX_NUM_CHAR_POS)) /**< CTRL1_RX_NUM_CHAR Mask */

/**@} end of group SPI_CTRL1_Register */

/**
 * @ingroup  spi_registers
 * @defgroup SPI_CTRL2 SPI_CTRL2
 * @brief    Register for controlling SPI peripheral.
 * @{
 */
 #define MXC_F_SPI_CTRL2_CLK_PHA_POS                    0 /**< CTRL2_CLK_PHA Position */
 #define MXC_F_SPI_CTRL2_CLK_PHA                        ((uint32_t)(0x1UL << MXC_F_SPI_CTRL2_CLK_PHA_POS)) /**< CTRL2_CLK_PHA Mask */

 #define MXC_F_SPI_CTRL2_CLK_POL_POS                    1 /**< CTRL2_CLK_POL Position */
 #define MXC_F_SPI_CTRL2_CLK_POL                        ((uint32_t)(0x1UL << MXC_F_SPI_CTRL2_CLK_POL_POS)) /**< CTRL2_CLK_POL Mask */

 #define MXC_F_SPI_CTRL2_NUM_BITS_POS                   8 /**< CTRL2_NUM_BITS Position */
 #define MXC_F_SPI_CTRL2_NUM_BITS                       ((uint32_t)(0xFUL << MXC_F_SPI_CTRL2_NUM_BITS_POS)) /**< CTRL2_NUM_BITS Mask */
 #define MXC_V_SPI_CTRL2_NUM_BITS_0                     ((uint32_t)0x0UL) /**< CTRL2_NUM_BITS_0 Value */
 #define MXC_S_SPI_CTRL2_NUM_BITS_0                     (MXC_V_SPI_CTRL2_NUM_BITS_0 << MXC_F_SPI_CTRL2_NUM_BITS_POS) /**< CTRL2_NUM_BITS_0 Setting */

 #define MXC_F_SPI_CTRL2_DATA_WIDTH_POS                 12 /**< CTRL2_DATA_WIDTH Position */
 #define MXC_F_SPI_CTRL2_DATA_WIDTH                     ((uint32_t)(0x3UL << MXC_F_SPI_CTRL2_DATA_WIDTH_POS)) /**< CTRL2_DATA_WIDTH Mask */
 #define MXC_V_SPI_CTRL2_DATA_WIDTH_MONO                ((uint32_t)0x0UL) /**< CTRL2_DATA_WIDTH_MONO Value */
 #define MXC_S_SPI_CTRL2_DATA_WIDTH_MONO                (MXC_V_SPI_CTRL2_DATA_WIDTH_MONO << MXC_F_SPI_CTRL2_DATA_WIDTH_POS) /**< CTRL2_DATA_WIDTH_MONO Setting */
 #define MXC_V_SPI_CTRL2_DATA_WIDTH_DUAL                ((uint32_t)0x1UL) /**< CTRL2_DATA_WIDTH_DUAL Value */
 #define MXC_S_SPI_CTRL2_DATA_WIDTH_DUAL                (MXC_V_SPI_CTRL2_DATA_WIDTH_DUAL << MXC_F_SPI_CTRL2_DATA_WIDTH_POS) /**< CTRL2_DATA_WIDTH_DUAL Setting */
 #define MXC_V_SPI_CTRL2_DATA_WIDTH_QUAD                ((uint32_t)0x2UL) /**< CTRL2_DATA_WIDTH_QUAD Value */
 #define MXC_S_SPI_CTRL2_DATA_WIDTH_QUAD                (MXC_V_SPI_CTRL2_DATA_WIDTH_QUAD << MXC_F_SPI_CTRL2_DATA_WIDTH_POS) /**< CTRL2_DATA_WIDTH_QUAD Setting */

 #define MXC_F_SPI_CTRL2_THREE_WIRE_POS                 15 /**< CTRL2_THREE_WIRE Position */
 #define MXC_F_SPI_CTRL2_THREE_WIRE                     ((uint32_t)(0x1UL << MXC_F_SPI_CTRL2_THREE_WIRE_POS)) /**< CTRL2_THREE_WIRE Mask */

 #define MXC_F_SPI_CTRL2_SS_POL_POS                     16 /**< CTRL2_SS_POL Position */
 #define MXC_F_SPI_CTRL2_SS_POL                         ((uint32_t)(0x1UL << MXC_F_SPI_CTRL2_SS_POL_POS)) /**< CTRL2_SS_POL Mask */

/**@} end of group SPI_CTRL2_Register */

/**
 * @ingroup  spi_registers
 * @defgroup SPI_SS_TIME SPI_SS_TIME
 * @brief    Register for controlling SPI peripheral/Slave Select Timing.
 * @{
 */
 #define MXC_F_SPI_SS_TIME_SSACT1_POS                   0 /**< SS_TIME_SSACT1 Position */
 #define MXC_F_SPI_SS_TIME_SSACT1                       ((uint32_t)(0xFFUL << MXC_F_SPI_SS_TIME_SSACT1_POS)) /**< SS_TIME_SSACT1 Mask */
 #define MXC_V_SPI_SS_TIME_SSACT1_256                   ((uint32_t)0x0UL) /**< SS_TIME_SSACT1_256 Value */
 #define MXC_S_SPI_SS_TIME_SSACT1_256                   (MXC_V_SPI_SS_TIME_SSACT1_256 << MXC_F_SPI_SS_TIME_SSACT1_POS) /**< SS_TIME_SSACT1_256 Setting */

 #define MXC_F_SPI_SS_TIME_SSACT2_POS                   8 /**< SS_TIME_SSACT2 Position */
 #define MXC_F_SPI_SS_TIME_SSACT2                       ((uint32_t)(0xFFUL << MXC_F_SPI_SS_TIME_SSACT2_POS)) /**< SS_TIME_SSACT2 Mask */
 #define MXC_V_SPI_SS_TIME_SSACT2_256                   ((uint32_t)0x0UL) /**< SS_TIME_SSACT2_256 Value */
 #define MXC_S_SPI_SS_TIME_SSACT2_256                   (MXC_V_SPI_SS_TIME_SSACT2_256 << MXC_F_SPI_SS_TIME_SSACT2_POS) /**< SS_TIME_SSACT2_256 Setting */

 #define MXC_F_SPI_SS_TIME_SSINACT_POS                  16 /**< SS_TIME_SSINACT Position */
 #define MXC_F_SPI_SS_TIME_SSINACT                      ((uint32_t)(0xFFUL << MXC_F_SPI_SS_TIME_SSINACT_POS)) /**< SS_TIME_SSINACT Mask */
 #define MXC_V_SPI_SS_TIME_SSINACT_256                  ((uint32_t)0x0UL) /**< SS_TIME_SSINACT_256 Value */
 #define MXC_S_SPI_SS_TIME_SSINACT_256                  (MXC_V_SPI_SS_TIME_SSINACT_256 << MXC_F_SPI_SS_TIME_SSINACT_POS) /**< SS_TIME_SSINACT_256 Setting */

/**@} end of group SPI_SS_TIME_Register */

/**
 * @ingroup  spi_registers
 * @defgroup SPI_CLK_CFG SPI_CLK_CFG
 * @brief    Register for controlling SPI clock rate.
 * @{
 */
 #define MXC_F_SPI_CLK_CFG_LO_POS                       0 /**< CLK_CFG_LO Position */
 #define MXC_F_SPI_CLK_CFG_LO                           ((uint32_t)(0xFFUL << MXC_F_SPI_CLK_CFG_LO_POS)) /**< CLK_CFG_LO Mask */
 #define MXC_V_SPI_CLK_CFG_LO_DIS                       ((uint32_t)0x0UL) /**< CLK_CFG_LO_DIS Value */
 #define MXC_S_SPI_CLK_CFG_LO_DIS                       (MXC_V_SPI_CLK_CFG_LO_DIS << MXC_F_SPI_CLK_CFG_LO_POS) /**< CLK_CFG_LO_DIS Setting */

 #define MXC_F_SPI_CLK_CFG_HI_POS                       8 /**< CLK_CFG_HI Position */
 #define MXC_F_SPI_CLK_CFG_HI                           ((uint32_t)(0xFFUL << MXC_F_SPI_CLK_CFG_HI_POS)) /**< CLK_CFG_HI Mask */
 #define MXC_V_SPI_CLK_CFG_HI_DIS                       ((uint32_t)0x0UL) /**< CLK_CFG_HI_DIS Value */
 #define MXC_S_SPI_CLK_CFG_HI_DIS                       (MXC_V_SPI_CLK_CFG_HI_DIS << MXC_F_SPI_CLK_CFG_HI_POS) /**< CLK_CFG_HI_DIS Setting */

 #define MXC_F_SPI_CLK_CFG_SCALE_POS                    16 /**< CLK_CFG_SCALE Position */
 #define MXC_F_SPI_CLK_CFG_SCALE                        ((uint32_t)(0xFUL << MXC_F_SPI_CLK_CFG_SCALE_POS)) /**< CLK_CFG_SCALE Mask */

/**@} end of group SPI_CLK_CFG_Register */

/**
 * @ingroup  spi_registers
 * @defgroup SPI_DMA SPI_DMA
 * @brief    Register for controlling DMA.
 * @{
 */
 #define MXC_F_SPI_DMA_TX_FIFO_LEVEL_POS                0 /**< DMA_TX_FIFO_LEVEL Position */
 #define MXC_F_SPI_DMA_TX_FIFO_LEVEL                    ((uint32_t)(0x1FUL << MXC_F_SPI_DMA_TX_FIFO_LEVEL_POS)) /**< DMA_TX_FIFO_LEVEL Mask */

 #define MXC_F_SPI_DMA_TX_FIFO_EN_POS                   6 /**< DMA_TX_FIFO_EN Position */
 #define MXC_F_SPI_DMA_TX_FIFO_EN                       ((uint32_t)(0x1UL << MXC_F_SPI_DMA_TX_FIFO_EN_POS)) /**< DMA_TX_FIFO_EN Mask */

 #define MXC_F_SPI_DMA_TX_FIFO_CLEAR_POS                7 /**< DMA_TX_FIFO_CLEAR Position */
 #define MXC_F_SPI_DMA_TX_FIFO_CLEAR                    ((uint32_t)(0x1UL << MXC_F_SPI_DMA_TX_FIFO_CLEAR_POS)) /**< DMA_TX_FIFO_CLEAR Mask */

 #define MXC_F_SPI_DMA_TX_FIFO_CNT_POS                  8 /**< DMA_TX_FIFO_CNT Position */
 #define MXC_F_SPI_DMA_TX_FIFO_CNT                      ((uint32_t)(0x3FUL << MXC_F_SPI_DMA_TX_FIFO_CNT_POS)) /**< DMA_TX_FIFO_CNT Mask */

 #define MXC_F_SPI_DMA_TX_DMA_EN_POS                    15 /**< DMA_TX_DMA_EN Position */
 #define MXC_F_SPI_DMA_TX_DMA_EN                        ((uint32_t)(0x1UL << MXC_F_SPI_DMA_TX_DMA_EN_POS)) /**< DMA_TX_DMA_EN Mask */

 #define MXC_F_SPI_DMA_RX_FIFO_LEVEL_POS                16 /**< DMA_RX_FIFO_LEVEL Position */
 #define MXC_F_SPI_DMA_RX_FIFO_LEVEL                    ((uint32_t)(0x1FUL << MXC_F_SPI_DMA_RX_FIFO_LEVEL_POS)) /**< DMA_RX_FIFO_LEVEL Mask */

 #define MXC_F_SPI_DMA_RX_FIFO_EN_POS                   22 /**< DMA_RX_FIFO_EN Position */
 #define MXC_F_SPI_DMA_RX_FIFO_EN                       ((uint32_t)(0x1UL << MXC_F_SPI_DMA_RX_FIFO_EN_POS)) /**< DMA_RX_FIFO_EN Mask */

 #define MXC_F_SPI_DMA_RX_FIFO_CLEAR_POS                23 /**< DMA_RX_FIFO_CLEAR Position */
 #define MXC_F_SPI_DMA_RX_FIFO_CLEAR                    ((uint32_t)(0x1UL << MXC_F_SPI_DMA_RX_FIFO_CLEAR_POS)) /**< DMA_RX_FIFO_CLEAR Mask */

 #define MXC_F_SPI_DMA_RX_FIFO_CNT_POS                  24 /**< DMA_RX_FIFO_CNT Position */
 #define MXC_F_SPI_DMA_RX_FIFO_CNT                      ((uint32_t)(0x3FUL << MXC_F_SPI_DMA_RX_FIFO_CNT_POS)) /**< DMA_RX_FIFO_CNT Mask */

 #define MXC_F_SPI_DMA_RX_DMA_EN_POS                    31 /**< DMA_RX_DMA_EN Position */
 #define MXC_F_SPI_DMA_RX_DMA_EN                        ((uint32_t)(0x1UL << MXC_F_SPI_DMA_RX_DMA_EN_POS)) /**< DMA_RX_DMA_EN Mask */

/**@} end of group SPI_DMA_Register */

/**
 * @ingroup  spi_registers
 * @defgroup SPI_INT_FL SPI_INT_FL
 * @brief    Register for reading and clearing interrupt flags. All bits are write 1 to
 *           clear.
 * @{
 */
 #define MXC_F_SPI_INT_FL_TX_LEVEL_POS                  0 /**< INT_FL_TX_LEVEL Position */
 #define MXC_F_SPI_INT_FL_TX_LEVEL                      ((uint32_t)(0x1UL << MXC_F_SPI_INT_FL_TX_LEVEL_POS)) /**< INT_FL_TX_LEVEL Mask */

 #define MXC_F_SPI_INT_FL_TX_EMPTY_POS                  1 /**< INT_FL_TX_EMPTY Position */
 #define MXC_F_SPI_INT_FL_TX_EMPTY                      ((uint32_t)(0x1UL << MXC_F_SPI_INT_FL_TX_EMPTY_POS)) /**< INT_FL_TX_EMPTY Mask */

 #define MXC_F_SPI_INT_FL_RX_LEVEL_POS                  2 /**< INT_FL_RX_LEVEL Position */
 #define MXC_F_SPI_INT_FL_RX_LEVEL                      ((uint32_t)(0x1UL << MXC_F_SPI_INT_FL_RX_LEVEL_POS)) /**< INT_FL_RX_LEVEL Mask */

 #define MXC_F_SPI_INT_FL_RX_FULL_POS                   3 /**< INT_FL_RX_FULL Position */
 #define MXC_F_SPI_INT_FL_RX_FULL                       ((uint32_t)(0x1UL << MXC_F_SPI_INT_FL_RX_FULL_POS)) /**< INT_FL_RX_FULL Mask */

 #define MXC_F_SPI_INT_FL_SSA_POS                       4 /**< INT_FL_SSA Position */
 #define MXC_F_SPI_INT_FL_SSA                           ((uint32_t)(0x1UL << MXC_F_SPI_INT_FL_SSA_POS)) /**< INT_FL_SSA Mask */

 #define MXC_F_SPI_INT_FL_SSD_POS                       5 /**< INT_FL_SSD Position */
 #define MXC_F_SPI_INT_FL_SSD                           ((uint32_t)(0x1UL << MXC_F_SPI_INT_FL_SSD_POS)) /**< INT_FL_SSD Mask */

 #define MXC_F_SPI_INT_FL_ABORT_POS                     9 /**< INT_FL_ABORT Position */
 #define MXC_F_SPI_INT_FL_ABORT                         ((uint32_t)(0x1UL << MXC_F_SPI_INT_FL_ABORT_POS)) /**< INT_FL_ABORT Mask */

 #define MXC_F_SPI_INT_FL_M_DONE_POS                    11 /**< INT_FL_M_DONE Position */
 #define MXC_F_SPI_INT_FL_M_DONE                        ((uint32_t)(0x1UL << MXC_F_SPI_INT_FL_M_DONE_POS)) /**< INT_FL_M_DONE Mask */

 #define MXC_F_SPI_INT_FL_TX_OVR_POS                    12 /**< INT_FL_TX_OVR Position */
 #define MXC_F_SPI_INT_FL_TX_OVR                        ((uint32_t)(0x1UL << MXC_F_SPI_INT_FL_TX_OVR_POS)) /**< INT_FL_TX_OVR Mask */

 #define MXC_F_SPI_INT_FL_TX_UND_POS                    13 /**< INT_FL_TX_UND Position */
 #define MXC_F_SPI_INT_FL_TX_UND                        ((uint32_t)(0x1UL << MXC_F_SPI_INT_FL_TX_UND_POS)) /**< INT_FL_TX_UND Mask */

 #define MXC_F_SPI_INT_FL_RX_OVR_POS                    14 /**< INT_FL_RX_OVR Position */
 #define MXC_F_SPI_INT_FL_RX_OVR                        ((uint32_t)(0x1UL << MXC_F_SPI_INT_FL_RX_OVR_POS)) /**< INT_FL_RX_OVR Mask */

 #define MXC_F_SPI_INT_FL_RX_UND_POS                    15 /**< INT_FL_RX_UND Position */
 #define MXC_F_SPI_INT_FL_RX_UND                        ((uint32_t)(0x1UL << MXC_F_SPI_INT_FL_RX_UND_POS)) /**< INT_FL_RX_UND Mask */

/**@} end of group SPI_INT_FL_Register */

/**
 * @ingroup  spi_registers
 * @defgroup SPI_INT_EN SPI_INT_EN
 * @brief    Register for enabling interrupts.
 * @{
 */
 #define MXC_F_SPI_INT_EN_TX_LEVEL_POS                  0 /**< INT_EN_TX_LEVEL Position */
 #define MXC_F_SPI_INT_EN_TX_LEVEL                      ((uint32_t)(0x1UL << MXC_F_SPI_INT_EN_TX_LEVEL_POS)) /**< INT_EN_TX_LEVEL Mask */

 #define MXC_F_SPI_INT_EN_TX_EMPTY_POS                  1 /**< INT_EN_TX_EMPTY Position */
 #define MXC_F_SPI_INT_EN_TX_EMPTY                      ((uint32_t)(0x1UL << MXC_F_SPI_INT_EN_TX_EMPTY_POS)) /**< INT_EN_TX_EMPTY Mask */

 #define MXC_F_SPI_INT_EN_RX_LEVEL_POS                  2 /**< INT_EN_RX_LEVEL Position */
 #define MXC_F_SPI_INT_EN_RX_LEVEL                      ((uint32_t)(0x1UL << MXC_F_SPI_INT_EN_RX_LEVEL_POS)) /**< INT_EN_RX_LEVEL Mask */

 #define MXC_F_SPI_INT_EN_RX_FULL_POS                   3 /**< INT_EN_RX_FULL Position */
 #define MXC_F_SPI_INT_EN_RX_FULL                       ((uint32_t)(0x1UL << MXC_F_SPI_INT_EN_RX_FULL_POS)) /**< INT_EN_RX_FULL Mask */

 #define MXC_F_SPI_INT_EN_SSA_POS                       4 /**< INT_EN_SSA Position */
 #define MXC_F_SPI_INT_EN_SSA                           ((uint32_t)(0x1UL << MXC_F_SPI_INT_EN_SSA_POS)) /**< INT_EN_SSA Mask */

 #define MXC_F_SPI_INT_EN_SSD_POS                       5 /**< INT_EN_SSD Position */
 #define MXC_F_SPI_INT_EN_SSD                           ((uint32_t)(0x1UL << MXC_F_SPI_INT_EN_SSD_POS)) /**< INT_EN_SSD Mask */

 #define MXC_F_SPI_INT_EN_FAULT_POS                     8 /**< INT_EN_FAULT Position */
 #define MXC_F_SPI_INT_EN_FAULT                         ((uint32_t)(0x1UL << MXC_F_SPI_INT_EN_FAULT_POS)) /**< INT_EN_FAULT Mask */

 #define MXC_F_SPI_INT_EN_ABORT_POS                     9 /**< INT_EN_ABORT Position */
 #define MXC_F_SPI_INT_EN_ABORT                         ((uint32_t)(0x1UL << MXC_F_SPI_INT_EN_ABORT_POS)) /**< INT_EN_ABORT Mask */

 #define MXC_F_SPI_INT_EN_M_DONE_POS                    11 /**< INT_EN_M_DONE Position */
 #define MXC_F_SPI_INT_EN_M_DONE                        ((uint32_t)(0x1UL << MXC_F_SPI_INT_EN_M_DONE_POS)) /**< INT_EN_M_DONE Mask */

 #define MXC_F_SPI_INT_EN_TX_OVR_POS                    12 /**< INT_EN_TX_OVR Position */
 #define MXC_F_SPI_INT_EN_TX_OVR                        ((uint32_t)(0x1UL << MXC_F_SPI_INT_EN_TX_OVR_POS)) /**< INT_EN_TX_OVR Mask */

 #define MXC_F_SPI_INT_EN_TX_UND_POS                    13 /**< INT_EN_TX_UND Position */
 #define MXC_F_SPI_INT_EN_TX_UND                        ((uint32_t)(0x1UL << MXC_F_SPI_INT_EN_TX_UND_POS)) /**< INT_EN_TX_UND Mask */

 #define MXC_F_SPI_INT_EN_RX_OVR_POS                    14 /**< INT_EN_RX_OVR Position */
 #define MXC_F_SPI_INT_EN_RX_OVR                        ((uint32_t)(0x1UL << MXC_F_SPI_INT_EN_RX_OVR_POS)) /**< INT_EN_RX_OVR Mask */

 #define MXC_F_SPI_INT_EN_RX_UND_POS                    15 /**< INT_EN_RX_UND Position */
 #define MXC_F_SPI_INT_EN_RX_UND                        ((uint32_t)(0x1UL << MXC_F_SPI_INT_EN_RX_UND_POS)) /**< INT_EN_RX_UND Mask */

/**@} end of group SPI_INT_EN_Register */

/**
 * @ingroup  spi_registers
 * @defgroup SPI_WAKE_FL SPI_WAKE_FL
 * @brief    Register for wake up flags. All bits in this register are write 1 to clear.
 * @{
 */
 #define MXC_F_SPI_WAKE_FL_TX_LEVEL_POS                 0 /**< WAKE_FL_TX_LEVEL Position */
 #define MXC_F_SPI_WAKE_FL_TX_LEVEL                     ((uint32_t)(0x1UL << MXC_F_SPI_WAKE_FL_TX_LEVEL_POS)) /**< WAKE_FL_TX_LEVEL Mask */

 #define MXC_F_SPI_WAKE_FL_TX_EMPTY_POS                 1 /**< WAKE_FL_TX_EMPTY Position */
 #define MXC_F_SPI_WAKE_FL_TX_EMPTY                     ((uint32_t)(0x1UL << MXC_F_SPI_WAKE_FL_TX_EMPTY_POS)) /**< WAKE_FL_TX_EMPTY Mask */

 #define MXC_F_SPI_WAKE_FL_RX_LEVEL_POS                 2 /**< WAKE_FL_RX_LEVEL Position */
 #define MXC_F_SPI_WAKE_FL_RX_LEVEL                     ((uint32_t)(0x1UL << MXC_F_SPI_WAKE_FL_RX_LEVEL_POS)) /**< WAKE_FL_RX_LEVEL Mask */

 #define MXC_F_SPI_WAKE_FL_RX_FULL_POS                  3 /**< WAKE_FL_RX_FULL Position */
 #define MXC_F_SPI_WAKE_FL_RX_FULL                      ((uint32_t)(0x1UL << MXC_F_SPI_WAKE_FL_RX_FULL_POS)) /**< WAKE_FL_RX_FULL Mask */

/**@} end of group SPI_WAKE_FL_Register */

/**
 * @ingroup  spi_registers
 * @defgroup SPI_WAKE_EN SPI_WAKE_EN
 * @brief    Register for wake up enable.
 * @{
 */
 #define MXC_F_SPI_WAKE_EN_TX_LEVEL_POS                 0 /**< WAKE_EN_TX_LEVEL Position */
 #define MXC_F_SPI_WAKE_EN_TX_LEVEL                     ((uint32_t)(0x1UL << MXC_F_SPI_WAKE_EN_TX_LEVEL_POS)) /**< WAKE_EN_TX_LEVEL Mask */

 #define MXC_F_SPI_WAKE_EN_TX_EMPTY_POS                 1 /**< WAKE_EN_TX_EMPTY Position */
 #define MXC_F_SPI_WAKE_EN_TX_EMPTY                     ((uint32_t)(0x1UL << MXC_F_SPI_WAKE_EN_TX_EMPTY_POS)) /**< WAKE_EN_TX_EMPTY Mask */

 #define MXC_F_SPI_WAKE_EN_RX_LEVEL_POS                 2 /**< WAKE_EN_RX_LEVEL Position */
 #define MXC_F_SPI_WAKE_EN_RX_LEVEL                     ((uint32_t)(0x1UL << MXC_F_SPI_WAKE_EN_RX_LEVEL_POS)) /**< WAKE_EN_RX_LEVEL Mask */

 #define MXC_F_SPI_WAKE_EN_RX_FULL_POS                  3 /**< WAKE_EN_RX_FULL Position */
 #define MXC_F_SPI_WAKE_EN_RX_FULL                      ((uint32_t)(0x1UL << MXC_F_SPI_WAKE_EN_RX_FULL_POS)) /**< WAKE_EN_RX_FULL Mask */

/**@} end of group SPI_WAKE_EN_Register */

/**
 * @ingroup  spi_registers
 * @defgroup SPI_STAT SPI_STAT
 * @brief    SPI Status register.
 * @{
 */
 #define MXC_F_SPI_STAT_BUSY_POS                        0 /**< STAT_BUSY Position */
 #define MXC_F_SPI_STAT_BUSY                            ((uint32_t)(0x1UL << MXC_F_SPI_STAT_BUSY_POS)) /**< STAT_BUSY Mask */

/**@} end of group SPI_STAT_Register */

#ifdef __cplusplus
}
#endif

#endif /* _SPI_REGS_H_ */
