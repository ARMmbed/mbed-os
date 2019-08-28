/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "nsdynmemLIB.h"
#include "mac_filter_api.h"
#include "mac_filter.h"
#include "mac_common_defines.h"
#include "mac_defines.h"
#include "mac_mcps.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/IEEE802_15_4/mac_header_helper_functions.h"

#define TRACE_GROUP "mFil"

// Signal floor assumed to be the noise level of radio for packet we cannot hear
#define MAC_FILTER_SIGNAL_FLOOR -94

// define this if you want extra debug info for this file
//#define EXTRA_DEBUG_INFO

#ifdef EXTRA_DEBUG_INFO
#define tr_debug_extra(...) tr_debug(__VA_ARGS__)
#else
#define tr_debug_extra(...)
#endif

typedef struct filter_struct {

    union address_union {
        uint16_t mac16;
        uint8_t mac64[8];
    } address;
    int16_t lqi_m;
    int16_t lqi_add;
    int16_t dbm_m;
    int16_t dbm_add;
    ns_list_link_t link;
} filter_t;

typedef NS_LIST_HEAD(filter_t, link) filter_list_t;

typedef struct instance_struct {
    filter_list_t short_filter_list;
    filter_list_t long_filter_list;
    mac_filter_map_short_to_extended_cb *resolve_long_cb;
    mac_filter_map_extented_to_short_cb *resolve_short_cb;
    bool enabled: 1;
    int8_t interface_id;
    int16_t lqi_m;
    int16_t lqi_add;
    int16_t dbm_m;
    int16_t dbm_add;
    ns_list_link_t link;
} filter_instance_t;

static NS_LIST_DEFINE(instance_list, filter_instance_t, link);

