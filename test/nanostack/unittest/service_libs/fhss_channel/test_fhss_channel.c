/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "fhss_api.h"
#include "fhss_config.h"
#include "Service_Libs/fhss/fhss.h"
#include "MAC/IEEE802_15_4/mac_fhss_callbacks.h"
#include "test_fhss_channel.h"
#include "nsdynmemLIB_stub.h"
#include "channel_list_stub.h"
#include "fhss_callbacks_stub.h"
#include "fhss_stub.h"

static uint16_t rand_value = 0;
static int rand_call_cnt = 0;

uint16_t randLIB_get_random_in_range(uint16_t min, uint16_t max)
{
    if (rand_call_cnt % 2){
        rand_value++;
    }
    rand_call_cnt++;
    return rand_value;
}

static int print_channel_table(uint16_t number_of_channels, uint16_t number_of_bc, uint8_t *channel_table)
{
    uint16_t bc_density = number_of_channels / number_of_bc;
    if (!channel_table) {
        return -1;
    }
    for (int i = 0; i < number_of_channels; i++) {
        if (i && !(i%bc_density)) {
            printf("\n");
        }
        printf("%u ",channel_table[i]);
        if (channel_table[i] < 10) {
            printf(" ");
        }
    }
    printf("\n");
    return 0;
}

static int generate_channel_sequence(uint16_t number_of_channels, uint16_t number_of_bc, uint8_t *channel_table)
{
    uint16_t bc_density = number_of_channels / number_of_bc;

    if (!channel_table) {
        return -1;
    }
    for (int i=0; i<number_of_channels; i++) {
        if (!(i % bc_density)) {
            channel_table[i] = number_of_channels - (number_of_bc - (i/bc_density));
        } else {
            channel_table[i] = (i - (i/bc_density)) - 1;
        }
    }
    return 0;
}

static uint32_t search_repeated_sequence(uint16_t repeat_pattern, uint8_t *table, uint32_t table_size)
{
    uint8_t *search_index = table + 1;
    uint32_t count = 1;

    while (search_index < (table + table_size)) {
        while (*table != *search_index && (search_index < (table + table_size))) {
            search_index++;
            count++;
        }
        if (search_index < (table + (table_size))) {
            uint16_t i;
            for (i = 0; i < repeat_pattern; i++) {
                if (*(table + i) != *(search_index + i) || (search_index >= (table + table_size))) {
                    break;
                }
            }
            if (i == repeat_pattern) {
                return count;
            }
            search_index++;
        }
    }
    return 0;
}

bool test_channel_randomization()
{
    uint16_t number_of_channels = 0;
    uint16_t number_of_bc = 0;

    // Runs all allowed channel list - broadcast list combinations
    for (number_of_channels = 1; number_of_channels < 257; number_of_channels++) {
        for (number_of_bc = 1; number_of_bc <= number_of_channels && number_of_bc < 256; number_of_bc++) {
            // number of channels must be divisible with number of broadcast channels
            if (!(number_of_channels % number_of_bc)) {
                uint8_t *channel_table = malloc(number_of_channels);
                if (!channel_table) {
                    return false;
                }
                uint8_t *randomized_table = malloc(number_of_channels);
                if (!randomized_table) {
                    return false;
                }
                if (generate_channel_sequence(number_of_channels, number_of_bc, channel_table)) {
                    return false;
                }
                for (int i = 0; i < number_of_channels; i++) {
                    randomized_table[i] = fhss_calc_channel_shuffle(channel_table[i], number_of_channels, number_of_bc);
                }
#if 0
                printf("Number of channels: %u, ", number_of_channels);
                printf("Number of BC: %u\n", number_of_bc);
                printf("Channel table:\n");
                print_channel_table(number_of_channels, number_of_bc, channel_table);
                printf("\n");
                printf("Randomized table:\n");
                print_channel_table(number_of_channels, number_of_bc, randomized_table);
                printf("\n");
#endif
                // Test that all channels are existing only once
                int channel_exists = 0;
                for (int j = 0; j < number_of_channels; j++) {
                    for (int k = 0; k < number_of_channels; k++) {
                        if (randomized_table[k] == j) {
                            channel_exists++;
                        }
                    }
                    if (channel_exists != 1) {
                        return false;
                    }
                    channel_exists = 0;
                }
                free(channel_table);
                free(randomized_table);
            }
        }
    }
    return true;
}

