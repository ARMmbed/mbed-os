/*
 * Copyright (c) 2017-2019, Arm Limited and affiliates.
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

/**
 * \brief Thread commercial mode implementation for CCM bootstrap.
 */
#include "nsconfig.h"
#include <ns_types.h>
#include <string.h>
#include "nsdynmemLIB.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "coap_service_api.h"
#include "thread_meshcop_lib.h"
#include "randLIB.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_discovery.h"
#include "6LoWPAN/Thread/thread_constants.h"
#include "6LoWPAN/Thread/thread_management_server.h"
#include "6LoWPAN/Thread/thread_nvm_store.h"
#include "6LoWPAN/Thread/thread_ccm.h"


/*
 * Thread 1.2 CCM security credentials
 */
#ifdef HAVE_THREAD_V2

#if 1
/* Hardcoded CSR request */
static const unsigned char csr_request[215] = {
    0x30, 0x81, 0xd4, 0x30, 0x7c, 0x02, 0x01, 0x00, 0x30, 0x1a, 0x31, 0x18, 0x30, 0x16, 0x06, 0x03,
    0x55, 0x04, 0x03, 0x0c, 0x0f, 0x54, 0x68, 0x72, 0x65, 0x61, 0x64, 0x44, 0x65, 0x76, 0x69, 0x63,
    0x65, 0x31, 0x32, 0x33, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02,
    0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0xb1,
    0xab, 0xe8, 0xa2, 0xa1, 0xe6, 0x62, 0x5e, 0xae, 0x9e, 0x7e, 0x41, 0xcf, 0x7e, 0x95, 0x58, 0x19,
    0x51, 0x47, 0xea, 0x0f, 0xe2, 0xf8, 0xc2, 0x1b, 0x61, 0xa5, 0x30, 0x05, 0xc0, 0x91, 0xb6, 0x07,
    0xc2, 0x43, 0x46, 0xda, 0x75, 0xc0, 0x58, 0xd8, 0x08, 0xa5, 0xbb, 0xdb, 0xdc, 0x0e, 0xf2, 0x05,
    0x62, 0xfb, 0x28, 0xbb, 0xa7, 0xd2, 0x9e, 0xef, 0x8f, 0xbf, 0xba, 0xcd, 0x51, 0xa5, 0xfd, 0xa0,
    0x00, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x03, 0x48, 0x00,
    0x30, 0x45, 0x02, 0x20, 0x4e, 0xb2, 0x25, 0x82, 0x24, 0xf6, 0xe1, 0x51, 0xd6, 0x0c, 0x19, 0x60,
    0x88, 0xb8, 0xe2, 0xfd, 0x90, 0xd2, 0xc1, 0x0f, 0xb0, 0x4f, 0x8e, 0x73, 0x13, 0x5c, 0x9f, 0x42,
    0x09, 0x68, 0xdf, 0x05, 0x02, 0x21, 0x00, 0xc9, 0xc2, 0x63, 0x83, 0x62, 0x24, 0x15, 0x73, 0xf1,
    0x63, 0xea, 0xe3, 0xd2, 0xf1, 0x50, 0x48, 0x56, 0xdf, 0x6b, 0xcf, 0xc4, 0x31, 0xc4, 0xcf, 0xbc,
    0x26, 0xe3, 0x5a, 0x74, 0x62, 0x0f, 0x70
};
#else
/* CSR request with CBOR header*/
static const unsigned char csr_request[244] = {
    0x58, 0xf2, 0x30, 0x81, 0xef, 0x30, 0x81, 0x95, 0x02, 0x01, 0x01, 0x30, 0x33, 0x31, 0x1c, 0x30, 0x1a, 0x06,
    0x03, 0x55, 0x04, 0x03, 0x0c, 0x13, 0x4c, 0x69, 0x67, 0x68, 0x74, 0x69, 0x66, 0x79, 0x20, 0x50, 0x72, 0x6f,
    0x20, 0x4d, 0x6f, 0x64, 0x75, 0x6c, 0x65, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x0a,
    0x4f, 0x53, 0x52, 0x41, 0x4d, 0x20, 0x47, 0x6d, 0x62, 0x48, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86,
    0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00,
    0x04, 0xd8, 0x19, 0x64, 0x07, 0xca, 0x38, 0x01, 0x62, 0xfd, 0x7e, 0xe7, 0x07, 0x8d, 0x21, 0x50, 0x0b, 0x9f,
    0x00, 0x71, 0x26, 0xaa, 0x55, 0x2a, 0x44, 0x9b, 0xe8, 0xfd, 0xfb, 0x0e, 0x8d, 0x41, 0x01, 0xf2, 0x7b, 0x2e,
    0x7b, 0xe4, 0x4f, 0x35, 0x00, 0x0b, 0x1f, 0xbc, 0x86, 0x57, 0xa1, 0x69, 0x32, 0x49, 0xcf, 0xd7, 0x2f, 0x0b,
    0xfa, 0x22, 0x44, 0x0b, 0x6f, 0xf4, 0xb5, 0xbd, 0x0f, 0x20, 0xab, 0xa0, 0x00, 0x30, 0x0a, 0x06, 0x08, 0x2a,
    0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x03, 0x49, 0x00, 0x30, 0x46, 0x02, 0x21, 0x00, 0xb5, 0xa2, 0x8b,
    0xf9, 0xbf, 0x7d, 0x2c, 0x72, 0x3e, 0xf0, 0xad, 0x1e, 0x38, 0x28, 0xc0, 0xa3, 0xe8, 0xd6, 0x70, 0x9a, 0x2a,
    0xf8, 0x1d, 0x33, 0x9d, 0xbb, 0x6c, 0x4f, 0x7c, 0x81, 0xb6, 0x71, 0x02, 0x21, 0x00, 0xba, 0x74, 0x50, 0xad,
    0x27, 0x2e, 0x00, 0x71, 0x68, 0x7f, 0xe0, 0x2c, 0x8c, 0x1b, 0x6f, 0x95, 0x8c, 0x58, 0x1e, 0xe7, 0xe3, 0xa5,
    0x50, 0xca, 0x12, 0x0a, 0x60, 0x56, 0xd2, 0x3a, 0xe2, 0xeb
}
#endif

