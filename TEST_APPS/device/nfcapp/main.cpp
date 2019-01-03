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
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <vector>
#include <stdlib.h> 
#include "mbed.h"
#include "mbed_events.h"
#include "mbed-client-cli/ns_cmdline.h"
#include "NFCEEPROMDriver.h"
#include "nfctestshim.h"
#include "nfccommands.h"
#include "smartposter.h"

#if MBED_CONF_NFCEEPROM
using mbed::nfc::NFCEEPROM;
using mbed::nfc::NFCEEPROMDriver;
#else
#ifndef TARGET_PN512
#warning [NOT_SUPPORTED] NFC not supported for this target
#endif

#include "nfc/controllers/PN512Driver.h"
#include "nfc/controllers/PN512SPITransportDriver.h"

#include "nfc/NFCRemoteInitiator.h"
#include "nfc/NFCController.h"

using mbed::nfc::NFCRemoteInitiator;
using mbed::nfc::NFCController;
using mbed::nfc::nfc_rf_protocols_bitmask_t;
#endif // MBED_CONF_NFCEEPROM

using mbed::Span;
using mbed::nfc::ndef::MessageBuilder;
using mbed::nfc::ndef::common::Text;
using mbed::nfc::ndef::common::URI;
using mbed::nfc::ndef::common::span_from_cstr;


void wrap_printf(const char *f, va_list a) {
    vprintf(f, a);
}

const char *errorcodes = // descriptions from nfc/stack/nfc_errors.h
        " 0 NFC_OK \n"
                " 1 NFC_ERR_UNKNOWN\n"
                " 2 NFC_ERR_LENGTH \n"
                " 3 NFC_ERR_NOT_FOUND\n"
                " 4 NFC_ERR_UNSUPPORTED\n"
                " 5 NFC_ERR_PARAMS \n"
                " 6 NFC_ERR_BUFFER_TOO_SMALL\n"
                " 7 NFC_ERR_TIMEOUT\n"
                " 8 NFC_ERR_CRC\n"
                " 9 NFC_ERR_NOPEER \n"
                "10 NFC_ERR_PARITY \n"
                "11 NFC_ERR_FIELD\n"
                "12 NFC_ERR_COLLISION\n"
                "13 NFC_ERR_WRONG_COMM \n"
                "14 NFC_ERR_PROTOCOL \n"
                "15 NFC_ERR_BUSY \n"
                "16 NFC_ERR_CONTROLLER \n"
                "17 NFC_ERR_HALTED \n"
                "18 NFC_ERR_MAC\n"
                "19 NFC_ERR_UNDERFLOW\n"
                "20 NFC_ERR_DISCONNECTED \n"
                "21 NFC_ERR_ABORTED\n";

// for easy manual UI interaction
int seteasy(int argc, char *argv[]) {
    const char msg[][20] =
            { "echo off", "set --retcode true", "set --vt100 off" };
    for (size_t i = 0; i < (sizeof(msg) / sizeof(msg[0])); i++) {
        cmd_exe((char*) msg[i]);
    }
    return (CMDLINE_RETCODE_SUCCESS);
}

int main(int argc, char *argv[]) {
    cmd_init(&wrap_printf);
    cmd_add("getlastnfcerror", HandleTestCommand::cmd_get_last_nfc_error,
            "last NFC error code", errorcodes);
    cmd_add("setlastnfcerror", HandleTestCommand::cmd_set_last_nfc_error,
            "self-test", "for self-test only");
    cmd_add("initnfc", HandleTestCommand::cmd_init_nfc, "init NFC driver",
            "call first");
    cmd_add("init", HandleTestCommand::cmd_init_nfc, "alias initnfc",
            "call first");
    cmd_add("setsmartposter", HandleTestCommand::cmd_set_smartposter,
            "send smartposter NDEF", "<uri>");
    cmd_add("iseeprom", HandleTestCommand::cmd_get_conf_nfceeprom,
            "get NFC configEEPROM  present",
            "true if config exists, else false");
    cmd_add("readmessage", HandleTestCommand::cmd_read_message,
            "read EEPROM else return last message", "returns hex dump");
    cmd_add("read", HandleTestCommand::cmd_read_message, "alias readmessage",
            "returns hex dump");
    cmd_add("erase", HandleTestCommand::cmd_erase,
            "erase EEPROM or clear last message", "erase entire flash/buffer");
    cmd_add("writelong", HandleTestCommand::cmd_write_long_ndef_message,
            "fill T NDEF with pattern up to <length>",
            "writelong <length> [messagetorepeat='thequickbrownfoxisadog']");
    cmd_add("start", HandleTestCommand::cmd_start_discovery,
            "start discovery [auto=autorestart|man=manual]",
            "loop restarts by default, man to disable");
    cmd_add("stop", HandleTestCommand::cmd_stop_discovery, "stop discovery",
            "[wait=0] with optional wait for session end");

    cmd_add("getprotocols", HandleTestCommand::cmd_get_supported_rf_protocols,
            "get supported protocols", "returns CSV list, see setprotocols");
    cmd_add("setprotocols", HandleTestCommand::cmd_configure_rf_protocols,
            "set rf protocols", "-p [t1t]/[t2t]/[t3t]/[isodep]/[nfcdep]/[t5t]");
    cmd_add("easy", seteasy, "Use human readable terminal output",
            "echo off,vt100 off,return-codes visible");

#if MBED_CONF_NFCEEPROM
    cmd_printf("MBED NFC EEPROM defined\r\n");
#else
    cmd_printf("MBED NFC Controller tests\r\n");
#endif

#ifdef TARGET_M24SR
    cmd_printf("Using driver:M24SR\r\n");
#endif
#ifdef TARGET_PN512
    cmd_printf("Using driver:PN512\r\n");
#endif

    int c;
    HandleTestCommand handleCommands; // starts handling nfc messages
    while ((c = getc(stdin)) != EOF) {
        cmd_char_input(c);
    }
    return 0;
}
