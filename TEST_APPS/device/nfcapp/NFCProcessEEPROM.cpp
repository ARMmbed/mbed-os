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
#include <stdlib.h>
#include "mbed_events.h"
#include "mbed-client-cli/ns_cmdline.h"

#include "nfc/stack/nfc_errors.h"
#include "nfc/ndef/common/Text.h"
#include "nfc/ndef/common/URI.h"
#include "nfc/ndef/common/Mime.h"
#include "nfc/ndef/MessageBuilder.h"
#include "nfc/ndef/common/util.h"
#if MBED_CONF_NFCEEPROM
#include "NFCEEPROMDriver.h"

#include "NFCProcessEEPROM.h"
#include "SmartPoster.h"

using mbed::Span;
using mbed::nfc::ndef::MessageBuilder;
using mbed::nfc::ndef::RecordType;
using mbed::nfc::ndef::Record;
using mbed::nfc::ndef::RecordID;
using mbed::nfc::ndef::RecordPayload;
using mbed::nfc::ndef::common::span_from_cstr;
using mbed::nfc::ndef::common::Mime;
using mbed::nfc::ndef::common::Text;
using mbed::nfc::ndef::common::URI;
using mbed::nfc::NFCEEPROM;
using mbed::nfc::NFCEEPROMDriver;

// implements : mbed::nfc::NFCEEPROM::Delegate
NFCProcessEEPROM::NFCProcessEEPROM(events::EventQueue &queue, NFCEEPROMDriver &eeprom_driver) :
    NFCTestShim(queue), _eeprom(&eeprom_driver, &queue, _ndef_buffer), _ptr_eeprom_driver(&eeprom_driver)
{}

nfc_err_t NFCProcessEEPROM::init()
{
    nfc_err_t err = _eeprom.initialize();
    if (err != NFC_OK) {
        cmd_printf("NFCProcessEEPROM::init() (error: %d)!\r\n", err);
        _queue.break_dispatch();
    } else {
        cmd_printf("NFCProcessEEPROM::init() OK\r\n");
    }
    _eeprom.set_delegate(this);
    return (err);
}


void NFCProcessEEPROM::cmd_get_max_ndef()
{
    cmd_printf("{{maxndef=%d}}\r\n", (int)_ptr_eeprom_driver->read_max_size());
    cmd_ready(CMDLINE_RETCODE_SUCCESS);
}

void NFCProcessEEPROM::queue_write_call()
{
    cmd_printf("NFCProcessEEPROM::queue_write_call() entry\r\n");
    _queue.call(&_eeprom, &NFCEEPROM::write_ndef_message);
}

void NFCProcessEEPROM::queue_read_call()
{
    cmd_printf("NFCProcessEEPROM::queue_read_call() entry\r\n");
    _queue.call(&_eeprom, &NFCEEPROM::read_ndef_message);
}

void NFCProcessEEPROM::queue_erase_call()
{
    cmd_printf("NFCProcessEEPROM::queue_erase_call() entry\r\n");
    _queue.call(&_eeprom, &NFCEEPROM::erase_ndef_message);
}

void NFCProcessEEPROM::on_ndef_message_written(nfc_err_t result)
{
    set_last_nfc_error(result);
    if (result == NFC_OK) {
        cmd_printf("message written successfully\r\n");
    } else {
        cmd_printf("Failed to write (error: %d)!\r\n", result);
    }
    // complete the async test method here
    cmd_ready(CMDLINE_RETCODE_SUCCESS);
}

void NFCProcessEEPROM::on_ndef_message_read(nfc_err_t result)
{
    set_last_nfc_error(result);
    if (result == NFC_OK) {
        cmd_printf("message read successfully\r\n");
    } else {
        cmd_printf("Failed to read (error: %d)!\r\n", result);
    }
    // complete the async test method here
    cmd_ready(CMDLINE_RETCODE_SUCCESS);
}

void NFCProcessEEPROM::on_ndef_message_erased(nfc_err_t result)
{
    set_last_nfc_error(result);
    if (result == NFC_OK) {
        cmd_printf("message erased successfully\r\n");
    } else {
        cmd_printf("Failed to erase (error: %d)!\r\n", result);
    }
    // complete the async test method here
    cmd_ready(CMDLINE_RETCODE_SUCCESS);
}

void NFCProcessEEPROM::parse_ndef_message(const Span<const uint8_t> &buffer)
{
    cmd_printf("Received an ndef message of size %d\r\n", buffer.size());
    print_ndef_message(buffer, buffer.size());
}

size_t NFCProcessEEPROM::build_ndef_message(const Span<uint8_t> &buffer)
{
    cmd_printf("Copying ndef message %d bytes into buffer\r\n", _ndef_write_buffer_used);
    // make a copy into our buffer
    memcpy(buffer.data(), _ndef_write_buffer, _ndef_write_buffer_used);
    for (size_t k = 0; k < _ndef_write_buffer_used; k++) {
        cmd_printf("%02x ", buffer[k]);
    }
    return _ndef_write_buffer_used;
}

#endif // MBED_CONF_NFCEEPROM