/*
 * Private key for certificate m_device_nxp_sn_and_8021ar.cert.pem -> test registrar will return this as default
 */
static const unsigned char domain_private_key[138] = {
    0x30, 0x81, 0x87, 0x02, 0x01, 0x00, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02,
    0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x04, 0x6d, 0x30, 0x6b, 0x02,
    0x01, 0x01, 0x04, 0x20, 0x49, 0x9c, 0x76, 0x67, 0xd9, 0xae, 0x4e, 0xb6, 0x38, 0xb7, 0xce, 0x25,
    0x2b, 0x21, 0xf4, 0x6a, 0x88, 0x28, 0xaa, 0x16, 0x74, 0x6e, 0xc3, 0xbc, 0xbb, 0x37, 0x50, 0xc9,
    0x87, 0xbd, 0xff, 0xbf, 0xa1, 0x44, 0x03, 0x42, 0x00, 0x04, 0x50, 0xe9, 0x5f, 0xd3, 0xb1, 0x72,
    0x74, 0xb6, 0xaf, 0x15, 0xae, 0xd7, 0x6d, 0xf9, 0x7f, 0xfa, 0x4f, 0xaf, 0xd6, 0x1f, 0x09, 0x29,
    0x37, 0xf8, 0x1d, 0x27, 0xeb, 0x31, 0x41, 0x62, 0x52, 0x4e, 0x6b, 0x51, 0x8e, 0x08, 0x72, 0xce,
    0xc2, 0x43, 0x69, 0x0a, 0x57, 0xb5, 0x54, 0xeb, 0x9b, 0x06, 0xad, 0xed, 0x7c, 0x56, 0x6e, 0x0c,
    0x23, 0xcb, 0x1e, 0x51, 0x78, 0xe4, 0xae, 0x41, 0x58, 0x15
};

const uint8_t meshcop_nmkp_resp_ignore[] = {
    MESHCOP_TLV_COMM_SIGNATURE,
    MESHCOP_TLV_COMM_TOKEN,
    MESHCOP_TLV_COMMISSIONER_SESSION_ID,
    MESHCOP_TLV_COMMISSIONER_ID,
    MESHCOP_TLV_STATE
};

static NS_LIST_DEFINE(ccm_instance_list, thread_ccm_credentials_t, link);

#define TRACE_GROUP "ccmb"

static int stringlen(const char *s, int n)
{
    char *end = memchr(s, 0, n);
    return end ? end - s : n;
}

static int8_t thread_ccm_find_id_by_service(int8_t service_id)
{
    ns_list_foreach(thread_ccm_credentials_t, cur, &ccm_instance_list) {
        if (cur->coap_service_secure_session_id == service_id) {
            return cur->interface_id;
        }
    }
    return -1;
}

static thread_ccm_credentials_t *thread_ccm_find_by_service(int8_t service_id)
{
    ns_list_foreach(thread_ccm_credentials_t, cur, &ccm_instance_list) {
        if (cur->coap_service_secure_session_id == service_id) {
            return cur;
        }
    }
    return NULL;
}

