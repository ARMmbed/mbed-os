/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if MBED_CONF_CELLULAR_USE_SMS

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "ThisThread.h"
#include "AT_CellularSMS.h"
#include "CellularUtil.h"
#include "CellularLog.h"

using namespace mbed_cellular_util;
using namespace mbed;
using namespace std;
using namespace rtos;

#define CTRL_Z  "\x1a"
#define ESC     "\x1b"

const uint8_t SMS_STATUS_SIZE = 12 + 1;
const uint8_t FIRST_OCTET_DELIVER_SUBMIT = 17;
const uint8_t TP_VALIDITY_PERIOD_24_HOURS = 167;
const uint8_t TP_PROTOCOL_IDENTIFIER = 0;
const uint8_t SMS_DATA_CODING_SCHEME = 0;

const uint8_t SMS_MAX_8BIT_CONCATENATED_SINGLE_SMS_SIZE = 134;
const uint8_t SMS_MAX_GSM7_CONCATENATED_SINGLE_SMS_SIZE = 153;
#define NVAM '?' // Not Valid ascii, ISO-8859-1 mark

// mapping table from 7-bit GSM to ascii (ISO-8859-1)
static const unsigned char gsm_to_ascii[] = {
    64,     // 0
    163,    // 1
    36,     // 2
    165,    // 3
    232,    // 4
    233,    // 5
    249,    // 6
    236,    // 7
    242,    // 8
    199,    // 9
    10,     // 10
    216,    // 11
    248,    // 12
    13,     // 13
    197,    // 14
    229,    // 15
    NVAM,   // 16
    95,     // 17
    NVAM,   // 18
    NVAM,   // 19
    NVAM,   // 20
    NVAM,   // 21
    NVAM,   // 22
    NVAM,   // 23
    NVAM,   // 24
    NVAM,   // 25
    NVAM,   // 26
    27,     // 27
    198,    // 28
    230,    // 29
    223,    // 30
    201,    // 31
    32,     // 32
    33,     // 33
    34,     // 34
    35,     // 35
    164,    // 36
    37,     // 37
    38,     // 38
    39,     // 39
    40,     // 40
    41,     // 41
    42,     // 42
    43,     // 43
    44,     // 44
    45,     // 45
    46,     // 46
    47,     // 47
    48,     // 48
    49,     // 49
    50,     // 50
    51,     // 51
    52,     // 52
    53,     // 53
    54,     // 54
    55,     // 55
    56,     // 56
    57,     // 57
    58,     // 58
    59,     // 59
    60,     // 60
    61,     // 61
    62,     // 62
    63,     // 63
    161,    // 64
    65,     // 65
    66,     // 66
    67,     // 67
    68,     // 68
    69,     // 69
    70,     // 70
    71,     // 71
    72,     // 72
    73,     // 73
    74,     // 74
    75,     // 75
    76,     // 76
    77,     // 77
    78,     // 78
    79,     // 79
    80,     // 80
    81,     // 81
    82,     // 82
    83,     // 83
    84,     // 84
    85,     // 85
    86,     // 86
    87,     // 87
    88,     // 88
    89,     // 89
    90,     // 90
    196,    // 91
    214,    // 92
    209,    // 93
    220,    // 94
    167,    // 95
    191,    // 96
    97,     // 97
    98,     // 98
    99,     // 99
    100,    // 100
    101,    // 101
    102,    // 102
    103,    // 103
    104,    // 104
    105,    // 105
    106,    // 106
    107,    // 107
    108,    // 108
    109,    // 109
    110,    // 110
    111,    // 111
    112,    // 112
    113,    // 113
    114,    // 114
    115,    // 115
    116,    // 116
    117,    // 117
    118,    // 118
    119,    // 119
    120,    // 120
    121,    // 121
    122,    // 122
    228,    // 123
    246,    // 124
    241,    // 125
    252,    // 126
    224     // 127
};

const int GSM_TO_ASCII_TABLE_SIZE = sizeof(gsm_to_ascii) / sizeof(gsm_to_ascii[0]);

AT_CellularSMS::AT_CellularSMS(ATHandler &at, AT_CellularDevice &device) : _cb(), _mode(CellularSMSMmodeText),
    _use_8bit_encoding(false), _sim_wait_time(0), _sms_message_ref_number(1),
    _sms_info(NULL), _at(at), _device(device)
{
}

AT_CellularSMS::~AT_CellularSMS()
{
}

void AT_CellularSMS::cmt_urc()
{
    tr_debug("CMT_URC called");
    //+CMT: <oa>,[<alpha>],<scts>[,<tooa>,<fo>,<pid>,<dcs>,<sca>,<tosca>,<length>]<CR><LF><data>
    (void)_at.consume_to_stop_tag();
    // call user defined callback function
    if (_cb) {
        _cb();
    } else {
        tr_warn("cmt_urc, no user defined callback for receiving sms!");
    }
}

void AT_CellularSMS::cmti_urc()
{
    //+CMTI: <mem>,<index>,
    tr_debug("CMTI_URC called");
    // call user defined callback function
    if (_cb) {
        _cb();
    } else {
        tr_warn("cmti_urc, no user defined callback for receiving sms!");
    }
}

