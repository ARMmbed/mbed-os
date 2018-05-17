/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctype.h>
#include <cstdio>
#include <string.h>
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "greentea-client/greentea_serial.h"
#include "greentea-client/greentea_metrics.h"


/**
 *   Generic test suite transport protocol keys
 */
const char* GREENTEA_TEST_ENV_END = "end";
const char* GREENTEA_TEST_ENV_EXIT = "__exit";
const char* GREENTEA_TEST_ENV_SYNC = "__sync";
const char* GREENTEA_TEST_ENV_TIMEOUT = "__timeout";
const char* GREENTEA_TEST_ENV_HOST_TEST_NAME = "__host_test_name";
const char* GREENTEA_TEST_ENV_HOST_TEST_VERSION = "__version";

/**
 *   Test suite success code strings
 */
const char* GREENTEA_TEST_ENV_SUCCESS = "success";
const char* GREENTEA_TEST_ENV_FAILURE = "failure";

/**
 *   Test case transport protocol start/finish keys
 */
const char* GREENTEA_TEST_ENV_TESTCASE_NAME = "__testcase_name";
const char* GREENTEA_TEST_ENV_TESTCASE_COUNT = "__testcase_count";
const char* GREENTEA_TEST_ENV_TESTCASE_START = "__testcase_start";
const char* GREENTEA_TEST_ENV_TESTCASE_FINISH = "__testcase_finish";
const char* GREENTEA_TEST_ENV_TESTCASE_SUMMARY = "__testcase_summary";
// Code Coverage (LCOV)  transport protocol keys
const char* GREENTEA_TEST_ENV_LCOV_START = "__coverage_start";

/**
 *   Auxilary functions
 */
static void greentea_notify_timeout(const int);
static void greentea_notify_hosttest(const char *);
static void greentea_notify_completion(const int);
static void greentea_notify_version();
static void greentea_write_string(const char *str);

/** \brief Handle the handshake with the host
 *  \details This is contains the shared handhshake functionality that is used between
 *           GREENTEA_SETUP and GREENTEA_SETUP_UUID.
 *           This function is blocking.
 */
void _GREENTEA_SETUP_COMMON(const int timeout, const char *host_test_name, char *buffer, size_t size) {
    greentea_metrics_setup();
    // Key-value protocol handshake function. Waits for {{__sync;...}} message
    // Sync preamble: "{{__sync;0dad4a9d-59a3-4aec-810d-d5fb09d852c1}}"
    // Example value of sync_uuid == "0dad4a9d-59a3-4aec-810d-d5fb09d852c1"

    char _key[8] = {0};

    while (1) {
        greentea_parse_kv(_key, buffer, sizeof(_key), size);
        greentea_write_string("mbedmbedmbedmbedmbedmbedmbedmbed\r\n");
        if (strcmp(_key, GREENTEA_TEST_ENV_SYNC) == 0) {
            // Found correct __sync message
            greentea_send_kv(_key, buffer);
            break;
        }
    }

    greentea_notify_version();
    greentea_notify_timeout(timeout);
    greentea_notify_hosttest(host_test_name);
}

/** \brief Handshake with host and send setup data (timeout and host test name)
 *  \details This function will send preamble to master.
 *           After host test name is received master will invoke host test script
 *           and add host test's callback handlers to main event loop
 *           This function is blocking.
 */
extern "C" void GREENTEA_SETUP(const int timeout, const char *host_test_name) {
    char _value[GREENTEA_UUID_LENGTH] = {0};
    _GREENTEA_SETUP_COMMON(timeout, host_test_name, _value, GREENTEA_UUID_LENGTH);
}

/** \brief Handshake with host and send setup data (timeout and host test name). Allows you to preserve sync UUID.
 *  \details This function will send preamble to master.
 *           After host test name is received master will invoke host test script
 *           and add host test's callback handlers to main event loop
 *           This function is blocking.
 *           This function differs from GREENTEA_SETUP because it allows you to
 *           preserve the UUID sent during the sync process.
 */