static void thread_ccm_attach_cb(void *arg)
{
    thread_ccm_credentials_t *this = arg;
    tr_debug("Attach to new network");
    if (!this) {
        return;
    }
    if (this->reattach_ongoing) {
        this->reattach_ongoing = false;
        thread_nvm_store_mleid_rloc_map_remove();
        thread_nvm_store_link_info_clear();
        thread_joiner_application_link_configuration_delete(this->interface_id);
        thread_bootstrap_connection_error(this->interface_id, CON_ERROR_NETWORK_KICK, NULL);
    } else {
        // Cleaning up the joining information
        if (this->ccm_done_cb) {// This is successfull
            this->ccm_done_cb(this->interface_id);
        }
    }
}

int8_t thread_ccm_network_reattach(int8_t service_id, uint16_t timeout, bool clear_data)
{
    thread_ccm_credentials_t *this = thread_ccm_find_by_service(service_id);
    if (!this) {
        return -1;
    }

    this->reattach_ongoing = clear_data;

    // re-attach in any case and close the secure connection
    this->attach_timeout = eventOS_timeout_ms(thread_ccm_attach_cb, timeout, this);

    return 0;
}

static int thread_ccm_security_start_cb(int8_t service_id, uint8_t address[static 16], uint16_t port, uint8_t *pw, uint8_t *pw_len)
{
    /* Certificates used, pskd not needed */
    (void) service_id;
    (void) address;
    (void) port;
    (void) pw;
    (void) pw_len;

    return 0;
}

static int thread_ccm_enroll_parse(protocol_interface_info_entry_t *cur, uint8_t *payload_ptr, uint16_t payload_len)
{
    uint8_t *ptr;
    uint16_t len, flen;

    ptr = payload_ptr;
    len = payload_len;
    // CBOR format check
    if (*ptr == 0x58) {
        flen = *(ptr + 1);
        ptr += 2;
        len -= 2;
    } else if (*ptr == 0x59) {
        flen = common_read_16_bit(ptr + 1);
        ptr += 3;
        len -= 3;
    } else {
        // no shorter than 23 byte certificates supported
        flen = 0;
    }

    if (flen != len ||
            0 > thread_ccm_network_certificate_set(cur, ptr, len)) {
        tr_warn("ae response parse failed, len %d != %d", len, flen);
    }

    return 0;
}

static int thread_ccm_simple_enroll_response_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    (void) source_address;
    (void) source_port;

    // re-attach in any case and close the secure connection
    thread_ccm_network_reattach(service_id, 1000, false);
    coap_service_close_secure_connection(service_id, source_address, source_port);

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(thread_ccm_find_id_by_service(service_id));

    tr_debug("Simple enrollment received len:%d - %s", response_ptr->payload_len, trace_array(response_ptr->payload_ptr, response_ptr->payload_len));

    if (!cur || !cur->thread_info || !response_ptr) {
        return -1;
    }

    thread_ccm_enroll_parse(cur, response_ptr->payload_ptr, response_ptr->payload_len);

    return 0;
}

static int thread_ccm_csrattrs_response_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    (void) response_ptr;

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(thread_ccm_find_id_by_service(service_id));

    if (!cur || !cur->thread_info) {
        return -1;
    }
    tr_info("Receiving csrattrs response sending simpleenroll");

    // TODO Create CSR and private key here... Now we use hardcoded stuff.
    thread_ccm_network_private_key_set(cur, domain_private_key, sizeof(domain_private_key));

    coap_service_request_send(service_id, COAP_REQUEST_OPTIONS_SECURE_BYPASS, source_address, source_port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_SIMPLEENROLL, THREAD_CONTENT_FORMAT_PKCS10,
                              csr_request, sizeof(csr_request), thread_ccm_simple_enroll_response_cb);

    return 0;
}


char *thread_ccm_parse_rat_response(uint8_t *data, uint16_t len)
{
    char *ptr;

    for (ptr = (char *)data; (ptr - (char *)data) < len; ptr++) {
        if (*ptr == 'd') {
            if (!strncmp(ptr, "domainCAcert", 12)) {
                return ptr;
            }
        }
    }

    return NULL;
}

