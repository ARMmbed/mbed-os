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
#include <string.h>
#include "../../../JEDEC_security_HAL/include/error.h"
#include "../../vendor_provisioning_impl.h"
#include "mx78_armor.h"
#include "mx78_armor_lib.h"
#include "provisioning/mx78_armor_provision.h"
#include "spi_nor.h"
#include "../../vendor_secureflash_defs.h"
#include "plat_secure_flash.h"
#include "secureflash_layout.h"

#define ARMOR_MC_SIZE               0x04
#define DEFAULT_SECURITY_PROFILE_ID 0

#define CFG_READ_SIZE_DEFAULT       0x20
#define HEALTH_TEST_TYPE_CNT        0x05
#define TMP_BUFFER_SIZE             0x20

#ifdef SECUREFLASH_PROVISION
uint8_t PROVISIONING_BLOB[] = {
    /* Major Header */
    'S',  'F',  'P',  'I',  // DWORD 0: [31:00] magic, "SFPI"
    0x01, 0x01, 0x3c, 0x00, // DWORD 1: [03:00] minor_version, [07:04] major_version, [15:08] sub header number, [31:16] total size
    0xc2, 0x29, 0x17, 0x00, // DWORD 2: [23:00] ID, [31:24] provision_disable_indicator

    /* Sub Header, ID: 0x03, APP_INFO */
    0x03, 0x04, 0x30, 0x80, // DWORD 0: [07:00] ID, [15:08] num, [30:16] app_info size, [31] save:1, not save: 0
    /* APP_INFO, Table */
    0xFE, 0xFF, 0xFF, 0xFF, // DWORD  1: [31:00] application id            (-2/0xfffffffe)
    0x55, 0xAA, 0x55, 0x00, // DWORD  2: [31:00] key id                    (app0)
    0x00, 0xFF, 0xFF, 0xFF, // DWORD  3: [07:00] zone id, [31:08] reserved (app0)
    0xFD, 0xFF, 0xFF, 0xFF, // DWORD  1: [31:00] application id            (-3/0xfffffffd)
    0x55, 0xAA, 0x55, 0x04, // DWORD  5: [31:00] key id                    (app1)
    0x01, 0xFF, 0xFF, 0xFF, // DWORD  6: [07:00] zone id, [31:08] reserved (app1)
    0xBA, 0x0B, 0x00, 0x00, // DWORD  1: [31:00] application id            (3002/0x00000bba)
    0x55, 0xAA, 0x55, 0x0B, // DWORD  8: [31:00] key id                    (app2)
    0x02, 0xFF, 0xFF, 0xFF, // DWORD  9: [07:00] zone id, [31:08] reserved (app2)
    0xBE, 0x0B, 0x00, 0x00, // DWORD 10: [31:00] application id            (3006/0x00000bbe)
    0x55, 0xAA, 0x55, 0x0F, // DWORD 11: [31:00] key id                    (app3)
    0x03, 0xFF, 0xFF, 0xFF, // DWORD 12: [07:00] zone id, [31:08] reserved (app3)
    };
#endif /* SECUREFLASH_PROVISION */

mx78_armor_context g_mx78_armor_ctx;
static mx78_armor_security_operation_t operation[CONC_OPER_NUM] = {};

/**
 * \brief Allocate an operation context.
 *
 * \param[in]  type      ArmorFlash operation type
 * \param[out] op_ctx    Pointer to allocated operation context
 * \return true or false
 */
static bool mx78_armor_operation_alloc(mx78_armor_operation_type_t type,
                                       mx78_armor_security_operation_t **op_ctx)
{
    for (uint8_t i = 0; i < CONC_OPER_NUM; i++) {
        if (operation[i].in_use == OPER_NOT_IN_USE) {
            operation[i].in_use = OPER_IN_USE;
            operation[i].type = type;
            *op_ctx = &operation[i];
            return true;
        }
    }
    *op_ctx = NULL;
    return false;
}

/**
 * \brief Release an operation context.
 *
 * \param[in] op_ctx  Pointer to the operation context
 * \return NULL
 */
static void mx78_armor_operation_release(mx78_armor_security_operation_t *op_ctx)
{
    op_ctx->in_use = OPER_NOT_IN_USE;
    memset(op_ctx, 0x00, sizeof(mx78_armor_security_operation_t));
}

/**
 * \brief Look up an operation context.
 * \param[in]  type    ArmorFlash operation type
 * \param[out] op_ctx  Pointer to the matched operation context
 * \return NULL
 */
static void mx78_armor_operation_lookup(mx78_armor_operation_type_t type,
                                        mx78_armor_security_operation_t **op_ctx)
{
    for (uint8_t i = 0; i < CONC_OPER_NUM; i++) {
        if ((operation[i].in_use == OPER_IN_USE) &&
            (operation[i].type == type)) {
            *op_ctx = &operation[i];
            return;
        }
    }
    *op_ctx = NULL;
}

static int32_t mx78_armor_packet_send(mx78_armor_context *mx78_armor_ctx,
                                      uint8_t *outdata, uint32_t outdata_len,
                                      uint8_t *cipher_text, uint32_t cipher_text_len,
                                      uint8_t *mac, uint32_t mac_len)
{
    int32_t status;
    uint32_t tx_packet_size;
    uint32_t outdata_actual_len = outdata_len;

    if ((mac != NULL) && (mac_len > 0)) {
        memcpy(outdata + outdata_len, mac, mac_len);
        outdata_actual_len += mac_len;
    }
    if ((cipher_text != NULL) && (cipher_text_len > 0)) {
        memcpy(outdata + outdata_actual_len, cipher_text, cipher_text_len);
        outdata_actual_len += cipher_text_len;
    }
    status = __packet_assemble(mx78_armor_ctx, PACKET_OUT,
                               outdata, outdata_actual_len,
                               mx78_armor_ctx->scratchpad, &tx_packet_size);
    if (status != JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_CMD_PACKET;
    }
    if (spi_write(mx78_armor_ctx->scratchpad, tx_packet_size)) {
        return JEDEC_ERROR_COMM_FAIL;
    }
    return JEDEC_ERROR_NONE;
}

static int32_t mx78_armor_packet_receive(mx78_armor_context *mx78_armor_ctx,
                                         uint8_t *indata, uint32_t indata_len)
{
    int32_t status;
    uint8_t rx_buffer[PACKET_MAX_LEN];
    uint32_t tx_packet_size, rx_packet_size;

    rx_packet_size = indata_len;
    status = __packet_assemble(mx78_armor_ctx, PACKET_IN, NULL, 0,
                               mx78_armor_ctx->scratchpad, &tx_packet_size);
    if (status != JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_CMD_PACKET;
    }
    if (spi_read(mx78_armor_ctx->scratchpad, tx_packet_size, rx_buffer, rx_packet_size)) {
        return JEDEC_ERROR_COMM_FAIL;
    }
    memcpy(indata, rx_buffer, indata_len);
    return JEDEC_ERROR_NONE;
}

/**
 * \brief Exchange nonce with ArmorFlash.
 *
 * \param[in]  mx78_armor_ctx     MX78 ArmorFlash context
 * \param[in]  nonce_in           Input nonce from host
 * \param[in]  nonce_in_len       Input nonce length
 * \param[out] nonce_out          Buffer for holding output nonce from ArmorFlash
 * \param[in]  buf_size           Buffer size
 * \param[out] actual_nonce_size  Actual size of output nonce from ArmorFlash
 *
 * \return JEDEC_ERROR_NONE if successful,
 *         JEDEC_ERROR_XX if failed
 */
