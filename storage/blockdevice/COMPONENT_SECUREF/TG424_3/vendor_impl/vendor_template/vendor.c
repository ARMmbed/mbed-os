/*
 * Copyright (c) 2022-2023 Macronix International Co. LTD. All rights reserved.
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
#include "vendor.h"
#include "../../vendor_secureflash_defs.h"

extern crypto_wrapper_t vendor_crypto_wrapper;

static int32_t vendor_secureflash_pre_create_session(vendor_secureflash_context *secureflash_ctx,
                                                     uint8_t *nonce_in, uint32_t nonce_in_len,
                                                     jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_create_session_packet(vendor_secureflash_context *secureflash_ctx,
                                                        uint32_t root_key_id,
                                                        uint8_t *packet, uint32_t *packet_len,
                                                        crypto_indicator_t *indicator)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_post_create_session(vendor_secureflash_context *secureflash_ctx,
                                                      uint32_t root_key_id,
                                                      uint32_t *session_key_id,
                                                      uint8_t *nonce_in, uint32_t nonce_in_len,
                                                      jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

#if defined(SESSION_CONFIRMATION)
static int32_t vendor_secureflash_pre_confirm_session()
{
    //TODO
    return 0;
}
static int32_t vendor_secureflash_confirm_session_packet()
{
    //TODO
    return 0;
}
static int32_t vendor_secureflash_post_confirm_session()
{
    //TODO
    return 0;
}
#endif

static int32_t vendor_secureflash_pre_terminate_session(vendor_secureflash_context *secureflash_ctx,
                                                        uint8_t *nonce_in, uint32_t nonce_in_len,
                                                        jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_terminate_session_packet(vendor_secureflash_context *secureflash_ctx,
                                                           uint32_t session_key_id,
                                                           uint8_t *packet,uint32_t *packet_len,
                                                           crypto_indicator_t *indicator)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_post_terminate_session(vendor_secureflash_context *secureflash_ctx,
                                                         jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_pre_secure_init(vendor_secureflash_context *secureflash_ctx,
                                                  uint8_t *nonce_in, uint32_t nonce_in_len,
                                                  jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_secure_init_packet(vendor_secureflash_context *secureflash_ctx,
                                                     uint32_t root_key_id,
                                                     uint8_t *packet, uint32_t *packet_len,
                                                     crypto_indicator_t *indicator)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_post_secure_init(vendor_secureflash_context *secureflash_ctx,
                                                   jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_pre_secure_uninit(vendor_secureflash_context *secureflash_ctx,
                                                    uint8_t *nonce_in, uint32_t nonce_in_len,
                                                    jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_secure_uninit_packet(vendor_secureflash_context *secureflash_ctx,
                                                       uint32_t root_key_id,
                                                       uint8_t *packet, uint32_t *packet_len,
                                                       crypto_indicator_t *indicator)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_post_secure_uninit(vendor_secureflash_context *secureflash_ctx,
                                                     jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_pre_secure_program(vendor_secureflash_context *secureflash_ctx, uint32_t addr,
                                                     uint32_t session_key_id, jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_secure_program_packet(vendor_secureflash_context *secureflash_ctx, uint32_t addr,
                                                        const uint8_t *data, uint32_t len, uint32_t session_key_id,
                                                        uint8_t *packet, uint32_t *packet_len,
                                                        crypto_indicator_t *indicator)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_post_secure_program(vendor_secureflash_context *secureflash_ctx,
                                                      uint32_t session_key_id,
                                                      uint8_t *resp_packet_buffer, uint32_t *resp_packet_buffer_size,
                                                      crypto_indicator_t *indicator,
                                                      jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_parse_secure_program_response(vendor_secureflash_context *secureflash_ctx,
                                                                crypto_indicator_t *indicator,
                                                                uint32_t *bytes_programmed)
{
    //TODO
    return 0;
}

#if defined(SECURE_READ)
static int32_t vendor_secureflash_pre_secure_read(vendor_secureflash_context *secureflash_ctx, uint32_t addr,
                                                  uint32_t session_key_id, jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_secure_read_packet(vendor_secureflash_context *secureflash_ctx,
                                                     uint32_t addr, uint32_t len, uint32_t session_key_id,
                                                     uint8_t *packet, uint32_t *packet_len,
                                                     crypto_indicator_t *indicator)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_post_secure_read(vendor_secureflash_context *secureflash_ctx,
                                                   uint32_t session_key_id,
                                                   uint8_t *resp_packet_buffer,
                                                   uint32_t *resp_packet_buffer_size,
                                                   crypto_indicator_t *indicator,
                                                   jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_parse_secure_read_response(vendor_secureflash_context *secureflash_ctx,
                                                             crypto_indicator_t *indicator,
                                                             uint8_t *data, uint32_t data_len,
                                                             uint32_t *bytes_read)
{
    //TODO
    return 0;
}
#endif

static int32_t vendor_secureflash_pre_secure_erase(vendor_secureflash_context *secureflash_ctx,
                                                   uint32_t addr, uint32_t len, uint32_t session_key_id,
                                                   jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_secure_erase_packet(vendor_secureflash_context *secureflash_ctx,
                                                      uint32_t addr, uint32_t len, uint32_t session_key_id,
                                                      uint8_t *packet, uint32_t *packet_len,
                                                      crypto_indicator_t *indicator)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_post_secure_erase(vendor_secureflash_context *secureflash_ctx,
                                                    uint32_t session_key_id,
                                                    uint8_t *resp_packet_buffer,
                                                    uint32_t *resp_packet_buffer_size,
                                                    crypto_indicator_t *indicator,
                                                    jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_parse_secure_erase_response(vendor_secureflash_context *secureflash_ctx,
                                                              crypto_indicator_t *indicator)
{
    //TODO
    return 0;
}

#if defined(SECURE_COPY)
static int32_t vendor_secureflash_pre_secure_copy()
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_secure_copy_packet()
{
    //TODO
    return 0;
}
static int32_t vendor_secureflash_post_secure_copy()
{
    //TODO
    return 0;
}
#endif

static int32_t vendor_secureflash_pre_get_region_info(vendor_secureflash_context *secureflash_ctx,
                                                      uint32_t session_key_id, bool *session_key_valid_flag,
                                                      jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_get_region_info_packet(vendor_secureflash_context *secureflash_ctx,
                                                         uint32_t session_key_id, int8_t region_index,
                                                         uint8_t *nonce_in, uint32_t nonce_in_len,
                                                         uint8_t *packet, uint32_t *packet_len,
                                                         crypto_indicator_t *indicator)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_post_get_region_info(vendor_secureflash_context *secureflash_ctx,
                                                       region_ll_node_t *region_descr_p, jqueue_t *resp_queue)
{
    //TODO
    return 0;
}

#if defined(SECURE_REGION_MANAGE)
static int32_t vendor_secureflash_pre_secure_manage_region()
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_secure_manage_region_packet()
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_post_secure_manage_region()
{
    //TODO
    return 0;
}
#endif

static int32_t vendor_secureflash_packet_send(vendor_secureflash_context *secureflash_ctx,
                                              uint8_t *outdata, uint32_t outdata_len,
                                              uint8_t *cipher_text, uint32_t cipher_text_len,
                                              uint8_t *mac, uint32_t mac_len)
{
    //TODO
    return 0;
}

static int32_t vendor_secureflash_packet_receive(vendor_secureflash_context *secureflash_ctx,
                                                 uint8_t *indata, uint32_t indata_len)
{
    //TODO
    return 0;
}


vendor_security_op_t vendor_secureflash = {
    .vendor_id = VENOR_ID,
    .pre_create_session = vendor_secureflash_pre_create_session,
    .create_session_packet = vendor_secureflash_create_session_packet,
    .post_create_session = vendor_secureflash_post_create_session,
#if defined(SESSION_CONFIRMATION)
    .pre_confirm_session = vendor_secureflash_pre_confirm_session,
    .confirm_session_packet = vendor_secureflash_confirm_session_packet,
    .post_confirm_session = vendor_secureflash_post_confirm_session,
#endif
    .pre_terminate_session = vendor_secureflash_pre_terminate_session,
    .terminate_session_packet = vendor_secureflash_terminate_session_packet,
    .post_terminate_session = vendor_secureflash_post_terminate_session,
    .pre_secure_init = vendor_secureflash_pre_secure_init,
    .secure_init_packet = vendor_secureflash_secure_init_packet,
    .post_secure_init = vendor_secureflash_post_secure_init,
    .pre_secure_uninit = vendor_secureflash_pre_secure_uninit,
    .secure_uninit_packet = vendor_secureflash_secure_uninit_packet,
    .post_secure_uninit = vendor_secureflash_post_secure_uninit,

    .pre_secure_program = vendor_secureflash_pre_secure_program,
    .secure_program_packet = vendor_secureflash_secure_program_packet,
    .post_secure_program = vendor_secureflash_post_secure_program,
    .parse_secure_program_response = vendor_secureflash_parse_secure_program_response,

    .pre_secure_erase = vendor_secureflash_pre_secure_erase,
    .secure_erase_packet = vendor_secureflash_secure_erase_packet,
    .post_secure_erase = vendor_secureflash_post_secure_erase,
    .parse_secure_erase_response = vendor_secureflash_parse_secure_erase_response,
#if defined(SECURE_COPY)
    .pre_secure_copy = vendor_secureflash_pre_secure_copy,
    .secure_copy_packet = vendor_secureflash_secure_copy_packet,
    .post_secure_copy = vendor_secureflash_post_secure_copy,
#endif
#if defined(SECURE_READ)
    .pre_secure_read = vendor_secureflash_pre_secure_read,
    .secure_read_packet = vendor_secureflash_secure_read_packet,
    .post_secure_read = vendor_secureflash_post_secure_read,
    .parse_secure_read_response = vendor_secureflash_parse_secure_read_response,
#endif
    .pre_secure_get_regions_info = vendor_secureflash_pre_get_region_info,
    .secure_get_regions_info_packet = vendor_secureflash_get_region_info_packet,
    .post_secure_get_regions_info = vendor_secureflash_post_get_region_info,
#if defined(SECURE_REGION_MANAGE)
    .pre_secure_manage_region = vendor_secureflash_pre_secure_manage_region,
    .secure_manage_region_packet = vendor_secureflash_secure_manage_region_packet,
    .post_secure_manage_region = vendor_secureflash_post_secure_manage_region,
#endif
    .packet_send = vendor_secureflash_packet_send,
    .packet_receive = vendor_secureflash_packet_receive,
};

const secure_flash_info_t vendor_secure_flash_info = {
    .id = {0xff, 0xff, 0xff},
    .id_len = 3,
    .vendor_security_op = &vendor_secureflash,
    .crypto_wrapper = &vendor_crypto_wrapper,
};