static int thread_ccm_rat_response_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    char *ca_cert_ptr = NULL;
    uint16_t ca_cert_len = 0;

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(thread_ccm_find_id_by_service(service_id));

    if (!cur || !cur->thread_info) {
        return -1;
    }

    tr_info("Receiving RAT response sending csrattrs request");

    if (!response_ptr || !response_ptr->payload_ptr) {
        tr_error("No response payload");
        return -1;
    }

    // Parse CA certificate from RAT response
    // Replace with CBOR library, when the time is right
    // Here we receive private key for the CA certi

    ca_cert_ptr = thread_ccm_parse_rat_response(response_ptr->payload_ptr, response_ptr->payload_len);

    if (ca_cert_ptr) {
        ca_cert_ptr += 13; // Jump over "domainCAcert and some cbor format byte...
        ca_cert_len = common_read_16_bit((uint8_t *)ca_cert_ptr);   // read length
        ca_cert_ptr += 2;
        tr_debug("CA cert len %d", ca_cert_len);
        /* Set domain CA certificate pointer and length */

        if (thread_info(cur)->ccm_credentials_ptr->domain_ca_certificate_ptr) {
            ns_dyn_mem_free(thread_info(cur)->ccm_credentials_ptr->domain_ca_certificate_ptr);
        }
        thread_info(cur)->ccm_credentials_ptr->domain_ca_certificate_ptr = ns_dyn_mem_alloc(ca_cert_len);
        if (!thread_info(cur)->ccm_credentials_ptr->domain_ca_certificate_ptr) {
            return -1;
        }
        memcpy(thread_info(cur)->ccm_credentials_ptr->domain_ca_certificate_ptr, ca_cert_ptr, ca_cert_len);
        thread_info(cur)->ccm_credentials_ptr->domain_ca_certificate_len = ca_cert_len;
    } else {
        tr_error("Response parse failed");
    }

    // TODO Verify nonce

    coap_service_request_send(service_id, COAP_REQUEST_OPTIONS_SECURE_BYPASS, source_address, source_port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_GET, THREAD_URI_CSRATTRS, THREAD_CONTENT_FORMAT_CSRATTRS, NULL, 0, thread_ccm_csrattrs_response_cb);

    return 0;
}
/*A2                     # map(2)
   67                  # text(7)
      76657273696F6E   # "version"
   61                  # text(1)
      31               # "1"
   65                  # text(5)
      6E6F6E6365       # "nonce"
   48                  # bytes(8)
      13ADD904605D973E # "\x13\xAD\xD9\x04`]\x97>"
 *
 */
static int thread_ccm_rat_request_build(uint8_t *rat_payload, int length)
{
    uint8_t *ptr = rat_payload;

    if (length < 30) {
        return 0;
    }

    *rat_payload++ = 0xa2;      // map (2)

    // text (7) "version" + unsigned (1) "1"
    *rat_payload++ = 0x67;
    memcpy(rat_payload, "version", 7);
    rat_payload += 7;
    *rat_payload++ = 0x61;
    *rat_payload++ = 0x31;

    // text (5) "nonce" + bytes (8) random nonce
    // todo: save nonce to verify response against reply.
    *rat_payload++ = 0x65;
    memcpy(rat_payload, "nonce", 5);
    rat_payload += 5;

    *rat_payload++ = 0x48;
    common_write_64_bit(randLIB_get_64bit(), rat_payload);
    rat_payload += 8;

    return rat_payload - ptr;
}

static int thread_ccm_ae_commission_start(int8_t interface_id, uint8_t parent_address[16], uint16_t port, thread_commission_done_cb *done_cb)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    uint8_t rat_payload[30];
    int rat_len;

    if (!done_cb || !cur) {
        return -1;
    }

    thread_info(cur)->ccm_credentials_ptr->coap_service_secure_session_id = coap_service_initialize(interface_id, port, COAP_SERVICE_OPTIONS_EPHEMERAL_PORT | COAP_SERVICE_OPTIONS_SECURE | 0x80, thread_ccm_security_start_cb, joiner_application_security_done_cb);

    if (0 > coap_service_certificate_set(thread_info(cur)->ccm_credentials_ptr->coap_service_secure_session_id, cur->thread_info->ccm_credentials_ptr->device_certificate_ptr, cur->thread_info->ccm_credentials_ptr->device_certificate_len,
                                         cur->thread_info->ccm_credentials_ptr->device_pk_ptr, cur->thread_info->ccm_credentials_ptr->device_pk_len)) {
        tr_debug("coap service certificate set failed");
        return -1;
    }
    tr_debug("start ae commissioning: interface %d, parent: %s, port %d", interface_id, trace_ipv6(parent_address), port);

    thread_info(cur)->ccm_credentials_ptr->ccm_done_cb = done_cb;
    memcpy(thread_info(cur)->ccm_credentials_ptr->ccm_addr, parent_address, 16);
    thread_info(cur)->ccm_credentials_ptr->ccm_port = port;

    rat_len = thread_ccm_rat_request_build(rat_payload, sizeof(rat_payload));
    if (rat_len == 0) {
        tr_debug("RAT request payload build failed");
        return -1;
    }

    // todo: This might not be needed if no extra certificate processing made by device and should directly call simpleenroll
    coap_service_request_send(thread_info(cur)->ccm_credentials_ptr->coap_service_secure_session_id, COAP_REQUEST_OPTIONS_SECURE_BYPASS, parent_address, port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_RAT, THREAD_CONTENT_FORMAT_AUDITNONCE, rat_payload, rat_len, thread_ccm_rat_response_cb);

    return 0;
}

