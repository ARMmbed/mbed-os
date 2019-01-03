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
#include <string>
#include <vector>
#include <stdlib.h>
#include "mbed.h"
#include "mbed_events.h"
#include "mbed-client-cli/ns_cmdline.h"
#include "rtos\Thread.h"

#include "nfctestshim.h"

#include "nfccommands.h"

events::EventQueue nfcQueue;
Thread nfcThread;
NFCTestShim * pNFC_Test_Shim = NULL;

NFCTestShim* new_testshim() {
#if MBED_CONF_NFCEEPROM
    mbed::nfc::NFCEEPROMDriver& eeprom_driver = get_eeprom_driver(nfcQueue);

    return ( (NFCTestShim *)(new NFCProcessEEPROM(nfcQueue, eeprom_driver)) );
#else
    return ((NFCTestShim *) (new NFCProcessController(nfcQueue)));
#endif // EEPROM

}

void nfcRoutine() {
    nfcQueue.dispatch_forever();
}

HandleTestCommand::HandleTestCommand() {
    osStatus status = nfcThread.start(callback(&nfcRoutine));
    MBED_ASSERT(status == osOK);
}

int HandleTestCommand::cmd_set_last_nfc_error(int argc, char *argv[]) {
    if (argc <= 1) {
        cmd_printf("setlastnfcerror() invalid parameter(s)\r\n");
        return (CMDLINE_RETCODE_INVALID_PARAMETERS);
    } else {
        int value = strtol(argv[1], NULL, 10);
        nfcQueue.call(NFCTestShim::cmd_set_last_nfc_error, value);
    }
    return (CMDLINE_RETCODE_EXCUTING_CONTINUE);
}

int HandleTestCommand::cmd_init_nfc(int argc, char *argv[]) {

    if (pNFC_Test_Shim) {
        cmd_printf("WARN init called again!\r\n"); // only legal here, if eeprom driver stops talking
    } else {
        pNFC_Test_Shim = new_testshim();
    }
    nfcQueue.call(pNFC_Test_Shim, &NFCTestShim::cmd_init);
    return (CMDLINE_RETCODE_EXCUTING_CONTINUE);
}

int HandleTestCommand::cmd_read_message(int argc, char *argv[]) {
    nfcQueue.call(pNFC_Test_Shim, &NFCTestShim::cmd_read_nfceeprom);

    return (CMDLINE_RETCODE_EXCUTING_CONTINUE);
}

int HandleTestCommand::cmd_set_smartposter(int argc, char *argv[]) {
    if (argc <= 1) {
        cmd_printf("setlastnfcerror() invalid parameter(s)\r\n");
        return (CMDLINE_RETCODE_INVALID_PARAMETERS);
    } else {
        // parse arg and queue it up
        char * uri = (char*) malloc(strlen(argv[1]) + 1);
        if (uri) {
            strcpy(uri, argv[1]);
            nfcQueue.call(pNFC_Test_Shim, &NFCTestShim::cmd_set_smartposter,
                    uri); // called thread must free
        }
		else {
            cmd_printf("WARN out of memory!\r\n");
            return (CMDLINE_RETCODE_FAIL);
		}
    }
    return (CMDLINE_RETCODE_EXCUTING_CONTINUE);
}

// todo: jira IOTPAN-295
int HandleTestCommand::cmd_erase(int argc, char *argv[]) {
    nfcQueue.call(pNFC_Test_Shim, &NFCTestShim::cmd_erase);
    return (CMDLINE_RETCODE_EXCUTING_CONTINUE);
}

int HandleTestCommand::cmd_write_long_ndef_message(int argc, char *argv[]) {
    size_t length, idx, sourceLength;
    static const char alphabet[] = "thequickbrownfoxjumpedoverthelazydog";
    char *data;
    const char *sourceMessage;

    // expect 2 or 3 args "<cmd> <length> [optional-text]"
    if (argc < 2) {
        cmd_printf("supply length of message\r\n");
        return (CMDLINE_RETCODE_INVALID_PARAMETERS);
    }

    int converted = sscanf(argv[1], "%d", &length);
    if (1 != converted) {
        cmd_printf("Cannot convert value to int\r\n");
        return (CMDLINE_RETCODE_INVALID_PARAMETERS);
    }
    data = (char*) malloc(length + 1);
	if (!data) {
        cmd_printf("WARN out of memory!\r\n");
        return (CMDLINE_RETCODE_FAIL);
    }
    if (argc > 2) {
        // user provided text to copy into text NDEF record
        sourceMessage = argv[2];
    } else {
        // use our internal default message to copy into the text NDEF
        sourceMessage = alphabet;
    }
    sourceLength = strlen(sourceMessage);
    for (idx = 0; idx < length; idx++) {
        data[idx] = sourceMessage[idx % sourceLength];
    }
    data[length] = '\0';

    // method must release buffer
    nfcQueue.call(pNFC_Test_Shim, &NFCTestShim::cmd_write_long, data);
    return (CMDLINE_RETCODE_EXCUTING_CONTINUE);
}

