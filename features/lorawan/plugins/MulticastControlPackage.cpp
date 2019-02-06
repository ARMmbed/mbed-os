/**
 * @file
 *
 * @brief      An implementation of LoRaWAN Remote Multicast Setup Specification v1.0.0
 *
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

#include <string.h>
#include <time.h>
#include "mbedtls/aes.h"
#include "MulticastControlPackage.h"
#include "mbed-trace/mbed_trace.h"
#define TRACE_GROUP "MPKG"

#define MCAST_PKG_IDENTIFIER                              2
#define MCAST_PKG_VERSION                                 1

MulticastControlPackage::MulticastControlPackage()
{
}

MulticastControlPackage::~MulticastControlPackage()
{
}

lorawan_status_t MulticastControlPackage::activate_multicast_control_package(const uint8_t *gen_app_key, uint32_t key_len)
{
    if (!gen_app_key || key_len != 16) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    uint8_t buf[16];
    memset(buf, 0, 16);
    buf[0] = 0x00;
    uint8_t mc_root_key[16];

    /**
     * Treatment for LoRaWAN protocol spec. 1.0.x
     * RFU -> would derive mc_root_key with input = 0x20
     */
    if (mcast_crypt(buf, 16, gen_app_key, key_len, mc_root_key) < 0) {
        tr_error("Crypto failed");
        return LORAWAN_STATUS_CRYPTO_FAIL;
    }

    if (mcast_crypt(buf, 16, mc_root_key, key_len, _mc_kekey) < 0) {
        tr_error("Crypto failed");
        return LORAWAN_STATUS_CRYPTO_FAIL;
    }

    return LORAWAN_STATUS_OK;
}

int MulticastControlPackage::mcast_crypt(const uint8_t *buffer, uint16_t size,
                                     const uint8_t *key, uint32_t key_length,
                                     uint8_t *dec_buffer, uint8_t mode)
{
    int ret = 0;

    mbedtls_aes_init(&aes_ctx);

    if (mode) {
        ret = mbedtls_aes_setkey_enc(&aes_ctx, key, key_length * 8);
    } else {
        ret = mbedtls_aes_setkey_dec(&aes_ctx, key, key_length * 8);
    }

    if (0 != ret)
        goto exit;

    ret = mbedtls_aes_crypt_ecb(&aes_ctx,
                                mode ? MBEDTLS_AES_ENCRYPT : MBEDTLS_AES_DECRYPT,
                                buffer,
                                dec_buffer);
    if (0 != ret)
        goto exit;

exit: mbedtls_aes_free(&aes_ctx);
    return ret;
}

