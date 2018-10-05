/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "nsconfig.h"

#ifdef HAVE_THREAD_BORDER_ROUTER

#include <string.h>
#include "ns_types.h"
#include <nsdynmemLIB.h>
#include "ns_trace.h"
#include "common_functions.h"
#include "6LoWPAN/Thread/thread_config.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "ns_sha256.h"
#include "ns_mdns_api.h"
#include "Service_Libs/utils/ns_crc.h"

#define TRACE_GROUP "tmDNS"

/*
 * mDNS data structure
 *
 */
typedef struct {
    ns_mdns_t server_instance;
    ns_mdns_service_t service_instance;
    ns_mdns_service_param_t service_params;
    uint8_t *txt_record_buffer;
    uint16_t txt_value_crc;
    uint8_t txt_record_buffer_len;
    int8_t interface_id;
    int8_t interface_id_mdns;
} thread_mdns_t;

static thread_mdns_t thread_mdns = {
    .server_instance = NULL,
    .service_instance = NULL,
    .txt_record_buffer = NULL,
    .txt_value_crc = 0,
    .txt_record_buffer_len = 0,
    .interface_id = -1,
    .interface_id_mdns = -1,
};

static thread_mdns_t *thread_mdns_ptr = &thread_mdns;

/* mDNS constants */
#define THREAD_MDNS_TIME_TO_LIVE                4500 /* Time to Live in seconds */
#define THREAD_MDNS_TIME_TO_LIVE_HOP_COUNT      120  /* Time to Live in hops */

/* mDNS TXT record State Bitmap */
#define THREAD_MDNS_TXT_SB_CONNECTION_MODE_BM   0x07 /* Mask  0000 0111 */
#define THREAD_MDNS_TXT_SB_INTERFACE_STATUS_BM  0x18 /* Mask  0001 1000 */
#define THREAD_MDNS_TXT_SB_AVAILABILITY_HIGH    0x20 /* Value 0010 0000 */

static uint8_t *thread_mdns_txt_record_sb_get(int interface_id, uint8_t *buf, uint32_t *state_bm)
{
    uint32_t state_bitmap = 0;
    uint8_t *ptr = buf;

    const uint8_t sb_string[] = {7, 's', 'b', '='};
    link_configuration_s *link_configuration = thread_management_configuration_get(interface_id);

    if (link_configuration) {
        if (link_configuration->securityPolicy & SECURITY_POLICY_EXTERNAL_COMMISSIONER_ALLOWED) {
            /* connection mode:
             * 1: DTLS connection to Border Agent allowed with a user chosen network Commissioner Credential and shared PSKc for the active Thread Network Partition
             * 2, DTLS connection to Border Agent allowed using the Border Agent Device Passphrase (PSKd) as the Commissioner Credential
             * */
            state_bitmap = 0x01;
        }

        /* Thread Interface Status
         *  2, Thread interface is initialized with a set of valid operational parameters and is actively part of a Thread Network Partition
         *  */
        state_bitmap |= 0x02<<3;

        /* Availability
         * 1: High availability – The Border Agent device and its Thread interface are part of stable, always-on network infrastructure
         * */
        state_bitmap |= THREAD_MDNS_TXT_SB_AVAILABILITY_HIGH;
    } else {
        /* connection mode:
         * -bits 0-2: 0=DTLS connection to Border Agent is not allowed
         *
         * Thread Interface Status
         * -bits 3-4: 0=Thread interface is not active and is not initialized with a set of valid operational network parameters
         *
         * Availability
         *  -bits 5-6: 0= Infrequent availability - Thread interface may become inactive when the Border Agent device is not in use
         *  */
        state_bitmap = 0x00;
    }

    *state_bm = state_bitmap;

    memcpy(ptr, sb_string, sizeof(sb_string));
    ptr += sizeof(sb_string);

    ptr = common_write_32_bit(state_bitmap, ptr);

    return ptr;
}