static int thread_ccm_nmkp_response_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    tr_debug("nmkp provisioning done");

    // re-attach in any case and close the secure connection
    thread_ccm_network_reattach(service_id, 1000, false);
    coap_service_close_secure_connection(service_id, source_address, source_port);

    // CoAP message failed - try to reattach
    if (!response_ptr || !response_ptr->payload_ptr) {
        tr_debug("nmkp provisioning failed - no response");
        return -1;
    }
    if (thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_NETWORK_MASTER_KEY, NULL) < 16 ||
            thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_XPANID, NULL) < 8 ||
            thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_NETWORK_NAME, NULL) == 0 ||
            thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_CHANNEL, NULL) == 0 ||
            thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_PANID, NULL) == 0 ||
            thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_PSKC, NULL) < 16) {
        // Absolutely minimum we must have is master secret to attach.
        // If commissioner wants to be connected we must have PSKc,Name,Xpanid
        // If there is some fields missing we could attach, but timestamp must be set to 0 which will then cause synchronization
        tr_error("Not include all TLv's %s", trace_array(response_ptr->payload_ptr, response_ptr->payload_len));
        return -1;
    }

    thread_joiner_application_active_configuration_update(thread_ccm_find_id_by_service(service_id), response_ptr->payload_ptr, response_ptr->payload_len, meshcop_nmkp_resp_ignore, sizeof(meshcop_nmkp_resp_ignore));

    return 0;
}
static int thread_ccm_nmkp_commission_start(int8_t interface_id, uint8_t parent_address[16], uint16_t port, thread_commission_done_cb *done_cb)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!done_cb || !cur) {
        return -1;
    }

    thread_info(cur)->ccm_credentials_ptr->coap_service_secure_session_id = coap_service_initialize(cur->id, port, COAP_SERVICE_OPTIONS_EPHEMERAL_PORT | COAP_SERVICE_OPTIONS_SECURE | 0x80, thread_ccm_security_start_cb, NULL);

    if (cur->thread_info->ccm_credentials_ptr) {
        //Use certificate if set otherwise use PSKd
        if (0 > coap_service_certificate_set(thread_info(cur)->ccm_credentials_ptr->coap_service_secure_session_id, cur->thread_info->ccm_credentials_ptr->domain_certificate_ptr, cur->thread_info->ccm_credentials_ptr->domain_certificate_len,
                                             cur->thread_info->ccm_credentials_ptr->domain_pk_ptr, cur->thread_info->ccm_credentials_ptr->domain_pk_len)) {
            tr_debug("coap service certificate set failed");
            return -1;
        }
    }
    tr_debug("start nmkp commissioning: interface %d, parent: %s, port %d", interface_id, trace_ipv6(parent_address), port);

    thread_info(cur)->ccm_credentials_ptr->ccm_done_cb = done_cb;
    memcpy(thread_info(cur)->ccm_credentials_ptr->ccm_addr, parent_address, 16);
    thread_info(cur)->ccm_credentials_ptr->ccm_port = port;

    uint8_t *ptr, *data_ptr;
    uint16_t length;

    tr_debug("Thread joiner finalisation send");
    length = thread_joiner_application_device_configuration_length(thread_joiner_application_get_device_config(interface_id));

    data_ptr = ptr = ns_dyn_mem_alloc(length);
    if (!ptr) {
        tr_error("Failed to start Commissioning");
        return -1;
    }
    ptr = thread_joiner_application_device_configuration_build(ptr, thread_joiner_application_get_device_config(interface_id));

    coap_service_request_send(thread_info(cur)->ccm_credentials_ptr->coap_service_secure_session_id, COAP_REQUEST_OPTIONS_SECURE_BYPASS, parent_address, port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_BBR_NMKP_REQ, COAP_CT_OCTET_STREAM, data_ptr, ptr - data_ptr, thread_ccm_nmkp_response_cb);

    ns_dyn_mem_free(data_ptr);

    return 0;
}

/*
 * Allocate thread commercial commission mode (CCM) struct
 */
void thread_ccm_allocate(protocol_interface_info_entry_t *cur)
{
    if (thread_info(cur)->ccm_credentials_ptr) {
        return;
    }

    thread_info(cur)->ccm_credentials_ptr = ns_dyn_mem_alloc(sizeof(thread_ccm_credentials_t));
    if (!thread_info(cur)->ccm_credentials_ptr) {
        return;
    }
    memset(thread_info(cur)->ccm_credentials_ptr, 0, sizeof(thread_ccm_credentials_t));

    thread_info(cur)->ccm_credentials_ptr->interface_id = cur->id;

    ns_list_add_to_start(&ccm_instance_list, thread_info(cur)->ccm_credentials_ptr);
}

/*
 * Free thread commercial commission mode (CCM) struct
 */

