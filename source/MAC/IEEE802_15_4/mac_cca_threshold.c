/*
 * Copyright (c) 2020, Pelion and affiliates.
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
#include "string.h"
#include "nsconfig.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/mac_cca_threshold.h"

#define TRACE_GROUP "mcth"

int8_t mac_cca_thr_init(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t number_of_channels, int8_t default_dbm, int8_t high_limit, int8_t low_limit)
{
    // No changes
    if (rf_ptr->cca_threshold &&
            (number_of_channels == rf_ptr->cca_threshold->number_of_channels) &&
            (default_dbm == rf_ptr->cca_threshold->default_dbm) &&
            (high_limit == rf_ptr->cca_threshold->high_limit) &&
            (low_limit == rf_ptr->cca_threshold->low_limit)) {
        return -1;
    }
    // Validate given dBm range. Default must be in between high and low limit.
    if ((default_dbm > high_limit) || (default_dbm < low_limit)) {
        return -1;
    }
    mac_cca_thr_deinit(rf_ptr);
    rf_ptr->cca_threshold = ns_dyn_mem_alloc(sizeof(mac_cca_threshold_s));
    if (!rf_ptr->cca_threshold) {
        return -1;
    }
    rf_ptr->cca_threshold->ch_thresholds = ns_dyn_mem_alloc(number_of_channels);
    if (!rf_ptr->cca_threshold->ch_thresholds) {
        ns_dyn_mem_free(rf_ptr->cca_threshold);
        rf_ptr->cca_threshold = 0;
        return -1;
    }
    memset(rf_ptr->cca_threshold->ch_thresholds, default_dbm, number_of_channels);
    rf_ptr->cca_threshold->high_limit = high_limit;
    rf_ptr->cca_threshold->low_limit = low_limit;
    rf_ptr->cca_threshold->default_dbm = default_dbm;
    rf_ptr->cca_threshold->number_of_channels = number_of_channels;
    tr_info("Initialized CCA threshold: %u, %i, %i, %i", number_of_channels, default_dbm, high_limit, low_limit);
    return 0;
}

int8_t mac_cca_thr_deinit(protocol_interface_rf_mac_setup_s *rf_ptr)
{
    if (!rf_ptr->cca_threshold) {
        return -1;
    }
    ns_dyn_mem_free(rf_ptr->cca_threshold->ch_thresholds);
    ns_dyn_mem_free(rf_ptr->cca_threshold);
    rf_ptr->cca_threshold = 0;
    return 0;
}

int8_t mac_cca_thr_get_dbm(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t channel)
{
    if (!rf_ptr->cca_threshold) {
        return CCA_FAILED_DBM;
    }
    // If channel is unknown, use default threshold
    if (channel > (rf_ptr->cca_threshold->number_of_channels - 1)) {
        return rf_ptr->cca_threshold->default_dbm;
    }
    return rf_ptr->cca_threshold->ch_thresholds[channel];
}

static int8_t mac_cca_thr_set_dbm(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t channel, int8_t dbm)
{
    if (rf_ptr->cca_threshold->ch_thresholds[channel] != dbm) {
        rf_ptr->cca_threshold->ch_thresholds[channel] = dbm;
        return 0;
    }
    return -1;
}

static int8_t mac_cca_thr_update_channel_threshold(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t channel, int8_t dbm)
{
    // Already on low limit
    if (rf_ptr->cca_threshold->ch_thresholds[channel] == rf_ptr->cca_threshold->low_limit) {
        return -1;
    }
    // Already using lower threshold
    if (rf_ptr->cca_threshold->ch_thresholds[channel] <= (dbm - CCA_THRESHOLD_STEP)) {
        return -1;
    }
    // Do not set below configured low limit
    if ((dbm - CCA_THRESHOLD_STEP) < rf_ptr->cca_threshold->low_limit) {
        return mac_cca_thr_set_dbm(rf_ptr, channel, rf_ptr->cca_threshold->low_limit);
    }
    return mac_cca_thr_set_dbm(rf_ptr, channel, dbm - CCA_THRESHOLD_STEP);
}

static int8_t mac_cca_thr_channel_failed(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t channel)
{
    // Already on high limit
    if (rf_ptr->cca_threshold->ch_thresholds[channel] == rf_ptr->cca_threshold->high_limit) {
        return -1;
    }
    // Do not set above configured high limit
    if ((rf_ptr->cca_threshold->ch_thresholds[channel] + CCA_THRESHOLD_STEP) > rf_ptr->cca_threshold->high_limit) {
        return mac_cca_thr_set_dbm(rf_ptr, channel, rf_ptr->cca_threshold->high_limit);
    }
    return mac_cca_thr_set_dbm(rf_ptr, channel, rf_ptr->cca_threshold->ch_thresholds[channel] + CCA_THRESHOLD_STEP);
}

int8_t mac_cca_threshold_update(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t channel, int8_t dbm)
{
    if (!rf_ptr->cca_threshold) {
        return -1;
    }
    if (channel > (rf_ptr->cca_threshold->number_of_channels - 1)) {
        return -1;
    }
    if (dbm == CCA_FAILED_DBM) {
        if (mac_cca_thr_channel_failed(rf_ptr, channel)) {
            return -1;
        }
    } else {
        if (mac_cca_thr_update_channel_threshold(rf_ptr, channel, dbm)) {
            return -1;
        }
    }
    tr_info("Channel %u CCA threshold to %i", channel, rf_ptr->cca_threshold->ch_thresholds[channel]);
    return 0;
}

mac_cca_threshold_s *mac_get_cca_threshold_table(protocol_interface_rf_mac_setup_s *rf_ptr)
{
    if (!rf_ptr->cca_threshold) {
        return NULL;
    }
    return rf_ptr->cca_threshold;
}
