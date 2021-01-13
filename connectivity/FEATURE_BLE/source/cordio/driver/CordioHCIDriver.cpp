/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble/driver/CordioHCIDriver.h"
#include "hci_api.h"
#include "hci_cmd.h"
#include "hci_core.h"
#include "bstream.h"
#include "hci_mbed_os_adaptation.h"

#include "mbed-trace/mbed_trace.h"
#include "common/ble_trace_helpers.h"

#define TRACE_GROUP "BLHC"

#define HCI_RESET_RAND_CNT        4

namespace ble {

namespace {

static void hciCoreReadMaxDataLen(void)
{
    /* if LE Data Packet Length Extensions is supported by Controller and included */
    if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_DATA_LEN_EXT) &&
        (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_DATA_LEN_EXT)) {
        /* send next command in sequence */
        HciLeReadMaxDataLen();
    } else {
        /* send next command in sequence */
        HciLeRandCmd();
    }
}

static void hciCoreReadResolvingListSize(void)
{
    /* if LL Privacy is supported by Controller and included */
    if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_PRIVACY) &&
        (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_PRIVACY)) {
        /* send next command in sequence */
        HciLeReadResolvingListSize();
    } else {
        hciCoreCb.resListSize = 0;

        /* send next command in sequence */
        hciCoreReadMaxDataLen();
    }
}

} // end of anonymous namespace


CordioHCIDriver::CordioHCIDriver(CordioHCITransportDriver& transport_driver) :
    _transport_driver(transport_driver) { }

void CordioHCIDriver::initialize()
{
    tr_info("CordioHCIDriver initializing");
    _transport_driver.initialize();
    do_initialize();
}

void CordioHCIDriver::terminate()
{
    tr_info("CordioHCIDriver terminating");
    do_terminate();
    _transport_driver.terminate();
}

buf_pool_desc_t CordioHCIDriver::get_default_buffer_pool_description()
{
    static union {
        uint8_t buffer[2250];
        uint64_t align;
    };
    static const wsfBufPoolDesc_t pool_desc[] = {
        {  16, 16 },
        {  32, 16 },
        {  64, 8 },
        { 128, 4 },
        { 272, 1 }
    };

    return buf_pool_desc_t(buffer, pool_desc);
}

void CordioHCIDriver::set_random_static_address(const ble::address_t& address)
{
    ble_error_t err = BLE::Instance().gap().setRandomStaticAddress(address);
    MBED_ASSERT(err == BLE_ERROR_NONE);
}


void CordioHCIDriver::start_reset_sequence()
{
    /* send an HCI Reset command to start the sequence */
    HciResetCmd();
}

