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

#include "nfc/ndef/MessageBuilder.h"

namespace mbed {
namespace nfc {
namespace ndef {

MessageBuilder::MessageBuilder(const Span<uint8_t> &buffer) :
    _message_buffer(buffer),
    _position(0),
    _message_started(false),
    _message_ended(false),
    _in_chunk(false)
{ }

bool MessageBuilder::append_record(
    const RecordType &type,
    const RecordPayload &payload,
    bool is_last_record
)
{
    Record record(
        type,
        payload,
        /* id */ RecordID(),
        /* chunk */ false,
        is_last_record
    );

    return append_record(record);
}

bool MessageBuilder::append_record(
    const RecordType &type,
    const PayloadBuilder &builder,
    bool is_last_record
)
{
    Record record(
        type,
        RecordPayload(),
        RecordID(),
        /* chunk */ false,
        is_last_record
    );

    return append_record(record, &builder);
}


bool MessageBuilder::append_record(const Record &record, const PayloadBuilder *builder)
{
    if (_message_ended) {
        return false;
    }

    if (record.type.value.size() > 255) {
        return false;
    }

    if (record.id.size() > 255) {
        return false;
    }

    if (!record.id.empty() && _in_chunk) {
        if (record.chunk) {
            // middle chunk
            return false;
        } else if (record.type.tnf == RecordType::unchanged) {
            // terminating chunk
            return false;
        }
    }

    if (_in_chunk && record.type.tnf != RecordType::unchanged) {
        return false;
    }

    if (!_in_chunk && record.chunk && record.type.tnf == RecordType::unchanged) {
        return false;
    }

    if (record.type.tnf == RecordType::empty) {
        if (!record.type.value.empty()) {
            return false;
        }

        if (!record.id.empty()) {
            return false;
        }

        if (get_payload_size(record, builder)) {
            return false;
        }
    }

    if (record.type.tnf == RecordType::well_known_type ||
            record.type.tnf == RecordType::media_type ||
            record.type.tnf == RecordType::absolute_uri ||
            record.type.tnf == RecordType::external_type
       ) {
        if (record.type.value.empty()) {
            return false;
        }
    }

    if (record.type.tnf == RecordType::unknown && !record.type.value.empty()) {
        return false;
    }

    size_t record_size = compute_record_size(record, builder);
    if (record_size > (_message_buffer.size() - _position)) {
        return false;
    }

    append_header(record, builder);
    append_type_length(record);
    append_payload_length(record, builder);
    append_id_length(record);
    append_type(record);
    append_id(record);
    append_payload(record, builder);

    if (record.chunk) {
        _in_chunk = true;
    } else if (record.type.tnf == RecordType::unchanged) {
        // last chunk reached
        _in_chunk = false;
    }

    return true;
}

void MessageBuilder::reset()
{
    _position = 0;
    _message_started = false;
    _message_ended = false;
    _in_chunk = false;
}

void MessageBuilder::reset(const Span<uint8_t> &buffer)
{
    _message_buffer = buffer;
    _position = 0;
    _message_started = false;
    _message_ended = false;
    _in_chunk = false;
}

bool MessageBuilder::is_message_complete() const
{
    return _message_ended;
}

Span<const uint8_t> MessageBuilder::get_message() const
{
    if (is_message_complete()) {
        return _message_buffer.first(_position);
    } else {
        return Span<const uint8_t>();
    }
}

size_t MessageBuilder::compute_record_size(const Record &record, const PayloadBuilder *builder)
{
    size_t record_size = 0;
    record_size = 1; /* header */
    record_size += 1; /* type length */
    record_size += is_short_payload(record, builder) ? 1 : 4;

    if (!record.id.empty()) {
        record_size += 1;
    }

    record_size += record.type.value.size();
    record_size += record.id.size();
    record_size += get_payload_size(record, builder);

    return record_size;
}

void MessageBuilder::append_header(const Record &record, const PayloadBuilder *builder)
{
    uint8_t header = 0;
    if (!_message_started) {
        header |= Header::message_begin_bit;
        _message_started = true;
    }

    if (record.last_record) {
        header |= Header::message_end_bit;
        _message_ended = true;
    }

    if (record.chunk) {
        header |= Header::chunk_flag_bit;
    }

    if (is_short_payload(record, builder)) {
        header |= Header::short_record_bit;
    }

    if (record.id.size()) {
        header |= Header::id_length_bit;
    }

    header |= record.type.tnf;
    _message_buffer[_position++] = header;
}

void MessageBuilder::append_type_length(const Record &record)
{
    _message_buffer[_position++] = record.type.value.size();
}

void MessageBuilder::append_payload_length(const Record &record, const PayloadBuilder *builder)
{
    size_t size = get_payload_size(record, builder);

    if (is_short_payload(record, builder)) {
        _message_buffer[_position++] = size;
    } else {
        _message_buffer[_position++] = (size >> 24) & 0xFF;
        _message_buffer[_position++] = (size >> 16) & 0xFF;
        _message_buffer[_position++] = (size >> 8) & 0xFF;
        _message_buffer[_position++] = size & 0xFF;
    }
}

void MessageBuilder::append_id_length(const Record &record)
{
    if (record.id.empty()) {
        return;
    }

    _message_buffer[_position++] = record.id.size();
}

void MessageBuilder::append_type(const Record &record)
{
    if (record.type.value.empty()) {
        return;
    }

    memcpy(
        _message_buffer.data() + _position,
        record.type.value.data(),
        record.type.value.size()
    );
    _position += record.type.value.size();
}

void MessageBuilder::append_id(const Record &record)
{
    if (record.id.empty()) {
        return;
    }

    memcpy(
        _message_buffer.data() + _position,
        record.id.data(),
        record.id.size()
    );
    _position += record.id.size();
}

void MessageBuilder::append_payload(const Record &record, const PayloadBuilder *builder)
{
    size_t size = get_payload_size(record, builder);
    if (!size) {
        return;
    }

    if (builder) {
        builder->build(_message_buffer.subspan(_position, size));
    } else {
        memcpy(
            _message_buffer.data() + _position,
            record.payload.data(),
            size
        );
    }

    _position += size;
}

bool MessageBuilder::is_short_payload(const Record &record, const PayloadBuilder *builder)
{
    if (get_payload_size(record, builder) <= 255) {
        return true;
    } else {
        return false;
    }
}

size_t MessageBuilder::get_payload_size(const Record &record, const PayloadBuilder *builder)
{
    return builder ? builder->size() : record.payload.size();
}

} // namespace ndef
} // namespace nfc
} // namespace mbed

