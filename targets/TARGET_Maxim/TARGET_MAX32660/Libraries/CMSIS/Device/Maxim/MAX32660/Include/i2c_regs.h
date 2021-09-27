/**
 * @file    i2c_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the I2C Peripheral Module.
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

#ifndef _I2C_REGS_H_
#define _I2C_REGS_H_

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
    __IO uint32_t ctrl0;                /**< <tt>\b 0x00:</tt> I2C CTRL0 Register */
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
    __IO uint32_t mstr_mode;            /**< <tt>\b 0x30:</tt> I2C MSTR_MODE Register */
    __IO uint32_t clklo;                /**< <tt>\b 0x34:</tt> I2C CLKLO Register */
    __IO uint32_t clkhi;                /**< <tt>\b 0x38:</tt> I2C CLKHI Register */
    __IO uint32_t hs_clk;               /**< <tt>\b 0x3C:</tt> I2C HS_CLK Register */
    __IO uint32_t timeout;              /**< <tt>\b 0x40:</tt> I2C TIMEOUT Register */
    __IO uint32_t sladdr;               /**< <tt>\b 0x44:</tt> I2C SLADDR Register */
    __IO uint32_t dma;                  /**< <tt>\b 0x48:</tt> I2C DMA Register */
} mxc_i2c_regs_t;

/* Register offsets for module I2C */
/**
 * @ingroup    i2c_registers
 * @defgroup   I2C_Register_Offsets Register Offsets
 * @brief      I2C Peripheral Register Offsets from the I2C Base Peripheral Address. 
 * @{
 */
 #define MXC_R_I2C_CTRL0                    ((uint32_t)0x00000000UL) /**< Offset from I2C Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_I2C_STATUS                   ((uint32_t)0x00000004UL) /**< Offset from I2C Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_I2C_INTFL0                   ((uint32_t)0x00000008UL) /**< Offset from I2C Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_I2C_INTEN0                   ((uint32_t)0x0000000CUL) /**< Offset from I2C Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_I2C_INTFL1                   ((uint32_t)0x00000010UL) /**< Offset from I2C Base Address: <tt> 0x0010</tt> */ 
 #define MXC_R_I2C_INTEN1                   ((uint32_t)0x00000014UL) /**< Offset from I2C Base Address: <tt> 0x0014</tt> */ 
 #define MXC_R_I2C_FIFOLEN                  ((uint32_t)0x00000018UL) /**< Offset from I2C Base Address: <tt> 0x0018</tt> */ 
 #define MXC_R_I2C_RXCTRL0                  ((uint32_t)0x0000001CUL) /**< Offset from I2C Base Address: <tt> 0x001C</tt> */ 
 #define MXC_R_I2C_RXCTRL1                  ((uint32_t)0x00000020UL) /**< Offset from I2C Base Address: <tt> 0x0020</tt> */ 
 #define MXC_R_I2C_TXCTRL0                  ((uint32_t)0x00000024UL) /**< Offset from I2C Base Address: <tt> 0x0024</tt> */ 
 #define MXC_R_I2C_TXCTRL1                  ((uint32_t)0x00000028UL) /**< Offset from I2C Base Address: <tt> 0x0028</tt> */ 
 #define MXC_R_I2C_FIFO                     ((uint32_t)0x0000002CUL) /**< Offset from I2C Base Address: <tt> 0x002C</tt> */ 
 #define MXC_R_I2C_MSTR_MODE                ((uint32_t)0x00000030UL) /**< Offset from I2C Base Address: <tt> 0x0030</tt> */ 
 #define MXC_R_I2C_CLKLO                    ((uint32_t)0x00000034UL) /**< Offset from I2C Base Address: <tt> 0x0034</tt> */ 
 #define MXC_R_I2C_CLKHI                    ((uint32_t)0x00000038UL) /**< Offset from I2C Base Address: <tt> 0x0038</tt> */ 
 #define MXC_R_I2C_HS_CLK                   ((uint32_t)0x0000003CUL) /**< Offset from I2C Base Address: <tt> 0x003C</tt> */ 
 #define MXC_R_I2C_TIMEOUT                  ((uint32_t)0x00000040UL) /**< Offset from I2C Base Address: <tt> 0x0040</tt> */ 
 #define MXC_R_I2C_SLADDR                   ((uint32_t)0x00000044UL) /**< Offset from I2C Base Address: <tt> 0x0044</tt> */ 
 #define MXC_R_I2C_DMA                      ((uint32_t)0x00000048UL) /**< Offset from I2C Base Address: <tt> 0x0048</tt> */ 
