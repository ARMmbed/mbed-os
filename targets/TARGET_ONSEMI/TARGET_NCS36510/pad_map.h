/**
******************************************************************************
* @file pad_map.h
* @brief PAD hw module register map
* @internal
* @author ON Semiconductor
* $Rev: 3166 $
* $Date: 2015-01-19 11:28:08 +0530 (Mon, 19 Jan 2015) $
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
* @ingroup pad
*
* @details
*/

#ifndef PAD_MAP_H_
#define PAD_MAP_H_

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

#include "architecture.h"

/*************************************************************************************************
*                                                                                                *
*  Symbolic Constants                                                                            *
*                                                                                                *
*************************************************************************************************/

/** no pull up nor pull down */
#define PAD_PULL_NONE    (uint8_t)0x01
/** pull down */
#define PAD_PULL_DOWN    (uint8_t)0x00
/** pull up */
#define PAD_PULL_UP      (uint8_t)0x03

/** Drive strength */
#define PAD_DRIVE_L0        (uint8_t)0x00
#define PAD_DRIVE_L1        (uint8_t)0x01
#define PAD_DRIVE_L2        (uint8_t)0x02
#define PAD_DRIVE_L3        (uint8_t)0x03
#define PAD_DRIVE_L4        (uint8_t)0x04
#define PAD_DRIVE_L5        (uint8_t)0x05
#define PAD_DRIVE_L6        (uint8_t)0x06

/** output configuration push/pull */
#define PAD_OUTCFG_PUSHPULL    (uint8_t)0x00
/** output configuration open drain */
#define PAD_OOUTCFG_OPENDRAIN (uint8_t)0x01

/** lowest power PAD configuration, shall be the default */
#define PAD_LOW_POWER    (PAD_PULL_NONE | (PAD_DRIVE_L0<<2) | (PAD_OOUTCFG_OPENDRAIN<<5))

/** custom Power PAD configuration */
#define PAD_OUTPUT_PN_L1_OD (PAD_PULL_NONE | (PAD_DRIVE_L1<<2) | (PAD_OOUTCFG_OPENDRAIN<<5))
#define PAD_INPUT_PD_L1_PP  (PAD_PULL_DOWN | (PAD_DRIVE_L1<<2) | (PAD_OUTCFG_PUSHPULL<<5))
#define PAD_UNUSED_PD_L1_PP (PAD_PULL_DOWN | (PAD_DRIVE_L1<<2) | (PAD_OUTCFG_PUSHPULL<<5))

#define PAD_UART_TX    (PAD_PULL_UP | (PAD_DRIVE_L1<<2) | (PAD_OUTCFG_PUSHPULL<<5))
#define PAD_UART_RX    (PAD_PULL_UP | (PAD_DRIVE_L1<<2) | (PAD_OOUTCFG_OPENDRAIN<<5))

/**************************************************************************************************
*                                                                                                 *
*  Type definitions                                                                               *
*                                                                                                 *
**************************************************************************************************/

/** Pad control
 * The pad control peripheral is used to setup any necessary pad parameters
 * not controlled directly via the cross-bar:  output drive strength,
 * push/pull control and output drive type.
 */
typedef struct {
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO0;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO1;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO2;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO3;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO4;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO5;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO6;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO7;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO8;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO9;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO10;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO11;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO12;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO13;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO14;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO15;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO16;
    union {
        struct {
            __IO uint32_t PULL :2; /**< 00 – Pull-down active, 01 – No pull active, 10 – No pull active, 11 – Pull-up active*/
            __IO uint32_t POWER :3; /**< Output Drive Strength*/
            __IO uint32_t TYPE :1; /**< Output Type: 0 – Push/Pull, 1 – Open Drain*/
        } BITS;
        __IO uint32_t WORD;
    } PADIO17;
} PadReg_t, *PadReg_pt;

#endif /* PAD_MAP_H_ */
