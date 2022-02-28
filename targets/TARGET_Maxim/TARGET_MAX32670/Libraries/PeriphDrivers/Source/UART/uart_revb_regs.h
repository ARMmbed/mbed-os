/**
 * @file    uart_revb_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the UART_REVB Peripheral Module.
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

#ifndef _UART_REVB_REGS_H_
#define _UART_REVB_REGS_H_

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
 * @ingroup     uart_revb
 * @defgroup    uart_revb_registers UART_REVB_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the UART_REVB Peripheral Module.
 * @details UART Low Power Registers
 */

/**
 * @ingroup uart_revb_registers
 * Structure type to access the UART_REVB Registers.
 */
typedef struct {
    __IO uint32_t ctrl;                 /**< <tt>\b 0x0000:</tt> UART_REVB CTRL Register */
    __I  uint32_t status;               /**< <tt>\b 0x0004:</tt> UART_REVB STATUS Register */
    __IO uint32_t int_en;               /**< <tt>\b 0x0008:</tt> UART_REVB INT_EN Register */
    __IO uint32_t int_fl;               /**< <tt>\b 0x000C:</tt> UART_REVB INT_FL Register */
    __IO uint32_t clkdiv;               /**< <tt>\b 0x0010:</tt> UART_REVB CLKDIV Register */
    __IO uint32_t osr;                  /**< <tt>\b 0x0014:</tt> UART_REVB OSR Register */
    __IO uint32_t txpeek;               /**< <tt>\b 0x0018:</tt> UART_REVB TXPEEK Register */
    __IO uint32_t pnr;                  /**< <tt>\b 0x001C:</tt> UART_REVB PNR Register */
    __IO uint32_t fifo;                 /**< <tt>\b 0x0020:</tt> UART_REVB FIFO Register */
    __R  uint32_t rsv_0x24_0x2f[3];
    __IO uint32_t dma;                  /**< <tt>\b 0x0030:</tt> UART_REVB DMA Register */
    __IO uint32_t wken;                 /**< <tt>\b 0x0034:</tt> UART_REVB WKEN Register */
    __IO uint32_t wkfl;                 /**< <tt>\b 0x0038:</tt> UART_REVB WKFL Register */
} mxc_uart_revb_regs_t;

/* Register offsets for module UART_REVB */
/**
 * @ingroup    uart_revb_registers
 * @defgroup   UART_REVB_Register_Offsets Register Offsets
 * @brief      UART_REVB Peripheral Register Offsets from the UART_REVB Base Peripheral Address. 
 * @{
 */
 #define MXC_R_UART_REVB_CTRL               ((uint32_t)0x00000000UL) /**< Offset from UART_REVB Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_UART_REVB_STATUS             ((uint32_t)0x00000004UL) /**< Offset from UART_REVB Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_UART_REVB_INT_EN             ((uint32_t)0x00000008UL) /**< Offset from UART_REVB Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_UART_REVB_INT_FL             ((uint32_t)0x0000000CUL) /**< Offset from UART_REVB Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_UART_REVB_CLKDIV             ((uint32_t)0x00000010UL) /**< Offset from UART_REVB Base Address: <tt> 0x0010</tt> */ 
 #define MXC_R_UART_REVB_OSR                ((uint32_t)0x00000014UL) /**< Offset from UART_REVB Base Address: <tt> 0x0014</tt> */ 
 #define MXC_R_UART_REVB_TXPEEK             ((uint32_t)0x00000018UL) /**< Offset from UART_REVB Base Address: <tt> 0x0018</tt> */ 
 #define MXC_R_UART_REVB_PNR                ((uint32_t)0x0000001CUL) /**< Offset from UART_REVB Base Address: <tt> 0x001C</tt> */ 
 #define MXC_R_UART_REVB_FIFO               ((uint32_t)0x00000020UL) /**< Offset from UART_REVB Base Address: <tt> 0x0020</tt> */ 
 #define MXC_R_UART_REVB_DMA                ((uint32_t)0x00000030UL) /**< Offset from UART_REVB Base Address: <tt> 0x0030</tt> */ 
 #define MXC_R_UART_REVB_WKEN               ((uint32_t)0x00000034UL) /**< Offset from UART_REVB Base Address: <tt> 0x0034</tt> */ 
 #define MXC_R_UART_REVB_WKFL               ((uint32_t)0x00000038UL) /**< Offset from UART_REVB Base Address: <tt> 0x0038</tt> */ 
