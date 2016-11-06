/**
 ******************************************************************************
 * @file flash_map.h
 * @brief Flash controller HW register map
 * @internal
 * @author ON Semiconductor
 * $Rev: 2686 $
 * $Date: 2014-01-23 13:31:54 +0530 (Thu, 23 Jan 2014) $
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
 * @ingroup flash
 *
 * @details
 * <p>
 * Flash controller HW register map description
 * </p>
 *
 */
#ifndef FLASH_MAP_H_
#define FLASH_MAP_H_

#include "architecture.h"

/** Flash Control HW Structure Overlay */
typedef struct {
    union {
        struct {
            __I uint32_t FLASH_A_BUSY:1;      /**< Busy A */
            __I uint32_t FLASH_B_BUSY:1;      /**< Busy B */
            __I uint32_t FLASH_A_UNLOCK:1;    /**< Unlock A */
            __I uint32_t FLASH_B_UNLOCK:1;    /**< Unlock B */
            __I uint32_t FLASH_ERROR:3;       /**< 000 – No Error, 111 – Attempt to access an array while it is busy powering up, 001 – Attempt to erase bootloader in the field, 010 – Attempt to access array during erase, 100 – Attempt to access array during write */
        } BITS;
        __I uint32_t WORD;
    } STATUS;
    union {
        struct {
            __IO uint32_t FLASHB_PD:1;
            __IO uint32_t FLASHA_PD:1;
            __IO uint32_t REMAP:1;
            __IO uint32_t WR_INT_EN:1;
            __IO uint32_t ERASE_INT_EN:1;
            __IO uint32_t ERROR_INT_EN:1;
            __IO uint32_t WRITE_BLOCK:1;
        } BITS;
        __IO uint32_t WORD;
    } CONTROL;
    union {
        struct {
            __IO uint32_t PAGEERASE:1;   /**< Erase a single page */
            __IO uint32_t MASSERASE:1;   /**< MASS Erase */
        } BITS;
        __IO uint32_t WORD;
    } COMMAND;
    __IO uint32_t ADDR;
    __IO uint32_t UNLOCK1;
    __IO uint32_t UNLOCKA;
    __IO uint32_t UNLOCKB;
    union {
        struct {
            __I uint32_t INT_PEND:1; // Interrupt pending
            __I uint32_t INT_TYPE:3; // Interrupt type
        } BITS;
        __I uint32_t WORD;
    } INT_STATUS;
} FlashReg_t, *FlashReg_pt;

#endif /* FLASH_MAP_H_ */
