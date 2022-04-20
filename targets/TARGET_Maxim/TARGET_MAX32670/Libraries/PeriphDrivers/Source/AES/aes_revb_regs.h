/**
 * @file    aes_revb_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the AES_REVB Peripheral Module.
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

#ifndef _AES_REVB_REGS_H_
#define _AES_REVB_REGS_H_

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
 * @ingroup     aes_revb
 * @defgroup    aes_revb_registers AES_REVB_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the AES_REVB Peripheral Module.
 * @details AES Keys.
 */

/**
 * @ingroup aes_revb_registers
 * Structure type to access the AES_REVB Registers.
 */
typedef struct {
    __IO uint32_t ctrl;                 /**< <tt>\b 0x0000:</tt> AES_REVB CTRL Register */
    __IO uint32_t status;               /**< <tt>\b 0x0004:</tt> AES_REVB STATUS Register */
    __IO uint32_t intfl;                /**< <tt>\b 0x0008:</tt> AES_REVB INTFL Register */
    __IO uint32_t inten;                /**< <tt>\b 0x000C:</tt> AES_REVB INTEN Register */
    __IO uint32_t fifo;                 /**< <tt>\b 0x0010:</tt> AES_REVB FIFO Register */
} mxc_aes_revb_regs_t;

/* Register offsets for module AES_REVB */
/**
 * @ingroup    aes_revb_registers
 * @defgroup   AES_REVB_Register_Offsets Register Offsets
 * @brief      AES_REVB Peripheral Register Offsets from the AES_REVB Base Peripheral Address. 
 * @{
 */
 #define MXC_R_AES_REVB_CTRL                ((uint32_t)0x00000000UL) /**< Offset from AES_REVB Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_AES_REVB_STATUS              ((uint32_t)0x00000004UL) /**< Offset from AES_REVB Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_AES_REVB_INTFL               ((uint32_t)0x00000008UL) /**< Offset from AES_REVB Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_AES_REVB_INTEN               ((uint32_t)0x0000000CUL) /**< Offset from AES_REVB Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_AES_REVB_FIFO                ((uint32_t)0x00000010UL) /**< Offset from AES_REVB Base Address: <tt> 0x0010</tt> */ 
/**@} end of group aes_revb_registers */