/**@} end of group uart_revb_registers */

/**
 * @ingroup  uart_revb_registers
 * @defgroup UART_REVB_CTRL UART_REVB_CTRL
 * @brief    Control register
 * @{
 */
 #define MXC_F_UART_REVB_CTRL_RX_THD_VAL_POS            0 /**< CTRL_RX_THD_VAL Position */
 #define MXC_F_UART_REVB_CTRL_RX_THD_VAL                ((uint32_t)(0xFUL << MXC_F_UART_REVB_CTRL_RX_THD_VAL_POS)) /**< CTRL_RX_THD_VAL Mask */

 #define MXC_F_UART_REVB_CTRL_PAR_EN_POS                4 /**< CTRL_PAR_EN Position */
 #define MXC_F_UART_REVB_CTRL_PAR_EN                    ((uint32_t)(0x1UL << MXC_F_UART_REVB_CTRL_PAR_EN_POS)) /**< CTRL_PAR_EN Mask */

 #define MXC_F_UART_REVB_CTRL_PAR_EO_POS                5 /**< CTRL_PAR_EO Position */
 #define MXC_F_UART_REVB_CTRL_PAR_EO                    ((uint32_t)(0x1UL << MXC_F_UART_REVB_CTRL_PAR_EO_POS)) /**< CTRL_PAR_EO Mask */

 #define MXC_F_UART_REVB_CTRL_PAR_MD_POS                6 /**< CTRL_PAR_MD Position */
 #define MXC_F_UART_REVB_CTRL_PAR_MD                    ((uint32_t)(0x1UL << MXC_F_UART_REVB_CTRL_PAR_MD_POS)) /**< CTRL_PAR_MD Mask */

 #define MXC_F_UART_REVB_CTRL_CTS_DIS_POS               7 /**< CTRL_CTS_DIS Position */
 #define MXC_F_UART_REVB_CTRL_CTS_DIS                   ((uint32_t)(0x1UL << MXC_F_UART_REVB_CTRL_CTS_DIS_POS)) /**< CTRL_CTS_DIS Mask */

 #define MXC_F_UART_REVB_CTRL_TX_FLUSH_POS              8 /**< CTRL_TX_FLUSH Position */
 #define MXC_F_UART_REVB_CTRL_TX_FLUSH                  ((uint32_t)(0x1UL << MXC_F_UART_REVB_CTRL_TX_FLUSH_POS)) /**< CTRL_TX_FLUSH Mask */

 #define MXC_F_UART_REVB_CTRL_RX_FLUSH_POS              9 /**< CTRL_RX_FLUSH Position */
 #define MXC_F_UART_REVB_CTRL_RX_FLUSH                  ((uint32_t)(0x3UL << MXC_F_UART_REVB_CTRL_RX_FLUSH_POS)) /**< CTRL_RX_FLUSH Mask */

 #define MXC_F_UART_REVB_CTRL_CHAR_SIZE_POS             10 /**< CTRL_CHAR_SIZE Position */
 #define MXC_F_UART_REVB_CTRL_CHAR_SIZE                 ((uint32_t)(0x3UL << MXC_F_UART_REVB_CTRL_CHAR_SIZE_POS)) /**< CTRL_CHAR_SIZE Mask */
 #define MXC_V_UART_REVB_CTRL_CHAR_SIZE_5BITS           ((uint32_t)0x0UL) /**< CTRL_CHAR_SIZE_5BITS Value */
 #define MXC_S_UART_REVB_CTRL_CHAR_SIZE_5BITS           (MXC_V_UART_REVB_CTRL_CHAR_SIZE_5BITS << MXC_F_UART_REVB_CTRL_CHAR_SIZE_POS) /**< CTRL_CHAR_SIZE_5BITS Setting */
 #define MXC_V_UART_REVB_CTRL_CHAR_SIZE_6BITS           ((uint32_t)0x1UL) /**< CTRL_CHAR_SIZE_6BITS Value */
 #define MXC_S_UART_REVB_CTRL_CHAR_SIZE_6BITS           (MXC_V_UART_REVB_CTRL_CHAR_SIZE_6BITS << MXC_F_UART_REVB_CTRL_CHAR_SIZE_POS) /**< CTRL_CHAR_SIZE_6BITS Setting */
 #define MXC_V_UART_REVB_CTRL_CHAR_SIZE_7BITS           ((uint32_t)0x2UL) /**< CTRL_CHAR_SIZE_7BITS Value */
 #define MXC_S_UART_REVB_CTRL_CHAR_SIZE_7BITS           (MXC_V_UART_REVB_CTRL_CHAR_SIZE_7BITS << MXC_F_UART_REVB_CTRL_CHAR_SIZE_POS) /**< CTRL_CHAR_SIZE_7BITS Setting */
 #define MXC_V_UART_REVB_CTRL_CHAR_SIZE_8BITS           ((uint32_t)0x3UL) /**< CTRL_CHAR_SIZE_8BITS Value */
 #define MXC_S_UART_REVB_CTRL_CHAR_SIZE_8BITS           (MXC_V_UART_REVB_CTRL_CHAR_SIZE_8BITS << MXC_F_UART_REVB_CTRL_CHAR_SIZE_POS) /**< CTRL_CHAR_SIZE_8BITS Setting */

 #define MXC_F_UART_REVB_CTRL_STOPBITS_POS              12 /**< CTRL_STOPBITS Position */
 #define MXC_F_UART_REVB_CTRL_STOPBITS                  ((uint32_t)(0x1UL << MXC_F_UART_REVB_CTRL_STOPBITS_POS)) /**< CTRL_STOPBITS Mask */

 #define MXC_F_UART_REVB_CTRL_HFC_EN_POS                13 /**< CTRL_HFC_EN Position */
 #define MXC_F_UART_REVB_CTRL_HFC_EN                    ((uint32_t)(0x1UL << MXC_F_UART_REVB_CTRL_HFC_EN_POS)) /**< CTRL_HFC_EN Mask */

 #define MXC_F_UART_REVB_CTRL_RTSDC_POS                 14 /**< CTRL_RTSDC Position */
 #define MXC_F_UART_REVB_CTRL_RTSDC                     ((uint32_t)(0x1UL << MXC_F_UART_REVB_CTRL_RTSDC_POS)) /**< CTRL_RTSDC Mask */

 #define MXC_F_UART_REVB_CTRL_BCLKEN_POS                15 /**< CTRL_BCLKEN Position */
 #define MXC_F_UART_REVB_CTRL_BCLKEN                    ((uint32_t)(0x1UL << MXC_F_UART_REVB_CTRL_BCLKEN_POS)) /**< CTRL_BCLKEN Mask */

 #define MXC_F_UART_REVB_CTRL_BCLKSRC_POS               16 /**< CTRL_BCLKSRC Position */
 #define MXC_F_UART_REVB_CTRL_BCLKSRC                   ((uint32_t)(0x3UL << MXC_F_UART_REVB_CTRL_BCLKSRC_POS)) /**< CTRL_BCLKSRC Mask */
 #define MXC_V_UART_REVB_CTRL_BCLKSRC_PERIPHERAL_CLOCK  ((uint32_t)0x0UL) /**< CTRL_BCLKSRC_PERIPHERAL_CLOCK Value */
 #define MXC_S_UART_REVB_CTRL_BCLKSRC_PERIPHERAL_CLOCK  (MXC_V_UART_REVB_CTRL_BCLKSRC_PERIPHERAL_CLOCK << MXC_F_UART_REVB_CTRL_BCLKSRC_POS) /**< CTRL_BCLKSRC_PERIPHERAL_CLOCK Setting */
 #define MXC_V_UART_REVB_CTRL_BCLKSRC_EXTERNAL_CLOCK    ((uint32_t)0x1UL) /**< CTRL_BCLKSRC_EXTERNAL_CLOCK Value */
 #define MXC_S_UART_REVB_CTRL_BCLKSRC_EXTERNAL_CLOCK    (MXC_V_UART_REVB_CTRL_BCLKSRC_EXTERNAL_CLOCK << MXC_F_UART_REVB_CTRL_BCLKSRC_POS) /**< CTRL_BCLKSRC_EXTERNAL_CLOCK Setting */
 #define MXC_V_UART_REVB_CTRL_BCLKSRC_CLK2              ((uint32_t)0x2UL) /**< CTRL_BCLKSRC_CLK2 Value */
 #define MXC_S_UART_REVB_CTRL_BCLKSRC_CLK2              (MXC_V_UART_REVB_CTRL_BCLKSRC_CLK2 << MXC_F_UART_REVB_CTRL_BCLKSRC_POS) /**< CTRL_BCLKSRC_CLK2 Setting */
 #define MXC_V_UART_REVB_CTRL_BCLKSRC_CLK3              ((uint32_t)0x3UL) /**< CTRL_BCLKSRC_CLK3 Value */
 #define MXC_S_UART_REVB_CTRL_BCLKSRC_CLK3              (MXC_V_UART_REVB_CTRL_BCLKSRC_CLK3 << MXC_F_UART_REVB_CTRL_BCLKSRC_POS) /**< CTRL_BCLKSRC_CLK3 Setting */

 #define MXC_F_UART_REVB_CTRL_DPFE_EN_POS               18 /**< CTRL_DPFE_EN Position */
 #define MXC_F_UART_REVB_CTRL_DPFE_EN                   ((uint32_t)(0x1UL << MXC_F_UART_REVB_CTRL_DPFE_EN_POS)) /**< CTRL_DPFE_EN Mask */

 #define MXC_F_UART_REVB_CTRL_BCLKRDY_POS               19 /**< CTRL_BCLKRDY Position */
 #define MXC_F_UART_REVB_CTRL_BCLKRDY                   ((uint32_t)(0x1UL << MXC_F_UART_REVB_CTRL_BCLKRDY_POS)) /**< CTRL_BCLKRDY Mask */

 #define MXC_F_UART_REVB_CTRL_UCAGM_POS                 20 /**< CTRL_UCAGM Position */
 #define MXC_F_UART_REVB_CTRL_UCAGM                     ((uint32_t)(0x1UL << MXC_F_UART_REVB_CTRL_UCAGM_POS)) /**< CTRL_UCAGM Mask */

 #define MXC_F_UART_REVB_CTRL_FDM_POS                   21 /**< CTRL_FDM Position */
 #define MXC_F_UART_REVB_CTRL_FDM                       ((uint32_t)(0x1UL << MXC_F_UART_REVB_CTRL_FDM_POS)) /**< CTRL_FDM Mask */

 #define MXC_F_UART_REVB_CTRL_DESM_POS                  22 /**< CTRL_DESM Position */
 #define MXC_F_UART_REVB_CTRL_DESM                      ((uint32_t)(0x1UL << MXC_F_UART_REVB_CTRL_DESM_POS)) /**< CTRL_DESM Mask */

