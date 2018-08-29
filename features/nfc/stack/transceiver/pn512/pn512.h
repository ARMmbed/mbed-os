/*
 * Copyright (c) 2013-2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * \file pn512.h
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 */

#ifndef PN512_H_
#define PN512_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stack/nfc_common.h"
#include "transceiver/transceiver.h"

#include "pn512_types.h"
#include "pn512_callback.h"

typedef enum __pn512_state {
    pn512_state_ready,
    pn512_state_target_autocoll,
    pn512_state_initiator_transceive_first_frame,
    pn512_state_transceive,
    pn512_state_transceive_last_frame,
} pn512_state_t;

typedef enum __pn512_transceive_mode {
    pn512_transceive_mode_idle,
    pn512_transceive_mode_target_autocoll,
    pn512_transceive_mode_transmit,
    pn512_transceive_mode_transmit_and_target_autocoll,
    pn512_transceive_mode_transceive,
    pn512_transceive_mode_receive,
} pn512_transceive_mode_t;

struct __pn512 {
    nfc_transceiver_t transceiver;
    //Impl specific
    pn512_registers_t registers;
    bool rf_on;
    struct {
        bool out;
        bool in;
    } crc;
    int timeout;

    struct {
        nfc_tech_t initiators;
        nfc_tech_t targets;
        polling_options_t options;
    } config;

    //Transceive options
    pn512_transceive_mode_t nextFrameMode;

    nfc_framing_t framing;
    uint16_t irqsEn;
    uint8_t payload[256]; //Incoming buffer

    ac_buffer_builder_t readBufBldr;
    ac_buffer_t writeBuf;

    uint8_t readFirstByteAlign;
    uint8_t readLastByteLength;
    uint8_t writeLastByteLength;

    //Task parameters
    struct {
        //Polling
        struct {
            enum {
                pn512_polling_state_start_listening,

                pn512_polling_state_listen_wait_for_remote_field,
                pn512_polling_state_listen_anticollision,

                pn512_polling_state_listen_no_target_found,

                pn512_polling_state_start_polling,

                pn512_polling_state_rf_collision_avoidance, // TID + n × TRFW, n is random, TID>4096/(13.56E6) ~ 302.06us, TRFW=51/(13.56E6) ~ 37.76us
                pn512_polling_state_polling_nfc_a_start,
                pn512_polling_state_polling_nfc_a_gt, // guard time nfc a >= 5.0 ms
                pn512_polling_state_polling_nfc_a_anticollision, // polling for nfc a
                pn512_polling_state_polling_nfc_b_start,
                pn512_polling_state_polling_nfc_b_gt, // guard time nfc b >= 5.0 ms
                pn512_polling_state_polling_nfc_b_anticollision, // polling for nfc b
                pn512_polling_state_polling_nfc_f_start,
                pn512_polling_state_polling_nfc_f_gt, // guard time nfc f >= 20 ms
                pn512_polling_state_polling_nfc_f_anticollision, // polling for nfc f

                pn512_polling_state_finish_polling,

            } state;

            pn512_cb_t cb;
        } poll;
        struct {
            pn512_cb_t cb;
            pn512_transceive_mode_t mode;
        } transceive;
        struct {
            pn512_cb_t cb;
        } rf;
        struct {
            union {
                // ISO A
                struct {
                    bool more_targets; // Collision detected
                    uint8_t cascade_level;
                    uint8_t cln[5];
                    uint8_t valid_bits; // valid bits within cascade level
                } iso_a;
                // ISO B
                struct {
                    bool more_targets; // Collision detected
                    uint8_t slots_num_exponent;
                    uint8_t slot_number;
                    bool found_one;
                } iso_b;
            };
            pn512_cb_t cb;
        } anticollision;
    };

};

nfc_err_t pn512_init(pn512_t *pPN512, nfc_transport_t *pTransport, nfc_scheduler_timer_t *pTimer);

nfc_transceiver_t *pn512_get_transceiver(pn512_t *pPN512);

#ifdef __cplusplus
}
#endif

#endif /* PN512_H_ */
