/*
 * Copyright (c) 2014-2021, Pelion and affiliates.
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
#include "common_functions.h"
#include "ns_trace.h"
#include "string.h"
#include "nsdynmemLIB.h"
#include "platform/arm_hal_phy.h"
#include "net_interface.h"

#include "Core/include/ns_address_internal.h"
#include "MLE/mle.h"
#include "NWK_INTERFACE/Include/protocol_abstract.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "Service_Libs/etx/etx.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"
#include "Service_Libs/utils/isqrt.h"

//TODO: Refactor this away!
#include "MAC/rf_driver_storage.h"

#define TRACE_GROUP "etx"

typedef struct {
    uint8_t attribute_index;
    const uint8_t *mac64;
} ext_neigh_info_t;

static uint16_t etx_current_calc(uint16_t etx, uint8_t accumulated_failures);
static uint16_t etx_dbm_lqi_calc(uint8_t lqi, int8_t dbm);
static void etx_value_change_callback_needed_check(uint16_t etx, uint16_t *stored_diff_etx, uint8_t accumulated_failures, ext_neigh_info_t *etx_neigh_info);
static void etx_accum_failures_callback_needed_check(etx_storage_t *entry, uint8_t attribute_index);
static void etx_cache_entry_init(uint8_t attribute_index);

#if ETX_ACCELERATED_SAMPLE_COUNT == 0 || ETX_ACCELERATED_SAMPLE_COUNT > 6
#error "ETX_ACCELERATED_SAMPLE_COUNT accepted values 1-6"
#endif

#if ETX_ACCELERATED_INTERVAL == 0
#error "ETX_ACCELERATED_INTERVAL can't be zero"
#endif

#if ETX_ACCELERATED_INTERVAL >= ETX_ACCELERATED_SAMPLE_COUNT
#error "ETX_ACCELERATED_INTERVAL must be < ETX_ACCELERATED_SAMPLE_COUNT"
#endif


typedef struct {
    etx_value_change_handler_t *callback_ptr;
    etx_accum_failures_handler_t *accum_cb_ptr;
    etx_storage_t *etx_storage_list;
    etx_sample_storage_t *etx_cache_storage_list;
    uint32_t max_etx_update;
    uint32_t max_etx;
    uint16_t hysteresis;                            // 12 bit fraction
    uint16_t init_etx_sample_count;
    uint8_t accum_threshold;
    uint8_t etx_min_sampling_time;
    uint8_t ext_storage_list_size;
    uint8_t min_attempts_count;
    uint8_t drop_bad_max;
    uint8_t bad_link_level;
    bool cache_sample_requested;
    int8_t interface_id;
} ext_info_t;

static ext_info_t etx_info = {
    .hysteresis = 0,
    .accum_threshold = 0,
    .callback_ptr = NULL,
    .accum_cb_ptr = NULL,
    .etx_storage_list = NULL,
    .etx_cache_storage_list = NULL,
    .ext_storage_list_size = 0,
    .min_attempts_count = 0,
    .drop_bad_max = 0,
    .bad_link_level = 0,
    .max_etx_update = 0,
    .max_etx = 0xffff,
    .init_etx_sample_count = 1,
    .cache_sample_requested = false,
    .etx_min_sampling_time = 0,
    .interface_id = -1
};

static void etx_calculation(etx_storage_t *entry, uint16_t attempts, uint8_t acks_rx, ext_neigh_info_t *etx_neigh_info)
{
    if (etx_info.hysteresis && !entry->stored_diff_etx) {
        if (entry->etx_samples >= etx_info.init_etx_sample_count) {
            entry->stored_diff_etx = entry->etx;
        }
    }

    uint32_t etx = attempts << (12 - ETX_MOVING_AVERAGE_FRACTION);

    if (acks_rx) {
        etx /= acks_rx;
    } else  {
        etx = 0xffff;
    }

    if ((etx_info.max_etx_update) && etx > etx_info.max_etx_update) {
        etx = etx_info.max_etx_update;
    }

    //tr_debug("Attempts %u ACK %u 1/8 update %u", attempts, acks_rx, etx);

    if (etx_info.cache_sample_requested && entry->etx_samples <= etx_info.init_etx_sample_count) {
        // skip the initial value as RSSI generated ETX is not valid
        etx = etx << 3;
    } else {
        //Add old etx 7/8 to new one
        etx += entry->etx - (entry->etx >> ETX_MOVING_AVERAGE_FRACTION);
    }

    if (etx > etx_info.max_etx) {
        etx = etx_info.max_etx;
    }

    // If real ETX value has been received do not update based on LQI or dBm
    entry->tmp_etx = false;

    entry->etx = etx;
    //Clear Drop count
    entry->drop_bad_count = 0;

    if (entry->etx_samples >= etx_info.init_etx_sample_count) {
        etx_cache_entry_init(etx_neigh_info->attribute_index);
        // Checks if ETX value change callback is needed
        etx_value_change_callback_needed_check(entry->etx, &(entry->stored_diff_etx), entry->accumulated_failures, etx_neigh_info);
    }
}

static void etx_cache_entry_init(uint8_t attribute_index)
{
    if (!etx_info.cache_sample_requested) {
        return;
    }

    etx_sample_storage_t *storage = etx_info.etx_cache_storage_list + attribute_index;
    storage->attempts_count = 0;
    storage->transition_count = 0;
    storage->etx_timer = etx_info.etx_min_sampling_time;
    storage->received_acks = 0;
}

static bool etx_update_possible(etx_sample_storage_t *storage, etx_storage_t *entry, uint16_t time_update)
{
    if (storage->etx_timer && time_update) {
        if (time_update >= storage->etx_timer) {
            storage->etx_timer = 0;
        } else {
            storage->etx_timer -= time_update;
        }
    }
    if (entry->etx_samples == etx_info.init_etx_sample_count && time_update == 0) {
        return true;
    }

    if (entry->etx_samples > etx_info.init_etx_sample_count) {
        //Slower ETX update phase
        if (storage->etx_timer == 0 || storage->attempts_count == 0xffff || storage->received_acks == 0xff) {
            //When time is going zero or too much sample data
            if (storage->transition_count >= etx_info.min_attempts_count) {
                //Got least min sample in requested time or max possible sample
                return true;
            } else if (storage->transition_count != storage->received_acks) {
                //Missing ack now ETX can be accelerated
                return true;
            }
        }
    }

    return false;

}


static etx_sample_storage_t *etx_cache_sample_update(uint8_t attribute_index, uint8_t attempts, bool ack_rx)
{
    etx_sample_storage_t *storage = etx_info.etx_cache_storage_list + attribute_index;
    storage->attempts_count += attempts;
    storage->transition_count++;
    if (ack_rx) {
        storage->received_acks++;
    }
    return storage;

}


static bool etx_drop_bad_sample(etx_storage_t *entry, uint8_t attempts, bool success)
{
    if (etx_info.bad_link_level == 0 || !success) {
        //Not enabled or Failure
        return false;
    }

    if (attempts < etx_info.bad_link_level) {
        //under configured value is accepted
        return false;
    }

    if (entry->drop_bad_count < etx_info.drop_bad_max) {
        //Accepted only configured max value 1-2
        entry->drop_bad_count++;
        return true;
    }

    return false;
}

/**
 * \brief A function to update ETX value based on transmission attempts
 *
 *  Update is made based on failed and successful message sending
 *  attempts for a message.
 *
 * \param attempts number of attempts to send message
 * \param success was message sending successful
 * \param addr_type address type, ADDR_802_15_4_SHORT or ADDR_802_15_4_LONG
 * \param addr_ptr PAN ID with 802.15.4 address
 */
