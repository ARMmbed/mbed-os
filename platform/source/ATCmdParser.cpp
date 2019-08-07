/* Copyright (c) 2017 ARM Limited
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
 *
 * @section DESCRIPTION
 *
 * Parser for the AT command syntax
 *
 */

#include "ATCmdParser.h"
#include "mbed_poll.h"
#include "mbed_debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef LF
#undef LF
#define LF  10
#else
#define LF  10
#endif

#ifdef CR
#undef CR
#define CR  13
#else
#define CR  13
#endif

namespace mbed {

// getc/putc handling with timeouts
int ATCmdParser::putc(char c)
{
    pollfh fhs;
    fhs.fh = _fh;
    fhs.events = POLLOUT;

    int count = poll(&fhs, 1, _timeout);
    if (count > 0 && (fhs.revents & POLLOUT)) {
        return _fh->write(&c, 1) == 1 ? 0 : -1;
    } else {
        return -1;
    }
}

int ATCmdParser::getc()
{
    pollfh fhs;
    fhs.fh = _fh;
    fhs.events = POLLIN;

    int count = poll(&fhs, 1, _timeout);
    if (count > 0 && (fhs.revents & POLLIN)) {
        unsigned char ch;
        return _fh->read(&ch, 1) == 1 ? ch : -1;
    } else {
        return -1;
    }
}

void ATCmdParser::flush()
{
    while (_fh->readable()) {
        unsigned char ch;
        _fh->read(&ch, 1);
    }
}


// read/write handling with timeouts
int ATCmdParser::write(const char *data, int size)
{
    int i = 0;
    for (; i < size; i++) {
        if (putc(data[i]) < 0) {
            return -1;
        }
    }
    return i;
}

int ATCmdParser::read(char *data, int size)
{
    int i = 0;
    for (; i < size; i++) {
        int c = getc();
        if (c < 0) {
            return -1;
        }
        data[i] = c;
    }
    return i;
}


// printf/scanf handling
int ATCmdParser::vprintf(const char *format, std::va_list args)
{

    if (vsprintf(_buffer, format, args) < 0) {
        return false;
    }

    int i = 0;
    for (; _buffer[i]; i++) {
        if (putc(_buffer[i]) < 0) {
            return -1;
        }
    }
    return i;
}

int ATCmdParser::vscanf(const char *format, std::va_list args)
{
    // Since format is const, we need to copy it into our buffer to
    // add the line's null terminator and clobber value-matches with asterisks.
    //
    // We just use the beginning of the buffer to avoid unnecessary allocations.
    int i = 0;
    int offset = 0;

    while (format[i]) {
        if (format[i] == '%' && format[i + 1] != '%' && format[i + 1] != '*') {
            _buffer[offset++] = '%';
            _buffer[offset++] = '*';
            i++;
        } else {
            _buffer[offset++] = format[i++];
        }
    }

    // Scanf has very poor support for catching errors
    // fortunately, we can abuse the %n specifier to determine
    // if the entire string was matched.
    _buffer[offset++] = '%';
    _buffer[offset++] = 'n';
    _buffer[offset++] = 0;

    // To workaround scanf's lack of error reporting, we actually
    // make two passes. One checks the validity with the modified
    // format string that only stores the matched characters (%n).
    // The other reads in the actual matched values.
    //
    // We keep trying the match until we succeed or some other error
    // derails us.
    int j = 0;

    while (true) {
        // Ran out of space
        if (j + 1 >= _buffer_size - offset) {
            return false;
        }
        // Receive next character
        int c = getc();
        if (c < 0) {
            return -1;
        }
        _buffer[offset + j++] = c;
        _buffer[offset + j] = 0;

        // Check for match
        int count = -1;
        sscanf(_buffer + offset, _buffer, &count);

        // We only succeed if all characters in the response are matched
        if (count == j) {
            // Store the found results
            vsscanf(_buffer + offset, format, args);
            return j;
        }
    }
}


// Command parsing with line handling
bool ATCmdParser::vsend(const char *command, std::va_list args)
{
    // Create and send command
    if (vsprintf(_buffer, command, args) < 0) {
        return false;
    }

    for (int i = 0; _buffer[i]; i++) {
        if (putc(_buffer[i]) < 0) {
            return false;
        }
    }

    // Finish with newline
    for (size_t i = 0; _output_delimiter[i]; i++) {
        if (putc(_output_delimiter[i]) < 0) {
            return false;
        }
    }

    debug_if(_dbg_on, "AT> %s\n", _buffer);
    return true;
}

bool ATCmdParser::vrecv(const char *response, std::va_list args)
{
restart:
    _aborted = false;
    // Iterate through each line in the expected response
    // response being NULL means we just want to check for OOBs
    while (!response || response[0]) {
        // Since response is const, we need to copy it into our buffer to
        // add the line's null terminator and clobber value-matches with asterisks.
        //
        // We just use the beginning of the buffer to avoid unnecessary allocations.
        int i = 0;
        int offset = 0;
        bool whole_line_wanted = false;

        while (response && response[i]) {
            if (response[i] == '%' && response[i + 1] != '%' && response[i + 1] != '*') {
                _buffer[offset++] = '%';
                _buffer[offset++] = '*';
                i++;
            } else {
                _buffer[offset++] = response[i++];
                // Find linebreaks, taking care not to be fooled if they're in a %[^\n] conversion specification
                if (response[i - 1] == '\n' && !(i >= 3 && response[i - 3] == '[' && response[i - 2] == '^')) {
                    whole_line_wanted = true;
                    break;
                }
            }
        }

        // Scanf has very poor support for catching errors
        // fortunately, we can abuse the %n specifier to determine
        // if the entire string was matched.
        _buffer[offset++] = '%';
        _buffer[offset++] = 'n';
        _buffer[offset++] = 0;

        debug_if(_dbg_on, "AT? %s\n", _buffer);
        // To workaround scanf's lack of error reporting, we actually
        // make two passes. One checks the validity with the modified
        // format string that only stores the matched characters (%n).
        // The other reads in the actual matched values.
        //
        // We keep trying the match until we succeed or some other error
        // derails us.
        int j = 0;

        while (true) {
            // If just peeking for OOBs, and at start of line, check
            // readability
            if (!response && j == 0 && !_fh->readable()) {
                return false;
            }
            // Receive next character
            int c = getc();
            if (c < 0) {
                debug_if(_dbg_on, "AT(Timeout)\n");
                return false;
            }
            // Simplify newlines (borrowed from retarget.cpp)
            if ((c == CR && _in_prev != LF) ||
                    (c == LF && _in_prev != CR)) {
                _in_prev = c;
                c = '\n';
            } else if ((c == CR && _in_prev == LF) ||
                       (c == LF && _in_prev == CR)) {
                _in_prev = c;
                // onto next character
                continue;
            } else {
                _in_prev = c;
            }
            _buffer[offset + j++] = c;
            _buffer[offset + j] = 0;

            // Check for oob data
            for (struct oob *oob = _oobs; oob; oob = oob->next) {
                if ((unsigned)j == oob->len && memcmp(
                            oob->prefix, _buffer + offset, oob->len) == 0) {
                    debug_if(_dbg_on, "AT! %s\n", oob->prefix);
                    _oob_cb_count++;
                    oob->cb();

                    if (_aborted) {
                        debug_if(_dbg_on, "AT(Aborted)\n");
                        return false;
                    }
                    // oob may have corrupted non-reentrant buffer,
                    // so we need to set it up again
                    goto restart;
                }
            }

            // Check for match
            int count = -1;
            if (whole_line_wanted && c != '\n') {
                // Don't attempt scanning until we get delimiter if they included it in format
                // This allows recv("Foo: %s\n") to work, and not match with just the first character of a string
                // (scanf does not itself match whitespace in its format string, so \n is not significant to it)
            } else if (response) {
                sscanf(_buffer + offset, _buffer, &count);
            }

            // We only succeed if all characters in the response are matched
            if (count == j) {
                debug_if(_dbg_on, "AT= %s\n", _buffer + offset);
                // Reuse the front end of the buffer
                memcpy(_buffer, response, i);
                _buffer[i] = 0;

                // Store the found results
                vsscanf(_buffer + offset, _buffer, args);

                // Jump to next line and continue parsing
                response += i;
                break;
            }

            // Clear the buffer when we hit a newline or ran out of space
            // running out of space usually means we ran into binary data
            if (c == '\n' || j + 1 >= _buffer_size - offset) {
                debug_if(_dbg_on, "AT< %s", _buffer + offset);
                j = 0;
            }
        }
    }

    return true;
}

// Mapping to vararg functions
int ATCmdParser::printf(const char *format, ...)
{
    std::va_list args;
    va_start(args, format);
    int res = vprintf(format, args);
    va_end(args);
    return res;
}

int ATCmdParser::scanf(const char *format, ...)
{
    std::va_list args;
    va_start(args, format);
    int res = vscanf(format, args);
    va_end(args);
    return res;
}

bool ATCmdParser::send(const char *command, ...)
{
    std::va_list args;
    va_start(args, command);
    bool res = vsend(command, args);
    va_end(args);
    return res;
}

bool ATCmdParser::recv(const char *response, ...)
{
    std::va_list args;
    va_start(args, response);
    bool res = vrecv(response, args);
    va_end(args);
    return res;
}

// oob registration
void ATCmdParser::oob(const char *prefix, Callback<void()> cb)
{
    struct oob *oob = new struct oob;
    oob->len = strlen(prefix);
    oob->prefix = prefix;
    oob->cb = cb;
    oob->next = _oobs;
    _oobs = oob;
}

void ATCmdParser::abort()
{
    _aborted = true;
}

bool ATCmdParser::process_oob()
{
    int pre_count = _oob_cb_count;
    recv(NULL);
    return _oob_cb_count != pre_count;
}

}
