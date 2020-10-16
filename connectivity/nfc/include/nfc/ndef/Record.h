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

#ifndef NFC_NDEF_RECORD_H_
#define NFC_NDEF_RECORD_H_

#include <stdint.h>

#include "platform/Span.h"

namespace mbed {
namespace nfc {
namespace ndef {

/**
 * @addtogroup nfc
 * @{
 */


/**
 * Set of constants of a record header
 */
struct Header {
    static const uint8_t message_begin_bit = (1 << 7);
    static const uint8_t message_end_bit = (1 << 6);
    static const uint8_t chunk_flag_bit = (1 << 5);
    static const uint8_t short_record_bit = (1 << 4);
    static const uint8_t id_length_bit = (1 << 3);
    static const uint8_t tnf_bits = (1 << 0) | (1 << 1) | (1 << 2);
};

/**
 * Encode a record type.
 *
 * A RecordType is composed of a type name format flag and an optional type
 * value.
 */
struct RecordType {
    /**
     * Type name format of a record.
     */
    enum tnf_t {
        /**
         * empty type; value must be empty.
         */
        empty = 0x00,          //!< empty

        /**
         * Type defined by the NFC forum; value must be defined.
         */
        well_known_type = 0x01,//!< well_known_type

        /**
         * Mime type; value must be defined.
         */
        media_type = 0x02,     //!< media_type

        /**
         * Absolute URI; value must be defined.
         */
        absolute_uri = 0x03,   //!< absolute_uri

        /**
         * Type defined by vendors; value must be defined.
         */
        external_type = 0x04,  //!< external_type

        /**
         * Unknown type; value must be empty.
         */
        unknown = 0x05,        //!< unknown

        /**
         * Use for middle and terminating chunk record.
         * value must be empty.
         */
        unchanged = 0x06       //!< unchanged
    };

    /**
     * Construct an unknown type.
     */
    RecordType() : tnf(unknown), value() { }

    /**
     * Construct a type with no value.
     *
     * @note Valid tnf are: empty, unknown and unchanged.
     *
     * @param tnf The type name format of the type.
     */
    RecordType(tnf_t tnf) :
        tnf(tnf), value()
    { }

    /**
     * Construct a RecordType from a type name format and its associated value.
     *
     * @param tnf The type name format of the record type.
     * @param value The value associated with the tnf.
     */
    RecordType(tnf_t tnf, const Span<const uint8_t> &value) :
        tnf(tnf), value(value)
    { }

    /**
     * Type name format of the record type.
     */
    tnf_t tnf;

    /**
     * Value associated with the record type. It can be empty.
     */
    Span<const uint8_t> value;
};

/**
 * Definition of a Record payload.
 *
 * @note A payload can be empty.
 */
typedef Span<const uint8_t> RecordPayload;

/**
 * Definition of a Record IR.
 *
 * @note ID's are optional and therefore it can be empty.
 */
typedef Span<const uint8_t> RecordID;

/**
 * Represent a record.
 */
struct Record {
    /**
     * Construct an empty record.
     */
    Record() : type(), payload(), id(), chunk(false), last_record(false) { }

    /**
     * Construct a record from its type, payload and id.
     *
     * The flags chunk and last_record can be added to indicate if the record
     * is aprt of a chunk or the last one in a message.
     *
     * @param type The type of the record.
     * @param payload The payload of the record.
     * @param id The id associated with the record.
     * @param chunk If true then this record is a chunk of a bigger record.
     * @param last_record If true then this record is the last of the message
     * containing it.
     */
    Record(
        RecordType type,
        const RecordPayload &payload,
        const RecordID &id,
        bool chunk,
        bool last_record
    ) :
        type(type),
        payload(payload),
        id(id),
        chunk(chunk),
        last_record(last_record)
    { }

    /**
     * Type of the record.
     */
    RecordType type;

    /**
     * Value of the payload.
     */
    RecordPayload payload;

    /**
     * ID of the record.
     */
    RecordID id;

    /**
     * If true, this record is a chunked record.
     */
    bool chunk: 1;

    /**
     * If true, this record is the last one of the payload containing it.
     */
    bool last_record: 1;
};


/**
 * @}
 */

} // namespace ndef
} // namespace nfc
} // namespace mbed

#endif /* NFC_NDEF_RECORD_H_ */
