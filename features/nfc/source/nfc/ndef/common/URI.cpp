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

#include "nfc/ndef/common/URI.h"

namespace {
static const uint8_t uri_id_code_size = 1;
static const uint8_t uri_id_index = 0;
static const uint8_t uri_field_index = 1;
static const uint8_t uri_record_type_value[] = { 'U' } ;
}

namespace mbed {
namespace nfc {
namespace ndef {
namespace common {

URI::URI() :
    _uri(NULL),
    _uri_size(0)
{
}

URI::URI(uri_identifier_code_t id, const Span<const uint8_t> &uri_field) :
    _uri(uri_field.size() ? new uint8_t[uri_id_code_size + uri_field.size()] : NULL),
    _uri_size(uri_id_code_size + uri_field.size())
{
    _uri[uri_id_index] = id;
    memcpy(_uri + uri_field_index, uri_field.data(), uri_field.size());
}

URI::URI(const URI &other) :
    _uri(other._uri ? new uint8_t[other._uri_size] : NULL),
    _uri_size(other._uri_size)
{
    memcpy(_uri, other._uri, other._uri_size);
}

URI::~URI()
{
    delete[] _uri;
}

URI &URI::operator=(const URI &other)
{
    delete[] _uri;

    if (!other._uri) {
        _uri = NULL;
        _uri_size = 0;
    } else {
        _uri = new uint8_t[other._uri_size];
        _uri_size = other._uri_size;
        memcpy(_uri, other._uri, other._uri_size);
    }

    return *this;
}

void URI::set_uri(
    uri_identifier_code_t id,
    const Span<const uint8_t> &uri_field
)
{
    delete[] _uri;

    if (uri_field.empty()) {
        _uri = NULL;
        _uri_size = 0;
        return;
    }

    _uri = new uint8_t[uri_id_code_size + uri_field.size()];
    _uri_size = uri_id_code_size + uri_field.size();
    _uri[uri_id_index] = id;
    memcpy(_uri + uri_field_index, uri_field.data(), uri_field.size());
}

URI::uri_identifier_code_t URI::get_id() const
{
    if (!_uri) {
        return NA;
    }

    return static_cast<uri_identifier_code_t>(_uri[uri_id_index]);
}

Span<const uint8_t> URI::get_uri_field() const
{
    if (!_uri) {
        return Span<const uint8_t>();
    }
    return make_const_Span(
               _uri + uri_field_index,
               _uri_size - uri_id_code_size
           );
}

bool URI::append_as_record(MessageBuilder &message_builder, bool is_last_record) const
{
    if (!_uri) {
        return false;
    }

    // Build the record type
    RecordType type(
        RecordType::well_known_type,
        uri_record_type_value
    );

    // build the record payload
    RecordPayload payload(_uri, _uri_size);

    return message_builder.append_record(type, payload, is_last_record);
}

size_t URI::get_record_size() const
{
    if (!_uri) {
        return 0;
    }

    return MessageBuilder::compute_record_size(
               Record(
                   RecordType(
                       RecordType::well_known_type,
                       uri_record_type_value
                   ),
                   RecordPayload(_uri, _uri_size),
                   RecordID(),
                   /* chunk */ false,
                   /* last record */ false
               )
           );
}

void URI::move_data(uint8_t *new_uri, size_t new_uri_size)
{
    delete[] _uri;
    _uri = new_uri;
    _uri_size = new_uri_size;
}

bool URIParser::do_parse(const Record &record, URI &uri)
{
    if (record.type.tnf != RecordType::well_known_type) {
        return false;
    }

    // the record type value should be equal to `U`
    if (record.type.value != make_const_Span(uri_record_type_value) ||
            record.payload.empty()
       ) {
        return false;
    }

    // create the buffer
    size_t uri_record_size = record.payload.size();
    uint8_t *uri_record = new uint8_t[uri_record_size];
    memcpy(uri_record, record.payload.data(), uri_record_size);

    uri.move_data(uri_record, uri_record_size);

    return true;
}

} // namespace common
} // namespace ndef
} // namespace nfc
} // namespace mbed


