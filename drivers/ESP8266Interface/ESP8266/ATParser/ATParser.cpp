/* Copyright (c) 2015 ARM Limited
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

#include "ATParser.h"
#include "mbed_debug.h"


// getc/putc handling with timeouts
int ATParser::putc(char c)
{
    Timer timer;
    timer.start();

    while (true) {
        if (_serial->writeable()) {
            return _serial->putc(c);
        }
        if (timer.read_ms() > _timeout) {
            return -1;
        }
    }
}

int ATParser::getc()
{
    Timer timer;
    timer.start();

    while (true) {
        if (_serial->readable()) {
            return _serial->getc();
        }
        if (timer.read_ms() > _timeout) {
            return -1;
        }
    }
}

void ATParser::flush()
{
    while (_serial->readable()) {
        _serial->getc();
    }
}


// read/write handling with timeouts
int ATParser::write(const char *data, int size)
{
    int i = 0;
    for ( ; i < size; i++) {
        if (putc(data[i]) < 0) {
            return -1;
        }
    }
    return i;
}

int ATParser::read(char *data, int size)
{
    int i = 0;
    for ( ; i < size; i++) {
        int c = getc();
        if (c < 0) {
            return -1;
        }
        data[i] = c;
    }
    return i;
}


// printf/scanf handling
int ATParser::vprintf(const char *format, va_list args)
{
    if (vsprintf(_buffer, format, args) < 0) {
        return false;
    }
    int i = 0;
    for ( ; _buffer[i]; i++) {
        if (putc(_buffer[i]) < 0) {
            return -1;
        }
    }
    return i;
}

int ATParser::vscanf(const char *format, va_list args)
{
    // Since format is const, we need to copy it into our buffer to
    // add the line's null terminator and clobber value-matches with asterisks.
    //
    // We just use the beginning of the buffer to avoid unnecessary allocations.
    int i = 0;
    int offset = 0;

    while (format[i]) {
        if (format[i] == '%' && format[i+1] != '%' && format[i+1] != '*') {
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
        if (j+1 >= _buffer_size - offset) {
            return false;
        }
        // Recieve next character
        int c = getc();
        if (c < 0) {
            return -1;
        }
        _buffer[offset + j++] = c;
        _buffer[offset + j] = 0;

        // Check for match
        int count = -1;
        sscanf(_buffer+offset, _buffer, &count);

        // We only succeed if all characters in the response are matched
        if (count == j) {
            // Store the found results
            vsscanf(_buffer+offset, format, args);
            return j;
        }
    }
}


// Command parsing with line handling
bool ATParser::vsend(const char *command, va_list args)
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
    for (int i = 0; _delimiter[i]; i++) {
        if (putc(_delimiter[i]) < 0) {
            return false;
        }
    }

    debug_if(dbg_on, "AT> %s\r\n", _buffer);
    return true;
}

bool ATParser::vrecv(const char *response, va_list args)
{
    // Iterate through each line in the expected response
    while (response[0]) {
        // Since response is const, we need to copy it into our buffer to
        // add the line's null terminator and clobber value-matches with asterisks.
        //
        // We just use the beginning of the buffer to avoid unnecessary allocations.
        int i = 0;
        int offset = 0;

        while (response[i]) {
            if (memcmp(&response[i+1-_delim_size], _delimiter, _delim_size) == 0) {
                i++;
                break;
            } else if (response[i] == '%' && response[i+1] != '%' && response[i+1] != '*') {
                _buffer[offset++] = '%';
                _buffer[offset++] = '*';
                i++;
            } else {
                _buffer[offset++] = response[i++];
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
            // Recieve next character
            int c = getc();
            if (c < 0) {
                return false;
            }
            _buffer[offset + j++] = c;
            _buffer[offset + j] = 0;

            // Check for match
            int count = -1;
            sscanf(_buffer+offset, _buffer, &count);

            // We only succeed if all characters in the response are matched
            if (count == j) {
                debug_if(dbg_on, "AT= %s\r\n", _buffer+offset);
                // Reuse the front end of the buffer
                memcpy(_buffer, response, i);
                _buffer[i] = 0;

                // Store the found results
                vsscanf(_buffer+offset, _buffer, args);

                // Jump to next line and continue parsing
                response += i;
                break;
            }

            // Clear the buffer when we hit a newline or ran out of space
            // running out of space usually means we ran into binary data
            if (j+1 >= _buffer_size - offset ||
                strcmp(&_buffer[offset + j-_delim_size], _delimiter) == 0) {

                debug_if(dbg_on, "AT< %s", _buffer+offset);
                j = 0;
            }
        }
    }

    return true;
}


// Mapping to vararg functions
int ATParser::printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int res = vprintf(format, args);
    va_end(args);
    return res;
}

int ATParser::scanf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int res = vscanf(format, args);
    va_end(args);
    return res;
}

bool ATParser::send(const char *command, ...)
{
    va_list args;
    va_start(args, command);
    bool res = vsend(command, args);
    va_end(args);
    return res;
}

bool ATParser::recv(const char *response, ...)
{
    va_list args;
    va_start(args, response);
    bool res = vrecv(response, args);
    va_end(args);
    return res;
}