mcast_ctrl_response_t *MulticastControlPackage::parse(const uint8_t *payload,
                                                      uint16_t size,
                                                      lorawan_mcast_register_t *mcast_register,
                                                      mcast_controller_cbs_t *cbs,
                                                      bool network_override)
{
    if (!payload || !mcast_register || !cbs || size == 0
            || size > MAX_CTRL_CMD_BUFFER_SIZE) {
        return NULL;
    }

    uint8_t idx = 0;
    uint8_t group_id = 0;
    uint8_t input[16];
    uint8_t status_field = 0;
    uint8_t nb_tot_groups = 0;
    uint8_t ans_group_mask = 0;
    uint8_t req_group_mask = 0;
    uint8_t temp_var = 0;
    lorawan_status_t retcode = LORAWAN_STATUS_OK;
    uint32_t cur_time = 0;
    uint32_t time_to_start = 0;
    bool response_generated = false;

    _resp.data = NULL;
    _resp.size = 0;

    memcpy(_inbound_buf, payload, size);

    unsigned i = 0;

    while (i < size){
        switch (_inbound_buf[i++]) {
            case MCAST_PACKAGE_VERSION:
                _outbound_buf[idx++] = MCAST_PACKAGE_VERSION;
                _outbound_buf[idx++] = MCAST_PKG_IDENTIFIER;
                _outbound_buf[idx++] = MCAST_PKG_VERSION;
                response_generated = true;
                break;
            case MCAST_GROUP_STATUS:
                _outbound_buf[idx++] = MCAST_GROUP_STATUS;

                req_group_mask = _inbound_buf[i++] & 0x0F;
                /**
                 * |RFU | NbTotalGroups | AnsGroupMask | [Optional: McGroupID+McAddr]
                 * | 1b |    3b         |      4b      |
                 */
                temp_var = mcast_register->active_mask;
                COUNT_SET_BITS(temp_var , nb_tot_groups);
                status_field = (nb_tot_groups << 4) & 0x70;
                //store current index in temp_var, will put ans_group_mask in here later
                temp_var = idx++;

                for (uint8_t j = 0; j < MBED_CONF_LORA_MAX_MULTICAST_SESSIONS; j++) {
                    if (BIT_SET_TEST(mcast_register->active_mask, j) &&
                    BIT_SET_TEST(req_group_mask, j)) {
                        SET_BIT(ans_group_mask, j);
                        _outbound_buf[idx++] = mcast_register->entry[j].g_id;
                        write_four_bytes(mcast_register->entry[j].addr, _outbound_buf + idx);
                        idx+=4;
                    }
                }

                status_field |= (ans_group_mask & 0x0F);
                // sneak in status_field
                _outbound_buf[temp_var] = status_field;
                response_generated = true;

                break;
            case MCAST_GROUP_SETUP:
                /**
                 *  | McGroupIDHeader | McAddr | McKey_encrypted | minMcFCount | max McFcount |
                 *  |       1B        | 4B     | 16B             |    4B       |      4B      |
                 */
                group_id = _inbound_buf[i++] & 0x03;

                if (BIT_SET_TEST(mcast_register->active_mask, group_id) && !network_override) {
                    tr_warn("Multicast session request rejected, network override not allowed");
                    _outbound_buf[idx++] = MCAST_GROUP_SETUP;
                    temp_var = group_id & 0x03;
                    // set the IDError bit
                    SET_BIT(temp_var, 2);
                    _outbound_buf[idx++] = temp_var;
                    // skip setup request
                    i+=28;
                    break;
                }

                _sessions_ctx.mcast_session[group_id].mcast_group_id = group_id;
                _sessions_ctx.mcast_session[group_id].mcast_addr = read_four_bytes(_inbound_buf + i);
                tr_debug("Multicast Address: %x", _sessions_ctx.mcast_session[group_id].mcast_addr);
                i+=4;

                // derive mcast_key for the group
                memset(_sessions_ctx.mcast_session[group_id].mcast_key, 0, 16);
                if (mcast_crypt(_inbound_buf+i, 16, _mc_kekey, 16,
                                _sessions_ctx.mcast_session[group_id].mcast_key, 0) < 0) {
                    tr_error("Crypto error");
                    return NULL;
                }

                i+=16;

                // derive mcast_nwk_skey
                memset(input, 0, 16);
                input[0] = 0x02;
                write_four_bytes(_sessions_ctx.mcast_session[group_id].mcast_addr, input + 1);

                if (mcast_crypt(input, 16, _sessions_ctx.mcast_session[group_id].mcast_key, 16,
                                _sessions_ctx.mcast_session[group_id].mcast_nwk_skey) < 0) {
                    tr_error("Crypto error");
                    return NULL;
                }

                // derive mcast_app_skey
                input[0] = 0x01;
                write_four_bytes(_sessions_ctx.mcast_session[group_id].mcast_addr, input + 1);

                if (mcast_crypt(input, 16, _sessions_ctx.mcast_session[group_id].mcast_key, 16,
                                _sessions_ctx.mcast_session[group_id].mcast_app_skey) < 0) {
                    tr_error("Crypto error");
                    return NULL;
                }

                _sessions_ctx.mcast_session[group_id].min_mcast_fcnt = read_four_bytes(_inbound_buf + i);
                i+=4;

                _sessions_ctx.mcast_session[group_id].max_mcast_fcnt = read_four_bytes(_inbound_buf + i);
                i+=4;

                //update multicast register
                SET_BIT(mcast_register->active_mask, group_id);
                mcast_register->entry[group_id].g_id = group_id;
                mcast_register->entry[group_id].addr = _sessions_ctx.mcast_session[group_id].mcast_addr;
                mcast_register->entry[group_id].fcnt = _sessions_ctx.mcast_session[group_id].min_mcast_fcnt;
                mcast_register->entry[group_id].min_fcnt = _sessions_ctx.mcast_session[group_id].min_mcast_fcnt;
                mcast_register->entry[group_id].max_fcnt = _sessions_ctx.mcast_session[group_id].max_mcast_fcnt;
                mcast_register->entry[group_id].app_session_key = _sessions_ctx.mcast_session[group_id].mcast_app_skey;
                mcast_register->entry[group_id].nwk_session_key = _sessions_ctx.mcast_session[group_id].mcast_nwk_skey;

                tr_info("Multicast Session Setup Request - Accepted"
                        "\n\t Group ID: %x"
                        "\n\t Multicast Addr: %x"
                        "\n\t FCnt: %lu"
                        "\n\t minFCnt: %lu"
                        "\n\t maxFCnt: %lu"
                        "\n\t AppSKey: %s"
                        "\n\t NwkSKey: %s",
                        group_id, mcast_register->entry[group_id].addr,
                        mcast_register->entry[group_id].fcnt, mcast_register->entry[group_id].min_fcnt,
                        mcast_register->entry[group_id].max_fcnt, trace_array(mcast_register->entry[group_id].app_session_key, 16),
                        trace_array(mcast_register->entry[group_id].nwk_session_key, 16));
                // prepare answer
                _outbound_buf[idx++] = MCAST_GROUP_SETUP;
                _outbound_buf[idx++] = group_id & 0x03;
                response_generated = true;

                break;
            case MCAST_GROUP_DELETE:
                group_id = _inbound_buf[i++] & 0x03;
                temp_var = group_id;

                if (BIT_SET_TEST(mcast_register->active_mask, group_id)) {
                    UNSET_BIT(mcast_register->active_mask, group_id);
                    mcast_register->entry[group_id].addr = 0;
                    mcast_register->entry[group_id].app_session_key = NULL;
                    mcast_register->entry[group_id].nwk_session_key = NULL;
                    mcast_register->entry[group_id].fcnt = 0;
                    mcast_register->entry[group_id].min_fcnt = 0;
                    mcast_register->entry[group_id].max_fcnt = 0;
                    mcast_register->entry[group_id].g_id = 0xFF;
                } else {
                    //turn on McGroupUndefined bit
                    SET_BIT(temp_var, 2);
                }

                _outbound_buf[idx++] = MCAST_GROUP_DELETE;
                _outbound_buf[idx++] = temp_var;
                response_generated = true;
                break;
            case MCAST_GROUP_CLASS_C_SESSION:
                /**
                 *  | McGroupIDHeader | Session Time | Session Timeout | DLFrequ | DR |
                 *  |       1B        | 4B           | 1B              |    3B   | 1B |
                 */
                group_id = _inbound_buf[i++] & 0x03;
                temp_var = group_id & 0x03;
                retcode = LORAWAN_STATUS_OK;

                _outbound_buf[idx++] = MCAST_GROUP_CLASS_C_SESSION;

                if (BIT_SET_TEST(mcast_register->active_mask, group_id)) {
                    _sessions_ctx.mcast_session[group_id].mcast_session_class = CLASS_C;
                    _sessions_ctx.mcast_session[group_id].session_time = read_four_bytes(_inbound_buf + i);
                    i+=4;
                    _sessions_ctx.mcast_session[group_id].session_timeout = (1 << (_inbound_buf[i++] & 0x0F));

                    _sessions_ctx.mcast_session[group_id].dl_freq = (uint32_t) _inbound_buf[i++];
                    _sessions_ctx.mcast_session[group_id].dl_freq |= (uint32_t) (_inbound_buf[i++] << 8);
                    _sessions_ctx.mcast_session[group_id].dl_freq |= (uint32_t) (_inbound_buf[i++] << 16);
                    _sessions_ctx.mcast_session[group_id].dl_freq *= 100;

                    _sessions_ctx.mcast_session[group_id].data_rate = _inbound_buf[i++];

                    if (!cbs->check_params_validity) {
                        tr_warn("Parameter validity callback missing");
                        break;
                    }

                    retcode = cbs->check_params_validity(_sessions_ctx.mcast_session[group_id].data_rate,
                                                         _sessions_ctx.mcast_session[group_id].dl_freq);

                    if (retcode == LORAWAN_STATUS_FREQUENCY_INVALID) {
                        SET_BIT(temp_var, 3);
                    } else if (retcode == LORAWAN_STATUS_FREQ_AND_DR_INVALID) {
                        SET_BIT(temp_var, 2);
                        SET_BIT(temp_var, 3);
                    }
                    _outbound_buf[idx++] = temp_var;

                    if (retcode == LORAWAN_STATUS_OK) {
                        // Add time to start field
                        time_t t = time(NULL);
                        cur_time = t - UNIX_GPS_EPOCH_DIFF;
                        // Adjust for leap seconds since 1980 (GPS+TAI counts them, UTC doesn't)
                        cur_time += (MBED_CONF_LORA_CURRENT_TAI_MINUS_UTC - MBED_CONF_LORA_GPS_EPOCH_TAI_MINUS_UTC);

                        tr_debug("Time at the moment = %s", ctime(&t));

                        if(cur_time >= _sessions_ctx.mcast_session[group_id].session_time) {
                            tr_debug("Time to switch class has already passed");
                            // switch immediately
                            time_to_start = 0;
                        } else {
                            time_to_start = _sessions_ctx.mcast_session[group_id].session_time - cur_time;
                            tr_debug("Time to start Class C session (seconds)%" PRIu32, time_to_start);
                        }

                        _outbound_buf[idx++] = time_to_start;
                        _outbound_buf[idx++] = time_to_start >> 8;
                        _outbound_buf[idx++] = time_to_start >> 16;

                        if (!cbs->switch_class) {
                            tr_warn("Class switching callback missing");
                            break;
                        }

                        cbs->switch_class(CLASS_C,
                                          time_to_start,
                                          _sessions_ctx.mcast_session[group_id].session_timeout,
                                          _sessions_ctx.mcast_session[group_id].data_rate,
                                          _sessions_ctx.mcast_session[group_id].dl_freq);
                    }
                } else {
                    SET_BIT(temp_var, 4);
                    //skip time to start
                    _outbound_buf[idx++] = temp_var;
                }
                response_generated = true;
                break;

            case MCAST_GROUP_CLASS_B_SESSION:
                tr_debug("Session request can't be handled. Class B is not supported.");
                i+=10;
                break;

            default:
                tr_error("Unknown MCAST control command");
                return NULL;
        }
    }

    if (response_generated) {
        _resp.data = _outbound_buf;
        _resp.size = idx;
    }

    return &_resp;
}