static uint8_t *thread_mdns_txt_record_nn_key_fill(int8_t interface_id, uint8_t *buf, uint32_t state_bitmap)
{
    uint8_t *ptr = buf;
    int i;
    uint8_t length;
    const char nn_string[4] = "nn=";

    if (! (state_bitmap & THREAD_MDNS_TXT_SB_CONNECTION_MODE_BM)) {
        // if connection mode is not allowed, elide nn-key
        return ptr;
    }

    ptr++; /* make room for length */
    length = sizeof(nn_string) - 1;
    memcpy(ptr, nn_string, length);
    ptr += length;

    if (state_bitmap & THREAD_MDNS_TXT_SB_AVAILABILITY_HIGH) {
        link_configuration_s *link_configuration = thread_management_configuration_get(interface_id);
        if (!link_configuration) {
            return ptr;
        }

        for (i = 0; i < 16 && link_configuration->name[i] != 0; i++) {
            *ptr++ = link_configuration->name[i];
        }

        length += i; /* update length */
    } else {
        // Thread interface seldomly available, use border agent product or model name
        memcpy(ptr, THREAD_VENDOR_MODEL, sizeof(THREAD_VENDOR_MODEL) - 1);
        ptr += sizeof(THREAD_VENDOR_MODEL) - 1;
        length += sizeof(THREAD_VENDOR_MODEL) - 1;
    }

    *buf = length; // update length

    return ptr;
}

static uint8_t *thread_mdns_txt_record_xp_key_fill(int8_t interface_id, uint8_t *buf, uint32_t state_bitmap)
{
    uint8_t *ptr = buf;
    int length;
    const uint8_t xp_string[4] = { 11, 'x', 'p', '=' };

    if (! (state_bitmap & THREAD_MDNS_TXT_SB_CONNECTION_MODE_BM)) {
        // if connection mode not allowed, skip xb-key
        return ptr;
    }

    length = sizeof xp_string;
    memcpy(ptr, xp_string, length);
    ptr += length;

    if(state_bitmap & THREAD_MDNS_TXT_SB_INTERFACE_STATUS_BM) {
        link_configuration_s *link_configuration = thread_management_configuration_get(interface_id);
        if (!link_configuration) {
            tr_error("Failed to read link configuration");
            return ptr;
        }
        memcpy(ptr, link_configuration->extented_pan_id, 8);
    } else {
        device_configuration_s *device_configuration = thread_management_device_configuration_get(interface_id);
        if (!device_configuration) {
            tr_error("Failed to read device configuration");
            return ptr;
        }
        ns_sha256_nbits(device_configuration->eui64, 8, ptr, 64);
    }

    ptr += 8;

    return ptr;
}

static uint8_t *thread_mdns_txt_record_fill(int8_t interface_id, uint8_t *buf)
{
    static const char mDNS_thread_version[9] = "tv=1.1.1";
    uint8_t *ptr = buf;
    uint32_t state_bitmap;

    /* rv */
    *ptr++ = 0x04; /* rv length */
    memcpy(ptr, "rv=1", 4);
    ptr += 4;

    /* tv */
    *ptr++ = sizeof(mDNS_thread_version) - 1; /* tv length */
    memcpy(ptr, mDNS_thread_version, sizeof(mDNS_thread_version)- 1);
    ptr +=  sizeof(mDNS_thread_version) - 1;

    /* state bitmap */
    ptr = thread_mdns_txt_record_sb_get(interface_id, ptr, &state_bitmap);

    tr_debug("state_bitmap %"PRIx32, state_bitmap);

    /* nn-key */
    ptr = thread_mdns_txt_record_nn_key_fill(interface_id, ptr, state_bitmap);

    /* xp-key */
    ptr = thread_mdns_txt_record_xp_key_fill(interface_id, ptr, state_bitmap);

    *ptr++ = 0;

    return ptr;
}

