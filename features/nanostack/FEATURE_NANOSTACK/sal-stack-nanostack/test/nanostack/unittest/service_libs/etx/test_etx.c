/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
#include "test_etx.h"
#include <string.h>
#include <inttypes.h>

#include "address.h"
#include "mle.h"
#include "etx.h"

#include "mle_stub.h"
#include "address_stub.h"

#define NEW_ETX_MULTIPLIER ((double) 1 / ((double) (1 << ETX_MOVING_AVERAGE_FRACTION)))
#define PREV_ETX_MULTIPLIER (((double) 1) - NEW_ETX_MULTIPLIER)

#define INT_TO_FLOAT(VAL, PRES) \
    ((double) VAL / (double) (1 << PRES))

#define ETX_CALLBACK_RANGE   ((double) 1 / (double) ETX_MOVING_AVERAGE_FRACTION)

// Add always ETX 1/1
bool test_ETX_etx_transm_attempts_update_1()
{
    uint8_t i;

    uint8_t attempts = 1;
    bool success = 1;
    addrtype_t addr_type = ADDR_802_15_4_LONG;
    uint8_t addr[10] = {0xfa,0xce,0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};

    mle_neigh_table_entry_t mle_neigh_table_entry;
    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;

    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    mle_neigh_table_entry.etx = 1 << 12;

    address_stub.uint8_value = true;

    for (i = 0; i < 50; i++) {
        etx_transm_attempts_update(0,attempts, success, addr_type, addr);

        if (mle_neigh_table_entry.etx != 1<<12) {
            return false;
        }
    }

    return true;
}