void CordioHCIDriver::handle_reset_sequence(uint8_t *pMsg)
{
    uint16_t       opcode;
    static uint8_t randCnt;

    /* if event is a command complete event */
    if (*pMsg == HCI_CMD_CMPL_EVT) {
        /* parse parameters */
        pMsg += HCI_EVT_HDR_LEN;
        pMsg++;                   /* skip num packets */
        BSTREAM_TO_UINT16(opcode, pMsg);
        pMsg++;                   /* skip status */

        /* decode opcode */
        switch (opcode) {
            case HCI_OPCODE_RESET:
                /* initialize rand command count */
                randCnt = 0;

                /* send next command in sequence */
                HciSetEventMaskCmd((uint8_t *) hciEventMask);
                break;

            case HCI_OPCODE_SET_EVENT_MASK:
                /* send next command in sequence */
                HciLeSetEventMaskCmd((uint8_t *) hciLeEventMask);
                break;

            case HCI_OPCODE_LE_SET_EVENT_MASK:
                /* send next command in sequence */
                HciSetEventMaskPage2Cmd((uint8_t *) hciEventMaskPage2);
                break;

            case HCI_OPCODE_SET_EVENT_MASK_PAGE2:
                /* send next command in sequence */
                HciReadBdAddrCmd();
                break;

            case HCI_OPCODE_READ_BD_ADDR: {
                /* parse and store event parameters */
                BdaCpy(hciCoreCb.bdAddr, pMsg);

                ble::address_t static_address;

                if (get_random_static_address(static_address)) {
                    // note: will send the HCI command to send the random address
                    set_random_static_address(static_address.data());
                } else {
                    /* send next command in sequence */
                    HciLeReadBufSizeCmd();
                }
                break;
            }

            case HCI_OPCODE_LE_SET_RAND_ADDR:
                /* send next command in sequence */
                HciLeReadBufSizeCmd();
                break;

            case HCI_OPCODE_LE_READ_BUF_SIZE:
                /* parse and store event parameters */
                BSTREAM_TO_UINT16(hciCoreCb.bufSize, pMsg);
                BSTREAM_TO_UINT8(hciCoreCb.numBufs, pMsg);

                /* initialize ACL buffer accounting */
                hciCoreCb.availBufs = hciCoreCb.numBufs;

                /* send next command in sequence */
                HciLeReadSupStatesCmd();
                break;

            case HCI_OPCODE_LE_READ_SUP_STATES:
                /* parse and store event parameters */
                memcpy(hciCoreCb.leStates, pMsg, HCI_LE_STATES_LEN);

                /* send next command in sequence */
                HciLeReadWhiteListSizeCmd();
                break;

            case HCI_OPCODE_LE_READ_WHITE_LIST_SIZE:
                /* parse and store event parameters */
                BSTREAM_TO_UINT8(hciCoreCb.whiteListSize, pMsg);

                /* send next command in sequence */
                HciLeReadLocalSupFeatCmd();
                break;

            case HCI_OPCODE_LE_READ_LOCAL_SUP_FEAT:
                /* parse and store event parameters */
                BSTREAM_TO_UINT16(hciCoreCb.leSupFeat, pMsg);

                /* send next command in sequence */
                hciCoreReadResolvingListSize();
                break;

            case HCI_OPCODE_LE_READ_RES_LIST_SIZE:
                /* parse and store event parameters */
                BSTREAM_TO_UINT8(hciCoreCb.resListSize, pMsg);

                /* send next command in sequence */
                hciCoreReadMaxDataLen();
                break;

            case HCI_OPCODE_LE_READ_MAX_DATA_LEN: {
                    uint16_t maxTxOctets;
                    uint16_t maxTxTime;

                    BSTREAM_TO_UINT16(maxTxOctets, pMsg);
                    BSTREAM_TO_UINT16(maxTxTime, pMsg);

                    /* use Controller's maximum supported payload octets and packet duration times
                    * for transmission as Host's suggested values for maximum transmission number
                    * of payload octets and maximum packet transmission time for new connections.
                    */
                    HciLeWriteDefDataLen(maxTxOctets, maxTxTime);
            }   break;

            case HCI_OPCODE_LE_WRITE_DEF_DATA_LEN:
                if (hciCoreCb.extResetSeq) {
                    /* send first extended command */
                    HciReadLocalVerInfoCmd();
                } else {
                    /* initialize extended parameters */
                    hciCoreCb.maxAdvDataLen = 0;
                    hciCoreCb.numSupAdvSets = 0;
                    hciCoreCb.perAdvListSize = 0;

                    /* send next command in sequence */
                    HciLeRandCmd();
                }
                break;

            case HCI_OPCODE_READ_LOCAL_VER_INFO:
            case HCI_OPCODE_LE_READ_MAX_ADV_DATA_LEN:
            case HCI_OPCODE_LE_READ_NUM_SUP_ADV_SETS:
            case HCI_OPCODE_LE_READ_PER_ADV_LIST_SIZE:
                if (hciCoreCb.extResetSeq) {
                    /* send next extended command in sequence */
                    (*hciCoreCb.extResetSeq)(pMsg, opcode);
                }
                break;

            case HCI_OPCODE_LE_RAND:
                /* check if need to send second rand command */
                if (randCnt < (HCI_RESET_RAND_CNT-1)) {
                    randCnt++;
                    HciLeRandCmd();
                } else {
                    /* last command in sequence; set resetting state and call callback */
                    signal_reset_sequence_done();
                }
                break;

            default:
                break;
        }
    }
}

bool CordioHCIDriver::get_random_static_address(ble::address_t& address)
{
    return false;
}