const uint8_t scramble_table_50[20] = {3, 7, 9, 11, 13, 17, 19, 21, 23, 27, 29, 31, 33, 37, 39, 41, 43, 47, 49, 51};
const uint8_t scramble_table_64[20] = {3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41};
uint8_t scramble_table_test[20];

bool test_channel_list_repeating_50_channels()
{
    uint16_t number_of_channels = 50;
    uint16_t number_of_bc = 10;
    // With 50 channels, repeating cycle should start after 50000 indexes
    uint32_t number_of_test_indexes = number_of_channels * number_of_channels * MAX_SCRAMBLE_TABLE_INDEXES;
    uint16_t channel_list_count = number_of_test_indexes / number_of_channels;

    uint8_t *channel_table = malloc(number_of_channels);
    if (!channel_table) {
        return false;
    }
    uint8_t *randomized_table = malloc(number_of_channels);
    if (!randomized_table) {
        return false;
    }
    uint8_t *final_table = malloc(number_of_channels * channel_list_count);
    if (!final_table) {
        return false;
    }

    if (generate_channel_sequence(number_of_channels, number_of_bc, channel_table)) {
        return false;
    }
    for (int i = 0; i < number_of_channels; i++) {
        randomized_table[i] = fhss_calc_channel_shuffle(channel_table[i], number_of_channels, number_of_bc);
    }
    int k = 0;
    uint16_t channel_list_counter = 0;
    // Create scramble table
    memcpy(scramble_table_test, scramble_table_50, 20);
    // Create first 50000 indexes
    for (int j = 0; j < number_of_channels * channel_list_count; j++) {
        final_table[j] = fhss_add_channel_list_counter(randomized_table[k], number_of_channels, channel_list_counter, scramble_table_test);
        if (++k == number_of_channels) {
            k = 0;
            channel_list_counter++;
        }
    }
    // Test if repeated cycle is found
    if (search_repeated_sequence(number_of_channels * 2, final_table, number_of_test_indexes)) {
        return false;
    }

    free(channel_table);
    free(randomized_table);
    free(final_table);
    return true;
}

bool test_channel_list_repeating_64_channels()
{
    uint16_t number_of_channels = 64;
    uint16_t number_of_bc = 8;
    // With 50 channels, repeating cycle should start after 81920 indexes
    uint32_t number_of_test_indexes = number_of_channels * number_of_channels * MAX_SCRAMBLE_TABLE_INDEXES;
    uint16_t channel_list_count = number_of_test_indexes / number_of_channels;

    uint8_t *channel_table = malloc(number_of_channels);
    if (!channel_table) {
        return false;
    }
    uint8_t *randomized_table = malloc(number_of_channels);
    if (!randomized_table) {
        return false;
    }
    uint8_t *final_table = malloc(number_of_channels * channel_list_count);
    if (!final_table) {
        return false;
    }

    if (generate_channel_sequence(number_of_channels, number_of_bc, channel_table)) {
        return false;
    }
    for (int i = 0; i < number_of_channels; i++) {
        randomized_table[i] = fhss_calc_channel_shuffle(channel_table[i], number_of_channels, number_of_bc);
    }
    int k = 0;
    uint16_t channel_list_counter = 0;
    // Create scramble table
    memcpy(scramble_table_test, scramble_table_64, 20);
    // Create first 81920 indexes
    for (int j = 0; j < number_of_channels * channel_list_count; j++) {
        final_table[j] = fhss_add_channel_list_counter(randomized_table[k], number_of_channels, channel_list_counter, scramble_table_test);
        if (++k == number_of_channels) {
            k = 0;
            channel_list_counter++;
        }
    }
    // Test if repeated cycle is found
    if (search_repeated_sequence(number_of_channels * 2, final_table, number_of_test_indexes)) {
        return false;
    }

    free(channel_table);
    free(randomized_table);
    free(final_table);
    return true;
}