/**@} end of group UART_REVB_CTRL_Register */

/**
 * @ingroup  uart_revb_registers
 * @defgroup UART_REVB_STATUS UART_REVB_STATUS
 * @brief    Status register
 * @{
 */
 #define MXC_F_UART_REVB_STATUS_TX_BUSY_POS             0 /**< STATUS_TX_BUSY Position */
 #define MXC_F_UART_REVB_STATUS_TX_BUSY                 ((uint32_t)(0x1UL << MXC_F_UART_REVB_STATUS_TX_BUSY_POS)) /**< STATUS_TX_BUSY Mask */

 #define MXC_F_UART_REVB_STATUS_RX_BUSY_POS             1 /**< STATUS_RX_BUSY Position */
 #define MXC_F_UART_REVB_STATUS_RX_BUSY                 ((uint32_t)(0x1UL << MXC_F_UART_REVB_STATUS_RX_BUSY_POS)) /**< STATUS_RX_BUSY Mask */

 #define MXC_F_UART_REVB_STATUS_RX_EM_POS               4 /**< STATUS_RX_EM Position */
 #define MXC_F_UART_REVB_STATUS_RX_EM                   ((uint32_t)(0x1UL << MXC_F_UART_REVB_STATUS_RX_EM_POS)) /**< STATUS_RX_EM Mask */

 #define MXC_F_UART_REVB_STATUS_RX_FULL_POS             5 /**< STATUS_RX_FULL Position */
 #define MXC_F_UART_REVB_STATUS_RX_FULL                 ((uint32_t)(0x1UL << MXC_F_UART_REVB_STATUS_RX_FULL_POS)) /**< STATUS_RX_FULL Mask */

 #define MXC_F_UART_REVB_STATUS_TX_EM_POS               6 /**< STATUS_TX_EM Position */
 #define MXC_F_UART_REVB_STATUS_TX_EM                   ((uint32_t)(0x1UL << MXC_F_UART_REVB_STATUS_TX_EM_POS)) /**< STATUS_TX_EM Mask */

 #define MXC_F_UART_REVB_STATUS_TX_FULL_POS             7 /**< STATUS_TX_FULL Position */
 #define MXC_F_UART_REVB_STATUS_TX_FULL                 ((uint32_t)(0x1UL << MXC_F_UART_REVB_STATUS_TX_FULL_POS)) /**< STATUS_TX_FULL Mask */

 #define MXC_F_UART_REVB_STATUS_RX_LVL_POS              8 /**< STATUS_RX_LVL Position */
 #define MXC_F_UART_REVB_STATUS_RX_LVL                  ((uint32_t)(0xFUL << MXC_F_UART_REVB_STATUS_RX_LVL_POS)) /**< STATUS_RX_LVL Mask */

 #define MXC_F_UART_REVB_STATUS_TX_LVL_POS              12 /**< STATUS_TX_LVL Position */
 #define MXC_F_UART_REVB_STATUS_TX_LVL                  ((uint32_t)(0xFUL << MXC_F_UART_REVB_STATUS_TX_LVL_POS)) /**< STATUS_TX_LVL Mask */