/* callback from mDNS to fill TXT record */
static const uint8_t *thread_mdns_txt_record_callback(void)
{
    uint8_t buffer[200]; /* 200 bytes enough space for TXT record */
    uint8_t *ptr;
    size_t new_record_length;

    ptr = thread_mdns_txt_record_fill(thread_mdns_ptr->interface_id, buffer);

    new_record_length = ptr - buffer;

    if (thread_mdns_ptr->txt_record_buffer_len < new_record_length) {
        // not enough space in current buffer
        ns_dyn_mem_free(thread_mdns_ptr->txt_record_buffer);
        thread_mdns_ptr->txt_record_buffer = NULL;
    }

    if (thread_mdns_ptr->txt_record_buffer == NULL) {
        thread_mdns_ptr->txt_record_buffer = ns_dyn_mem_alloc(new_record_length);
    }

    if (!thread_mdns_ptr->txt_record_buffer) {
        tr_error("mDNS record not allocated");
        return (uint8_t*)'\0';
    }

    memcpy(thread_mdns_ptr->txt_record_buffer, buffer, new_record_length);
    thread_mdns_ptr->txt_record_buffer_len = new_record_length;

    tr_debug("mDNS TXT record created, %d bytes", thread_mdns_ptr->txt_record_buffer_len);

    return thread_mdns_ptr->txt_record_buffer;
}

static uint16_t thread_mdns_txt_values_crc_get(void)
{
    link_configuration_s *link_configuration;
    uint16_t txt_values_crc = 0;

    if (!thread_mdns_ptr->server_instance) {
        // mdns not active
        return txt_values_crc;
    }

    link_configuration = thread_joiner_application_get_config(thread_mdns_ptr->interface_id);
    if (link_configuration) {
        // calculate CRC for values in mDNS TXT record that may change
        txt_values_crc = crc16_ccitt(link_configuration->name, 16);
        txt_values_crc |= crc16_ccitt(link_configuration->extented_pan_id, 8);
        txt_values_crc |= crc16_ccitt(&link_configuration->securityPolicy, 1);
    }

    return txt_values_crc;
}

int thread_mdns_stop(void)
{
    if (thread_mdns_ptr->server_instance == NULL) {
        return -1;
    }

    ns_mdns_server_stop(thread_mdns_ptr->server_instance);

    thread_mdns_ptr->server_instance = NULL;
    ns_dyn_mem_free(thread_mdns_ptr->txt_record_buffer);
    thread_mdns_ptr->txt_record_buffer = NULL;
    thread_mdns_ptr->txt_record_buffer_len = 0;

    return 0;
}

int thread_mdns_start(int8_t interface_id, int8_t interface_id_mdns, const char *service_name)
{
    static const char *mDNS_service_type = "_meshcop._udp";

    if (thread_mdns_ptr->server_instance != NULL) {
        return -1;
    }

    thread_mdns_ptr->server_instance = ns_mdns_server_start(service_name, THREAD_MDNS_TIME_TO_LIVE, THREAD_MDNS_TIME_TO_LIVE_HOP_COUNT, interface_id_mdns);
    if (thread_mdns_ptr->server_instance == NULL) {
        tr_error("mDNS server initialization failed");
        return -2;
    }

    thread_mdns_ptr->interface_id = interface_id;
    thread_mdns_ptr->interface_id_mdns = interface_id_mdns;
    thread_mdns_ptr->service_params.service_type = mDNS_service_type;
    thread_mdns_ptr->service_params.service_port = THREAD_COMMISSIONING_PORT;
    thread_mdns_ptr->service_params.service_get_txt = thread_mdns_txt_record_callback;
    thread_mdns_ptr->service_instance = ns_mdns_service_register(thread_mdns_ptr->server_instance, &thread_mdns_ptr->service_params);
    thread_mdns_ptr->txt_value_crc = thread_mdns_txt_values_crc_get();

    if (thread_mdns_ptr->service_instance == NULL) {
        tr_error("mDNS service init failed");
        thread_mdns_stop();
        return -3;
    }

    return 0;
}

#endif //HAVE_THREAD_BORDER_ROUTER

void thread_mdns_network_data_update_notify(void)
{
#ifdef HAVE_THREAD_BORDER_ROUTER
    uint16_t txt_value_crc;
    txt_value_crc = thread_mdns_txt_values_crc_get();
    if (txt_value_crc != thread_mdns_ptr->txt_value_crc) {
        // Values for TXT-record have changed, send mDNS announcement
        thread_mdns_ptr->txt_value_crc = txt_value_crc;
        ns_mdns_announcement_send(thread_mdns_ptr->server_instance);
    }
#endif //HAVE_THREAD_BORDER_ROUTER
}

