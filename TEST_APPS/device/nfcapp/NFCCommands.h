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
#include "NFCProcessEEPROM.h"
#else
#include "NFCProcessCtrl.h"
#endif

// see https://support.microsoft.com/en-my/help/208427/maximum-url-length-is-2-083-characters-in-internet-explorer
#define MAX_URL_LENGTH  2000

#ifndef MBED_CONF_NFCEEPROM
#define MBED_CONF_NFCEEPROM false
#endif


/**
 * \brief adds human-readable traces not needed by the framework, filter is via a switch set_trace_enable()
 * \param fmt standard printf formatter
 * \param varargs
 */
void trace_printf(const char *fmt, ...);

/**
 * \brief turns on human readable traces
 * \param enabled : set to false to disable the extra traces
 */
void set_trace_enable(bool enabled = true);

/**
 * HandleTestCommand turns all the typed-in/serial commands into function calls sent via a eventqueue to the driver
 * shim/wrapper class.
 * Methods with cmd_ prefix map to the serial commands, and are building blocks for test cases and scenarios. The
 * first function a test must call is typically the initnfc command. Commands report back a test verdict, and a
 * NFC status code. The test verdict is always success unless the command is not allowed. Tests much check the
 * NFC error code for NFC_OK or zero; this pattern allows us to write negative tests which expect a specific NFC error.
 *
 * Handlers are statics because the test framework is not supporting C++
 */
class HandleTestCommand {
public:
    HandleTestCommand();

    /** return and clear the last result code. Type "help getlastnfcerror" for a list of error codes */
    static int cmd_get_last_nfc_error(int argc, char *argv[]);

    /** internal function to test getlastnfcerror */
    static int cmd_set_last_nfc_error(int argc, char *argv[]);

    /** returns compile time flag if NFC EEPROM was compiled */
    static int cmd_get_conf_nfceeprom(int argc, char *argv[]);

    static int cmd_set_trace(int argc, char *argv[]);

    /** For EEPROM, returns the driver max_ndef value, else returns the app config MBED_CONF_APP_TEST_NDEF_MSG_MAX */
    static int cmd_get_max_ndef(int argc, char *argv[]);

    /** Init must be called before invoking any other calls, obtains a driver reference and initializes driver */
    static int cmd_init_nfc(int argc, char *argv[]);
    /** write a smartposter url, 'Sp' NDEF to the target */
    static int cmd_set_smartposter(int argc, char *argv[]);
    /** erases the EEPROM if present */
    static int cmd_erase(int argc, char *argv[]);

    /** Returns a CSV list of protocols supported */
    static int cmd_get_supported_rf_protocols(int argc, char *argv[]);
    /** Sets the protocols supported (unimplemented) */
    static int cmd_configure_rf_protocols(int argc, char *argv[]);
    /** starts the NFC discovery loop if controller, has no effect on EEPROM */
    static int cmd_start_discovery(int argc, char *argv[]);
    /** stops the NFC discovery loop if controller */
    static int cmd_stop_discovery(int argc, char *argv[]);

    /** read raw EEPROM contents, reads a buffer if a controller is used */
    static int cmd_read_message(int argc, char *argv[]);
    /** write a text 'T' NDEF message to the target */
    static int cmd_write_long_ndef_message(int argc, char *argv[]);

private:
    /** set corresponding mask bit on in referenced structure, return false if the supplied string cannot parse */
    static bool set_protocol_target(mbed::nfc::nfc_rf_protocols_bitmask_t &bitmask, const char *protocolName);
    static NFCTestShim *new_testshim();
    static void nfc_routine();

    static events::EventQueue _nfcQueue;
};


// un-implemented or sparse support in drivers, so not covered
int cmd_is_iso7816_supported(int argc, char *argv[]);
int cmd_add_iso7816_application(int argc, char *argv[]);
int cmd_set_tagtype(int argc, char *argv[]);
int cmd_get_tagtype(int argc, char *argv[]);

#endif // _NFCCOMMANDS_H_INCLUDED