/**@} end of group UART_REVB_STATUS_Register */

/**
 * @ingroup  uart_revb_registers
 * @defgroup UART_REVB_INT_EN UART_REVB_INT_EN
 * @brief    Interrupt Enable control register
 * @{
 */
 #define MXC_F_UART_REVB_INT_EN_RX_FERR_POS             0 /**< INT_EN_RX_FERR Position */
 #define MXC_F_UART_REVB_INT_EN_RX_FERR                 ((uint32_t)(0x1UL << MXC_F_UART_REVB_INT_EN_RX_FERR_POS)) /**< INT_EN_RX_FERR Mask */

 #define MXC_F_UART_REVB_INT_EN_RX_PAR_POS              1 /**< INT_EN_RX_PAR Position */
 #define MXC_F_UART_REVB_INT_EN_RX_PAR                  ((uint32_t)(0x1UL << MXC_F_UART_REVB_INT_EN_RX_PAR_POS)) /**< INT_EN_RX_PAR Mask */

 #define MXC_F_UART_REVB_INT_EN_CTS_EV_POS              2 /**< INT_EN_CTS_EV Position */
 #define MXC_F_UART_REVB_INT_EN_CTS_EV                  ((uint32_t)(0x1UL << MXC_F_UART_REVB_INT_EN_CTS_EV_POS)) /**< INT_EN_CTS_EV Mask */

 #define MXC_F_UART_REVB_INT_EN_RX_OV_POS               3 /**< INT_EN_RX_OV Position */
 #define MXC_F_UART_REVB_INT_EN_RX_OV                   ((uint32_t)(0x1UL << MXC_F_UART_REVB_INT_EN_RX_OV_POS)) /**< INT_EN_RX_OV Mask */

 #define MXC_F_UART_REVB_INT_EN_RX_THD_POS              4 /**< INT_EN_RX_THD Position */
 #define MXC_F_UART_REVB_INT_EN_RX_THD                  ((uint32_t)(0x1UL << MXC_F_UART_REVB_INT_EN_RX_THD_POS)) /**< INT_EN_RX_THD Mask */

 #define MXC_F_UART_REVB_INT_EN_TX_HE_POS               6 /**< INT_EN_TX_HE Position */
 #define MXC_F_UART_REVB_INT_EN_TX_HE                   ((uint32_t)(0x1UL << MXC_F_UART_REVB_INT_EN_TX_HE_POS)) /**< INT_EN_TX_HE Mask */

