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

#include "ATHandler.h"
#include "mbed_poll.h"
#include "FileHandle.h"
#include "Timer.h"
#include "mbed_wait_api.h"
#include "mbed_debug.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "rtos/Thread.h"
#endif
#include "Kernel.h"

using namespace mbed;
using namespace events;

//#define MBED_TRACE_MAX_LEVEL TRACE_LEVEL_DEBUG
#include "CellularLog.h"

#if MBED_CONF_MBED_TRACE_ENABLE
#define at_debug(format, ...) do { if (_debug_on) debug(format, ## __VA_ARGS__); } while (0)
#else
#define at_debug(...)
#endif

const char *mbed::OK = "OK\r\n";
const uint8_t OK_LENGTH = 4;
const char *mbed::CRLF = "\r\n";
const uint8_t CRLF_LENGTH = 2;
const char *CME_ERROR = "+CME ERROR:";
const uint8_t CME_ERROR_LENGTH = 11;
const char *CMS_ERROR = "+CMS ERROR:";
const uint8_t CMS_ERROR_LENGTH = 11;
const char *ERROR_ = "ERROR\r\n";
const uint8_t ERROR_LENGTH = 7;
const uint8_t MAX_RESP_LENGTH = CMS_ERROR_LENGTH;
const char DEFAULT_DELIMITER = ',';

static const uint8_t map_3gpp_errors[][2] =  {
    { 103, 3 },  { 106, 6 },  { 107, 7 },  { 108, 8 },  { 111, 11 }, { 112, 12 }, { 113, 13 }, { 114, 14 },
    { 115, 15 }, { 122, 22 }, { 125, 25 }, { 172, 95 }, { 173, 96 }, { 174, 97 }, { 175, 99 }, { 176, 111 },
    { 177, 8 },  { 126, 26 }, { 127, 27 }, { 128, 28 }, { 129, 29 }, { 130, 30 }, { 131, 31 }, { 132, 32 },
    { 133, 33 }, { 134, 34 }, { 140, 40 }, { 141, 41 }, { 142, 42 }, { 143, 43 }, { 144, 44 }, { 145, 45 },
    { 146, 46 }, { 178, 65 }, { 179, 66 }, { 180, 48 }, { 181, 83 }, { 171, 49 },
};

ATHandler::ATHandler(FileHandle *fh, EventQueue &queue, int timeout, const char *output_delimiter, uint16_t send_delay) :
    _nextATHandler(0),
    _fileHandle(fh),
    _queue(queue),
    _last_err(NSAPI_ERROR_OK),
    _last_3gpp_error(0),
    _oob_string_max_length(0),
    _oobs(NULL),
    _at_timeout(timeout),
    _previous_at_timeout(timeout),
    _at_send_delay(send_delay),
    _last_response_stop(0),
    _fh_sigio_set(false),
    _processing(false),
    _ref_count(1),
    _stop_tag(NULL),
    _delimiter(DEFAULT_DELIMITER),
    _prefix_matched(false),
    _urc_matched(false),
    _error_found(false),
    _max_resp_length(MAX_RESP_LENGTH),
    _debug_on(false),
    _cmd_start(false)
{
    //enable_debug(true);

    clear_error();

    if (output_delimiter) {
        _output_delimiter_length = strlen(output_delimiter);
        _output_delimiter = new char[_output_delimiter_length];
        for (unsigned i=0; i<_output_delimiter_length; i++) {
            _output_delimiter[i] = output_delimiter[i];
        }
    } else {
        _output_delimiter = NULL;
        _output_delimiter_length = 0;
    }

    reset_buffer();
    memset(_info_resp_prefix, 0, sizeof(_info_resp_prefix));

    _current_scope = NotSet;
    set_tag(&_resp_stop, OK);
    set_tag(&_info_stop, CRLF);
    set_tag(&_elem_stop, ")");

    _fileHandle->set_blocking(false);

    set_filehandle_sigio();
}

void ATHandler::enable_debug(bool enable)
{
    _debug_on = enable;
}

