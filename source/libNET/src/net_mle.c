/*
 * Copyright (c) 2016-2017, 2019, Arm Limited and affiliates.
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

/*
 * \file net_mle.c
 * \brief 6LoWPAN MLE options control API
 *
 */
#include "nsconfig.h"
#include "ns_types.h"
#include "string.h"
#include "ns_trace.h"
#include "net_mle_api.h"
#include "Core/include/ns_address_internal.h"
#include "net_interface.h"
#include "MLE/mle.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"

/** Set parameter.

 * \return zero on success, negative on failure.

 */

int8_t arm_nwk_6lowpan_mle_router_lifetime_set(int8_t interface_id, uint16_t lifetime)
{
    protocol_interface_info_entry_t *cur;
    mle_6lowpan_data_t *mle_6lowpan_data;

    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || cur->nwk_id != IF_6LoWPAN) {
        return -1;
    }

    mle_6lowpan_data = protocol_6lowpan_mle_data_get();

    if (!mle_6lowpan_data) {
        return -1;
    }

    if (lifetime < 64 || lifetime > 2560) {
        return -1;
    }

    mle_6lowpan_data->router_lifetime = lifetime;

    return 0;
}

int8_t arm_nwk_6lowpan_mle_host_lifetime_set(int8_t interface_id, uint16_t lifetime)
{
    protocol_interface_info_entry_t *cur;
    mle_6lowpan_data_t *mle_6lowpan_data;

    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || cur->nwk_id != IF_6LoWPAN) {
        return -1;
    }

    mle_6lowpan_data = protocol_6lowpan_mle_data_get();

    if (!mle_6lowpan_data) {
        return -1;
    }

    if (lifetime < 64 || lifetime > 2560) {
        return -1;
    }

    mle_6lowpan_data->host_lifetime = lifetime;

    return 0;
}

int8_t arm_nwk_6lowpan_mle_neighbor_limits_set(int8_t interface_id, uint16_t lower_threshold, uint16_t upper_threshold, uint16_t max)
{
    protocol_interface_info_entry_t *cur;
    mle_6lowpan_data_t *mle_6lowpan_data;

    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || cur->nwk_id != IF_6LoWPAN) {
        return -1;
    }

    if (lower_threshold == 0 || upper_threshold == 0 || max == 0) {
        if (lower_threshold != 0) {
            return -1;
        }
        if (upper_threshold != 0) {
            return -1;
        }
        if (max != 0) {
            return -1;
        }
    }

    if ((lower_threshold < 5 && lower_threshold != 0) || lower_threshold > 499) {
        return -1;
    }
    if ((upper_threshold < 5  && upper_threshold != 0) || upper_threshold > 500) {
        return -1;
    }
    if ((max < 5  && max != 0) || max > 500) {
        return -1;
    }
    if (lower_threshold >= upper_threshold) {
        return -1;
    }
    if (lower_threshold >= max) {
        return -1;
    }
    if (upper_threshold > max) {
        return -1;
    }

    mle_6lowpan_data = protocol_6lowpan_mle_data_get();

    if (!mle_6lowpan_data) {
        return -1;
    }

    mle_6lowpan_data->nbr_of_neigh_lower_threshold = lower_threshold;
    mle_6lowpan_data->nbr_of_neigh_upper_threshold = upper_threshold;
    mle_6lowpan_data->nbr_of_neigh_max = max;

    return 0;
}

int8_t arm_nwk_6lowpan_mle_token_bucket_settings_set(int8_t interface_id, uint8_t size, uint8_t rate, uint8_t count)
{
    protocol_interface_info_entry_t *cur;
    mle_6lowpan_data_t *mle_6lowpan_data;

    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || cur->nwk_id != IF_6LoWPAN) {
        return -1;
    }

    if (size == 0 || rate == 0 || count == 0) {
        if (size != 0) {
            return -1;
        }
        if (rate != 0) {
            return -1;
        }
        if (count != 0) {
            return -1;
        }
    }

    mle_6lowpan_data = protocol_6lowpan_mle_data_get();

    if (!mle_6lowpan_data) {
        return -1;
    }

    mle_6lowpan_data->token_bucket_size = size;
    mle_6lowpan_data->token_bucket_rate = rate;
    mle_6lowpan_data->token_bucket_count = count;

    return mle_service_interface_token_bucket_settings_set(interface_id, size, rate, count);
}

