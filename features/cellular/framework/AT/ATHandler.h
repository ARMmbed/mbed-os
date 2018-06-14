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

#ifndef AT_HANDLER_H_
#define AT_HANDLER_H_

#include "platform/mbed_retarget.h"

#include "EventQueue.h"
#include "PlatformMutex.h"
#include "nsapi_types.h"

#include "PlatformMutex.h"
#include "Callback.h"
#include "EventQueue.h"

namespace mbed {

class FileHandle;

/**
 * If application calls associated FileHandle only from single thread context
  * then locking between AT command and response is not needed. However,
  * note that many cellular functions are called indirectly, for example with the socket API.
  * If you are unsure, then AT_HANDLER_MUTEX must be defined.
  */
#define AT_HANDLER_MUTEX

extern const char *OK;
extern const char *CRLF;

#define BUFF_SIZE 16

/* AT Error types enumeration */
enum DeviceErrorType {
    DeviceErrorTypeNoError = 0,
    DeviceErrorTypeError,       // AT ERROR
    DeviceErrorTypeErrorCMS,    // AT ERROR CMS
    DeviceErrorTypeErrorCME     // AT ERROR CME
};

/* struct used when getting at response error. Defines error code and type */
struct device_err_t {
    DeviceErrorType errType;
    int errCode;
};

/** Class ATHandler
 *
 *  Class for sending AT commands and parsing AT responses.
 */
class ATHandler {

public:
    /** Constructor
     *
     *  @param fh               file handle used for reading AT responses and writing AT commands
     *  @param queue            Event queue used to transfer sigio events to this thread
     *  @param timeout          Timeout when reading for AT response
     *  @param output_delimiter delimiter used when parsing at responses, "\r" should be used as output_delimiter
     *  @param send_delay       the minimum delay in ms between the end of last response and the beginning of a new command
     */
    ATHandler(FileHandle *fh, events::EventQueue &queue, int timeout, const char *output_delimiter, uint16_t send_delay = 0);
    ~ATHandler();

    /** Return used file handle.
     *
     *  @return used file handle
     */
    FileHandle *get_file_handle();

    /** Locks the mutex for file handle if AT_HANDLER_MUTEX is defined.
     */
    void lock();

    /** Unlocks the mutex for file handle if AT_HANDLER_MUTEX is defined.
     */
    void unlock();

    /** Locks the mutex for file handle if AT_HANDLER_MUTEX is defined and returns the last error.
     *
     * @return last error that happened when parsing AT responses
     */
    nsapi_error_t unlock_return_error();

    /** Set the urc callback for urc. If urc is found when parsing AT responses, then call if called.
     *  If urc is already set then it's not set twice.
     *
     *  @param prefix   Register urc prefix for callback. Urc could be for example "+CMTI: "
     *  @param callback Callback, which is called if urc is found in AT response
     *  @return NSAPI_ERROR_OK or NSAPI_ERROR_NO_MEMORY if no memory
     */
    nsapi_error_t set_urc_handler(const char *prefix, mbed::Callback<void()> callback);

    /** Remove urc handler from linked list of urc's
     *
     *  @param prefix   Register urc prefix for callback. Urc could be for example "+CMTI: "
     *  @param callback Callback, which is called if urc is found in AT response
     */
    void remove_urc_handler(const char *prefix, mbed::Callback<void()> callback);

    ATHandler *_nextATHandler; // linked list

    /** returns the last error while parsing AT responses.
     *
     *  @return last error
     */
    nsapi_error_t get_last_error() const;

    /** returns the last device error while parsing AT responses. Actually AT error (CME/CMS).
     *
     *  @return last error struct device_err_t
     */
    device_err_t get_last_device_error() const;

    /** Increase reference count. Used for counting references to this instance.
     */
    void inc_ref_count();

    /** Decrease reference count. Used for counting references to this instance.
     */
    void dec_ref_count();

    /** Get the current reference count. Used for counting references to this instance.
     *
     *  @return current reference count
     */
    int get_ref_count();

    /** Set timeout in milliseconds for AT commands
     *
     *  @param timeout_milliseconds  Timeout in milliseconds
     *  @param default_timeout       Store as default timeout
     */
    void set_at_timeout(uint32_t timeout_milliseconds, bool default_timeout = false);