static int32_t _armor_generate_nonce(mx78_armor_context *mx78_armor_ctx,
                                     uint8_t *nonce_in, uint32_t nonce_in_len,
                                     uint8_t *nonce_out, uint32_t buf_size,
                                     uint32_t *actual_nonce_size)
{
    uint8_t rw_packet[PACKET_MAX_LEN];
    uint32_t wr_packet_len, rd_packet_len;
    uint8_t nonce_len;
    int32_t status;
    mx78_armor_security_operation_t *oper_ctx;

    if (!mx78_armor_operation_alloc(GENERATE_NONCE, &oper_ctx)) {
        return JEDEC_ERROR_INSUFFICIENT_MEMORY;
    }
    nonce_len = __get_nonce_size(oper_ctx);
    oper_ctx->out_size = nonce_len;
    if (nonce_in_len > nonce_len) {
        nonce_in_len = nonce_len;
    }
    status = __prepare_write_packet(oper_ctx,
                                    nonce_in, nonce_in_len,
                                    NULL, 0,
                                    rw_packet, &wr_packet_len,
                                    &rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_generate_nonce_exit_point;
    }
    status = mx78_armor_packet_send(mx78_armor_ctx,
                                    rw_packet, wr_packet_len,
                                    NULL, 0, NULL, 0);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_generate_nonce_exit_point;
    }
    status = spi_nor_polling_for_out_ready();
    if (status) {
        return JEDEC_ERROR_DEVICE_BUSY;
    }
    status = mx78_armor_packet_receive(mx78_armor_ctx, rw_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_generate_nonce_exit_point;
    }
    status = __parse_read_packet(oper_ctx,
                                 nonce_out, buf_size,
                                 NULL, 0,
                                 rw_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_generate_nonce_exit_point;
    }
    *actual_nonce_size = nonce_len;
_armor_generate_nonce_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

/**
 * \brief Read current ArmorFlash sessions status.
 *
 * \param[in]  mx78_armor_ctx  MX78 ArmorFlash context
 * \param[out] session_status  Buffer to store session status
 * \param[in]  status_size     Size of status in byte
 * \return JEDEC_ERROR_NONE if successful,
 *         JEDEC_ERROR_XX if failed
 */
static int32_t _armor_check_sessions_status(mx78_armor_context *mx78_armor_ctx,
                                            uint8_t *session_status,
                                            uint32_t status_size)
{
    uint8_t rw_packet[PACKET_MAX_LEN];
    uint32_t wr_packet_len, rd_packet_len;
    int32_t status;
    mx78_armor_security_operation_t *oper_ctx;

    if (!mx78_armor_operation_alloc(READ_SESSION_STATUS, &oper_ctx)) {
        return JEDEC_ERROR_INSUFFICIENT_MEMORY;
    }
    oper_ctx->out_size = status_size;
    status = __prepare_write_packet(oper_ctx,
                                    NULL, 0,
                                    NULL, 0,
                                    rw_packet, &wr_packet_len,
                                    &rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_check_sessions_status_exit_point;
    }
    status = mx78_armor_packet_send(mx78_armor_ctx,
                                    rw_packet, wr_packet_len,
                                    NULL, 0, NULL, 0);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_check_sessions_status_exit_point;
    }
    status = spi_nor_polling_for_out_ready();
    if (status) {
        return JEDEC_ERROR_DEVICE_BUSY;
    }
    memset(rw_packet, 0x00, sizeof(rw_packet));
    status = mx78_armor_packet_receive(mx78_armor_ctx, rw_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_check_sessions_status_exit_point;
    }
    status = __parse_read_packet(oper_ctx,
                                 session_status, status_size,
                                 NULL, 0,
                                 rw_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_check_sessions_status_exit_point;
    }
_armor_check_sessions_status_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

/**
 * \brief Confirm session key.
 *
 * \param[in]  mx78_armor_ctx  MX78 ArmorFlash context
 * \param[in]  session_key_id  Session key id
 * \param[out] resp_queue      Response queue
 *
 * \return JEDEC_ERROR_NONE if successful,
 *         JEDEC_ERROR_XX if failed
 */
static int32_t _armor_confirm_session(mx78_armor_context *mx78_armor_ctx,
                                      uint32_t session_key_id,
                                      jqueue_t *resp_queue)
{
    uint8_t rw_packet[PACKET_MAX_LEN];
    uint32_t wr_packet_len, rd_packet_len;
    uint8_t mac[MAC_SIZE];
    uint8_t resp_buf[MAX_RESP_SIZE];
    uint32_t actual_resp_len;
    int32_t status;
    resp_param_t response = {};
    mx78_armor_security_operation_t *oper_ctx;

    if (!mx78_armor_operation_alloc(CONFIRM_SESSION, &oper_ctx)) {
         return JEDEC_ERROR_INSUFFICIENT_MEMORY;
    }
    oper_ctx->crypto_key_id = session_key_id;
    status = __prepare_write_packet(oper_ctx,
                                    NULL, 0,
                                    NULL, 0,
                                    rw_packet, &wr_packet_len,
                                    &rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_confirm_session_exit_point;
    }
    status = mx78_armor_packet_send(mx78_armor_ctx,
                                    rw_packet, wr_packet_len,
                                    NULL, 0,
                                    NULL, 0);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_confirm_session_exit_point;
    }
    status = spi_nor_polling_for_out_ready();
    if (status) {
        return JEDEC_ERROR_DEVICE_BUSY;
    }
    status = mx78_armor_packet_receive(mx78_armor_ctx, rw_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_confirm_session_exit_point;
    }
    status = __parse_read_packet(oper_ctx,
                                 NULL, 0,
                                 mac, MAC_SIZE,
                                 rw_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_confirm_session_exit_point;
    }
    /* Pack response packet */
    (void)__pack_response(oper_ctx, resp_buf, sizeof(resp_buf), &actual_resp_len);
    /* Add response packet and mac to queue */
    response.alg = ALG_HMAC_SHA_256;
    response.property = PROP_HMAC_VERIFY;
    response.hmac.key_id = session_key_id;
    response.hmac.key_len = 32;
    response.hmac.idata = resp_buf;
    response.hmac.idata_len = (uint16_t)actual_resp_len;
    response.hmac.mac = mac;
    response.hmac.mac_len = MAC_SIZE;
    queue_add(resp_queue, &response);
_armor_confirm_session_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

