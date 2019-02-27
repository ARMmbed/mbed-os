/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#ifndef TLS_SEC_PROT_LIB_H_
#define TLS_SEC_PROT_LIB_H_

/*
 * TLS security protocol library to connect to mbed TLS
 *
 */

typedef struct tls_security_s tls_security_t;

typedef enum {
    TLS_SEC_PROT_LIB_ERROR = -2,
    TLS_SEC_PROT_LIB_NO_DATA = -1,
    TLS_SEC_PROT_LIB_CONTINUE = 0,
    TLS_SEC_PROT_LIB_CALCULATING,
    TLS_SEC_PROT_LIB_HANDSHAKE_OVER,
} tls_sec_prot_lib_ret_e;

typedef enum {
    TLS_SEC_PROT_LIB_TIMER_CANCELLED = -1,
    TLS_SEC_PROT_LIB_TIMER_NO_EXPIRY = 0,
    TLS_SEC_PROT_LIB_TIMER_INT_EXPIRY = 1,
    TLS_SEC_PROT_LIB_TIMER_FIN_EXPIRY = 2,
} tls_sec_prot_lib_timer_e;

// Maximum operations made on one round of ECC calculation
#define ECC_CALCULATION_MAX_OPS            200

/**
 * tls_sec_prot_lib_init initialize security library
 *
 * \param sec security library instance
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t tls_sec_prot_lib_init(tls_security_t *sec);

/**
 * tls_sec_prot_lib_size get security library instance size
 *
 * \return size
 */
uint16_t tls_sec_prot_lib_size(void);

/**
 * tls_sec_prot_lib_send send data callback
 *
 * \param handle caller defined handle
 * \param buf buffer to be send
 * \param len length of the buffer
 *
 * \return length of the send data
 */
typedef int16_t tls_sec_prot_lib_send(void *handle, const void *buf, size_t len);

/**
 * tls_sec_prot_lib_receive receive data callback
 *
 * \param handle caller defined handle
 * \param buf receive buffer
 * \param len receive buffer length
 *
 * \return length of the received data written to receive buffer
 * \return TLS_SEC_PROT_LIB_NO_DATA no more data received
 */
typedef int16_t tls_sec_prot_lib_receive(void *handle, unsigned char *buf, size_t len);

/**
 * tls_sec_prot_lib_set_timer set timer callback
 *
 * \param handle caller defined handle
 * \param inter intermediate timeout
 * \param fin final timeout
 *
 */
typedef void tls_sec_prot_lib_set_timer(void *handle, uint32_t inter, uint32_t fin);

/**
 * tls_sec_prot_lib_get_timer get timer callback
 *
 * \param handle caller defined handle
 *
 * \return TLS_SEC_PROT_LIB_TIMER_CANCELLED timer cancelled
 * \return TLS_SEC_PROT_LIB_TIMER_FIN_EXPIRY final timeout has expired
 * \return TLS_SEC_PROT_LIB_TIMER_INT_EXPIRY intermediate timeout has expired
 * \retunt TLS_SEC_PROT_LIB_TIMER_NO_EXPIRY timer has not expired
 *
 */
typedef int8_t tls_sec_prot_lib_get_timer(void *handle);

/**
 * tls_sec_prot_lib_export_keys export key material after handshake is completed
 *
 * \param handle caller defined handle
 * \param master_secret TLS master secret, 48 bytes
 * \param eap_tls_key_material EAP TLS key material, 128 bytes
 *
 */
typedef void tls_sec_prot_lib_export_keys(void *handle, const uint8_t *master_secret, const uint8_t *eap_tls_key_material);

/**
 * tls_sec_prot_lib_set_cb_register register callbacks to library
 *
 * \param sec security library instance
 * \param handle caller defined handle
 * \param send send data callback
 * \param receive receive data callback
 * \param export_keys export keys callback
 * \param set_timer set timer callback
 * \param get_timer get timer callback
 *
 */
void tls_sec_prot_lib_set_cb_register(tls_security_t *sec, void *handle,
                                      tls_sec_prot_lib_send *send, tls_sec_prot_lib_receive *receive,
                                      tls_sec_prot_lib_export_keys *export_keys, tls_sec_prot_lib_set_timer *set_timer,
                                      tls_sec_prot_lib_get_timer *get_timer);

/**
 * tls_sec_prot_lib_free free security library internal data (e.g. TLS data)
 *
 * \param sec security library instance
 *
 */
void tls_sec_prot_lib_free(tls_security_t *sec);

/**
 * tls_sec_prot_lib_connect start TLS handshake
 *
 * \param sec security library instance
 * \param is_server TRUE if TLS server, FALSE for TLS client
 * \param certs certificates
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t tls_sec_prot_lib_connect(tls_security_t *sec, bool is_server, const sec_prot_certs_t *certs);

/**
 * tls_sec_prot_lib_process process TLS (call e.g. after incoming message)
 *
 * \param sec Security library instance
 *
 * \return TLS_SEC_PROT_LIB_ERROR failure, failure, stop TLS negotiation
 * \return TLS_SEC_PROT_LIB_CONTINUE continue processing (send output message)
 * \return TLS_SEC_PROT_LIB_CALCULATING calculation ongoing, call process again
 * \return TLS_SEC_PROT_LIB_HANDSHAKE_OVER handshake completed successfully
 *
 */
int8_t tls_sec_prot_lib_process(tls_security_t *sec);

#endif /* TLS_SEC_PROT_LIB_H_ */