/**
 * @ingroup  aes_revb_registers
 * @defgroup AES_REVB_CTRL AES_REVB_CTRL
 * @brief    AES Control Register
 * @{
 */
 #define MXC_F_AES_REVB_CTRL_EN_POS                     0 /**< CTRL_EN Position */
 #define MXC_F_AES_REVB_CTRL_EN                         ((uint32_t)(0x1UL << MXC_F_AES_REVB_CTRL_EN_POS)) /**< CTRL_EN Mask */

 #define MXC_F_AES_REVB_CTRL_DMA_RX_EN_POS              1 /**< CTRL_DMA_RX_EN Position */
 #define MXC_F_AES_REVB_CTRL_DMA_RX_EN                  ((uint32_t)(0x1UL << MXC_F_AES_REVB_CTRL_DMA_RX_EN_POS)) /**< CTRL_DMA_RX_EN Mask */

 #define MXC_F_AES_REVB_CTRL_DMA_TX_EN_POS              2 /**< CTRL_DMA_TX_EN Position */
 #define MXC_F_AES_REVB_CTRL_DMA_TX_EN                  ((uint32_t)(0x1UL << MXC_F_AES_REVB_CTRL_DMA_TX_EN_POS)) /**< CTRL_DMA_TX_EN Mask */

 #define MXC_F_AES_REVB_CTRL_START_POS                  3 /**< CTRL_START Position */
 #define MXC_F_AES_REVB_CTRL_START                      ((uint32_t)(0x1UL << MXC_F_AES_REVB_CTRL_START_POS)) /**< CTRL_START Mask */

 #define MXC_F_AES_REVB_CTRL_INPUT_FLUSH_POS            4 /**< CTRL_INPUT_FLUSH Position */
 #define MXC_F_AES_REVB_CTRL_INPUT_FLUSH                ((uint32_t)(0x1UL << MXC_F_AES_REVB_CTRL_INPUT_FLUSH_POS)) /**< CTRL_INPUT_FLUSH Mask */

 #define MXC_F_AES_REVB_CTRL_OUTPUT_FLUSH_POS           5 /**< CTRL_OUTPUT_FLUSH Position */
 #define MXC_F_AES_REVB_CTRL_OUTPUT_FLUSH               ((uint32_t)(0x1UL << MXC_F_AES_REVB_CTRL_OUTPUT_FLUSH_POS)) /**< CTRL_OUTPUT_FLUSH Mask */

 #define MXC_F_AES_REVB_CTRL_KEY_SIZE_POS               6 /**< CTRL_KEY_SIZE Position */
 #define MXC_F_AES_REVB_CTRL_KEY_SIZE                   ((uint32_t)(0x3UL << MXC_F_AES_REVB_CTRL_KEY_SIZE_POS)) /**< CTRL_KEY_SIZE Mask */
 #define MXC_V_AES_REVB_CTRL_KEY_SIZE_AES128            ((uint32_t)0x0UL) /**< CTRL_KEY_SIZE_AES128 Value */
 #define MXC_S_AES_REVB_CTRL_KEY_SIZE_AES128            (MXC_V_AES_REVB_CTRL_KEY_SIZE_AES128 << MXC_F_AES_REVB_CTRL_KEY_SIZE_POS) /**< CTRL_KEY_SIZE_AES128 Setting */
 #define MXC_V_AES_REVB_CTRL_KEY_SIZE_AES192            ((uint32_t)0x1UL) /**< CTRL_KEY_SIZE_AES192 Value */
 #define MXC_S_AES_REVB_CTRL_KEY_SIZE_AES192            (MXC_V_AES_REVB_CTRL_KEY_SIZE_AES192 << MXC_F_AES_REVB_CTRL_KEY_SIZE_POS) /**< CTRL_KEY_SIZE_AES192 Setting */
 #define MXC_V_AES_REVB_CTRL_KEY_SIZE_AES256            ((uint32_t)0x2UL) /**< CTRL_KEY_SIZE_AES256 Value */
 #define MXC_S_AES_REVB_CTRL_KEY_SIZE_AES256            (MXC_V_AES_REVB_CTRL_KEY_SIZE_AES256 << MXC_F_AES_REVB_CTRL_KEY_SIZE_POS) /**< CTRL_KEY_SIZE_AES256 Setting */

 #define MXC_F_AES_REVB_CTRL_TYPE_POS                   8 /**< CTRL_TYPE Position */
 #define MXC_F_AES_REVB_CTRL_TYPE                       ((uint32_t)(0x3UL << MXC_F_AES_REVB_CTRL_TYPE_POS)) /**< CTRL_TYPE Mask */

/**@} end of group AES_REVB_CTRL_Register */

/**
 * @ingroup  aes_revb_registers
 * @defgroup AES_REVB_STATUS AES_REVB_STATUS
 * @brief    AES Status Register
 * @{
 */
 #define MXC_F_AES_REVB_STATUS_BUSY_POS                 0 /**< STATUS_BUSY Position */
 #define MXC_F_AES_REVB_STATUS_BUSY                     ((uint32_t)(0x1UL << MXC_F_AES_REVB_STATUS_BUSY_POS)) /**< STATUS_BUSY Mask */

 #define MXC_F_AES_REVB_STATUS_INPUT_EM_POS             1 /**< STATUS_INPUT_EM Position */
 #define MXC_F_AES_REVB_STATUS_INPUT_EM                 ((uint32_t)(0x1UL << MXC_F_AES_REVB_STATUS_INPUT_EM_POS)) /**< STATUS_INPUT_EM Mask */

 #define MXC_F_AES_REVB_STATUS_INPUT_FULL_POS           2 /**< STATUS_INPUT_FULL Position */
 #define MXC_F_AES_REVB_STATUS_INPUT_FULL               ((uint32_t)(0x1UL << MXC_F_AES_REVB_STATUS_INPUT_FULL_POS)) /**< STATUS_INPUT_FULL Mask */

 #define MXC_F_AES_REVB_STATUS_OUTPUT_EM_POS            3 /**< STATUS_OUTPUT_EM Position */
 #define MXC_F_AES_REVB_STATUS_OUTPUT_EM                ((uint32_t)(0x1UL << MXC_F_AES_REVB_STATUS_OUTPUT_EM_POS)) /**< STATUS_OUTPUT_EM Mask */

 #define MXC_F_AES_REVB_STATUS_OUTPUT_FULL_POS          4 /**< STATUS_OUTPUT_FULL Position */
 #define MXC_F_AES_REVB_STATUS_OUTPUT_FULL              ((uint32_t)(0x1UL << MXC_F_AES_REVB_STATUS_OUTPUT_FULL_POS)) /**< STATUS_OUTPUT_FULL Mask */

