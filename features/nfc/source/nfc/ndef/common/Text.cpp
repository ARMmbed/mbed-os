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

#include <string.h>

#include "nfc/ndef/common/Text.h"

namespace {
static const uint8_t utf16_encoding_bit = (1 << 7);
static const uint8_t language_code_size_mask = 0x3F;
static const uint8_t header_index = 0;
static const uint8_t language_code_index = 1;
static const uint8_t header_size = 1;
static const uint8_t text_record_type_value[] = { 'T' };
}

namespace mbed {
namespace nfc {
namespace ndef {
namespace common {

Text::Text() :
    _text_record(NULL),
    _text_record_size(0)
{ }

Text::Text(const Text &other) :
    _text_record(other._text_record ? new uint8_t[other._text_record_size] : NULL),
    _text_record_size(other._text_record_size)
{
    memcpy(_text_record, other._text_record, _text_record_size);
}

Text::Text(
    encoding_t text_encoding,
    const Span<const uint8_t> &language_code,
    const Span<const uint8_t> &text
) : _text_record(NULL),
    _text_record_size(0)
{
    set_text(text_encoding, language_code, text);
}

Text::~Text()
{
    delete[] _text_record;
}

Text &Text::operator=(const Text &other)
{
    if (this == &other) {
        return *this;
    }

    _text_record_size = other._text_record_size;

    delete[] _text_record;
    if (!other._text_record) {
        _text_record = NULL;
    } else {
        _text_record = new uint8_t[_text_record_size];
        memcpy(_text_record, other._text_record, _text_record_size);
    }

    return *this;
}

void Text::set_text(
    encoding_t text_encoding,
    const Span<const uint8_t> &language_code,
    const Span<const uint8_t> &text
)
{
    delete[] _text_record;

    _text_record_size = header_size + language_code.size() + text.size();
    _text_record = new uint8_t[_text_record_size];

    // build the header
    _text_record[header_index] = 0;
    if (text_encoding == UTF16) {
        _text_record[header_index] |= utf16_encoding_bit;
    }
    _text_record[header_index] |= language_code.size();

    // language code
    memcpy(_text_record + language_code_index, language_code.data(), language_code.size());

    // actual text
    memcpy(_text_record + language_code_index + language_code.size(), text.data(), text.size());
}

Text::encoding_t Text::get_encoding() const
{
    return (_text_record[header_index] & utf16_encoding_bit) ? UTF16 : UTF8;
}

Span<const uint8_t> Text::get_language_code() const
{
    return make_const_Span(
               _text_record + language_code_index,
               _text_record[header_index] & language_code_size_mask
           );
}

Span<const uint8_t> Text::get_text() const
{
    if (!_text_record) {
        return Span<const uint8_t>();
    }

    size_t language_code_size = get_language_code().size();

    return make_const_Span(
               _text_record + header_size + language_code_size,
               _text_record_size - header_size - language_code_size
           );
}

void Text::move_data(uint8_t *text, size_t size)
{
    delete[] _text_record;
    _text_record = text;
    _text_record_size = size;
}

bool Text::append_as_record(
    MessageBuilder &message_builder,
    bool is_last_record
) const
{
    if (!_text_record) {
        return false;
    }

    // Build the record type
    RecordType type(
        RecordType::well_known_type,
        text_record_type_value
    );

    // build the record payload
    RecordPayload payload(_text_record, _text_record_size);
    return message_builder.append_record(type, payload, is_last_record);
}

size_t Text::get_record_size() const
{
    if (!_text_record) {
        return 0;
    }

    return MessageBuilder::compute_record_size(
               Record(
                   RecordType(
                       RecordType::well_known_type,
                       text_record_type_value
                   ),
                   RecordPayload(_text_record, _text_record_size),
                   RecordID(),
                   /* chunk */ false,
                   /* last record */ false
               )
           );
}

bool TextParser::do_parse(const Record &record, Text &text)
{
    if (record.type.tnf != RecordType::well_known_type) {
        return false;
    }

    // the record type value should be equal to `T`
    if (record.type.value != make_const_Span(text_record_type_value) ||
            record.payload.empty()
       ) {
        return false;
    }

    // create the buffer
    size_t text_record_size = record.payload.size();
    uint8_t *text_record = new uint8_t[text_record_size];
    memcpy(text_record, record.payload.data(), text_record_size);

    text.move_data(text_record, text_record_size);

    return true;
}

} // namespace common
} // namespace ndef
} // namespace nfc
} // namespace mbed