/**@} end of group UART_REVB_INT_EN_Register */

/**
 * @ingroup  uart_revb_registers
 * @defgroup UART_REVB_INT_FL UART_REVB_INT_FL
 * @brief    Interrupt status flags Control register
 * @{
 */
 #define MXC_F_UART_REVB_INT_FL_RX_FERR_POS             0 /**< INT_FL_RX_FERR Position */
 #define MXC_F_UART_REVB_INT_FL_RX_FERR                 ((uint32_t)(0x1UL << MXC_F_UART_REVB_INT_FL_RX_FERR_POS)) /**< INT_FL_RX_FERR Mask */

 #define MXC_F_UART_REVB_INT_FL_RX_PAR_POS              1 /**< INT_FL_RX_PAR Position */
 #define MXC_F_UART_REVB_INT_FL_RX_PAR                  ((uint32_t)(0x1UL << MXC_F_UART_REVB_INT_FL_RX_PAR_POS)) /**< INT_FL_RX_PAR Mask */

 #define MXC_F_UART_REVB_INT_FL_CTS_EV_POS              2 /**< INT_FL_CTS_EV Position */
 #define MXC_F_UART_REVB_INT_FL_CTS_EV                  ((uint32_t)(0x1UL << MXC_F_UART_REVB_INT_FL_CTS_EV_POS)) /**< INT_FL_CTS_EV Mask */

 #define MXC_F_UART_REVB_INT_FL_RX_OV_POS               3 /**< INT_FL_RX_OV Position */
 #define MXC_F_UART_REVB_INT_FL_RX_OV                   ((uint32_t)(0x1UL << MXC_F_UART_REVB_INT_FL_RX_OV_POS)) /**< INT_FL_RX_OV Mask */

 #define MXC_F_UART_REVB_INT_FL_RX_THD_POS              4 /**< INT_FL_RX_THD Position */
 #define MXC_F_UART_REVB_INT_FL_RX_THD                  ((uint32_t)(0x1UL << MXC_F_UART_REVB_INT_FL_RX_THD_POS)) /**< INT_FL_RX_THD Mask */

 #define MXC_F_UART_REVB_INT_FL_TX_HE_POS               6 /**< INT_FL_TX_HE Position */
 #define MXC_F_UART_REVB_INT_FL_TX_HE                   ((uint32_t)(0x1UL << MXC_F_UART_REVB_INT_FL_TX_HE_POS)) /**< INT_FL_TX_HE Mask */