void etx_transm_attempts_update(int8_t interface_id, uint8_t attempts, bool success, uint8_t attribute_index, const uint8_t *mac64_addr_ptr)
{
    uint8_t accumulated_failures;
    // Gets table entry
    etx_storage_t *entry = etx_storage_entry_get(interface_id, attribute_index);
    if (!entry) {
        return;
    }

    ext_neigh_info_t etx_neigh_info;
    etx_neigh_info.attribute_index = attribute_index;
    etx_neigh_info.mac64 = mac64_addr_ptr;

    if (entry->etx_samples < 7) {

        if (etx_drop_bad_sample(entry, attempts, success)) {
            tr_debug("Drop bad etx init %u", attempts);
            return;
        }
        entry->etx_samples++;
    }

    if (etx_info.cache_sample_requested) {

        etx_sample_storage_t *storage = etx_cache_sample_update(attribute_index, attempts, success);
        entry->accumulated_failures = 0;

        if (!etx_update_possible(storage, entry, 0)) {
            return;
        }

        etx_calculation(entry, storage->attempts_count, storage->received_acks, &etx_neigh_info);
        return;
    }

    accumulated_failures = entry->accumulated_failures;

    if (!success) {
        /* Stores failed attempts to estimate ETX and to calculate
           new ETX after successful sending */
        if (accumulated_failures + attempts < 32) {
            entry->accumulated_failures += attempts;
        } else {
            success = true;
        }
    }

    if (success) {
        entry->accumulated_failures = 0;
    } else {
        etx_accum_failures_callback_needed_check(entry, attribute_index);
    }

    if (entry->etx) {

        if (success) {
            etx_calculation(entry, attempts + accumulated_failures, 1, &etx_neigh_info);
        }
    }
}

