/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef NFC_COMMON_SIMPLEMESSAGEPARSER_H_
#define NFC_COMMON_SIMPLEMESSAGEPARSER_H_

#include "platform/Span.h"

#include "nfc/ndef/MessageParser.h"
#include "nfc/ndef/RecordParser.h"
#include "nfc/ndef/common/URI.h"
#include "nfc/ndef/common/Text.h"
#include "nfc/ndef/common/Mime.h"

namespace mbed {
namespace nfc {
namespace ndef {
namespace common {

/** @addtogroup nfc
 * @{
 */

/**
 * Basic message parser that aggregates URIParser, TextParser and MimeParser.
 *
 * Custom parsers can be added at runtime as well.
 */
class SimpleMessageParser :
    MessageParser::Delegate,
    URIParser::Delegate,
    TextParser::Delegate,
    MimeParser::Delegate {
public:
    /**
     * Delegate invoked when the parser raise an event.
     */
    struct Delegate {
        /**
         * Invoked when an error is present in the message.
         * @param error The error present in the message.
         */
        virtual void on_parsing_error(MessageParser::error_t error) { }

        /**
         * Invoked when parsing as started.
         */
        virtual void on_parsing_started() { }

        /**
         * Invoked when a text element has been parsed.
         *
         * @param text The text parsed.
         * @param id The RecordId of the text object.
         */
        virtual void on_text_parsed(const Text &text, const RecordID &id) { }

        /**
         * Invoked when a text element has been parsed.
         *
         * @param uri The uri parsed.
         * @param id The RecordId of the uri object.
         */
        virtual void on_uri_parsed(const URI &uri, const RecordID &id) { }

        /**
         * Invoked when a mime element has been parsed.
         *
         * @param mime The mime object parsed.
         * @param id The RecordId of the mime object.
         */
        virtual void on_mime_parsed(const Mime &mime, const RecordID &id) { }

        /**
         * Invoked when an unknown record has been parsed.
         * @param record The record freshly parsed.
         */
        virtual void on_unknown_record_parsed(const Record &record) { }

        /**
         * Invoked when parsing is over.
         */
        virtual void on_parsing_terminated() { }

    protected:
        ~Delegate() { }
    };

    /**
     * Construct a new CommonMessageParser.
     */
    SimpleMessageParser();

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

    /**
     * Insert a new parser in the parser chain.
     * @param parser The parser to add in the parsing chain.
     */
    void add_record_parser(RecordParser *parser);

private:
    ////////////////////////////////////////////////////////////////////////////
    /// Implementation of MessageParser::EventHandler

    virtual void on_parsing_error(MessageParser::error_t error);

    virtual void on_parsing_started();

    virtual void on_record_parsed(const Record &record);

    virtual void on_parsing_terminated();

    ////////////////////////////////////////////////////////////////////////////
    /// Implementation of URIParser::EventHandler

    virtual void on_record_parsed(const URI &uri, const RecordID &id);

    ////////////////////////////////////////////////////////////////////////////
    /// Implementation of TextParser::EventHandler

    virtual void on_record_parsed(const Text &text, const RecordID &id);

    ////////////////////////////////////////////////////////////////////////////
    /// Implementation of MimeParser::EventHandler

    virtual void on_record_parsed(const Mime &mime, const RecordID &id);

    MessageParser _message_parser;
    RecordParserChain _record_parser_chain;
    URIParser _uri_parser;
    TextParser _text_parser;
    MimeParser _mime_parser;
    Delegate *_delegate;
};
/** @}*/
} // namespace common
} // namespace ndef
} // namespace nfc
} // namespace mbed

#endif /* NFC_COMMON_SIMPLEMESSAGEPARSER_H_ */

