/***************************************************************************//**
* \file cyhal_comp_lp.h
*
* Provides an implementation of the comp HAL on top of the LP (low power) block
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/**
* \addtogroup group_hal_comp_lp COMP (LP Comparator block)
* \ingroup group_hal_impl
* \{
* Implementation of the analog comparator (COMP) driver on top of the LP (low power) comparator.
*
*/

#pragma once

#include "cyhal_comp.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Initialize the Comparator peripheral for a LP-based comparator.
 *
 * @param[out] obj          Pointer to a Comparator object. The caller must allocate the memory
 *                            for this object but the init function will initialize its contents.
 * @param[in]  vin_p        Non-inverting input pin
 * @param[in]  vin_m        Inverting input pin
 * @param[in]  output       Comparator output pin. May be @ref NC.
 * @param[in]  cfg          Configuration structure
 * @return The status of the init request
 */
cy_rslt_t _cyhal_comp_lp_init(cyhal_comp_t *obj, cyhal_gpio_t vin_p, cyhal_gpio_t vin_m, cyhal_gpio_t output, cyhal_comp_config_t *cfg);

/** Deinitialize the Comparator peripheral for a LP-based comparator.
 *
 * @param[in] obj Comparator object
 */
void _cyhal_comp_lp_free(cyhal_comp_t *obj);

/** Changes the current operating power level of the comparator for a LP-based comparator.
 *
 * If the power level is set to @ref CYHAL_POWER_LEVEL_OFF, the comparator will be powered-off
 * but it will retain its configuration, so it is not necessary to reconfigure it when changing
 * the power level from @ref CYHAL_POWER_LEVEL_OFF to any other value.
 *
 * @param[in] obj   Comparator object
 * @param[in] power The power level to set
 * @return The status of the set power request
 */
cy_rslt_t _cyhal_comp_lp_set_power(cyhal_comp_t *obj, cyhal_power_level_t power);

/** Reconfigure the Comparator for a LP-based comparator.
 *
 * This retains the powered state of the comparator.
 * Depending on the implementation, it may be necessary to temporarily deconfigure and/or
 * power off the comparator in order to apply the new configuration. However, if the
 * comparator is powered-off when this function is called, it will remain powered-off after
 * it returns. Likewise, if the comparator is powered-on when this function is called,
 * it will remain powered-on after it returns.
 *
 * @param[in] obj  Comparator object
 * @param[in] cfg  New configuration to apply
 * @return The status of the configure request
 */
cy_rslt_t _cyhal_comp_lp_configure(cyhal_comp_t *obj, cyhal_comp_config_t *cfg);

/** Reads the Comparator state for a LP-based comparator.
 *
 * @param[in]  obj    Comparator object
 * @return The Comparator state. True if the non-inverting pin voltage is greater than the
 * inverting pin voltage, false otherwise.
 */
bool _cyhal_comp_lp_read(cyhal_comp_t *obj);

/** Enable or Disable a Comparator event for a LP-based comparator
 *
 * When an enabled event occurs, the function specified by \ref cyhal_comp_register_callback will be called.
 *
 * @param[in] obj            Comparator object
 * @param[in] event          Comparator event
 * @param[in] intr_priority  Priority for NVIC interrupt events
 * @param[in] enable         True to turn on event, False to turn off
 */
void _cyhal_comp_lp_enable_event(cyhal_comp_t *obj, cyhal_comp_event_t event, uint8_t intr_priority, bool enable);


#if defined(__cplusplus)
}
#endif

/** \} group_hal_comp */