/**
 * \brief A function to update ETX value based on remote incoming IDR
 *
 *  Update is made based on remote incoming IDR received from
 *  neighbor.
 *
 * \param remote_incoming_idr Remote incoming IDR
 * \param mac64_addr_ptr long MAC address
 */
void etx_remote_incoming_idr_update(int8_t interface_id, uint8_t remote_incoming_idr, uint8_t attribute_index, const uint8_t *mac64_addr_ptr)
{
    etx_storage_t *entry = etx_storage_entry_get(interface_id, attribute_index);
    if (!entry) {
        return;
    }

    ext_neigh_info_t etx_neigh_info;
    etx_neigh_info.attribute_index = attribute_index;
    etx_neigh_info.mac64 = mac64_addr_ptr;


    // If ETX has been set
    if (entry->etx) {
        // If hysteresis is set stores ETX value to enable comparison
        if (etx_info.hysteresis && !entry->stored_diff_etx) {
            entry->stored_diff_etx = entry->etx;
        }
        // remote EXT = remote incoming IDR^2 (12 bit fraction)
        uint32_t remote_ext = ((uint32_t)remote_incoming_idr * remote_incoming_idr) << 2;

        // ETX = 7/8 * current ETX + 1/8 * remote ETX */
        uint32_t etx = entry->etx - (entry->etx >> ETX_MOVING_AVERAGE_FRACTION);
        etx += remote_ext >> ETX_MOVING_AVERAGE_FRACTION;

        if (etx > 0xffff) {
            entry->etx = 0xffff;
        } else {
            entry->etx = etx;
        }

        // Checks if ETX value change callback is needed
        etx_value_change_callback_needed_check(entry->etx, &(entry->stored_diff_etx), entry->accumulated_failures, &etx_neigh_info);
    }
    entry->remote_incoming_idr = remote_incoming_idr;

}

/**
 * \brief A function to read ETX value
 *
 *  Returns ETX value for an address
 *
 * \param interface_id network interface id
 * \param addr_type address type, ADDR_802_15_4_SHORT or ADDR_802_15_4_LONG
 * \param addr_ptr PAN ID with 802.15.4 address
 *
 * \return 0x0100 to 0xFFFF ETX value (8 bit fraction)
 * \return 0xFFFF address not associated
 * \return 0x0000 address unknown or other error
 * \return 0x0001 no ETX statistics on this interface
 */
