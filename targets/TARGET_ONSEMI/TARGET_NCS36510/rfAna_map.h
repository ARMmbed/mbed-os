/**
******************************************************************************
* @file rfAna_map.h
* @brief rfAna hw module register map
* @internal
* @author  ON Semiconductor
* $Rev: 2953 $
* $Date: 2014-09-15 18:13:01 +0530 (Mon, 15 Sep 2014) $
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
* @ingroup rfAna
*
* @details
* <p>
* Rf and Analog control and trimming hw module register map
* </p>
*/

#ifndef RFANA_MAP_H_
#define RFANA_MAP_H_

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

/** rfAna register map (control part) */
typedef struct {
    union {
        struct {
            __IO uint32_t FRACT_WORD:24;
            __IO uint32_t INT_WORD:8;
        } BITS;
        __IO uint32_t WORD;
    } TX_LO_CONTROL;
    union {
        struct {
            __IO uint32_t FRACT_WORD:24;
            __IO uint32_t INT_WORD:8;
        } BITS;
        __IO uint32_t WORD;
    } RX_LO_CONTROL;
    union {
        struct {
            __IO uint32_t PLL_RESET_TIME:10;
            __I uint32_t RESERVED:6;
            __IO uint32_t PLL_LOCK_TIME:10;
        } BITS;
        __IO uint32_t WORD;
    } PLL_TIMING;
    union {
        struct {
            __IO uint32_t LNA_GAIN_MODE:1;
            __IO uint32_t ADC_DITHER_MODE:1;
        } BITS;
        __IO uint32_t WORD;
    } RX_CONTROL;
    __IO uint32_t TX_POWER;
    __I uint32_t RECEIVER_GAIN;
} RfAnaReg_t, *RfAnaReg_pt;

/** rfAna register map (trimming part) */
typedef struct {
    __IO uint32_t PMU_TRIM;
    __IO uint32_t RESERVED;
    __IO uint32_t RX_CHAIN_TRIM;
    union {
        struct {
            __I uint32_t BIAS_VCO_TRIM:4;
            __I uint32_t MODULATION_TRIM:4;
            __IO uint32_t TX_VCO_TRIM:4;
            __IO uint32_t RX_VCO_TRIM:4;
            __I uint32_t DIV_TRIM:3;
            __I uint32_t REG_TRIM:2;
            __I uint32_t LFR_TRIM:3;
            __I uint32_t PAD0:4;
            __I uint32_t CHARGE_PUMP_RANGE:4;
        } BITS;
        __IO uint32_t WORD;
    } PLL_TRIM;
    __IO uint32_t PLL_VCO_TAP_LOCATION;
    union {
        struct {
            __IO uint32_t TX_TUNE:4;
            __IO uint32_t PA_REGULATOR_TRIM:4;
            __IO uint32_t REGULATOR_TRIM:2;
            __IO uint32_t RESERVED:2;
        } BITS;
        __IO uint32_t WORD;
    } TX_TRIM;

    __IO uint32_t RX_VCO_TRIM_LUT2;             /** 0x40019098 */
    __IO uint32_t RX_VCO_TRIM_LUT1;             /** 0x4001909C */
    __IO uint32_t TX_VCO_TRIM_LUT2;             /** 0x400190A0 */
    __IO uint32_t TX_VCO_TRIM_LUT1;             /** 0x400190A4 */
    __IO uint32_t ADC_OFFSET_BUF;               /** 0x400190A8 */
} RfAnaTrimReg_t, *RfAnaTrimReg_pt;

#endif /* RFANA_MAP_H_ */