static int32_t mx78_armor_health_test(mx78_armor_context *mx78_armor_ctx)
{
    uint8_t rw_packet[PACKET_MAX_LEN];
    uint32_t wr_packet_len, rd_packet_len;
    int32_t status;
    mx78_armor_security_operation_t *oper_ctx;

    if (!mx78_armor_operation_alloc(HEALTH_TEST, &oper_ctx)) {
        return JEDEC_ERROR_INSUFFICIENT_MEMORY;
    }
    for (uint8_t i = 0; i < HEALTH_TEST_TYPE_CNT; i++) {
        oper_ctx->addr = i;
        status = __prepare_write_packet(oper_ctx,
                                        NULL, 0,
                                        NULL, 0,
                                        rw_packet, &wr_packet_len,
                                        &rd_packet_len);
        if (status != JEDEC_ERROR_NONE) {
            goto armor_health_test_exit_point;
        }
        status = mx78_armor_packet_send(mx78_armor_ctx, rw_packet, wr_packet_len,
                                        NULL, 0, NULL, 0);
        if (status != JEDEC_ERROR_NONE) {
            goto armor_health_test_exit_point;
        }
        status = spi_nor_polling_for_out_ready();
        if (status) {
            return JEDEC_ERROR_DEVICE_BUSY;
        }
        status = mx78_armor_packet_receive(mx78_armor_ctx, rw_packet, rd_packet_len);
        if (status != JEDEC_ERROR_NONE) {
            goto armor_health_test_exit_point;
        }
        status = __parse_read_packet(oper_ctx,
                                     NULL, 0,
                                     NULL, 0,
                                     rw_packet, rd_packet_len);
        if (status != JEDEC_ERROR_NONE) {
            goto armor_health_test_exit_point;
        }
    }
armor_health_test_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

static int32_t mx78_armor_configure_security_profile(mx78_armor_context *mx78_armor_ctx)
{
    uint8_t rw_packet[PACKET_MAX_LEN];
    uint32_t wr_packet_len, rd_packet_len;
    int32_t status;
    mx78_armor_security_operation_t *oper_ctx;

    if (!mx78_armor_operation_alloc(CFG_SECURITY_PROFILE, &oper_ctx)) {
        return JEDEC_ERROR_INSUFFICIENT_MEMORY;
    }
    oper_ctx->id.profile = DEFAULT_SECURITY_PROFILE_ID;
    status = __prepare_write_packet(oper_ctx,
                                    NULL, 0,
                                    NULL, 0,
                                    rw_packet, &wr_packet_len,
                                    &rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto armor_configure_security_profile_exit_point;
    }
    status = mx78_armor_packet_send(mx78_armor_ctx,
                                    rw_packet, wr_packet_len,
                                    NULL, 0,
                                    NULL, 0);
    if (status != JEDEC_ERROR_NONE) {
        goto armor_configure_security_profile_exit_point;
    }
    status = spi_nor_polling_for_out_ready();
    if (status) {
        return JEDEC_ERROR_DEVICE_BUSY;
    }
    status = mx78_armor_packet_receive(mx78_armor_ctx, rw_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto armor_configure_security_profile_exit_point;
    }
    status = __parse_read_packet(oper_ctx,
                                 NULL, 0,
                                 NULL, 0,
                                 rw_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto armor_configure_security_profile_exit_point;
    }
armor_configure_security_profile_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

static int32_t mx78_armor_read_mc(mx78_armor_context *mx78_armor_ctx,
                                  uint8_t mc_id, uint8_t *mc,
                                  uint32_t mc_buf_size, uint32_t *mc_size)
{
    (void)mc_buf_size;
    uint8_t rw_packet[PACKET_MAX_LEN];
    uint32_t wr_packet_len, rd_packet_len;
    int32_t status = JEDEC_ERROR_NONE;
    uint8_t mc_buf[ARMOR_MC_SIZE];
    mx78_armor_security_operation_t *oper_ctx;

    if (!mx78_armor_operation_alloc(MC_READ, &oper_ctx)) {
        return JEDEC_ERROR_INSUFFICIENT_MEMORY;
    }
    oper_ctx->id.mc = mc_id;
    status = __prepare_write_packet(oper_ctx,
                                    NULL, 0,
                                    NULL, 0,
                                    rw_packet, &wr_packet_len,
                                    &rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto armor_mc_rd_exit_point;
    }
    status = mx78_armor_packet_send(mx78_armor_ctx,
                                    rw_packet, wr_packet_len,
                                    NULL, 0,
                                    NULL, 0);
    if (status != JEDEC_ERROR_NONE) {
        goto armor_mc_rd_exit_point;
    }
    status = spi_nor_polling_for_out_ready();
    if (status) {
        return JEDEC_ERROR_DEVICE_BUSY;
    }
    status = mx78_armor_packet_receive(mx78_armor_ctx, rw_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto armor_mc_rd_exit_point;
    }
    status = __parse_read_packet(oper_ctx,
                                 mc_buf, ARMOR_MC_SIZE,
                                 NULL, 0,
                                 rw_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto armor_mc_rd_exit_point;
    }
    *mc_size = ARMOR_MC_SIZE;
    memcpy(mc, mc_buf, ARMOR_MC_SIZE);
armor_mc_rd_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

/**
 * \brief Pre-processing of creating session.
 *
 * \param[in]  mx78_armor_ctx  MX78 ArmorFlash context
 * \param[in]  nonce_in        Input nonce
 * \param[in]  nonce_in_len    Input nonce length
 * \param[out] resp_queue      Response queue
 *
 * \return JEDEC_ERROR_NONE if successful,
 *         JEDEC_ERROR_XX if failed
 */
static int32_t mx78_armor_pre_create_session(mx78_armor_context *mx78_armor_ctx,
                                             uint8_t *nonce_in, uint32_t nonce_in_len,
                                             jqueue_t *resp_queue)
{
    (void)resp_queue;
    int32_t status;
    uint8_t nonce_out[MAX_NONCE_SIZE];
    uint32_t actual_nonce_size;
    uint8_t session_status = SESSION_STATUS_ABUNDANT;

    /* check whether session has been exhausted */
    status = _armor_check_sessions_status(mx78_armor_ctx, &session_status, sizeof(session_status));
    if (status != JEDEC_ERROR_NONE) {
        return status;
    }
    if (session_status == SESSION_STATUS_EXHAUST) {
        return JEDEC_ERROR_MAX_SESSIONS_REACHED;
    }
    /*exchange nonce with secure flash*/
    status = _armor_generate_nonce(mx78_armor_ctx, nonce_in, nonce_in_len,
                                   nonce_out, MAX_NONCE_SIZE, &actual_nonce_size);
    if (status != JEDEC_ERROR_NONE) {
        return status;
    }
    return status;
}

static int32_t mx78_armor_create_session_packet(mx78_armor_context *mx78_armor_ctx,
                                                uint32_t root_key_id,
                                                uint8_t *packet, uint32_t *packet_len,
                                                crypto_indicator_t *indicator)
{
    uint32_t rd_packet_len;
    int32_t status;
    uint8_t mc[ARMOR_MC_SIZE];
    uint32_t actual_mc_size;
    mx78_armor_security_operation_t *oper_ctx;

    if (!mx78_armor_operation_alloc(CREATE_SESSION, &oper_ctx)) {
        return JEDEC_ERROR_INSUFFICIENT_MEMORY;
    }
    oper_ctx->crypto_key_id = root_key_id;
    /* get linked monotonic counter id */
    status = __get_linked_mc_id(oper_ctx);
    if (status != JEDEC_ERROR_NONE) {
        return status;
    }
    status = mx78_armor_read_mc(mx78_armor_ctx, oper_ctx->id.mc, mc,
                                ARMOR_MC_SIZE, &actual_mc_size);
    /* get kdf info */
    __get_kdf_msg(root_key_id, mc, ARMOR_MC_SIZE, indicator);
    /* prepare create session cmd packet*/
    status = __prepare_write_packet(oper_ctx,
                                    NULL, 0,
                                    NULL, 0,
                                    packet, packet_len, &rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_create_session_exit_point;
    }
    return status;

_armor_create_session_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

static int32_t mx78_armor_post_create_session(mx78_armor_context *mx78_armor_ctx,
                                              uint32_t root_key_id,
                                              uint32_t session_key_id,
                                              uint8_t *nonce_in, uint32_t nonce_in_len,
                                              jqueue_t *resp_queue)
{
    int32_t status;
    uint8_t nonce_out[MAX_NONCE_SIZE];
    uint32_t actual_nonce_size;
    uint8_t rd_packet[PACKET_MAX_LEN];
    uint32_t rd_packet_len;
    mx78_armor_security_operation_t *oper_ctx;

    mx78_armor_operation_lookup(CREATE_SESSION, &oper_ctx);
    if (oper_ctx == NULL) {
        return JEDEC_ERROR_INV_ARGS;
    }
    __get_read_packet_size(oper_ctx, &rd_packet_len);
    status = spi_nor_polling_for_out_ready();
    if (status) {
        goto _armor_post_create_session_exit_point;
    }
    status = mx78_armor_packet_receive(mx78_armor_ctx, rd_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_create_session_exit_point;
    }
    status = __parse_read_packet(oper_ctx,
                                 NULL, 0,
                                 NULL, 0,
                                 rd_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_create_session_exit_point;
    }
    /*exchange nonce with secure flash*/
    status = _armor_generate_nonce(mx78_armor_ctx, nonce_in, nonce_in_len,
                                   nonce_out, MAX_NONCE_SIZE, &actual_nonce_size);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_create_session_exit_point;
    }
    status = _armor_confirm_session(mx78_armor_ctx, session_key_id, resp_queue);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_create_session_exit_point;
    }
_armor_post_create_session_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

static int32_t mx78_armor_pre_terminate_session(mx78_armor_context *mx78_armor_ctx,
                                                uint8_t *nonce_in, uint32_t nonce_in_len,
                                                jqueue_t *resp_queue)
{
    (void)resp_queue;
    int32_t status;
    uint8_t nonce_out[MAX_NONCE_SIZE];
    uint32_t actual_nonce_size;

    /*exchange nonce with secure flash*/
    status = _armor_generate_nonce(mx78_armor_ctx, nonce_in, nonce_in_len,
                                   nonce_out, MAX_NONCE_SIZE, &actual_nonce_size);
    if (status != JEDEC_ERROR_NONE) {
        return status;
    }
    return JEDEC_ERROR_NONE;
}

static int32_t mx78_armor_terminate_session_packet(mx78_armor_context *mx78_armor_ctx,
                                                   uint32_t session_key_id,
                                                   uint8_t *packet, uint32_t *packet_len,
                                                   crypto_indicator_t *indicator)
{
    (void)mx78_armor_ctx;
    (void)indicator;
    uint32_t rd_packet_len;
    int32_t status;
    mx78_armor_security_operation_t *oper_ctx;
    
    if (!mx78_armor_operation_alloc(TERMINATE_SESSION, &oper_ctx)) {
        return JEDEC_ERROR_INSUFFICIENT_MEMORY;
    }
    oper_ctx->crypto_key_id = session_key_id;
    /* prepare create session cmd packet*/
    status = __prepare_write_packet(oper_ctx,
                                    NULL, 0,
                                    NULL, 0,
                                    packet, packet_len,
                                    &rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_terminate_session_exit_point;
    }
    return status;

_armor_terminate_session_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

static int32_t mx78_armor_post_terminate_session(mx78_armor_context *mx78_armor_ctx,
                                                 jqueue_t *resp_queue)
{
    (void)resp_queue;
    int32_t status;
    uint32_t rd_packet_len;
    mx78_armor_security_operation_t *oper_ctx;

    mx78_armor_operation_lookup(TERMINATE_SESSION, &oper_ctx);
    if (oper_ctx == NULL) {
        return JEDEC_ERROR_INV_ARGS;
    }
    __get_read_packet_size(oper_ctx, &rd_packet_len);
    status = spi_nor_polling_for_out_ready();
    if (status) {
        goto _armor_post_terminate_session_exit_point;
    }

    status = mx78_armor_packet_receive(mx78_armor_ctx, mx78_armor_ctx->scratchpad, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_terminate_session_exit_point;
    }
    status = __parse_read_packet(oper_ctx,
                                 NULL, 0,
                                 NULL, 0,
                                 mx78_armor_ctx->scratchpad, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_terminate_session_exit_point;
    }
_armor_post_terminate_session_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}


static int32_t mx78_armor_pre_secure_init(mx78_armor_context *mx78_armor_ctx,
                                          uint8_t *nonce_in, uint32_t nonce_in_len,
                                          jqueue_t *resp_queue)
{
    (void)resp_queue;
    int32_t status;
    uint8_t nonce_out[MAX_NONCE_SIZE];
    uint32_t actual_nonce_size;

    status = mx78_armor_health_test(mx78_armor_ctx);
    if (status) {
        return JEDEC_ERROR_INIT_FAIL;
    }
    status = mx78_armor_configure_security_profile(mx78_armor_ctx);
    if (status != JEDEC_ERROR_NONE) {
        return status;
    }
    /*exchange nonce with secure flash*/
    return _armor_generate_nonce(mx78_armor_ctx, nonce_in, nonce_in_len,
                                   nonce_out, MAX_NONCE_SIZE, &actual_nonce_size);
}

static int32_t mx78_armor_secure_init_packet(mx78_armor_context *mx78_armor_ctx,
                                             uint32_t root_key_id,
                                             uint8_t *packet, uint32_t *packet_len,
                                             crypto_indicator_t *indicator)
{
    (void)mx78_armor_ctx;
    (void)root_key_id;
    (void)indicator;
    uint32_t rd_packet_len;
    int32_t status;
    mx78_armor_security_operation_t *oper_ctx;

    if (!mx78_armor_operation_alloc(SECURE_INIT, &oper_ctx)) {
        return JEDEC_ERROR_INSUFFICIENT_MEMORY;
    }
    oper_ctx->crypto_key_id = root_key_id;
    status = __prepare_write_packet(oper_ctx,
                                    NULL, 0,
                                    NULL, 0,
                                    packet, packet_len,
                                    &rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_secure_init_packet_exit_point;
    }
    return status;

_armor_secure_init_packet_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

static int32_t mx78_armor_post_secure_init(mx78_armor_context *mx78_armor_ctx,
                                           jqueue_t *resp_queue)
{
    (void)resp_queue;
    int32_t status;
    uint32_t rd_packet_len;
    uint8_t init_resp_buf[TMP_BUFFER_SIZE];
    mx78_armor_security_operation_t *oper_ctx;

    mx78_armor_operation_lookup(SECURE_INIT, &oper_ctx);
    if (oper_ctx == NULL) {
        return JEDEC_ERROR_INV_ARGS;
    }
    __get_read_packet_size(oper_ctx, &rd_packet_len);
    status = spi_nor_polling_for_out_ready();
    if (status) {
        goto _armor_post_secure_init_exit_point;
    }
    status = mx78_armor_packet_receive(mx78_armor_ctx, mx78_armor_ctx->scratchpad, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_secure_init_exit_point;
    }
    status = __parse_read_packet(oper_ctx,
                                 init_resp_buf, oper_ctx->out_size,
                                 NULL, 0,
                                 mx78_armor_ctx->scratchpad, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_secure_init_exit_point;
    }
_armor_post_secure_init_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

static int32_t mx78_armor_pre_secure_uninit(mx78_armor_context *mx78_armor_ctx,
                                            uint8_t *nonce_in, uint32_t nonce_in_len,
                                            jqueue_t *resp_queue)
{
    (void)resp_queue;
    uint8_t nonce_out[MAX_NONCE_SIZE];
    uint32_t actual_nonce_size;

    /*exchange nonce with secure flash*/
    return _armor_generate_nonce(mx78_armor_ctx, nonce_in, nonce_in_len,
                                   nonce_out, MAX_NONCE_SIZE, &actual_nonce_size);
}

static int32_t mx78_armor_secure_uninit_packet(mx78_armor_context *mx78_armor_ctx,
                                               uint32_t root_key_id,
                                               uint8_t *packet, uint32_t *packet_len,
                                               crypto_indicator_t *indicator)
{
    (void)mx78_armor_ctx;
    (void)root_key_id;
    (void)packet;
    (void)packet_len;
    (void)indicator;
    *packet_len = 0;
    return JEDEC_ERROR_NONE;
}

static int32_t mx78_armor_post_secure_uninit(mx78_armor_context *mx78_armor_ctx,
                                             jqueue_t *resp_queue)
{
    (void)mx78_armor_ctx;
    (void)resp_queue;
    return JEDEC_ERROR_NONE;
}

static int32_t mx78_armor_pre_secure_program(mx78_armor_context *mx78_armor_ctx,
                                             uint32_t addr, uint32_t session_key_id,
                                             jqueue_t *resp_queue)
{
    (void)mx78_armor_ctx;
    (void)addr;
    (void)session_key_id;
    (void)resp_queue;
    return JEDEC_ERROR_NONE;
}

static int32_t mx78_armor_secure_program_packet(mx78_armor_context *mx78_armor_ctx,
                                                uint32_t addr, const uint8_t *data,
                                                uint32_t len, uint32_t session_key_id,
                                                uint8_t *packet, uint32_t *packet_len,
                                                crypto_indicator_t *indicator)
{
    int32_t status;
    uint32_t rd_packet_len;
    uint8_t mc[ARMOR_MC_SIZE];
    uint32_t actual_mc_size;
    mx78_armor_security_operation_t *oper_ctx;

    if (!mx78_armor_operation_alloc(SECURITY_WRITE, &oper_ctx)) {
        return JEDEC_ERROR_INSUFFICIENT_MEMORY;
    }
    oper_ctx->addr = addr;
    oper_ctx->in_data = data;
    oper_ctx->in_size = len;
    oper_ctx->crypto_key_id = session_key_id;

    /* get linked monotonic counter id */
    status = __get_linked_mc_id(oper_ctx);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_secure_program_exit_point;
    }
    status = mx78_armor_read_mc(mx78_armor_ctx, oper_ctx->id.mc, mc,
                                ARMOR_MC_SIZE, &actual_mc_size);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_secure_program_exit_point;
    }
    /* get aead info */
    indicator->algorithm = ALG_AES_GCM_256;
    indicator->property = PROP_ENCRYPT_TAG_DATA;
    if (__get_aead_msg(oper_ctx, indicator) != JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_INV_ARGS;
    }
    /* Prepare secure program packet without tag and ciphertext*/
    status = __prepare_write_packet(oper_ctx,
                                    NULL, 0,
                                    NULL, 0,
                                    packet, packet_len,
                                    &rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_secure_program_exit_point;
    }
    /* Make the pointers point to the packet buffer in advance */
    indicator->aead.key_id = session_key_id;
    indicator->aead.key_len = 16;
    indicator->aead.tag = &packet[*packet_len];
    indicator->aead.cipher_text = &packet[*packet_len + indicator->aead.tag_len];
    return status;

_armor_secure_program_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}


static int32_t mx78_armor_post_secure_program(mx78_armor_context *mx78_armor_ctx,
                                              uint32_t session_key_id,
                                              uint8_t *resp_packet_buffer,
                                              uint32_t *resp_packet_buffer_size,
                                              crypto_indicator_t *indicator,
                                              jqueue_t *resp_queue)
{
    (void)session_key_id;
    (void)resp_queue;
    (void)resp_packet_buffer_size;
    uint8_t rd_packet[PACKET_MAX_LEN];
    uint32_t rd_packet_len;
    int32_t status;
    uint8_t mc[ARMOR_MC_SIZE];
    uint32_t actual_mc_size;
    mx78_armor_security_operation_t *oper_ctx;

    mx78_armor_operation_lookup(SECURITY_WRITE, &oper_ctx);
    if (oper_ctx == NULL) {
        return JEDEC_ERROR_INV_ARGS;
    }
    indicator->property = PROP_AUTHEN_TAG_DECRYPT_DATA;
    __get_read_packet_size(oper_ctx, &rd_packet_len);
    status = spi_nor_polling_for_out_ready();
    if (status) {
       goto _armor_post_secure_program_exit_point;
    }
    status = mx78_armor_packet_receive(mx78_armor_ctx, rd_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
       goto _armor_post_secure_program_exit_point;
    }
    /* get linked monotonic counter id */
    status = __get_linked_mc_id(oper_ctx);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_secure_program_exit_point;
    }
    status = mx78_armor_read_mc(mx78_armor_ctx, oper_ctx->id.mc, mc,
                                ARMOR_MC_SIZE, &actual_mc_size);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_secure_program_exit_point;
    }
    /* get aead info */
    if (__get_aead_msg(oper_ctx, indicator) != JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_INV_ARGS;
    }
    /* Allocate buffer for AEAD information */
    indicator->aead.cipher_text = resp_packet_buffer;
    indicator->aead.tag = resp_packet_buffer + indicator->aead.text_len;
    indicator->aead.plain_text = resp_packet_buffer + indicator->aead.text_len + indicator->aead.tag_len;
    status = __parse_read_packet(oper_ctx,
                                 indicator->aead.cipher_text, indicator->aead.text_len,
                                 indicator->aead.tag, (uint8_t)indicator->aead.tag_len,
                                 rd_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_secure_program_exit_point;
    }
_armor_post_secure_program_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

static int32_t mx78_armor_parse_secure_program_response(mx78_armor_context *mx78_armor_ctx,
                                                        crypto_indicator_t *indicator,
                                                        uint32_t *bytes_programmed)
{
    (void)mx78_armor_ctx;

    if (__parse_secure_program_response(indicator->aead.plain_text,
                                        (uint16_t)indicator->aead.text_len,
                                        bytes_programmed) == JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_NONE;
    } else {
        return JEDEC_ERROR_COMM_FAIL;
    }
}


static int32_t mx78_armor_pre_secure_read(mx78_armor_context *mx78_armor_ctx,
                                          uint32_t addr, uint32_t session_key_id,
                                          jqueue_t *resp_queue)
{
    (void)mx78_armor_ctx;
    (void)addr;
    (void)session_key_id;
    (void)resp_queue;
    return JEDEC_ERROR_NONE;
}

static int32_t mx78_armor_secure_read_packet(mx78_armor_context *mx78_armor_ctx,
                                             uint32_t addr, uint32_t len,
                                             uint32_t session_key_id,
                                             uint8_t *packet, uint32_t *packet_len,
                                             crypto_indicator_t *indicator)
{
    int32_t status;
    uint32_t rd_packet_len;
    uint8_t mc[ARMOR_MC_SIZE];
    uint32_t actual_mc_size;
    mx78_armor_security_operation_t *oper_ctx;

    if (!mx78_armor_operation_alloc(SECURITY_READ, &oper_ctx)) {
        return JEDEC_ERROR_INSUFFICIENT_MEMORY;
    }
    oper_ctx->addr = addr;
    oper_ctx->out_size = len;
    oper_ctx->crypto_key_id = session_key_id;
    /* get linked monotonic counter id */
    status = __get_linked_mc_id(oper_ctx);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_secure_read_exit_point;
    }
    status = mx78_armor_read_mc(mx78_armor_ctx, oper_ctx->id.mc, mc,
                                ARMOR_MC_SIZE, &actual_mc_size);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_secure_read_exit_point;
    }
    /* get aead info */
    indicator->algorithm = ALG_AES_GCM_256;
    indicator->property = PROP_ENCRYPT_TAG_DATA;
    if (__get_aead_msg(oper_ctx, indicator) != JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_INV_ARGS;
    }
    /* Prepare secure read packet without tag and ciphertext*/
    status = __prepare_write_packet(oper_ctx,
                                    NULL, 0,
                                    NULL, 0,
                                    packet, packet_len, &rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_secure_read_exit_point;
    }
    /* Make the pointers point to the packet buffer in advance */
    indicator->aead.key_id = session_key_id;
    indicator->aead.key_len = 16;
    indicator->aead.tag = &packet[*packet_len];
    indicator->aead.cipher_text = &packet[*packet_len + indicator->aead.tag_len];
    return status;

_armor_secure_read_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

static int32_t mx78_armor_post_secure_read(mx78_armor_context *mx78_armor_ctx,
                                           uint32_t session_key_id,
                                           uint8_t *resp_packet_buffer,
                                           uint32_t *resp_packet_buffer_size,
                                           crypto_indicator_t *indicator,
                                           jqueue_t *resp_queue)
{
    (void)resp_queue;
    (void)resp_packet_buffer_size;
    uint8_t rd_packet[PACKET_MAX_LEN];
    uint32_t rd_packet_len;
    int32_t status;
    uint8_t mc[ARMOR_MC_SIZE];
    uint32_t actual_mc_size;
    mx78_armor_security_operation_t *oper_ctx = NULL;

    mx78_armor_operation_lookup(SECURITY_READ, &oper_ctx);
    if (oper_ctx == NULL) {
        return JEDEC_ERROR_INV_ARGS;
    }    
    __get_read_packet_size(oper_ctx, &rd_packet_len);
    status = spi_nor_polling_for_out_ready();
    if (status) {
        return JEDEC_ERROR_DEVICE_BUSY;
    }
    status = mx78_armor_packet_receive(mx78_armor_ctx, rd_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_secure_read_exit_point;
    }
   /* get linked monotonic counter id */
    status = __get_linked_mc_id(oper_ctx);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_secure_read_exit_point;
    }
    status = mx78_armor_read_mc(mx78_armor_ctx, oper_ctx->id.mc, mc,
                                ARMOR_MC_SIZE, &actual_mc_size);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_secure_read_exit_point;
    }
    indicator->algorithm = ALG_AES_GCM_256;
    indicator->property = PROP_AUTHEN_TAG_DECRYPT_DATA;
    /* get aead info */
    if (__get_aead_msg(oper_ctx, indicator) != JEDEC_ERROR_NONE) {
       return JEDEC_ERROR_INV_ARGS;
    }
    /* Allocate buffer for AEAD information */
    indicator->aead.key_id = session_key_id;
    indicator->aead.key_len = 16;
    indicator->aead.cipher_text = resp_packet_buffer;
    indicator->aead.tag = resp_packet_buffer + indicator->aead.text_len;
    indicator->aead.plain_text = resp_packet_buffer + indicator->aead.text_len + indicator->aead.tag_len;

    status = __parse_read_packet(oper_ctx,
                                 indicator->aead.cipher_text, indicator->aead.text_len,
                                 indicator->aead.tag, (uint8_t)indicator->aead.tag_len,
                                 rd_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_secure_read_exit_point;
    }
_armor_post_secure_read_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

static int32_t mx78_armor_parse_secure_read_response(mx78_armor_context *mx78_armor_ctx,
                                                     crypto_indicator_t *indicator,
                                                     uint8_t *data, uint32_t data_len,
                                                     uint32_t *bytes_read)
{
    (void)mx78_armor_ctx;

    if (__parse_secure_read_response(indicator->aead.plain_text,
                                     (uint16_t)indicator->aead.text_len,
                                     data, data_len, bytes_read) == JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_NONE;
    } else {
        return JEDEC_ERROR_COMM_FAIL;
    }
}


static int32_t mx78_armor_pre_secure_erase(mx78_armor_context *mx78_armor_ctx,
                                           uint32_t addr, uint32_t len,
                                           uint32_t session_key_id,
                                           jqueue_t *resp_queue)
{
    (void)resp_queue;
    (void)mx78_armor_ctx;
    (void)session_key_id;
    uint32_t secure_erase_size;

    secure_erase_size = __secure_erase_min_size();
    if (secure_erase_size == 0) {
        return JEDEC_ERROR_INV_ARGS;
    }
    if (((addr % secure_erase_size) != 0) || (((addr + len) % secure_erase_size) != 0)) {
        return JEDEC_ERROR_INV_ARGS;
    }
    return JEDEC_ERROR_NONE;
}

static int32_t mx78_armor_secure_erase_packet(mx78_armor_context *mx78_armor_ctx,
                                              uint32_t addr, uint32_t len,
                                              uint32_t session_key_id,
                                              uint8_t *packet, uint32_t *packet_len,
                                              crypto_indicator_t *indicator)
{
    int32_t status;
    uint32_t rd_packet_len;
    uint8_t mc[ARMOR_MC_SIZE];
    uint32_t actual_mc_size;
    mx78_armor_security_operation_t *oper_ctx = NULL;

    if (!mx78_armor_operation_alloc(SECURITY_ERASE, &oper_ctx)) {
        return JEDEC_ERROR_INSUFFICIENT_MEMORY;
    }
    oper_ctx->addr = addr;
    oper_ctx->in_size = len;
    oper_ctx->crypto_key_id = session_key_id;
    /* get linked monotonic counter id */
    status = __get_linked_mc_id(oper_ctx);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_secure_erase_exit_point;
    }
    status = mx78_armor_read_mc(mx78_armor_ctx, oper_ctx->id.mc, mc,
                                ARMOR_MC_SIZE, &actual_mc_size);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_secure_erase_exit_point;
    }
    indicator->algorithm = ALG_AES_GCM_256;
    indicator->property = PROP_ENCRYPT_TAG_DATA;
    /* get aead info */
    if (__get_aead_msg(oper_ctx, indicator) != JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_INV_ARGS;
    }
    status = __prepare_write_packet(oper_ctx,
                                    NULL, 0,
                                    NULL, 0,
                                    packet, packet_len, &rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_secure_erase_exit_point;
    }
    indicator->aead.key_id = session_key_id;
    indicator->aead.key_len = 16;
    indicator->aead.tag = &packet[*packet_len];
    indicator->aead.cipher_text = &packet[*packet_len + indicator->aead.tag_len];
    return status;