uint16_t etx_read(int8_t interface_id, addrtype_t addr_type, const uint8_t *addr_ptr)
{
    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(interface_id);

    if (!addr_ptr || !interface) {
        return 0;
    }

    if (interface->etx_read_override) {
        // Interface has modified ETX calculation
        return interface->etx_read_override(interface, addr_type, addr_ptr);
    }

    uint8_t attribute_index;
    if (interface->nwk_id == IF_IPV6) {
        return 1;
    }

    //Must Support old MLE table and new still same time
    mac_neighbor_table_entry_t *mac_neighbor = mac_neighbor_table_address_discover(mac_neighbor_info(interface), addr_ptr + PAN_ID_LEN, addr_type);
    if (!mac_neighbor) {
        return 0xffff;
    }
    attribute_index = mac_neighbor->index;

    etx_storage_t *entry = etx_storage_entry_get(interface_id, attribute_index);

    if (!entry) {
        return 0xffff;
    }

    uint16_t etx  = etx_current_calc(entry->etx, entry->accumulated_failures);
    etx >>= 4;

    return etx;
}

/**
 * \brief A function to read local incoming IDR value
 *
 *  Returns local incoming IDR value for an address
 *
 * \param mac64_addr_ptr long MAC address
 *
 * \return 0x0100 to 0xFFFF incoming IDR value (8 bit fraction)
 * \return 0x0000 address unknown
 */
uint16_t etx_local_incoming_idr_read(int8_t interface_id, uint8_t attribute_index)
{
    uint32_t local_incoming_idr = 0;
    etx_storage_t *entry = etx_storage_entry_get(interface_id, attribute_index);
    if (entry) {
        uint16_t local_etx = etx_current_calc(entry->etx, entry->accumulated_failures);

        local_incoming_idr = isqrt32((uint32_t)local_etx << 16);
        // divide by sqrt(2^12)
        local_incoming_idr = local_incoming_idr >> 6;
    }

    return local_incoming_idr;
}

/**
 * \brief A function to read local incoming IDR value
 *
 *  Returns local incoming IDR value for an address
 *
 * \param mac64_addr_ptr long MAC address
 *
 * \return 0x0100 to 0xFFFF incoming IDR value (8 bit fraction)
 * \return 0x0000 address unknown
 */
uint16_t etx_local_etx_read(int8_t interface_id, uint8_t attribute_index)
{
    etx_storage_t *entry = etx_storage_entry_get(interface_id, attribute_index);
    if (!entry) {
        return 0;
    }

    if (etx_info.cache_sample_requested && entry->etx_samples < etx_info.init_etx_sample_count) {
        //Not ready yet
        return 0xffff;
    }

    return etx_current_calc(entry->etx, entry->accumulated_failures) >> 4;
}

/**
 * \brief A function to calculate current ETX
 *
 *  Returns current ETX value based on ETX and failed attempts. Return
 *  value is scaled by scaling factor
 *
 * \param etx ETX (12 bit fraction)
 * \param accumulated_failures failed attempts
 *
 * \return ETX value (12 bit fraction)
 */
static uint16_t etx_current_calc(uint16_t etx, uint8_t accumulated_failures)
{
    uint32_t current_etx;

    // If there is no failed attempts
    if (accumulated_failures == 0) {
        current_etx = etx;
    } else {
        /* Calculates ETX estimate based on failed attempts
           ETX = current ETX + 1/8 * (failed attempts << 12) */
        current_etx = etx + (accumulated_failures << (12 - ETX_MOVING_AVERAGE_FRACTION));
        if (current_etx > 0xffff) {
            current_etx = 0xffff;
        }
    }

    return current_etx;
}