void CordioHCIDriver::signal_reset_sequence_done()
{
    tr_debug("Signal cordio HCI reset sequence done");
    hci_mbed_os_signal_reset_sequence_done();
}

uint16_t CordioHCIDriver::write(uint8_t type, uint16_t len, uint8_t *pData)
{
#if MBED_CONF_CORDIO_TRACE_HCI_PACKETS
    const char *type_prefix;
    switch(type) {
        case 0x01: type_prefix = "CMD"; break;
        case 0x02: type_prefix = "ACL"; break;
        /* illegal packet */
        default: type_prefix = "ERR"; break;
    }
    tr_debug("HOST->LL %s %s", type_prefix, trace_array(pData, len));
#endif
    return _transport_driver.write(type, len, pData);
}

void CordioHCIDriver::on_host_stack_inactivity()
{
}

void CordioHCIDriver::handle_test_end(bool success, uint16_t packets)
{
    MBED_ASSERT(_test_end_handler);
    tr_info("LE receiver mode ended (success: %s)", to_string(success));

    if (_test_end_handler) {
        _test_end_handler(success, packets);
        _test_end_handler = nullptr;
    }
}

ble_error_t CordioHCIDriver::rf_test_start_le_receiver_test(
    test_end_handler_t test_end_handler, uint8_t channel
)
{
    if (_test_end_handler) {
        tr_warning("Cannot start LE %s test mode - already started", "receiver");
        return BLE_ERROR_INVALID_STATE;
    }

    if (!test_end_handler) {
        tr_warning("Cannot start LE %s  test mode - invalid handler", "receiver");
        return BLE_ERROR_INVALID_PARAM;
    }

    _test_end_handler = test_end_handler;
    uint8_t *buf = hciCmdAlloc(HCI_OPCODE_LE_RECEIVER_TEST, HCI_LEN_LE_RECEIVER_TEST);

    if (buf) {
        tr_info("LE %s mode starter on channel %hhd", "receiver", channel);

        uint8_t* p = buf + HCI_CMD_HDR_LEN;
        UINT8_TO_BSTREAM(p, channel);
        hciCmdSend(buf);

        return BLE_ERROR_NONE;
    }

    return BLE_ERROR_NO_MEM;
}

ble_error_t CordioHCIDriver::rf_test_start_le_transmitter_test(
    test_end_handler_t test_end_handler, uint8_t channel, uint8_t length, uint8_t type
)
{
    if (_test_end_handler) {
        tr_warning("Cannot start LE %s test mode - already started", "transmitter");
        return BLE_ERROR_INVALID_STATE;
    }

    if (!test_end_handler) {
        tr_warning("Cannot start LE %s  test mode - invalid handler", "transmitter");
        return BLE_ERROR_INVALID_PARAM;
    }

    _test_end_handler = test_end_handler;
    uint8_t *buf = hciCmdAlloc(HCI_OPCODE_LE_TRANSMITTER_TEST, HCI_LEN_LE_TRANSMITTER_TEST);

    if (buf) {
        tr_info("LE %s mode starter on channel %hhd", "transmitter", channel);

        uint8_t* p = buf + HCI_CMD_HDR_LEN;
        UINT8_TO_BSTREAM(p, channel);
        UINT8_TO_BSTREAM(p, length);
        UINT8_TO_BSTREAM(p, type);
        hciCmdSend(buf);

        return BLE_ERROR_NONE;
    }

    return BLE_ERROR_NO_MEM;
}

ble_error_t CordioHCIDriver::rf_test_end()
{
    MBED_ASSERT(_test_end_handler);

    if (!_test_end_handler) {
        tr_info("Cannot end LE transmitter mode test - missing handler (test never started?)");
        return BLE_ERROR_INVALID_STATE;
    }

    uint8_t *buf = hciCmdAlloc(HCI_OPCODE_LE_TEST_END, HCI_LEN_LE_TEST_END);

    if (buf) {
        hciCmdSend(buf);
        tr_info("LE transmitter mode ended");

        return BLE_ERROR_NONE;
    }

    return BLE_ERROR_NO_MEM;
}

ble_error_t CordioHCIDriver::set_tx_power(int8_t level_db)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

} // namespace ble
