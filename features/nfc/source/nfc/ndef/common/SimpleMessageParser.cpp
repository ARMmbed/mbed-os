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

#include "nfc/ndef/common/SimpleMessageParser.h"

namespace mbed {
namespace nfc {
namespace ndef {
namespace common {

SimpleMessageParser::SimpleMessageParser() :
    _message_parser(),
    _record_parser_chain(),
    _uri_parser(),
    _text_parser(),
    _mime_parser(),
    _delegate(NULL)
{
    // setup the parser chain
    _record_parser_chain.set_next_parser(&_uri_parser);
    _record_parser_chain.set_next_parser(&_text_parser);
    _record_parser_chain.set_next_parser(&_mime_parser);

    // wire event handling
    _message_parser.set_delegate(this);
    _uri_parser.set_delegate(this);
    _text_parser.set_delegate(this);
    _mime_parser.set_delegate(this);
}

void SimpleMessageParser::set_delegate(Delegate *delegate)
{
    _delegate = delegate;
}

void SimpleMessageParser::parse(const Span<const uint8_t> &data_buffer)
{
    _message_parser.parse(data_buffer);
}

void SimpleMessageParser::add_record_parser(RecordParser *parser)
{
    _record_parser_chain.set_next_parser(parser);
}

void SimpleMessageParser::on_parsing_error(MessageParser::error_t error)
{
    if (_delegate) {
        _delegate->on_parsing_error(error);
    }
}

void SimpleMessageParser::on_parsing_started()
{
    if (_delegate) {
        _delegate->on_parsing_started();
    }
}

void SimpleMessageParser::on_record_parsed(const Record &record)
{
    bool parsed = _record_parser_chain.parse(record);

    if (!parsed && _delegate) {
        _delegate->on_unknown_record_parsed(record);
    }
}

void SimpleMessageParser::on_parsing_terminated()
{
    if (_delegate) {
        _delegate->on_parsing_terminated();
    }
}

void SimpleMessageParser::on_record_parsed(
    const URI &uri,
    const RecordID &id
)
{
    if (_delegate) {
        _delegate->on_uri_parsed(uri, id);
    }
}

void SimpleMessageParser::on_record_parsed(
    const Text &text,
    const RecordID &id
)
{
    if (_delegate) {
        _delegate->on_text_parsed(text, id);
    }
}

void SimpleMessageParser::on_record_parsed(
    const Mime &mime,
    const RecordID &id
)
{
    if (_delegate) {
        _delegate->on_mime_parsed(mime, id);
    }
}

} // namespace common
}  // namespace ndef
} // namespace nfc
} // namespace mbed


