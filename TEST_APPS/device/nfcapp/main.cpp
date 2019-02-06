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
#include "platform/Span.h"
#include "mbed_events.h"
#include "mbed-client-cli/ns_cmdline.h"
#include "NFCTestShim.h"
#include "NFCCommands.h"

#if MBED_CONF_NFCEEPROM
using mbed::nfc::NFCEEPROM;
using mbed::nfc::NFCEEPROMDriver;
#else
#ifndef TARGET_PN512
#warning [NOT_SUPPORTED] NFC not supported for this target
#endif

#endif // MBED_CONF_NFCEEPROM

const char *errorcodes = // descriptions from nfc/stack/nfc_errors.h
    " 0 NFC_OK\r\n"
    " 1 NFC_ERR_UNKNOWN\r\n"
    " 2 NFC_ERR_LENGTH\r\n"
    " 3 NFC_ERR_NOT_FOUND\r\n"
    " 4 NFC_ERR_UNSUPPORTED\r\n"
    " 5 NFC_ERR_PARAMS\r\n"
    " 6 NFC_ERR_BUFFER_TOO_SMALL\r\n"
    " 7 NFC_ERR_TIMEOUT\r\n"
    " 8 NFC_ERR_CRC\r\n"
    " 9 NFC_ERR_NOPEER\r\n"
    "10 NFC_ERR_PARITY\r\n"
    "11 NFC_ERR_FIELD\r\n"
    "12 NFC_ERR_COLLISION\r\n"
    "13 NFC_ERR_WRONG_COMM\r\n"
    "14 NFC_ERR_PROTOCOL\r\n"
    "15 NFC_ERR_BUSY\r\n"
    "16 NFC_ERR_CONTROLLER\r\n"
    "17 NFC_ERR_HALTED\r\n"
    "18 NFC_ERR_MAC\r\n"
    "19 NFC_ERR_UNDERFLOW\r\n"
    "20 NFC_ERR_DISCONNECTED\r\n"
    "21 NFC_ERR_ABORTED\r\n";


void wrap_printf(const char *f, va_list a)
{
    vprintf(f, a);
}


/** Disables VT100 etc. for easy manual UI interaction */
int set_easy_printer(int argc, char *argv[])
{
    const char msg[][20] =
    { "echo off", "set --retcode true", "set --vt100 off" };
    for (size_t i = 0; i < (sizeof(msg) / sizeof(msg[0])); i++) {
        cmd_exe((char *) msg[i]);
    }
    return (CMDLINE_RETCODE_SUCCESS);
}


/**
 * This test app can be used standalone interactively with at 115200 baud terminal. It is designed to work with the
 * IceTea test framework https://os.mbed.com/docs/latest/tools/icetea-testing-applications.html . This app does
 * not run test cases, it wraps functions in the API in ways that let us test multiple cases and scenarios.
 * Test commands are routed in the CommandHandler class to an eventqueue, so it interfaces to the driver
 * on a separate driver thread, which means test functions complete asynchronously.
 * The driver is only called in a thread in the shim or wrapper class called NFCTestShim, which handles driver
 * callbacks and completes the async part of a test function. NFCTestShim has specializations for NFC controllers
 * and EEPROMS. The classes NFCProcessController and NFCProcessEEPROM respectively implement the needed specialization.
 *
 * If using an NFC EEPROM, an extra library is needed. Please see the documentation in the README.MD for instructions
 * on how to modify this test for new drivers/targets, and the steps to run this test suite.
 */
int main()
{
    cmd_init(&wrap_printf);
    HandleTestCommand handleCommands; // For handling test commands and set nfc message queue

    cmd_add("getlastnfcerror", HandleTestCommand::cmd_get_last_nfc_error,
            "last NFC error code", errorcodes);
    cmd_add("setlastnfcerror", HandleTestCommand::cmd_set_last_nfc_error,
            "self-test", "for self-test only");
    cmd_add("iseeprom", HandleTestCommand::cmd_get_conf_nfceeprom,
            "get NFC configEEPROM  present",
            "true if config exists, else false");
    cmd_add("initnfc", HandleTestCommand::cmd_init_nfc, "init NFC driver",
            "call first");
    cmd_add("getmaxndef", HandleTestCommand::cmd_get_max_ndef, "get max NDEF record target supports",
            "returns the eeprom size, or max buffer if a controller");
    cmd_add("init", HandleTestCommand::cmd_init_nfc, "alias initnfc",
            "call first");
    cmd_add("setsmartposter", HandleTestCommand::cmd_set_smartposter,
            "send smartposter NDEF", "<uri>");
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
    cmd_add("easy", set_easy_printer, "Use human readable terminal output",
            "echo off,vt100 off,return-codes visible");
    cmd_add("trace", HandleTestCommand::cmd_set_trace, "detailed tracing on/off, ",
            "Defaults to enabled; values like 'on','true','1' all turn it on, anything else turns off detailed tracing.");

#if MBED_CONF_NFCEEPROM
    cmd_printf("MBED NFC EEPROM defined\r\n");
#else
    cmd_printf("MBED NFC Controller tests\r\n");
#endif

#ifdef TARGET_M24SR
    cmd_printf("Using driver: M24SR\r\n");
#endif
#ifdef TARGET_PN512
    cmd_printf("Using driver: PN512\r\n");
#endif
    {
        int c;
        while ((c = getc(stdin)) != EOF) {
            cmd_char_input(c);
        }
    }
    return 0;
}