ATHandler::~ATHandler()
{
    while (_oobs) {
        struct oob_t *oob = _oobs;
        _oobs = oob->next;
        delete oob;
    }
    if (_output_delimiter) {
        delete [] _output_delimiter;
    }
}

void ATHandler::inc_ref_count()
{
    _ref_count++;
}

void ATHandler::dec_ref_count()
{
    _ref_count--;
}

int ATHandler::get_ref_count()
{
    return _ref_count;
}

FileHandle *ATHandler::get_file_handle()
{
    return _fileHandle;
}

void ATHandler::set_file_handle(FileHandle *fh)
{
    _fileHandle = fh;
}

void ATHandler::set_urc_handler(const char *prefix, mbed::Callback<void()> callback)
{
    struct oob_t *oob = new struct oob_t;
    oob->matching_to_received = true;
    size_t prefix_len = strlen(prefix);
    if (prefix_len > _oob_string_max_length) {
        _oob_string_max_length = prefix_len;
        if (_oob_string_max_length > _max_resp_length) {
            _max_resp_length = _oob_string_max_length;
        }
    }
    oob->prefix = prefix;
    oob->cb = callback;
    oob->next = _oobs;
    _oobs = oob;
}

void ATHandler::event()
{
    // _processing must be set before filehandle write/read to avoid repetitive sigio events
    if (!_processing) {
        _processing = true;
        (void) _queue.call(Callback<void(void)>(this, &ATHandler::process_oob));
    }
}

void ATHandler::lock()
{
#ifdef AT_HANDLER_MUTEX
    _fileHandleMutex.lock();
#endif
    _processing = true;
    clear_error();
}

void ATHandler::unlock()
{
    _processing = false;
#ifdef AT_HANDLER_MUTEX
    _fileHandleMutex.unlock();
#endif
    if (_fileHandle->readable() || (_recv_pos < _recv_len)) {
        (void) _queue.call(Callback<void(void)>(this, &ATHandler::process_oob));
    }
}

nsapi_error_t ATHandler::unlock_return_error()
{
    nsapi_error_t err = _last_err;
    unlock();
    return err;
}

void ATHandler::set_at_timeout(uint32_t timeout_milliseconds, bool default_timeout)
{
    if (default_timeout) {
        _previous_at_timeout = timeout_milliseconds;
        _at_timeout = timeout_milliseconds;
    } else if (timeout_milliseconds != _at_timeout) {
        _previous_at_timeout = _at_timeout;
        _at_timeout = timeout_milliseconds;
    }
}

void ATHandler::restore_at_timeout()
{
    if (_previous_at_timeout != _at_timeout) {
        _at_timeout =_previous_at_timeout;
    }
}

void ATHandler::process_oob()
{
    lock();
    tr_debug("process_oob %d", (_fileHandle->readable() || (_recv_pos < _recv_len)));
    if (_fileHandle->readable() || (_recv_pos < _recv_len)) {
        _current_scope = NotSet;
        Timer timer;
        timer.start();
        do {
            if (match_urc()) {
                if (_fileHandle->readable() || (_recv_pos < _recv_len)) {
                    continue;
                }
                break;
            }
            // If no match found, look for CRLF and consume everything up to CRLF
            if (mem_str(_recv_buff, _recv_len, CRLF, CRLF_LENGTH)) {
                consume_to_tag(CRLF, true);
            } else {
                if (_fileHandle->readable()) {
                    fill_buffer();
                } else {
#ifdef MBED_CONF_RTOS_PRESENT
                    rtos::Thread::yield();
#endif
                }
            }
        } while (timer.read_ms() < 20); // URC's are very short so 20ms should be enough
    }
    tr_debug("process_oob exit");

    flush(); // consume anything that could not be handled

    unlock();
}

void ATHandler::set_filehandle_sigio()
{
    if (_fh_sigio_set) {
        return;
    }
    _fileHandle->sigio(mbed::Callback<void()>(this, &ATHandler::event));
    _fh_sigio_set = true;
}

void ATHandler::reset_buffer()
{
    tr_debug("%s", __func__);
    _recv_pos = 0; _recv_len = 0;
}