// Add always ETX 1/2
bool test_ETX_etx_transm_attempts_update_2()
{
    uint8_t i;
    uint8_t attempts;
    bool success;
    addrtype_t addr_type = ADDR_802_15_4_LONG;
    uint8_t addr[10] = {0xfa,0xce,0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    double etx = 1;
    double output_etx;
    mle_neigh_table_entry_t mle_neigh_table_entry;

    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    mle_neigh_table_entry.etx = 1 << 12;
    address_stub.uint8_value = true;

    // 2/1 ETX
    attempts = 2;
    success = 1;

    for (i = 0; i < 50; i++) {
        etx_transm_attempts_update(0,attempts, success, addr_type, addr);

        etx = PREV_ETX_MULTIPLIER * etx + NEW_ETX_MULTIPLIER * attempts;

        output_etx = INT_TO_FLOAT(mle_neigh_table_entry.etx, 12);

        if (fabs(etx - output_etx) > 0.01) {
            return false;
        }
    }

    return true;
}

// Add (rand 1 to 5)/1 ETX
bool test_ETX_etx_transm_attempts_update_3()
{
    uint8_t i;
    uint8_t attempts;
    bool success;
    addrtype_t addr_type = ADDR_802_15_4_LONG;
    uint8_t addr[10] = {0xfa,0xce,0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    double etx = 1;
    double output_etx;
    mle_neigh_table_entry_t mle_neigh_table_entry;

    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    mle_neigh_table_entry.etx = 1 << 12;
    address_stub.uint8_value = true;

    // (rand 1 to 5)/1 ETX
    attempts = 1;
    success = 1;

    srand(time(NULL));

    for (i = 0; i < 50; i++) {

        attempts = (rand() % 5) + 1;

        etx_transm_attempts_update(0,attempts, success, addr_type, addr);

        etx = PREV_ETX_MULTIPLIER * etx + NEW_ETX_MULTIPLIER * attempts;

        output_etx = INT_TO_FLOAT(mle_neigh_table_entry.etx, 12);

        if (fabs(etx - output_etx) > 0.01) {
            return false;
        }
    }
    return true;
}

// Add (rand 1 to 5)/0 ETX
bool test_ETX_etx_transm_attempts_update_4()
{
    uint16_t i;
    uint16_t attempts;
    uint16_t attempts_cumul;
    bool success;
    addrtype_t addr_type = ADDR_802_15_4_LONG;
    uint8_t addr[10] = {0xfa,0xce,0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    double etx = 1;
    double acc_etx = 1;
    double output_etx;
    uint16_t output_etx_int;
    mle_neigh_table_entry_t mle_neigh_table_entry;

    mle_stub.bool_value = true;
    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    mle_neigh_table_entry.etx = 1 << 12;
    address_stub.uint8_value = true;

    // (rand 1 to 5)/1 ETX
    attempts = 1;
    attempts_cumul = 0;
    success = 1;

    srand(time(NULL));

    for (i = 0; i < 50; i++) {

        attempts = (rand() % 5) + 1;
        attempts_cumul += attempts;

        etx_transm_attempts_update(0,attempts, 0, addr_type, addr);

        if (attempts_cumul > 31) {
            etx = PREV_ETX_MULTIPLIER * etx + NEW_ETX_MULTIPLIER * attempts_cumul;
            acc_etx = etx;
            attempts_cumul = 0;
        } else {
            acc_etx = etx + NEW_ETX_MULTIPLIER * attempts_cumul;
        }

        output_etx_int = etx_read(0, addr_type, addr);

        output_etx = INT_TO_FLOAT(output_etx_int, 8);

        if (acc_etx < 15.99) {
            if (fabs(acc_etx - output_etx) > 0.01) {
                return false;
            }
        }
    }
    return true;
}

// Add 5/0 ETX
bool test_ETX_etx_transm_attempts_update_5()
{
    uint16_t i;
    uint16_t attempts;
    uint16_t attempts_cumul;
    bool success;
    addrtype_t addr_type = ADDR_802_15_4_LONG;
    uint8_t addr[10] = {0xfa,0xce,0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    double etx = 1;
    double acc_etx = 1;
    double output_etx;
    double output_idr;
    double output_idr_etx;
    uint16_t output_idr_int;
    uint16_t output_etx_int;
    mle_neigh_table_entry_t mle_neigh_table_entry;

    mle_stub.bool_value = true;
    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    mle_neigh_table_entry.etx = 1 << 12;
    address_stub.uint8_value = true;

    attempts_cumul = 0;

    for (i = 0; i < 50; i++) {

        attempts = 5;
        attempts_cumul += attempts;

        etx_transm_attempts_update(0,attempts, 0, addr_type, addr);

        if (attempts_cumul > 31) {
            etx = PREV_ETX_MULTIPLIER * etx + NEW_ETX_MULTIPLIER * attempts_cumul;
            acc_etx = etx;
            attempts_cumul = 0;
        } else {
            acc_etx = etx + NEW_ETX_MULTIPLIER * attempts_cumul;
        }

        output_etx_int = etx_read(0, addr_type, addr);

        output_etx = INT_TO_FLOAT(output_etx_int, 8);

        output_idr_int = etx_local_incoming_idr_read(0,&mle_neigh_table_entry);

        output_idr = INT_TO_FLOAT(output_idr_int, 8);

        output_idr_etx = output_idr * output_idr;

        if (acc_etx < 15.99) {
            if (fabs(acc_etx - output_etx) > 0.01) {
                return false;
            }

            if (fabs(acc_etx - output_idr_etx) > 0.03) {
                return false;
            }
        }
    }
    return true;
}

bool test_ETX_etx_read_1()
{
    uint8_t i;
    uint8_t attempts;
    bool success;
    addrtype_t addr_type = ADDR_802_15_4_LONG;
    uint8_t addr[10] = {0xfa,0xce,0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    double etx = 1;
    double local_etx;
    double remote_incoming_idr;
    uint16_t output_etx_int;
    double output_etx;
    double expected_output_etx;
    mle_neigh_table_entry_t mle_neigh_table_entry;

    mle_stub.bool_value = true;
    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    mle_neigh_table_entry.etx = 1 << 12;
    mle_neigh_table_entry.remote_incoming_idr = 0x30;
    address_stub.uint8_value = true;

    // 2/1 ETX
    attempts = 2;
    success = 1;

    for (i = 0; i < 50; i++) {
        etx_transm_attempts_update(0,attempts, success, addr_type, addr);

        output_etx_int = etx_read(0, addr_type, addr);

        output_etx = INT_TO_FLOAT(output_etx_int, 8);

        local_etx = INT_TO_FLOAT(mle_neigh_table_entry.etx, 12);

        if (fabs(local_etx  - output_etx) > 0.01) {
            return false;
        }
    }

    return true;
}

bool test_ETX_etx_read_2()
{
    addrtype_t addr_type = ADDR_802_15_4_LONG;
    uint8_t addr[8] = {0xfa,0xce,0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    uint16_t output_etx_int;

    mle_stub.bool_value = true;
    mle_stub.mle_neigh_table_entry_ptr = NULL;

    output_etx_int = etx_read(0, addr_type, addr);

    if (output_etx_int == 0xffff) {
       return true;
    } else {
       return false;
    }
}

bool test_ETX_etx_read_3()
{
    addrtype_t addr_type = ADDR_802_15_4_LONG;
    uint8_t addr[8] = {0xfa,0xce,0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    uint16_t output_etx_int;

    mle_stub.bool_value = false;
    mle_stub.mle_neigh_table_entry_ptr = 0x1234;

    output_etx_int = etx_read(0, addr_type, addr);

    if (output_etx_int == 0x0001) {
       return true;
    } else {
       return false;
    }
}

bool test_ETX_etx_remote_incoming_idr_update_1()
{
    uint8_t i;
    uint8_t addr[8] = {0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    double expected_etx = 1;
    double output_etx;
    double expected_remote_incoming_idr;
    uint8_t remote_incoming_idr;
    mle_neigh_table_entry_t mle_neigh_table_entry;

    mle_stub.bool_value = true;
    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    mle_neigh_table_entry.etx = 1 << 12;
    address_stub.uint8_value = true;

    mle_neigh_table_entry.etx = 1 << 12;

    for (i = 0; i < 20; i++) {

        remote_incoming_idr = 48; // 1.5

        etx_remote_incoming_idr_update(0, remote_incoming_idr, &mle_neigh_table_entry);

        expected_remote_incoming_idr = INT_TO_FLOAT(remote_incoming_idr, 5);

        expected_etx = PREV_ETX_MULTIPLIER * expected_etx + NEW_ETX_MULTIPLIER * (expected_remote_incoming_idr * expected_remote_incoming_idr);

        output_etx = INT_TO_FLOAT(mle_neigh_table_entry.etx, 12);

        if (fabs(expected_etx - output_etx) > 0.01) {
            return false;
        }
    }

    return true;
}

bool test_ETX_etx_local_incoming_idr_read_1()
{
    uint8_t i;
    uint8_t addr[8] = {0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    mle_neigh_table_entry_t mle_neigh_table_entry;

    mle_stub.bool_value = true;
    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    address_stub.uint8_value = true;

    double output_remote_incoming_idr;
    double expected_etx;
    uint16_t output_remote_incoming_idr_int;
    double expected_remote_incoming_idr;

    mle_neigh_table_entry.etx = 1 << 12;

    for (i = 0; i < 100; i++) {

        output_remote_incoming_idr_int = etx_local_incoming_idr_read(0,&mle_neigh_table_entry);

        output_remote_incoming_idr = INT_TO_FLOAT(output_remote_incoming_idr_int, 8);

        expected_etx = INT_TO_FLOAT(mle_neigh_table_entry.etx, 12);

        expected_remote_incoming_idr = sqrt(expected_etx);

        if (fabs(expected_remote_incoming_idr - output_remote_incoming_idr) > 0.01) {
            return false;
        }

        mle_neigh_table_entry.etx += 1000;
    }

    return true;
}

bool test_ETX_etx_local_incoming_idr_read_2()
{
    uint8_t i;
    addrtype_t addr_type = ADDR_802_15_4_LONG;
    uint8_t addr[10] = {0xfa,0xce,0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    mle_neigh_table_entry_t mle_neigh_table_entry;

    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    address_stub.uint8_value = true;

    double output_remote_incoming_idr;
    double expected_etx;
    uint16_t output_remote_incoming_idr_int;
    double expected_remote_incoming_idr;

    mle_neigh_table_entry.etx = 1 << 12;

    for (i = 0; i < 10; i++) {
        output_remote_incoming_idr_int = etx_local_incoming_idr_read(0, &mle_neigh_table_entry);

        output_remote_incoming_idr = INT_TO_FLOAT(output_remote_incoming_idr_int, 8);

        expected_etx = INT_TO_FLOAT(mle_neigh_table_entry.etx, 12);
        expected_etx += NEW_ETX_MULTIPLIER * (double) mle_neigh_table_entry.accumulated_failures;

        expected_remote_incoming_idr = sqrt(expected_etx);

        if (output_remote_incoming_idr < 3.99) {
            if (fabs(expected_remote_incoming_idr - output_remote_incoming_idr) > 0.01) {
                return false;
            }
        } else if (output_remote_incoming_idr < 1 || output_remote_incoming_idr > 4.0) {
            return false;
        }
        etx_transm_attempts_update(0,5, 0, addr_type, addr);
    }

    etx_transm_attempts_update(0,5, 1, addr_type, addr);

    return true;
}

bool test_ETX_etx_lqi_dbm_update_1()
{
    uint8_t i;
    uint8_t addr[8] = {0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    mle_neigh_table_entry_t mle_neigh_table_entry;

    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    address_stub.uint8_value = true;

    double output_remote_incoming_idr;
    double expected_etx;
    uint16_t output_remote_incoming_idr_int;
    double expected_remote_incoming_idr;

    uint8_t lqi = 255;
    int8_t dbm = -30;

    for (i = 0; i < 10; i++) {
        output_remote_incoming_idr_int = etx_lqi_dbm_update(0,lqi, dbm, &mle_neigh_table_entry);

        output_remote_incoming_idr = INT_TO_FLOAT(output_remote_incoming_idr_int, 8);

        if (output_remote_incoming_idr < 1 || output_remote_incoming_idr > 1.2) {
            return false;
        }

        mle_neigh_table_entry.remote_incoming_idr = 1<<5;
    }

    return true;
}

bool test_ETX_etx_lqi_dbm_update_2()
{
    uint8_t i;
    uint8_t addr[8] = {0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};

    mle_stub.mle_neigh_table_entry_ptr = NULL;
    address_stub.uint8_value = true;

    double output_remote_incoming_idr;
    double expected_etx;
    uint16_t output_remote_incoming_idr_int;
    double expected_remote_incoming_idr;

    uint8_t lqi = 255;
    int8_t dbm = -30;

    for (i = 0; i < 10; i++) {
        output_remote_incoming_idr_int = etx_lqi_dbm_update(0,lqi, dbm, NULL);

        output_remote_incoming_idr = INT_TO_FLOAT(output_remote_incoming_idr_int, 8);

        if (output_remote_incoming_idr < 1 || output_remote_incoming_idr > 1.2) {
            return false;
        }
    }

    return true;
}

#define CHANGE_IND_HANDLER_INDEX_MAX 10

typedef struct
{
    nwk_interface_id nwk_id;
    uint16_t previous_etx;
    uint16_t current_etx;
    uint8_t *mac64_addr_ptr;
    uint8_t mac16_addr;
    uint8_t accumulated_failures;
} change_ind_handler_values_t;

change_ind_handler_values_t change_ind_handler_values[CHANGE_IND_HANDLER_INDEX_MAX];

uint8_t change_ind_handler_index = 0;

void etx_value_change_ind_handler(nwk_interface_id nwk_id, uint16_t previous_etx, uint16_t current_etx, const uint8_t *mac64_addr_ptr, uint16_t mac16_addr)
{
    if (change_ind_handler_index < 10) {

    change_ind_handler_values[change_ind_handler_index].nwk_id = nwk_id;
    change_ind_handler_values[change_ind_handler_index].previous_etx = previous_etx;
    change_ind_handler_values[change_ind_handler_index].current_etx = current_etx;
    change_ind_handler_values[change_ind_handler_index].mac64_addr_ptr = mac64_addr_ptr;
    change_ind_handler_values[change_ind_handler_index].mac16_addr = mac16_addr;

    change_ind_handler_index++;
    }
}

void etx_accum_failures_ind_handler(int8_t interface_id, uint8_t accumulated_failures, struct mle_neigh_table_entry_t *neigh_table_ptr)
{
    if (change_ind_handler_index < 10) {
        change_ind_handler_values[change_ind_handler_index].accumulated_failures = accumulated_failures;
        change_ind_handler_index++;
    }
}

bool test_ETX_etx_value_change_callback_register_1()
{
    uint8_t i;
    uint8_t attempts;
    bool success;
    addrtype_t addr_type = ADDR_802_15_4_LONG;
    uint8_t addr[10] = {0xfa,0xce,0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    double etx = 1;
    double output_etx;
    double current_etx;
    mle_neigh_table_entry_t mle_neigh_table_entry;

    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    mle_neigh_table_entry.etx = 1 << 12;
    address_stub.uint8_value = true;

    memset(change_ind_handler_values, 0, sizeof(change_ind_handler_values));
    change_ind_handler_index = 0;

    // 3/1 ETX
    attempts = 3;
    success = 1;

    (void) etx_value_change_callback_register(IF_6LoWPAN, 0, 128, etx_value_change_ind_handler);

    for (i = 0; i < 200; i++) {
        etx_transm_attempts_update(0,attempts, success, addr_type, addr);

        etx = PREV_ETX_MULTIPLIER * etx + NEW_ETX_MULTIPLIER * attempts;

        output_etx = INT_TO_FLOAT(mle_neigh_table_entry.etx, 12);

        if (fabs(etx - output_etx) > 0.1) {
            return false;
        }
    }

    // Checks callbacks
    for (i = 0; i < 10; i++) {
        if (change_ind_handler_values[i].current_etx) {
            current_etx = INT_TO_FLOAT(change_ind_handler_values[i].current_etx, 8);

            etx = 1.5 + i * 0.5;

            if (fabs(etx - current_etx) > ETX_CALLBACK_RANGE) {
                return false;
            }
        }
    }

    return true;
}

bool test_ETX_etx_accum_failures_callback_register_1()
{
    uint8_t i, j;
    uint8_t attempts;
    bool success;
    addrtype_t addr_type = ADDR_802_15_4_LONG;
    uint8_t addr[10] = {0xfa,0xce,0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    mle_neigh_table_entry_t mle_neigh_table_entry;

    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    address_stub.uint8_value = true;

    memset(change_ind_handler_values, 0, sizeof(change_ind_handler_values));
    change_ind_handler_index = 0;

    attempts = 4;
    success = 0;

    (void) etx_accum_failures_callback_register(IF_6LoWPAN, 0, 16, etx_accum_failures_ind_handler);

    for (i = 0; i < 10; ++i) {
        etx_transm_attempts_update(0, attempts, success, addr_type, addr);
    }

    // Check callbacks (total of 4 calls)
    if (change_ind_handler_index != 4) {
        return false;
    }

    for (i = 0, j = 16; i < change_ind_handler_index; ++i, j += 4) {
        if (change_ind_handler_values[i].accumulated_failures != j) {
            return false;
        }
    }

    return true;
}

bool test_ETX_etx_neighbor_remove_1()
{
    uint8_t i;
    uint8_t attempts;
    bool success;
    uint8_t addr[8] = {0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    double etx = 1;
    double output_etx;
    mle_neigh_table_entry_t mle_neigh_table_entry;

    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    mle_neigh_table_entry.etx = 1 << 12;
    address_stub.uint8_value = true;

    memset(change_ind_handler_values, 0, sizeof(change_ind_handler_values));
    change_ind_handler_index = 0;

    // 3/1 ETX
    attempts = 3;
    success = 1;

    (void) etx_value_change_callback_register(IF_6LoWPAN, 0, 128, etx_value_change_ind_handler);

    etx_neighbor_remove(0, &mle_neigh_table_entry);

    if ((change_ind_handler_values[0].current_etx != 0xffff) &&
        (change_ind_handler_values[0].previous_etx != 0x0100)) {
        return false;
        }

    return true;
}

bool test_ETX_etx_neighbor_add_1()
{
    uint8_t i;
    uint8_t attempts;
    bool success;
    uint8_t addr[8] = {0x14,0x6e,0x0a,0x00,0x00,0x00,0x00,0x01};
    double etx = 1;
    double output_etx;
    mle_neigh_table_entry_t mle_neigh_table_entry;

    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    mle_neigh_table_entry.etx = 1 << 12;
    address_stub.uint8_value = true;

    memset(change_ind_handler_values, 0, sizeof(change_ind_handler_values));
    change_ind_handler_index = 0;

    // 3/1 ETX
    attempts = 3;
    success = 1;

    (void) etx_value_change_callback_register(IF_6LoWPAN, 0, 128, etx_value_change_ind_handler);

    etx_neighbor_add(0,&mle_neigh_table_entry);

    if ((change_ind_handler_values[0].current_etx != 0x0100) ||
        (change_ind_handler_values[0].previous_etx != 0x0100)) {
        return false;
        }

    return true;
}
