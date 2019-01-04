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
#include "mbed-client-cli/ns_cmdline.h"
#include "nfc/ndef/common/Text.h"
#include "nfc/ndef/common/URI.h"
#include "nfc/ndef/common/Mime.h"
#include "nfc/ndef/MessageBuilder.h"
#include "nfc/ndef/common/util.h"
#include "nfc/nfcdefinitions.h"

#include "NFCEEPROMDriver.h"
#include "nfcCommands.h"
#include "nfcTestShim.h"
#include "SmartPoster.h"

using mbed::Span;
using mbed::nfc::ndef::MessageBuilder;
using mbed::nfc::ndef::RecordType;
using mbed::nfc::ndef::common::span_from_cstr;
using mbed::nfc::ndef::common::Text;
using mbed::nfc::ndef::common::URI;
using mbed::nfc::nfc_rf_protocols_bitmask_t;

// statics
namespace {
char long_string[0x2000];

int last_nfc_error = 0;
#if MBED_CONF_NFCEEPROM
int using_eeprom = true;
#else
int using_eeprom = false;
#endif
}

NFCTestShim::NFCTestShim() :
    _ndef_write_buffer_used(0), ndef_poster_message(_ndef_write_buffer),
    _discovery_restart(true) // on disconnect, will restart discovery
{
}

/** \brief The last failed NFC API call status, gets cleared upon reading it.
  * \return void The NFC error is set asyncronously by sending text back over serial
  */
void NFCTestShim::get_last_nfc_error()
{
    int last = ::last_nfc_error;
    ::last_nfc_error = 0;
    // return data as text to the plugin framework
    cmd_printf("{{lastnfcerror=%d}}\r\n", last);
    cmd_ready(CMDLINE_RETCODE_SUCCESS);
}

void NFCTestShim::set_last_nfc_error(int err)
{
    ::last_nfc_error = err;
    cmd_printf("\r\n{{lastnfcerror=%d}}\r\n", ::last_nfc_error);
}

// if an NFC EEPROM driver is configured
void NFCTestShim::get_conf_nfceeprom()
{
    set_last_nfc_error(NFC_OK);
    // return data as text to the plugin framework
    cmd_printf("{{iseeprom=%s}}\r\n", (::using_eeprom ? "true" : "false"));
    cmd_ready(CMDLINE_RETCODE_SUCCESS);
}

void NFCTestShim::print_ndef_message(const Span<const uint8_t> &buffer,
                                     size_t length)
{
    // return data as text to the plugin framework
    cmd_printf("{{nfcmessage=");
    for (size_t k = 0; k < length; k++) {
        cmd_printf("%02x ", buffer.data()[k]);
    }
    cmd_printf("}}\r\n");
}

void NFCTestShim::cmd_init()
{
    nfc_err_t ret = init();
    set_last_nfc_error(ret);

    if (NFC_OK != ret) {
        cmd_ready(CMDLINE_RETCODE_FAIL);
    } else {
        cmd_ready(CMDLINE_RETCODE_SUCCESS);
    }
}

void NFCTestShim::cmd_get_rf_protocols()
{
#if MBED_CONF_NFCEEPROM
    cmd_printf("EEPROM cannot get protocol()\r\n");
    set_last_nfc_error(NFC_ERR_UNSUPPORTED);
    cmd_ready(CMDLINE_RETCODE_INVALID_PARAMETERS);

#else
    nfc_rf_protocols_bitmask_t protocols = get_rf_protocols();

    static char strSupported[7 * 6 + 1] = "";
    if (protocols.target_t1t) {
        strcat(strSupported, "t1t,");
    }
    if (protocols.target_t2t) {
        strcat(strSupported, "t2t,");
    }
    if (protocols.target_t3t) {
        strcat(strSupported, "t3t,");
    }
    if (protocols.target_iso_dep) {
        strcat(strSupported, "isodep,");
    }
    if (protocols.target_nfc_dep) {
        strcat(strSupported, "nfcdep,");
    }
    if (protocols.target_t5t) {
        strcat(strSupported, "t5t,");
    }
    if (strlen(strSupported)) {
        strSupported[strlen(strSupported) - 1] = '\0'; // strip trailing comma
    }
    // return data as text to the plugin framework
    cmd_printf("{{protocols=%s}}", strSupported);
    set_last_nfc_error(NFC_OK);
    cmd_ready(CMDLINE_RETCODE_SUCCESS);
#endif
}

void NFCTestShim::cmd_configure_rf_protocols(
    nfc_rf_protocols_bitmask_t protocols)
{
#if MBED_CONF_NFCEEPROM
    cmd_printf("EEPROM cannot set protocol()\r\n");
    set_last_nfc_error(NFC_ERR_UNSUPPORTED);
    cmd_ready(CMDLINE_RETCODE_INVALID_PARAMETERS);
#else

    nfc_err_t err = set_rf_protocols(protocols);
    set_last_nfc_error(err);
    if (NFC_OK != err) {
        cmd_ready(CMDLINE_RETCODE_FAIL);
    } else {
        cmd_ready(CMDLINE_RETCODE_SUCCESS);
    }
#endif
}

/** \brief Copy data from the Controller buffer, or if EEPROM will initiate a read of the
  * eeprom contents which get dumped as a string {{bytes=XX XX XX XX.. }} and parsed by
  * the framework
  * \return void An ICETEA error code and NFC error is set asyncronously
  */