void ATHandler::rewind_buffer()
{
    tr_debug("%s", __func__);
    if (_recv_pos > 0 && _recv_len >= _recv_pos) {
        _recv_len -= _recv_pos;
        // move what is not read to beginning of buffer
        memmove(_recv_buff, _recv_buff + _recv_pos, _recv_len);
        _recv_pos = 0;
    }
}

// we are always expecting to receive something so there is wait timeout
void ATHandler::fill_buffer()
{
    tr_debug("%s", __func__);
    // Reset buffer when full
    if (sizeof(_recv_buff) == _recv_len) {
        reset_buffer();
    }

    Timer timer;
    timer.start();
    do {
       ssize_t len = _fileHandle->read(_recv_buff + _recv_len, sizeof(_recv_buff) - _recv_len);
        if (len > 0) {
            _recv_len += len;
           at_debug("\n----------readable----------: %d\n", _recv_len);
           for (size_t i = _recv_pos; i < _recv_len; i++) {
               at_debug("%c", _recv_buff[i]);
           }
           at_debug("\n----------readable----------\n");
           return;
       } else if (len != -EAGAIN && len != 0) {
           tr_warn("%s error: %d while reading", __func__, len);
           break;
       }
#ifdef MBED_CONF_RTOS_PRESENT
        rtos::Thread::yield();
#endif
    } while ((uint32_t)timer.read_ms() < _at_timeout);

    set_error(NSAPI_ERROR_DEVICE_ERROR);
    tr_debug("AT TIMEOUT, scope: %d timeout: %lu", _current_scope, _at_timeout);
}

int ATHandler::get_char()
{
    if (_recv_pos == _recv_len) {
        tr_debug("%s", __func__);
        reset_buffer(); // try to read as much as possible
        fill_buffer();
        if (get_last_error()) {
            tr_debug("%s: -1", __func__);
            return -1; // timeout to read
        }
    }

    tr_debug("%s: %c", __func__, _recv_buff[_recv_pos]);

    return _recv_buff[_recv_pos++];
}

void ATHandler::skip_param(uint32_t count)
{
    tr_debug("%s", __func__);
    if (_last_err || !_stop_tag || _stop_tag->found) {
        return;
    }

    for (uint32_t i = 0; (i < count && !_stop_tag->found); i++) {
        size_t match_pos = 0;
        while (true) {
            int c = get_char();
            if (c == -1) {
                set_error(NSAPI_ERROR_DEVICE_ERROR);
                return;
            } else if (c == _delimiter) {
                break;
            } else if (_stop_tag->len && c == _stop_tag->tag[match_pos]) {
                match_pos++;
                if (match_pos == _stop_tag->len) {
                    _stop_tag->found = true;
                    break;
                }
            } else if (match_pos) {
                match_pos = 0;
            }
        }
    }
    return;
}

void ATHandler::skip_param(ssize_t len, uint32_t count)
{
    tr_debug("%s", __func__);
    if (_last_err || !_stop_tag || _stop_tag->found) {
        return;
    }

    for (uint32_t i = 0; i < count; i++) {
        ssize_t read_len = 0;
        while (read_len < len) {
            int c = get_char();
            if (c == -1) {
                set_error(NSAPI_ERROR_DEVICE_ERROR);
                return;
            }
            read_len++;
        }
    }
    return;
}

ssize_t ATHandler::read_bytes(uint8_t *buf, size_t len)
{
    tr_debug("%s", __func__);
    if (_last_err) {
        return -1;
    }

    size_t read_len = 0;
    for (; read_len < len; read_len++) {
        int c = get_char();
        if (c == -1) {
            set_error(NSAPI_ERROR_DEVICE_ERROR);
            return -1;
        }
        buf[read_len] = c;
    }
    return read_len;
}