nsapi_error_t AT_CellularSMS::set_cnmi()
{
    if (!_device.get_property(AT_CellularDevice::PROPERTY_AT_CNMI)) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    return _at.at_cmd_discard("+CNMI", "=2,1");
}

nsapi_error_t AT_CellularSMS::set_cmgf(int msg_format)
{
    if (!_device.get_property(AT_CellularDevice::PROPERTY_AT_CMGF)) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    return _at.at_cmd_discard("+CMGF", "=", "%d", msg_format);
}

nsapi_error_t AT_CellularSMS::set_csmp(int fo, int vp, int pid, int dcs)
{
    if (!_device.get_property(AT_CellularDevice::PROPERTY_AT_CSMP)) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    return _at.at_cmd_discard("+CSMP", "=", "%d%d%d%d", fo, vp, pid, dcs);
}

nsapi_error_t AT_CellularSMS::set_csdh(int show_header)
{
    if (!_device.get_property(AT_CellularDevice::PROPERTY_AT_CSDH)) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    return _at.at_cmd_discard("+CSDH", "=", "%d", show_header);
}

nsapi_error_t AT_CellularSMS::initialize(CellularSMSMmode mode,
                                         CellularSMSEncoding encoding)
{
    _use_8bit_encoding = (encoding == CellularSMSEncoding8Bit);

    _at.set_urc_handler("+CMTI:", callback(this, &AT_CellularSMS::cmti_urc));
    _at.set_urc_handler("+CMT:", callback(this, &AT_CellularSMS::cmt_urc));

    _at.lock();
    set_cnmi();     //set new SMS indication
    set_cmgf(mode); //set message format/PDU

    if (mode == CellularSMSMmodeText) {
        set_csmp(FIRST_OCTET_DELIVER_SUBMIT, TP_VALIDITY_PERIOD_24_HOURS, TP_PROTOCOL_IDENTIFIER,
                 SMS_DATA_CODING_SCHEME);    //set Set Text Mode Parameters(default values for SMS-SUBMIT and RECEIVE)
        set_csdh(1);//set header extra info as it's needed
    }

    _mode = mode;

    return _at.unlock_return_error();
}

void AT_CellularSMS::set_extra_sim_wait_time(int sim_wait_time)
{
    _sim_wait_time = sim_wait_time;
}

char *AT_CellularSMS::create_pdu(const char *phone_number, const char *message, uint8_t message_length, uint8_t msg_parts,
                                 uint8_t msg_part_number, uint8_t &header_size)
{
    int totalPDULength = 0;
    const bool is_number_international = (phone_number[0] == '+');
    if (is_number_international) {
        ++phone_number;
    }
    int number_len = strlen(phone_number);

    totalPDULength += number_len;
    if (number_len & 0x01) { // if phone number length is not even length we must pad it and so +1
        totalPDULength += 1;
    }

    totalPDULength += 16; // all other than phone number and message length
    if (msg_parts > 1) {// add more space for UDH
        totalPDULength += 12;
    }
    // there might be need for padding so some more space
    totalPDULength += 2;

    // 8-bit message, converted to hex so it will take twice as much space
    totalPDULength += message_length * 2;

    // terminating nullbyte, because callers use strlen() to find out PDU size
    totalPDULength += 1;

    char *pdu = new char[totalPDULength];
    memset(pdu, 0, totalPDULength);
    int x = 0;
    // See more how to create PDU from 3GPP specification 23040
    // first two define that we use service center number which is set with +CSCA
    pdu[x++] = '0';
    pdu[x++] = '0';
    // First Octet of the TPDU. 41 means SMS SUBMIT, no validity period, no status report, use User Data Header.
    // 01 means SMS SUBMIT, no validity period, no status report, NO User Data Header.
    if (msg_parts > 1) { // concatenated, must use UDH
        pdu[x++] = '4';
    } else {
        pdu[x++] = '0';
    }
    pdu[x++] = '1';
    // assign a message reference automatically. We have defined TP-RD bit as 0 so duplicates are not rejected.
    pdu[x++] = '0';
    pdu[x++] = '0';
    // [6] and [7] Length of the Destination Phone Number
    int_to_hex_str(number_len, pdu + x);
    x += 2;
    // Type of the Destination Phone Number
    if (is_number_international) {
        pdu[x++] = '9'; // international
    } else {
        pdu[x++] = '8'; // unknown
    }
    pdu[x++] = '1';

    // phone number as reverse nibble encoded
    int i = 0;
    for (; i < number_len; i += 2) {
        if (i + 1 == number_len) {
            pdu[x++] = 'f';
        } else {
            pdu[x++] = phone_number[i + 1];
        }
        pdu[x++] = phone_number[i];
    }

    // Protocol Identifier
    pdu[x++] = '0';
    pdu[x++] = '0';
    // Data Coding Scheme, GSM 7-bit default alphabet = '00', 8-bit '04'
    pdu[x++] = '0';

    if (_use_8bit_encoding) {
        pdu[x++] = '4';
    } else {
        pdu[x++] = '0';
    }

    uint8_t udhlen = 0;
    // Length can be update after we have created PDU, store position for later use.
    int lengthPos = x;
    x += 2;

    int paddingBits = 0;
    if (msg_parts > 1) { // concatenated, must use UDH
        // user data header length in chars
        pdu[x++] = '0';
        udhlen = 6; // udh length in chars (5) + udhl length in chars
        pdu[x++] = '5';
        // Information element identifier
        pdu[x++] = '0';
        pdu[x++] = '0';
        //  Information element data length
        pdu[x++] = '0';
        pdu[x++] = '3';
        //  A reference number (must be the same for all parts of the same larger messages)
        int_to_hex_str(_sms_message_ref_number & 0xFF, pdu + x);
        x += 2;
        // How many parts does this message have?
        int_to_hex_str(msg_parts, pdu + x);
        x += 2;
        // this is a part number
        int_to_hex_str(msg_part_number, pdu + x);
        x += 2;

        // if there is padding bits then udhlen is octet bigger as we need to keep septet boundary
        paddingBits = (udhlen * 8) % 7;
        if (paddingBits) {
            paddingBits = 7 - paddingBits;
            udhlen += 1;
        }
    }

    if (_use_8bit_encoding) {
        char_str_to_hex_str(message, message_length, pdu + x);
    } else {
        // we might need to send zero length sms
        if (message_length) {
            if (pack_7_bit_gsm_and_hex(message, message_length, pdu + x, paddingBits) == 0) {
                delete [] pdu;
                return NULL;
            }
        }
    }

    // now we know the correct length of the UDL (User Data Length)
    int_to_hex_str(message_length + udhlen, pdu + lengthPos);
    header_size = x;

    return pdu;
}