static filter_instance_t *filter_instance_find(int8_t interface_id)
{
    filter_instance_t *this = NULL;
    ns_list_foreach(filter_instance_t, cur_ptr, &instance_list) {
        if (cur_ptr->interface_id == interface_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}
static filter_t *filter_find_long(filter_instance_t *instance_ptr, uint8_t mac64[8])
{
    filter_t *this = NULL;
    tr_debug_extra("mac_filter_find_long %s", trace_array(mac64, 8));

    ns_list_foreach(filter_t, cur_ptr, &instance_ptr->long_filter_list) {
        if (memcmp(cur_ptr->address.mac64, mac64, 8) == 0) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static filter_t *filter_find_short(filter_instance_t *instance_ptr, uint16_t mac16)
{
    filter_t *this = NULL;

    ns_list_foreach(filter_t, cur_ptr, &instance_ptr->short_filter_list) {
        if (cur_ptr->address.mac16 == mac16) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}
static filter_instance_t *mac_filter_create(int8_t interface_id)
{
    filter_instance_t *this;
    tr_debug_extra("mac_filter_create");
    this = ns_dyn_mem_alloc(sizeof(filter_instance_t));
    if (!this) {
        return NULL;
    }
    memset(this, 0, sizeof(filter_instance_t));
    this->interface_id = interface_id;
    this->enabled = false;
    ns_list_init(&this->long_filter_list);
    ns_list_init(&this->short_filter_list);
    ns_list_add_to_start(&instance_list, this);
    return this;
}

static void mac_filter_delete(filter_instance_t *this)
{
    tr_debug_extra("mac_filter_delete");
    ns_list_remove(&instance_list, this);
    ns_dyn_mem_free(this);
    return;
}


/**
 * Public functions.
 */

int_fast8_t mac_filter_start(int8_t interface_id, int16_t lqi_m, int16_t lqi_add, int16_t dbm_m, int16_t dbm_add)
{
    filter_instance_t *this = filter_instance_find(interface_id);

    tr_debug_extra("mac_filter_start");
    if (!this) {
        this = mac_filter_create(interface_id);
    }
    if (!this) {
        return -1;
    }
    this->enabled = true;
    this->lqi_m = lqi_m;
    this->lqi_add = lqi_add;
    this->dbm_m = dbm_m;
    this->dbm_add = dbm_add;
    return 0;
}

void mac_filter_stop(int8_t interface_id)
{
    filter_instance_t *this = filter_instance_find(interface_id);
    tr_debug_extra("mac_filter_stop");
    if (!this) {
        return;
    }

    mac_filter_clear(interface_id);
    this->enabled = false;
    if (!this->resolve_long_cb && !this->resolve_short_cb && !this->enabled) {
        // no usage anymore we can safely delete this
        mac_filter_delete(this);
    }
}

int_fast8_t mac_filter_set_address_mapping(int8_t interface_id, mac_filter_map_short_to_extended_cb *long_cb, mac_filter_map_extented_to_short_cb *short_cb)
{
    filter_instance_t *this = filter_instance_find(interface_id);

    tr_debug_extra("mac_filter_set_address_mapping");
    if (!this) {
        this = mac_filter_create(interface_id);
    }
    if (!this) {
        return -1;
    }
    this->resolve_long_cb = long_cb;
    this->resolve_short_cb = short_cb;
    if (!this->resolve_long_cb && !this->resolve_short_cb && !this->enabled) {
        // no usage anymore we can safely delete this
        mac_filter_delete(this);
    }
    return 0;
}

int_fast8_t mac_filter_clear(int8_t interface_id)
{
    filter_instance_t *this = filter_instance_find(interface_id);
    tr_debug_extra("mac_filter_clear");
    if (!this) {
        return -1;
    }

    ns_list_foreach_safe(filter_t, cur_ptr, &this->long_filter_list) {
        ns_list_remove(&this->long_filter_list, cur_ptr);
        ns_dyn_mem_free(cur_ptr);
    }
    ns_list_foreach_safe(filter_t, cur_ptr, &this->short_filter_list) {
        ns_list_remove(&this->short_filter_list, cur_ptr);
        ns_dyn_mem_free(cur_ptr);
    }
    return 0;
}

int_fast8_t mac_filter_delete_short(int8_t interface_id, uint16_t mac16)
{
    filter_instance_t *this = filter_instance_find(interface_id);
    filter_t *filter_ptr;
    if (!this) {
        return -1;
    }
    filter_ptr = filter_find_short(this, mac16);
    if (filter_ptr) {
        ns_list_remove(&this->short_filter_list, filter_ptr);
        ns_dyn_mem_free(filter_ptr);
    }
    return 0;
}

int_fast8_t mac_filter_delete_long(int8_t interface_id, uint8_t mac64[8])
{
    filter_instance_t *this = filter_instance_find(interface_id);
    filter_t *filter_ptr;
    if (!this || !mac64) {
        return -1;
    }
    filter_ptr = filter_find_long(this, mac64);
    if (filter_ptr) {
        ns_list_remove(&this->short_filter_list, filter_ptr);
        ns_dyn_mem_free(filter_ptr);
    }
    return 0;
}

int_fast8_t mac_filter_add_short(int8_t interface_id, uint16_t mac16, int16_t lqi_m, int16_t lqi_add, int16_t dbm_m, int16_t dbm_add)
{
    filter_instance_t *this = filter_instance_find(interface_id);
    filter_t *filter_ptr;
    if (!this) {
        return -1;
    }
    filter_ptr = ns_dyn_mem_alloc(sizeof(filter_t));
    if (!filter_ptr) {
        return -2;
    }

    ns_list_add_to_end(&this->short_filter_list, filter_ptr);
    filter_ptr->address.mac16 = mac16;
    filter_ptr->lqi_m = lqi_m;
    filter_ptr->lqi_add = lqi_add;
    filter_ptr->dbm_m = dbm_m;
    filter_ptr->dbm_add = dbm_add;

    return 0;
}

int_fast8_t mac_filter_add_long(int8_t interface_id, uint8_t mac64[8], int16_t lqi_m, int16_t lqi_add, int16_t dbm_m, int16_t dbm_add)
{
    filter_instance_t *this = filter_instance_find(interface_id);
    filter_t *filter_ptr;
    tr_debug_extra("mac_filter_add_long mac %s filter found lqi_m %d lqi_add %d dbm_m %d dbm_add %d", trace_array(mac64, 8), lqi_m, lqi_add, dbm_m, dbm_add);
    if (!this || !mac64) {
        return -1;
    }
    filter_ptr = ns_dyn_mem_alloc(sizeof(filter_t));
    if (!filter_ptr) {
        return -2;
    }

    ns_list_add_to_end(&this->long_filter_list, filter_ptr);
    memcpy(filter_ptr->address.mac64, mac64, 8);
    filter_ptr->lqi_m = lqi_m;
    filter_ptr->lqi_add = lqi_add;
    filter_ptr->dbm_m = dbm_m;
    filter_ptr->dbm_add = dbm_add;

    return 0;
}

int_fast8_t mac_filter_modify_link_quality(int8_t interface_id, mac_pre_parsed_frame_t *mac_frame)
{
    filter_instance_t *this = filter_instance_find(interface_id);
    filter_t *filter_ptr = NULL;
    int16_t lqi_m;
    int16_t lqi_add;
    int16_t dbm_m;
    int16_t dbm_add;

    if (!this || !mac_frame) {
        return 0;
    }

    if (!this->enabled) {
        return 0;
    }

    tr_debug_extra("mac_filter_modify_link_quality lqi %d dbm %d", mac_frame->LQI, mac_frame->dbm);

    if (mac_frame->fcf_dsn.SrcAddrMode == MAC_ADDR_MODE_NONE) {
        return 0;
    }
    uint8_t srcAddress[8];
    mac_header_get_src_address(&mac_frame->fcf_dsn, mac_header_message_start_pointer(mac_frame), srcAddress);


//Find filter for specific address
    if (mac_frame->fcf_dsn.SrcAddrMode == MAC_ADDR_MODE_16_BIT) {
        uint16_t mac16 = common_read_16_bit(srcAddress);
        filter_ptr = filter_find_short(this, mac16);
        if (!filter_ptr && this->resolve_long_cb) {
            // Get the long mapping
            uint8_t mac64[8];
            if (this->resolve_long_cb(this->interface_id, mac64, mac16) == 0) {
                filter_ptr = filter_find_long(this, mac64);
            }
        }
    } else {
        filter_ptr = filter_find_long(this, srcAddress);
        if (!filter_ptr && this->resolve_short_cb) {
            // Get the short mapping
            uint16_t mac16;
            if (this->resolve_short_cb(this->interface_id, srcAddress, &mac16) == 0) {
                filter_ptr = filter_find_short(this, mac16);
            }
        }
    }

    // check if filter found or use the defaults
    if (filter_ptr) {
        lqi_m = filter_ptr->lqi_m;
        lqi_add = filter_ptr->lqi_add;
        dbm_m = filter_ptr->dbm_m;
        dbm_add = filter_ptr->dbm_add;
        tr_debug_extra("mac_filter_modify_link_quality filter found lqi_m %d lqi_add %d dbm_m %d dbm_add %d", lqi_m, lqi_add, dbm_m, dbm_add);
    } else {
        lqi_m = this->lqi_m;
        lqi_add = this->lqi_add;
        dbm_m = this->dbm_m;
        dbm_add = this->dbm_add;
    }

    //calculate
    int16_t lqi = ((mac_frame->LQI * lqi_m) >> 8) + lqi_add;
    // Saturate
    if (lqi > 255) {
        mac_frame->LQI = 255;
    } else  if (lqi < 0) {
        mac_frame->LQI = 0;
    } else {
        mac_frame->LQI = lqi;
    }

    //calculate
    int16_t dbm = (((mac_frame->dbm - MAC_FILTER_SIGNAL_FLOOR) * dbm_m) >> 8) + dbm_add + MAC_FILTER_SIGNAL_FLOOR;
    // Saturate
    if (dbm > 127) {
        mac_frame->dbm = 127;
    } else  if (dbm < -128) {
        mac_frame->dbm = -128;
    } else {
        mac_frame->dbm = dbm;
    }

    tr_debug_extra("mac_filter_modify_link_quality result lqi %d dbm %d", mac_frame->LQI, mac_frame->dbm);
    // If quality goes below treshold packet is dropped
    if ((dbm_m != 0x100 || dbm_add != 0) && (mac_frame->dbm < MAC_FILTER_SIGNAL_FLOOR)) {
        tr_debug_extra("Filter dropped packet signal too low");
        return 1;
    }

    if ((lqi_m != 0x100 || lqi_add != 0) && (mac_frame->LQI < 1)) {
        tr_debug_extra("Filter dropped packet LQI < 1");
        return 1;
    }
    return 0;
}

