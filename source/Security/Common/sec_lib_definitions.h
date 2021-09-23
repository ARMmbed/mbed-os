/*
 * Copyright (c) 2014-2019, Pelion and affiliates.
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

#ifndef SEC_LIB_DEFINITIONS_H_
#define SEC_LIB_DEFINITIONS_H_

#include "ns_sha256.h"

#define SEC_CIPHERSUITE_PSK 1
#define SEC_CIPHERSUITE_ECC 2

#define SEC_DEFAULT_SUPPORTED_CHIPHER_SUITES SEC_CIPHERSUITE_PSK

//Pana & TLS Auhentication Parameters


#define AUTHENTICATION_DONE     0x80
#define AUTHENTICATION_FAIL     0
#define AUTHENTICATION_CLOSE    1
#define TLS_SERVER_MODE 0x10
#define TLS_HANSHAKE_HASH 0x08

#define TLS_ECC_CERTIFICATE_NO_REQUEST   0
#define TLS_ECC_CERTIFICATE_REQUESTED    0x04
#define TLS_ECC_CERTIFICATE_RECEIVED     0x40
#define TLS_ECC_CERTIFICATE_VERIFY       0x80

#define PANA_KEY_UPDATE_DEFAULT_TIMEOUT 120000

#define SOCKET_ID_MASK  0x03
#define TLS_CIPHER_NONE 0

#define TLS_CERT_TYPE_ECDSA             64


typedef enum {
    CHIPHER_NONE = 0,
    CHIPHER_PSK,
    CHIPHER_ECC
} tls_chipher_mode_t;

typedef struct auth_info {
    uint8_t network_key[16];
    uint8_t key_id;
} auth_info_t;


#define ALERT_CLOSE 0
#define ALERT_BAD_RECORD    20
#define ALERT_INTERNAL_ERR  80
#define ALERT_BAD_CERTIFICATE 42


/****************************************
 * TLS BASE
 *
 * Type     Version     Length
 *
 *  8-bit   0x03 0x03   16-bit
 *
 *
 */

#define PANA_ST_IDLE 0
#define PANA_ST_READY 1
#define PANA_ST_KEY_WRAP 2
#define PANA_ST_SERVER  4

typedef struct {
    uint8_t security_support;
    uint16_t pan_id;
    uint32_t psk_key_id;
} pana_tls_setup_s;

#define CLIENT_HELLO_PTR 0
#define SERVER_HELLO_PTR 32

/* HandshakeType */
#define TLS_CLIENT_HELLO                1
#define TLS_SERVER_HELLO                2
#define TLS_CERTIFICATE                 11
#define TLS_SERVER_KEY_EXCHANGE         12
#define TLS_CERTIFICATE_REQUEST         13
#define TLS_SERVER_HELLO_DONE           14
#define TLS_CERTIFICATE_VERIFY          15
#define TLS_CLIENT_KEY_EXCHANGE         16
#define TLS_FINISHED                    20

/* ContentType */
#define TLS_CHANGE_CIPHER_SPEC          20
#define TLS_ALERT_TYPE                  21
#define TLS_HANDSHAKE                   22
#define TLS_APP_DATA                    23

#define TLS_1_2_VERSION                 0x0303

#define TLS_PSK_WITH_AES_128_CCM_8                  0xc0a8  // RFC 6655
#define TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8          0xc0ae  // RFC 7251
#define TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8_COMPAT   0xc0c6  // Non-standard, from ZigBee IP

#define TLS_COMPRESSION_METHOD_NULL         0

#define TLS_EXTENSION_ELLIPTIC_CURVES       10
#define TLS_EXTENSION_EC_POINT_FORMATS      11
#define TLS_EXTENSION_SIGNATURE_ALGORITHMS  13

#define TLS_HASH_SHA256                     4
#define TLS_SIGNATURE_ECDSA                 3
#define TLS_SIG_HASH_ALG_SHA256_ECDSA       ((TLS_HASH_SHA256 << 8) | TLS_SIGNATURE_ECDSA)

#define TLS_EC_CURVE_TYPE_NAMED_CURVE       3

#define TLS_NAMED_CURVE_SECP256R1           23