_armor_secure_erase_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

static int32_t mx78_armor_post_secure_erase(mx78_armor_context *mx78_armor_ctx,
                                            uint32_t session_key_id,
                                            uint8_t *resp_packet_buffer,
                                            uint32_t *resp_packet_buffer_size,
                                            crypto_indicator_t *indicator,
                                            jqueue_t *resp_queue)
{
    (void)session_key_id;
    (void)resp_queue;
    (void)resp_packet_buffer_size;
    uint8_t rd_packet[PACKET_MAX_LEN];
    uint32_t rd_packet_len;
    int32_t status;
    uint8_t mc[ARMOR_MC_SIZE];
    uint32_t actual_mc_size;
    mx78_armor_security_operation_t *oper_ctx = NULL;

    mx78_armor_operation_lookup(SECURITY_ERASE, &oper_ctx);
    if (oper_ctx == NULL) {
        return JEDEC_ERROR_INV_ARGS;
    }
    indicator->property = PROP_AUTHEN_TAG_DECRYPT_DATA;
    __get_read_packet_size(oper_ctx, &rd_packet_len);
    status = spi_nor_polling_for_out_ready();
    if (status) {
       goto _armor_post_secure_erase_exit_point;
    }
    status = mx78_armor_packet_receive(mx78_armor_ctx, rd_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
       goto _armor_post_secure_erase_exit_point;
    }
    /* get linked monotonic counter id */
    status = __get_linked_mc_id(oper_ctx);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_secure_erase_exit_point;
    }
    status = mx78_armor_read_mc(mx78_armor_ctx, oper_ctx->id.mc, mc,
                                ARMOR_MC_SIZE, &actual_mc_size);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_secure_erase_exit_point;
    }
    /* get aead info */
    if (__get_aead_msg(oper_ctx, indicator) != JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_INV_ARGS;
    }
    /* Allocate buffer for AEAD information */
    indicator->aead.cipher_text = resp_packet_buffer;
    indicator->aead.tag = resp_packet_buffer + indicator->aead.text_len;
    indicator->aead.plain_text = resp_packet_buffer + indicator->aead.text_len + indicator->aead.tag_len;
    status = __parse_read_packet(oper_ctx,
                                 indicator->aead.cipher_text, indicator->aead.text_len,
                                 indicator->aead.tag, (uint8_t)indicator->aead.tag_len,
                                 rd_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_post_secure_erase_exit_point;
    }