void NFCTestShim::cmd_read_nfceeprom()
{
#if MBED_CONF_NFCEEPROM
    ((NFCProcessEEPROM *)this)->queue_read_call();
    cmd_printf("NFCTestShim::read_nfceeprom() exit\r\n");

#else
    // returns last message "written", since we cannot read
    print_ndef_message(_ndef_write_buffer, _ndef_write_buffer_used);
    set_last_nfc_error(NFC_OK);

    cmd_printf("NFCTestShim::read_nfceeprom()\r\n");
    cmd_ready(CMDLINE_RETCODE_SUCCESS);
#endif
}

void NFCTestShim::cmd_erase()
{

#if MBED_CONF_NFCEEPROM
    ((NFCProcessEEPROM *)this)->queue_erase_call();

#else
    cmd_printf("erase %d bytes, last msg\r\n",
               sizeof(_ndef_write_buffer));
    _ndef_write_buffer_used = 0;
    memset(_ndef_write_buffer, 0, sizeof(_ndef_write_buffer));
    set_last_nfc_error(NFC_OK); // effectively a no-op
    cmd_ready(CMDLINE_RETCODE_SUCCESS);
#endif
}

/** \brief Writes a Text T record buffer with really long message - length checks to be done by driver only.
  * If an NFC controller, no write to the chip happens, we copy the data into a Controller buffer
  * \param uri This method must free the passed in pointer
  * \return void An ICETEA error code and NFC error is set asyncronously
  */
void NFCTestShim::cmd_write_long(char *data)
{
    MessageBuilder builder(ndef_poster_message);

    strcpy(::long_string, data); //max_ndef - header - overheads
    Text text(Text::UTF8, span_from_cstr("en-US"),
              span_from_cstr((const char *)(::long_string)));

    text.append_as_record(builder, true);
    _ndef_write_buffer_used = builder.get_message().size();
    cmd_printf("Composed NDEF message %d bytes\r\n", _ndef_write_buffer_used);

#if MBED_CONF_NFCEEPROM
    ((NFCProcessEEPROM *)this)->queue_write_call();
#else
    // not on a wire, so the caller will store the message in a buffer
    set_last_nfc_error(NFC_OK);
    cmd_ready(CMDLINE_RETCODE_SUCCESS);
#endif

    cmd_printf("NFCTestShim::write_long() exit\r\n");
    free(data);
}

/** \brief Write a URI Use case would be to prompt to install an app from the appstore using the tag
  * \param uri This method must free the passed in pointer
  * \return void An ICETEA error code and NFC error is set asyncronously
  */
void NFCTestShim::cmd_set_smartposter(char *cmdUri)
{
    MessageBuilder builder(ndef_poster_message);

    uint8_t smart_poster_buffer[1024];
    MessageBuilder smart_poster_builder(smart_poster_buffer);

    char *urlbegin = strstr(cmdUri, ".");
    urlbegin++;
    URI uri(URI::HTTPS_WWW, span_from_cstr(urlbegin));
    uri.append_as_record(smart_poster_builder, true);

    builder.append_record(
        RecordType(RecordType::well_known_type, span_from_cstr("Sp")),
        smart_poster_builder.get_message(), true);

    _ndef_write_buffer_used = builder.get_message().size();
    cmd_printf("Composed NDEF message %d bytes\r\n", _ndef_write_buffer_used);

#if MBED_CONF_NFCEEPROM
    ((NFCProcessEEPROM *)this)->queue_write_call();
#else
    // not on a wire, so the call just stores the message in a buffer
    set_last_nfc_error(NFC_OK);
    cmd_ready(CMDLINE_RETCODE_SUCCESS);
#endif
    cmd_printf("NFCTestShim::setsmartposter() exit\r\n");
    free(cmdUri);
}

// disabled in EEPROMs, overridden if controller present
void NFCTestShim::cmd_start_discovery(bool manual)
{
#if MBED_CONF_NFCEEPROM
    cmd_printf("EEPROM cannot start_discovery()\r\n");
    set_last_nfc_error(NFC_ERR_UNSUPPORTED);
    cmd_ready(CMDLINE_RETCODE_INVALID_PARAMETERS);

#else

    // todo: Jira logged. remove hard coded protocol
    nfc_rf_protocols_bitmask_t protocols = { 0 };
    protocols.target_iso_dep = 1;

    nfc_err_t err = set_rf_protocols(protocols);

    if (manual) {
        set_discovery_restart_manual();
    } else {
        set_discovery_restart_auto();
    }
    err = start_discovery();
    set_last_nfc_error(err);
    if (NFC_OK != err) {
        cmd_ready(CMDLINE_RETCODE_FAIL);
    } else {
        cmd_ready(CMDLINE_RETCODE_SUCCESS);
    }
#endif
}

// disabled in EEPROMs, overridden if controller present
void NFCTestShim::cmd_stop_discovery()
{

#if MBED_CONF_NFCEEPROM
    cmd_printf("EEPROM cannot stop_discovery()\r\n");
    set_last_nfc_error(NFC_ERR_UNSUPPORTED);
    cmd_ready(CMDLINE_RETCODE_INVALID_PARAMETERS);
#else
    nfc_err_t err = stop_discovery();
    set_last_nfc_error(err);
    if (NFC_OK != err) {
        cmd_ready(CMDLINE_RETCODE_FAIL);
    } else {
        cmd_ready(CMDLINE_RETCODE_SUCCESS);
    }

#endif
}