nsapi_size_or_error_t AT_CellularSMS::send_sms(const char *phone_number, const char *message, int msg_len)
{
    int single_sms_max_length = _use_8bit_encoding ?
                                SMS_MAX_SIZE_8BIT_SINGLE_SMS_SIZE :
                                SMS_MAX_SIZE_GSM7_SINGLE_SMS_SIZE;
    if ((_mode == CellularSMSMmodeText && msg_len > single_sms_max_length) || !phone_number) {
        return NSAPI_ERROR_PARAMETER;
    }

    _at.lock();

    int write_size = 0;

    ThisThread::sleep_for(_sim_wait_time);

    if (_mode == CellularSMSMmodeText) {
        _at.cmd_start_stop("+CMGS", "=", "%s", phone_number);

        ThisThread::sleep_for(_sim_wait_time);
        _at.resp_start("> ", true);

        if (_at.get_last_error() == NSAPI_ERROR_OK) {
            write_size = _at.write_bytes((uint8_t *)message, msg_len);
            if (write_size < msg_len) {
                // sending can be cancelled by giving <ESC> character (IRA 27).
                _at.cmd_start(ESC);
                _at.cmd_stop();
                _at.unlock();
                return write_size;
            }
            // <ctrl-Z> (IRA 26) must be used to indicate the ending of the message body.
            _at.cmd_start(CTRL_Z);
            _at.cmd_stop();
            _at.resp_start("+CMGS:");
            _at.resp_stop();
        }
    } else {
        // supports uncompressed 8 bit data and GSM 7 bit default alphabet data. Current implementation uses only
        // GSM 7 bit default but support is done for 8 bit data.
        int sms_count;
        int concatenated_sms_length = _use_8bit_encoding ?
                                      SMS_MAX_8BIT_CONCATENATED_SINGLE_SMS_SIZE :
                                      SMS_MAX_GSM7_CONCATENATED_SINGLE_SMS_SIZE;

        if (msg_len <= single_sms_max_length) {
            // single message
            sms_count = 1;
        } else {
            // concatenated message
            sms_count = msg_len / concatenated_sms_length;
            if (msg_len % concatenated_sms_length != 0) {
                sms_count++;
            }
        }

        int remaining_len = msg_len;
        int pdu_len;
        int msg_write_len = 0;
        uint8_t header_len;
        char *pdu_str;
        for (int i = 0; i < sms_count; i++) {

            header_len = 0;
            if (sms_count == 1) {
                pdu_len = msg_len;
            } else {
                pdu_len = remaining_len > concatenated_sms_length ? concatenated_sms_length : remaining_len;
            }

            pdu_str = create_pdu(phone_number, message + i * concatenated_sms_length, pdu_len,
                                 sms_count, i + 1, header_len);
            if (!pdu_str) {
                _at.unlock();
                return NSAPI_ERROR_NO_MEMORY;
            }
            pdu_len = strlen(pdu_str);

            // specification says that service center number should not be included so we subtract -2 from pdu_len as we use '00' for automatic service center number

            _at.cmd_start_stop("+CMGS", "=", "%d", (pdu_len - 2) / 2);

            ThisThread::sleep_for(_sim_wait_time);
            _at.resp_start("> ", true);

            if (_at.get_last_error() == NSAPI_ERROR_OK) {
                write_size = _at.write_bytes((uint8_t *)pdu_str, pdu_len);
                if (write_size < pdu_len) {
                    // calculate exact size of what we have send
                    if (write_size <= header_len) {
                        // managed only to write header or some of it so actual msg write size in this iteration is 0
                        write_size = 0;
                    } else {
                        write_size = (write_size - header_len) / 2; // as hex encoded so divide by two
                    }
                    msg_write_len += write_size;

                    // sending can be cancelled by giving <ESC> character (IRA 27).
                    _at.cmd_start(ESC);
                    _at.cmd_stop();
                    _at.unlock();
                    delete [] pdu_str;
                    return msg_write_len;
                }

                // <ctrl-Z> (IRA 26) must be used to indicate the ending of the message body.
                _at.cmd_start(CTRL_Z);
                _at.cmd_stop();
                _at.resp_start("+CMGS:");
                _at.resp_stop();
            }
            delete [] pdu_str;
            remaining_len -= concatenated_sms_length;
            if (_at.get_last_error() != NSAPI_ERROR_OK) {
                return _at.unlock_return_error();
            }
        }
    }

    _sms_message_ref_number++;
    nsapi_error_t ret = _at.get_last_error();
    _at.unlock();

    return (ret == NSAPI_ERROR_OK) ? msg_len : ret;
}