_armor_post_secure_erase_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

static int32_t mx78_armor_parse_secure_erase_response(mx78_armor_context *mx78_armor_ctx,
                                                      crypto_indicator_t *indicator)
{
    (void)mx78_armor_ctx;

    if (__parse_secure_erase_response(indicator->aead.plain_text,
                                      indicator->aead.text_len) == JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_NONE;
    } else {
        return JEDEC_ERROR_COMM_FAIL;
    }
}

static int32_t mx78_armor_pre_get_region_info(mx78_armor_context *mx78_armor_ctx,
                                              uint32_t session_key_id,
                                              bool *session_key_valid_flag,
                                              jqueue_t *resp_queue)
{ 
    (void)mx78_armor_ctx;
    (void)resp_queue;

    __mx78_armor_check_session_key_id_validity(session_key_id, session_key_valid_flag);
    return JEDEC_ERROR_NONE;
}

static int32_t mx78_armor_get_region_info_packet(mx78_armor_context *mx78_armor_ctx,
                                                 uint32_t session_key_id, int8_t region_index,
                                                 uint8_t *nonce_in, uint32_t nonce_in_len,
                                                 uint8_t *packet, uint32_t *packet_len,
                                                 crypto_indicator_t *indicator)
{
    (void)indicator;
    int32_t status;
    uint32_t rd_packet_len;
    uint8_t nonce_out[MAX_NONCE_SIZE];
    uint32_t actual_nonce_size;
    mx78_armor_security_operation_t *oper_ctx = NULL;

    if (!mx78_armor_operation_alloc(SECURITY_GET_REGION_CONFIG, &oper_ctx)) {
        return JEDEC_ERROR_INSUFFICIENT_MEMORY;
    }
    if (session_key_id == INVALID_SESSION_KEY_ID) {
        oper_ctx->crypto_key_id = 0;
    } else {
        oper_ctx->crypto_key_id = session_key_id;
        /*exchange nonce with secure flash*/
        status = _armor_generate_nonce(mx78_armor_ctx, nonce_in, nonce_in_len,
                                       nonce_out, MAX_NONCE_SIZE, &actual_nonce_size);
        if (status != JEDEC_ERROR_NONE) {
            return status;
        }
    }
    oper_ctx->id.region = region_index;
    oper_ctx->out_size = CFG_SIZE_PER_REGION;
    status = __prepare_write_packet(oper_ctx,
                                    NULL, 0,
                                    NULL, 0,
                                    packet, packet_len, &rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
        goto _armor_get_region_config_exit_point;
    }
_armor_get_region_config_exit_point:
    return status;
}