#ifdef ECC
#include "GMPNlib.h"
#include "GELAlib.h"
#include "ecdsa.h"
#include "libX509_V3.h"

typedef struct {
    EllipticPoint cert_pub_key;     // 80 bytes
    uint8_t client_public_key[64];  // Pk Client: client_public_key[0-31] X point, client_public_key[32-63] y point remember change byte order
    uint8_t server_public_key[64];  // Pk server server_public_key[0-31] X point, server_public_key[32-63] y point remember change byte order
    uint8_t pre_secret_mat[32];     // Client Pk server * k*curve and server Pk client*k*curve only x point
    ECDSASignature *sgnt;
    MPint private_key;
} tls_ecc_heap_t;
#endif

typedef struct {
#ifdef ECC
    tls_ecc_heap_t *ecc_heap;
#endif
    uint8_t temp_buf[64];               // len 64 bytes
    uint8_t verify[16];             // len 16 bytes
    uint8_t tls_hello_random[64];
    uint8_t hash_buf[32];           //32 bytes
    uint16_t tls_handshake_h_len;
    uint16_t client_verify_buf_len;
    uint8_t *client_verify_buf;
    tls_chipher_mode_t tls_chipher_mode;
    ns_sha256_context sha256_ctx;
#ifdef ECC
    certificate_chain_t rx_ceri_chain;
    bool client_knows_standard_ecc_ciphersuite;
#endif /*ECC*/
    uint8_t pointer_types;
    void *cert_temp_buf;
    void *signature_temp_buf;
    uint8_t *key_signature_ptr;
} tls_heap_t;

typedef struct {
    uint8_t type;
    uint16_t length;
    uint8_t *ptr;
} tls_header_t;

typedef struct {
    uint16_t type;
    uint16_t len;
    uint8_t *msg_ptr;
} tls_msg_t;


typedef enum {
    PANA_INIT = 0,
    PANA_PCI_TX,
    PANA_REQUEST_TX,
    PANA_START_RESPONSE,
    PANA_PING_REQ,
    PANA_KEY_PULL,
    PANA_RE_VALID,
    PANA_PULL_DONE,
    PANA_READY,
    PANA_KEY_UPDATE, /*10*/
    PANA_FAILURE,
    PANA_FAILURE_RESPONSE,
    EAP_IDENTITY_REQ,
    EAP_IDENTITY_RES,
    EAP_FINISH,
    EAP_PANA_FINISH,
    TLS_START,
    TLS_INIT,
    PRF_CALC,
    PRF_CALC2, /*20*/
    TLS_ECC_CERTIFICATE_VERIFY_SIGNATURE,
    TLS_ECC_CERTIFICATE_SIGNATURE_CHECK,
    TLS_ECC_MESSAGE_VERIFY,
    TLS_ECC_MESSAGE_VERIFY_START,
    TLS_ECC_MESSAGE_VERIFY_START2,
    TLS_ECC_MESSAGE_SERVER_VERIFY_START,
    TLS_ECC_GENERATE_PUBLIC_KEY,
    TLS_ECC_GENERATE_PUBLIC_KEY_START,
    TLS_ECC_GENERATE_PREMASTER_SECRET,
    TLS_ECC_SIGNATURE_MESSAGE,/*30*/
    TLS_ECC_CLIENT_SIGNATURE_START,
    TLS_KEY_CHANGE,
    TLS_CHANGE_CHIPHER,
    TLS_HELLO_DONE,
    TLS_FINNISH_RX,
    TLS_FINISH,
    TLS_EAP_END_PANA_VERIFY,
    TLS_ALERT,
    TLS_ALERT_INTERNAL,
    TLS_ALERT_CHIPHER_SUITE,/*40*/
    TLS_ALERT_DECRYPT,
    TLS_ALERT_BAD_CERTIFICATE,
    TLS_ALERT_CLOSE_FATAL,
    TLS_SERVER_TX_SERVER_HELLO,
    TLS_UPDATE_HAS_WITH_CERTIFICATE,
    TLS_TX_SERVER_KEY_EXCHANGE,
    TLS_SERVER_ECC_PUB_KEY_GEN,
    TLS_SERVER_TX_CHANGE_CIPHERSUITE,
    TLS_CLIENT_TX_CERTIFICATE_VERIFY,
    TLS_SERVER_WAIT_CHANGE_CHIPHERSUITE,/*50*/
    TLS_SERVER_WAIT_CLIENT_HELLO,
    TLS_CERTIFICATE_RX,
    TLS_SERVER_KEY_EXCHANGE_RX,
    TLS_CLIENT_KEY_EXCHANGE_RX,
    TLS_HELLO_RX,
    PANA_ERROR
} sec_state_machine_t;