void thread_ccm_free(protocol_interface_info_entry_t *cur)
{
    if (!cur || !thread_info(cur) || !thread_info(cur)->ccm_credentials_ptr) {
        return;
    }

    if (thread_info(cur)->ccm_credentials_ptr->domain_certificate_ptr) {
        ns_dyn_mem_free(thread_info(cur)->ccm_credentials_ptr->domain_certificate_ptr);
    }
    if (thread_info(cur)->ccm_credentials_ptr->domain_pk_ptr) {
        ns_dyn_mem_free(thread_info(cur)->ccm_credentials_ptr->domain_pk_ptr);
    }
    ns_list_remove(&ccm_instance_list, thread_info(cur)->ccm_credentials_ptr);
    ns_dyn_mem_free(thread_info(cur)->ccm_credentials_ptr);
    thread_info(cur)->ccm_credentials_ptr = NULL;
}

int thread_ccm_device_certificate_set(protocol_interface_info_entry_t *cur, const unsigned char *device_certificate_ptr, uint16_t device_certificate_len, const unsigned char *priv_key_ptr, uint16_t priv_key_len)
{

    if (!thread_info(cur)->ccm_credentials_ptr) {
        thread_ccm_allocate(cur);
    }

    thread_info(cur)->ccm_credentials_ptr->device_certificate_ptr = device_certificate_ptr;
    thread_info(cur)->ccm_credentials_ptr->device_certificate_len = device_certificate_len;
    thread_info(cur)->ccm_credentials_ptr->device_pk_ptr = priv_key_ptr;
    thread_info(cur)->ccm_credentials_ptr->device_pk_len = priv_key_len;

    return 0;
}

bool thread_ccm_network_certificate_available(protocol_interface_info_entry_t *cur)
{
    if (!thread_info(cur)->ccm_credentials_ptr || !thread_info(cur)->ccm_credentials_ptr->domain_certificate_ptr) {
        return false;
    }

    return true;
}

int thread_ccm_network_certificate_set(protocol_interface_info_entry_t *cur, const unsigned char *domain_certificate_ptr, uint16_t domain_certificate_len)
{
    if (!thread_info(cur)->ccm_credentials_ptr) {
        thread_ccm_allocate(cur);
    }

    if (thread_info(cur)->ccm_credentials_ptr->domain_certificate_ptr) {
        ns_dyn_mem_free(thread_info(cur)->ccm_credentials_ptr->domain_certificate_ptr);
        thread_info(cur)->ccm_credentials_ptr->domain_certificate_ptr = NULL;
        thread_info(cur)->ccm_credentials_ptr->domain_certificate_len = 0;
    }

    /* Set domain certificate pointer and length */
    if (domain_certificate_ptr) {
        thread_info(cur)->ccm_credentials_ptr->domain_certificate_ptr = ns_dyn_mem_alloc(domain_certificate_len + thread_info(cur)->ccm_credentials_ptr->domain_ca_certificate_len);
        if (!thread_info(cur)->ccm_credentials_ptr->domain_certificate_ptr) {
            ns_dyn_mem_free(thread_info(cur)->ccm_credentials_ptr);
            return -1;
        }
        memcpy(thread_info(cur)->ccm_credentials_ptr->domain_certificate_ptr, domain_certificate_ptr, domain_certificate_len);
        if (thread_info(cur)->ccm_credentials_ptr->domain_ca_certificate_ptr) {
            memcpy(thread_info(cur)->ccm_credentials_ptr->domain_certificate_ptr + domain_certificate_len, thread_info(cur)->ccm_credentials_ptr->domain_ca_certificate_ptr, thread_info(cur)->ccm_credentials_ptr->domain_ca_certificate_len);
        }
        thread_info(cur)->ccm_credentials_ptr->domain_certificate_len = domain_certificate_len + thread_info(cur)->ccm_credentials_ptr->domain_ca_certificate_len;
    }

    return 0;
}

int thread_ccm_network_private_key_set(protocol_interface_info_entry_t *cur, const unsigned char *priv_key_ptr, uint16_t priv_key_len)
{
    if (!thread_info(cur)->ccm_credentials_ptr) {
        thread_ccm_allocate(cur);
    }

    if (thread_info(cur)->ccm_credentials_ptr->domain_pk_ptr) {
        ns_dyn_mem_free(thread_info(cur)->ccm_credentials_ptr->domain_pk_ptr);
        thread_info(cur)->ccm_credentials_ptr->domain_pk_ptr = NULL;
        thread_info(cur)->ccm_credentials_ptr->domain_pk_len = 0;
    }

    if (priv_key_ptr) {
        thread_info(cur)->ccm_credentials_ptr->domain_pk_ptr = ns_dyn_mem_alloc(priv_key_len);
        if (!thread_info(cur)->ccm_credentials_ptr->domain_pk_ptr) {
            ns_dyn_mem_free(thread_info(cur)->ccm_credentials_ptr->domain_certificate_ptr);
            ns_dyn_mem_free(thread_info(cur)->ccm_credentials_ptr);
            return -1;
        }

        memcpy(thread_info(cur)->ccm_credentials_ptr->domain_pk_ptr, priv_key_ptr, priv_key_len);
        thread_info(cur)->ccm_credentials_ptr->domain_pk_len = priv_key_len;
    }

    return 0;
}