void GREENTEA_SETUP_UUID(const int timeout, const char *host_test_name, char *buffer, size_t size) {
    _GREENTEA_SETUP_COMMON(timeout, host_test_name, buffer, size);
}

/** \brief Notify host (__exit message) side that test suite execution was complete
 *  \result Test suite result
 *  \details If __exit is not received by host side we will assume TIMEOUT
 */
void GREENTEA_TESTSUITE_RESULT(const int result) {
    greentea_notify_completion(result);
}

/**
 *  Test Case support
 */

/** \brief Notify host side that test case started
 *  \details test_case_name Test case name
 */
void GREENTEA_TESTCASE_START(const char *test_case_name) {
    greentea_send_kv(GREENTEA_TEST_ENV_TESTCASE_START, test_case_name);
}

/** \brief Notify host side that test case finished
 *  \details test_case_name Test case name
 *  \details result Test case result (0 -OK, non zero...)
 */
void GREENTEA_TESTCASE_FINISH(const char *test_case_name, const size_t passes, const size_t failed) {
    greentea_send_kv(GREENTEA_TEST_ENV_TESTCASE_FINISH, test_case_name, passes, failed);
}

/**
 *****************************************************************************
 *  Auxilary functions and key-value protocol support
 *****************************************************************************
 */


/**
 *****************************************************************************
 *  LCOV support
 *****************************************************************************
 */
#ifdef MBED_CFG_DEBUG_OPTIONS_COVERAGE
extern "C" void __gcov_flush(void);
extern bool coverage_report;

/**
 * \brief Send code coverage (gcov/LCOV) notification to master
 *
 *        Generates preamble of message sent to notify host about code coverage data dump.
 *
 *        This function is used by mbedOS software
 *        (see: mbed-drivers/source/retarget.cpp file) to generate code coverage
 *        messages to host. When code coverage feature is turned on slave will
 *        print-out code coverage data in form of key-value protocol.
 *        Message with code coverage data will contain message name, path to code
 *        coverage output file host will touch and fill with code coverage binary
 *        payload. Coverage payload is encoded as stream of ASCII coded bytes ("%02X").
 *
 * \param path to file with code coverage payload (set by gcov instrumentation)
 *
 */
void greentea_notify_coverage_start(const char *path) {
    printf("{{%s;%s;", GREENTEA_TEST_ENV_LCOV_START, path);
}

/**
 *  \brief Sufix for code coverage message to master (closing statement)
 *
 *         This function is used by mbedOS software
 *         (see: mbed-drivers/source/retarget.cpp file) to generate code coverage
 *         messages to host. When code coverage feature is turned on slave will
 *         print-out code coverage data in form of key-value protocol.
 *         Message with code coverage data will contain message name, path to code
 *         coverage output file host will touch and fill with code coverage binary
 *         payload. Coverage payload is encoded as stream of ASCII coded bytes ("%02X").
 *
 *         Companion function greentea_notify_coverage_start() defines code coverage message structure
 *
 */
void greentea_notify_coverage_end() {
    printf("}}" NL);
}

#endif

/**
 *****************************************************************************
 *  Key-value protocol support
 *****************************************************************************
 */

/**
 * \brief Write the preamble characters to the serial port
 *
 *        This function writes the preamble "{{" which is required
 *        for key-value comunication between the target and the host.
 *        This uses a Rawserial object, greentea_serial, which provides
 *        a direct interface to the USBTX and USBRX serial pins and allows
 *        the direct writing of characters using the putc() method.
 *        This suite of functions are provided to allow for serial communication
 *        to the host from within a thread/ISR.
 *
 */
inline void greentea_write_preamble()
{
    greentea_serial->putc('{');
    greentea_serial->putc('{');
}

/**
 * \brief Write the postamble characters to the serial port
 *
 *        This function writes the postamble "{{\n" which is required
 *        for key-value comunication between the target and the host.
 *        This uses a Rawserial object, greentea_serial, which provides
 *        a direct interface to the USBTX and USBRX serial pins and allows
 *        the direct writing of characters using the putc() method.
 *        This suite of functions are provided to allow for serial communication
 *        to the host from within a thread/ISR.
 *
 */