void AT_CellularSMS::set_sms_callback(Callback<void()> func)
{
    _cb = func;
}

nsapi_error_t AT_CellularSMS::set_cpms(const char *memr, const char *memw, const char *mems)
{
    return _at.at_cmd_discard("+CPMS", "=", "%s%s%s", memr, memw, mems);
}

nsapi_error_t AT_CellularSMS::set_csca(const char *sca, int type)
{
    return _at.at_cmd_discard("+CSCA", "=", "%s%d", sca, type);
}

nsapi_size_or_error_t AT_CellularSMS::set_cscs(const char *chr_set)
{
    return _at.at_cmd_discard("+CSCS", "=", "%s", chr_set);
}

nsapi_error_t AT_CellularSMS::delete_sms(sms_info_t *sms)
{
    _at.lock();
    for (int i = 0; i < sms->parts; i++) {
        _at.at_cmd_discard("+CMGD", "=", "%d", sms->msg_index[i]);
    }

    return _at.unlock_return_error();
}

// we need this as for example concatenated sms can get different sms reference numbers
// if for example last part take much more time to arrive. This situation happened while testing.
// What this means that after this we can't read another sms because we always read the oldest sms
// that was corrupted. So we need to have delete all messages.
nsapi_error_t AT_CellularSMS::delete_all_messages()
{
    return _at.at_cmd_discard("+CMGD", "=1,4");
}

// read msg in text mode
nsapi_size_or_error_t AT_CellularSMS::read_sms_from_index(int msg_index, char *buf, uint16_t len, char *phone_num,
                                                          char *time_stamp)
{
    /*
     * +CMGR: <stat>,<oa>,<alpha>,<scts>[,<tooa>,<fo>,<pid>,<dcs>,<sca>,<tosca>,<length>]<CR><LF><data><CR><LF>OK<CR><LF>
     */
    ThisThread::sleep_for(_sim_wait_time);
    _at.cmd_start_stop("+CMGR", "=", "%d", msg_index);

    // TODO: NOTE:    If the selected <mem1> can contain different types of SMs (e.g. SMS-DELIVERs, SMS-SUBMITs, SMS-STATUS-REPORTs and SMS-COMMANDs),
    // the response may be a mix of the responses of different SM types. TE application can recognize the response format by examining the third response parameter.
    // for now we support sms reading of received messages
    int buf_len = 0;
    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        char status[SMS_STATUS_SIZE];
        // first we read msg status and with that we can decide how the rest of message format
        _at.resp_start("+CMGR:");

        if (_at.info_resp()) {
            _at.read_string(status, SMS_STATUS_SIZE);
            uint16_t status_len = strlen(status);
            if (((status_len == sizeof("REC READ") - 1) && memcmp("REC READ", status, status_len) == 0)
                    || ((status_len == sizeof("REC UNREAD") - 1) && memcmp("REC UNREAD", status, status_len) == 0)) {
                // Received message
                if (phone_num) {
                    _at.read_string(phone_num, SMS_MAX_PHONE_NUMBER_SIZE);
                } else {
                    _at.skip_param(); // <oa>,<alpha>
                }
                _at.skip_param(); // <alpha>
                if (time_stamp) {
                    int len = _at.read_string(time_stamp, SMS_MAX_TIME_STAMP_SIZE);
                    if (len < (SMS_MAX_TIME_STAMP_SIZE - 2)) {
                        time_stamp[len++] = ',';
                        _at.read_string(&time_stamp[len], SMS_MAX_TIME_STAMP_SIZE - len);
                    }
                }
                (void)_at.consume_to_stop_tag(); // consume until <CR><LF>
                if (buf) {
                    _at.read_string(buf, len);
                    buf_len = strlen(buf);
                }
            }
        }
        _at.resp_stop();
    }

    return (_at.get_last_error() == NSAPI_ERROR_OK) ? buf_len : _at.get_last_error();
}

