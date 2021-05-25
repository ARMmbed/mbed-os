/***************************************************************************//**
* \file cy_ppu_v1.h
* \version 1.0
*
* This file provides the header for ARM PPU Player specific driver
*
********************************************************************************
* \copyright
* Copyright 2016-2020 Cypress Semiconductor Corporation
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



#ifndef CY_PPU_V1_H
#define CY_PPU_V1_H

//#include <stdbool.h>
//#include <stddef.h>
#include <stdint.h>
//#include <fwk_id.h> //TBD
//#include <mod_power_domain.h> //TBD
#include "ppu_v1.h"

/*!
 * \addtogroup GroupModules Modules
 * @{
 */

/*!
 * \defgroup GroupModulePPUv1 PPUv1 Driver
 * @{
 */

/*!
 * \brief Indexes of the interfaces exposed by the module.
 */
enum cy_ppu_v1_api_idx {
    /*! Power domain driver API */
    CY_PPU_V1_API_IDX_POWER_DOMAIN_DRIVER,
    /*! interrupt Service Routine driver API */
    CY_PPU_V1_API_IDX_ISR,
    /*! System boot API */
    CY_PPU_V1_API_IDX_BOOT,
    /*! Number of exposed interfaces */
    CY_PPU_V1_API_IDX_COUNT,
};

/*!
 * \brief Types of power domain.
 */
enum cy_pd_type {
    /*! Processor. */
    CY_PD_TYPE_CPUSS,
    /*! Processor cluster. */
    CY_PD_TYPE_CLUSTER,
    /*! Generic device. */
    CY_PD_TYPE_DEVICE,
    /*! Debug device. */
    CY_PD_TYPE_DEVICE_DEBUG,
    /*! System. */
    CY_PD_TYPE_SYSTEM,
    /*! Number of power domain types. */
    CY_PD_TYPE_COUNT
};


/**
* \addtogroup group_sysclk_returns
* \{
*/
/** Defines general-purpose function return values */
typedef enum
{
    CY_PPU_SUCCESS       = 0x00UL, /**< Command completed with no errors */
    CY_PPU_BAD_PARAM     = 0x01UL, /**< Invalid function input parameter */
    CY_PPU_TIMEOUT       = 0x02UL, /**< Timeout occurred */
    CY_PPU_INVALID_STATE = 0x03UL  /**< Clock is in an invalid state */
} cy_en_ppu_status_t;
/** \} group_sysclk_returns */

/*!
 * \brief Identifiers of the power domain states. The other states are defined
 *      by the platform code for more flexibility. The power states defined by
 *      the platform must be ordered from the shallowest to the deepest state.
 */
enum cy_pd_state {
    /*! \c OFF power state */
    CY_PD_STATE_OFF,

    /*! \c ON power state */
    CY_PD_STATE_ON,

    /*! \c SLEEP power state */
    CY_PD_STATE_SLEEP,

    /*! Number of power states */
    CY_PD_STATE_COUNT
};

/*!
 * \brief Masks for the power domain states.
 */
enum cy_pd_state_mask {
    CY_PD_STATE_OFF_MASK = 1 << CY_PD_STATE_OFF,
    CY_PD_STATE_ON_MASK = 1 << CY_PD_STATE_ON,
    CY_PD_STATE_SLEEP_MASK = 1 << CY_PD_STATE_SLEEP,
};

/*!
 * \brief Types of system shutdown
 */
enum cy_pd_system_shutdown {
    /*! System shutdown */
    CY_PD_SYSTEM_SHUTDOWN,

    /*! System cold reset */
    CY_PD_SYSTEM_COLD_RESET,

    /*! System warm reset */
    CY_PD_SYSTEM_WARM_RESET,

    /*! Sub-system reset */
    CY_PD_SYSTEM_SUB_SYSTEM_RESET,

    /*! Forced system shutdown */
    CY_PD_SYSTEM_FORCED_SHUTDOWN,

    /*! Number of shutdown types */
    CY_PD_SYSTEM_COUNT,
};


/*!
 * \brief Power domain PPU descriptor.
 */
struct cy_ppu_v1 {
    /*! Base address of the PPU registers */
    uintptr_t reg_base;

    /*! PPU's IRQ number */
    unsigned int irq;
};

/*!
 * \brief PPU_V1 module configuration
 */
struct cy_ppu_v1_config ;

/*!
 * \brief Configuration data of a power domain of the PPU_V1 driver module.
 */
struct cy_ppu_v1_pd_config {
    /*! Power domain type */
    enum cy_pd_type pd_type;

    /*! PPU descriptor */
    struct cy_ppu_v1 ppu;

    /*!
     *  In the case of a core power domain, identifier of the cluster power
     *  domain it belongs to. If the power domain is not a core power domain,
     *  the value of this field is undefined.
     */
    //fwk_id_t cluster_id; //TBD

    /*!
     * Flag indicating if this domain should be powered on during element
     * init. This flag is only supported for device and system PPUs and should
     * not be set for any other type.
     */
    bool default_power_on;

    /*!
     * \brief Identifier of an entity wishing to be notified when the PPU
     *     transitions out of the OFF state.
     *
     * \note This field may be set to \ref FWK_ID_NONE, in which case no
     *     observer will be set.
     */
    //fwk_id_t observer_id; //TBD

    /*!
     * \brief Identifier of the power state observer API implemented by
     *     \ref observer_id.
     */
    //fwk_id_t observer_api; //TBD

    /*!
     * \brief Parameter passed to
     *     \ref mod_ppu_v1_power_state_observer_api::post_ppu_on().
     */
    void *post_ppu_on_param;
};

/*!
 * \brief PPU_V1 Power State Observer API.
 *
 * \details This API should be implemented by any modules that should be
 *     notified when a PPU changes state.
 */
struct cy_ppu_v1_power_state_observer_api {
    /*!
     * \brief Called after a PPU has turned on.
     *
     * \param param Generic configurable parameter.
     */
    void (*post_ppu_on)(void *param);
};

/*!
 * \brief PPU_V1 module ISR API
 */
struct ppu_v1_isr_api {
    /*!
     * \brief Handle a power domain PPU interrupt
     *
     * \param pd_id Identifier of the power domain
     */
    void (*ppu_interrupt_handler)(struct ppu_v1_reg *ppu); //TBD
};

/*!
 * \brief PPU_V1 module boot API
 */
struct ppu_v1_boot_api {
    /*!
     * \brief Power on a specified power domain
     *
     * \param pd_id Identifier of the power domain
     *
     * \retval FWK_SUCCESS Operation successful.
     * \return One of the standard framework error codes.
     */
    int (*power_mode_on)(struct ppu_v1_reg *ppu); //TBD
};

/*!
 * @}
 */

/*!
 * @}
 */

#endif /* CY_PPU_V1_H */