inline void greentea_write_postamble()
{
    greentea_serial->putc('}');
    greentea_serial->putc('}');
    greentea_serial->putc('\r');
    greentea_serial->putc('\n');
}

/**
 * \brief Write a string to the serial port
 *
 *        This function writes a '\0' terminated string from the target
 *        to the host. It writes directly to the serial port using the
 *        greentea_serial, Rawserial object.
 *
 * \param str - string value
 *
 */
inline void greentea_write_string(const char *str)
{
    while (*str != '\0') {
        greentea_serial->putc(*str);
        str ++;
    }
}


/**
 * \brief Write an int to the serial port
 *
 *        This function writes an integer value from the target
 *        to the host. The integer value is converted to a string and
 *        and then written character by character directly to the serial
 *        port using the greentea_serial, Rawserial object.
 *        sprintf() is used to convert the int to a string. Sprintf if
 *        inherently thread safe so can be used.
 *
 * \param val - integer value
 *
 */
#define MAX_INT_STRING_LEN 15
inline void greentea_write_int(const int val)
{
    char intval[MAX_INT_STRING_LEN];
    unsigned int i = 0;
    sprintf(intval, "%d", val);
    while (intval[i] != '\0') {
        greentea_serial->putc(intval[i]);
        i++;
    }
}

/**
 * \brief Encapsulate and send key-value message from DUT to host
 *
 *        This function uses underlying functions to write directly
 *        to the serial port, (USBTX). This allows KVs to be used
 *        from within interrupt context.
 *
 * \param key Message key (message/event name)
 * \param value Message payload, string value
 *
 */
extern "C" void greentea_send_kv(const char *key, const char *val) {
    if (key && val) {
        greentea_write_preamble();
        greentea_write_string(key);
        greentea_serial->putc(';');
        greentea_write_string(val);
        greentea_write_postamble();
    }
}

/**
 * \brief Encapsulate and send key-value message from DUT to host
 *
 *        This function uses underlying functions to write directly
 *        to the serial port, (USBTX). This allows KVs to be used
 *        from within interrupt context.
 *        Last value is an integer to avoid integer to string conversion
 *        made by the user.
 *
 * \param key Message key (message/event name)
 * \param value Message payload, integer value
 *
 */
void greentea_send_kv(const char *key, const int val) {
    if (key) {
        greentea_write_preamble();
        greentea_write_string(key);
        greentea_serial->putc(';');
        greentea_write_int(val);
        greentea_write_postamble();
    }
}

/**
 * \brief Encapsulate and send key-value-value message from DUT to host
 *
 *        This function uses underlying functions to write directly
 *        to the serial port, (USBTX). This allows KVs to be used
 *        from within interrupt context.
 *        Last value is an integer to avoid integer to string conversion
 *        made by the user.
 *
 * \param key Message key (message/event name)
 * \param value Message payload, string value
 * \param result Send additional integer formatted data
 *
 */
void greentea_send_kv(const char *key, const char *val, const int result) {
    if (key) {
        greentea_write_preamble();
        greentea_write_string(key);
        greentea_serial->putc(';');
        greentea_write_string(val);
        greentea_serial->putc(';');
        greentea_write_int(result);
        greentea_write_postamble();

    }
}

/**
 * \brief Encapsulate and send key-value-value-value message from DUT to host
 *
 *        This function uses underlying functions to write directly
 *        to the serial port, (USBTX). This allows KVs to be used
 *        from within interrupt context.
 *        Last 2 values are integers to avoid integer to string conversion
 *        made by the user.
 *
 *        Names of the parameters: this function is used to send test case
 *        name with number of passes and failures to host. But it can be used
 *        to send any key-value-value-value (string-string-integer-integer)
 *        set to host.
 *
 * \param key Message key (message/event name)
 * \param value Message payload, string value
 * \param passes Send additional integer formatted data
 * \param failures Send additional integer formatted data
 *
 */