// read msg in PDU mode
nsapi_size_or_error_t AT_CellularSMS::read_sms(sms_info_t *sms, char *buf, char *phone_num, char *time_stamp)
{
    // +CMGR: <stat>,[<alpha>],<length><CR><LF><pdu>
    int index;
    if (sms->parts == sms->parts_added) {
        char *pdu; // we need a temp buffer as payload is hexencoded ---> can't use buf as it might be enough for message but not hexenconded pdu.
        int status;
        int msg_len;
        index = 0;
        int pduSize;

        for (int i = 0; i < sms->parts; i++) {
            ThisThread::sleep_for(_sim_wait_time);
            _at.cmd_start_stop("+CMGR", "=", "%d", sms->msg_index[i]);
            _at.resp_start("+CMGR:");

            if (_at.info_resp()) {
                status = _at.read_int();
                _at.skip_param(); // <alpha>
                if ((_at.get_last_error() == NSAPI_ERROR_OK) && (status == 0 || status == 1)) {
                    msg_len = _at.read_int();
                    if (msg_len > 0) {
                        pduSize = msg_len * 2 + 20; // *2 as it's hex encoded and +20 as service center number is not included in size given by CMGR
                        pdu = new char[pduSize];
                        memset(pdu, 0, pduSize);
                        if (!pdu) {
                            _at.resp_stop();
                            return NSAPI_ERROR_NO_MEMORY;
                        }
                        _at.read_string(pdu, pduSize, true);
                        if (_at.get_last_error() == NSAPI_ERROR_OK) {
                            msg_len = get_data_from_pdu(pdu, NULL, NULL, phone_num, buf + index);
                            if (msg_len >= 0) { // we need to allow zero length messages
                                index += msg_len;
                            } else {
                                delete [] pdu;
                                _at.resp_stop();
                                return -1;
                            }
                        }
                        delete [] pdu;
                    }
                }
            }
            _at.resp_stop();
        }

        if (_at.get_last_error() == NSAPI_ERROR_OK) {
            if (time_stamp) {
                strcpy(time_stamp, sms->date);
            }
            buf[index] = '\0';
        }
    } else {
        tr_warn("NOT all concatenated parts were received...");
        index = SMS_ERROR_MULTIPART_ALL_PARTS_NOT_READ;
    }

    return index;
}

nsapi_size_or_error_t AT_CellularSMS::get_sms(char *buf, uint16_t len, char *phone_num, uint16_t phone_len,
                                              char *time_stamp, uint16_t time_len, int *buf_size)
{
    // validate buffer sizes already here to avoid any necessary function calls and locking of _at
    if ((phone_num && phone_len < SMS_MAX_PHONE_NUMBER_SIZE) || (time_stamp && time_len < SMS_MAX_TIME_STAMP_SIZE) ||
            buf == NULL) {
        return NSAPI_ERROR_PARAMETER;
    }

    _at.lock();

    nsapi_size_or_error_t err = list_messages();
    if (err == NSAPI_ERROR_OK) {
        // we return the oldest sms and delete it after successful read
        sms_info_t *info = get_oldest_sms_index();

        if (info) {
            if (info->msg_size > len) {
                tr_warn("Given buf too small, len is: %d but is must be: %d", len, info->msg_size);
                if (buf_size) {
                    *buf_size = info->msg_size;
                }
                free_linked_list();
                _at.unlock();
                return NSAPI_ERROR_PARAMETER;
            }

            if (_mode == CellularSMSMmodePDU) {
                err = read_sms(info, buf, phone_num, time_stamp);
            } else {
                err = read_sms_from_index(info->msg_index[0], buf, len, phone_num, time_stamp);
            }

            if (err > 0) {
                int delerr = delete_sms(info);
                if (delerr) {
                    err = delerr;
                }
            }
        } else {
            // No messages were found, return -1
            err = -1;
        }
    }

    free_linked_list();

    _at.unlock();

    // update error only when there really was an error, otherwise we return the length
    if (_at.get_last_error()) {
        err = _at.get_last_error();
    }
    return err;
}

