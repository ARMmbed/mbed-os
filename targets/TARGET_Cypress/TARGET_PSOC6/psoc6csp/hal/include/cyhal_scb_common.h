/***************************************************************************//**
* \file cyhal_scb_common.h
*
* \brief
* Provides a struct definitions for configuration resources in the SCB (UART, I2C, SPI).
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

/** \cond INTERNAL */
/**
 * \addtogroup group_hal_impl_scb_common SCB Common Functionality
 * \ingroup group_hal_impl
 * \{
 * Code shared between the SCB resources (UART, I2C, and SPI).
 */

#pragma once

#include "cy_device.h"
#include "cy_pdl.h"
#include "cyhal_utils.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(CY_IP_MXSCB_INSTANCES)
#if defined(CY_DEVICE_PSOC6A256K)
//Special case for 256k device which has 6 SCBs numbered 0, 1, 2, 4, 5, 6
#define _SCB_ARRAY_SIZE                 (CY_IP_MXSCB_INSTANCES + 1)
#else
#define _SCB_ARRAY_SIZE                 (CY_IP_MXSCB_INSTANCES)
#endif /* CY_DEVICE_PSOC6A256K */
#elif defined(CY_IP_M0S8SCB_INSTANCES)
#define _SCB_ARRAY_SIZE                 (CY_IP_M0S8SCB_INSTANCES)
#endif /* CY_IP_MXSCB_INSTANCES */

/** The start address of the SCB blocks */
extern CySCB_Type* const _CYHAL_SCB_BASE_ADDRESSES[_SCB_ARRAY_SIZE];
/** The interrupt number of the SCB blocks. */
extern const IRQn_Type _CYHAL_SCB_IRQ_N[_SCB_ARRAY_SIZE];


/** Get the SCB block corresponding to an IRQn.
 *
 * @param[in] irqn The IRQn to get the corresponding block from
 * @return         The corresponding SCB block
 */
uint8_t cyhal_scb_get_block_from_irqn(IRQn_Type irqn);

/** Get the SCB object corresponding to the currently running ISR.
 *
 * @return A pointer to the SCB object corresponding to the currently running ISR.
 */
void *_cyhal_scb_get_irq_obj(void);

/** Sets the desired clocks & data rate to achieve the specified frequency
 * @param[in] base      The I2C object to configure the peri divider for
 * @param[in] block_num The SCB block number being used
 * @praam[in] clock     The clock configuration to apply
 * @praam[in] freq      The desired frequency
 * @param[in] is_slave  Is this an I2C slave (true) or master (false)
 * @return The achieved data rate in Hz, or 0 if there was an error.
 */
uint32_t _cyhal_i2c_set_peri_divider(CySCB_Type *base, uint32_t block_num, cyhal_clock_t *clock, uint32_t freq, bool is_slave);

/** Find an available SCB instance that matches 'pin'.
 * @param pin Pin
 * @param pin_map Pin mapping array
 * @param count Number of entries in pin_map
 * @return Pin map pointer or NULL if none found
 */
const cyhal_resource_pin_mapping_t* _cyhal_scb_find_map(cyhal_gpio_t pin, const cyhal_resource_pin_mapping_t *pin_map, 
                    size_t count, const cyhal_resource_inst_t *block_res);

#define _CYHAL_SCB_FIND_MAP(pin, pin_map) \
                    _cyhal_scb_find_map(pin, pin_map, sizeof(pin_map)/sizeof(cyhal_resource_pin_mapping_t), NULL)
#define _CYHAL_SCB_FIND_MAP_BLOCK(pin, pin_map, block) \
                    _cyhal_scb_find_map(pin, pin_map, sizeof(pin_map)/sizeof(cyhal_resource_pin_mapping_t), block)

/**
 * Function pointer to determine a specific scb instance can is ready for low power transition.
 */
typedef bool (*cyhal_scb_instance_pm_callback)(void *obj_ptr, cyhal_syspm_callback_state_t state, cy_en_syspm_callback_mode_t pdl_mode);

/** Updates data in cyhal_scb_config_structs and cyhal_scb_config_modes_t structs based on block_num proveded
 * @param[in] block_num Index of SCB block which data to be updated
 * @param[in] obj       SCB-based driver object (cyhal_uart_t, cyhal_spi_t, cyhal_i2c_t or cyhal_ezi2c_t)
 */
void _cyhal_scb_update_instance_data(uint8_t block_num, void *obj, cyhal_scb_instance_pm_callback pm_callback);

/** Whether power management transition is pending and communication should be suspended. */
bool _cyhal_scb_pm_transition_pending(void);

/** Finds the en_clk_dst_t clock connection index for provided SCB block number
 *
 * @param[in] block_num Index of SCB block
 * @return en_clk_dst_t clock connection index
 */
static inline en_clk_dst_t _cyhal_scb_get_clock_index(uint32_t block_num)
{
    en_clk_dst_t clk;
    // PSOC6A256K does not have SCB 3
    #if defined(CY_DEVICE_PSOC6A256K)
    if (block_num < 3)
        clk = (en_clk_dst_t)((uint32_t)PCLK_SCB0_CLOCK + block_num);
    else
        clk = (en_clk_dst_t)((uint32_t)PCLK_SCB0_CLOCK + block_num -1);
    #else
        clk = (en_clk_dst_t)((uint32_t)PCLK_SCB0_CLOCK + block_num);
    #endif
    return clk;
}

#if defined(__cplusplus)
}
#endif

/** \} group_hal_impl_scb_common */
/** \endcond */
