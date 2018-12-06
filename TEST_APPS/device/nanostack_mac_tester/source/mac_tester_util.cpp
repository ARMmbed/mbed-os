/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
#include "mac_tester_util.h"

int string_to_bytes(const char *str, uint8_t *buf, int bytes)
{
    int len = strlen(str);

    if (len <= (3 * bytes - 1)) {
        int i;

        for (i = 0; i < bytes; ++i) {
            if (i * 3 < len) {
                buf[i] = (uint8_t)strtoul(str + i * 3, NULL, 16);
            } else {
                buf[i] = 0;
            }
        }
        return 0;
    }
    return -1;
}

const char *mlme_status_string(uint8_t status)
{
    switch (status) {
        case MLME_SUCCESS:
            return "MLME_SUCCESS";
        case MLME_BUSY_CHAN:
            return "MLME_BUSY_CHAN";
        case MLME_BUSY_RX:
            return "MLME_BUSY_RX";
        case MLME_BUSY_TX:
            return "MLME_BUSY_TX";
        case MLME_FORCE_TRX_OFF:
            return "MLME_FORCE_TRX_OFF";
        case MLME_IDLE:
            return "MLME_IDLE";
        case MLME_RX_ON:
            return "MLME_RX_ON";
        case MLME_TRX_OFF:
            return "MLME_TRX_OFF";
        case MLME_TX_ON:
            return "MLME_TX_ON";
        case MLME_COUNTER_ERROR:
            return "MLME_COUNTER_ERROR";
        case MLME_IMPROPER_KEY_TYPE:
            return "MLME_IMPROPER_KEY_TYPE";
        case MLME_IMPROPER_SECURITY_LEVEL:
            return "MLME_IMPROPER_SECURITY_LEVEL";
        case MLME_UNSUPPORTED_LEGACY:
            return "MLME_UNSUPPORTED_LEGACY";
        case MLME_UNSUPPORTED_SECURITY:
            return "MLME_UNSUPPORTED_SECURITY";
        case MLME_SECURITY_FAIL:
            return "MLME_SECURITY_FAIL";
        case MLME_FRAME_TOO_LONG:
            return "MLME_FRAME_TOO_LONG";
        case MLME_INVALID_HANDLE:
            return "MLME_INVALID_HANDLE";
        case MLME_INVALID_PARAMETER:
            return "MLME_INVALID_PARAMETER";
        case MLME_TX_NO_ACK:
            return "MLME_TX_NO_ACK";
        case MLME_NO_BEACON:
            return "MLME_NO_BEACON";
        case MLME_NO_DATA:
            return "MLME_NO_DATA";
        case MLME_NO_SHORT_ADDRESS:
            return "MLME_NO_SHORT_ADDRESS";
        case MLME_PAN_ID_CONFLICT:
            return "MLME_PAN_ID_CONFLICT";
        case MLME_TRANSACTION_EXPIRED:
            return "MLME_TRANSACTION_EXPIRED";
        case MLME_TRANSACTION_OVERFLOW:
            return "MLME_TRANSACTION_OVERFLOW";
        case MLME_UNAVAILABLE_KEY:
            return "MLME_UNAVAILABLE_KEY";
        case MLME_UNSUPPORTED_ATTRIBUTE:
            return "MLME_UNSUPPORTED_ATTRIBUTE";
        case MLME_INVALID_ADDRESS:
            return "MLME_INVALID_ADDRESS";
        case MLME_INVALID_INDEX:
            return "MLME_INVALID_INDEX";
        case MLME_LIMIT_REACHED:
            return "MLME_LIMIT_REACHED";
        case MLME_READ_ONLY:
            return "MLME_READ_ONLY";
        case MLME_SCAN_IN_PROGRESS:
            return "MLME_SCAN_IN_PROGRESS";
        case MLME_DATA_POLL_NOTIFICATION:
            return "MLME_DATA_POLL_NOTIFICATION";
        default:
            return NULL;
    }
}

int channel_from_mask(uint32_t channel_mask, int index)
{
    int expected_index = 0;
    for (int i = 0; i < 27; ++i) {
        if ((channel_mask >> i) & 1) {
            if (expected_index == index) {
                return i;
            }
            ++expected_index;
        }
    }
    return -1;
}
