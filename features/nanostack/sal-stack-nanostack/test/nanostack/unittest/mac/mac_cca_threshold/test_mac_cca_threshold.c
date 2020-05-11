/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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
#include "test_mac_cca_threshold.h"
#include "mac_defines.h"
#include "nsdynmemLIB_stub.h"
#include "MAC/IEEE802_15_4/mac_cca_threshold.h"

static protocol_interface_rf_mac_setup_s setup;

static void test_free()
{
    if (setup.cca_threshold && setup.cca_threshold->ch_thresholds) {
        free(setup.cca_threshold->ch_thresholds);
    }
    if (setup.cca_threshold) {
        free(setup.cca_threshold);
    }
}

static void test_initialize(void)
{
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
}

bool test_mac_cca_threshold_update()
{
    uint8_t number_of_channels = 64;
    int8_t default_dbm = -60;
    int8_t high_limit = -30;
    int8_t low_limit = -120;
    test_initialize();
    // Initialize CCA threshold
    nsdynmemlib_stub.returnCounter = 2;
    mac_cca_thr_init(&setup, number_of_channels, default_dbm, high_limit, low_limit);
    // Test calling init again
    nsdynmemlib_stub.returnCounter = 2;
    mac_cca_thr_init(&setup, number_of_channels, default_dbm, high_limit, low_limit);
    // Test all channels set to default dBm
    for (int i = 0; i < number_of_channels; i++) {
        if (mac_cca_thr_get_dbm(&setup, i) != default_dbm) {
            return false;
        }
    }
    // Test several CCA fails
    for (int i = 0; i < 5; i++) {
        mac_cca_threshold_update(&setup, 10, 127);
    }
    if (mac_cca_thr_get_dbm(&setup, 10) != default_dbm + (5 * CCA_THRESHOLD_STEP)) {
        return false;
    }
    // Test setting to low limit
    mac_cca_threshold_update(&setup, 10, -125);
    if (mac_cca_thr_get_dbm(&setup, 10) != low_limit) {
        return false;
    }
    mac_cca_threshold_update(&setup, 10, 127);
    if (mac_cca_thr_get_dbm(&setup, 10) != (low_limit + CCA_THRESHOLD_STEP)) {
        return false;
    }
    // Test receiving same dBm as low limit
    mac_cca_threshold_update(&setup, 10, -120);
    if (mac_cca_thr_get_dbm(&setup, 10) != low_limit) {
        return false;
    }
    // Test receiving step above low limit
    mac_cca_threshold_update(&setup, 10, -120 + CCA_THRESHOLD_STEP);
    if (mac_cca_thr_get_dbm(&setup, 10) != low_limit) {
        return false;
    }
    test_free();
    return true;
}


bool test_mac_cca_thr_channel_failed()
{
    uint8_t number_of_channels = 35;
    int8_t default_dbm = -31;
    int8_t high_limit = -30;
    int8_t low_limit = -100;
    test_initialize();
    // Initialize CCA threshold
    nsdynmemlib_stub.returnCounter = 2;
    mac_cca_thr_init(&setup, number_of_channels, default_dbm, high_limit, low_limit);
    // Test that default dBm is set
    if (setup.cca_threshold->ch_thresholds[10] != default_dbm) {
        return false;
    }
    // Test setting above high limit
    if (mac_cca_threshold_update(&setup, 10, 127) || setup.cca_threshold->ch_thresholds[10] != high_limit) {
        return false;
    }
    // Test setting above high limit again
    if (!mac_cca_threshold_update(&setup, 10, 127) || setup.cca_threshold->ch_thresholds[10] != high_limit) {
        return false;
    }
    //Set threshold to -65 - CCA_THRESHOLD_STEP
    mac_cca_threshold_update(&setup, 10, -65);
    // Test setting one step high with CCA fail
    if (mac_cca_threshold_update(&setup, 10, 127) || setup.cca_threshold->ch_thresholds[10] != -65) {
        return false;
    }
    test_free();
    return true;
}

bool test_mac_cca_thr_update_channel_threshold()
{
    uint8_t number_of_channels = 35;
    int8_t default_dbm = -50;
    int8_t high_limit = -30;
    int8_t low_limit = -100;
    test_initialize();
    // Initialize CCA threshold
    nsdynmemlib_stub.returnCounter = 2;
    mac_cca_thr_init(&setup, number_of_channels, default_dbm, high_limit, low_limit);
    // Test that default dBm is set
    if (setup.cca_threshold->ch_thresholds[10] != default_dbm) {
        return false;
    }
    // Test setting new threshold
    if (mac_cca_threshold_update(&setup, 10, -70) || setup.cca_threshold->ch_thresholds[10] != (-70 - CCA_THRESHOLD_STEP)) {
        return false;
    }
    // Test if packet received with higher dBm
    if (!mac_cca_threshold_update(&setup, 10, -65) || setup.cca_threshold->ch_thresholds[10] != (-70 - CCA_THRESHOLD_STEP)) {
        return false;
    }
    // Test setting to low limit
    if (mac_cca_threshold_update(&setup, 10, -101) || setup.cca_threshold->ch_thresholds[10] != low_limit) {
        return false;
    }
    // Test received lower RSSI than low limit
    if (!mac_cca_threshold_update(&setup, 10, -101) || setup.cca_threshold->ch_thresholds[10] != low_limit) {
        return false;
    }
    test_free();
    return true;
}

bool test_mac_cca_thr_init()
{
    test_initialize();
    // Test default dBm above high limit
    if (!mac_cca_thr_init(&setup, 35, -10, -11, -110)) {
        return false;
    }
    // Test default dBm below low limit
    if (!mac_cca_thr_init(&setup, 35, -111, -11, -110)) {
        return false;
    }
    // Test high limit below low limit
    if (!mac_cca_thr_init(&setup, 35, -50, -110, -11)) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 0;
    // Test allocation failed
    if (!mac_cca_thr_init(&setup, 35, -50, -30, -100)) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    // Test second allocation failed
    if (!mac_cca_thr_init(&setup, 35, -50, -30, -100)) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 2;
    // Test success
    if (mac_cca_thr_init(&setup, 35, -50, -30, -100)) {
        return false;
    }
    test_free();
    return true;
}