bool test_channels_used_equally()
{
    uint16_t number_of_channels = 50;
    uint16_t number_of_bc = 10;
    uint32_t number_of_test_indexes = number_of_channels * number_of_channels * MAX_SCRAMBLE_TABLE_INDEXES;
    uint16_t channel_list_count = number_of_test_indexes / number_of_channels;
    uint16_t channel_count[50];

    uint8_t *channel_table = malloc(number_of_channels);
    if (!channel_table) {
        return false;
    }
    uint8_t *randomized_table = malloc(number_of_channels);
    if (!randomized_table) {
        return false;
    }
    uint8_t *final_table = malloc(number_of_channels * channel_list_count);
    if (!final_table) {
        return false;
    }

    if (generate_channel_sequence(number_of_channels, number_of_bc, channel_table)) {
        return false;
    }
    for (int i = 0; i < number_of_channels; i++) {
        randomized_table[i] = fhss_calc_channel_shuffle(channel_table[i], number_of_channels, number_of_bc);
    }
    int k = 0;
    uint16_t channel_list_counter = 0;
    // Create scramble table
    memcpy(scramble_table_test, scramble_table_64, 20);
    // Create first 50000 indexes
    for (int j = 0; j < number_of_channels * channel_list_count; j++) {
        final_table[j] = fhss_add_channel_list_counter(randomized_table[k], number_of_channels, channel_list_counter, scramble_table_test);
        if (++k == number_of_channels) {
            k = 0;
            channel_list_counter++;
        }
    }
    memset(channel_count, 0, sizeof(channel_count));
    for (uint32_t l=0; l<number_of_test_indexes; l++) {
        channel_count[final_table[l]]++;
    }
    // Every channel should be used number_of_test_indexes/number_of_channels times
    for (int m=0; m<number_of_channels; m++) {
        if (channel_count[m] != (number_of_test_indexes/number_of_channels)) {
            return false;
        }
    }

    free(channel_table);
    free(randomized_table);
    free(final_table);
    return true;
}

bool test_fhss_is_current_channel_broadcast()
{
    fhss_structure_t fhss_struct;
    // should return broadcast when FHSS not created
    if (fhss_is_current_channel_broadcast(NULL) == false) {
        return false;
    }
    // Test zero divider
    fhss_struct.synch_configuration.fhss_number_of_bc_channels = 0;
    if (fhss_is_current_channel_broadcast(&fhss_struct) == false) {
        return false;
    }
    fhss_struct.fhss_state = FHSS_SYNCHRONIZED;
    fhss_struct.number_of_channels = 50;
    fhss_struct.synch_configuration.fhss_number_of_bc_channels = 10;
    // With above configuration, every fifth channel should be broadcast channel
    for (fhss_struct.current_channel_index = 0; fhss_struct.current_channel_index < fhss_struct.number_of_channels; fhss_struct.current_channel_index++) {
        if (fhss_struct.current_channel_index % (fhss_struct.number_of_channels / fhss_struct.synch_configuration.fhss_number_of_bc_channels)) {
            if (fhss_is_current_channel_broadcast(&fhss_struct) == true) {
                return false;
            }
        } else {
           if (fhss_is_current_channel_broadcast(&fhss_struct) == false) {
               return false;
           }
       }
    }

    fhss_struct.number_of_channels = 50;
    fhss_struct.synch_configuration.fhss_number_of_bc_channels = 50;
    // With above configuration, every channel should be broadcast channel
    for (fhss_struct.current_channel_index = 0; fhss_struct.current_channel_index < fhss_struct.number_of_channels; fhss_struct.current_channel_index++) {
        if (fhss_is_current_channel_broadcast(&fhss_struct) == false) {
            return false;
        }
    }
    return true;
}

bool test_fhss_update_channel()
{
    fhss_structure_t fhss_struct;
    fhss_struct.number_of_channels = 50;
    fhss_struct.synch_configuration.fhss_number_of_bc_channels = 10;
    fhss_struct.uc_channel_index = fhss_struct.number_of_channels - fhss_struct.synch_configuration.fhss_number_of_bc_channels;
    fhss_struct.callbacks.change_channel = &mac_set_channel;
    fhss_struct.callbacks.send_fhss_frame = &mac_fhss_frame_tx;
    fhss_struct.callbacks.read_tx_queue_size = &mac_read_tx_queue_sizes;
    fhss_struct.fhss_state = FHSS_SYNCHRONIZED;
    fhss_callbacks_stub.uint8_value = 3;
    // Test broadcast channel
    fhss_struct.current_channel_index = 0;
    if (fhss_change_to_next_channel(&fhss_struct) == false) {
        return false;
    }
    // Test unicast channel
    fhss_struct.current_channel_index = 1;
    if (fhss_change_to_next_channel(&fhss_struct) == true) {
        return false;
    }

    fhss_callbacks_stub.uint8_value = 9;
    fhss_struct.current_channel_index = 0;
    if (fhss_change_to_next_channel(&fhss_struct) == false) {
        return false;
    }
    return true;
}

