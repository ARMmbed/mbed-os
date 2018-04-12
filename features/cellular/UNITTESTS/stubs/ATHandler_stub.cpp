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

#include <ctype.h>
#include "nsapi_types.h"
#include "ATHandler.h"
#include "EventQueue.h"
#include "ATHandler_stub.h"

using namespace mbed;
using namespace events;

#include "CellularLog.h"

const int DEFAULT_AT_TIMEOUT = 1000; // at default timeout in milliseconds

nsapi_error_t ATHandler_stub::nsapi_error_value = 0;
uint8_t ATHandler_stub::nsapi_error_ok_counter = 0;
int ATHandler_stub::int_value = -1;
ssize_t ATHandler_stub::ssize_value = 0;
char* ATHandler_stub::read_string_value = NULL;
size_t ATHandler_stub::size_value = 0;
size_t ATHandler_stub::return_given_size = false;
bool ATHandler_stub::bool_value = false;
uint8_t ATHandler_stub::uint8_value = 0;
FileHandle_stub *ATHandler_stub::fh_value = NULL;
device_err_t ATHandler_stub::device_err_value;
Callback<void()> ATHandler_stub::callback = NULL;
uint8_t ATHandler_stub::resp_info_true_counter = false;

ATHandler::ATHandler(FileHandle *fh, EventQueue &queue, int timeout, const char *output_delimiter) :
    _nextATHandler(0),
    _fileHandle(fh),
    _queue(queue)
{
}

void ATHandler::enable_debug(bool enable)
{
}

ATHandler::~ATHandler()
{
}

void ATHandler::inc_ref_count()
{
}

void ATHandler::dec_ref_count()
{
}

int ATHandler::get_ref_count()
{
    return ATHandler_stub::int_value;
}

FileHandle *ATHandler::get_file_handle()
{
    return ATHandler_stub::fh_value;
}

void ATHandler::set_file_handle(FileHandle *fh)
{
}

nsapi_error_t ATHandler::set_urc_handler(const char *urc, mbed::Callback<void()> cb)
{
    ATHandler_stub::callback = cb;
    return NSAPI_ERROR_OK;
}

nsapi_error_t ATHandler::get_last_error() const
{
    if (ATHandler_stub::nsapi_error_ok_counter) {
        ATHandler_stub::nsapi_error_ok_counter--;
        return NSAPI_ERROR_OK;
    }
    return ATHandler_stub::nsapi_error_value;
}

void ATHandler::lock()
{
}

void ATHandler::unlock()
{
}

nsapi_error_t ATHandler::unlock_return_error()
{
    return ATHandler_stub::nsapi_error_value;
}

void ATHandler::set_at_timeout(uint32_t timeout_milliseconds, bool default_timeout)
{
}

void ATHandler::restore_at_timeout()
{
}

void ATHandler::process_oob()
{
}

void ATHandler::clear_error()
{
}

void ATHandler::skip_param(uint32_t count) {

}

void ATHandler::skip_param(ssize_t len, uint32_t count)
{
}

ssize_t ATHandler::read_bytes(uint8_t *buf, size_t len)
{
    return ATHandler_stub::ssize_value;
}

ssize_t ATHandler::read_string(char *buf, size_t size, bool read_even_stop_tag)
{
    if (ATHandler_stub::read_string_value && ATHandler_stub::ssize_value >= 0) {
        memcpy(buf, ATHandler_stub::read_string_value, ATHandler_stub::ssize_value);
    }
    return ATHandler_stub::ssize_value;
}

int ATHandler::read_int()
{
    return ATHandler_stub::int_value;
}

void ATHandler::set_delimiter(char delimiter)
{
}

void ATHandler::set_default_delimiter()
{
}

void ATHandler::set_stop_tag(const char *stop_tag_seq)
{
}

int ATHandler::get_3gpp_error()
{
    return ATHandler_stub::int_value;
}

void ATHandler::resp_start(const char *prefix, bool stop)
{
}

bool ATHandler::info_resp()
{
    if (ATHandler_stub::resp_info_true_counter) {
        ATHandler_stub::resp_info_true_counter--;
        return true;
    }
    return ATHandler_stub::bool_value;
}

bool ATHandler::info_elem(char start_tag)
{
    return ATHandler_stub::bool_value;
}

bool ATHandler::consume_to_stop_tag()
{
    return ATHandler_stub::bool_value;
}

void ATHandler::resp_stop()
{
}

void ATHandler::cmd_start(const char* cmd)
{
}

void ATHandler::write_int(int param)
{
}

void ATHandler::write_string(const char* param, bool useQuotations)
{
}

size_t ATHandler::write_bytes(const uint8_t* param, size_t len)
{
    if (ATHandler_stub::return_given_size) {
        return len;
    }
    return ATHandler_stub::size_value;
}

void ATHandler::cmd_stop()
{
}

device_err_t ATHandler::get_last_device_error() const
{
    return ATHandler_stub::device_err_value;
}

void ATHandler::flush()
{

}
