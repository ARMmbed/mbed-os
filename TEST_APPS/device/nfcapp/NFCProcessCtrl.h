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

#ifndef _NFCPROCESS_H_INCLUDED
#define _NFCPROCESS_H_INCLUDED

#include <string>
#include <vector>
#include <stdio.h>
#include <stdarg.h>
#include "mbed_events.h"
#include "nfc/ndef/MessageBuilder.h"
#include "nfc/ndef/common/URI.h"
#include "nfc/ndef/common/util.h"
#include "NFCTestShim.h"

#if !MBED_CONF_NFCEEPROM

#include "nfc/nfcdefinitions.h"
#ifdef TARGET_PN512
#include "nfc/controllers/PN512Driver.h"
#include "nfc/controllers/PN512SPITransportDriver.h"
#endif
#include "nfc/NFCRemoteInitiator.h"
#include "nfc/NFCController.h"
#include "nfc/ndef/common/util.h"

/**
 * Wrapper class handles calls and callbacks for NFC controller drivers. Note, that users must call "start"
 * in order to start the discovery loop for controllers. An internal buffer stores the NFC message and records.
 */
class NFCProcessController: NFCTestShim,
    mbed::nfc::NFCRemoteInitiator::Delegate,
    mbed::nfc::NFCController::Delegate {
public:
    NFCProcessController(events::EventQueue &queue);

    void cmd_get_max_ndef();
    nfc_err_t init();
    nfc_err_t start_discovery();
    nfc_err_t stop_discovery();
    mbed::nfc::nfc_rf_protocols_bitmask_t get_rf_protocols();
    nfc_err_t set_rf_protocols(mbed::nfc::nfc_rf_protocols_bitmask_t protocols);

    virtual void parse_ndef_message(const mbed::Span<const uint8_t> &buffer);
    virtual size_t build_ndef_message(const mbed::Span<uint8_t> &buffer);
    const char *str_discovery_terminated_reason(
        nfc_discovery_terminated_reason_t reason);

private:
    // these events are handled, to restart discovery
    /**
     * Implementation of NFCRemoteEndpoint::Delegate */
    virtual void on_connected();
    /**
     * Implementation of NFCRemoteEndpoint::Delegate */
    virtual void on_disconnected();
    /**
     * Implementation of NFCController::Delegate */
    virtual void on_discovery_terminated(
        nfc_discovery_terminated_reason_t reason);
    /**
     * Implementation of NFCController::Delegate */
    virtual void on_nfc_initiator_discovered(
        const SharedPtr<mbed::nfc::NFCRemoteInitiator> &nfc_initiator);

private:
    mbed::nfc::PN512SPITransportDriver _pn512_transport;
    mbed::nfc::PN512Driver _pn512_driver;
    mbed::nfc::NFCController _nfc_controller;
    SharedPtr<mbed::nfc::NFCRemoteInitiator> _nfc_remote_initiator;
};
#endif // Controller

#endif // _NFCPROCESS_H_INCLUDED
