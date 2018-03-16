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
#ifndef __FHSS_BEACON_H__
#define __FHSS_BEACON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define FHSS_DATA_START_DELIMETER   0x00

#define FHSS_SYNCH_BEACON_LIMIT     2
#define FHSS_STOP_SENDING_BEACONS   8
#define FHSS_SYNCHRONIZATION_LOST   10

// Based on datarate and Beacon frame packet length 71 bytes
#define BEACON_TX_TIME              (1000000 / (dev_get_phy_datarate(cur) / 8) * 71)

#define BEACON_INTERVAL_INIT_DIVIDER 100
#define FHSS_SYNCH_DRIFT_TOO_HIGH_LIMIT 20000
#define CLOSE_TO_SUPERFRAME_LENGTH  2000

/** beacon synchronization info */
typedef struct {
    /** Start delimeter */
    uint8_t data_start_delimeter;

    /** Channel index */
    uint8_t channel_index;

    /** Sender unicast channel index */
    uint8_t sender_unicast_channel;

    /** Current superframe */
    uint16_t current_superframe;

    /** Remaining time (us) to next superframe */
    uint16_t remaining_slots;

    /** Channel list counter */
    uint16_t channel_list_counter;

    /** Hop count */
    uint8_t hop_count;

    /** Number of broadcast channels */
    uint8_t number_of_broadcast_channels;

    /** Number of TX slots per channel */
    uint8_t number_of_tx_slots;

    /** Time since last beacon (us) */
    uint32_t time_since_last_beacon;

    /** Processing delay (us) */
    uint16_t processing_delay;

    /** Superframe length */
    uint16_t superframe_length;

    /** Number of superframes per channel */
    uint8_t number_of_superframes_per_channel;

} fhss_synchronization_beacon_payload_s;


#define BEACON_FIELD_SIZE(field) (sizeof((fhss_synchronization_beacon_payload_s*)0)->field)

// Calculate the size of data encoded from fhss_synchronization_beacon_payload_s,
// please add new items from that struct to this macro to keep code working.
#define FHSS_SYNC_BEACON_PAYLOAD_SYNC_SIZE \
    (BEACON_FIELD_SIZE(data_start_delimeter) + \
    BEACON_FIELD_SIZE(channel_index) + \
    BEACON_FIELD_SIZE(sender_unicast_channel) + \
    BEACON_FIELD_SIZE(current_superframe) + \
    BEACON_FIELD_SIZE(remaining_slots) + \
    BEACON_FIELD_SIZE(channel_list_counter) + \
    BEACON_FIELD_SIZE(hop_count) + \
    BEACON_FIELD_SIZE(number_of_broadcast_channels) + \
    BEACON_FIELD_SIZE(number_of_tx_slots) + \
    BEACON_FIELD_SIZE(time_since_last_beacon) + \
    BEACON_FIELD_SIZE(processing_delay) + \
    BEACON_FIELD_SIZE(superframe_length) + \
    BEACON_FIELD_SIZE(number_of_superframes_per_channel))

// 1 byte for protocol id,
// 1 byte for accept join
// 16 bytes for network id, TODO: find proper define to replace this magic value
#define FHSS_SYNC_BEACON_PAYLOAD_PREFIX_SIZE (1 + 1 + 16)

// this counts the common prefix and the fhss specific sync payload
#define FHSS_SYNC_BEACON_PAYLOAD_COMPLETE_SIZE \
    (FHSS_SYNC_BEACON_PAYLOAD_PREFIX_SIZE + FHSS_SYNC_BEACON_PAYLOAD_SYNC_SIZE)


int fhss_beacon_periodic_start(fhss_structure_t *fhss_structure, uint32_t time_to_first_beacon);
void fhss_beacon_periodic_stop(fhss_structure_t *fhss_structure);

void fhss_beacon_build(fhss_structure_t *fhss_structure, uint8_t *dest);

/**
 * Encode the given beacon syncronization structure into given buffer.
 *
 * Note: this information will not include protocol-id, accept_join or network-id
 *
 * @param buffer target buffer, must be at least FHSS_SYNC_BEACON_PAYLOAD_SIZE in size
 * @param source source synchronization data to be encoded
 * @return pointer to one byte after the encoded data
 */
uint8_t* fhss_beacon_encode_raw(uint8_t *buffer, const fhss_synchronization_beacon_payload_s *source);

/**
 * Decode the given beacon syncronization structure from given buffer and process the data.
 *
 * @param protocol_id protocol id from beacon payload
 * @param accept_join accept join from beacon payload
 * @param network_id network id from beacon payload, 16 bytes in size
 * @param dest decoded data
 * @param buffer source buffer, must be at least FHSS_SYNC_BEACON_PAYLOAD_SIZE in size
 * @param elapsed_time time since previous beacon
 * @param number_of_channels number of channels
 */
void fhss_beacon_decode(fhss_synchronization_beacon_payload_s *dest, const uint8_t *buffer, uint32_t elapsed_time, uint16_t number_of_channels);

/**
 * Decode the beacon data as-is to the given struct.
 * 
 * @param dest decoded data
 * @param buffer source buffer, must be at least FHSS_SYNC_BEACON_PAYLOAD_SIZE in size
 */
void fhss_beacon_decode_raw(fhss_synchronization_beacon_payload_s* dest, const uint8_t* buffer);

int fhss_beacon_update_payload(fhss_structure_t *fhss_structure,
                                fhss_synchronization_beacon_payload_s *payload);

void fhss_beacon_received(fhss_structure_t *fhss_structure, const uint8_t *synch_info, const uint32_t elapsed_time);

/**
 * This function is called whenever a node receives a beacon in the "proper state".
 * The implmentation of it stores the data received and eventually synchronizes
 * itself.
 *
 * Note: this is in completely illogical header file, but we have a
 * circular dependency with net_fhss.h and protocol.h.
 *
 * @param cur the network interface which received beacon
 * @param payload decoded beacon payload information
 *
 * @return 0 on success
 */
int fhss_sync_with_beacon(fhss_structure_t *fhss_structure,
                          const fhss_synchronization_beacon_payload_s *payload);

uint8_t fhss_calculate_uc_index(uint8_t channel_index, uint16_t number_of_channels, uint8_t number_of_broadcast_channels);
int8_t fhss_beacon_create_tasklet(fhss_structure_t *fhss_structure);

#ifdef __cplusplus
}
#endif

#endif // !__FHSS_BEACON_H__
