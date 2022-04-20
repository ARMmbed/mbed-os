/**
 * @file    i2c_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the I2C Peripheral Module.
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

#ifndef _I2C_REVA_REGS_H_
#define _I2C_REVA_REGS_H_

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
 * @ingroup     i2c
 * @defgroup    i2c_registers I2C_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the I2C Peripheral Module.
 * @details Inter-Integrated Circuit.
 */

/**
 * @ingroup i2c_registers
 * Structure type to access the I2C Registers.
 */
typedef struct {
    __IO uint32_t ctrl;                 /**< <tt>\b 0x00:</tt> I2C CTRL Register */
    __IO uint32_t status;               /**< <tt>\b 0x04:</tt> I2C STATUS Register */
    __IO uint32_t intfl0;               /**< <tt>\b 0x08:</tt> I2C INTFL0 Register */
    __IO uint32_t inten0;               /**< <tt>\b 0x0C:</tt> I2C INTEN0 Register */
    __IO uint32_t intfl1;               /**< <tt>\b 0x10:</tt> I2C INTFL1 Register */
    __IO uint32_t inten1;               /**< <tt>\b 0x14:</tt> I2C INTEN1 Register */
    __IO uint32_t fifolen;              /**< <tt>\b 0x18:</tt> I2C FIFOLEN Register */
    __IO uint32_t rxctrl0;              /**< <tt>\b 0x1C:</tt> I2C RXCTRL0 Register */
    __IO uint32_t rxctrl1;              /**< <tt>\b 0x20:</tt> I2C RXCTRL1 Register */
    __IO uint32_t txctrl0;              /**< <tt>\b 0x24:</tt> I2C TXCTRL0 Register */
    __IO uint32_t txctrl1;              /**< <tt>\b 0x28:</tt> I2C TXCTRL1 Register */
    __IO uint32_t fifo;                 /**< <tt>\b 0x2C:</tt> I2C FIFO Register */
    __IO uint32_t mstctrl;              /**< <tt>\b 0x30:</tt> I2C MSTCTRL Register */
    __IO uint32_t clklo;                /**< <tt>\b 0x34:</tt> I2C CLKLO Register */
    __IO uint32_t clkhi;                /**< <tt>\b 0x38:</tt> I2C CLKHI Register */
    __IO uint32_t hsclk;                /**< <tt>\b 0x3C:</tt> I2C HSCLK Register */
    __IO uint32_t timeout;              /**< <tt>\b 0x40:</tt> I2C TIMEOUT Register */
    __R  uint32_t rsv_0x44;
    __IO uint32_t dma;                  /**< <tt>\b 0x48:</tt> I2C DMA Register */
    __IO uint32_t slave;                /**< <tt>\b 0x4C:</tt> I2C SLAVE Register */
} mxc_i2c_reva_regs_t;

/* Register offsets for module I2C */
/**
 * @ingroup    i2c_registers
 * @defgroup   I2C_Register_Offsets Register Offsets
 * @brief      I2C Peripheral Register Offsets from the I2C Base Peripheral Address. 
 * @{
 */
 #define MXC_R_I2C_REVA_CTRL                     ((uint32_t)0x00000000UL) /**< Offset from I2C Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_I2C_REVA_STATUS                   ((uint32_t)0x00000004UL) /**< Offset from I2C Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_I2C_REVA_INTFL0                   ((uint32_t)0x00000008UL) /**< Offset from I2C Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_I2C_REVA_INTEN0                   ((uint32_t)0x0000000CUL) /**< Offset from I2C Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_I2C_REVA_INTFL1                   ((uint32_t)0x00000010UL) /**< Offset from I2C Base Address: <tt> 0x0010</tt> */ 
 #define MXC_R_I2C_REVA_INTEN1                   ((uint32_t)0x00000014UL) /**< Offset from I2C Base Address: <tt> 0x0014</tt> */ 
 #define MXC_R_I2C_REVA_FIFOLEN                  ((uint32_t)0x00000018UL) /**< Offset from I2C Base Address: <tt> 0x0018</tt> */ 
 #define MXC_R_I2C_REVA_RXCTRL0                  ((uint32_t)0x0000001CUL) /**< Offset from I2C Base Address: <tt> 0x001C</tt> */ 
 #define MXC_R_I2C_REVA_RXCTRL1                  ((uint32_t)0x00000020UL) /**< Offset from I2C Base Address: <tt> 0x0020</tt> */ 
 #define MXC_R_I2C_REVA_TXCTRL0                  ((uint32_t)0x00000024UL) /**< Offset from I2C Base Address: <tt> 0x0024</tt> */ 
 #define MXC_R_I2C_REVA_TXCTRL1                  ((uint32_t)0x00000028UL) /**< Offset from I2C Base Address: <tt> 0x0028</tt> */ 
 #define MXC_R_I2C_REVA_FIFO                     ((uint32_t)0x0000002CUL) /**< Offset from I2C Base Address: <tt> 0x002C</tt> */ 
 #define MXC_R_I2C_REVA_MSTCTRL                  ((uint32_t)0x00000030UL) /**< Offset from I2C Base Address: <tt> 0x0030</tt> */ 
 #define MXC_R_I2C_REVA_CLKLO                    ((uint32_t)0x00000034UL) /**< Offset from I2C Base Address: <tt> 0x0034</tt> */ 
 #define MXC_R_I2C_REVA_CLKHI                    ((uint32_t)0x00000038UL) /**< Offset from I2C Base Address: <tt> 0x0038</tt> */ 
 #define MXC_R_I2C_REVA_HSCLK                    ((uint32_t)0x0000003CUL) /**< Offset from I2C Base Address: <tt> 0x003C</tt> */ 
 #define MXC_R_I2C_REVA_TIMEOUT                  ((uint32_t)0x00000040UL) /**< Offset from I2C Base Address: <tt> 0x0040</tt> */ 
 #define MXC_R_I2C_REVA_DMA                      ((uint32_t)0x00000048UL) /**< Offset from I2C Base Address: <tt> 0x0048</tt> */ 
 #define MXC_R_I2C_REVA_SLAVE                    ((uint32_t)0x0000004CUL) /**< Offset from I2C Base Address: <tt> 0x004C</tt> */ 
