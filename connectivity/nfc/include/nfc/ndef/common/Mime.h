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

#ifndef NFC_COMMON_MIME_H_
#define NFC_COMMON_MIME_H_

#include <stdint.h>

#include "platform/Span.h"

#include "nfc/ndef/RecordParser.h"
#include "nfc/ndef/MessageBuilder.h"

namespace mbed {
namespace nfc {
namespace ndef {
namespace common {

/**
 * @addtogroup nfc
 * @{
 */

/**
 * Represent a mime object.
 */
class Mime {
public:
    /**
     * Construct an empty Mime object.
     */
    Mime();

    /**
     * Construct a mime object from its type and content
     *
     * @param mime_type The mime type of the object.
     * @param content The content of the object.
     *
     * @note To remove the NULL terminator of the C-string of the mime_type
     * parameter, you can use the utility function span_from_cstr.
     */
    Mime(
        const Span<const uint8_t> &mime_type,
        const Span<const uint8_t> &content
    );

    /**
     * Copy construct a Mime object.
     * @param other The Mime object copied.
     */
    Mime(const Mime &other);

    /**
     * Destroy a Mime object.
     */
    ~Mime();

    /**
     * Copy assign a Mime object.
     *
     * @param other The Mime object to copy.
     *
     * @return a reference to this object
     */
    Mime &operator=(const Mime &other);

    /**
     * Set all attributes of a mime object.
     *
     * @param mime_type Type of the mime object.
     * @param content Content of the mime object.
     *
     * @note To remove the NULL terminator of the C-string of the mime_type
     * parameter, you can use the utility function span_from_cstr.
     */
    void set_mime(
        const Span<const uint8_t> &mime_type,
        const Span<const uint8_t> &content
    );

    /**
     * Return the mime type.
     * @return The mime type.
     */
    Span<const uint8_t> get_mime_type() const;

    /**
     * Return the content of the mime object.
     * @return the content of the mime object.
     */
    Span<const uint8_t> get_mime_content() const;

    /**
     * Append into a message builder
     */
    bool append_as_record(
        MessageBuilder &message_builder,
        bool is_last_record = false
    ) const;

    /**
     * Compute the size of this Mime object in a ndef record.
     *
     * @return The size of the ndef record required to store this object.
     */
    size_t get_record_size() const;

private:
    friend class MimeParser;

    void move_data(
        uint8_t *mime_record,
        size_t mime_type_size,
        size_t mime_content_size
    );

    size_t mime_size() const;

    uint8_t *_mime;
    size_t _type_size;
    size_t _content_size;
};

/**
 * Parse a Mime payload.
 */
class MimeParser : public GenericRecordParser<MimeParser, Mime> {
public:
    bool do_parse(const Record &record, Mime &mime);
};

/**
 * @}
 */

} // namespace common
} // namespace ndef
} // namespace nfc
} // namespace mbed

#endif /* NFC_COMMON_MIME_H_ */
