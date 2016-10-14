/**
******************************************************************************
* @file pmu_map.h
* @brief PMU hw module register map
* @internal
* @author ON Semiconductor
* $Rev: 3372 $
* $Date: 2015-04-22 12:18:18 +0530 (Wed, 22 Apr 2015) $
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
* @ingroup pmu
*
* @details
*/

#ifndef PMU_MAP_H_
#define PMU_MAP_H_

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

/** PMU control
 * The Power Management Unit (PMU) is used to control the differing power modes.
 */
typedef struct {
    union {
        struct {
            __IO uint32_t ENCOMA :1; /**< 0- Sleep or SleepDeep depending on System Control Register (see WFI and WFE instructions), 1 – Coma */
            __IO uint32_t SRAMA :1;  /**< SRAMA Powered in Coma Modes: 0 – SRAM Powered, 1 – SRAM Un-Powered */
            __IO uint32_t SRAMB :1;  /**< SRAMB Powered in Coma Modes: 0 – SRAM Powered, 1 – SRAM Un-Powered */
            __IO uint32_t EXT32K :1; /**< External 32.768kHz Enable: 0 – Disabled (off), 1 – Enabled (on), Hardware guarantees that this oscillator cannot be powered if the internal 32kHz oscillator is already powered down. Hardware insures that one of the 32kHz oscillators is running. */
            __IO uint32_t INT32K :1; /**< Internal 32kHz Enable: 0 – Enabled (on), 1 – Disabled (Off), Hardware guarantees that this oscillator cannot be powered down if the external 32.768kHz oscillator is already powered down. Hardware insures that one of the 32kHz oscillators is running. */
            __IO uint32_t INT32M :1; /**< Internal 32MHz Enable: 0 – Enabled (on), 1 – Disabled (off), This bit will automatically get cleared when exiting Coma, or SleepDeep modes of operation. This bit should be set by software after switching over to the external 32MHz oscillator using the Oscillator Select bit in the Clock Control register */
            __IO uint32_t C1V1:1;    /**< Coma mode 1V1 regulator setting: 0 - Linear regulator, 1 - switching regulator */
            __IO uint32_t N1V1:1;    /**< Regular mode (Run sleep and deepsleep) 1V1 regulator mode: 0 - Linear regulator, 1 - switching regulator */
            __IO uint32_t DBGPOW :1; /**< Debugger Power Behavior: 0 – Normal power behavior when the debugger is present, 1 – When debugger is present the ASIC can only enter SleepDeep mode and FVDDH and FVDDL always remain powered. The 32MHz oscillators can never be powered down in this mode either. */
            __IO uint32_t UVIC:1;    /**< Under voltage indicator control: 0 - disabled, 1 - enabled */
            __IO uint32_t UVII:1;    /**< Under voltage indicator input: 0 - 1V1 regulator, 1 - FVDDH regulator */
            __IO uint32_t UVIR:1;    /**< Under voltage indicator reset: 0 - do not reset, 1 - reset */
        } BITS;
        __IO uint32_t WORD;
    } CONTROL;        /* 0x4001D000 */
    union {
        struct {
            __I uint32_t BATTDET:1;    /**< Detected battery: 0 - 1V, 1 - 3V */
            __I uint32_t UVIC:1;       /**< Under voltage status: 0 - normal, 1 - low */

        } BITS;
        __IO uint32_t WORD;
    } STATUS;         /* 0x4001D004 */

    __IO uint32_t PLACEHOLDER;      /* 0x4001D008 */
    __IO uint32_t FVDD_TSTARTUP;    /**< Regulator start time. */    /* 0x4001D00C */
    __IO uint32_t PLACEHOLDER1;     /* 0x4001D010 */
    __IO uint32_t FVDD_TSETTLE;     /**< Regulator settle time. */    /* 0x4001D014 */
    union {
        struct {
            __IO uint32_t TH:6;       /**< Threshold */
            __I  uint32_t PAD:2;
            __I  uint32_t UVIVAL:6;   /**< UVI value */
        } BITS;
        __IO uint32_t WORD;
    } UVI_TBASE;                     /* 0x4001D018 */
    __IO uint32_t SRAM_TRIM;         /* 0x4001D01C */

} PmuReg_t, *PmuReg_pt;

#endif /* PMU_MAP_H_ */