/**
 * \brief A function to update ETX value based on LQI and dBm
 *
 *  Update is made based on dBM and LQI of received message.
 *
 * \param lqi link quality indicator
 * \param dbm measured dBm
 * \param mac64_addr_ptr long MAC address
 *
 * \return 0x0100 to 0xFFFF local incoming IDR value (8 bit fraction)
 */
uint16_t etx_lqi_dbm_update(int8_t interface_id, uint8_t lqi, int8_t dbm, uint8_t attribute_index, const uint8_t *mac64_addr_ptr)
{
    uint32_t local_incoming_idr = 0;
    uint32_t etx = 0;

    etx_storage_t *entry = etx_storage_entry_get(interface_id, attribute_index);


    if (entry) {
        ext_neigh_info_t etx_neigh_info;
        etx_neigh_info.attribute_index = attribute_index;
        etx_neigh_info.mac64 = mac64_addr_ptr;
        // If local ETX is not set calculate it based on LQI and dBm
        if (!entry->etx) {
            etx = etx_dbm_lqi_calc(lqi, dbm);
            entry->etx = etx;
            entry->stored_diff_etx = etx;
            entry->tmp_etx = true;
            if (etx_info.callback_ptr) {
                etx_info.callback_ptr(etx_info.interface_id, 0, entry->etx >> 4,
                                      attribute_index, mac64_addr_ptr);
            }
        }
        // If local ETX has been calculated without remote incoming IDR and
        // remote incoming IDR is available update it by remote incoming IDR value
        if (entry->remote_incoming_idr && entry->tmp_etx) {
            entry->tmp_etx = false;

            local_incoming_idr = isqrt32((uint32_t)entry->etx << 16);
            // divide by sqrt(2^12) and scale to 12 bit fraction
            local_incoming_idr = local_incoming_idr >> 2;

            etx = local_incoming_idr * (((uint16_t)entry->remote_incoming_idr) << 7);
            entry->etx = etx >> 12;

            local_incoming_idr >>= 4;
            etx_value_change_callback_needed_check(entry->etx, &(entry->stored_diff_etx), entry->accumulated_failures, &etx_neigh_info);
        }
    }

    // If local ETX is not set return temporary ETX based on LQI and dB,
    if (!local_incoming_idr) {
        if (!etx) {
            etx = etx_dbm_lqi_calc(lqi, dbm);
        }

        local_incoming_idr = isqrt32(etx << 16);
        // divide by sqrt(2^12)
        local_incoming_idr >>= 6;
    }

    return local_incoming_idr;
}

/**
 * \brief A function to calculate ETX value based on dBm and LQI
 *
 *  Calculation is made using RF driver service. If service does not
 *  exists then local function is used.
 *
 * \param lqi link quality indicator
 * \param dbm measured dBm
 *
 * \return ETX value (12 bit fraction)
 */
static uint16_t etx_dbm_lqi_calc(uint8_t lqi, int8_t dbm)
{
    protocol_interface_info_entry_t *cur = 0;
    int8_t driver_ret_value = -1;
    uint16_t etx;

    phy_signal_info_s signal_info;
    signal_info.type = PHY_SIGNAL_INFO_ETX;
    signal_info.lqi = lqi;
    signal_info.dbm = dbm;
    signal_info.result = 0xffff;

    //TODO: This is needed, but RF driver cannot be accessed directly! Figure out MAC extension for this.
    cur = protocol_stack_interface_info_get(IF_6LoWPAN);
    if ((cur) && (cur->dev_driver) && (cur->dev_driver->phy_driver)) {
        phy_device_driver_s *dev_driver = cur->dev_driver->phy_driver;
        if (dev_driver->extension) {
            driver_ret_value = dev_driver->extension(PHY_EXTENSION_CONVERT_SIGNAL_INFO, (uint8_t *)&signal_info);
        }
    }

    if ((driver_ret_value != -1) && (signal_info.result != 0xffff)) {
        etx = signal_info.result;
        etx <<= 4;
    } else {
        /* Atmel version
           dBm = RSSI base value [dBm] + 1.03 [dB] x ED level
           LQI = errors in received frame */

        // for dBm -90 and LQI 0 ETX will be 2.4
        etx = ((dbm * -1) * (256 - lqi));
        etx >>= 1; // scale result to 12 bits
        etx += 1 << 12; // add one (perfect link)
    }

    return etx;
}

