/***************************************************************************//**
* \file cyhal_interconnect_impl.h
*
* \brief
* Implementation details for the PSoC4 interconnect.
*
********************************************************************************
* \copyright
* Copyright 2018-2021 Cypress Semiconductor Corporation
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
 * \addtogroup group_hal_impl_interconnect Interconnect (Internal Digital Routing)
 * \ingroup group_hal_impl
 * \{
 * The interconnect system connects the various hardware peripherals using trigger
 * signals. Triggers are output when a particular event occurs or condition is
 * met by one of the peripherals. These triggers can be routed to other
 * peripherals using the interconnect system in order to initiate an action at
 * the destination peripheral.
 *
 * Peripherals must be configured to produce/accept trigger signals. Therefore
 * in practice, there is no need to call \ref \_cyhal_connect_signal manually.
 * Instead, use the per-peripheral cyhal_<PERI>_connect_digital or
 * cyhal_<PERI>_enable_output which will handle configuring the peripheral and
 * making the connections internally.
 *
 * Trigger routing is implemented using trigger multiplexers. A single source
 * trigger can be routed to multiple destinations but a single destination can
 * only be connected to a single source. There are different trigger layouts
 * depending on device architecture.
 */

#if defined(COMPONENT_CAT1A)
/*
 * <b>PSoC 6S1 Triggers:</b>
 * \image html psoc6able2_trigger_layout.png width=800px
 * <b>PSoC 6S2 Triggers:</b>
 * \image html psoc6a2m_trigger_layout.png width=800px
 * <b>PSoC 6S3 Triggers:</b>
 * \image html psoc6a512k_trigger_layout.png width=800px
 * <b>PSoC 6S4 Triggers:</b>
 * \image html psoc6a256k_trigger_layout.png width=800px
 */
#elif defined(COMPONENT_CAT2)
/*
 * <b>PSoC 4000S Triggers:</b>
 * \image html psoc4000s_trigger_layout.png width=540px
 * <b>PSoC 4100S Triggers:</b>
 * \image html psoc4100s_trigger_layout.png width=540px
 * <b>PSoC 4100S Plus Triggers:</b>
 * \image html psoc4100sp_trigger_layout.png width=540px
 * <b>PSoC 4100S Plus 256k Triggers:</b>
 * \image html psoc4100sp256k_trigger_layout.png width=540px
 */
#endif /* defined(COMPONENT_CAT1A) */

#pragma once

#include "cy_result.h"
#include "cyhal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Connects two digital signals on the device using the internal interconnect.
 * A single source can drive multiple destinations, but a destination can be
 * driven by only one source.  If the destination is already connected, or the
 * connection can not be established an error will be returned.
 * @param[in] source The source of the signal to connect
 * @param[in] dest   The destination of the signal to connect
 * @param[in] type   Whether the signal is edge or level triggered
 * @return The status of the connect request
 */
cy_rslt_t _cyhal_connect_signal(cyhal_source_t source, cyhal_dest_t dest, cyhal_signal_type_t type);

/** Disconnects two digital signals on the device that were previously
 * connected using the internal interconnect.
 * @param[in] source The source of the signal to disconnect
 * @param[in] dest   The destination of the signal to disconnect
 * @return The status of the disconnect request
 */
cy_rslt_t _cyhal_disconnect_signal(cyhal_source_t source, cyhal_dest_t dest);

/** Checks to see if a signal can be connected between the provided source and dest.
 * @param[in] source The source of the signal to check
 * @param[in] dest   The destination of the signal to check
 * @return Indication of whether a signal can connect between the provided points
 */
bool _cyhal_can_connect_signal(cyhal_source_t source, cyhal_dest_t dest);

#if defined(__cplusplus)
}
#endif

/** \} group_hal_impl_interconnect */