ssize_t ATHandler::read_string(char *buf, size_t size, bool read_even_stop_tag)
{
    tr_debug("%s", __func__);
    at_debug("\n----------read_string buff:----------\n");
    for (size_t i = _recv_pos; i < _recv_len; i++) {
        at_debug("%c", _recv_buff[i]);
    }
    at_debug("\n----------buff----------\n");

    if (_last_err || !_stop_tag || (_stop_tag->found && read_even_stop_tag == false)) {
        return -1;
    }

    uint8_t *pbuf = (uint8_t*)buf;

    size_t len = 0;
    size_t match_pos = 0;

    consume_char('\"');

    for (; len < (size + match_pos); len++) {
        int c = get_char();
        if (c == -1) {
            pbuf[len] = '\0';
            set_error(NSAPI_ERROR_DEVICE_ERROR);
            return -1;
        } else if (c == _delimiter) {
            pbuf[len] = '\0';
            break;
        } else if (c == '\"') {
            match_pos = 0;
            len--;
            continue;
        } else if (_stop_tag->len && c == _stop_tag->tag[match_pos]) {
            match_pos++;
            if (match_pos == _stop_tag->len) {
                _stop_tag->found = true;
                // remove tag from string if it was matched
                len -= (_stop_tag->len - 1);
                pbuf[len] = '\0';
                break;
            }
        } else if (match_pos) {
            match_pos = 0;
        }

        pbuf[len] = c;
    }

    // Do we need _stop_found set after reading by size -> is _stop_tag_by_len needed or not?
    return len;
}

int32_t ATHandler::read_int()
{
    tr_debug("%s", __func__);

     if (_last_err || !_stop_tag || _stop_tag->found) {
         return -1;
     }

    char buff[BUFF_SIZE];
    char *first_no_digit;

    if (read_string(buff, (size_t)sizeof(buff)) == 0) {
        return -1;
    }

    return std::strtol(buff, &first_no_digit, 10);
}

void ATHandler::set_delimiter(char delimiter)
{
    _delimiter = delimiter;
}

void ATHandler::set_default_delimiter()
{
    _delimiter = DEFAULT_DELIMITER;
}

void ATHandler::set_tag(tag_t* tag_dst, const char *tag_seq)
{
    if (tag_seq) {
        size_t tag_len = strlen(tag_seq);
        set_string(tag_dst->tag, tag_seq, tag_len);
        tag_dst->len = tag_len;
        tag_dst->found = false;
    } else {
        _stop_tag = NULL;
    }
}

void ATHandler::set_stop_tag(const char *stop_tag_seq)
{
    if (_last_err || !_stop_tag) {
        return;
    }

    set_tag(_stop_tag, stop_tag_seq);
}

void ATHandler::set_scope(ScopeType scope_type)
{
    tr_debug("%s: %d", __func__, scope_type);
    if (_current_scope != scope_type) {
        _current_scope = scope_type;
        switch (_current_scope) {
        case RespType:
            _stop_tag = &_resp_stop;
            _stop_tag->found = false;
            break;
        case InfoType:
            _stop_tag = &_info_stop;
            _stop_tag->found = false;
            consume_char(' ');
            break;
        case ElemType:
            _stop_tag = &_elem_stop;
            _stop_tag->found = false;
            break;
        case NotSet:
            _stop_tag = NULL;
            return;
        default:
            break;
        }
    }
}

// should match from recv_pos?
bool ATHandler::match(const char* str, size_t size)
{
    tr_debug("%s: %s", __func__, str);
    rewind_buffer();

    if ((_recv_len - _recv_pos) < size) {
        return false;
    }

    if (str && memcmp(_recv_buff + _recv_pos, str, size) == 0) {
        // consume matching part
        _recv_pos += size;
        return true;
    }
    return false;
}

bool ATHandler::match_urc()
{
    tr_debug("%s", __func__);
    rewind_buffer();
    size_t prefix_len = 0;
    for (struct oob_t *oob = _oobs; oob; oob = oob->next) {
        prefix_len = strlen(oob->prefix);
        if (_recv_len >= prefix_len) {
            if (match(oob->prefix, prefix_len)) {
                tr_debug("URC! %s", oob->prefix);
                set_scope(InfoType);
                if (oob->cb) {
                    oob->cb();
                }
                information_response_stop();
                return true;
            }
        }
    }
    return false;
}