nsapi_size_or_error_t AT_CellularSMS::get_data_from_pdu(const char *pdu, sms_info_t *info, int *part_number,
                                                        char *phone_number, char *msg)
{
    int index = 0;
    int tmp;
    bool userDataHeader;
    int oaLength;
    int dataScheme;
    nsapi_size_or_error_t err = NSAPI_ERROR_OK;

    // read Length of the SMSC information
    oaLength = hex_str_to_int(pdu, 2);
    index += 2; // length we just read
    index += oaLength * 2; // skip service center number

    // read first the lower part of first octet as there is message type
    index++;
    tmp = hex_str_to_int(pdu + index, 1);
    //ThisThread::sleep_for(200);
    if ((tmp & 0x03) == 0) {// SMS-DELIVER type, last two bits should be zero
        // UDH present? Check from first octets higher part
        tmp = hex_str_to_int(pdu + (--index), 1);
        userDataHeader = ((tmp & 0x04) == 0) ? false : true;

        index += 2; // we just read the high bits of first octet so move +2
        // originating address length
        oaLength = hex_str_to_int(pdu + index, 2);
        index += 2; // add  index over address length
        int type = hex_str_to_int(pdu + index, 1);
        index += 2; // add  index over type
        if (phone_number) {
            // phone number as reverse nibble encoded
            int a = 0, field_length = oaLength;

            if (type == 9) {
                //add the plus sign in case of international number (23.040 chapter address fields)
                phone_number[a++] = '+';
                field_length++;
            }

            for (; a < field_length; a += 2) {
                if ((a + 1) == field_length) {
                    phone_number[a] = pdu[index + 1];
                    index++;
                } else {
                    phone_number[a] = pdu[index + 1];
                    phone_number[a + 1] = pdu[index];
                    index += 2;
                }
            }
            phone_number[field_length] = '\0';
        } else {
            index += oaLength;
        }


        if (oaLength & 0x01) { // if phone number length is odd then it has padded F so skip that
            index++;
        }
        index += 2; // skip TP-Protocol identifier

        dataScheme = hex_str_to_int(pdu + index, 2);
        index += 2; // skip TP-Data-Coding-Scheme

        // next one is date, it's length is 7 octets according to 3GPP TS 23.040
        // create time string
        if (info) {
            int i = 0;
            // year
            info->date[i++] = pdu[index + 1];
            info->date[i++] = pdu[index];
            index += 2;
            info->date[i++] = '/';
            // month
            info->date[i++] = pdu[index + 1];
            info->date[i++] = pdu[index];
            index += 2;
            info->date[i++] = '/';
            // Day
            info->date[i++] = pdu[index + 1];
            info->date[i++] = pdu[index];
            index += 2;
            info->date[i++] = ',';
            // Hour
            info->date[i++] = pdu[index + 1];
            info->date[i++] = pdu[index];
            index += 2;
            info->date[i++] = ':';
            // Minute
            info->date[i++] = pdu[index + 1];
            info->date[i++] = pdu[index];
            index += 2;
            info->date[i++] = ':';
            // Second
            info->date[i++] = pdu[index + 1];
            info->date[i++] = pdu[index];
            index += 2;
            // timezone related to GMT. pdu[index+1] most significant bit indicates the sign related to gmt
            tmp = hex_str_to_int(pdu + index + 1, 1);
            if (tmp & 0x08) {
                info->date[i++] = '-';
            } else {
                info->date[i++] = '+';
            }

            // pdu[index+1 & 0x07 is the most significant bits of the timezone
            // pdu [index] is the least significant bits
            info->date[i++] = '0' + (tmp & 0x07);
            info->date[i++] = pdu[index];
            info->date[i] = '\0';
            index += 2;
        } else {
            index += 14;
        }

        int udl =  hex_str_to_int(pdu + index, 2);
        index += 2;

        int paddingBits = 0;
        int partnro = 1;
        if (userDataHeader) {
            // we need to read User Defined Header to know what part number this message is.
            index += read_udh_from_pdu(pdu + index, info, partnro, paddingBits);
        }

        if (part_number) {
            *part_number = partnro;
        }

        if (msg) {
            // we are reading the message
            err = read_pdu_payload(pdu + index, udl, dataScheme, msg, paddingBits);
        } else {
            if (dataScheme == 0x00) {
                // when listing messages we need to calculated length. Other way would be unpacking the whole message.
                err = strlen(pdu + index) >> 1;
                err *= 8;
                err /= 7;
            } else if (dataScheme == 0x04) {
                err = strlen(pdu + index) >> 1;
            } else {
                return NSAPI_ERROR_UNSUPPORTED;
            }
        }

        return err;
    } else {
        // message was not DELIVER so discard it
        return NSAPI_ERROR_UNSUPPORTED;
    }
}

// read params from User Defined Header
int AT_CellularSMS::read_udh_from_pdu(const char *pdu, sms_info_t *info, int &part_number, int &padding_bits)
{

    int index = 0;
    int udhLength = hex_str_to_int(pdu, 2);
    index += 2;

    // if there is padding bits then udhlen is octet bigger as we need to keep septet boundary
    padding_bits = ((udhLength + 1) * 8) % 7; // +1 is for udhLength itself

    if (padding_bits) {
        padding_bits = 7 - padding_bits;
    } else {
        padding_bits = 0;
    }

    int tmp = hex_str_to_int(pdu + index, 2);
    index += 4;

    if (tmp == 0) { // 8-bit reference number
        if (info) {
            info->msg_ref_number = (uint16_t)hex_str_to_int(pdu + index, 2);
        }
        index += 2;
    } else {                  // 16-bit reference number
        if (info) {
            info->msg_ref_number = (uint16_t)hex_str_to_int(pdu + index + 2, 2);
            tmp = hex_str_to_int(pdu + index, 2);
            info->msg_ref_number |= (tmp << 8);
        }
        index += 4;
    }

    if (info) {
        info->parts = hex_str_to_int(pdu + index, 2);
    }
    index += 2;

    part_number = hex_str_to_int(pdu + index, 2);
    index += 2;

    return (udhLength * 2 + 2); // udh in hex and udhl
}

nsapi_size_or_error_t AT_CellularSMS::read_pdu_payload(const char *pdu, int msg_len, int scheme, char *msg, int padding_bits)
{
    if (scheme == 0x00) {
        // 7 bit gsm encoding, must do the conversions from hex to 7-bit encoding and to ascii
        return unpack_7_bit_gsm_to_str(pdu, strlen(pdu) / 2, msg, padding_bits, msg_len);
    } else if (scheme == 0x04) {
        // 8bit scheme so just convert hexstring to charstring
        return hex_str_to_char_str(pdu, strlen(pdu), msg);
    } else {
        tr_error("Received unsupported data coding scheme: 0x%02x", scheme);
        return NSAPI_ERROR_UNSUPPORTED;
    }
}

