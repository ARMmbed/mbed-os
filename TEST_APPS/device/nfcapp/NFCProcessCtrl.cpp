/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <stdarg.h>
#include <cstring>
#include <string.h>
#include "mbed_events.h"
#include "mbed-client-cli/ns_cmdline.h"

#include "nfc/stack/nfc_errors.h"

#include "nfc/ndef/common/Text.h"
#include "nfc/ndef/common/URI.h"
#include "nfc/ndef/common/Mime.h"
#include "nfc/ndef/MessageBuilder.h"
#include "nfc/ndef/common/util.h"


#include "NFCCommands.h"
#include "NFCProcessCtrl.h"
#include "SmartPoster.h"

using mbed::Span;
#if ! MBED_CONF_NFCEEPROM
using mbed::nfc::nfc_rf_protocols_bitmask_t;
using mbed::nfc::ndef::MessageBuilder;
using mbed::nfc::ndef::RecordType;
using mbed::nfc::ndef::Record;
using mbed::nfc::ndef::RecordID;
using mbed::nfc::ndef::RecordPayload;
using mbed::nfc::ndef::common::span_from_cstr;
using mbed::nfc::ndef::common::Mime;
using mbed::nfc::ndef::common::Text;
using mbed::nfc::ndef::common::URI;
using mbed::nfc::NFCController;

/**
 * Implements NFCRemoteInitiator::Delegate, NFCController::Delegate
 */

NFCProcessController::NFCProcessController(events::EventQueue &queue) :
    NFCTestShim(queue),
    // pins: mosi, miso, sclk, ssel, irq, rst
    _pn512_transport(D11, D12, D13, D10, A1, A0), _pn512_driver(
        &_pn512_transport),
    _nfc_controller(&_pn512_driver, &queue, _ndef_buffer)
{
}

/**
 * Initialise and configure the NFC controller.
 *
 * @return NFC_OK in case of success or a meaningful error code in case of
 * failure.
 */
nfc_err_t NFCProcessController::init()
{
    trace_printf("init()\r\n");

    // register callbacks
    _nfc_controller.set_delegate(this);
    return _nfc_controller.initialize();
}


void NFCProcessController::cmd_get_max_ndef()
{
    cmd_printf("{{maxndef=%d}}\r\n", (int)MBED_CONF_APP_TEST_NDEF_MSG_MAX);
    cmd_ready(CMDLINE_RETCODE_SUCCESS);
}

/**
 * Start the discovery of peers.
 *
 * @return NFC_OK in case of success or a meaningful error code in case of
 * failure.
 */
nfc_err_t NFCProcessController::start_discovery()
{
    trace_printf("start_discovery()\r\n");

    return _nfc_controller.start_discovery();
}

/**
 * Stop discovery.
 *
 * @return NFC_OK in case of success or a meaningful error code in case of
 * failure.
 */
nfc_err_t NFCProcessController::stop_discovery()
{
    trace_printf("stop_discovery()\r\n");
    return _nfc_controller.cancel_discovery();
}

nfc_rf_protocols_bitmask_t NFCProcessController::get_rf_protocols()
{
    trace_printf("get_supported_rf_protocols()\r\n");
    return _nfc_controller.get_supported_rf_protocols();
}

nfc_err_t NFCProcessController::set_rf_protocols(
    nfc_rf_protocols_bitmask_t protocols)
{
    trace_printf("configure_rf_protocols()\r\n");

    return _nfc_controller.configure_rf_protocols(protocols);
}


/* ------------------------------------------------------------------------
 * Implementation of NFCRemoteInitiator::Delegate
 */
void NFCProcessController::on_connected()
{
    cmd_printf("on_connected()\r\n");
}

void NFCProcessController::on_disconnected()
{
    cmd_printf("on_disconnected()\r\n");

    // reset the state of the remote initiator
    _nfc_remote_initiator->set_delegate(NULL);
    _nfc_remote_initiator.reset();

    // restart peer discovery
    _nfc_controller.start_discovery();
}

void NFCProcessController::parse_ndef_message(
    const Span<const uint8_t> &buffer)
{
    size_t len = buffer.size();
    // copy remotely written message into our dummy buffer
    if (len <= sizeof(_ndef_write_buffer)) {
        trace_printf("Store remote ndef message of size %d\r\n", len);
        memcpy(_ndef_write_buffer, buffer.data(), len);
        _ndef_write_buffer_used = len;
    } else {
        trace_printf("Remote ndef message of size %d too large!\r\n", len);
    }
}

size_t NFCProcessController::build_ndef_message(const Span<uint8_t> &buffer)
{
    trace_printf("Copying message %d bytes to query buffer\r\n",
                 _ndef_write_buffer_used);
    memcpy(buffer.data(), _ndef_write_buffer, _ndef_write_buffer_used);
    for (size_t k = 0; k < _ndef_write_buffer_used; k++) {
        cmd_printf("%02x ", buffer[k]);
    }
    return _ndef_write_buffer_used;
}

const char *NFCProcessController::str_discovery_terminated_reason(
    nfc_discovery_terminated_reason_t reason)
{
    static const char *reasons[4] = { "completed", "cancelled", "rf error"};
    switch (reason) {
        case nfc_discovery_terminated_completed :
        case nfc_discovery_terminated_canceled:
        case nfc_discovery_terminated_rf_error:
            return reasons[reason];
    }
    return "unexpected!";
}


void NFCProcessController::on_discovery_terminated(
    nfc_discovery_terminated_reason_t reason)
{
    trace_printf("on_discovery_terminated(%s)\r\n",
                 str_discovery_terminated_reason(reason));
    if (reason != nfc_discovery_terminated_completed
            && this->_discovery_restart) {
        start_discovery();
    }
}

void NFCProcessController::on_nfc_initiator_discovered(
    const SharedPtr<mbed::nfc::NFCRemoteInitiator> &nfc_initiator)
{
    trace_printf("on_nfc_initiator_discovered()\r\n");

    // setup the local remote initiator
    _nfc_remote_initiator = nfc_initiator;
    _nfc_remote_initiator->set_delegate(this);
    _nfc_remote_initiator->connect();
}

#endif // #if ! MBED_CONF_NFCEEPROM