bool ATHandler::match_error()
{
    tr_debug("%s", __func__);

    if (match(CME_ERROR, CME_ERROR_LENGTH)) {
        at_error(true, DeviceErrorTypeErrorCME);
        return true;
    } else if (match(CMS_ERROR, CMS_ERROR_LENGTH)) {
        at_error(true, DeviceErrorTypeErrorCMS);
        return true;
    } else if (match(ERROR_, ERROR_LENGTH)) {
        at_error(false, DeviceErrorTypeNoError);
        return true;
    }

    return false;
}

void ATHandler::clear_error()
{
    _last_err = NSAPI_ERROR_OK;
    _last_at_err.errCode = 0;
    _last_at_err.errType = DeviceErrorTypeNoError;
    _last_3gpp_error = 0;
}

nsapi_error_t ATHandler::get_last_error() const
{
    return _last_err;
}

device_err_t ATHandler::get_last_device_error() const
{
    return _last_at_err;
}

void ATHandler::set_error(nsapi_error_t err)
{
    if (_last_err == NSAPI_ERROR_OK) {
        _last_err = err;
    }

    if (_last_err != err) {
        tr_warn("AT error code changed from %d to %d!", _last_err, err);
    }
}

int ATHandler::get_3gpp_error()
{
    return _last_3gpp_error;
}

void ATHandler::set_3gpp_error(int err, DeviceErrorType error_type)
{
    if (_last_3gpp_error) { // don't overwrite likely root cause error
        return;
    }

    if (error_type == DeviceErrorTypeErrorCMS && err < 128) {
        // CMS errors 0-127 maps straight to 3GPP errors
        _last_3gpp_error = err;
    } else {
        for (size_t i = 0; i<sizeof(map_3gpp_errors)/sizeof(map_3gpp_errors[0]); i++) {
            if (map_3gpp_errors[i][0] == err) {
                _last_3gpp_error = map_3gpp_errors[i][1];
                tr_debug("AT3GPP error code %d", get_3gpp_error());
                break;
            }
        }
    }
}

void ATHandler::at_error(bool error_code_expected, DeviceErrorType error_type)
{
    int32_t err = -1;

    if (error_code_expected && (error_type == DeviceErrorTypeErrorCMS || error_type == DeviceErrorTypeErrorCME)) {
        set_scope(InfoType);
        err = read_int();

        if (err != -1) {
            set_3gpp_error(err, error_type);
            _last_at_err.errCode = err;
            _last_at_err.errType = error_type;
            tr_debug("ATHandler ERROR: %d", err);
        } else {
            tr_debug("ATHandler ERROR reading failed");
        }
    }

    _last_err = NSAPI_ERROR_DEVICE_ERROR;
}

void ATHandler::resp(const char *prefix, bool check_urc)
{
    tr_debug("%s: %s", __func__, prefix);

    at_debug("\n----------resp buff:----------\n");
    for (size_t i = _recv_pos; i < _recv_len; i++) {
        at_debug("%c", _recv_buff[i]);
    }
    at_debug("\n----------buff----------\n");

    _prefix_matched = false;
    _urc_matched = false;
    _error_found = false;

    while (!get_last_error()) {

        match(CRLF, CRLF_LENGTH);

        if (match(OK, OK_LENGTH)) {
            set_scope(RespType);
            _stop_tag->found = true;
            return;
        }

        if (match_error()) {
            _error_found = true;
            return;
        }

        if (prefix && match(prefix, strlen(prefix))) {
            _prefix_matched = true;
            return;
        }

        if (check_urc && match_urc()) {
            _urc_matched = true;
        }

        // If no match found, look for CRLF and consume everything up to and including CRLF
        if (mem_str(_recv_buff, _recv_len, CRLF, CRLF_LENGTH)) {
            // If no prefix, return on CRLF - means data to read
            if (!prefix) {
                return;
            }
            consume_to_tag(CRLF, true);
        } else {
            // If no prefix, no CRLF and no more chance to match for OK, ERROR or URC(since max resp length is already in buffer)
            // return so data could be read
            if (!prefix && ((_recv_len-_recv_pos) >= _max_resp_length)) {
                return;
            }
            fill_buffer();
        }
    }

    return;
    // something went wrong so application need to recover and retry
}