static int32_t mx78_armor_post_get_region_info(mx78_armor_context *mx78_armor_ctx,
                                               region_ll_node_t *region_descr_p,
                                               jqueue_t *resp_queue)
{
    uint8_t rd_packet[PACKET_MAX_LEN];
    uint32_t rd_packet_len;
    uint8_t cfg[CFG_READ_SIZE_DEFAULT];
    uint8_t mac[MAC_SIZE];
    uint8_t resp_buf[MAX_RESP_SIZE];
    uint32_t actual_resp_len;
    resp_param_t response;
    int32_t status;
    mx78_armor_security_operation_t *oper_ctx = NULL;

    mx78_armor_operation_lookup(SECURITY_GET_REGION_CONFIG, &oper_ctx);
    if (oper_ctx == NULL) {
        return JEDEC_ERROR_INV_ARGS;
    }
    __get_read_packet_size(oper_ctx, &rd_packet_len);
    status = spi_nor_polling_for_out_ready();
    if (status) {
       goto _armor_post_get_region_config_exit_point;
    }
    status = mx78_armor_packet_receive(mx78_armor_ctx, rd_packet, rd_packet_len);
    if (status != JEDEC_ERROR_NONE) {
       goto _armor_post_get_region_config_exit_point;
    }
    oper_ctx->out_data = cfg;
    if (oper_ctx->crypto_key_id != 0) {
        status = __parse_read_packet(oper_ctx,
                                     cfg, CFG_SIZE_PER_REGION,
                                     mac, MAC_SIZE,
                                     rd_packet, rd_packet_len);
        if (status != JEDEC_ERROR_NONE) {
            goto _armor_post_get_region_config_exit_point;
        }
        /* Pack response packet */
       (void)__pack_response(oper_ctx, resp_buf, sizeof(resp_buf), &actual_resp_len);
       /* Add response packet and mac to queue */
       response.alg = ALG_HMAC_SHA_256;
       response.hmac.idata = resp_buf;
       response.hmac.idata_len = (uint16_t)actual_resp_len;
       response.hmac.mac = mac;
       response.hmac.mac_len = MAC_SIZE;
       queue_add(resp_queue, &response);
    } else {
        status = __parse_read_packet(oper_ctx,
                                     cfg, CFG_SIZE_PER_REGION,
                                     NULL, 0,
                                     rd_packet, rd_packet_len);
        if (status != JEDEC_ERROR_NONE) {
            goto _armor_post_get_region_config_exit_point;
        }
    }
   /* Return region's attributes */
   status = __parse_region_config(oper_ctx, region_descr_p);
   if (status != JEDEC_ERROR_NONE) {
       goto _armor_post_get_region_config_exit_point;
   }
_armor_post_get_region_config_exit_point:
    mx78_armor_operation_release(oper_ctx);
    return status;
}

