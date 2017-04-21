/**
******************************************************************************
* @file test_map.h
* @brief Test hw module register map
* @internal
* @author  ON Semiconductor
* $Rev: 2848 $
* $Date: 2014-04-01 22:48:18 +0530 (Tue, 01 Apr 2014) $
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
* @ingroup test
*
* @details
*/

#ifndef TEST_MAP_H_
#define TEST_MAP_H_

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

#include "architecture.h"

/**************************************************************************************************
*                                                                                                 *
*  Type definitions                                                                               *
*                                                                                                 *
**************************************************************************************************/

/** General test registers
 *
 */
typedef struct {
    __IO uint32_t UNLOCK;
    __IO uint32_t ANA_TEST_MUX;
    __IO uint32_t OVD_ENA_MODE;
    __IO uint32_t OVD_VAL;
    __IO uint32_t ANA_TEST_MODE;
    __IO uint32_t CLK_TEST_MODE;
    union {
        struct {
            __IO uint32_t PAD0:1;
            __IO uint32_t PAD1:1;
            __IO uint32_t FORCE_SOURCE:1;
            __IO uint32_t FORCE_SINK:1;
            __IO uint32_t PD_CONTROL:3;
            __IO uint32_t PAD3:1;
            __IO uint32_t BYPASS_PLL_REG:1;
            __IO uint32_t PAD4:4;
            __IO uint32_t DITHER_MODE:1;
            __IO uint32_t PLL_MODE:1;
            __IO uint32_t FORCE_LOCK:1;
        } BITS;
        __IO uint32_t WORD;
    } PLL_TEST_MODE;
    __IO uint32_t RX_TEST_MODE;
    __IO uint32_t PMU_TEST_MODE;
} TestReg_t, *TestReg_pt;

/** Digital test registers
 *
 */
typedef struct {
    union {
        struct {
            __IO uint32_t PAD0 :4; /**<   */
            __IO uint32_t DIO4 :4; /**< DIO4 Test Mux Control */
            __IO uint32_t DIO5 :4; /**< DIO5 Test Mux Control */
            __IO uint32_t DIO6 :4; /**< DIO6 Test Mux Control */
            __IO uint32_t DIO7 :4; /**< DIO7 Test Mux Control */
            __IO uint32_t DIO8 :4; /**< DIO8 Test Mux Control */
            __IO uint32_t DIO9 :4; /**< DIO9 Test Mux Control */
            __IO uint32_t DIO10 :4; /**< DIO10 Test Mux Control */
        } BITS;
        __IO uint32_t WORD;
    } DIG_TEST_MUX;
    __IO uint32_t DIG_TEST_MODE;
    union {
        struct {
            __IO uint32_t PAD0 :12; /**<   */
            __IO uint32_t DIO5 :3; /**< DIO5 Input Test Mux Control */
            __IO uint32_t DIO6 :3; /**< DIO6 Input Test Mux Control */
            __IO uint32_t DIO7 :3; /**< DIO7 Input Test Mux Control */
            __IO uint32_t DIO8 :3; /**< DIO8 Input Test Mux Control */
            __IO uint32_t DIO9 :3; /**< DIO9 Input Test Mux Control */
            __IO uint32_t DIO10 :3; /**< DIO10 Input Test Mux Control */
        } BITS;
        __IO uint32_t WORD;
    } DIG_IN_TEST_MUX;
    __IO uint32_t SCAN_MODE;
    __IO uint32_t BIST_TEST_MUX;
    __IO uint32_t RAM_DIAG_ADDR;
    __IO uint32_t RAM_DIAG_DATA;
    __IO uint32_t SRAMA_DIAG_COMP;
    __IO uint32_t SRAMB_DIAG_COMP;
    __IO uint32_t RAM_BUF_TEST_MODE;
} TestDigReg_t, *TestDigReg_pt;

/** NVM test registers
 *
 */
typedef struct {
    __O uint32_t PAD;
} TestNvmReg_t, *TestNvmReg_pt;

#endif /* TEST_MAP_H_ */