    /** Restore timeout to previous timeout. Handy if there is a need to change timeout temporarily.
     */
    void restore_at_timeout();

    /** Clear pending error flag. By default, error is cleared only in lock().
     */
    void clear_error();

    /**
     * Flushes the underlying stream
     */
    void flush();

    /** Tries to find oob's from the AT response. Call the urc callback if one is found.
     */
    void process_oob();

    /** Set sigio for the current file handle. Sigio event goes through eventqueue so that it's handled in current thread.
     */
    void set_filehandle_sigio();

    /** Set file handle, which is used for reading AT responses and writing AT commands
     *
     *  @param fh file handle used for reading AT responses and writing AT commands
     */
    void set_file_handle(FileHandle *fh);

    /** Set is file handle usable. Some situations like after going to data mode, file handle is not usable anymore.
     *  Any items in queue are not to be processed.
     *
     *  @param usable true for usable filehandle
     */
    void set_is_filehandle_usable(bool usable);

protected:
    void event();
#ifdef AT_HANDLER_MUTEX
    PlatformMutex _fileHandleMutex;
#endif
    FileHandle *_fileHandle;
private:

    void set_error(nsapi_error_t err);

    events::EventQueue &_queue;
    nsapi_error_t _last_err;
    int _last_3gpp_error;
    device_err_t  _last_at_err;
    uint16_t _oob_string_max_length;
    char *_output_delimiter;

    struct oob_t {
        const char *prefix;
        int prefix_len;
        mbed::Callback<void()> cb;
        oob_t *next;
    };
    oob_t *_oobs;
    uint32_t _at_timeout;
    uint32_t _previous_at_timeout;

    uint16_t _at_send_delay;
    uint64_t _last_response_stop;

    bool _fh_sigio_set;

    bool _processing;
    int32_t _ref_count;
    bool _is_fh_usable;

    //*************************************
public:

    /** Starts the command writing by clearing the last error and writing the given command.
     *  In case of failure when writing, the last error is set to NSAPI_ERROR_DEVICE_ERROR.
     *
     *  @param cmd  AT command to be written to modem
     */
    void cmd_start(const char *cmd);

    /** Writes integer type AT command subparameter. Starts with the delimiter if not the first param after cmd_start.
     *  In case of failure when writing, the last error is set to NSAPI_ERROR_DEVICE_ERROR.
     *
     *   @param param int to be written to modem as AT command subparameter
     */
    void write_int(int32_t param);

    /** Writes string type AT command subparamater. Quotes are added to surround the given string.
     *  Starts with the delimiter if not the first param after cmd_start.
     *  In case of failure when writing, the last error is set to NSAPI_ERROR_DEVICE_ERROR.
     *
     *  @param param string to be written to modem as AT command subparameter
     *  @param useQuotations flag indicating whether the string should be included in quotation marks
     */
    void write_string(const char *param,  bool useQuotations = true);

    /** Stops the AT command by writing command-line terminator CR to mark command as finished.
     */
    void cmd_stop();

    /** Write bytes without any subparameter delimiters, such as comma.
     *  In case of failure when writing, the last error is set to NSAPI_ERROR_DEVICE_ERROR.
     *
     *  @param data bytes to be written to modem
     *  @param len  length of data string
     *
     *  @return     number of characters successfully written
     */
    size_t write_bytes(const uint8_t *data, size_t len);

    /** Sets the stop tag for the current scope (response/information response/element)
     *  Parameter's reading routines will stop the reading when such tag is found and will set the found flag.
     *  Consume routines will read everything until such tag is found.
     *
     *  @param stop_tag_seq string to be set as stop tag
     */
    void set_stop_tag(const char *stop_tag_seq);

    /** Sets the delimiter between parameters or between elements of the information response.
     *  Parameter's reading routines will stop when such char is read.
     *
     *  @param delimiter char to be set as _delimiter
     */
    void set_delimiter(char delimiter);

    /** Sets the delimiter to default value defined by DEFAULT_DELIMITER.
     */
    void set_default_delimiter();

    /** Consumes the reading buffer up to the delimiter or stop_tag
     *
     *  @param count number of parameters to be skipped
     */
    void skip_param(uint32_t count = 1);

