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

#include "nfc/ndef/MessageBuilder.h"
#include "nfc/ndef/common/URI.h"
#include "nfc/ndef/common/util.h"
#include "nfcTestShim.h"

#if MBED_CONF_NFCEEPROM
#include "NFCEEPROM.h"
#include "EEPROMDriver.h"
#else
#include "nfc/nfcdefinitions.h"
#ifdef TARGET_PN512
#include "nfc/controllers/PN512Driver.h"
#include "nfc/controllers/PN512SPITransportDriver.h"
#endif
#include "nfc/NFCRemoteInitiator.h"
#include "nfc/NFCController.h"
#include "nfc/ndef/common/util.h"

#endif // MBED_CONF_NFCEEPROM

using mbed::nfc::ndef::MessageBuilder;
using mbed::nfc::ndef::common::URI;
using mbed::nfc::ndef::common::span_from_cstr;
using mbed::Span;


#if MBED_CONF_NFCEEPROM

using mbed::nfc::NFCEEPROM;
using mbed::nfc::NFCEEPROMDriver;

class NFCProcessEEPROM : NFCTestShim, mbed::nfc::NFCEEPROM::Delegate {
public:
    NFCProcessEEPROM(events::EventQueue &queue, NFCEEPROMDriver &eeprom_driver) ;
    nfc_err_t init();
    void queue_write_call();
    void queue_write_long_call();
    void queue_read_call();
    void queue_erase_call();

private:
    virtual void on_ndef_message_written(nfc_err_t result);
    virtual void on_ndef_message_read(nfc_err_t result);
    virtual void parse_ndef_message(const Span<const uint8_t> &buffer);
    virtual size_t build_ndef_message(const Span<uint8_t> &buffer);
    virtual void on_ndef_message_erased(nfc_err_t result);
private:
    uint8_t _ndef_buffer[0x2000]; // if this buffer is smaller than the EEPROM, the driver may crash see IOTPAN-297
    NFCEEPROM _eeprom;
    EventQueue &_queue;
};

#else //  NFC Controller

class NFCProcessController : NFCTestShim, NFCRemoteInitiator::Delegate, NFCController::Delegate {
public:
    NFCProcessController(events::EventQueue &queue);

    nfc_err_t init();
    nfc_err_t start_discovery();
    nfc_err_t stop_discovery();
    void set_discovery_restart_auto()
    {
        _discovery_restart = true;
    };
    void set_discovery_restart_manual()
    {
        _discovery_restart = false;
    };
    nfc_rf_protocols_bitmask_t get_rf_protocols();
    nfc_err_t set_rf_protocols(nfc_rf_protocols_bitmask_t protocols);

    virtual void parse_ndef_message(const Span<const uint8_t> &buffer);
    virtual size_t build_ndef_message(const Span<uint8_t> &buffer);
    const char *str_discovery_terminated_reason(nfc_discovery_terminated_reason_t reason);


private:
    // these events are handled, to restart discovery
    virtual void on_connected();
    virtual void on_disconnected();
    virtual void on_discovery_terminated(nfc_discovery_terminated_reason_t reason);
    virtual void on_nfc_initiator_discovered(const SharedPtr<NFCRemoteInitiator> &nfc_initiator);

private:
    bool _discovery_restart;
    uint8_t _ndef_buffer[1024];
    mbed::nfc::PN512SPITransportDriver _pn512_transport;
    mbed::nfc::PN512Driver _pn512_driver;
protected:
    EventQueue &_queue;
private:
    NFCController _nfc_controller;
    SharedPtr<NFCRemoteInitiator> _nfc_remote_initiator;
};
#endif // TARGET_M24SR / else TARGET_PN512


#endif // _NFCPROCESS_H_INCLUDED
