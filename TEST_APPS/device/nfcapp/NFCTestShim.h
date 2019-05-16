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
#include "nfc/NFCDefinitions.h"

/**
 * Test app driver wrapper. This is a base class containing shared EEPROM and Controller test data + logic.
 * Variations for the 2 different kinds of driver supported are delegated to derived classes.
 */
class NFCTestShim {
public:
    NFCTestShim(events::EventQueue &queue);

    static void cmd_get_last_nfc_error();
    static void cmd_set_last_nfc_error(int err);
    static void cmd_get_conf_nfceeprom();
    /**
     * For an EEPROM, this queries and responds with the flash size,
     * For a Controller, responds with the config macro TEST_NDEF_MSG_MAX
     */
    virtual void cmd_get_max_ndef() = 0;

    static void get_last_nfc_error();
    static void set_last_nfc_error(int err);
    static void get_conf_nfceeprom();
    static void print_ndef_message(const mbed::Span<const uint8_t> &buffer,
                                   size_t length);
    static mbed::nfc::ndef::common::URI::uri_identifier_code_t get_ndef_record_type(char const *url);
    static char const *get_ndef_record_type_prefix(mbed::nfc::ndef::common::URI::uri_identifier_code_t id);

    void cmd_init();

    void cmd_set_smartposter(char *cmdUri);
    void cmd_erase();
    void cmd_write_long(char *text_string);
    void cmd_read_nfc_contents();
    void cmd_start_discovery(bool manual = false);
    void cmd_stop_discovery();
    void cmd_configure_rf_protocols(mbed::nfc::nfc_rf_protocols_bitmask_t protocols);
    void cmd_get_rf_protocols();

protected:
    // implement/declare EEPROM and Controller model underlying common BH and delegate specializations
    virtual nfc_err_t init() = 0;
    virtual nfc_err_t set_rf_protocols(mbed::nfc::nfc_rf_protocols_bitmask_t protocols)
    {
        return NFC_ERR_UNSUPPORTED ;
    };
    virtual mbed::nfc::nfc_rf_protocols_bitmask_t get_rf_protocols()
    {
        mbed::nfc::nfc_rf_protocols_bitmask_t none;
        memset((void *)&none, 0, sizeof(none));
        return none;
    };
    virtual nfc_err_t start_discovery()
    {
        return NFC_ERR_UNSUPPORTED ;
    };
    virtual nfc_err_t stop_discovery()
    {
        return NFC_ERR_UNSUPPORTED ;
    };
    void set_discovery_restart_auto()
    {
        _discovery_restart = true;
    };
    void set_discovery_restart_manual()
    {
        _discovery_restart = false;
    };

protected:
    size_t _ndef_write_buffer_used;
    mbed::Span<uint8_t> _ndef_poster_message; // message to build and send
    uint8_t _ndef_write_buffer[MBED_CONF_APP_TEST_NDEF_MSG_MAX]; // if this buffer is smaller than the EEPROM, the driver may crash see IOTPAN-297
    uint8_t _ndef_buffer[MBED_CONF_APP_TEST_NDEF_MSG_MAX];       // driver I/O buffer
    bool _discovery_restart;            // default true, restart discovery loop again on remote disconnect
    events::EventQueue &_queue;
private:
    static int last_nfc_error;
};

// forward declare single instance
extern NFCTestShim *pNFC_Test_Shim;

#endif // _NFCTESTSHIM_H_INCLUDED