    /** Consumes the given length from the reading buffer
     *
     *  @param len length to be consumed from reading buffer
     *  @param count number of parameters to be skipped
     */
    void skip_param(ssize_t len, uint32_t count);

    /** Reads given number of bytes from receiving buffer without checking any subparameter delimiters, such as comma.
     *
     *  @param buf output buffer for the read
     *  @param len maximum number of bytes to read
     *  @return number of successfully read bytes or -1 in case of error
     */
    ssize_t read_bytes(uint8_t *buf, size_t len);

    /** Reads chars from reading buffer. Terminates with null. Skips the quotation marks.
     *  Stops on delimiter or stop tag.
     *
     *  @param str output buffer for the read
     *  @param size maximum number of chars to output
     *  @param read_even_stop_tag if true then try to read even if the stop tag was found previously
     *  @return length of output string or -1 in case of read timeout before delimiter or stop tag is found
     */
    ssize_t read_string(char *str, size_t size, bool read_even_stop_tag = false);

    /** Reads chars representing hex ascii values and converts them to the corresponding chars.
     *  For example: "4156" to "AV".
     *  Terminates with null. Skips the quotation marks.
     *  Stops on delimiter or stop tag.
     *
     *  @param str output buffer for the read
     *  @param size maximum number of chars to output
     *  @return length of output string or -1 in case of read timeout before delimiter or stop tag is found
     */
    ssize_t read_hex_string(char *str, size_t size);

    /** Reads as string and converts result to integer. Supports only positive integers.
     *
     *  @return the positive integer or -1 in case of error.
     */
    int32_t read_int();

    /**  This looks for necessary matches: prefix, OK, ERROR, URCs and sets the correct scope.
     *
     *  @param prefix string to be matched from receiving buffer. If not NULL and match succeeds, then scope
     *          will be set as information response(info_type)
     *  @param stop flag to indicate if we go to information response scope or not.
     *        (needed when nothing is expected to be received anymore after the prefix match:
     *         sms case: "> ", bc95 reboot case)
     */
    void resp_start(const char *prefix = NULL, bool stop = false);

    /**  Ends all scopes starting from current scope.
     *   Consumes everything until the scope's stop tag is found, then
     *   goes to next scope until response scope is ending.
     *   Possible sequence:
     *   element scope -> information response scope -> response scope
     */
    void resp_stop();

    /**  Looks for matching the prefix given to resp_start() call.
     *   If needed, it ends the scope of a previous information response.
     *   Sets the information response scope if new prefix is found and response scope if prefix is not found.
     *
     *  @return true if new information response is found, false otherwise
     */
    bool info_resp();

    /**  Looks for matching the start tag.
     *   If needed, it ends the scope of a previous element.
     *   Sets the element scope if start tag is found and information response scope if start tag is not found.
     *
     *  @param start_tag tag to be matched to begin parsing an element of an information response
     *  @return true if new element is found, false otherwise
     */
    bool info_elem(char start_tag);

    /**  Consumes the received content until current stop tag is found.
     *
     *  @return true if stop tag is found, false otherwise
     */
    bool consume_to_stop_tag();

    /** Return the last 3GPP error code.
     *  @return last 3GPP error code
     */
    int get_3gpp_error();

public: // just for debugging
    /**
     * AT debugging, when enabled will print all data read and written,
     * non-printable chars are printed as "[%d]".
     *
     * AT debug can be enabled at compile time using MBED_CONF_CELLULAR_DEBUG_AT flag or at runtime
     * calling set_debug(). Note that MBED_CONF_MBED_TRACE_ENABLE must also be enabled.
     *
     *  @param debug_on Enable/disable debugging
     */
    void set_debug(bool debug_on);

private:

    // should fit any prefix and int
    char _recv_buff[BUFF_SIZE];
    // reading position
    size_t _recv_len;
    // reading length
    size_t _recv_pos;

    // resp_type: the part of the response that doesn't include the information response (+CMD1,+CMD2..)
    //            ends with OK or (CME)(CMS)ERROR
    // info_type: the information response part of the response: starts with +CMD1 and ends with CRLF
    //            information response contains parameters or subsets of parameters (elements), both separated by comma
    // elem_type: subsets of parameters that are part of information response, its parameters are separated by comma
    enum ScopeType {RespType, InfoType, ElemType, NotSet};
    void set_scope(ScopeType scope_type);
    ScopeType _current_scope;