static int32_t _check_major_header(uint8_t *provision_data_blob,
                                   provision_major_header_t **major_header)
{
    *major_header = (provision_major_header_t *)provision_data_blob;

    SF_DBG_PR("major_header-magic:      %c%c%c%c\r\n",
              (*major_header)->magic[0],(*major_header)->magic[1],
              (*major_header)->magic[2],(*major_header)->magic[3]);
    SF_DBG_PR("major_header-major-version: %X\r\n",
              (*major_header)->major_version);
    SF_DBG_PR("major_header-minor-version: %X\r\n",
              (*major_header)->minor_version);
    SF_DBG_PR("major_header-total size:     %X\r\n",
              (*major_header)->total_size);
    SF_DBG_PR("major_header-sub_header num: %X\r\n",
              (*major_header)->sub_header_num);
    if (memcmp((*major_header)->magic, "SFPI", 4) != 0) {
        SF_ERR_PR("Check magic \"SFPI\" failed\r\n");
        return -1;
    }
    if (memcmp((*major_header)->id, macronix_mx78_info.id, macronix_mx78_info.id_len) != 0) {
        return -1;
    }
    return 0;
}
#ifdef SECUREFLASH_PROVISION
static int32_t mx78_armor_provision_perform_and_verify(mx78_armor_context *mx78_armor_ctx,
                                                       uint8_t *provision_data,
                                                       uint32_t data_length)
{
    int32_t status;
    provision_major_header_t *major_header = NULL;
    uint8_t provision_data_buf[PROVISION_INFO_SIZE] = {};
    uint8_t vfy_provision_data_buf[PROVISION_INFO_SIZE] = {};    
    uint32_t data_store_size = 0;

    if (provision_data == NULL) {        
        provision_data = PROVISIONING_BLOB;
        data_length = sizeof(PROVISIONING_BLOB);
    }    
    memcpy(provision_data_buf, provision_data, SFPI_MAJOR_HEADER_SIZE);
    /* check major header */
    if (_check_major_header(provision_data_buf, &major_header) != 0) {
        return JEDEC_ERROR_PROVISION;
    }
    status = mx78_armor_configure_security_profile(mx78_armor_ctx);
    if (status != JEDEC_ERROR_NONE) {
        return status;
    }
    status = mx78_armor_provision_perform(mx78_armor_ctx, provision_data, data_length,
                                          provision_data_buf, &data_store_size);
    if (status != JEDEC_ERROR_NONE) {
        return JEDEC_ERROR_PROVISION;
    }
    status = plat_store_secure_flash_provision_info(provision_data_buf, data_store_size);
    if (status != 0) {
        return JEDEC_ERROR_PROVISION;
    }
    status = plat_get_secure_flash_provision_info(vfy_provision_data_buf, data_store_size);
    if (status != 0) {
        return JEDEC_ERROR_PROVISION;
    }
    /* Read back provision data stored by above code line, and check whether
       provision data has been stored successfully */
    if (memcmp(vfy_provision_data_buf, provision_data_buf, data_store_size) != 0) {
        return JEDEC_ERROR_PROVISION;
    }
    return status;
}
#endif /* SECUREFLASH_PROVISION */
static int32_t mx78_armor_provision_item_get_data(mx78_armor_context *mx78_armor_ctx,
                                                  provision_item_type_e item_type,
                                                  uint8_t *item_data_buf,
                                                  uint32_t item_data_buf_size,
                                                  uint8_t *item_data_num,
                                                  uint32_t *item_data_act_size)
{
    (void)mx78_armor_ctx;
    int32_t status;
    uint8_t provision_data_buf[PROVISION_INFO_SIZE] = {};
    uint32_t act_size = 0;
    uint8_t i;
    provision_major_header_t *major_header = NULL;
    provision_sub_item_header_t *sub_header = NULL;
    /* Get the major header of provisioning info*/
    status = plat_get_secure_flash_provision_info(provision_data_buf, SFPI_MAJOR_HEADER_SIZE);
    if (0 != status) {
        return JEDEC_ERROR_PROVISION;
    }
    major_header = provision_data_buf;
    /* Check major header magic */
    if (memcmp(major_header->magic, "SFPI", 4) != 0) {
        SF_ERR_PR("Provision info header magic check failed\r\n");
        return JEDEC_ERROR_GET_PROVISION_INFO;
    }
    act_size = major_header->total_size + sizeof(major_header->magic);
    if (act_size > PROVISION_INFO_SIZE) {
        return JEDEC_ERROR_INV_ARGS;
    }
    status = plat_get_secure_flash_provision_info(provision_data_buf, act_size);
    if (JEDEC_ERROR_NONE != status) {
        return JEDEC_ERROR_GET_PROVISION_INFO;
    }
    sub_header = provision_data_buf + SFPI_MAJOR_HEADER_SIZE;
    switch (item_type) {
        case ITEM_APP_INFO:
            for (i = 0; i < major_header->sub_header_num; i++) {
                if (sub_header->id == SUB_ID_APP_INFO) {
                    if (item_data_buf_size < sub_header->size) {
                       return JEDEC_ERROR_PROVISION;
                    }
                    *item_data_num = sub_header->num;
                    memcpy(item_data_buf, (uint8_t *)sub_header + SFPI_SUB_HEADER_SIZE, sub_header->size);
                    if (__parse_app_info(item_data_buf, sub_header->size, sub_header->num) != JEDEC_ERROR_NONE) {
                        return JEDEC_ERROR_GET_PROVISION_INFO;
                    }
                    *item_data_act_size = sub_header->size;
                    return JEDEC_ERROR_NONE;
                }
                /* Move to next sub-item header */
                sub_header = (uint8_t *)sub_header + SFPI_SUB_HEADER_SIZE + sub_header->size;
            }
            break;
            /*TODO*/
        default:
            break;
    }
    return JEDEC_ERROR_GET_PROVISION_INFO;
}

