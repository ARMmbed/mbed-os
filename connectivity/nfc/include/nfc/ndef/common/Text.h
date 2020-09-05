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

#ifndef NFC_COMMON_TEXT_H_
#define NFC_COMMON_TEXT_H_

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
 * Represent the well known type text.
 */
class Text {
public:
    /**
     * Encoding of the text.
     */
    enum encoding_t {
        UTF8 = 0,//!< UTF8
        UTF16 = 1//!< UTF16
    };

    /**
     * Construct an empty text element.
     */
    Text();

    /**
     * Construct a text element from a data buffer and an encoding.
     *
     * @param text_encoding The encoding of the text.
     * @param language_code The string of the language code.
     * @param text The text buffer.
     *
     * @note To remove the NULL terminator of the C-string of the language_code
     * and text parameters, you can use the utility function span_from_cstr.
     */
    Text(
        encoding_t text_encoding,
        const Span<const uint8_t> &language_code,
        const Span<const uint8_t> &text
    );

    /**
     * Copy construct a text element.
     * @param to_copy
     */
    Text(const Text &to_copy);

    /**
     * Destroy a text element.
     */
    ~Text();

    /**
     * Copy assignment of another text element.
     * @param to_copy The Text instance to copy
     * @return a reference to this object.
     */
    Text &operator=(const Text &to_copy);

    /**
     * Copy a text from an external buffer.
     *
     * @param text_encoding The encoding of the text.
     * @param language_code The language code of the text.
     * @param text The text to copy.
     *
     * @note To remove the NULL terminator of the C-string of the language_code
     * and text parameters, you can use the utility function span_from_cstr.
     */
    void set_text(
        encoding_t text_encoding,
        const Span<const uint8_t> &language_code,
        const Span<const uint8_t> &text
    );

    /**
     * Get the encoding of the text.
     * @return The encoding of the text.
     */
    encoding_t get_encoding() const;

    /**
     * Return the language code.
     * @return The language code.
     */
    Span<const uint8_t> get_language_code() const;

    /**
     * Return the text contained in this object.
     * @return The text contained in this object.
     */
    Span<const uint8_t> get_text() const;

    /**
     * Append into a message builder
     */
    bool append_as_record(
        MessageBuilder &message_builder,
        bool is_last_record = false
    ) const;

    /**
     * Compute the size of this object in a ndef record.
     *
     * @return The size of the ndef record required to store this object.
     */
    size_t get_record_size() const;

private:
    friend class TextParser;

    void move_data(uint8_t *text, size_t size);

    uint8_t *_text_record;
    size_t _text_record_size;
};

/**
 * Parse a Text.
 */
class TextParser : public GenericRecordParser<TextParser, Text> {
public:
    virtual bool do_parse(const Record &record, Text &text);
};

/**
 * @}
 */

} // namespace common
}  // namespace ndef
} // namespace nfc
} // namespace mbed


#endif /* NFC_COMMON_TEXT_H_ */
