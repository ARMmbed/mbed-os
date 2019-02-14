/**
 * @file
 *
 * @brief      An implementation of LoRaWAN Application layer Clock Sync spec v1.0.0
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
#include "ClockSyncControlPackage.h"
#include "platform/mbed_rtc_time.h"
#include "mbed-trace/mbed_trace.h"
#define TRACE_GROUP "CPKG"

#define CLK_SYNC_PKG_IDENTIFIER                              1
#define CLK_SYNC_PKG_VERSION                                 1

ClockSyncControlPackage::ClockSyncControlPackage()
{
    _clock_sync_token_req = 0;
    _app_time_periodicty = 0;
    _forced_nb_trans = 0;
}

ClockSyncControlPackage::~ClockSyncControlPackage()
{
}

uint8_t ClockSyncControlPackage::get_resync_nb_trans(void)
{
    return _forced_nb_trans;

}
uint8_t ClockSyncControlPackage::get_sync_req_periodicity(void)
{
    return _app_time_periodicty;
}

uint16_t ClockSyncControlPackage::prepare_clock_sync_request(bool ans_required, uint16_t index)
{
    // Adjust epoch for 1970 versus 1980
    time_t t = time(NULL);
    uint32_t dev_time = t - UNIX_GPS_EPOCH_DIFF;
    // Adjust for leap seconds since 1980 (GPS+TAI counts them, UTC doesn't)
    dev_time += (MBED_CONF_LORA_CURRENT_TAI_MINUS_UTC - MBED_CONF_LORA_GPS_EPOCH_TAI_MINUS_UTC);

    tr_debug("Current device time and date = %s", ctime(&t));

    _clock_sync_token_req = (_clock_sync_token_req + 1) & 0x000F;
    uint8_t param_field = _clock_sync_token_req;
    if (ans_required) {
        SET_BIT(param_field, 4);
    }

    _outbound_buf[index++] = CLK_SYNC_APP_TIME;
    _outbound_buf[index++] = dev_time & 0xFF;
    _outbound_buf[index++] = (dev_time >> 8) & 0xFF;
    _outbound_buf[index++] = (dev_time >> 16) & 0xFF;
    _outbound_buf[index++] = (dev_time >> 24) & 0xFF;
    _outbound_buf[index++] = param_field;

    return index;
}

clk_sync_response_t *ClockSyncControlPackage::request_clock_sync(bool ans_required)
{
    prepare_clock_sync_request(ans_required, 0);
    _resp.data = _outbound_buf;
    _resp.size = APP_TIME_ANS_LENGTH;
    _resp.forced_resync_required = false;
    _resp.forced_resync_nb_trans = 0;

    return &_resp;

    // stack will automatically disables ADR and QoS for outgoing traffic on
    // CLOCK_SYNC_PORT port
}

clk_sync_response_t *ClockSyncControlPackage::parse(const uint8_t *payload,
                                                    uint16_t size)
{
    if (!payload || size == 0) {
        return NULL;
    }

    if (size > MAX_CLK_SYNC_CMD_INBOUND_BUFFER_SIZE) {
        tr_error("Can't fit ClockSync commands");
        return NULL;
    }

    uint8_t idx = 0;
    uint8_t mask = 0;
    uint8_t token_ans = 0;
    uint32_t time_correction;

    _resp.data = NULL;
    _resp.size = 0;
    _resp.forced_resync_nb_trans = 0;
    _resp.forced_resync_required = false;

    memcpy(_inbound_buf, payload, size);
    bool response_generated = false;

    uint8_t i = 0;

    while (i < size) {
        switch (_inbound_buf[i++]) {
            case CLK_SYNC_PACKAGE_VERSION:
                _outbound_buf[idx++] = CLK_SYNC_PACKAGE_VERSION;
                _outbound_buf[idx++] = CLK_SYNC_PKG_IDENTIFIER;
                _outbound_buf[idx++] = CLK_SYNC_PKG_VERSION;
                response_generated = true;
                break;

            case CLK_SYNC_APP_TIME:
                time_correction = read_four_bytes(_inbound_buf + i);
                i += 4;
                token_ans = _inbound_buf[i++] & 0x0F;

                if (token_ans != _clock_sync_token_req) {
                    tr_error("Token doesn't match - Dropping %d, token_req %d",
                             token_ans, _clock_sync_token_req);
                    break;
                }

                // Apply time correction
                tr_debug("Time correction (seconds)%" PRIu32, time_correction);
                if (time_correction >= UINT32_C(MIN_CORRECTION)
                        && time_correction <= -UINT32_C(MIN_CORRECTION)) {
                    set_time(time(NULL) + time_correction);
                    time_t now = time(NULL);
                    tr_debug("Clock set (UTC) = %s", ctime(&now));
                }

                break;

            case CLK_SYNC_APP_TIME_PERIODICITY:
                // we don't support it at the moment
                _app_time_periodicty = _inbound_buf[i++] & 0x0F;
                _outbound_buf[idx++] = CLK_SYNC_APP_TIME_PERIODICITY;
                SET_BIT(mask, 0);
                _outbound_buf[idx++] = mask & 0x01;
                write_four_bytes(0x00, _outbound_buf + idx);
                idx += 4;
                response_generated = true;
                break;

            case CLK_SYNC_FORCE_RESYNC:
                _forced_nb_trans = _inbound_buf[i++] & 0x07;

                if (_forced_nb_trans >= 1) {
                    _resp.forced_resync_required = (_forced_nb_trans == 1) ? false : true;
                    // because we will add 1 transmission in the current response
                    _resp.forced_resync_nb_trans = _forced_nb_trans - 1;
                    idx = prepare_clock_sync_request(false, idx);
                } else {
                    _resp.forced_resync_required = false;
                    _resp.forced_resync_nb_trans = 0;
                }

                break;

            default:
                tr_error("Unknown ClockSync command, %x", _inbound_buf[i]);
                return NULL;
        }
    }

    if (response_generated) {
        _resp.data = _outbound_buf;
        _resp.size = idx;
        return &_resp;
    }

    return NULL;
}