/**@} end of group UART_REVB_INT_FL_Register */

/**
 * @ingroup  uart_revb_registers
 * @defgroup UART_REVB_CLKDIV UART_REVB_CLKDIV
 * @brief    Clock Divider register
 * @{
 */
 #define MXC_F_UART_REVB_CLKDIV_CLKDIV_POS              0 /**< CLKDIV_CLKDIV Position */
 #define MXC_F_UART_REVB_CLKDIV_CLKDIV                  ((uint32_t)(0xFFFFFUL << MXC_F_UART_REVB_CLKDIV_CLKDIV_POS)) /**< CLKDIV_CLKDIV Mask */

/**@} end of group UART_REVB_CLKDIV_Register */

/**
 * @ingroup  uart_revb_registers
 * @defgroup UART_REVB_OSR UART_REVB_OSR
 * @brief    Over Sampling Rate register
 * @{
 */
 #define MXC_F_UART_REVB_OSR_OSR_POS                    0 /**< OSR_OSR Position */
 #define MXC_F_UART_REVB_OSR_OSR                        ((uint32_t)(0x7UL << MXC_F_UART_REVB_OSR_OSR_POS)) /**< OSR_OSR Mask */

/**@} end of group UART_REVB_OSR_Register */

/**
 * @ingroup  uart_revb_registers
 * @defgroup UART_REVB_TXPEEK UART_REVB_TXPEEK
 * @brief    TX FIFO Output Peek register
 * @{
 */
 #define MXC_F_UART_REVB_TXPEEK_DATA_POS                0 /**< TXPEEK_DATA Position */
 #define MXC_F_UART_REVB_TXPEEK_DATA                    ((uint32_t)(0xFFUL << MXC_F_UART_REVB_TXPEEK_DATA_POS)) /**< TXPEEK_DATA Mask */

