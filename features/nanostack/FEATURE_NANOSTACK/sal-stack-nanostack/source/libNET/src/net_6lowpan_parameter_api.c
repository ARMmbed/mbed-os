/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
 */

#include "nsconfig.h"
#include "ns_types.h"
#include "string.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "net_6lowpan_parameter_api.h"
#include "6LoWPAN/ND/nd_router_object.h"

/**
 * \brief API to change 6LoWPAN ND bootstrap parameters.
 *
 * Note: This function should be called after net_init_core() and definitely
 * before creating any 6LoWPAN interface.
 *
 * For future compatibility, to avoid problems with extensions to the
 * structure, read the parameters using
 * net_6lowpan_timer_parameter_read(), modify known fields, then set.
 *
 * \param parameter_ptr Pointer for ND parameters
 *
 * \return 0, Change OK
 * \return -1, Invalid values
 * \return -2, 6LoWPAN interface already active
 *
 */
int8_t net_6lowpan_nd_parameter_set(const nd_parameters_s *p)
{
#ifdef HAVE_6LOWPAN_ND
    if (protocol_stack_interface_info_get(IF_6LoWPAN)) {
        return -2;
    }

    if (p->rs_retry_interval_min == 0 || p->ns_retry_interval_min == 0) {
        return -1;
    }

    if ((uint32_t) p->rs_retry_interval_min + p->timer_random_max > 0xFFFF ||
            (uint32_t) p->ns_retry_interval_min + p->timer_random_max +
            ((uint32_t) p->ns_retry_max - 1) * p->ns_retry_linear_backoff > 0xFFFF) {
        return -1;
    }

    /* Random range must be power of two minus 1, so we can just mask */
    if ((p->timer_random_max + 1) & p->timer_random_max) {
        return -1;
    }

    nd_params = *p;

    return 0;
#else
    return -2;
#endif
}

/**
 * \brief API for change 6LoWPAN bootstrap base tick 100ms multiplier.
 *
 * Note: This function MUST be called after net_init_core(). Do not change this
 * unless you really want 6LoWPAN bootstrap working slower than normally.
 * \param base_tick_x_100ms Tick resolution in 100ms units.
 *        Max value 10 --> 10 times slower functionality
 *
 * \return 0, Change OK
 * \return -1, Invalid value (<1 or >10)
 *
 */
int8_t net_6lowpan_nd_timer_base_tick_set(uint8_t base_tick_x_100ms)
{
#ifdef HAVE_6LOWPAN_ND
    if (base_tick_x_100ms < 1 || base_tick_x_100ms > 10) {
        return -1;
    }

    nd_base_tick = base_tick_x_100ms;
#endif
    return 0;
}

/**
 * \brief API to read 6LoWPAN ND bootstrap parameters.
 *
 * \param parameter_ptr Output pointer for ND parameters
 *
 */
void net_6lowpan_nd_parameter_read(nd_parameters_s *p)
{
#ifdef HAVE_6LOWPAN_ND
    *p = nd_params;
#endif
}