int HandleTestCommand::cmd_start_discovery(int argc, char *argv[]) {
    if ((argc > 1) && (0 == strcmp(argv[1], "man"))) {
        cmd_printf("User must restart discovery manually()\r\n");
        nfcQueue.call(pNFC_Test_Shim, &NFCTestShim::cmd_start_discovery, false);
    } else {
        cmd_printf("App will restart discovery loop on auto()\r\n");
        nfcQueue.call(pNFC_Test_Shim, &NFCTestShim::cmd_start_discovery, true);
    }
    return (CMDLINE_RETCODE_EXCUTING_CONTINUE);
}

int HandleTestCommand::cmd_stop_discovery(int argc, char *argv[]) {
    nfcQueue.call(pNFC_Test_Shim, &NFCTestShim::cmd_stop_discovery);
    return (CMDLINE_RETCODE_EXCUTING_CONTINUE);
}

int HandleTestCommand::cmd_get_supported_rf_protocols(int argc, char *argv[]) {
    nfcQueue.call(pNFC_Test_Shim, &NFCTestShim::cmd_get_rf_protocols);
    return (CMDLINE_RETCODE_EXCUTING_CONTINUE);
}

bool HandleTestCommand::set_protocol_target(
        nfc_rf_protocols_bitmask_t & bitmask, const char *protocolName) {
    bool parsed = false;
    if (0 == strcmp(protocolName, "t1t")) {
        parsed = bitmask.target_t1t = true;
    }
    if (0 == strcmp(protocolName, "t2t")) {
        parsed = bitmask.target_t2t = true;
    }
    if (0 == strcmp(protocolName, "t3t")) {
        parsed = bitmask.target_t3t = true;
    }
    if (0 == strcmp(protocolName, "t5t")) {
        parsed = bitmask.target_t5t = true;
    }
    if (0 == strcmp(protocolName, "isodep")) {
        parsed = bitmask.target_iso_dep = true;
    }
    if (0 == strcmp(protocolName, "nfcdep")) {
        parsed = bitmask.target_nfc_dep = true;
    }
    return (parsed);
}

int HandleTestCommand::cmd_configure_rf_protocols(int argc, char *argv[]) {
    nfc_rf_protocols_bitmask_t protocols = { 0 };

    int argindex = argc;
    while (argindex > 1) {
        if (!set_protocol_target(protocols, argv[argindex - 1])) {
            cmd_printf("Unknown protocol %s", argv[argindex - 1]);
            return (CMDLINE_RETCODE_INVALID_PARAMETERS);
        }
        argindex--;
    }
    nfcQueue.call(pNFC_Test_Shim, &NFCTestShim::cmd_configure_rf_protocols,
            protocols);
    return (CMDLINE_RETCODE_EXCUTING_CONTINUE);
}

// todo: implement
int cmd_start_stop_discovery_wait_tag(int argc, char *argv[]) {

    return (CMDLINE_RETCODE_COMMAND_NOT_IMPLEMENTED);
}

/////////////////////////////////////////////////////////////////////
// boilerplate only

int cmd_is_iso7816_supported(int argc, char *argv[]) {
    return (CMDLINE_RETCODE_COMMAND_NOT_IMPLEMENTED);
}

int cmd_add_iso7816_application(int argc, char *argv[]) {
    return (CMDLINE_RETCODE_COMMAND_NOT_IMPLEMENTED);
}

int cmd_set_tagtype(int argc, char *argv[]) {
    return (CMDLINE_RETCODE_COMMAND_NOT_IMPLEMENTED);
}

int cmd_get_tagtype(int argc, char *argv[]) {
    return (CMDLINE_RETCODE_COMMAND_NOT_IMPLEMENTED);
}