bool test_fhss_change_to_tx_channel()
{
    uint8_t destination_address[8] = {0, 0, 0, 0, 0, 0, 0 , 1};
    fhss_structure_t fhss_struct;
    fhss_struct.number_of_channels = 50;
    fhss_struct.synch_configuration.fhss_number_of_bc_channels = 10;
    fhss_struct.rx_channel = 10;
    fhss_struct.fhss_state = FHSS_SYNCHRONIZED;
    memcpy(fhss_struct.fhss_scramble_table, scramble_table_50, 20);
    fhss_struct.callbacks.change_channel = &mac_set_channel;
    // Test broadcast channel
    fhss_struct.current_channel_index = 0;
    if (fhss_change_to_tx_channel(&fhss_struct, &destination_address)) {
        return false;
    }
    // On broadcast channel destination channel should same as rx_channel
    if (fhss_callbacks_stub.uint8_value != 10) {
        return false;
    }
    // Test unicast channel
    fhss_struct.current_channel_index = 1;
    channel_list_stub.uint8_value = 20;
    if (fhss_change_to_tx_channel(&fhss_struct, &destination_address)) {
        return false;
    }
    // On unicast channel destination channel should come from channel list stub
    if (fhss_callbacks_stub.uint8_value != 20) {
        return false;
    }
    return true;
}

bool test_fhss_change_to_parent_channel()
{
    fhss_structure_t fhss_struct;
    fhss_struct.number_of_channels = 50;
    fhss_struct.synch_configuration.fhss_number_of_bc_channels = 10;
    fhss_struct.callbacks.change_channel = &mac_set_channel;
    fhss_stub.int_value = 0;
    channel_list_stub.uint8_value = 20;
    if (fhss_change_to_parent_channel(&fhss_struct)) {
        return false;
    }
    // Parent channel should come from channel list stub
    if (fhss_callbacks_stub.uint8_value != 20) {
        return false;
    }
    fhss_stub.int_value = -1;
    if (fhss_change_to_parent_channel(&fhss_struct) != -1) {
        return false;
    }
    return true;
}

bool test_fhss_change_to_rx_channel()
{
    fhss_structure_t fhss_struct;
    fhss_struct.fhss_state = FHSS_SYNCHRONIZED;
    fhss_struct.rx_channel = 10;
    fhss_struct.callbacks.change_channel = &mac_set_channel;
    // Test NULL parameter
    if (!fhss_change_to_rx_channel(NULL)) {
        return false;
    }
    // Test with proper configuration
    if (fhss_change_to_rx_channel(&fhss_struct)) {
        return false;
    }
    // Should return given rx_channel
    if (fhss_callbacks_stub.uint8_value != 10) {
        return false;
    }
    return true;
}

bool test_fhss_get_offset()
{
    fhss_structure_t fhss_struct;
    uint8_t mac_addr[8];
    // When all channels are broadcast channels should return 0
    fhss_struct.number_of_channels = 50;
    fhss_struct.synch_configuration.fhss_number_of_bc_channels = 50;
    if (fhss_get_offset(&fhss_struct, mac_addr) != 0) {
        return false;
    }

    /* Test when MAC address is 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88
     * Should return 16, when number of unicast channels is 40
     */

    fhss_struct.number_of_channels = 50;
    fhss_struct.synch_configuration.fhss_number_of_bc_channels = 10;

    for (int i=1; i<9; i++) {
        mac_addr[i-1] = i * 0x11;
    }

    if (fhss_get_offset(&fhss_struct, mac_addr) != 16) {
        return false;
    }
    // Test when number of unicast channels is 25, should return 11
    fhss_struct.number_of_channels = 50;
    fhss_struct.synch_configuration.fhss_number_of_bc_channels = 25;
    if (fhss_get_offset(&fhss_struct, mac_addr) != 11) {
        return false;
    }
    return true;
}
