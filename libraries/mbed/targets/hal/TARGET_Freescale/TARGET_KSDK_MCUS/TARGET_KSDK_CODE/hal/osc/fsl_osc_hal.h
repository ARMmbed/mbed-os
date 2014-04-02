/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#if !defined(__FSL_OSC_HAL_H__)
#define __FSL_OSC_HAL_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "fsl_device_registers.h"
#include "fsl_osc_features.h"

/*! @addtogroup osc_hal*/
/*! @{*/

/*! @file fsl_osc_hal.h */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Oscillator instance.*/
typedef enum _osc_instance {
    kOsc0 = 0,           /*!< Oscillator 0*/
#if FSL_FEATURE_OSC_HAS_OSC1
    kOsc1 = 1            /*!< Oscillator 1*/
#endif
} osc_instance_t;

/*! @brief Oscillator capacitor load configurations.*/
typedef enum _osc_capacitor_config {
    kOscCapacitor2p = OSC_CR_SC2P_MASK,     /*!< 2 pF capacitor load */
    kOscCapacitor4p = OSC_CR_SC4P_MASK,     /*!< 4 pF capacitor load */
    kOscCapacitor8p = OSC_CR_SC8P_MASK,     /*!< 8 pF capacitor load */
    kOscCapacitor16p = OSC_CR_SC16P_MASK    /*!< 16 pF capacitor load */
} osc_capacitor_config_t;

/*! @brief Oscillator capacitor load configurations mask*/
#define kOscCapacitorMask (OSC_CR_SC2P_MASK | OSC_CR_SC4P_MASK | OSC_CR_SC8P_MASK | OSC_CR_SC16P_MASK) 


/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*! @name oscillator control APIs*/
/*@{*/


/*!
 * @brief Enables the external reference clock for oscillator
 *
 * This function  enables the external reference clock output 
 * for the oscillator - that is the OSCERCLK. This clock  is used
 * by many peripherals. It should be enabled at an early system initialization
 * stage to ensure the peripherals could select it and use it.
 *
 * @param instance Oscillator instance
 */
void osc_hal_enable_external_reference_clock(osc_instance_t instance);

/*!
 * @brief Disables the external reference clock for oscillator.
 *
 * This function  disables the external reference clock output 
 * for oscillator - that is the OSCERCLK. This clock  is used
 * by many peripherals. It should be enabled at an early system initialization
 * stage to ensure the peripherals could select  and use it.
 * 
 * @param instance Oscillator instance
 */
void osc_hal_disable_external_reference_clock(osc_instance_t instance);

/*!
 * @brief Enables the external reference clock in stop mode.
 *
 * This function  enables the external reference clock (OSCERCLK) when
 * MCU enters Stop mode. 
 *
 * @param instance Oscillator instance
 */
void osc_hal_enable_external_reference_clock_in_stop_mode(osc_instance_t instance);

/*!
 * @brief Disables the external reference clock in stop mode.
 *
 * This function  disables the external reference clock (OSCERCLK) when 
 * MCU enters Stop mode. 
 *
 * @param instance Oscillator instance
 */
void osc_hal_disable_external_reference_clock_in_stop_mode(osc_instance_t instance);

/*!
 * @brief Enables the capacitor configuration for oscillator.
 *
 * This function  enables the specified capacitors configuration for the 
 * oscillator. This should be done in the early system level initialization function call
 * based on system configuration.
 *
 * @param instance        Oscillator instance
 * @param capacitorConfig Capacitor configurations. Combination of OSC_CR_SCxP_MASK
 */
void osc_hal_enable_capacitor_config(osc_instance_t instance, uint32_t capacitorConfigs);

/*!
 * @brief Disables the capacitor configuration for specific oscillator.
 *
 * This function  enables the specified capacitors configuration for the 
 * oscillator.
 *
 * @param instance        Oscillator instance
 * @param capacitorConfig Capacitor configurations. Combination of OSC_CR_SCxP_MASK
 */
void osc_hal_disable_capacitor_config(osc_instance_t instance, uint32_t capacitorConfigs);
    
/*@}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* __FSL_OSC_HAL_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