/**
 * \brief A function to register ETX value change callback
 *
 *  Register ETX value change callback. When ETX value has changed more or equal
 *  to hysteresis value ETX module calls ETX value change callback.
 *
 * \param nwk_interface_id network interface id
 * \param hysteresis hysteresis value (8 bit fraction)
 * \param callback_ptr callback function pointer
 *
 * \return 0 not 6LowPAN interface
 * \return 1 success
 */
uint8_t etx_value_change_callback_register(nwk_interface_id nwk_id, int8_t interface_id, uint16_t hysteresis, etx_value_change_handler_t *callback_ptr)
{
    if ((nwk_id == IF_6LoWPAN) && hysteresis && callback_ptr) {
        etx_info.hysteresis = hysteresis << 4;
        etx_info.callback_ptr = callback_ptr;
        etx_info.interface_id = interface_id;
        return 1;
    } else {
        return 0;
    }
}

bool etx_storage_list_allocate(int8_t interface_id, uint8_t etx_storage_size)
{
    if (!etx_storage_size) {
        ns_dyn_mem_free(etx_info.etx_storage_list);
        ns_dyn_mem_free(etx_info.etx_cache_storage_list);
        etx_info.etx_cache_storage_list = NULL;
        etx_info.cache_sample_requested = false;
        etx_info.etx_storage_list = NULL;
        etx_info.ext_storage_list_size = 0;
        return true;
    }

    if (etx_info.ext_storage_list_size == etx_storage_size) {
        return true;
    }

    ns_dyn_mem_free(etx_info.etx_storage_list);
    etx_info.cache_sample_requested = false;
    etx_info.ext_storage_list_size = 0;
    etx_info.etx_storage_list = ns_dyn_mem_alloc(sizeof(etx_storage_t) * etx_storage_size);

    if (!etx_info.etx_storage_list) {
        ns_dyn_mem_free(etx_info.etx_storage_list);
        etx_info.etx_storage_list = NULL;
        etx_info.ext_storage_list_size = 0;
        return false;
    }


    etx_info.ext_storage_list_size = etx_storage_size;
    etx_info.interface_id = interface_id;
    etx_storage_t *list_ptr = etx_info.etx_storage_list;
    for (uint8_t i = 0; i < etx_storage_size; i++) {
        memset(list_ptr, 0, sizeof(etx_storage_t));

        list_ptr++;
    }
    return true;

}

bool etx_cached_etx_parameter_set(uint8_t min_wait_time, uint8_t etx_min_attempts_count, uint8_t init_etx_sample_count)
{
    //No ini ETX allocation done yet
    if (etx_info.ext_storage_list_size == 0) {
        return false;
    }

    if (min_wait_time || etx_min_attempts_count) {
        if (init_etx_sample_count == 0) {
            return false;
        }

        if (!etx_info.etx_cache_storage_list) {
            //allocate
            etx_info.etx_cache_storage_list = ns_dyn_mem_alloc(sizeof(etx_sample_storage_t) * etx_info.ext_storage_list_size);

            if (!etx_info.etx_cache_storage_list) {
                return false;
            }
            etx_info.cache_sample_requested = true;
            etx_sample_storage_t *sample_list = etx_info.etx_cache_storage_list;
            for (uint8_t i = 0; i < etx_info.ext_storage_list_size; i++) {
                memset(sample_list, 0, sizeof(etx_sample_storage_t));
                sample_list++;
            }
        }

    } else {
        //Free Cache table we not need that anymore
        etx_info.cache_sample_requested = false;
        ns_dyn_mem_free(etx_info.etx_cache_storage_list);
        etx_info.etx_cache_storage_list = NULL;
    }

    etx_info.min_attempts_count = etx_min_attempts_count;
    etx_info.etx_min_sampling_time = min_wait_time;
    etx_info.init_etx_sample_count = init_etx_sample_count;

    return true;
}

