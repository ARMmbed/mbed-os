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
#include "NFCTestShim.h"

#if MBED_CONF_NFCEEPROM
#include "NFCEEPROM.h"
#include "EEPROMDriver.h"


/**
 * Wrapper class handles events specific to the EEPROM driver.
 */
class NFCProcessEEPROM : NFCTestShim, mbed::nfc::NFCEEPROM::Delegate {
public:
    NFCProcessEEPROM(events::EventQueue &queue, mbed::nfc::NFCEEPROMDriver &eeprom_driver);
    virtual nfc_err_t init();
    void queue_write_call();
    void queue_write_long_call();
    void queue_read_call();
    void queue_erase_call();
    virtual void cmd_get_max_ndef();


private:
    virtual void on_ndef_message_written(nfc_err_t result);
    virtual void on_ndef_message_read(nfc_err_t result);
    virtual void parse_ndef_message(const mbed::Span<const uint8_t> &buffer);
    virtual size_t build_ndef_message(const mbed::Span<uint8_t> &buffer);
    virtual void on_ndef_message_erased(nfc_err_t result);
private:
    mbed::nfc::NFCEEPROM _eeprom;
    mbed::nfc::NFCEEPROMDriver *_ptr_eeprom_driver;
};

#endif //  eeprom

#endif // _NFCPROCESS_H_INCLUDED
