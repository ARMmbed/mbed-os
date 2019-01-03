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

#ifndef _NFCCOMMANDS_H_INCLUDED
#define _NFCCOMMANDS_H_INCLUDED

#if MBED_CONF_NFCEEPROM
#include "nfcProcessEeprom.h"
#else
#include "nfcProcessCtrl.h"
#endif

extern events::EventQueue nfcQueue;

// see https://support.microsoft.com/en-my/help/208427/maximum-url-length-is-2-083-characters-in-internet-explorer
#define MAX_URL_LENGTH  2000

class HandleTestCommand {
public:
    // start thread and handle queue
    HandleTestCommand();
    /* set corresponding mask bit on, return false if the supplied string cannot parse */
    static bool set_protocol_target(nfc_rf_protocols_bitmask_t & bitmask, const char *protocolName);

    /* return and clear the last result code. Type "help getlastnfcerror" for a list of error codes */
    static int cmd_get_last_nfc_error(int argc, char *argv[]) {
        nfcQueue.call(NFCTestShim::cmd_get_last_nfc_error);
        return (CMDLINE_RETCODE_EXCUTING_CONTINUE);
    }

    /* internal function to test getlastnfcerror */
    static int cmd_set_last_nfc_error(int argc, char *argv[]);

    /* compile time flag */
    static int cmd_get_conf_nfceeprom(int argc, char *argv[]) {
        nfcQueue.call(NFCTestShim::cmd_get_conf_nfceeprom);
        return (CMDLINE_RETCODE_EXCUTING_CONTINUE);
    }

    /* must be called before invoking any other calls */
    static int cmd_init_nfc(int argc, char *argv[]);
    /* write a smartposter url, 'Sp' NDEF to the target */
    static int cmd_set_smartposter(int argc, char *argv[]);
    /* erase EEPROM */
    static int cmd_erase(int argc, char *argv[]);

    // controller driver methods:
    static int cmd_get_supported_rf_protocols(int argc, char *argv[]);
    static int cmd_configure_rf_protocols(int argc, char *argv[]);
    static int cmd_start_discovery(int argc, char *argv[]);
    static int cmd_stop_discovery(int argc, char *argv[]);

    /* read raw EEPROM contents */
    static int cmd_read_message(int argc, char *argv[]);
    /* write a text 'T' NDEF message to the target */
    static int cmd_write_long_ndef_message(int argc, char *argv[]);

};


// un-implemented or sparse support in drivers, so not covered
int cmd_is_iso7816_supported(int argc, char *argv[]);
int cmd_add_iso7816_application(int argc, char *argv[]);
int cmd_set_tagtype(int argc, char *argv[]);
int cmd_get_tagtype(int argc, char *argv[]);

#endif // _NFCCOMMANDS_H_INCLUDED