typedef struct {
    uint8_t pana_handshake[120];// 80 bytes
    uint16_t handshake_req_offset;
    uint16_t handshake_len;
    uint8_t client_nonce[16];
    uint8_t agent_nonce[16];
    /* Pana MSK */
    uint8_t MSK[64];        // calculated only ones / session
} pana_heap_t;

typedef struct {
    uint8_t key_id;
    uint8_t new_key_material[16];
    uint16_t key_delivery_cnt;
} pana_key_update_t;

typedef struct {
    uint8_t key_material[16];
    uint8_t key_id;
} pana_key_material_t;


typedef struct {
    uint8_t type;
    uint16_t length;
    uint16_t offset;
    uint8_t *ptr;
} tls_record_segment_t;

typedef struct {
    int8_t network_interface_id;
    uint32_t pana_key_id;
    uint32_t session_lifetime;
    uint32_t key_wrap;
    uint32_t prf_algorythm;
    uint32_t integrity_algorythm;
    uint32_t next_session_id;
    pana_key_material_t sec_key_material[2];
    uint8_t primary_material;
    uint8_t supported_chipher_suites;
    pana_key_update_t *pana_key_update;
    uint32_t key_update_delay;
    uint8_t auth_cnt;
    uint8_t open_pana_authentication_cnt;
    uint32_t pana_key_update_delay_used;
} pana_server_base_t;


typedef struct pana_session_t {
    uint16_t nvm_offset;
    /* Pana Session Key */
    uint8_t pana_auth_key[32]; //calculated only ones /session
    /* Pana Encryption key */
    uint8_t pana_PAA_enc_key[16]; // calculated only ones /session
    //uint8_t pana_PAC_enc_key[16]; // calculated only ones /session
    uint32_t pana_key_id;
    uint32_t session_id;
    uint32_t req_seq;
    uint32_t res_seq;
    uint32_t session_lifetime;

    uint32_t key_wrap;
    uint32_t prf_algorythm;
    uint32_t integrity_algorythm;
    /* EAP Segment */

    buffer_t *eap_frag_buf;
    uint16_t frag_off_set;
    uint16_t frag_length;

    buffer_t *eap_assy_buf;
    uint16_t assy_off_set;
    uint16_t assy_length;
    uint16_t last_assy_size;
    uint8_t eap_id_seq;
    uint8_t auth_cnt;
    uint8_t nwk_key_id;
    bool session_ready: 1;
    bool key_warp: 1;
    bool user_server: 1;
    bool packet_delivered: 1;
    /* Define Relay usage */
    uint8_t address_status;
    uint8_t session_relay_address[16];
    uint16_t relay_port;

    pana_heap_t *pana_heap;
    auth_info_t *auth_info;
} pana_session_t;

typedef struct tls_session_t {
    uint8_t key_expansion[64];
    uint8_t id_length;
    uint8_t tls_session_id[32];
    uint8_t tls_nonce_explit[8];
    uint8_t temp_buf[5];
    uint8_t master_secret[48];  //len 48 bytes
    tls_heap_t *tls_heap;
} tls_session_t;

typedef struct sec_suite_t {
    pana_session_t pana_session;
    tls_session_t *tls_session;
    protocol_interface_info_entry_t *interface;
    sec_state_machine_t state;
    uint8_t retry_counter;
    uint8_t supported_chipher_suites;
    uint16_t setups;
    uint16_t session_port;
    uint32_t timer;
    int32_t psk_key_id;
    uint8_t session_address[16];
    uint16_t pan_id;
    ns_list_link_t link;
} sec_suite_t;

#endif /* SEC_LIB_DEFINITIONS_H_ */