int thread_ccm_thread_name_set(protocol_interface_info_entry_t *cur, char thread_name[16])
{
    if (!thread_info(cur)->ccm_credentials_ptr) {
        thread_info(cur)->ccm_credentials_ptr = ns_dyn_mem_alloc(sizeof(thread_ccm_credentials_t));
        if (!thread_info(cur)->ccm_credentials_ptr) {
            return -1;
        }
        memset(thread_info(cur)->ccm_credentials_ptr, 0, sizeof(thread_ccm_credentials_t));
    }
    memset(thread_info(cur)->ccm_credentials_ptr->domain_name, 0, 16);
    memcpy(thread_info(cur)->ccm_credentials_ptr->domain_name, thread_name, 16);

    return 0;
}

int thread_ccm_commission_start(protocol_interface_info_entry_t *cur, uint8_t parent_address[16], discovery_response_list_t *nwk_info, thread_commission_done_cb *done_cb)
{

    if (thread_info(cur)->version >= THREAD_VERSION_1_2 && thread_info(cur)->ccm_credentials_ptr) {
        if (thread_info(cur)->ccm_credentials_ptr->domain_certificate_ptr && nwk_info->ccm_info.nmk_port) {
            return thread_ccm_nmkp_commission_start(cur->id, parent_address, nwk_info->ccm_info.nmk_port, done_cb);
        } else if (thread_info(cur)->ccm_credentials_ptr->device_certificate_ptr && nwk_info->ccm_info.ae_port) {
            return thread_ccm_ae_commission_start(cur->id, parent_address, nwk_info->ccm_info.ae_port, done_cb);
        } else if (nwk_info->joiner_port) {
            return thread_joiner_application_pskd_commission_start(cur->id, parent_address, nwk_info->joiner_port, nwk_info->pan_id, nwk_info->extented_pan_id, nwk_info->channel, done_cb);
        } else {
            return -1;
        }
    }
    return -1;
}

discovery_response_list_t *thread_ccm_network_select(protocol_interface_info_entry_t *cur, thread_nwk_discovery_response_list_t *discover_response)
{
    discovery_response_list_t *discovered_network_ptr = NULL;

    if (thread_info(cur)->version >= THREAD_VERSION_1_2 && thread_info(cur)->ccm_credentials_ptr) {
        /* If we have domain certificate, search for domain to join */
        if (thread_info(cur)->ccm_credentials_ptr->domain_certificate_ptr) {
            ns_list_foreach_safe(discovery_response_list_t, cur_class, discover_response) {
                if ((stringlen((const char *) cur_class->ccm_info.domain_name, 16)) &&
                        (!memcmp(cur_class->ccm_info.domain_name, thread_info(cur)->ccm_credentials_ptr->domain_name, stringlen((const char *) cur_class->ccm_info.domain_name, 16)))) {
                    discovered_network_ptr = cur_class;
                    break;
                }
            }
        }

        /* No domain, use device certificate*/
        if (!discovered_network_ptr) {
            ns_list_foreach_safe(discovery_response_list_t, cur_class, discover_response) {
                if (cur_class->ccm_info.ccm_supported) {
                    discovered_network_ptr = cur_class;
                    break;
                }
            }
        }
    }

    return discovered_network_ptr;
}

uint8_t thread_ccm_thread_name_length_get(protocol_interface_info_entry_t *cur)
{
    if (!thread_info(cur)->ccm_credentials_ptr) {
        return 0;
    }

    return stringlen((const char *)thread_info(cur)->ccm_credentials_ptr->domain_name, 16);
}

uint8_t *thread_ccm_thread_name_ptr_get(protocol_interface_info_entry_t *cur)
{
    if (!thread_info(cur)->ccm_credentials_ptr) {
        return NULL;
    }

    return thread_info(cur)->ccm_credentials_ptr->domain_name;
}

int thread_ccm_network_certificate_enable(protocol_interface_info_entry_t *cur, int8_t coap_service_id)
{
    // SET certificates
    if (cur->thread_info->ccm_credentials_ptr && cur->thread_info->ccm_credentials_ptr->domain_certificate_ptr) {
        if (0 > coap_service_certificate_set(coap_service_id, cur->thread_info->ccm_credentials_ptr->domain_certificate_ptr, cur->thread_info->ccm_credentials_ptr->domain_certificate_len,
                                             cur->thread_info->ccm_credentials_ptr->domain_pk_ptr, cur->thread_info->ccm_credentials_ptr->domain_pk_len)) {
            tr_debug("pBBR certificate set failed");
            return -1;
        }
    }
    return 0;
}