/**@} end of group AES_REVB_STATUS_Register */

/**
 * @ingroup  aes_revb_registers
 * @defgroup AES_REVB_INTFL AES_REVB_INTFL
 * @brief    AES Interrupt Flag Register
 * @{
 */
 #define MXC_F_AES_REVB_INTFL_DONE_POS                  0 /**< INTFL_DONE Position */
 #define MXC_F_AES_REVB_INTFL_DONE                      ((uint32_t)(0x1UL << MXC_F_AES_REVB_INTFL_DONE_POS)) /**< INTFL_DONE Mask */

 #define MXC_F_AES_REVB_INTFL_KEY_CHANGE_POS            1 /**< INTFL_KEY_CHANGE Position */
 #define MXC_F_AES_REVB_INTFL_KEY_CHANGE                ((uint32_t)(0x1UL << MXC_F_AES_REVB_INTFL_KEY_CHANGE_POS)) /**< INTFL_KEY_CHANGE Mask */

 #define MXC_F_AES_REVB_INTFL_KEY_ZERO_POS              2 /**< INTFL_KEY_ZERO Position */
 #define MXC_F_AES_REVB_INTFL_KEY_ZERO                  ((uint32_t)(0x1UL << MXC_F_AES_REVB_INTFL_KEY_ZERO_POS)) /**< INTFL_KEY_ZERO Mask */

 #define MXC_F_AES_REVB_INTFL_OV_POS                    3 /**< INTFL_OV Position */
 #define MXC_F_AES_REVB_INTFL_OV                        ((uint32_t)(0x1UL << MXC_F_AES_REVB_INTFL_OV_POS)) /**< INTFL_OV Mask */

/**@} end of group AES_REVB_INTFL_Register */

/**
 * @ingroup  aes_revb_registers
 * @defgroup AES_REVB_INTEN AES_REVB_INTEN
 * @brief    AES Interrupt Enable Register
 * @{
 */
 #define MXC_F_AES_REVB_INTEN_DONE_POS                  0 /**< INTEN_DONE Position */
 #define MXC_F_AES_REVB_INTEN_DONE                      ((uint32_t)(0x1UL << MXC_F_AES_REVB_INTEN_DONE_POS)) /**< INTEN_DONE Mask */

 #define MXC_F_AES_REVB_INTEN_KEY_CHANGE_POS            1 /**< INTEN_KEY_CHANGE Position */
 #define MXC_F_AES_REVB_INTEN_KEY_CHANGE                ((uint32_t)(0x1UL << MXC_F_AES_REVB_INTEN_KEY_CHANGE_POS)) /**< INTEN_KEY_CHANGE Mask */

 #define MXC_F_AES_REVB_INTEN_KEY_ZERO_POS              2 /**< INTEN_KEY_ZERO Position */
 #define MXC_F_AES_REVB_INTEN_KEY_ZERO                  ((uint32_t)(0x1UL << MXC_F_AES_REVB_INTEN_KEY_ZERO_POS)) /**< INTEN_KEY_ZERO Mask */

 #define MXC_F_AES_REVB_INTEN_OV_POS                    3 /**< INTEN_OV Position */
 #define MXC_F_AES_REVB_INTEN_OV                        ((uint32_t)(0x1UL << MXC_F_AES_REVB_INTEN_OV_POS)) /**< INTEN_OV Mask */

/**@} end of group AES_REVB_INTEN_Register */

/**
 * @ingroup  aes_revb_registers
 * @defgroup AES_REVB_FIFO AES_REVB_FIFO
 * @brief    AES Data Register
 * @{
 */
 #define MXC_F_AES_REVB_FIFO_DATA_POS                   0 /**< FIFO_DATA Position */
 #define MXC_F_AES_REVB_FIFO_DATA                       ((uint32_t)(0x1UL << MXC_F_AES_REVB_FIFO_DATA_POS)) /**< FIFO_DATA Mask */

/**@} end of group AES_REVB_FIFO_Register */

#ifdef __cplusplus
}
#endif

#endif /* _AES_REVB_REGS_H_ */