flash_profile_t macronix_armor_mx78_profile = {
    .security_feature.security_storage = SECURE_FLASH_SECURITY_STORAGE_CAP,
    .architecture.regions_min_secure_erase_size = SECURE_FLASH_SECTOR_SIZE,
    .architecture.secure_program_size = SECURE_FLASH_PROGRAM_SIZE,
    .architecture.secure_read_size = SECURE_FLASH_READ_SIZE,
    .architecture.secure_zone_number = SECURE_FLASH_ZONE_NUM,
    .architecture.secure_zone_size = SECURE_FLASH_ZONE_SIZE,
    .architecture.secure_zone_total_size = SECURE_FLASH_SIZE
};

vendor_security_op_t macronix_armor_mx78 = {
    .vendor_id = 0x03,
    .pre_create_session = mx78_armor_pre_create_session,
    .create_session_packet = mx78_armor_create_session_packet,
    .post_create_session = mx78_armor_post_create_session,
#if defined(SESSION_CONFIRMATION)
    .pre_confirm_session = null,
    .confirm_session_packet = null,
    .post_confirm_session = null,
#endif
    .pre_terminate_session = mx78_armor_pre_terminate_session,
    .terminate_session_packet = mx78_armor_terminate_session_packet,
    .post_terminate_session = mx78_armor_post_terminate_session,
    .pre_secure_init = mx78_armor_pre_secure_init,
    .secure_init_packet = mx78_armor_secure_init_packet,
    .post_secure_init = mx78_armor_post_secure_init,
    .pre_secure_uninit = mx78_armor_pre_secure_uninit,
    .secure_uninit_packet = mx78_armor_secure_uninit_packet,
    .post_secure_uninit = mx78_armor_post_secure_uninit,
    .pre_secure_program = mx78_armor_pre_secure_program,
    .secure_program_packet = mx78_armor_secure_program_packet,
    .post_secure_program = mx78_armor_post_secure_program,
    .parse_secure_program_response = mx78_armor_parse_secure_program_response,
    .pre_secure_erase = mx78_armor_pre_secure_erase,
    .secure_erase_packet = mx78_armor_secure_erase_packet,
    .post_secure_erase = mx78_armor_post_secure_erase,
    .parse_secure_erase_response = mx78_armor_parse_secure_erase_response,
#if defined(SECURE_COPY)
    .pre_secure_copy = null,
    .secure_copy_packet = null,//TODO:return not supported
    .post_secure_copy = null,
#endif
#if defined(SECURE_READ)
    .pre_secure_read = mx78_armor_pre_secure_read,
    .secure_read_packet = mx78_armor_secure_read_packet,
    .post_secure_read = mx78_armor_post_secure_read,
    .parse_secure_read_response = mx78_armor_parse_secure_read_response,
#endif
    .pre_secure_get_regions_info = mx78_armor_pre_get_region_info,
    .secure_get_regions_info_packet = mx78_armor_get_region_info_packet,
    .post_secure_get_regions_info = mx78_armor_post_get_region_info,
#if defined(SECURE_REGION_MANAGE)
    .pre_secure_manage_region = null,
    .secure_manage_region_packet = null,
    .post_secure_manage_region = null,
#endif
    .packet_send = mx78_armor_packet_send,
    .packet_receive = mx78_armor_packet_receive,
};

vendor_provisioning_op_t macronix_armor_mx78_provisioning = {
#ifdef SECUREFLASH_PROVISION
    .perform_and_verify = mx78_armor_provision_perform_and_verify,
#endif /* SECUREFLASH_PROVISION */
    .provision_item_get_data = mx78_armor_provision_item_get_data
};

extern crypto_wrapper_t mx78_armor_crypto_wrapper;

const secure_flash_info_t macronix_mx78_info = {
    "mx78u64xx",
    {0xc2, 0x29, 0x17},
    3,
    .flash_profile = &macronix_armor_mx78_profile,
    .vendor_security_op = &macronix_armor_mx78,
    .vendor_provisioning_op = &macronix_armor_mx78_provisioning,
    .crypto_wrapper = &mx78_armor_crypto_wrapper,
    &g_mx78_armor_ctx
};