void ATHandler::resp_start(const char *prefix, bool stop)
{
    tr_debug("%s: %s", __func__, prefix);

    if (_last_err) {
        return;
    }

    // Try get as much data as possible
    rewind_buffer();
    fill_buffer();

    if (prefix) {
        if ((strlen(prefix) < sizeof(_info_resp_prefix))) {
            strcpy(_info_resp_prefix, prefix);
        } else {
            MBED_ASSERT(0);
        }
    }

    set_scope(RespType);

    resp(prefix, true);

    if (!stop && prefix && _prefix_matched) {
        set_scope(InfoType);
    }
}

// check urc because of error as urc
bool ATHandler::info_resp()
{
    tr_debug("%s", __func__);
    if (_last_err || _resp_stop.found) {
        return false;
    }

    if (_prefix_matched) {
        _prefix_matched = false;
       return true;
    }

    // If coming here after another info response was started(looping), stop the previous one.
    // Trying to handle stopping in this level instead of doing it in upper level.
    if (get_scope() == InfoType) {
        information_response_stop();
    }

    resp(_info_resp_prefix, false);

    if (_prefix_matched) {
       set_scope(InfoType);
       _prefix_matched = false;
       return true;
    }

    // On mismatch go to response scope
    set_scope(RespType);
    return false;
}

bool ATHandler::info_elem(char start_tag)
{
    tr_debug("%s: %c", __func__, start_tag);
    if (_last_err) {
        return false;
    }

    // If coming here after another info response element was started(looping), stop the previous one.
    // Trying to handle stopping in this level instead of doing it in upper level.
    if (get_scope() == ElemType) {
        information_response_element_stop();
    }

    consume_char(_delimiter);

    if (consume_char(start_tag)) {
        _prefix_matched = true;
        set_scope(ElemType);
        return true;
    }

    // On mismatch go to information response scope
    set_scope(InfoType);
    return false;
}

bool ATHandler::consume_char(char ch)
{
    tr_debug("%s: %c", __func__, ch);
    int read_char = get_char();
    // If we read something else than ch, recover it
    if (read_char != ch && read_char != -1) {
        _recv_pos--;
        return false;
    }
    return true;
}

bool ATHandler::consume_to_tag(const char *tag, bool consume_tag)
{
    tr_debug("%s: %s", __func__, tag);
    size_t match_pos = 0;

    while (true) {
        int c = get_char();
        if (c == -1) {
            break;
        } else if (c == tag[match_pos]) {
            match_pos++;
            if (match_pos == strlen(tag)) {
                if (!consume_tag) {
                    _recv_pos -= strlen(tag);
                }
                return true;
            }
        } else if (match_pos) {
            match_pos = 0;
        }
    }
    tr_debug("consume_to_tag not found");
    return false;
}

bool ATHandler::consume_to_stop_tag()
{
    tr_debug("%s", __func__);

    if (!_stop_tag || (_stop_tag && _stop_tag->found) || _error_found) {
        return true;
    }

    if (consume_to_tag((const char*)_stop_tag->tag, true)) {
        return true;
    }

    tr_debug("consume_to_stop_tag not found");
    set_error(NSAPI_ERROR_DEVICE_ERROR);
    return false;
}

// consume by size needed?

void ATHandler::resp_stop()
{
    // Do not return on error so that we can consume whatever there is in the buffer

    tr_debug("%s", __func__);

    if (_current_scope == ElemType) {
        information_response_element_stop();
        set_scope(InfoType);
    }

    if (_current_scope == InfoType) {
        information_response_stop();
    }

    // Go for response stop_tag
    if (consume_to_stop_tag()) {
        set_scope(NotSet);
    }

    // Restore stop tag to OK
    set_tag(&_resp_stop, OK);
    // Reset info resp prefix
    memset(_info_resp_prefix, 0, sizeof(_info_resp_prefix));

    _last_response_stop = rtos::Kernel::get_ms_count();
}