bool etx_allow_drop_for_poor_measurements(uint8_t bad_link_level, uint8_t max_allowed_drops)
{
    //No ini ETX allocation done yet
    if (etx_info.ext_storage_list_size == 0) {
        return false;
    }

    if (bad_link_level == 0) {
        //Disable feature
        etx_info.bad_link_level = 0;
        etx_info.drop_bad_max = 0;
        return true;
    }

    if (bad_link_level < 2) {
        // 2 attepts is min value
        return false;
    }

    if (max_allowed_drops == 0 || max_allowed_drops > 3) {
        //Accepted values is 1-3
        return false;
    }


    etx_info.bad_link_level = bad_link_level;
    etx_info.drop_bad_max = max_allowed_drops;
    return true;
}

void etx_max_update_set(uint16_t etx_max_update)
{
    if (etx_max_update) {
        //Define MAX ETX UPDATE
        etx_info.max_etx_update = (etx_max_update / 128) << (12 - ETX_MOVING_AVERAGE_FRACTION);
    } else {
        etx_info.max_etx_update = 0;
    }
}

void etx_max_set(uint16_t etx_max)
{
    if (etx_max) {
        //Define MAX ETX possible value
        etx_info.max_etx = (etx_max / 128) << 12;
    } else {
        etx_info.max_etx = 0xffff;
    }
}

etx_storage_t *etx_storage_entry_get(int8_t interface_id, uint8_t attribute_index)
{
    if (etx_info.interface_id != interface_id || !etx_info.etx_storage_list || attribute_index >= etx_info.ext_storage_list_size) {
        return NULL;
    }

    etx_storage_t *entry = etx_info.etx_storage_list + attribute_index;
    return entry;
}


/**
 * \brief A function to register accumulated failures callback
 *
 *  When the number of accumulated failures has reached the threshold
 *  value, the ETX module calls the accumulated failures callback on
 *  every transmission failure.
 *
 * \param nwk_id network ID (6LoWPAN)
 * \param interface_id interface ID
 * \param threshold threshold value for accumulated failures
 * \param callback_ptr callback function pointer
 *
 * \return 0 not 6LowPAN interface
 * \return 1 success
 */
uint8_t etx_accum_failures_callback_register(nwk_interface_id nwk_id, int8_t interface_id, uint8_t threshold, etx_accum_failures_handler_t *callback_ptr)
{
    if ((nwk_id == IF_6LoWPAN) && threshold && callback_ptr) {
        etx_info.interface_id = interface_id;
        etx_info.accum_threshold = threshold;
        etx_info.accum_cb_ptr = callback_ptr;
        return 1;
    } else {
        return 0;
    }
}

/**
 * \brief A function to check if ETX value change callback is needed
 *
 *  Calculates current ETX and compares it against stored ETX. If change
 *  of the values is more than hysteresis calls ETX value change
 *  callback.
 *
 * \param etx ETX (12 bit fraction)
 * \param stored_diff_etx stored ETX value
 * \param accumulated_failures failed attempts
 * \param mac64_addr_ptr long MAC address
 * \param mac16_addr short MAC address or 0xffff address is not set
 *
 * \return ETX value (12 bit fraction)
 */
