/*
 * Copyright (c) , Arm Limited and affiliates.
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

#include <time.h>
#include "AT_CellularSMS.h"
#include "CellularLog.h"

using namespace mbed;

#define CTRL_Z  "\x1a"
#define ESC     "\x1b"

const int SMS_STATUS_SIZE = 12 + 1;
const int FIRST_OCTET_DELIVER_SUBMIT = 17;
const int TP_VALIDITY_PERIOD_24_HOURS = 167;
const int TP_PROTOCOL_IDENTIFIER = 0;
const int SMS_DATA_CODING_SCHEME = 0;

const uint16_t SMS_MAX_8BIT_CONCATENATED_SINGLE_SMS_SIZE = 134;
const uint16_t SMS_MAX_GSM7_CONCATENATED_SINGLE_SMS_SIZE = 153;


AT_CellularSMS::AT_CellularSMS(ATHandler &at, AT_CellularDevice &device) : _cb(0), _mode(CellularSMSMmodeText),
    _use_8bit_encoding(false), _sim_wait_time(0), _sms_message_ref_number(1), _sms_info(NULL), _at(at), _device(device)
{
}

AT_CellularSMS::~AT_CellularSMS()
{
}

void AT_CellularSMS::cmt_urc()
{
}

void AT_CellularSMS::cmti_urc()
{
}

nsapi_error_t AT_CellularSMS::set_cnmi()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularSMS::set_cmgf(int msg_format)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularSMS::set_csmp(int fo, int vp, int pid, int dcs)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularSMS::set_csdh(int show_header)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularSMS::initialize(CellularSMSMmode mode,
                                         CellularSMSEncoding encoding)
{
    return NSAPI_ERROR_OK;
}

void AT_CellularSMS::set_extra_sim_wait_time(int sim_wait_time)
{
}

char *AT_CellularSMS::create_pdu(const char *phone_number, const char *message, uint8_t message_length, uint8_t msg_parts,
                                 uint8_t msg_part_number, uint8_t &header_size)
{
    return NULL;
}

nsapi_size_or_error_t AT_CellularSMS::send_sms(const char *phone_number, const char *message, int msg_len)
{
    return NSAPI_ERROR_OK;
}

void AT_CellularSMS::set_sms_callback(Callback<void()> func)
{
}

nsapi_error_t AT_CellularSMS::set_cpms(const char *memr, const char *memw, const char *mems)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularSMS::set_csca(const char *sca, int type)
{
    return NSAPI_ERROR_OK;
}

nsapi_size_or_error_t AT_CellularSMS::set_cscs(const char *chr_set)
{
    return NSAPI_ERROR_OK;
}

//nsapi_error_t AT_CellularSMS::set_csms(int msg_service)
//{
//    return NSAPI_ERROR_OK;
//}

nsapi_error_t AT_CellularSMS::delete_sms(sms_info_t *sms)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularSMS::delete_all_messages()
{
    return NSAPI_ERROR_OK;
}

nsapi_size_or_error_t AT_CellularSMS::read_sms_from_index(int msg_index, char *buf, uint16_t len, char *phone_num, char *time_stamp)
{
    return NSAPI_ERROR_OK;
}

// read msg in PDU mode
nsapi_size_or_error_t AT_CellularSMS::read_sms(sms_info_t *sms, char *buf, char *phone_num, char *time_stamp)
{
    return NSAPI_ERROR_OK;
}

nsapi_size_or_error_t AT_CellularSMS::get_sms(char *buf, uint16_t len, char *phone_num, uint16_t phone_len,
                                              char *time_stamp, uint16_t time_len, int *buf_size)
{
    return NSAPI_ERROR_OK;
}

nsapi_size_or_error_t AT_CellularSMS::get_data_from_pdu(const char *pdu, sms_info_t *info, int *part_number, char *phone_number, char *msg)
{
    return NSAPI_ERROR_OK;
}

// read params from User DEfined Header
int AT_CellularSMS::read_udh_from_pdu(const char *pdu, sms_info_t *info, int &part_number, int &padding_bits)
{
    return 0;
}

nsapi_size_or_error_t AT_CellularSMS::read_pdu_payload(const char *pdu, int msg_len, int scheme, char *msg, int padding_bits)
{
    return NSAPI_ERROR_OK;
}

void AT_CellularSMS::free_linked_list()
{
}

void AT_CellularSMS::add_info(sms_info_t *info, int index, int part_number)
{
}

// reads all the messages to the linked list AT_CellularSMS::_sms_info
nsapi_error_t AT_CellularSMS::list_messages()
{
    return NSAPI_ERROR_OK;
}

AT_CellularSMS::sms_info_t *AT_CellularSMS::get_oldest_sms_index()
{
    return NULL;
}

// if time_string_1 is greater (more fresh date) then return 1, same 0, smaller -1. Error -2
int AT_CellularSMS::compare_time_strings(const char *time_string_1, const char *time_string_2)
{
    return 0;
}

bool AT_CellularSMS::create_time(const char *time_string, time_t *time)
{
    return 0;
}