void ATHandler::information_response_stop()
{
    tr_debug("%s", __func__);
    if (consume_to_stop_tag()) {
        set_scope(RespType);
    }
}

void ATHandler::information_response_element_stop()
{
    tr_debug("%s", __func__);
    if (consume_to_stop_tag()) {
        set_scope(InfoType);
    }
}

ATHandler::ScopeType ATHandler::get_scope()
{
    return _current_scope;
}

void ATHandler::set_string(char *dest, const char *src, size_t src_len)
{
    memcpy(dest, src, src_len);
    dest[src_len] = '\0';
}

const char* ATHandler::mem_str(const char* dest, size_t dest_len, const char* src, size_t src_len)
{
    if (dest_len > src_len) {
        for(size_t i = 0; i < dest_len-src_len+1; ++i) {
            if(memcmp(dest+i, src, src_len) == 0) {
                return dest+i;
            }
        }
    }
    return NULL;
}

void ATHandler::cmd_start(const char* cmd)
{

    if (_at_send_delay) {
        uint64_t current_time = rtos::Kernel::get_ms_count();
        uint64_t time_difference = current_time - _last_response_stop;

        if (time_difference < (uint64_t)_at_send_delay) {
            wait_ms((uint64_t)_at_send_delay - time_difference);
            tr_debug("AT wait %llu %llu", current_time, _last_response_stop);
        } 
    } 

    tr_debug("AT> %s", cmd);



    if (_last_err != NSAPI_ERROR_OK) {
        return;
    }

    (void)write(cmd, strlen(cmd));

    _cmd_start = true;
}

void ATHandler::write_int(int32_t param)
{
    tr_debug("write_int: %d", param);
    // do common checks before sending subparameter
    if (check_cmd_send() == false) {
        return;
    }

    // write the integer subparameter
    const int32_t str_len = 12;
    char number_string[str_len];
    int32_t result = sprintf(number_string, "%ld", param);
    if (result > 0 && result < str_len) {
        (void)write(number_string, strlen(number_string));
    }
}

void ATHandler::write_string(const char* param, bool useQuotations)
{
    tr_debug("write_string: %s, %d", param, useQuotations);
    // do common checks before sending subparameter
    if (check_cmd_send() == false) {
        return;
    }

    // we are writing string, surround it with quotes
    if (useQuotations && write("\"", 1) != 1) {
        return;
    }

    (void)write(param, strlen(param));

    if (useQuotations) {
        // we are writing string, surround it with quotes
        (void)write("\"", 1);
    }
}

void ATHandler::cmd_stop()
{
    if (_last_err != NSAPI_ERROR_OK) {
        return;
    }
     // Finish with CR
    (void)write(_output_delimiter, _output_delimiter_length);
}

size_t ATHandler::write_bytes(const uint8_t *data, size_t len)
{
    if (_last_err != NSAPI_ERROR_OK) {
        return 0;
    }

    ssize_t write_len = write(data, len);
    return write_len < 0 ? 0 : (size_t)write_len;
}

ssize_t ATHandler::write(const void *data, size_t len)
{
    pollfh fhs;
    fhs.fh = _fileHandle;
    fhs.events = POLLOUT;
    ssize_t write_len = -1;

    int count = poll(&fhs, 1, _at_timeout);
    if (count > 0 && (fhs.revents & POLLOUT)) {
        write_len = _fileHandle->write(data, len);
    }

    if (write_len < 0 || (size_t)write_len != len) {
        set_error(NSAPI_ERROR_DEVICE_ERROR);
    }

    return write_len;
}

// do common checks before sending subparameters
bool ATHandler::check_cmd_send()
{
    if (_last_err != NSAPI_ERROR_OK) {
        return false;
    }

    // Don't write delimiter if this is the first subparameter
    if (_cmd_start) {
        _cmd_start = false;
    } else {
        if (write(&_delimiter, 1) != 1) {
            // writing of delimiter failed, return. write() already have set the _last_err
            return false;
        }
    }

    return true;
}

void ATHandler::flush()
{
    while (_fileHandle->readable()) {
        reset_buffer();
        fill_buffer();
    }
    reset_buffer();
}