/**@} end of group i2c_registers */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_CTRL I2C_CTRL
 * @brief    Control Register0.
 * @{
 */
 #define MXC_F_I2C_REVA_CTRL_EN_POS                          0 /**< CTRL_EN Position */
 #define MXC_F_I2C_REVA_CTRL_EN                              ((uint32_t)(0x1UL << MXC_F_I2C_REVA_CTRL_EN_POS)) /**< CTRL_EN Mask */

 #define MXC_F_I2C_REVA_CTRL_MST_MODE_POS                    1 /**< CTRL_MST_MODE Position */
 #define MXC_F_I2C_REVA_CTRL_MST_MODE                        ((uint32_t)(0x1UL << MXC_F_I2C_REVA_CTRL_MST_MODE_POS)) /**< CTRL_MST_MODE Mask */

 #define MXC_F_I2C_REVA_CTRL_GC_ADDR_EN_POS                  2 /**< CTRL_GC_ADDR_EN Position */
 #define MXC_F_I2C_REVA_CTRL_GC_ADDR_EN                      ((uint32_t)(0x1UL << MXC_F_I2C_REVA_CTRL_GC_ADDR_EN_POS)) /**< CTRL_GC_ADDR_EN Mask */

 #define MXC_F_I2C_REVA_CTRL_IRXM_EN_POS                     3 /**< CTRL_IRXM_EN Position */
 #define MXC_F_I2C_REVA_CTRL_IRXM_EN                         ((uint32_t)(0x1UL << MXC_F_I2C_REVA_CTRL_IRXM_EN_POS)) /**< CTRL_IRXM_EN Mask */

 #define MXC_F_I2C_REVA_CTRL_IRXM_ACK_POS                    4 /**< CTRL_IRXM_ACK Position */
 #define MXC_F_I2C_REVA_CTRL_IRXM_ACK                        ((uint32_t)(0x1UL << MXC_F_I2C_REVA_CTRL_IRXM_ACK_POS)) /**< CTRL_IRXM_ACK Mask */

 #define MXC_F_I2C_REVA_CTRL_SCL_OUT_POS                     6 /**< CTRL_SCL_OUT Position */
 #define MXC_F_I2C_REVA_CTRL_SCL_OUT                         ((uint32_t)(0x1UL << MXC_F_I2C_REVA_CTRL_SCL_OUT_POS)) /**< CTRL_SCL_OUT Mask */

 #define MXC_F_I2C_REVA_CTRL_SDA_OUT_POS                     7 /**< CTRL_SDA_OUT Position */
 #define MXC_F_I2C_REVA_CTRL_SDA_OUT                         ((uint32_t)(0x1UL << MXC_F_I2C_REVA_CTRL_SDA_OUT_POS)) /**< CTRL_SDA_OUT Mask */

 #define MXC_F_I2C_REVA_CTRL_SCL_POS                         8 /**< CTRL_SCL Position */
 #define MXC_F_I2C_REVA_CTRL_SCL                             ((uint32_t)(0x1UL << MXC_F_I2C_REVA_CTRL_SCL_POS)) /**< CTRL_SCL Mask */

 #define MXC_F_I2C_REVA_CTRL_SDA_POS                         9 /**< CTRL_SDA Position */
 #define MXC_F_I2C_REVA_CTRL_SDA                             ((uint32_t)(0x1UL << MXC_F_I2C_REVA_CTRL_SDA_POS)) /**< CTRL_SDA Mask */

 #define MXC_F_I2C_REVA_CTRL_BB_MODE_POS                     10 /**< CTRL_BB_MODE Position */
 #define MXC_F_I2C_REVA_CTRL_BB_MODE                         ((uint32_t)(0x1UL << MXC_F_I2C_REVA_CTRL_BB_MODE_POS)) /**< CTRL_BB_MODE Mask */

 #define MXC_F_I2C_REVA_CTRL_READ_POS                        11 /**< CTRL_READ Position */
 #define MXC_F_I2C_REVA_CTRL_READ                            ((uint32_t)(0x1UL << MXC_F_I2C_REVA_CTRL_READ_POS)) /**< CTRL_READ Mask */

 #define MXC_F_I2C_REVA_CTRL_CLKSTR_DIS_POS                  12 /**< CTRL_CLKSTR_DIS Position */
 #define MXC_F_I2C_REVA_CTRL_CLKSTR_DIS                      ((uint32_t)(0x1UL << MXC_F_I2C_REVA_CTRL_CLKSTR_DIS_POS)) /**< CTRL_CLKSTR_DIS Mask */

 #define MXC_F_I2C_REVA_CTRL_ONE_MST_MODE_POS                13 /**< CTRL_ONE_MST_MODE Position */
 #define MXC_F_I2C_REVA_CTRL_ONE_MST_MODE                    ((uint32_t)(0x1UL << MXC_F_I2C_REVA_CTRL_ONE_MST_MODE_POS)) /**< CTRL_ONE_MST_MODE Mask */

 #define MXC_F_I2C_REVA_CTRL_HS_EN_POS                       15 /**< CTRL_HS_EN Position */
 #define MXC_F_I2C_REVA_CTRL_HS_EN                           ((uint32_t)(0x1UL << MXC_F_I2C_REVA_CTRL_HS_EN_POS)) /**< CTRL_HS_EN Mask */

/**@} end of group I2C_CTRL_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_STATUS I2C_STATUS
 * @brief    Status Register.
 * @{
 */
 #define MXC_F_I2C_REVA_STATUS_BUSY_POS                      0 /**< STATUS_BUSY Position */
 #define MXC_F_I2C_REVA_STATUS_BUSY                          ((uint32_t)(0x1UL << MXC_F_I2C_REVA_STATUS_BUSY_POS)) /**< STATUS_BUSY Mask */

 #define MXC_F_I2C_REVA_STATUS_RX_EM_POS                     1 /**< STATUS_RX_EM Position */
 #define MXC_F_I2C_REVA_STATUS_RX_EM                         ((uint32_t)(0x1UL << MXC_F_I2C_REVA_STATUS_RX_EM_POS)) /**< STATUS_RX_EM Mask */

 #define MXC_F_I2C_REVA_STATUS_RX_FULL_POS                   2 /**< STATUS_RX_FULL Position */
 #define MXC_F_I2C_REVA_STATUS_RX_FULL                       ((uint32_t)(0x1UL << MXC_F_I2C_REVA_STATUS_RX_FULL_POS)) /**< STATUS_RX_FULL Mask */

 #define MXC_F_I2C_REVA_STATUS_TX_EM_POS                     3 /**< STATUS_TX_EM Position */
 #define MXC_F_I2C_REVA_STATUS_TX_EM                         ((uint32_t)(0x1UL << MXC_F_I2C_REVA_STATUS_TX_EM_POS)) /**< STATUS_TX_EM Mask */

 #define MXC_F_I2C_REVA_STATUS_TX_FULL_POS                   4 /**< STATUS_TX_FULL Position */
 #define MXC_F_I2C_REVA_STATUS_TX_FULL                       ((uint32_t)(0x1UL << MXC_F_I2C_REVA_STATUS_TX_FULL_POS)) /**< STATUS_TX_FULL Mask */

 #define MXC_F_I2C_REVA_STATUS_MST_BUSY_POS                  5 /**< STATUS_MST_BUSY Position */
 #define MXC_F_I2C_REVA_STATUS_MST_BUSY                      ((uint32_t)(0x1UL << MXC_F_I2C_REVA_STATUS_MST_BUSY_POS)) /**< STATUS_MST_BUSY Mask */

/**@} end of group I2C_STATUS_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_INTFL0 I2C_INTFL0
 * @brief    Interrupt Status Register.
 * @{
 */
 #define MXC_F_I2C_REVA_INTFL0_DONE_POS                      0 /**< INTFL0_DONE Position */
 #define MXC_F_I2C_REVA_INTFL0_DONE                          ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_DONE_POS)) /**< INTFL0_DONE Mask */

 #define MXC_F_I2C_REVA_INTFL0_IRXM_POS                      1 /**< INTFL0_IRXM Position */
 #define MXC_F_I2C_REVA_INTFL0_IRXM                          ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_IRXM_POS)) /**< INTFL0_IRXM Mask */

 #define MXC_F_I2C_REVA_INTFL0_GC_ADDR_MATCH_POS             2 /**< INTFL0_GC_ADDR_MATCH Position */
 #define MXC_F_I2C_REVA_INTFL0_GC_ADDR_MATCH                 ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_GC_ADDR_MATCH_POS)) /**< INTFL0_GC_ADDR_MATCH Mask */

 #define MXC_F_I2C_REVA_INTFL0_ADDR_MATCH_POS                3 /**< INTFL0_ADDR_MATCH Position */
 #define MXC_F_I2C_REVA_INTFL0_ADDR_MATCH                    ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_ADDR_MATCH_POS)) /**< INTFL0_ADDR_MATCH Mask */

 #define MXC_F_I2C_REVA_INTFL0_RX_THD_POS                    4 /**< INTFL0_RX_THD Position */
 #define MXC_F_I2C_REVA_INTFL0_RX_THD                        ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_RX_THD_POS)) /**< INTFL0_RX_THD Mask */

 #define MXC_F_I2C_REVA_INTFL0_TX_THD_POS                    5 /**< INTFL0_TX_THD Position */
 #define MXC_F_I2C_REVA_INTFL0_TX_THD                        ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_TX_THD_POS)) /**< INTFL0_TX_THD Mask */

 #define MXC_F_I2C_REVA_INTFL0_STOP_POS                      6 /**< INTFL0_STOP Position */
 #define MXC_F_I2C_REVA_INTFL0_STOP                          ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_STOP_POS)) /**< INTFL0_STOP Mask */

 #define MXC_F_I2C_REVA_INTFL0_ADDR_ACK_POS                  7 /**< INTFL0_ADDR_ACK Position */
 #define MXC_F_I2C_REVA_INTFL0_ADDR_ACK                      ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_ADDR_ACK_POS)) /**< INTFL0_ADDR_ACK Mask */

 #define MXC_F_I2C_REVA_INTFL0_ARB_ERR_POS                   8 /**< INTFL0_ARB_ERR Position */
 #define MXC_F_I2C_REVA_INTFL0_ARB_ERR                       ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_ARB_ERR_POS)) /**< INTFL0_ARB_ERR Mask */

 #define MXC_F_I2C_REVA_INTFL0_TO_ERR_POS                    9 /**< INTFL0_TO_ERR Position */
 #define MXC_F_I2C_REVA_INTFL0_TO_ERR                        ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_TO_ERR_POS)) /**< INTFL0_TO_ERR Mask */

 #define MXC_F_I2C_REVA_INTFL0_ADDR_NACK_ERR_POS             10 /**< INTFL0_ADDR_NACK_ERR Position */
 #define MXC_F_I2C_REVA_INTFL0_ADDR_NACK_ERR                 ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_ADDR_NACK_ERR_POS)) /**< INTFL0_ADDR_NACK_ERR Mask */

 #define MXC_F_I2C_REVA_INTFL0_DATA_ERR_POS                  11 /**< INTFL0_DATA_ERR Position */
 #define MXC_F_I2C_REVA_INTFL0_DATA_ERR                      ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_DATA_ERR_POS)) /**< INTFL0_DATA_ERR Mask */

 #define MXC_F_I2C_REVA_INTFL0_DNR_ERR_POS                   12 /**< INTFL0_DNR_ERR Position */
 #define MXC_F_I2C_REVA_INTFL0_DNR_ERR                       ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_DNR_ERR_POS)) /**< INTFL0_DNR_ERR Mask */

 #define MXC_F_I2C_REVA_INTFL0_START_ERR_POS                 13 /**< INTFL0_START_ERR Position */
 #define MXC_F_I2C_REVA_INTFL0_START_ERR                     ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_START_ERR_POS)) /**< INTFL0_START_ERR Mask */

 #define MXC_F_I2C_REVA_INTFL0_STOP_ERR_POS                  14 /**< INTFL0_STOP_ERR Position */
 #define MXC_F_I2C_REVA_INTFL0_STOP_ERR                      ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_STOP_ERR_POS)) /**< INTFL0_STOP_ERR Mask */

 #define MXC_F_I2C_REVA_INTFL0_TX_LOCKOUT_POS                15 /**< INTFL0_TX_LOCKOUT Position */
 #define MXC_F_I2C_REVA_INTFL0_TX_LOCKOUT                    ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_TX_LOCKOUT_POS)) /**< INTFL0_TX_LOCKOUT Mask */

 #define MXC_F_I2C_REVA_INTFL0_MAMI_POS                      16 /**< INTFL0_MAMI Position */
 #define MXC_F_I2C_REVA_INTFL0_MAMI                          ((uint32_t)(0x3FUL << MXC_F_I2C_REVA_INTFL0_MAMI_POS)) /**< INTFL0_MAMI Mask */

 #define MXC_F_I2C_REVA_INTFL0_RD_ADDR_MATCH_POS             22 /**< INTFL0_RD_ADDR_MATCH Position */
 #define MXC_F_I2C_REVA_INTFL0_RD_ADDR_MATCH                 ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_RD_ADDR_MATCH_POS)) /**< INTFL0_RD_ADDR_MATCH Mask */

 #define MXC_F_I2C_REVA_INTFL0_WR_ADDR_MATCH_POS             23 /**< INTFL0_WR_ADDR_MATCH Position */
 #define MXC_F_I2C_REVA_INTFL0_WR_ADDR_MATCH                 ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL0_WR_ADDR_MATCH_POS)) /**< INTFL0_WR_ADDR_MATCH Mask */

/**@} end of group I2C_INTFL0_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_INTEN0 I2C_INTEN0
 * @brief    Interrupt Enable Register.
 * @{
 */
 #define MXC_F_I2C_REVA_INTEN0_DONE_POS                      0 /**< INTEN0_DONE Position */
 #define MXC_F_I2C_REVA_INTEN0_DONE                          ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_DONE_POS)) /**< INTEN0_DONE Mask */

 #define MXC_F_I2C_REVA_INTEN0_IRXM_POS                      1 /**< INTEN0_IRXM Position */
 #define MXC_F_I2C_REVA_INTEN0_IRXM                          ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_IRXM_POS)) /**< INTEN0_IRXM Mask */

 #define MXC_F_I2C_REVA_INTEN0_GC_ADDR_MATCH_POS             2 /**< INTEN0_GC_ADDR_MATCH Position */
 #define MXC_F_I2C_REVA_INTEN0_GC_ADDR_MATCH                 ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_GC_ADDR_MATCH_POS)) /**< INTEN0_GC_ADDR_MATCH Mask */

 #define MXC_F_I2C_REVA_INTEN0_ADDR_MATCH_POS                3 /**< INTEN0_ADDR_MATCH Position */
 #define MXC_F_I2C_REVA_INTEN0_ADDR_MATCH                    ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_ADDR_MATCH_POS)) /**< INTEN0_ADDR_MATCH Mask */

 #define MXC_F_I2C_REVA_INTEN0_RX_THD_POS                    4 /**< INTEN0_RX_THD Position */
 #define MXC_F_I2C_REVA_INTEN0_RX_THD                        ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_RX_THD_POS)) /**< INTEN0_RX_THD Mask */

 #define MXC_F_I2C_REVA_INTEN0_TX_THD_POS                    5 /**< INTEN0_TX_THD Position */
 #define MXC_F_I2C_REVA_INTEN0_TX_THD                        ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_TX_THD_POS)) /**< INTEN0_TX_THD Mask */

 #define MXC_F_I2C_REVA_INTEN0_STOP_POS                      6 /**< INTEN0_STOP Position */
 #define MXC_F_I2C_REVA_INTEN0_STOP                          ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_STOP_POS)) /**< INTEN0_STOP Mask */

 #define MXC_F_I2C_REVA_INTEN0_ADDR_ACK_POS                  7 /**< INTEN0_ADDR_ACK Position */
 #define MXC_F_I2C_REVA_INTEN0_ADDR_ACK                      ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_ADDR_ACK_POS)) /**< INTEN0_ADDR_ACK Mask */

 #define MXC_F_I2C_REVA_INTEN0_ARB_ERR_POS                   8 /**< INTEN0_ARB_ERR Position */
 #define MXC_F_I2C_REVA_INTEN0_ARB_ERR                       ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_ARB_ERR_POS)) /**< INTEN0_ARB_ERR Mask */

 #define MXC_F_I2C_REVA_INTEN0_TO_ERR_POS                    9 /**< INTEN0_TO_ERR Position */
 #define MXC_F_I2C_REVA_INTEN0_TO_ERR                        ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_TO_ERR_POS)) /**< INTEN0_TO_ERR Mask */

 #define MXC_F_I2C_REVA_INTEN0_ADDR_NACK_ERR_POS             10 /**< INTEN0_ADDR_NACK_ERR Position */
 #define MXC_F_I2C_REVA_INTEN0_ADDR_NACK_ERR                 ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_ADDR_NACK_ERR_POS)) /**< INTEN0_ADDR_NACK_ERR Mask */

 #define MXC_F_I2C_REVA_INTEN0_DATA_ERR_POS                  11 /**< INTEN0_DATA_ERR Position */
 #define MXC_F_I2C_REVA_INTEN0_DATA_ERR                      ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_DATA_ERR_POS)) /**< INTEN0_DATA_ERR Mask */

 #define MXC_F_I2C_REVA_INTEN0_DNR_ERR_POS                   12 /**< INTEN0_DNR_ERR Position */
 #define MXC_F_I2C_REVA_INTEN0_DNR_ERR                       ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_DNR_ERR_POS)) /**< INTEN0_DNR_ERR Mask */

 #define MXC_F_I2C_REVA_INTEN0_START_ERR_POS                 13 /**< INTEN0_START_ERR Position */
 #define MXC_F_I2C_REVA_INTEN0_START_ERR                     ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_START_ERR_POS)) /**< INTEN0_START_ERR Mask */

 #define MXC_F_I2C_REVA_INTEN0_STOP_ERR_POS                  14 /**< INTEN0_STOP_ERR Position */
 #define MXC_F_I2C_REVA_INTEN0_STOP_ERR                      ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_STOP_ERR_POS)) /**< INTEN0_STOP_ERR Mask */

 #define MXC_F_I2C_REVA_INTEN0_TX_LOCKOUT_POS                15 /**< INTEN0_TX_LOCKOUT Position */
 #define MXC_F_I2C_REVA_INTEN0_TX_LOCKOUT                    ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_TX_LOCKOUT_POS)) /**< INTEN0_TX_LOCKOUT Mask */

 #define MXC_F_I2C_REVA_INTEN0_MAMI_POS                      16 /**< INTEN0_MAMI Position */
 #define MXC_F_I2C_REVA_INTEN0_MAMI                          ((uint32_t)(0x3FUL << MXC_F_I2C_REVA_INTEN0_MAMI_POS)) /**< INTEN0_MAMI Mask */

 #define MXC_F_I2C_REVA_INTEN0_RD_ADDR_MATCH_POS             22 /**< INTEN0_RD_ADDR_MATCH Position */
 #define MXC_F_I2C_REVA_INTEN0_RD_ADDR_MATCH                 ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_RD_ADDR_MATCH_POS)) /**< INTEN0_RD_ADDR_MATCH Mask */

 #define MXC_F_I2C_REVA_INTEN0_WR_ADDR_MATCH_POS             23 /**< INTEN0_WR_ADDR_MATCH Position */
 #define MXC_F_I2C_REVA_INTEN0_WR_ADDR_MATCH                 ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN0_WR_ADDR_MATCH_POS)) /**< INTEN0_WR_ADDR_MATCH Mask */

/**@} end of group I2C_INTEN0_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_INTFL1 I2C_INTFL1
 * @brief    Interrupt Status Register 1.
 * @{
 */
 #define MXC_F_I2C_REVA_INTFL1_RX_OV_POS                     0 /**< INTFL1_RX_OV Position */
 #define MXC_F_I2C_REVA_INTFL1_RX_OV                         ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL1_RX_OV_POS)) /**< INTFL1_RX_OV Mask */

 #define MXC_F_I2C_REVA_INTFL1_TX_UN_POS                     1 /**< INTFL1_TX_UN Position */
 #define MXC_F_I2C_REVA_INTFL1_TX_UN                         ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL1_TX_UN_POS)) /**< INTFL1_TX_UN Mask */

 #define MXC_F_I2C_REVA_INTFL1_START_POS                     2 /**< INTFL1_START Position */
 #define MXC_F_I2C_REVA_INTFL1_START                         ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTFL1_START_POS)) /**< INTFL1_START Mask */

/**@} end of group I2C_INTFL1_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_INTEN1 I2C_INTEN1
 * @brief    Interrupt Staus Register 1.
 * @{
 */
 #define MXC_F_I2C_REVA_INTEN1_RX_OV_POS                     0 /**< INTEN1_RX_OV Position */
 #define MXC_F_I2C_REVA_INTEN1_RX_OV                         ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN1_RX_OV_POS)) /**< INTEN1_RX_OV Mask */

 #define MXC_F_I2C_REVA_INTEN1_TX_UN_POS                     1 /**< INTEN1_TX_UN Position */
 #define MXC_F_I2C_REVA_INTEN1_TX_UN                         ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN1_TX_UN_POS)) /**< INTEN1_TX_UN Mask */

 #define MXC_F_I2C_REVA_INTEN1_START_POS                     2 /**< INTEN1_START Position */
 #define MXC_F_I2C_REVA_INTEN1_START                         ((uint32_t)(0x1UL << MXC_F_I2C_REVA_INTEN1_START_POS)) /**< INTEN1_START Mask */

/**@} end of group I2C_INTEN1_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_FIFOLEN I2C_FIFOLEN
 * @brief    FIFO Configuration Register.
 * @{
 */
 #define MXC_F_I2C_REVA_FIFOLEN_RX_DEPTH_POS                 0 /**< FIFOLEN_RX_DEPTH Position */
 #define MXC_F_I2C_REVA_FIFOLEN_RX_DEPTH                     ((uint32_t)(0xFFUL << MXC_F_I2C_REVA_FIFOLEN_RX_DEPTH_POS)) /**< FIFOLEN_RX_DEPTH Mask */

 #define MXC_F_I2C_REVA_FIFOLEN_TX_DEPTH_POS                 8 /**< FIFOLEN_TX_DEPTH Position */
 #define MXC_F_I2C_REVA_FIFOLEN_TX_DEPTH                     ((uint32_t)(0xFFUL << MXC_F_I2C_REVA_FIFOLEN_TX_DEPTH_POS)) /**< FIFOLEN_TX_DEPTH Mask */

/**@} end of group I2C_FIFOLEN_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_RXCTRL0 I2C_RXCTRL0
 * @brief    Receive Control Register 0.
 * @{
 */
 #define MXC_F_I2C_REVA_RXCTRL0_DNR_POS                      0 /**< RXCTRL0_DNR Position */
 #define MXC_F_I2C_REVA_RXCTRL0_DNR                          ((uint32_t)(0x1UL << MXC_F_I2C_REVA_RXCTRL0_DNR_POS)) /**< RXCTRL0_DNR Mask */

 #define MXC_F_I2C_REVA_RXCTRL0_FLUSH_POS                    7 /**< RXCTRL0_FLUSH Position */
 #define MXC_F_I2C_REVA_RXCTRL0_FLUSH                        ((uint32_t)(0x1UL << MXC_F_I2C_REVA_RXCTRL0_FLUSH_POS)) /**< RXCTRL0_FLUSH Mask */

 #define MXC_F_I2C_REVA_RXCTRL0_THD_LVL_POS                  8 /**< RXCTRL0_THD_LVL Position */
 #define MXC_F_I2C_REVA_RXCTRL0_THD_LVL                      ((uint32_t)(0xFUL << MXC_F_I2C_REVA_RXCTRL0_THD_LVL_POS)) /**< RXCTRL0_THD_LVL Mask */

/**@} end of group I2C_RXCTRL0_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_RXCTRL1 I2C_RXCTRL1
 * @brief    Receive Control Register 1.
 * @{
 */
 #define MXC_F_I2C_REVA_RXCTRL1_CNT_POS                      0 /**< RXCTRL1_CNT Position */
 #define MXC_F_I2C_REVA_RXCTRL1_CNT                          ((uint32_t)(0xFFUL << MXC_F_I2C_REVA_RXCTRL1_CNT_POS)) /**< RXCTRL1_CNT Mask */

 #define MXC_F_I2C_REVA_RXCTRL1_LVL_POS                      8 /**< RXCTRL1_LVL Position */
 #define MXC_F_I2C_REVA_RXCTRL1_LVL                          ((uint32_t)(0xFUL << MXC_F_I2C_REVA_RXCTRL1_LVL_POS)) /**< RXCTRL1_LVL Mask */

/**@} end of group I2C_RXCTRL1_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_TXCTRL0 I2C_TXCTRL0
 * @brief    Transmit Control Register 0.
 * @{
 */
 #define MXC_F_I2C_REVA_TXCTRL0_PRELOAD_MODE_POS             0 /**< TXCTRL0_PRELOAD_MODE Position */
 #define MXC_F_I2C_REVA_TXCTRL0_PRELOAD_MODE                 ((uint32_t)(0x1UL << MXC_F_I2C_REVA_TXCTRL0_PRELOAD_MODE_POS)) /**< TXCTRL0_PRELOAD_MODE Mask */

 #define MXC_F_I2C_REVA_TXCTRL0_TX_READY_MODE_POS            1 /**< TXCTRL0_TX_READY_MODE Position */
 #define MXC_F_I2C_REVA_TXCTRL0_TX_READY_MODE                ((uint32_t)(0x1UL << MXC_F_I2C_REVA_TXCTRL0_TX_READY_MODE_POS)) /**< TXCTRL0_TX_READY_MODE Mask */

 #define MXC_F_I2C_REVA_TXCTRL0_GC_ADDR_FLUSH_DIS_POS        2 /**< TXCTRL0_GC_ADDR_FLUSH_DIS Position */
 #define MXC_F_I2C_REVA_TXCTRL0_GC_ADDR_FLUSH_DIS            ((uint32_t)(0x1UL << MXC_F_I2C_REVA_TXCTRL0_GC_ADDR_FLUSH_DIS_POS)) /**< TXCTRL0_GC_ADDR_FLUSH_DIS Mask */

 #define MXC_F_I2C_REVA_TXCTRL0_WR_ADDR_FLUSH_DIS_POS        3 /**< TXCTRL0_WR_ADDR_FLUSH_DIS Position */
 #define MXC_F_I2C_REVA_TXCTRL0_WR_ADDR_FLUSH_DIS            ((uint32_t)(0x1UL << MXC_F_I2C_REVA_TXCTRL0_WR_ADDR_FLUSH_DIS_POS)) /**< TXCTRL0_WR_ADDR_FLUSH_DIS Mask */

 #define MXC_F_I2C_REVA_TXCTRL0_RD_ADDR_FLUSH_DIS_POS        4 /**< TXCTRL0_RD_ADDR_FLUSH_DIS Position */
 #define MXC_F_I2C_REVA_TXCTRL0_RD_ADDR_FLUSH_DIS            ((uint32_t)(0x1UL << MXC_F_I2C_REVA_TXCTRL0_RD_ADDR_FLUSH_DIS_POS)) /**< TXCTRL0_RD_ADDR_FLUSH_DIS Mask */

 #define MXC_F_I2C_REVA_TXCTRL0_NACK_FLUSH_DIS_POS           5 /**< TXCTRL0_NACK_FLUSH_DIS Position */
 #define MXC_F_I2C_REVA_TXCTRL0_NACK_FLUSH_DIS               ((uint32_t)(0x1UL << MXC_F_I2C_REVA_TXCTRL0_NACK_FLUSH_DIS_POS)) /**< TXCTRL0_NACK_FLUSH_DIS Mask */

 #define MXC_F_I2C_REVA_TXCTRL0_FLUSH_POS                    7 /**< TXCTRL0_FLUSH Position */
 #define MXC_F_I2C_REVA_TXCTRL0_FLUSH                        ((uint32_t)(0x1UL << MXC_F_I2C_REVA_TXCTRL0_FLUSH_POS)) /**< TXCTRL0_FLUSH Mask */

 #define MXC_F_I2C_REVA_TXCTRL0_THD_LVL_POS                  8 /**< TXCTRL0_THD_LVL Position */
 #define MXC_F_I2C_REVA_TXCTRL0_THD_LVL                      ((uint32_t)(0xFUL << MXC_F_I2C_REVA_TXCTRL0_THD_LVL_POS)) /**< TXCTRL0_THD_LVL Mask */

/**@} end of group I2C_TXCTRL0_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_TXCTRL1 I2C_TXCTRL1
 * @brief    Transmit Control Register 1.
 * @{
 */
 #define MXC_F_I2C_REVA_TXCTRL1_PRELOAD_RDY_POS              0 /**< TXCTRL1_PRELOAD_RDY Position */
 #define MXC_F_I2C_REVA_TXCTRL1_PRELOAD_RDY                  ((uint32_t)(0x1UL << MXC_F_I2C_REVA_TXCTRL1_PRELOAD_RDY_POS)) /**< TXCTRL1_PRELOAD_RDY Mask */

 #define MXC_F_I2C_REVA_TXCTRL1_LVL_POS                      8 /**< TXCTRL1_LVL Position */
 #define MXC_F_I2C_REVA_TXCTRL1_LVL                          ((uint32_t)(0xFUL << MXC_F_I2C_REVA_TXCTRL1_LVL_POS)) /**< TXCTRL1_LVL Mask */

/**@} end of group I2C_TXCTRL1_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_FIFO I2C_FIFO
 * @brief    Data Register.
 * @{
 */
 #define MXC_F_I2C_REVA_FIFO_DATA_POS                        0 /**< FIFO_DATA Position */
 #define MXC_F_I2C_REVA_FIFO_DATA                            ((uint32_t)(0xFFUL << MXC_F_I2C_REVA_FIFO_DATA_POS)) /**< FIFO_DATA Mask */

/**@} end of group I2C_FIFO_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_MSTCTRL I2C_MSTCTRL
 * @brief    Master Control Register.
 * @{
 */
 #define MXC_F_I2C_REVA_MSTCTRL_START_POS                    0 /**< MSTCTRL_START Position */
 #define MXC_F_I2C_REVA_MSTCTRL_START                        ((uint32_t)(0x1UL << MXC_F_I2C_REVA_MSTCTRL_START_POS)) /**< MSTCTRL_START Mask */

 #define MXC_F_I2C_REVA_MSTCTRL_RESTART_POS                  1 /**< MSTCTRL_RESTART Position */
 #define MXC_F_I2C_REVA_MSTCTRL_RESTART                      ((uint32_t)(0x1UL << MXC_F_I2C_REVA_MSTCTRL_RESTART_POS)) /**< MSTCTRL_RESTART Mask */

 #define MXC_F_I2C_REVA_MSTCTRL_STOP_POS                     2 /**< MSTCTRL_STOP Position */
 #define MXC_F_I2C_REVA_MSTCTRL_STOP                         ((uint32_t)(0x1UL << MXC_F_I2C_REVA_MSTCTRL_STOP_POS)) /**< MSTCTRL_STOP Mask */

 #define MXC_F_I2C_REVA_MSTCTRL_EX_ADDR_EN_POS               7 /**< MSTCTRL_EX_ADDR_EN Position */
 #define MXC_F_I2C_REVA_MSTCTRL_EX_ADDR_EN                   ((uint32_t)(0x1UL << MXC_F_I2C_REVA_MSTCTRL_EX_ADDR_EN_POS)) /**< MSTCTRL_EX_ADDR_EN Mask */

/**@} end of group I2C_MSTCTRL_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_CLKLO I2C_CLKLO
 * @brief    Clock Low Register.
 * @{
 */
 #define MXC_F_I2C_REVA_CLKLO_LO_POS                         0 /**< CLKLO_LO Position */
 #define MXC_F_I2C_REVA_CLKLO_LO                             ((uint32_t)(0x1FFUL << MXC_F_I2C_REVA_CLKLO_LO_POS)) /**< CLKLO_LO Mask */

/**@} end of group I2C_CLKLO_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_CLKHI I2C_CLKHI
 * @brief    Clock high Register.
 * @{
 */
 #define MXC_F_I2C_REVA_CLKHI_HI_POS                         0 /**< CLKHI_HI Position */
 #define MXC_F_I2C_REVA_CLKHI_HI                             ((uint32_t)(0x1FFUL << MXC_F_I2C_REVA_CLKHI_HI_POS)) /**< CLKHI_HI Mask */

/**@} end of group I2C_CLKHI_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_HSCLK I2C_HSCLK
 * @brief    Clock high Register.
 * @{
 */
 #define MXC_F_I2C_REVA_HSCLK_LO_POS                         0 /**< HSCLK_LO Position */
 #define MXC_F_I2C_REVA_HSCLK_LO                             ((uint32_t)(0xFFUL << MXC_F_I2C_REVA_HSCLK_LO_POS)) /**< HSCLK_LO Mask */

 #define MXC_F_I2C_REVA_HSCLK_HI_POS                         8 /**< HSCLK_HI Position */
 #define MXC_F_I2C_REVA_HSCLK_HI                             ((uint32_t)(0xFFUL << MXC_F_I2C_REVA_HSCLK_HI_POS)) /**< HSCLK_HI Mask */

/**@} end of group I2C_HSCLK_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_TIMEOUT I2C_TIMEOUT
 * @brief    Timeout Register
 * @{
 */
 #define MXC_F_I2C_REVA_TIMEOUT_SCL_TO_VAL_POS               0 /**< TIMEOUT_SCL_TO_VAL Position */
 #define MXC_F_I2C_REVA_TIMEOUT_SCL_TO_VAL                   ((uint32_t)(0xFFFFUL << MXC_F_I2C_REVA_TIMEOUT_SCL_TO_VAL_POS)) /**< TIMEOUT_SCL_TO_VAL Mask */

/**@} end of group I2C_TIMEOUT_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_DMA I2C_DMA
 * @brief    DMA Register.
 * @{
 */
 #define MXC_F_I2C_REVA_DMA_TX_EN_POS                        0 /**< DMA_TX_EN Position */
 #define MXC_F_I2C_REVA_DMA_TX_EN                            ((uint32_t)(0x1UL << MXC_F_I2C_REVA_DMA_TX_EN_POS)) /**< DMA_TX_EN Mask */

 #define MXC_F_I2C_REVA_DMA_RX_EN_POS                        1 /**< DMA_RX_EN Position */
 #define MXC_F_I2C_REVA_DMA_RX_EN                            ((uint32_t)(0x1UL << MXC_F_I2C_REVA_DMA_RX_EN_POS)) /**< DMA_RX_EN Mask */

/**@} end of group I2C_DMA_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_SLAVE I2C_SLAVE
 * @brief    Slave Address Register.
 * @{
 */
 #define MXC_F_I2C_REVA_SLAVE_ADDR_POS                       0 /**< SLAVE_ADDR Position */
 #define MXC_F_I2C_REVA_SLAVE_ADDR                           ((uint32_t)(0x3FFUL << MXC_F_I2C_REVA_SLAVE_ADDR_POS)) /**< SLAVE_ADDR Mask */

 #define MXC_F_I2C_REVA_SLAVE_EXT_ADDR_EN_POS                15 /**< SLAVE_EXT_ADDR_EN Position */
 #define MXC_F_I2C_REVA_SLAVE_EXT_ADDR_EN                    ((uint32_t)(0x1UL << MXC_F_I2C_REVA_SLAVE_EXT_ADDR_EN_POS)) /**< SLAVE_EXT_ADDR_EN Mask */

/**@} end of group I2C_SLAVE_Register */

#ifdef __cplusplus
}
#endif

#endif /* _I2C_REGS_H_ */
