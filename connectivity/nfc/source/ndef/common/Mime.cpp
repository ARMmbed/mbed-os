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

#include "nfc/ndef/common/Mime.h"

namespace mbed {
namespace nfc {
namespace ndef {
namespace common {

Mime::Mime() :
    _mime(NULL),
    _type_size(0),
    _content_size(0)
{ }

Mime::Mime(
    const Span<const uint8_t> &mime_type,
    const Span<const uint8_t> &content
) : _mime(new uint8_t[mime_type.size() + content.size()]),
    _type_size(mime_type.size()),
    _content_size(content.size())
{
    memcpy(_mime, mime_type.data(), mime_type.size());
    memcpy(_mime + mime_type.size(), content.data(), content.size());
}

Mime::Mime(const Mime &to_copy) :
    _mime(new uint8_t[to_copy.mime_size()]),
    _type_size(to_copy._type_size),
    _content_size(to_copy._content_size)
{
    memcpy(_mime, to_copy._mime, to_copy.mime_size());
}

Mime::~Mime()
{
    delete[] _mime;
}

Mime &Mime::operator=(const Mime &to_copy)
{
    if (this == &to_copy) {
        return * this;
    }

    delete[] _mime;

    _mime = new uint8_t[to_copy.mime_size()];
    memcpy(_mime, to_copy._mime, to_copy.mime_size());
    _type_size = to_copy._type_size;
    _content_size = to_copy._content_size;

    return *this;
}

void Mime::set_mime(
    const Span<const uint8_t> &mime_type,
    const Span<const uint8_t> &content
)
{
    delete[] _mime;

    _mime = new uint8_t[mime_type.size() + content.size()];
    memcpy(_mime, mime_type.data(), mime_type.size());
    memcpy(_mime + mime_type.size(), content.data(), content.size());
    _type_size = mime_type.size();
    _content_size = content.size();
}

Span<const uint8_t> Mime::get_mime_type() const
{
    return make_const_Span(_mime, _type_size);
}

Span<const uint8_t> Mime::get_mime_content() const
{
    return make_const_Span(_mime + _type_size, _content_size);
}

bool Mime::append_as_record(
    MessageBuilder &message_builder,
    bool is_last_record
) const
{
    return message_builder.append_record(
               RecordType(
                   RecordType::media_type,
                   get_mime_type()
               ),
               get_mime_content(),
               is_last_record
           );
}

size_t Mime::get_record_size() const
{
    return MessageBuilder::compute_record_size(
               Record(
                   RecordType(
                       RecordType::media_type,
                       get_mime_type()
                   ),
                   get_mime_content(),
                   RecordID(),
                   /* chunk */ false,
                   /* last record */ false
               )
           );
}

void Mime::move_data(
    uint8_t *mime_record,
    size_t mime_type_size,
    size_t mime_content_size
)
{
    delete[] _mime;
    _mime = mime_record;
    _type_size = mime_type_size;
    _content_size = mime_content_size;
}

size_t Mime::mime_size() const
{
    return _type_size + _content_size;
}

bool MimeParser::do_parse(const Record &record, Mime &mime)
{
    if (record.type.tnf != RecordType::media_type) {
        return false;
    }

    // A type and a payload should be present
    if (record.type.value.empty() || record.payload.empty()) {
        return false;
    }

    // create the buffer
    size_t type_size = record.type.value.size();
    size_t content_size = record.payload.size();
    uint8_t *mime_buffer = new uint8_t[type_size + content_size];

    // copy type
    memcpy(mime_buffer, record.type.value.data(), type_size);

    // copy content
    memcpy(mime_buffer + type_size, record.payload.data(), content_size);

    mime.move_data(mime_buffer, type_size, content_size);

    return true;
}

} // namespace common
} // namespace ndef
} // namespace nfc
} // namespace mbed