/**@} end of group UART_REVB_TXPEEK_Register */

/**
 * @ingroup  uart_revb_registers
 * @defgroup UART_REVB_PNR UART_REVB_PNR
 * @brief     Pin register
 * @{
 */
 #define MXC_F_UART_REVB_PNR_CTS_POS                    0 /**< PNR_CTS Position */
 #define MXC_F_UART_REVB_PNR_CTS                        ((uint32_t)(0x1UL << MXC_F_UART_REVB_PNR_CTS_POS)) /**< PNR_CTS Mask */

 #define MXC_F_UART_REVB_PNR_RTS_POS                    1 /**< PNR_RTS Position */
 #define MXC_F_UART_REVB_PNR_RTS                        ((uint32_t)(0x1UL << MXC_F_UART_REVB_PNR_RTS_POS)) /**< PNR_RTS Mask */

/**@} end of group UART_REVB_PNR_Register */

/**
 * @ingroup  uart_revb_registers
 * @defgroup UART_REVB_FIFO UART_REVB_FIFO
 * @brief    FIFO Read/Write register
 * @{
 */
 #define MXC_F_UART_REVB_FIFO_DATA_POS                  0 /**< FIFO_DATA Position */
 #define MXC_F_UART_REVB_FIFO_DATA                      ((uint32_t)(0xFFUL << MXC_F_UART_REVB_FIFO_DATA_POS)) /**< FIFO_DATA Mask */

 #define MXC_F_UART_REVB_FIFO_RX_PAR_POS                8 /**< FIFO_RX_PAR Position */
 #define MXC_F_UART_REVB_FIFO_RX_PAR                    ((uint32_t)(0x1UL << MXC_F_UART_REVB_FIFO_RX_PAR_POS)) /**< FIFO_RX_PAR Mask */

/**@} end of group UART_REVB_FIFO_Register */