void greentea_send_kv(const char *key, const char *val, const int passes, const int failures) {
    if (key) {
        greentea_write_preamble();
        greentea_write_string(key);
        greentea_serial->putc(';');
        greentea_write_string(val);
        greentea_serial->putc(';');
        greentea_write_int(passes);
        greentea_serial->putc(';');
        greentea_write_int(failures);
        greentea_write_postamble();
    }
}

/**
 * \brief Encapsulate and send key-value-value message from DUT to host
 *
 *        This function uses underlying functions to write directly
 *        to the serial port, (USBTX). This allows key-value-value to be used
 *        from within interrupt context.
 *        Both values are integers to avoid integer to string conversion
 *        made by the user.
 *
 *        Names of the parameters: this function is used to send number
 *        of passes and failures to host. But it can be used to send any
 *        key-value-value (string-integer-integer) message to host.
 *
 * \param key Message key (message/event name)
 * \param value Message payload, integer value
 * \param passes Send additional integer formatted data
 * \param failures Send additional integer formatted data
 *
 */
void greentea_send_kv(const char *key, const int passes, const int failures) {
    if (key) {
        greentea_write_preamble();
        greentea_write_string(key);
        greentea_serial->putc(';');
        greentea_write_int(passes);
        greentea_serial->putc(';');
        greentea_write_int(failures);
        greentea_write_postamble();
    }
}

/**
 * \brief Send message with timeout to master in seconds
 *
 *        GREENTEA_TEST_ENV_TIMEOUT message is part of preamble
 *        sent from DUT to host during synchronisation (beginning of test
 *        suite execution).
 *
 *        Notification about total test suite timeout. Timeout is measured
 *        from the moment of GREENTEA_TEST_ENV_TIMEOUT reception by host.
 *        If timeout is reached host (and host test) will be stopped and
 *        control will return to Greentea.
 *
 * \param timeout Test suite timeout in seconds
 *
 */
static void greentea_notify_timeout(const int timeout) {
    greentea_send_kv(GREENTEA_TEST_ENV_TIMEOUT, timeout);
}

/**
 * \brief Send host test name to master
 *
 *        GREENTEA_TEST_ENV_HOST_TEST_NAME message is part of preamble
 *        sent from DUT to host during synchronisation (beginning of test
 *        suite execution).
 *
 *        Host test Python script implements host side callbacks
 *        for key-value events sent from DUT to host. Host test's
 *        callbacks are registered after GREENTEA_TEST_ENV_HOST_TEST_NAME
 *        message reaches host.
 *
 * \param host_test_name Host test name, host test will be loaded by mbedhtrun
 */
static void greentea_notify_hosttest(const char *host_test_name) {
    greentea_send_kv(GREENTEA_TEST_ENV_HOST_TEST_NAME, host_test_name);
}

/**
 * \brief Send to master information that test suite finished its execution
 *
 *        GREENTEA_TEST_ENV_END and GREENTEA_TEST_ENV_EXIT messages
 *        are sent just before test suite execution finishes (noting
 *        else to do). You can place it just before you return from your
 *        main() function.
 *
 *        Code coverage: If MEBD_CFG_DEBUG_OPTIONS_COVERAGE is set in the
 *        project via build configuration function will output series
 *        of code coverage messages GREENTEA_TEST_ENV_LCOV_START with code
 *        coverage binary data. This data is captured by Greentea and can
 *        be used to generate LCOV reports.
 *
 * \param result Test suite result from DUT (0 - FAIl, !0 - SUCCESS)
 *
 */
static void greentea_notify_completion(const int result) {
    const char *val = result ? GREENTEA_TEST_ENV_SUCCESS : GREENTEA_TEST_ENV_FAILURE;
#ifdef MBED_CFG_DEBUG_OPTIONS_COVERAGE
    coverage_report = true;
    __gcov_flush();
    coverage_report = false;
#endif
    greentea_metrics_report();
    greentea_send_kv(GREENTEA_TEST_ENV_END, val);
    greentea_send_kv(GREENTEA_TEST_ENV_EXIT, 0);
}

/**
 * \brief Send to master greentea-client version
 */
