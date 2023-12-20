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
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "jedec_security_hal.h"
#include "include/error.h"

static jedec_ctx_t ctx = {};

jedec_error_t jedec_set_vendor(vendor_security_op_t *vendor,
                               crypto_wrapper_t *crypto_wrapper,
                               void *vendor_ctx)
{
    ctx.vendor_security_op = vendor;
    ctx.crypto_wrapper = crypto_wrapper,
    ctx.vendor_ctx = vendor_ctx;
}

jedec_error_t jedec_secure_init(uint32_t auth_key_id)
{
    jedec_error_t err = JEDEC_ERROR_NONE;
    crypto_indicator_t indicator = {};
    uint8_t random[MAX_RANDOM_SIZE];

    if (ctx.is_initialized) {
        return JEDEC_ERROR_NONE;
    }
    /* Check whether auth_key_id is valid */
    if (auth_key_id == INVALID_KEY_ID || ctx.vendor_security_op == NULL ||
        ctx.crypto_wrapper == NULL || ctx.vendor_ctx == NULL) {
        return JEDEC_ERROR_INV_ARGS;
    }
    err = ctx.crypto_wrapper->open_key(auth_key_id);
    if (err != JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_KEY_OPERATION;
    }
    queue_clear(&ctx.q);
    err = ctx.crypto_wrapper->generate_random(random, MAX_RANDOM_SIZE);
    if (err != JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_RANDOM_GEN;
    }
    err = ctx.vendor_security_op->pre_secure_init(ctx.vendor_ctx,
                                                  random, MAX_RANDOM_SIZE,
                                                  &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.vendor_security_op->secure_init_packet(ctx.vendor_ctx, auth_key_id,
                                                     ctx.packet, &ctx.packet_len,
                                                     &indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.crypto_wrapper->crypto_func(&indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.vendor_security_op->packet_send(ctx.vendor_ctx,
                                              ctx.packet, ctx.packet_len,
                                              NULL,
                                              0,
                                              indicator.hmac.mac,
                                              indicator.hmac.mac_len);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.vendor_security_op->post_secure_init(ctx.vendor_ctx, &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Verify response from Flash */
    if (queue_verify(ctx.crypto_wrapper, &ctx.q) == false) {
        return JEDEC_ERROR_AUTH_FAIL;
    }
    ctx.is_initialized = true;
    return JEDEC_ERROR_NONE;
}

jedec_error_t jedec_secure_uninit(uint32_t auth_key_id)
{
    jedec_error_t err = JEDEC_ERROR_NONE;
    crypto_indicator_t indicator = {};
    uint8_t random[MAX_RANDOM_SIZE];

    if (!ctx.is_initialized) {
        return JEDEC_ERROR_NONE;
    }
    if (auth_key_id == INVALID_KEY_ID) {
        return JEDEC_ERROR_INV_ARGS;
    }
    queue_clear(&ctx.q);
    err = ctx.crypto_wrapper->generate_random(random, MAX_RANDOM_SIZE);
    if (err != JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_RANDOM_GEN;
    }
    err = ctx.vendor_security_op->pre_secure_uninit(ctx.vendor_ctx,
                                                    random, MAX_RANDOM_SIZE,
                                                    &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.vendor_security_op->secure_uninit_packet(ctx.vendor_ctx,
                                                       auth_key_id,
                                                       ctx.packet, &ctx.packet_len,
                                                       &indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.crypto_wrapper->crypto_func(&indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.vendor_security_op->packet_send(ctx.vendor_ctx,
                                              ctx.packet, ctx.packet_len,
                                              NULL, 0,
                                              indicator.hmac.mac,
                                              indicator.hmac.mac_len);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    queue_clear(&ctx.q);
    err = ctx.vendor_security_op->post_secure_uninit(ctx.vendor_ctx, &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Verify response from Flash */
    if (queue_verify(ctx.crypto_wrapper, &ctx.q) == false) {
        return JEDEC_ERROR_AUTH_FAIL;
    }
    /* Close root_key_id */
    err = ctx.crypto_wrapper->close_key(auth_key_id);
    if (err != JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_KEY_OPERATION;
    }
    ctx.is_initialized = false;
    memset(&ctx, 0x00, sizeof (jedec_ctx_t));
    return JEDEC_ERROR_NONE;
}

jedec_error_t jedec_create_session(uint32_t root_key_id, bool verify, uint32_t *session_key_id)
{
    jedec_error_t err = JEDEC_ERROR_NONE;
    crypto_indicator_t indicator = {};
    uint8_t random[MAX_RANDOM_SIZE];

    if (!ctx.is_initialized) {
        return JEDEC_ERROR_NOT_PERMITTED;
    }
    queue_clear(&ctx.q);
    err = ctx.crypto_wrapper->generate_random(random, MAX_RANDOM_SIZE);
    if (err != JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_RANDOM_GEN;
    }
    err = ctx.vendor_security_op->pre_create_session(ctx.vendor_ctx,
                                                     random, MAX_RANDOM_SIZE,
                                                     &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.vendor_security_op->create_session_packet(ctx.vendor_ctx,
                                                        root_key_id,
                                                        ctx.packet, &ctx.packet_len,
                                                        &indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.vendor_security_op->packet_send(ctx.vendor_ctx,
                                              ctx.packet, ctx.packet_len,
                                              NULL, 0,
                                              NULL, 0);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }    
    /* derive session key from root key */
    err = ctx.crypto_wrapper->key_derive(&indicator, session_key_id);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.crypto_wrapper->generate_random(random, MAX_RANDOM_SIZE);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.vendor_security_op->post_create_session(ctx.vendor_ctx,
                                                      root_key_id,
                                                      *session_key_id,
                                                      random, MAX_RANDOM_SIZE,
                                                      &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Verify response from Flash */
    if (queue_verify(ctx.crypto_wrapper, &ctx.q) == false) {
        return JEDEC_ERROR_AUTH_FAIL;
    }
    return JEDEC_ERROR_NONE;
}

jedec_error_t jedec_terminate_session(uint32_t session_key_id)
{
    jedec_error_t err = JEDEC_ERROR_NONE;
    crypto_indicator_t indicator = {};
    uint8_t random[MAX_RANDOM_SIZE];

    if (!ctx.is_initialized) {
        return JEDEC_ERROR_NOT_PERMITTED;
    }
    queue_clear(&ctx.q);
    err = ctx.crypto_wrapper->generate_random(random, MAX_RANDOM_SIZE);
    if (err != JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_RANDOM_GEN;
    }
    err = ctx.vendor_security_op->pre_terminate_session(ctx.vendor_ctx,
                                                        random, MAX_RANDOM_SIZE,
                                                        &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.vendor_security_op->terminate_session_packet(ctx.vendor_ctx,
                                                           session_key_id,
                                                           ctx.packet, &ctx.packet_len,
                                                           &indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.crypto_wrapper->crypto_func(&indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.vendor_security_op->packet_send(ctx.vendor_ctx,
                                              ctx.packet, ctx.packet_len,
                                              NULL, 0,
                                              indicator.hmac.mac,
                                              indicator.hmac.mac_len);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.vendor_security_op->post_terminate_session(ctx.vendor_ctx, 
                                                         session_key_id,
                                                         &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Verify response from Flash */
    if (queue_verify(ctx.crypto_wrapper, &ctx.q) == false) {
        return JEDEC_ERROR_AUTH_FAIL;
    }
    /* Destroy companion crypto system session keys */
    err = ctx.crypto_wrapper->destroy_key(session_key_id);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    return JEDEC_ERROR_NONE;
}


jedec_error_t jedec_secure_program(uint32_t addr, const uint8_t *data, uint32_t len,
                       uint32_t session_key_id, uint32_t *bytes_programmed)
{
    jedec_error_t err = JEDEC_ERROR_NONE;
    crypto_indicator_t indicator = {};

    if (!ctx.is_initialized) {
        return JEDEC_ERROR_NOT_PERMITTED;
    }
    queue_clear(&ctx.q);
    err = ctx.vendor_security_op->pre_secure_program(ctx.vendor_ctx, addr,
                                                     session_key_id, &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Verify response from Flash */
    if (queue_verify(ctx.crypto_wrapper, &ctx.q) == false) {
        return JEDEC_ERROR_AUTH_FAIL;
    }
    err = ctx.vendor_security_op->secure_program_packet(ctx.vendor_ctx,
                                                        addr, data, len,
                                                        session_key_id,
                                                        ctx.packet, &ctx.packet_len,
                                                        &indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.crypto_wrapper->crypto_func(&indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.vendor_security_op->packet_send(ctx.vendor_ctx,
                                              ctx.packet, ctx.packet_len,
                                              indicator.aead.cipher_text,
                                              indicator.aead.text_len,
                                              indicator.aead.tag,
                                              indicator.aead.tag_len);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Get secure program response from secure Flash */
    err = ctx.vendor_security_op->post_secure_program(ctx.vendor_ctx, 
                                                      session_key_id,
                                                      ctx.packet, &ctx.packet_len,
                                                      &indicator,
                                                      &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Verify response from secure Flash */
    if (queue_verify(ctx.crypto_wrapper, &ctx.q) == false) {
        return JEDEC_ERROR_AUTH_FAIL;
    }
    err = ctx.crypto_wrapper->crypto_func(&indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Parse response from secure Flash */
    err = ctx.vendor_security_op->parse_secure_program_response(ctx.vendor_ctx,
                                                                &indicator,
                                                                bytes_programmed);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    return JEDEC_ERROR_NONE;
}

jedec_error_t jedec_secure_erase(uint32_t addr, uint32_t len, uint32_t session_key_id)
{
    jedec_error_t err = JEDEC_ERROR_NONE;
    crypto_indicator_t indicator = {};

    if (!ctx.is_initialized) {
        return JEDEC_ERROR_NOT_PERMITTED;
    }
    queue_clear(&ctx.q);
    err = ctx.vendor_security_op->pre_secure_erase(ctx.vendor_ctx, addr, len,
                                                   session_key_id, &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Verify response from Flash */
    if (queue_verify(ctx.crypto_wrapper, &ctx.q) == false) {
        return JEDEC_ERROR_AUTH_FAIL;
    }
    err = ctx.vendor_security_op->secure_erase_packet(ctx.vendor_ctx,
                                                      addr, len,
                                                      session_key_id,
                                                      ctx.packet, &ctx.packet_len,
                                                      &indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.crypto_wrapper->crypto_func(&indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.vendor_security_op->packet_send(ctx.vendor_ctx,
                                              ctx.packet, ctx.packet_len,
                                              indicator.aead.cipher_text,
                                              indicator.aead.text_len,
                                              indicator.aead.tag,
                                              indicator.aead.tag_len);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Get secure program response from secure Flash */
    err = ctx.vendor_security_op->post_secure_erase(ctx.vendor_ctx, 
                                                    session_key_id,
                                                    ctx.packet, &ctx.packet_len,
                                                    &indicator,
                                                    &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Verify response from secure Flash */
    if (queue_verify(ctx.crypto_wrapper, &ctx.q) == false) {
        return JEDEC_ERROR_AUTH_FAIL;
    }
    err = ctx.crypto_wrapper->crypto_func(&indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Parse response from secure Flash */
    err = ctx.vendor_security_op->parse_secure_erase_response(ctx.vendor_ctx, &indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    return JEDEC_ERROR_NONE;
}

jedec_error_t jedec_secure_read(uint32_t addr, uint8_t *data, uint32_t len,
                    uint32_t session_key_id, uint32_t *bytes_read)
{
    jedec_error_t err = JEDEC_ERROR_NONE;
    crypto_indicator_t indicator = {};

    if (!ctx.is_initialized) {
        return JEDEC_ERROR_NOT_PERMITTED;
    }
    queue_clear(&ctx.q);
    err = ctx.vendor_security_op->pre_secure_read(ctx.vendor_ctx, addr,
                                                  session_key_id, &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Verify response from Flash */
    if (queue_verify(ctx.crypto_wrapper, &ctx.q) == false) {
        return JEDEC_ERROR_AUTH_FAIL;
    }
    err = ctx.vendor_security_op->secure_read_packet(ctx.vendor_ctx,
                                                     addr, len,
                                                     session_key_id,
                                                     ctx.packet, &ctx.packet_len,
                                                     &indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.crypto_wrapper->crypto_func(&indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    err = ctx.vendor_security_op->packet_send(ctx.vendor_ctx,
                                              ctx.packet, ctx.packet_len,
                                              indicator.aead.cipher_text,
                                              indicator.aead.text_len,
                                              indicator.aead.tag,
                                              indicator.aead.tag_len);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Get secure program response from secure Flash */
    err = ctx.vendor_security_op->post_secure_read(ctx.vendor_ctx,
                                                   session_key_id,
                                                   ctx.packet, &ctx.packet_len,
                                                   &indicator,
                                                   &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Verify response from secure Flash */
    if (queue_verify(ctx.crypto_wrapper, &ctx.q) == false) {
        return JEDEC_ERROR_AUTH_FAIL;
    }
    err = ctx.crypto_wrapper->crypto_func(&indicator);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    /* Parse response from secure Flash */
    err = ctx.vendor_security_op->parse_secure_read_response(ctx.vendor_ctx,
                                                             &indicator,
                                                             data, len,
                                                             bytes_read);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    return JEDEC_ERROR_NONE;
}

jedec_error_t jedec_secure_copy(uint32_t src_addr, uint32_t dst_addr, uint32_t len,
                    uint32_t session_key_id)
{
    //TODO
    return JEDEC_ERROR_NONE;
}

jedec_error_t jedec_secure_get_regions_info(region_ll_node_t *region_descr_p,
                                            uint32_t session_key_id, int8_t region_index)
{
    jedec_error_t err = JEDEC_ERROR_NONE;
    crypto_indicator_t indicator = {};
    bool session_key_valid_flag;
    uint16_t region_index_start, region_index_end;
    uint8_t random[MAX_RANDOM_SIZE];

    if (!ctx.is_initialized) {
        return JEDEC_ERROR_NOT_PERMITTED;
    }
    region_ll_node_t *node_ptr = region_descr_p;
    region_ll_node_t *next_node_ptr = region_descr_p;
    memset(&indicator, 0x00, sizeof(indicator));
    queue_clear(&ctx.q);
    err = ctx.vendor_security_op->pre_secure_get_regions_info(ctx.vendor_ctx,
                                                              session_key_id,
                                                              &session_key_valid_flag,
                                                              &ctx.q);
    if (err != JEDEC_ERROR_NONE) {
        return err;
    }
    if (session_key_valid_flag) {
        if (queue_verify(ctx.crypto_wrapper, &ctx.q) == false) {
            return JEDEC_ERROR_AUTH_FAIL;
        }
    }
    if (region_index < 0) {
        region_index_start = 0;
        region_index_end = MAX_REGION_NUM - 1;
    } else if (region_index < MAX_REGION_NUM) {
        region_index_start = region_index;
        region_index_end = region_index;
    } else {
        return JEDEC_ERROR_INV_ARGS;
    }
    for (uint16_t index = region_index_start; index <= region_index_end; index++) {
        if (session_key_valid_flag) {
            err = ctx.crypto_wrapper->generate_random(random, MAX_RANDOM_SIZE);
            if (err != JEDEC_ERROR_NONE) {
                return err;
            }
            err = ctx.vendor_security_op->secure_get_regions_info_packet(ctx.vendor_ctx,
                                                                         session_key_id,
                                                                         index,
                                                                         random, MAX_RANDOM_SIZE,
                                                                         ctx.packet, &ctx.packet_len,
                                                                         &indicator);
            if (err != JEDEC_ERROR_NONE) {
                return err;
            }
            err = ctx.crypto_wrapper->crypto_func(&indicator);
            if (err != JEDEC_ERROR_NONE) {
                return err;
            }
            err = ctx.vendor_security_op->packet_send(ctx.vendor_ctx,
                                                      ctx.packet,
                                                      ctx.packet_len,
                                                      NULL,
                                                      0,
                                                      indicator.hmac.mac,
                                                      indicator.hmac.mac_len);
            if (err != JEDEC_ERROR_NONE) {
                return err;
            }
            queue_clear(&ctx.q);
            err = ctx.vendor_security_op->post_secure_get_regions_info(ctx.vendor_ctx, node_ptr, &ctx.q);
            if (err != JEDEC_ERROR_NONE) {
                return err;
            }
            /* Verify response from secure Flash */
            if (queue_verify(ctx.crypto_wrapper, &ctx.q) == false) {
                return JEDEC_ERROR_AUTH_FAIL;
            }
        } else {
            err = ctx.vendor_security_op->secure_get_regions_info_packet(ctx.vendor_ctx,
                                                                         0,
                                                                         index,
                                                                         NULL, 0,
                                                                         ctx.packet, &ctx.packet_len,
                                                                         NULL);
            if (err != JEDEC_ERROR_NONE) {
                return err;
            }
            err = ctx.vendor_security_op->packet_send(ctx.vendor_ctx,
                                                      ctx.packet, ctx.packet_len,
                                                      NULL, 0,
                                                      NULL, 0);
            if (err != JEDEC_ERROR_NONE) {
                return err;
            }
            err = ctx.vendor_security_op->post_secure_get_regions_info(ctx.vendor_ctx, node_ptr, NULL);
            if (err != JEDEC_ERROR_NONE) {
                return err;
            }
        }
        next_node_ptr = node_ptr + 1;
        node_ptr->next = next_node_ptr;
        node_ptr = next_node_ptr;
        next_node_ptr->next = NULL;
    }
    return JEDEC_ERROR_NONE;
}

jedec_error_t jedec_secure_manage_region(region_ll_node_t *head)
{
    //TODO
    return JEDEC_ERROR_NONE;
}