/**@} end of group i2c_registers */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_CTRL0 I2C_CTRL0
 * @brief    Control Register0.
 * @{
 */
 #define MXC_F_I2C_CTRL0_I2CEN_POS                      0 /**< CTRL0_I2CEN Position */
 #define MXC_F_I2C_CTRL0_I2CEN                          ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_I2CEN_POS)) /**< CTRL0_I2CEN Mask */

 #define MXC_F_I2C_CTRL0_MST_POS                        1 /**< CTRL0_MST Position */
 #define MXC_F_I2C_CTRL0_MST                            ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_MST_POS)) /**< CTRL0_MST Mask */

 #define MXC_F_I2C_CTRL0_GCEN_POS                       2 /**< CTRL0_GCEN Position */
 #define MXC_F_I2C_CTRL0_GCEN                           ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_GCEN_POS)) /**< CTRL0_GCEN Mask */

 #define MXC_F_I2C_CTRL0_IRXM_POS                       3 /**< CTRL0_IRXM Position */
 #define MXC_F_I2C_CTRL0_IRXM                           ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_IRXM_POS)) /**< CTRL0_IRXM Mask */

 #define MXC_F_I2C_CTRL0_ACK_POS                        4 /**< CTRL0_ACK Position */
 #define MXC_F_I2C_CTRL0_ACK                            ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_ACK_POS)) /**< CTRL0_ACK Mask */

 #define MXC_F_I2C_CTRL0_SCLO_POS                       6 /**< CTRL0_SCLO Position */
 #define MXC_F_I2C_CTRL0_SCLO                           ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_SCLO_POS)) /**< CTRL0_SCLO Mask */

 #define MXC_F_I2C_CTRL0_SDAO_POS                       7 /**< CTRL0_SDAO Position */
 #define MXC_F_I2C_CTRL0_SDAO                           ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_SDAO_POS)) /**< CTRL0_SDAO Mask */

 #define MXC_F_I2C_CTRL0_SCL_POS                        8 /**< CTRL0_SCL Position */
 #define MXC_F_I2C_CTRL0_SCL                            ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_SCL_POS)) /**< CTRL0_SCL Mask */

 #define MXC_F_I2C_CTRL0_SDA_POS                        9 /**< CTRL0_SDA Position */
 #define MXC_F_I2C_CTRL0_SDA                            ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_SDA_POS)) /**< CTRL0_SDA Mask */

 #define MXC_F_I2C_CTRL0_SWOE_POS                       10 /**< CTRL0_SWOE Position */
 #define MXC_F_I2C_CTRL0_SWOE                           ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_SWOE_POS)) /**< CTRL0_SWOE Mask */

 #define MXC_F_I2C_CTRL0_READ_POS                       11 /**< CTRL0_READ Position */
 #define MXC_F_I2C_CTRL0_READ                           ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_READ_POS)) /**< CTRL0_READ Mask */

 #define MXC_F_I2C_CTRL0_SCL_STRD_POS                   12 /**< CTRL0_SCL_STRD Position */
 #define MXC_F_I2C_CTRL0_SCL_STRD                       ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_SCL_STRD_POS)) /**< CTRL0_SCL_STRD Mask */

 #define MXC_F_I2C_CTRL0_SCL_PPM_POS                    13 /**< CTRL0_SCL_PPM Position */
 #define MXC_F_I2C_CTRL0_SCL_PPM                        ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_SCL_PPM_POS)) /**< CTRL0_SCL_PPM Mask */

 #define MXC_F_I2C_CTRL0_HSMODE_POS                     15 /**< CTRL0_HSMODE Position */
 #define MXC_F_I2C_CTRL0_HSMODE                         ((uint32_t)(0x1UL << MXC_F_I2C_CTRL0_HSMODE_POS)) /**< CTRL0_HSMODE Mask */

/**@} end of group I2C_CTRL0_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_STATUS I2C_STATUS
 * @brief    Status Register.
 * @{
 */
 #define MXC_F_I2C_STATUS_BUSY_POS                      0 /**< STATUS_BUSY Position */
 #define MXC_F_I2C_STATUS_BUSY                          ((uint32_t)(0x1UL << MXC_F_I2C_STATUS_BUSY_POS)) /**< STATUS_BUSY Mask */

 #define MXC_F_I2C_STATUS_RXE_POS                       1 /**< STATUS_RXE Position */
 #define MXC_F_I2C_STATUS_RXE                           ((uint32_t)(0x1UL << MXC_F_I2C_STATUS_RXE_POS)) /**< STATUS_RXE Mask */

 #define MXC_F_I2C_STATUS_RXF_POS                       2 /**< STATUS_RXF Position */
 #define MXC_F_I2C_STATUS_RXF                           ((uint32_t)(0x1UL << MXC_F_I2C_STATUS_RXF_POS)) /**< STATUS_RXF Mask */

 #define MXC_F_I2C_STATUS_TXE_POS                       3 /**< STATUS_TXE Position */
 #define MXC_F_I2C_STATUS_TXE                           ((uint32_t)(0x1UL << MXC_F_I2C_STATUS_TXE_POS)) /**< STATUS_TXE Mask */

 #define MXC_F_I2C_STATUS_TXF_POS                       4 /**< STATUS_TXF Position */
 #define MXC_F_I2C_STATUS_TXF                           ((uint32_t)(0x1UL << MXC_F_I2C_STATUS_TXF_POS)) /**< STATUS_TXF Mask */

 #define MXC_F_I2C_STATUS_CKMD_POS                      5 /**< STATUS_CKMD Position */
 #define MXC_F_I2C_STATUS_CKMD                          ((uint32_t)(0x1UL << MXC_F_I2C_STATUS_CKMD_POS)) /**< STATUS_CKMD Mask */

 #define MXC_F_I2C_STATUS_STAT_POS                      8 /**< STATUS_STAT Position */
 #define MXC_F_I2C_STATUS_STAT                          ((uint32_t)(0xFUL << MXC_F_I2C_STATUS_STAT_POS)) /**< STATUS_STAT Mask */
 #define MXC_V_I2C_STATUS_STAT_IDLE                     ((uint32_t)0x0UL) /**< STATUS_STAT_IDLE Value */
 #define MXC_S_I2C_STATUS_STAT_IDLE                     (MXC_V_I2C_STATUS_STAT_IDLE << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_IDLE Setting */
 #define MXC_V_I2C_STATUS_STAT_MTX_ADDR                 ((uint32_t)0x1UL) /**< STATUS_STAT_MTX_ADDR Value */
 #define MXC_S_I2C_STATUS_STAT_MTX_ADDR                 (MXC_V_I2C_STATUS_STAT_MTX_ADDR << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_MTX_ADDR Setting */
 #define MXC_V_I2C_STATUS_STAT_MRX_ADDR_ACK             ((uint32_t)0x2UL) /**< STATUS_STAT_MRX_ADDR_ACK Value */
 #define MXC_S_I2C_STATUS_STAT_MRX_ADDR_ACK             (MXC_V_I2C_STATUS_STAT_MRX_ADDR_ACK << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_MRX_ADDR_ACK Setting */
 #define MXC_V_I2C_STATUS_STAT_MTX_EX_ADDR              ((uint32_t)0x3UL) /**< STATUS_STAT_MTX_EX_ADDR Value */
 #define MXC_S_I2C_STATUS_STAT_MTX_EX_ADDR              (MXC_V_I2C_STATUS_STAT_MTX_EX_ADDR << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_MTX_EX_ADDR Setting */
 #define MXC_V_I2C_STATUS_STAT_MRX_EX_ADDR              ((uint32_t)0x4UL) /**< STATUS_STAT_MRX_EX_ADDR Value */
 #define MXC_S_I2C_STATUS_STAT_MRX_EX_ADDR              (MXC_V_I2C_STATUS_STAT_MRX_EX_ADDR << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_MRX_EX_ADDR Setting */
 #define MXC_V_I2C_STATUS_STAT_SRX_ADDR                 ((uint32_t)0x5UL) /**< STATUS_STAT_SRX_ADDR Value */
 #define MXC_S_I2C_STATUS_STAT_SRX_ADDR                 (MXC_V_I2C_STATUS_STAT_SRX_ADDR << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_SRX_ADDR Setting */
 #define MXC_V_I2C_STATUS_STAT_STX_ADDR_ACK             ((uint32_t)0x6UL) /**< STATUS_STAT_STX_ADDR_ACK Value */
 #define MXC_S_I2C_STATUS_STAT_STX_ADDR_ACK             (MXC_V_I2C_STATUS_STAT_STX_ADDR_ACK << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_STX_ADDR_ACK Setting */
 #define MXC_V_I2C_STATUS_STAT_SRX_EX_ADDR              ((uint32_t)0x7UL) /**< STATUS_STAT_SRX_EX_ADDR Value */
 #define MXC_S_I2C_STATUS_STAT_SRX_EX_ADDR              (MXC_V_I2C_STATUS_STAT_SRX_EX_ADDR << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_SRX_EX_ADDR Setting */
 #define MXC_V_I2C_STATUS_STAT_STX_EX_ADDR_ACK          ((uint32_t)0x8UL) /**< STATUS_STAT_STX_EX_ADDR_ACK Value */
 #define MXC_S_I2C_STATUS_STAT_STX_EX_ADDR_ACK          (MXC_V_I2C_STATUS_STAT_STX_EX_ADDR_ACK << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_STX_EX_ADDR_ACK Setting */
 #define MXC_V_I2C_STATUS_STAT_TX                       ((uint32_t)0x9UL) /**< STATUS_STAT_TX Value */
 #define MXC_S_I2C_STATUS_STAT_TX                       (MXC_V_I2C_STATUS_STAT_TX << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_TX Setting */
 #define MXC_V_I2C_STATUS_STAT_RX_ACK                   ((uint32_t)0xAUL) /**< STATUS_STAT_RX_ACK Value */
 #define MXC_S_I2C_STATUS_STAT_RX_ACK                   (MXC_V_I2C_STATUS_STAT_RX_ACK << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_RX_ACK Setting */
 #define MXC_V_I2C_STATUS_STAT_RX                       ((uint32_t)0xBUL) /**< STATUS_STAT_RX Value */
 #define MXC_S_I2C_STATUS_STAT_RX                       (MXC_V_I2C_STATUS_STAT_RX << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_RX Setting */
 #define MXC_V_I2C_STATUS_STAT_TX_ACK                   ((uint32_t)0xCUL) /**< STATUS_STAT_TX_ACK Value */
 #define MXC_S_I2C_STATUS_STAT_TX_ACK                   (MXC_V_I2C_STATUS_STAT_TX_ACK << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_TX_ACK Setting */
 #define MXC_V_I2C_STATUS_STAT_NACK                     ((uint32_t)0xDUL) /**< STATUS_STAT_NACK Value */
 #define MXC_S_I2C_STATUS_STAT_NACK                     (MXC_V_I2C_STATUS_STAT_NACK << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_NACK Setting */
 #define MXC_V_I2C_STATUS_STAT_BY_ST                    ((uint32_t)0xFUL) /**< STATUS_STAT_BY_ST Value */
 #define MXC_S_I2C_STATUS_STAT_BY_ST                    (MXC_V_I2C_STATUS_STAT_BY_ST << MXC_F_I2C_STATUS_STAT_POS) /**< STATUS_STAT_BY_ST Setting */

/**@} end of group I2C_STATUS_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_INTFL0 I2C_INTFL0
 * @brief    Interrupt Status Register.
 * @{
 */
 #define MXC_F_I2C_INTFL0_DONEI_POS                     0 /**< INTFL0_DONEI Position */
 #define MXC_F_I2C_INTFL0_DONEI                         ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_DONEI_POS)) /**< INTFL0_DONEI Mask */

 #define MXC_F_I2C_INTFL0_IRXMI_POS                     1 /**< INTFL0_IRXMI Position */
 #define MXC_F_I2C_INTFL0_IRXMI                         ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_IRXMI_POS)) /**< INTFL0_IRXMI Mask */

 #define MXC_F_I2C_INTFL0_GCI_POS                       2 /**< INTFL0_GCI Position */
 #define MXC_F_I2C_INTFL0_GCI                           ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_GCI_POS)) /**< INTFL0_GCI Mask */

 #define MXC_F_I2C_INTFL0_AMI_POS                       3 /**< INTFL0_AMI Position */
 #define MXC_F_I2C_INTFL0_AMI                           ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_AMI_POS)) /**< INTFL0_AMI Mask */

 #define MXC_F_I2C_INTFL0_RXTHI_POS                     4 /**< INTFL0_RXTHI Position */
 #define MXC_F_I2C_INTFL0_RXTHI                         ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_RXTHI_POS)) /**< INTFL0_RXTHI Mask */

 #define MXC_F_I2C_INTFL0_TXTHI_POS                     5 /**< INTFL0_TXTHI Position */
 #define MXC_F_I2C_INTFL0_TXTHI                         ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_TXTHI_POS)) /**< INTFL0_TXTHI Mask */

 #define MXC_F_I2C_INTFL0_STOPI_POS                     6 /**< INTFL0_STOPI Position */
 #define MXC_F_I2C_INTFL0_STOPI                         ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_STOPI_POS)) /**< INTFL0_STOPI Mask */

 #define MXC_F_I2C_INTFL0_ADRACKI_POS                   7 /**< INTFL0_ADRACKI Position */
 #define MXC_F_I2C_INTFL0_ADRACKI                       ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_ADRACKI_POS)) /**< INTFL0_ADRACKI Mask */

 #define MXC_F_I2C_INTFL0_ARBERI_POS                    8 /**< INTFL0_ARBERI Position */
 #define MXC_F_I2C_INTFL0_ARBERI                        ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_ARBERI_POS)) /**< INTFL0_ARBERI Mask */

 #define MXC_F_I2C_INTFL0_TOERI_POS                     9 /**< INTFL0_TOERI Position */
 #define MXC_F_I2C_INTFL0_TOERI                         ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_TOERI_POS)) /**< INTFL0_TOERI Mask */

 #define MXC_F_I2C_INTFL0_ADRERI_POS                    10 /**< INTFL0_ADRERI Position */
 #define MXC_F_I2C_INTFL0_ADRERI                        ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_ADRERI_POS)) /**< INTFL0_ADRERI Mask */

 #define MXC_F_I2C_INTFL0_DATERI_POS                    11 /**< INTFL0_DATERI Position */
 #define MXC_F_I2C_INTFL0_DATERI                        ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_DATERI_POS)) /**< INTFL0_DATERI Mask */

 #define MXC_F_I2C_INTFL0_DNRERI_POS                    12 /**< INTFL0_DNRERI Position */
 #define MXC_F_I2C_INTFL0_DNRERI                        ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_DNRERI_POS)) /**< INTFL0_DNRERI Mask */

 #define MXC_F_I2C_INTFL0_STRTERI_POS                   13 /**< INTFL0_STRTERI Position */
 #define MXC_F_I2C_INTFL0_STRTERI                       ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_STRTERI_POS)) /**< INTFL0_STRTERI Mask */

 #define MXC_F_I2C_INTFL0_STOPERI_POS                   14 /**< INTFL0_STOPERI Position */
 #define MXC_F_I2C_INTFL0_STOPERI                       ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_STOPERI_POS)) /**< INTFL0_STOPERI Mask */

 #define MXC_F_I2C_INTFL0_TXLOI_POS                     15 /**< INTFL0_TXLOI Position */
 #define MXC_F_I2C_INTFL0_TXLOI                         ((uint32_t)(0x1UL << MXC_F_I2C_INTFL0_TXLOI_POS)) /**< INTFL0_TXLOI Mask */

/**@} end of group I2C_INTFL0_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_INTEN0 I2C_INTEN0
 * @brief    Interrupt Enable Register.
 * @{
 */
 #define MXC_F_I2C_INTEN0_DONEIE_POS                    0 /**< INTEN0_DONEIE Position */
 #define MXC_F_I2C_INTEN0_DONEIE                        ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_DONEIE_POS)) /**< INTEN0_DONEIE Mask */

 #define MXC_F_I2C_INTEN0_IRXMIE_POS                    1 /**< INTEN0_IRXMIE Position */
 #define MXC_F_I2C_INTEN0_IRXMIE                        ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_IRXMIE_POS)) /**< INTEN0_IRXMIE Mask */

 #define MXC_F_I2C_INTEN0_GCIE_POS                      2 /**< INTEN0_GCIE Position */
 #define MXC_F_I2C_INTEN0_GCIE                          ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_GCIE_POS)) /**< INTEN0_GCIE Mask */

 #define MXC_F_I2C_INTEN0_AMIE_POS                      3 /**< INTEN0_AMIE Position */
 #define MXC_F_I2C_INTEN0_AMIE                          ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_AMIE_POS)) /**< INTEN0_AMIE Mask */

 #define MXC_F_I2C_INTEN0_RXTHIE_POS                    4 /**< INTEN0_RXTHIE Position */
 #define MXC_F_I2C_INTEN0_RXTHIE                        ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_RXTHIE_POS)) /**< INTEN0_RXTHIE Mask */

 #define MXC_F_I2C_INTEN0_TXTHIE_POS                    5 /**< INTEN0_TXTHIE Position */
 #define MXC_F_I2C_INTEN0_TXTHIE                        ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_TXTHIE_POS)) /**< INTEN0_TXTHIE Mask */

 #define MXC_F_I2C_INTEN0_STOPIE_POS                    6 /**< INTEN0_STOPIE Position */
 #define MXC_F_I2C_INTEN0_STOPIE                        ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_STOPIE_POS)) /**< INTEN0_STOPIE Mask */

 #define MXC_F_I2C_INTEN0_ADRACKIE_POS                  7 /**< INTEN0_ADRACKIE Position */
 #define MXC_F_I2C_INTEN0_ADRACKIE                      ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_ADRACKIE_POS)) /**< INTEN0_ADRACKIE Mask */

 #define MXC_F_I2C_INTEN0_ARBERIE_POS                   8 /**< INTEN0_ARBERIE Position */
 #define MXC_F_I2C_INTEN0_ARBERIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_ARBERIE_POS)) /**< INTEN0_ARBERIE Mask */

 #define MXC_F_I2C_INTEN0_TOERIE_POS                    9 /**< INTEN0_TOERIE Position */
 #define MXC_F_I2C_INTEN0_TOERIE                        ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_TOERIE_POS)) /**< INTEN0_TOERIE Mask */

 #define MXC_F_I2C_INTEN0_ADRERIE_POS                   10 /**< INTEN0_ADRERIE Position */
 #define MXC_F_I2C_INTEN0_ADRERIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_ADRERIE_POS)) /**< INTEN0_ADRERIE Mask */

 #define MXC_F_I2C_INTEN0_DATERIE_POS                   11 /**< INTEN0_DATERIE Position */
 #define MXC_F_I2C_INTEN0_DATERIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_DATERIE_POS)) /**< INTEN0_DATERIE Mask */

 #define MXC_F_I2C_INTEN0_DNRERIE_POS                   12 /**< INTEN0_DNRERIE Position */
 #define MXC_F_I2C_INTEN0_DNRERIE                       ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_DNRERIE_POS)) /**< INTEN0_DNRERIE Mask */

 #define MXC_F_I2C_INTEN0_STRTERIE_POS                  13 /**< INTEN0_STRTERIE Position */
 #define MXC_F_I2C_INTEN0_STRTERIE                      ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_STRTERIE_POS)) /**< INTEN0_STRTERIE Mask */

 #define MXC_F_I2C_INTEN0_STOPERIE_POS                  14 /**< INTEN0_STOPERIE Position */
 #define MXC_F_I2C_INTEN0_STOPERIE                      ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_STOPERIE_POS)) /**< INTEN0_STOPERIE Mask */

 #define MXC_F_I2C_INTEN0_TXLOIE_POS                    15 /**< INTEN0_TXLOIE Position */
 #define MXC_F_I2C_INTEN0_TXLOIE                        ((uint32_t)(0x1UL << MXC_F_I2C_INTEN0_TXLOIE_POS)) /**< INTEN0_TXLOIE Mask */

/**@} end of group I2C_INTEN0_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_INTFL1 I2C_INTFL1
 * @brief    Interrupt Status Register 1.
 * @{
 */
 #define MXC_F_I2C_INTFL1_RXOFI_POS                     0 /**< INTFL1_RXOFI Position */
 #define MXC_F_I2C_INTFL1_RXOFI                         ((uint32_t)(0x1UL << MXC_F_I2C_INTFL1_RXOFI_POS)) /**< INTFL1_RXOFI Mask */

 #define MXC_F_I2C_INTFL1_TXUFI_POS                     1 /**< INTFL1_TXUFI Position */
 #define MXC_F_I2C_INTFL1_TXUFI                         ((uint32_t)(0x1UL << MXC_F_I2C_INTFL1_TXUFI_POS)) /**< INTFL1_TXUFI Mask */

/**@} end of group I2C_INTFL1_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_INTEN1 I2C_INTEN1
 * @brief    Interrupt Staus Register 1.
 * @{
 */
 #define MXC_F_I2C_INTEN1_RXOFIE_POS                    0 /**< INTEN1_RXOFIE Position */
 #define MXC_F_I2C_INTEN1_RXOFIE                        ((uint32_t)(0x1UL << MXC_F_I2C_INTEN1_RXOFIE_POS)) /**< INTEN1_RXOFIE Mask */

 #define MXC_F_I2C_INTEN1_TXUFIE_POS                    1 /**< INTEN1_TXUFIE Position */
 #define MXC_F_I2C_INTEN1_TXUFIE                        ((uint32_t)(0x1UL << MXC_F_I2C_INTEN1_TXUFIE_POS)) /**< INTEN1_TXUFIE Mask */

/**@} end of group I2C_INTEN1_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_FIFOLEN I2C_FIFOLEN
 * @brief    FIFO Configuration Register.
 * @{
 */
 #define MXC_F_I2C_FIFOLEN_RXLEN_POS                    0 /**< FIFOLEN_RXLEN Position */
 #define MXC_F_I2C_FIFOLEN_RXLEN                        ((uint32_t)(0xFFUL << MXC_F_I2C_FIFOLEN_RXLEN_POS)) /**< FIFOLEN_RXLEN Mask */

 #define MXC_F_I2C_FIFOLEN_TXLEN_POS                    8 /**< FIFOLEN_TXLEN Position */
 #define MXC_F_I2C_FIFOLEN_TXLEN                        ((uint32_t)(0xFFUL << MXC_F_I2C_FIFOLEN_TXLEN_POS)) /**< FIFOLEN_TXLEN Mask */

/**@} end of group I2C_FIFOLEN_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_RXCTRL0 I2C_RXCTRL0
 * @brief    Receive Control Register 0.
 * @{
 */
 #define MXC_F_I2C_RXCTRL0_DNR_POS                      0 /**< RXCTRL0_DNR Position */
 #define MXC_F_I2C_RXCTRL0_DNR                          ((uint32_t)(0x1UL << MXC_F_I2C_RXCTRL0_DNR_POS)) /**< RXCTRL0_DNR Mask */

 #define MXC_F_I2C_RXCTRL0_RXFSH_POS                    7 /**< RXCTRL0_RXFSH Position */
 #define MXC_F_I2C_RXCTRL0_RXFSH                        ((uint32_t)(0x1UL << MXC_F_I2C_RXCTRL0_RXFSH_POS)) /**< RXCTRL0_RXFSH Mask */

 #define MXC_F_I2C_RXCTRL0_RXTH_POS                     8 /**< RXCTRL0_RXTH Position */
 #define MXC_F_I2C_RXCTRL0_RXTH                         ((uint32_t)(0xFUL << MXC_F_I2C_RXCTRL0_RXTH_POS)) /**< RXCTRL0_RXTH Mask */

/**@} end of group I2C_RXCTRL0_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_RXCTRL1 I2C_RXCTRL1
 * @brief    Receive Control Register 1.
 * @{
 */
 #define MXC_F_I2C_RXCTRL1_RXCNT_POS                    0 /**< RXCTRL1_RXCNT Position */
 #define MXC_F_I2C_RXCTRL1_RXCNT                        ((uint32_t)(0xFFUL << MXC_F_I2C_RXCTRL1_RXCNT_POS)) /**< RXCTRL1_RXCNT Mask */

 #define MXC_F_I2C_RXCTRL1_RXFIFO_POS                   8 /**< RXCTRL1_RXFIFO Position */
 #define MXC_F_I2C_RXCTRL1_RXFIFO                       ((uint32_t)(0xFUL << MXC_F_I2C_RXCTRL1_RXFIFO_POS)) /**< RXCTRL1_RXFIFO Mask */

/**@} end of group I2C_RXCTRL1_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_TXCTRL0 I2C_TXCTRL0
 * @brief    Transmit Control Register 0.
 * @{
 */
 #define MXC_F_I2C_TXCTRL0_TXPRELD_POS                  0 /**< TXCTRL0_TXPRELD Position */
 #define MXC_F_I2C_TXCTRL0_TXPRELD                      ((uint32_t)(0x1UL << MXC_F_I2C_TXCTRL0_TXPRELD_POS)) /**< TXCTRL0_TXPRELD Mask */

 #define MXC_F_I2C_TXCTRL0_TX_READY_MODE_POS            1 /**< TXCTRL0_TX_READY_MODE Position */
 #define MXC_F_I2C_TXCTRL0_TX_READY_MODE                ((uint32_t)(0x1UL << MXC_F_I2C_TXCTRL0_TX_READY_MODE_POS)) /**< TXCTRL0_TX_READY_MODE Mask */

 #define MXC_F_I2C_TXCTRL0_TXFSH_POS                    7 /**< TXCTRL0_TXFSH Position */
 #define MXC_F_I2C_TXCTRL0_TXFSH                        ((uint32_t)(0x1UL << MXC_F_I2C_TXCTRL0_TXFSH_POS)) /**< TXCTRL0_TXFSH Mask */

 #define MXC_F_I2C_TXCTRL0_TXTH_POS                     8 /**< TXCTRL0_TXTH Position */
 #define MXC_F_I2C_TXCTRL0_TXTH                         ((uint32_t)(0xFUL << MXC_F_I2C_TXCTRL0_TXTH_POS)) /**< TXCTRL0_TXTH Mask */

/**@} end of group I2C_TXCTRL0_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_TXCTRL1 I2C_TXCTRL1
 * @brief    Transmit Control Register 1.
 * @{
 */
 #define MXC_F_I2C_TXCTRL1_TXRDY_POS                    0 /**< TXCTRL1_TXRDY Position */
 #define MXC_F_I2C_TXCTRL1_TXRDY                        ((uint32_t)(0x1UL << MXC_F_I2C_TXCTRL1_TXRDY_POS)) /**< TXCTRL1_TXRDY Mask */

 #define MXC_F_I2C_TXCTRL1_TXLAST_POS                   1 /**< TXCTRL1_TXLAST Position */
 #define MXC_F_I2C_TXCTRL1_TXLAST                       ((uint32_t)(0x1UL << MXC_F_I2C_TXCTRL1_TXLAST_POS)) /**< TXCTRL1_TXLAST Mask */

 #define MXC_F_I2C_TXCTRL1_FLSH_GCADDR_DIS_POS          2 /**< TXCTRL1_FLSH_GCADDR_DIS Position */
 #define MXC_F_I2C_TXCTRL1_FLSH_GCADDR_DIS              ((uint32_t)(0x1UL << MXC_F_I2C_TXCTRL1_FLSH_GCADDR_DIS_POS)) /**< TXCTRL1_FLSH_GCADDR_DIS Mask */

 #define MXC_F_I2C_TXCTRL1_FLSH_SLADDR_DIS_POS          4 /**< TXCTRL1_FLSH_SLADDR_DIS Position */
 #define MXC_F_I2C_TXCTRL1_FLSH_SLADDR_DIS              ((uint32_t)(0x1UL << MXC_F_I2C_TXCTRL1_FLSH_SLADDR_DIS_POS)) /**< TXCTRL1_FLSH_SLADDR_DIS Mask */

 #define MXC_F_I2C_TXCTRL1_FLSH_NACK_DIS_POS            5 /**< TXCTRL1_FLSH_NACK_DIS Position */
 #define MXC_F_I2C_TXCTRL1_FLSH_NACK_DIS                ((uint32_t)(0x1UL << MXC_F_I2C_TXCTRL1_FLSH_NACK_DIS_POS)) /**< TXCTRL1_FLSH_NACK_DIS Mask */

 #define MXC_F_I2C_TXCTRL1_TXFIFO_POS                   8 /**< TXCTRL1_TXFIFO Position */
 #define MXC_F_I2C_TXCTRL1_TXFIFO                       ((uint32_t)(0xFUL << MXC_F_I2C_TXCTRL1_TXFIFO_POS)) /**< TXCTRL1_TXFIFO Mask */

/**@} end of group I2C_TXCTRL1_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_FIFO I2C_FIFO
 * @brief    Data Register.
 * @{
 */
 #define MXC_F_I2C_FIFO_DATA_POS                        0 /**< FIFO_DATA Position */
 #define MXC_F_I2C_FIFO_DATA                            ((uint32_t)(0xFFUL << MXC_F_I2C_FIFO_DATA_POS)) /**< FIFO_DATA Mask */

/**@} end of group I2C_FIFO_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_MSTR_MODE I2C_MSTR_MODE
 * @brief    Master Control Register.
 * @{
 */
 #define MXC_F_I2C_MSTR_MODE_START_POS                  0 /**< MSTR_MODE_START Position */
 #define MXC_F_I2C_MSTR_MODE_START                      ((uint32_t)(0x1UL << MXC_F_I2C_MSTR_MODE_START_POS)) /**< MSTR_MODE_START Mask */

 #define MXC_F_I2C_MSTR_MODE_RESTART_POS                1 /**< MSTR_MODE_RESTART Position */
 #define MXC_F_I2C_MSTR_MODE_RESTART                    ((uint32_t)(0x1UL << MXC_F_I2C_MSTR_MODE_RESTART_POS)) /**< MSTR_MODE_RESTART Mask */

 #define MXC_F_I2C_MSTR_MODE_STOP_POS                   2 /**< MSTR_MODE_STOP Position */
 #define MXC_F_I2C_MSTR_MODE_STOP                       ((uint32_t)(0x1UL << MXC_F_I2C_MSTR_MODE_STOP_POS)) /**< MSTR_MODE_STOP Mask */

 #define MXC_F_I2C_MSTR_MODE_SEA_POS                    7 /**< MSTR_MODE_SEA Position */
 #define MXC_F_I2C_MSTR_MODE_SEA                        ((uint32_t)(0x1UL << MXC_F_I2C_MSTR_MODE_SEA_POS)) /**< MSTR_MODE_SEA Mask */

/**@} end of group I2C_MSTR_MODE_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_CLKLO I2C_CLKLO
 * @brief    Clock Low Register.
 * @{
 */
 #define MXC_F_I2C_CLKLO_SCL_LO_POS                     0 /**< CLKLO_SCL_LO Position */
 #define MXC_F_I2C_CLKLO_SCL_LO                         ((uint32_t)(0x1FFUL << MXC_F_I2C_CLKLO_SCL_LO_POS)) /**< CLKLO_SCL_LO Mask */

/**@} end of group I2C_CLKLO_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_CLKHI I2C_CLKHI
 * @brief    Clock high Register.
 * @{
 */
 #define MXC_F_I2C_CLKHI_SCL_HI_POS                     0 /**< CLKHI_SCL_HI Position */
 #define MXC_F_I2C_CLKHI_SCL_HI                         ((uint32_t)(0x1FFUL << MXC_F_I2C_CLKHI_SCL_HI_POS)) /**< CLKHI_SCL_HI Mask */

/**@} end of group I2C_CLKHI_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_HS_CLK I2C_HS_CLK
 * @brief    HS-Mode Clock Control Register
 * @{
 */
 #define MXC_F_I2C_HS_CLK_HS_CLK_LO_POS                 0 /**< HS_CLK_HS_CLK_LO Position */
 #define MXC_F_I2C_HS_CLK_HS_CLK_LO                     ((uint32_t)(0xFFUL << MXC_F_I2C_HS_CLK_HS_CLK_LO_POS)) /**< HS_CLK_HS_CLK_LO Mask */

 #define MXC_F_I2C_HS_CLK_HS_CLK_HI_POS                 8 /**< HS_CLK_HS_CLK_HI Position */
 #define MXC_F_I2C_HS_CLK_HS_CLK_HI                     ((uint32_t)(0xFFUL << MXC_F_I2C_HS_CLK_HS_CLK_HI_POS)) /**< HS_CLK_HS_CLK_HI Mask */

/**@} end of group I2C_HS_CLK_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_TIMEOUT I2C_TIMEOUT
 * @brief    Timeout Register
 * @{
 */
 #define MXC_F_I2C_TIMEOUT_TO_POS                       0 /**< TIMEOUT_TO Position */
 #define MXC_F_I2C_TIMEOUT_TO                           ((uint32_t)(0xFFFFUL << MXC_F_I2C_TIMEOUT_TO_POS)) /**< TIMEOUT_TO Mask */

/**@} end of group I2C_TIMEOUT_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_SLADDR I2C_SLADDR
 * @brief    Slave Address Register.
 * @{
 */
 #define MXC_F_I2C_SLADDR_SLA_POS                       0 /**< SLADDR_SLA Position */
 #define MXC_F_I2C_SLADDR_SLA                           ((uint32_t)(0x3FFUL << MXC_F_I2C_SLADDR_SLA_POS)) /**< SLADDR_SLA Mask */

 #define MXC_F_I2C_SLADDR_EA_POS                        15 /**< SLADDR_EA Position */
 #define MXC_F_I2C_SLADDR_EA                            ((uint32_t)(0x1UL << MXC_F_I2C_SLADDR_EA_POS)) /**< SLADDR_EA Mask */

/**@} end of group I2C_SLADDR_Register */

/**
 * @ingroup  i2c_registers
 * @defgroup I2C_DMA I2C_DMA
 * @brief    DMA Register.
 * @{
 */
 #define MXC_F_I2C_DMA_TXEN_POS                         0 /**< DMA_TXEN Position */
 #define MXC_F_I2C_DMA_TXEN                             ((uint32_t)(0x1UL << MXC_F_I2C_DMA_TXEN_POS)) /**< DMA_TXEN Mask */

 #define MXC_F_I2C_DMA_RXEN_POS                         1 /**< DMA_RXEN Position */
 #define MXC_F_I2C_DMA_RXEN                             ((uint32_t)(0x1UL << MXC_F_I2C_DMA_RXEN_POS)) /**< DMA_RXEN Mask */

/**@} end of group I2C_DMA_Register */

#ifdef __cplusplus
}
#endif

#endif /* _I2C_REGS_H_ */