static void greentea_notify_version() {
    greentea_send_kv(GREENTEA_TEST_ENV_HOST_TEST_VERSION, MBED_GREENTEA_CLIENT_VERSION_STRING);
}

/**
 *****************************************************************************
 *  Parse engine for KV values which replaces scanf
 *****************************************************************************
 *
 *  Example usage:
 *
 *  char key[10];
 *  char value[48];
 *
 *  greentea_parse_kv(key, value, 10, 48);
 *  greentea_parse_kv(key, value, 10, 48);
 *
 */


static int gettok(char *, const int);
static int getNextToken(char *, const int);
static int HandleKV(char *,  char *,  const int,  const int);
static int isstring(int);

/**
 *  \brief Current token of key-value protocol's tokenizer
 */
static int CurTok = 0;

/**
 *  \enum Token enumeration for key-value protocol tokenizer
 *
 *        This enum is used by key-value protocol tokenizer
 *        to detect parts of protocol in stream.
 *
 *        tok_eof       ::= EOF (end of file)
 *        tok_open      ::= "{{"
 *        tok_close     ::= "}}"
 *        tok_semicolon ::= ";"
 *        tok_string    ::= [a-zA-Z0-9_-!@#$%^&*()]+    // See isstring() function
 *
 */
enum Token {
    tok_eof = -1,
    tok_open = -2,
    tok_close = -3,
    tok_semicolon = -4,
    tok_string = -5
};

/**
 * \brief Read character from stream of data
 *
 *        Closure for default "get character" function.
 *        This function is used to read characters from the stream
 *        (default is serial port RX). Key-value protocol tokenizer
 *        will build stream of tokes used by key-value protocol to
 *        detect valid messages.
 *
 *        If EOF is received parser finishes parsing and stops. In
 *        situation where we have serial port stream of data parsing
 *        goes forever.
 *
 * \return Next character from the stream or EOF if stream has ended.
 *
 */
extern "C" int greentea_getc() {
    return greentea_serial->getc();
}

/**
 * \brief parse input string for key-value pairs: {{key;value}}
 *        This function should replace scanf() used to
 *        check for incoming messages from master. All data
 *        parsed and rejected is discarded.
 *
 * \param out_key Ouput data with key
 * \param out_value Ouput data with value
 * \param out_key_size out_key total size
 * \param out_value_size out_value total data
 *
 * success != 0 when key-value pair was found
 * success == 0 when end of the stream was found
 *
 */
extern "C" int greentea_parse_kv(char *out_key,
                      char *out_value,
                      const int out_key_size,
                      const int out_value_size) {
    getNextToken(0, 0);
    while (1) {
        switch (CurTok) {
        case tok_eof:
            return 0;

        case tok_open:
            if (HandleKV(out_key, out_value, out_key_size, out_value_size)) {
                // We've found {{ KEY ; VALUE }} expression
                return 1;
            }
            break;

        default:
            // Load next token and pray...
            getNextToken(0, 0);
            break;
        }
    }
}

/**
 *  \brief Get next token from stream
 *
 *         Key-value TOKENIZER feature
 *
 *         This function is used by key-value parser determine
 *         if key-value message is embedded in stream data.
 *
 *  \param str Output parameters to store token string value
 *  \param str_size Size of 'str' parameter in bytes (characters)
 *
 */
static int getNextToken(char *str, const int str_size) {
    return CurTok = gettok(str, str_size);
}

/**
 *  \brief Check if character is punctuation character
 *
 *          Auxilary key-value TOKENIZER function
 *
 *          Defines if character is in subset of allowed punctuation
 *          characters which can be part of a key or value string.
 *          Not allowed characters are: ";{}"
 *
 *  \param c Input character to check
 *  \return Return 1 if character is allowed punctuation character, otherwise return false
 *
 */
static int ispunctuation(int c) {
    static const char punctuation[] = "_-!@#$%^&*()=+:<>,./?\\\"'";  // No ";{}"
    for (size_t i=0; i< sizeof(punctuation); ++i) {
        if (c == punctuation[i]) {
            return 1;
        }
    }
    return 0;
}

