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

#include "nfc/ndef/MessageParser.h"
#include "nfc/ndef/Record.h"

namespace {
struct buffer_iterator_t {
    buffer_iterator_t(const mbed::Span<const uint8_t> &buffer) :
        buffer(buffer),
        position(0)
    { }

    uint8_t operator*()
    {
        return buffer[position];
    }

    buffer_iterator_t &operator++()
    {
        ++position;
        return *this;
    }

    buffer_iterator_t operator++(int)
    {
        buffer_iterator_t previous = *this;
        ++*this;
        return previous;
    }

    buffer_iterator_t &operator+=(size_t increment)
    {
        position += increment;
        return *this;
    }

    operator bool() const
    {
        return (position >= buffer.size()) ? false : true;
    }

    size_t remaining_size() const
    {
        return buffer.size() - position;
    }

    void read_le(uint8_t *dest, size_t size)
    {
        memcpy(dest, buffer.data() + position, size);
        position += size;
    }

    void read_be(uint8_t *dest, size_t size)
    {
        // TODO: Needs proper network to host function
        std::reverse_copy(
            buffer.data() + position,
            buffer.data() + position + size,
            dest
        );
        position += size;
    }

    mbed::Span<const uint8_t> get_underlying_buffer() const
    {
        return buffer.last(buffer.size() - position);
    }

private:
    mbed::Span<const uint8_t> buffer;
    mbed::Span<const uint8_t>::index_type position;
};

} // end of anonymous namespace

namespace mbed {
namespace nfc {
namespace ndef {

struct MessageParser::parsing_state_t {
    parsing_state_t(const Span<const uint8_t> &data_buffer) :
        it(data_buffer),
        first_record_parsed(false),
        last_record_parsed(false),
        error(false)
    { }

    buffer_iterator_t it;
    bool first_record_parsed: 1;
    bool last_record_parsed: 1;
    bool error: 1;
};

MessageParser::MessageParser() :
    _delegate(NULL)
{ }

void MessageParser::set_delegate(Delegate *delegate)
{
    _delegate = delegate;
}

void MessageParser::parse(const Span<const uint8_t> &data_buffer)
{
    parsing_state_t parsing_state(data_buffer);
    report_parsing_started();
    while (parsing_state.it && parse_record(parsing_state));
    if (!parsing_state.error && !parsing_state.last_record_parsed) {
        report_parsing_error(MISSING_MESSAGE_END, parsing_state);
    }
    report_parsing_terminated();
}

bool MessageParser::parse_record(parsing_state_t &s)
{
    if (s.error || s.last_record_parsed) {
        return false;
    }

    // ensure that the header can be extracted
    if (s.it.remaining_size() < 1) {
        report_parsing_error(INSUFICIENT_DATA, s);
        return false;
    }

    uint8_t header = *s.it++;

    // NOTE: report an error until the chunk parsing design is sorted out
    if (header & Header::chunk_flag_bit) {
        report_parsing_error(CHUNK_RECORD_NOT_SUPPORTED, s);
        return false;
    }

    // handle first record cases
    if (s.first_record_parsed == false) {
        if (header & Header::message_begin_bit) {
            s.first_record_parsed = true;
        } else {
            report_parsing_error(INVALID_MESSAGE_START, s);
            return false;
        }
    } else if (header & Header::message_begin_bit) {
        report_parsing_error(INVALID_MESSAGE_START, s);
        return false;
    }

    // handle last record
    if (header & Header::message_end_bit) {
        s.last_record_parsed = true;
    }

    // ensure their is enough space to contain the type length, payload
    // length and id length
    uint8_t lengths_size = compute_lengths_size(header);
    if (s.it.remaining_size() < lengths_size) {
        report_parsing_error(INSUFICIENT_DATA, s);
        return false;
    }

    // extract the various length from the message
    uint8_t type_length = extract_type_length(s);
    uint32_t payload_length = extract_payload_length(s, header);
    uint8_t id_length = extract_id_length(s, header);

    // there should be enough bytes left in the buffer
    if (s.it.remaining_size() < (type_length + id_length + payload_length)) {
        report_parsing_error(INSUFICIENT_DATA, s);
        return false;
    }

    // validate the Type Name Format of the header
    switch (header & Header::tnf_bits) {
        case RecordType::empty:
            if (type_length || payload_length || id_length) {
                report_parsing_error(INVALID_EMPTY_RECORD, s);
                return false;
            }
            break;
        case RecordType::well_known_type:
        case RecordType::media_type:
        case RecordType::absolute_uri:
        case RecordType::external_type:
            if (!type_length) {
                report_parsing_error(MISSING_TYPE_VALUE, s);
                return false;
            }
            break;
        case RecordType::unknown:
            if (type_length) {
                report_parsing_error(INVALID_UNKNOWN_TYPE_LENGTH, s);
                return false;
            }
            break;
        case RecordType::unchanged:
            // shouldn't be handled outside of chunk handling
            report_parsing_error(INVALID_UNCHANGED_TYPE, s);
            return false;
        default:
            report_parsing_error(INVALID_TYPE_NAME_FORMAT, s);
            return false;
    }

    // build the record
    Record record;

    // flags
    record.last_record = header & Header::message_end_bit;

    // type
    record.type.tnf = static_cast<RecordType::tnf_t>(header & Header::tnf_bits);
    if (type_length) {
        record.type.value = s.it.get_underlying_buffer().first(type_length);
        s.it += type_length;
    }

    // id
    if (id_length) {
        record.id = s.it.get_underlying_buffer().first(id_length);
        s.it += id_length;
    }

    // payload
    if (payload_length) {
        record.payload = s.it.get_underlying_buffer().first(payload_length);
        s.it += payload_length;
    }

    s.it += payload_length;

    report_record_parsed(record);

    return true;
}

uint8_t MessageParser::compute_lengths_size(uint8_t header)
{
    return 1 /* type_length size */ +
           ((header & Header::short_record_bit) ? 1 : 4) /* payload length */ +
           ((header & Header::id_length_bit) ? 1 : 0);
}

uint8_t MessageParser::extract_type_length(parsing_state_t &s)
{
    return *s.it++;
}

uint32_t MessageParser::extract_payload_length(parsing_state_t &s, uint8_t header)
{
    uint32_t payload_length = 0;
    if (header & Header::short_record_bit) {
        payload_length = *s.it++;
    } else {
        s.it.read_be(
            reinterpret_cast<uint8_t *>(&payload_length),
            sizeof(payload_length)
        );
    }
    return payload_length;
}

uint8_t MessageParser::extract_id_length(parsing_state_t &s, uint8_t header)
{
    return (header & Header::id_length_bit) ? *s.it++ : 0;
}

void MessageParser::report_parsing_started()
{
    if (_delegate) {
        _delegate->on_parsing_started();
    }
}

void MessageParser::report_record_parsed(const Record &record)
{
    if (_delegate) {
        _delegate->on_record_parsed(record);
    }
}

void MessageParser::report_parsing_terminated()
{
    if (_delegate) {
        _delegate->on_parsing_terminated();
    }
}

void MessageParser::report_parsing_error(error_t error, parsing_state_t &parsing_state)
{
    parsing_state.error = true;
    if (_delegate) {
        _delegate->on_parsing_error(error);
    }
}

} // namespace ndef
} // namespace nfc
} // namespace mbed