void AT_CellularSMS::free_linked_list()
{
    sms_info_t *info = _sms_info;
    sms_info_t *old;
    while (info) {
        old = info;
        info = info->next_info;
        delete old;
    }
    _sms_info = NULL;
}

void AT_CellularSMS::add_info(sms_info_t *info, int index, int part_number)
{
    // check for same message reference id. If found, update it and delete the given info.
    // if NOT found then add to the end of the list.

    if (!_sms_info) {
        info->msg_index[part_number - 1] = index; // part numbering starts from 1 so -1 to put to right index
        _sms_info = info;
        return;
    }
    sms_info_t *current = _sms_info;
    sms_info_t *prev = NULL;
    bool found_msg = false;
    while (current) {
        prev = current;
        // sms messages can have same reference number so additional checks are needed.
        // TODO: should we include phone number also?
        if (current->msg_ref_number == info->msg_ref_number && current->parts > current->parts_added &&
                info->parts > info->parts_added) {
            // multipart sms, update msg size and index
            current->msg_size += info->msg_size;
            current->msg_index[part_number - 1] = index; // part numbering starts from 1 so -1 to put to right index
            current->parts_added++;
            // update oldest part as date
            if (compare_time_strings(info->date, current->date) == -1) {
                strcpy(current->date, info->date);
            }
            found_msg = true;
            break;
        }
        current = current->next_info;
    }

    if (found_msg) {
        // info was added to existing item in linked list, must be deleted
        delete info;
    } else {
        // message not found, add to linked list
        info->msg_index[part_number - 1] = index;
        prev->next_info = info;
    }
}

// reads all the messages to the linked list AT_CellularSMS::_sms_info
nsapi_error_t AT_CellularSMS::list_messages()
{
    // TODO: NOTE:  If the selected <mem1> can contain different types of SMs (e.g. SMS-DELIVERs, SMS-SUBMITs, SMS-STATUS-REPORTs and SMS-COMMANDs),
    // the response may be a mix of the responses of different SM types. TE application can recognize the response format by examining the third response parameter.
    // for now we assume that only SMS-DELIVER messages are read.
    if (_mode == CellularSMSMmodePDU) {
        _at.cmd_start_stop("+CMGL", "=4");
    } else {
        _at.cmd_start_stop("+CMGL", "=\"ALL\"");
    }

    sms_info_t *info = NULL;
    // init for 1 so that in text mode we will add to the correct place without any additional logic in addInfo() in text mode
    int part_number = 1;
    int index = 0;
    int length = 0;
    char *pdu = NULL;

    _at.resp_start("+CMGL:");
    while (_at.info_resp()) {
        info = new sms_info_t();
        if (_mode == CellularSMSMmodePDU) {
            //+CMGL: <index>,<stat>,[<alpha>],<length><CR><LF><pdu>[<CR><LF>
            // +CMGL:<index>,<stat>,[<alpha>],<length><CR><LF><pdu>
            //[...]]
            index = _at.read_int();
            _at.skip_param(2); // <stat>,[<alpha>]
            length = _at.read_int();
            length = length * 2 + 20; // *2 as it's hex encoded and +20 as service center number is not included in size given by CMGL
            pdu = new char[length];
            memset(pdu, 0, length);
            _at.read_string(pdu, length, true);
            if (_at.get_last_error() == NSAPI_ERROR_OK) {
                info->msg_size = get_data_from_pdu(pdu, info, &part_number);
            }
        } else {
            // +CMGL: <index>,<stat>,<oa/da>,[<alpha>],[<scts>][,<tooa/toda>,<length>]<CR><LF><data>[<CR><LF>
            // +CMGL: <index>,<stat>,<da/oa>,[<alpha>],[<scts>][,<tooa/toda>,<length>]<CR><LF><data>[...]]
            index = _at.read_int();
            (void)_at.consume_to_stop_tag(); // consume until <CR><LF>
            (void)_at.consume_to_stop_tag(); // consume until <CR><LF>
        }

        if (index >= 0) {
            add_info(info, index, part_number);
        } else {
            delete info;
            info = NULL;
        }
        delete [] pdu;
        pdu = NULL;
    }


    _at.resp_stop();

    return _at.get_last_error();
}

AT_CellularSMS::sms_info_t *AT_CellularSMS::get_oldest_sms_index()
{
    /*
     * Different scenarios when finding the oldest concatenated sms
     *
     * 1. Find first parts first and it was received first
     * 2. Find first parts first and it was NOT received first -> older timestamp might exist in some other part
     * 3. Find other than first part first and it was received first
     * 4. Find other than first part first and it was NOT received first -> older timestamp might exist in some other part
     *
     * So must take all message to a linked list and loop that for the oldest
     */

    // if text mode we need to read sms with +CMGR because time stamp is optional while looping with +CMGL
    sms_info_t *retVal = NULL;
    sms_info_t *current = _sms_info;
    nsapi_size_or_error_t err = 0;
    while (current) {
        if (_mode == CellularSMSMmodeText) {
            ThisThread::sleep_for(_sim_wait_time);
            err = read_sms_from_index(current->msg_index[0], NULL, 0, NULL, current->date);
            if (err != 0) {
                return NULL;
            }
        }

        if (retVal == NULL) {
            retVal = current;
        } else if (compare_time_strings(current->date, retVal->date) == -1) {
            // found older sms, update return value to oldest
            retVal = current;
        }
        current = current->next_info;
    }

    return retVal;
}

