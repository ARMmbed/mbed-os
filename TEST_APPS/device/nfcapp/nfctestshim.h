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

#ifndef _NFCTESTSHIM_H_INCLUDED
#define _NFCTESTSHIM_H_INCLUDED

#include <string>
#include <vector>
#include <stdio.h>
#include <stdarg.h>

#include "mbed_config.h"
#include "nfc/ndef/MessageBuilder.h"
#include "nfc/ndef/common/URI.h"
#include "nfc/ndef/common/util.h"
#include "nfc/nfcdefinitions.h"

// all targets that have an EEPROM
#if MBED_CONF_NFCEEPROM
#define TEST_NFCEEPROM_TARGET
#endif

// all targets that have a controller
#if defined (TARGET_PN512)
#define TEST_NFCCONTRL_TARGET
#endif

#if MBED_CONF_NFCEEPROM
#include "NFCEEPROM.h"
#include "EEPROMDriver.h"

#else
#ifdef TARGET_PN512
#include "nfc/controllers/PN512Driver.h"
#include "nfc/controllers/PN512SPITransportDriver.h"
#endif
#include "nfc/NFCRemoteInitiator.h"
#include "nfc/NFCController.h"

using mbed::Span;
using mbed::nfc::NFCRemoteInitiator;
using mbed::nfc::NFCController;
#endif // TEST_EEPROM_TARGET

using mbed::nfc::ndef::MessageBuilder;
using mbed::nfc::ndef::common::URI;
using mbed::nfc::ndef::common::span_from_cstr;
using mbed::nfc::nfc_rf_protocols_bitmask_t;

class NFCTestShim {
public:
    NFCTestShim();

    static void cmd_get_last_nfc_error() {
        get_last_nfc_error();
    }
    ;
    static void cmd_set_last_nfc_error(int err) {
        set_last_nfc_error(err);
        cmd_ready (CMDLINE_RETCODE_SUCCESS);
    }
    ;
    static void cmd_get_conf_nfceeprom() {
        get_conf_nfceeprom();
    }
    ;
    static void get_last_nfc_error();
    static void set_last_nfc_error(int err);
    static void get_conf_nfceeprom();
    static void print_ndef_message(const Span<const uint8_t> &buffer,
            size_t length);

    void cmd_init();
    virtual nfc_err_t init() = 0;

    void cmd_set_smartposter(char *cmdUri);
    void cmd_erase();
    void cmd_write_long(char *data);
    void cmd_read_nfceeprom();
    void cmd_start_discovery(bool manual = false);
    void cmd_stop_discovery();
    void cmd_configure_rf_protocols(nfc_rf_protocols_bitmask_t protocols);
    void cmd_get_rf_protocols();

protected:
    // implement/declare EEPROM and Controller model underlying common BH and delegate specializations
    virtual nfc_err_t set_rf_protocols(nfc_rf_protocols_bitmask_t protocols) {return NFC_ERR_UNSUPPORTED ;};
    virtual nfc_err_t start_discovery() {return NFC_ERR_UNSUPPORTED ;};
    virtual nfc_err_t stop_discovery() {return NFC_ERR_UNSUPPORTED ;};
    void set_discovery_restart_auto() {
        _discovery_restart = true;
    };
    void set_discovery_restart_manual() {
        _discovery_restart = false;
    };


protected:
    size_t _ndef_write_buffer_used;
    Span<uint8_t> ndef_poster_message; // message to build and send
    uint8_t _ndef_write_buffer[0x2000]; // if this buffer is smaller than the EEPROM, the driver may crash see IOTPAN-297
    uint8_t _ndef_buffer[0x2000];       // driver buffer
    bool _discovery_restart;

private:
    static int last_nfc_error;

    static int using_eeprom;
    static char long_string[0x2000];
};

// forward declare single instance
extern NFCTestShim * pNFC_Test_Shim;

#endif // _NFCTESTSHIM_H_INCLUDED