static void etx_value_change_callback_needed_check(uint16_t etx, uint16_t *stored_diff_etx, uint8_t accumulated_failures, ext_neigh_info_t *etx_neigh_info)
{
    uint16_t current_etx;
    bool callback = false;
    if (!etx_info.hysteresis) {
        return;
    }

    // Calculates current ETX
    current_etx = etx_current_calc(etx, accumulated_failures);

    // If difference is more than hysteresis
    if (current_etx > *stored_diff_etx) {
        if (current_etx - *stored_diff_etx >= etx_info.hysteresis) {
            callback = true;
        } else if (current_etx == etx_info.max_etx && *stored_diff_etx != etx_info.max_etx) {
            callback = true;
        }
    } else if (current_etx < *stored_diff_etx) {
        if (*stored_diff_etx - current_etx >= etx_info.hysteresis) {
            callback = true;
        }
    }

    // Calls callback function
    if (callback) {
        etx_info.callback_ptr(etx_info.interface_id, (*stored_diff_etx) >> 4, current_etx >> 4, etx_neigh_info->attribute_index, etx_neigh_info->mac64);
        *stored_diff_etx = current_etx;
    }
}

/**
 * \brief A function to check if accumulated failures callback is needed
 *
 *  If the current number of accumulated failures is equal or greater than
 *  the set threshold value, the function calls accumulated failures callback.
 *
 * \param neigh_table_ptr the neighbor node in question
 */
static void etx_accum_failures_callback_needed_check(etx_storage_t *entry, uint8_t attribute_index)
{
    if (!etx_info.accum_threshold) {
        return;
    }

    if (entry->accumulated_failures < etx_info.accum_threshold) {
        return;
    }

    etx_info.accum_cb_ptr(etx_info.interface_id, entry->accumulated_failures, attribute_index);
}

/**
 * \brief A function to remove ETX neighbor
 *
 *  Notifies ETX module that neighbor has been removed. Calls ETX value change callback
 *  if that is set.
 *
 * \param mac64_addr_ptr long MAC address
 *
 */
void etx_neighbor_remove(int8_t interface_id, uint8_t attribute_index, const uint8_t *mac64_addr_ptr)
{

    //tr_debug("Remove attribute %u", attribute_index);
    uint16_t stored_diff_etx;
    etx_storage_t *entry = etx_storage_entry_get(interface_id, attribute_index);
    if (entry && etx_info.callback_ptr) {

        if (entry->etx) {
            stored_diff_etx = entry->stored_diff_etx >> 4;
            if (!stored_diff_etx) {
                stored_diff_etx = 0xffff;
            }

            etx_info.callback_ptr(etx_info.interface_id, stored_diff_etx, 0xffff, attribute_index, mac64_addr_ptr);
        }

        if (etx_info.cache_sample_requested) {
            //Clear cached values
            etx_sample_storage_t *cache_entry = etx_info.etx_cache_storage_list + attribute_index;
            memset(cache_entry, 0, sizeof(etx_sample_storage_t));
        }
        //Clear all data base back to zero for new user
        memset(entry, 0, sizeof(etx_storage_t));
    }
}

void etx_cache_timer(int8_t interface_id, uint16_t seconds_update)
{
    if (!etx_info.cache_sample_requested) {
        return;
    }

    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface || !mac_neighbor_info(interface)) {
        return;
    }

    ns_list_foreach(mac_neighbor_table_entry_t, neighbour, &mac_neighbor_info(interface)->neighbour_list) {

        etx_storage_t *etx_entry = etx_storage_entry_get(interface_id, neighbour->index);

        if (!etx_entry || etx_entry->tmp_etx) {
            continue;
        }
        etx_sample_storage_t *storage = etx_info.etx_cache_storage_list + neighbour->index;

        if (etx_update_possible(storage, etx_entry, seconds_update)) {
            ext_neigh_info_t etx_neigh_info;
            etx_neigh_info.attribute_index = neighbour->index;
            etx_neigh_info.mac64 = neighbour->mac64;
            etx_calculation(etx_entry, storage->attempts_count, storage->received_acks, &etx_neigh_info);
        }
    }

}
