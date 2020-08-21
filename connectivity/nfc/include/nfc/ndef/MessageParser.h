/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef NFC_NDEF_MESSAGEPARSER_H_
#define NFC_NDEF_MESSAGEPARSER_H_

#include <stdlib.h>
#include "platform/Span.h"

namespace mbed {
namespace nfc {
namespace ndef {

/** @addtogroup nfc
 * @{
 */

// Forward declaration
class Record;

/**
 * Event driven NDEF Message parser
 */
class MessageParser {
public:
    /**
     * Error that can be reported by the parsing operation.
     */
    enum error_t {
        /**
         * The message doesn't start with a message start tag.
         */
        INVALID_MESSAGE_START,

        /**
         * There is not enough data left to pursue parsing of the message.
         */
        INSUFICIENT_DATA,

        /**
         * The type name of a record is invalid.
         */
        INVALID_TYPE_NAME_FORMAT,

        /**
         * An empty record is malformed.
         */
        INVALID_EMPTY_RECORD,

        /**
         * Record of unknown type embed a type length different than 0.
         */
        INVALID_UNKNOWN_TYPE_LENGTH,

        /**
         * Record of unchanged type contains a type.
         */
        INVALID_UNCHANGED_TYPE,

        /**
         * Chunk record encountered.
         */
        CHUNK_RECORD_NOT_SUPPORTED,

        /**
         * Message is not properly closed.
         */
        MISSING_MESSAGE_END,

        /**
         * Type is missing in a record expecting a type (well known type, media
         * type, absolute uri or external type).
         */
        MISSING_TYPE_VALUE
    };

    /**
     * Report parsing event to the application.
     */
    struct Delegate {
        /**
         * Invoked when parsing as started.
         */
        virtual void on_parsing_started() { }

        /**
         * Invoked when a record has been parsed.
         *
         * @param record The record obtained from parsing.
         */
        virtual void on_record_parsed(const Record &record) { }

        /**
         * Invoked when parsing is over.
         */
        virtual void on_parsing_terminated() { }

        /**
         * Invoked when an error is present in the message.
         * @param error The error present in the message.
         */
        virtual void on_parsing_error(error_t error) { }

    protected:
        /**
         * Protected non virtual destructor.
         * Delegate is not meant to be destroyed in a polymorphic manner.
         */
        ~Delegate() { }
    };

    /**
     * Construct a message parser.
     */
    MessageParser();

    /**
     * Set the handler that processes parsing events.
     * @param delegate The parsing event handler.
     */
    void set_delegate(Delegate *delegate);

    /**
     * Parse an NDEF Message.
     *
     * Records and errors are reported to the handler registered with
     * set_event_handler.
     *
     * @param data_buffer The data buffer that contains the NDEF message.
     */
    void parse(const Span<const uint8_t> &data_buffer);

private:
    struct parsing_state_t;

    // parser
    bool parse_record(parsing_state_t &it);

    static uint8_t compute_lengths_size(uint8_t header);
    static uint8_t extract_type_length(parsing_state_t &s);
    static uint32_t extract_payload_length(parsing_state_t &s, uint8_t header);
    static uint8_t extract_id_length(parsing_state_t &s, uint8_t header);

    // reporting
    void report_parsing_started();
    void report_record_parsed(const Record &record);
    void report_parsing_terminated();
    void report_parsing_error(error_t error, parsing_state_t &parsing_state);

    Delegate *_delegate;
};
/** @}*/
} // namespace ndef
} // namespace nfc
} // namespace mbed


#endif /* NFC_NDEF_MESSAGEPARSER_H_ */