/**
 *  \brief Check if character is string token character
 *
 *          Auxilary key-value TOKENIZER function
 *
 *          Defines if character is in subset of allowed string
 *          token characters.
 *          String defines set of characters which can be a key or value string.
 *
 *          Allowed subset includes:
 *          - Alphanumerical characters
 *          - Digits
 *          - White spaces and
 *          - subset of punctuation characters.
 *
 *  \param c Input character to check
 *  \return Return 1 if character is allowed punctuation character, otherwise return false
 *
 */
static int isstring(int c) {
    return (isalpha(c) ||
            isdigit(c) ||
            isspace(c) ||
            ispunctuation(c));
}

/**
 *  \brief TOKENIZER of key-value protocol
 *
 *         Actual key-value TOKENIZER engine
 *
 *         TOKENIZER defines #Token enum to map recognized tokens to integer values.
 *
 *         <TOK_EOF>       ::= EOF (end of file)
 *         <TOK_OPEN>      ::= "{{"
 *         <TOK_CLOSE>     ::= "}}"
 *         <TOK_SEMICOLON> ::= ";"
 *         <TOK_STRING>    ::= [a-zA-Z0-9_-!@#$%^&*()]+    // See isstring() function *
 *
 *  \param out_str Output string with parsed token (string)
 *  \param str_size Size of str buffer we can use
 *
 *  \return Return #Token enum value used by parser to check for key-value occurrences
 *
 */
static int gettok(char *out_str, const int str_size) {
    static int LastChar = '!';
    static int str_idx = 0;

    // whitespace ::=
    while (isspace(LastChar)) {
        LastChar = greentea_getc();
    }

    // string ::= [a-zA-Z0-9_-!@#$%^&*()]+
    if (isstring(LastChar)) {
        str_idx = 0;
        if (out_str && str_idx < str_size - 1) {
            out_str[str_idx++] = LastChar;
        }

        while (isstring((LastChar = greentea_getc())))
            if (out_str && str_idx < str_size - 1) {
                out_str[str_idx++] = LastChar;
            }
        if (out_str && str_idx < str_size) {
            out_str[str_idx] = '\0';
        }

        return tok_string;
    }

    // semicolon ::= ';'
    if (LastChar == ';') {
        LastChar = greentea_getc();
        return tok_semicolon;
    }

    // open ::= '{{'
    if (LastChar == '{') {
        LastChar = greentea_getc();
        if (LastChar == '{') {
            LastChar = greentea_getc();
            return tok_open;
        }
    }

    // close ::= '}'
	if (LastChar == '}') {
		LastChar = greentea_getc();
		if (LastChar == '}') {
			LastChar = '!';
			return tok_close;
		}
	}

    if (LastChar == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = greentea_getc();
    return ThisChar;
}

/**
 *  \brief Key-value parser
 *
 *         Key-value message grammar
 *
 *         <MESSAGE>: <TOK_OPEN> <TOK_STRING> <TOK_SEMICOLON> <TOK_STRING> <TOK_CLOSE>
 *
 *         Examples:
 *         message:     "{{__timeout; 1000}}"
 *                      "{{__sync; 12345678-1234-5678-1234-567812345678}}"
 *
 *  \param out_key Output buffer to store key string value
 *  \param out_value Output buffer to store value string value
 *  \param out_key_size Buffer 'out_key' buffer size
 *  \param out_value_size Buffer 'out_value_size' buffer size
 *  \return Returns 1 if key-value message was parsed successfully in stream of tokens from tokenizer
 *
 */
static int HandleKV(char *out_key,
                    char *out_value,
                    const int out_key_size,
                    const int out_value_size) {
    // We already started with <open>
    if (getNextToken(out_key, out_key_size) == tok_string) {
        if (getNextToken(0, 0) == tok_semicolon) {
            if (getNextToken(out_value, out_value_size) == tok_string) {
                if (getNextToken(0, 0) == tok_close) {
                    // <open> <string> <semicolon> <string> <close>
                    // Found "{{KEY;VALUE}}" expression
                    return 1;
                }
            }
        }
    }
    getNextToken(0, 0);
    return 0;
}