static int thread_ccm_reenroll_resp_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    (void) service_id;
    (void) source_address;
    (void) source_port;
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(thread_ccm_find_id_by_service(service_id));

    // Close secure connection
    coap_service_close_secure_connection(service_id, source_address, source_port);

    if (!response_ptr || !cur) {
        tr_debug("re-enroll failed");
        return -1;
    }

    tr_debug("re-enroll resp len %d", response_ptr->payload_len);

    // todo:check new certificate
    // Update certificate
    if (0 == thread_ccm_enroll_parse(cur, response_ptr->payload_ptr, response_ptr->payload_len)) {
        // start NMKP with new certificates
        thread_ccm_network_reattach(service_id, 5000, true);
    }

    return 0;
}
static int thread_ccm_reenroll_csrattrs_resp_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    (void) service_id;

    if (!response_ptr) {
        tr_debug("No response to re-enroll csrattr req");
        return -1;
    }

    coap_service_request_send(service_id, COAP_REQUEST_OPTIONS_SECURE_BYPASS, source_address, source_port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_SIMPLEREENROLL, THREAD_CONTENT_FORMAT_PKCS10,
                              csr_request, sizeof(csr_request), thread_ccm_reenroll_resp_cb);

    return 0;

}

static int thread_ccm_reenroll_registrar_addr_resp_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    (void) service_id;
    (void) source_address;
    (void) source_port;

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(thread_management_server_interface_id_get(service_id));
    uint8_t *addr_ptr = NULL;

    if (!response_ptr) {
        tr_debug("No response to registrar addr get");
        return -1;
    }

    if (!thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_REGISTRAR_IPV6_ADDRESS, &addr_ptr)) {
        tr_debug("Registrar addr get failed");
        return -1;
    }

    tr_debug("Received registrar address %s", trace_ipv6(addr_ptr));

    //todo: what port?
    thread_info(cur)->ccm_credentials_ptr->coap_service_secure_session_id = coap_service_initialize(cur->id, 5684, COAP_SERVICE_OPTIONS_EPHEMERAL_PORT | COAP_SERVICE_OPTIONS_SECURE, thread_ccm_security_start_cb, NULL);

    if (cur->thread_info->ccm_credentials_ptr) {
        //Use certificate if set otherwise use PSKd
        if (0 > coap_service_certificate_set(thread_info(cur)->ccm_credentials_ptr->coap_service_secure_session_id, cur->thread_info->ccm_credentials_ptr->domain_certificate_ptr, cur->thread_info->ccm_credentials_ptr->domain_certificate_len,
                                             cur->thread_info->ccm_credentials_ptr->domain_pk_ptr, cur->thread_info->ccm_credentials_ptr->domain_pk_len)) {
            tr_debug("coap service certificate set failed");
            return -1;
        }
    }

    coap_service_request_send(thread_info(cur)->ccm_credentials_ptr->coap_service_secure_session_id, COAP_REQUEST_OPTIONS_SECURE_BYPASS, addr_ptr, THREAD_DEFAULT_REGISTRAR_PORT, // TBD: get port from somewhere
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_GET, THREAD_URI_CSRATTRS, THREAD_CONTENT_FORMAT_CSRATTRS, NULL, 0, thread_ccm_reenroll_csrattrs_resp_cb);

    return 0;
}

int thread_ccm_reenrollment_start(protocol_interface_info_entry_t *cur, int8_t service_id, uint8_t *pbbr_addr)
{
    (void) cur;
    uint8_t get_tlv[3] = {0};
    uint8_t *ptr = get_tlv;
    uint8_t type = MESHCOP_TLV_REGISTRAR_IPV6_ADDRESS;

    //Payload = get tlv including registrar IPV6 address TLV (68)
    ptr = thread_meshcop_tlv_data_write(get_tlv, MESHCOP_TLV_GET, 1, &type);

    // Send MGMT_BBR_GET.req message to the Primary BBR, requesting the Registrar IPv6 Address TLV
    coap_service_request_send(service_id, COAP_REQUEST_OPTIONS_NONE, pbbr_addr, THREAD_MANAGEMENT_PORT, COAP_MSG_TYPE_CONFIRMABLE,
                              COAP_MSG_CODE_REQUEST_GET, THREAD_URI_BBR_DATA_REQ, COAP_CT_NONE, get_tlv, ptr - get_tlv, thread_ccm_reenroll_registrar_addr_resp_cb);

    return 0;
}

#endif /* HAVE_THREAD_V2 */
