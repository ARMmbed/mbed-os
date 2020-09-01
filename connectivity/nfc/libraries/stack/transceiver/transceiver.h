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
 * \file transceiver.h
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 */

#ifndef TRANSCEIVER_H_
#define TRANSCEIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stack/nfc_common.h"

typedef struct __nfc_tech nfc_tech_t;
typedef struct __transceiver nfc_transceiver_t;
typedef struct __transceiver_impl transceiver_impl_t;

#include "protocols.h"
#include "platform/nfc_transport.h"
#include "platform/nfc_scheduler.h"

enum __nfc_framing {
    nfc_framing_unknown,

    nfc_framing_target_mode_detector, //Framing is unknown and will be detected by the hardware
    nfc_framing_target_a_106,
    nfc_framing_target_b_106,
    nfc_framing_target_f_212,
    nfc_framing_target_f_424,

    nfc_framing_initiator_a_106,
    nfc_framing_initiator_b_106,
    nfc_framing_initiator_f_212,
    nfc_framing_initiator_f_424,
};
typedef enum __nfc_framing nfc_framing_t;

struct __nfc_tech {
    unsigned int nfc_type1 : 1;
    unsigned int nfc_type2 : 1;
    unsigned int nfc_type3 : 1;
    unsigned int nfc_iso_dep_a : 1;
    unsigned int nfc_iso_dep_b : 1;
    unsigned int nfc_nfc_dep_a : 1;
    unsigned int nfc_nfc_dep_f_212 : 1;
    unsigned int nfc_nfc_dep_f_424 : 1;
};

typedef struct __polling_options polling_options_t;
struct __polling_options {
    unsigned int bail_at_first_target : 1;
    unsigned int bail_at_first_tech : 1;
    int32_t listen_for;
};

typedef void (*transceiver_cb_t)(nfc_transceiver_t *pTransceiver, nfc_err_t ret, void *pUserData);
typedef void (*set_protocols_fn_t)(nfc_transceiver_t *pTransceiver, nfc_tech_t initiators, nfc_tech_t targets, polling_options_t options);
typedef void (*poll_fn_t)(nfc_transceiver_t *pTransceiver);
typedef void (*set_crc_fn_t)(nfc_transceiver_t *pTransceiver, bool crcOut, bool crcIn);
typedef void (*set_timeout_fn_t)(nfc_transceiver_t *pTransceiver, int timeout);
typedef void (*set_transceive_options_fn_t)(nfc_transceiver_t *pTransceiver, bool transmit, bool receive, bool repoll);
typedef void (*set_transceive_framing_fn_t)(nfc_transceiver_t *pTransceiver, nfc_framing_t framing);
typedef void (*set_write_fn_t)(nfc_transceiver_t *pTransceiver, ac_buffer_t *pWriteBuf); //Set write buffer
typedef ac_buffer_t *(*get_read_fn_t)(nfc_transceiver_t *pTransceiver); //Get read buffer
typedef size_t (*get_last_byte_length_fn_t)(nfc_transceiver_t *pTransceiver);
typedef void (*set_last_byte_length_fn_t)(nfc_transceiver_t *pTransceiver, size_t lastByteLength);
typedef size_t (*get_first_byte_align_fn_t)(nfc_transceiver_t *pTransceiver);
typedef void (*set_first_byte_align_fn_t)(nfc_transceiver_t *pTransceiver, size_t firstByteAlign);
typedef void (*transceive_fn_t)(nfc_transceiver_t *pTransceiver);
typedef void (*abort_fn_t)(nfc_transceiver_t *pTransceiver);
typedef void (*close_fn_t)(nfc_transceiver_t *pTransceiver);
typedef void (*sleep_fn_t)(nfc_transceiver_t *pTransceiver, bool sleep);

struct __transceiver_impl {
    set_protocols_fn_t set_protocols;
    poll_fn_t poll;
    set_crc_fn_t set_crc;
    set_timeout_fn_t set_timeout;
    set_transceive_options_fn_t set_transceive_options;
    set_transceive_framing_fn_t set_transceive_framing;
    set_write_fn_t set_write;
    get_read_fn_t get_read;
    set_last_byte_length_fn_t set_last_byte_length;
    get_last_byte_length_fn_t get_last_byte_length;
    set_first_byte_align_fn_t set_first_byte_align;
    transceive_fn_t transceive;
    abort_fn_t abort;
    close_fn_t close;
    sleep_fn_t sleep;
};

typedef struct __nfc_a_info nfc_a_info_t;
struct __nfc_a_info {
    uint8_t uid[10];
    size_t uidLength;
    uint8_t sak;
    uint8_t atqa[2];
};

typedef struct __nfc_b_info nfc_b_info_t;
struct __nfc_b_info {
    uint8_t pupi[4];
    uint8_t application_data[4];
    uint8_t protocol_info[3];
};

typedef struct __nfc_f_info nfc_f_info_t;
struct __nfc_f_info {
    uint8_t nfcid2[8];
};

typedef struct __nfc_info nfc_info_t;