/**
 * @ingroup  uart_revb_registers
 * @defgroup UART_REVB_DMA UART_REVB_DMA
 * @brief    DMA Configuration register
 * @{
 */
 #define MXC_F_UART_REVB_DMA_TX_THD_VAL_POS             0 /**< DMA_TX_THD_VAL Position */
 #define MXC_F_UART_REVB_DMA_TX_THD_VAL                 ((uint32_t)(0xFUL << MXC_F_UART_REVB_DMA_TX_THD_VAL_POS)) /**< DMA_TX_THD_VAL Mask */

 #define MXC_F_UART_REVB_DMA_TX_EN_POS                  4 /**< DMA_TX_EN Position */
 #define MXC_F_UART_REVB_DMA_TX_EN                      ((uint32_t)(0x1UL << MXC_F_UART_REVB_DMA_TX_EN_POS)) /**< DMA_TX_EN Mask */

 #define MXC_F_UART_REVB_DMA_RX_THD_VAL_POS             5 /**< DMA_RX_THD_VAL Position */
 #define MXC_F_UART_REVB_DMA_RX_THD_VAL                 ((uint32_t)(0xFUL << MXC_F_UART_REVB_DMA_RX_THD_VAL_POS)) /**< DMA_RX_THD_VAL Mask */

 #define MXC_F_UART_REVB_DMA_RX_EN_POS                  9 /**< DMA_RX_EN Position */
 #define MXC_F_UART_REVB_DMA_RX_EN                      ((uint32_t)(0x1UL << MXC_F_UART_REVB_DMA_RX_EN_POS)) /**< DMA_RX_EN Mask */

/**@} end of group UART_REVB_DMA_Register */

/**
 * @ingroup  uart_revb_registers
 * @defgroup UART_REVB_WKEN UART_REVB_WKEN
 * @brief    Wake up enable Control register
 * @{
 */
 #define MXC_F_UART_REVB_WKEN_RX_NE_POS                 0 /**< WKEN_RX_NE Position */
 #define MXC_F_UART_REVB_WKEN_RX_NE                     ((uint32_t)(0x1UL << MXC_F_UART_REVB_WKEN_RX_NE_POS)) /**< WKEN_RX_NE Mask */

 #define MXC_F_UART_REVB_WKEN_RX_FULL_POS               1 /**< WKEN_RX_FULL Position */
 #define MXC_F_UART_REVB_WKEN_RX_FULL                   ((uint32_t)(0x1UL << MXC_F_UART_REVB_WKEN_RX_FULL_POS)) /**< WKEN_RX_FULL Mask */

 #define MXC_F_UART_REVB_WKEN_RX_THD_POS                1 /**< WKEN_RX_THD Position */
 #define MXC_F_UART_REVB_WKEN_RX_THD                    ((uint32_t)(0x3UL << MXC_F_UART_REVB_WKEN_RX_THD_POS)) /**< WKEN_RX_THD Mask */

/**@} end of group UART_REVB_WKEN_Register */

/**
 * @ingroup  uart_revb_registers
 * @defgroup UART_REVB_WKFL UART_REVB_WKFL
 * @brief    Wake up Flags register
 * @{
 */
 #define MXC_F_UART_REVB_WKFL_RX_NE_POS                 0 /**< WKFL_RX_NE Position */
 #define MXC_F_UART_REVB_WKFL_RX_NE                     ((uint32_t)(0x1UL << MXC_F_UART_REVB_WKFL_RX_NE_POS)) /**< WKFL_RX_NE Mask */

 #define MXC_F_UART_REVB_WKFL_RX_FULL_POS               1 /**< WKFL_RX_FULL Position */
 #define MXC_F_UART_REVB_WKFL_RX_FULL                   ((uint32_t)(0x1UL << MXC_F_UART_REVB_WKFL_RX_FULL_POS)) /**< WKFL_RX_FULL Mask */

 #define MXC_F_UART_REVB_WKFL_RX_THD_POS                2 /**< WKFL_RX_THD Position */
 #define MXC_F_UART_REVB_WKFL_RX_THD                    ((uint32_t)(0x1UL << MXC_F_UART_REVB_WKFL_RX_THD_POS)) /**< WKFL_RX_THD Mask */

/**@} end of group UART_REVB_WKFL_Register */

#ifdef __cplusplus
}
#endif

#endif /* _UART_REVB_REGS_H_ */