    struct tag_t {
        char tag[7];
        size_t len;
        bool found;
    };

    // tag to stop response scope
    tag_t _resp_stop;
    // tag to stop information response scope
    tag_t _info_stop;
    // tag to stop element scope
    tag_t _elem_stop;
    // reference to the stop tag of current scope (resp/info/elem)
    tag_t *_stop_tag;

    // delimiter between parameters and also used for delimiting elements of information response
    char _delimiter;
    // set true on prefix match -> indicates start of an information response or of an element
    bool _prefix_matched;
    // set true on urc match
    bool _urc_matched;
    // set true on (CME)(CMS)ERROR
    bool _error_found;
    // Max length of OK,(CME)(CMS)ERROR and URCs
    size_t _max_resp_length;

    // prefix set during resp_start and used to try matching possible information responses
    char _info_resp_prefix[BUFF_SIZE];
    bool _debug_on;
    bool _cmd_start;

    // time when a command or an URC processing was started
    uint64_t _start_time;

    // Gets char from receiving buffer.
    // Resets and fills the buffer if all are already read (receiving position equals receiving length).
    // Returns a next char or -1 on failure (also sets error flag)
    int get_char();
    // Sets to 0 the reading position, reading length and the whole buffer content.
    void reset_buffer();
    // Reading position set to 0 and buffer's unread content moved to beginning
    void rewind_buffer();
    // Calculate remaining time for polling based on request start time and AT timeout.
    // Returns 0 or time in ms for polling.
    int poll_timeout(bool wait_for_timeout = true);
    // Reads from serial to receiving buffer.
    // Returns true on successful read OR false on timeout.
    bool fill_buffer(bool wait_for_timeout = true);

    void set_tag(tag_t *tag_dest, const char *tag_seq);

    // Rewinds the receiving buffer and compares it against given str.
    bool match(const char *str, size_t size);
    // Iterates URCs and checks if they match the receiving buffer content.
    // If URC match sets the scope to information response and after urc's cb returns
    // finishes the information response scope(consumes to CRLF).
    bool match_urc();
    // Checks if any of the error strings are matching the receiving buffer content.
    bool match_error();
    // Checks if current char in buffer matches ch and consumes it,
    // if no match lets the buffer unchanged.
    bool consume_char(char ch);
    // Consumes the received content until tag is found.
    // Consumes the tag only if consume_tag flag is true.
    bool consume_to_tag(const char *tag, bool consume_tag);
    // Checks if receiving buffer contains OK, ERROR, URC or given prefix.
    void resp(const char *prefix, bool check_urc);


    ScopeType get_scope();

    // Consumes to information response stop tag which is CRLF. Sets scope to response.
    void information_response_stop();
    // Consumes to element stop tag. Sets scope to information response
    void information_response_element_stop();

    // Reads the error code if expected and sets it as last error.
    void at_error(bool error_code, DeviceErrorType error_type);

    /** Convert AT error code to 3GPP error codes
     *  @param err AT error code read from CME/CMS ERROR responses
     *  @param error_type error type (CMS/CME/ERROR)
     */
    void set_3gpp_error(int err, DeviceErrorType error_type);

    bool check_cmd_send();
    size_t write(const void *data, size_t len);

    /** Copy content of one char buffer to another buffer and sets NULL terminator
     *
     *  @param dest                  destination char buffer
     *  @param src                   source char buffer
     *  @param src_len               number of bytes to copy
     *
     */
    void set_string(char *dest, const char *src, size_t src_len);

    /** Finds occurrence of one char buffer inside another char buffer.
     *
     * @param dest                  destination char buffer
     * @param dest_len              length of dest
     * @param src                   string to be searched for
     * @param src_len               length of string to be searched for
     *
     * @return pointer to first occurrence of src in dest
     */
    const char *mem_str(const char *dest, size_t dest_len, const char *src, size_t src_len);

    // check is urc is already added
    bool find_urc_handler(const char *prefix, mbed::Callback<void()> callback);

    ssize_t read(char *buf, size_t size, bool read_even_stop_tag, bool hex);

    // print contents of a buffer to trace log
    void debug_print(char *p, int len);
};

} // namespace mbed

#endif //AT_HANDLER_H_