// if time_string_1 is greater (more fresh date) then return 1, same 0, smaller -1. Error -2
int AT_CellularSMS::compare_time_strings(const char *time_string_1, const char *time_string_2)
{
    time_t t1;
    time_t t2;

    bool success = create_time(time_string_1, &t1) && create_time(time_string_2, &t2);
    int retVal = -2;

    if (success) {
        time_t diff = t1 - t2;

        if (diff > 0) {
            retVal = 1;
        } else if (diff == 0) {
            retVal = 0;
        } else {
            retVal = -1;
        }
    }

    return retVal;
}

bool AT_CellularSMS::create_time(const char *time_string, time_t *time)
{
    const int kNumberOfElements = 8;
    tm time_struct = { 0 };
    int gmt = 0;
    char sign;
    bool retVal = false;

    if (sscanf(time_string, "%d/%d/%d,%d:%d:%d%c%d", &time_struct.tm_year, &time_struct.tm_mon, &time_struct.tm_mday,
               &time_struct.tm_hour, &time_struct.tm_min, &time_struct.tm_sec, &sign, &gmt) == kNumberOfElements) {
        *time = mktime(&time_struct);
        // add timezone as seconds. gmt is in quarter of hours.
        int x = (60 / 4) * 60 * gmt;
        if (sign == '+') {
            *time += x;
        } else {
            *time -= x;
        }
        retVal = true;
    }

    return retVal;
}

uint16_t AT_CellularSMS::pack_7_bit_gsm_and_hex(const char *str, uint16_t len, char *buf,
                                                int number_of_padding_bit)
{
    uint16_t strCnt = 0;
    uint16_t i = 0;
    uint8_t shift;
    char tmp;

    if (len == 0) {
        return 0;
    }
    // convert to 7bit gsm first
    char *gsm_str = new char[len];
    for (uint16_t y = 0; y < len; y++) {
        for (int x = 0; x < GSM_TO_ASCII_TABLE_SIZE; x++) {
            if (gsm_to_ascii[x] == static_cast<unsigned char>(str[y])) {
                gsm_str[y] = x;
            }
        }
    }

    // then packing and converting to hex
    if (number_of_padding_bit) {
        tmp = gsm_str[strCnt] << number_of_padding_bit;
        strCnt++;
        char_str_to_hex_str(&tmp, 1, &buf[i * 2]);
        i++;
    }

    while (strCnt < len) {
        if (number_of_padding_bit) {
            shift = (i + number_of_padding_bit - 2) % 7;
        } else {
            shift = i % 7;
        }

        if (strCnt + 1 == len) {
            tmp = (gsm_str[strCnt] >> shift);
        } else {
            tmp = (gsm_str[strCnt] >> shift) | (gsm_str[strCnt + 1] << (7 - shift));
        }

        char_str_to_hex_str(&tmp, 1, buf + (i * 2));

        if (shift == 6) {
            strCnt++;
        }
        strCnt++;
        i++;
    }

    delete [] gsm_str;

    return i;
}

uint16_t AT_CellularSMS::unpack_7_bit_gsm_to_str(const char *str, int len, char *buf, int padding_bits,
                                                 int msg_len)
{
    int strCount = 0;
    uint16_t decodedCount = 0;
    uint8_t shift;
    char tmp;
    char tmp1;

    if (padding_bits) {
        hex_to_char(str, tmp);
        buf[decodedCount] = gsm_to_ascii[(tmp >> padding_bits) & 0x7F];
        strCount++;
        decodedCount++;
    }

    while (strCount < len) {
        shift = (strCount - padding_bits) % 7;
        hex_to_char(str + strCount * 2, tmp);
        if (shift == 0) {
            buf[decodedCount] = gsm_to_ascii[tmp & 0x7F];
        } else if (shift == 6) {
            hex_to_char(str + (strCount - 1) * 2, tmp1);
            buf[decodedCount] = gsm_to_ascii[(((tmp1 >> 2)) | (tmp << 6)) & 0x7F];
            if (decodedCount + 1 < msg_len) {
                hex_to_char(str + strCount * 2, tmp);
                decodedCount++;
                buf[decodedCount] = gsm_to_ascii[(tmp >> 1) & 0x7F];
            }
        } else {
            hex_to_char(str + (strCount - 1) * 2, tmp1);
            buf[decodedCount] = gsm_to_ascii[(((tmp1 >> (8 - shift))) | ((tmp << shift))) & 0x7F];
        }

        strCount++;
        decodedCount++;
    }

    return decodedCount;
}

#endif //MBED_CONF_CELLULAR_USE_SMS