struct __nfc_info {
    nfc_tech_t type;
    union {
        nfc_a_info_t nfcA;
        nfc_b_info_t nfcB;
        nfc_f_info_t nfcF;
    };
};

#define MUNFC_MAX_REMOTE_TARGETS 4
struct __transceiver {
    const transceiver_impl_t *fn; //vtable

    bool initiator_ntarget;
    nfc_info_t remote_targets[MUNFC_MAX_REMOTE_TARGETS];
    size_t remote_targets_count;

    nfc_tech_t active_tech;

    transceiver_cb_t cb; //Callback to upper layer
    void *pUserData;
    nfc_task_t task; //Task for deferred execution

    nfc_transport_t *pTransport;
    nfc_scheduler_t scheduler;
};

void transceiver_init(nfc_transceiver_t *pTransceiver, nfc_transport_t *pTransport, nfc_scheduler_timer_t *pTimer);

static inline void transceiver_set_protocols(nfc_transceiver_t *pTransceiver, nfc_tech_t initiators, nfc_tech_t targets, polling_options_t options)
{
    pTransceiver->fn->set_protocols(pTransceiver, initiators, targets, options);
}

static inline void transceiver_poll(nfc_transceiver_t *pTransceiver, transceiver_cb_t cb, void *pUserData)
{
    pTransceiver->cb = cb;
    pTransceiver->pUserData = pUserData;
    pTransceiver->fn->poll(pTransceiver);
}

static inline void transceiver_set_crc(nfc_transceiver_t *pTransceiver, bool crcOut, bool crcIn)
{
    pTransceiver->fn->set_crc(pTransceiver, crcOut, crcIn);
}

static inline void transceiver_set_timeout(nfc_transceiver_t *pTransceiver, int timeout)
{
    pTransceiver->fn->set_timeout(pTransceiver, timeout);
}

static inline void transceiver_set_transceive_options(nfc_transceiver_t *pTransceiver, bool transmit, bool receive, bool repoll)
{
    pTransceiver->fn->set_transceive_options(pTransceiver, transmit, receive, repoll);
}

static inline void transceiver_set_transceive_framing(nfc_transceiver_t *pTransceiver, nfc_framing_t framing)
{
    pTransceiver->fn->set_transceive_framing(pTransceiver, framing);
}

static inline void transceiver_set_write(nfc_transceiver_t *pTransceiver, ac_buffer_t *pWriteBuf)
{
    pTransceiver->fn->set_write(pTransceiver, pWriteBuf);
}

static inline ac_buffer_t *transceiver_get_read(nfc_transceiver_t *pTransceiver)
{
    return pTransceiver->fn->get_read(pTransceiver);
}

static inline size_t transceiver_get_last_byte_length(nfc_transceiver_t *pTransceiver)
{
    return pTransceiver->fn->get_last_byte_length(pTransceiver);
}

static inline void transceiver_set_last_byte_length(nfc_transceiver_t *pTransceiver, size_t lastByteLength)
{
    pTransceiver->fn->set_last_byte_length(pTransceiver, lastByteLength);
}

static inline void transceiver_set_first_byte_align(nfc_transceiver_t *pTransceiver, size_t firstByteAlign)
{
    pTransceiver->fn->set_first_byte_align(pTransceiver, firstByteAlign);
}

static inline void nfc_transceiver_transceive(nfc_transceiver_t *pTransceiver, transceiver_cb_t cb, void *pUserData)
{
    pTransceiver->cb = cb;
    pTransceiver->pUserData = pUserData;
    pTransceiver->fn->transceive(pTransceiver);
}

static inline void transceiver_abort(nfc_transceiver_t *pTransceiver)
{
    pTransceiver->fn->abort(pTransceiver);
}

static inline void transceiver_close(nfc_transceiver_t *pTransceiver)
{
    pTransceiver->fn->close(pTransceiver);
}

static inline bool transceiver_is_initiator_mode(nfc_transceiver_t *pTransceiver)
{
    return pTransceiver->initiator_ntarget;
}

static inline nfc_tech_t transceiver_get_active_techs(nfc_transceiver_t *pTransceiver)
{
    return pTransceiver->active_tech;
}

static inline nfc_scheduler_t *transceiver_get_scheduler(nfc_transceiver_t *pTransceiver)
{
    return &pTransceiver->scheduler;
}

static inline const nfc_info_t *transceiver_get_remote_target_info(nfc_transceiver_t *pTransceiver, size_t number)
{
    if (number > pTransceiver->remote_targets_count) {
        return NULL;
    }
    return &pTransceiver->remote_targets[number];
}

static inline size_t transceiver_get_remote_targets_count(nfc_transceiver_t *pTransceiver)
{
    return pTransceiver->remote_targets_count;
}

static inline void transceiver_sleep(nfc_transceiver_t *pTransceiver, bool sleep)
{
    pTransceiver->fn->sleep(pTransceiver, sleep);
}

#ifdef __cplusplus
}
#endif

#endif /* TRANSCEIVER_H_ */
