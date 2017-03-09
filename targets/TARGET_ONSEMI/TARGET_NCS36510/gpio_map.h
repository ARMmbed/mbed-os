/**
 ******************************************************************************
 * @file gpio_map.h
 * @brief GPIO HW register map
 * @internal
 * @author  ON Semiconductor
 * $Rev: 2115 $
 * $Date: 2013-07-17 18:08:17 +0530 (Wed, 17 Jul 2013) $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup gpio
 *
 * @details
 * <p>
 * GPIO HW register map description
 * </p>
 *
 * <h1> Reference document(s) </h1>
 * <p>
 * <a href="../pdf/IPC7203_GPIO_APB_DS_v1P1.pdf" target="_blank">
 * Reference document: IPC7203 APB GPIO Design Specification v1.2</a>
 * </p>
 */

#ifndef GPIO_MAP_H_
#define GPIO_MAP_H_

#include "architecture.h"

/** Structure overlay for GPIO control registers, see memory_map.h
 * For most registers, bit lockations match GPIO numbers.*/
typedef struct {
    __IO uint32_t R_STATE_W_SET;         /**< Read synchronized input / Write ones to bits to set corresponding output IO's*/
    __IO uint32_t R_IRQ_W_CLEAR;         /**< Read state of irq / Write ones to bits to clear corresponging output IO's */
    __IO uint32_t W_OUT;                 /**< Write ones to set direction to output */
    __IO uint32_t W_IN;                  /**< Write ones to set direction to input */
    __IO uint32_t IRQ_ENABLE_SET;        /**< Read active high irq enable / Write ones to enable irq */
    __IO uint32_t IRQ_ENABLE_CLEAR;      /**< Read active high irq enable / Write ones to disable irq */
    __IO uint32_t IRQ_EDGE;              /**< Read irq configuration (edge or level) / Write ones to set irq to edge-sensitive */
    __IO uint32_t IRQ_LEVEL;             /**< Read irq configuration (edge or level) / Write ones to set irq to level-sensitive */
    __IO uint32_t IRQ_POLARITY_SET;      /**< Read irq polarity / Write ones to set irq to active high or rising edge */
    __IO uint32_t IRQ_POLARITY_CLEAR;    /**< Read irq polarity / Write ones to set interrupts to active low or falling edge */
    __IO uint32_t ANYEDGE_SET;           /**< Read irq anyedge configuration / Write ones to override irq edge selection & irq on any edge */
    __IO uint32_t ANYEDGE_CLEAR;         /**< Read irq anyedge configuration / Write ones to clear edge selection override */
    __IO uint32_t IRQ_CLEAR;             /**< Write ones to clear edge-sensitive irq */
    __IO uint32_t CONTROL;               /**< Controls loopback/normal mode selection */
} GpioReg_t, *GpioReg_pt;

#endif /* GPIO_MAP_H_ */
