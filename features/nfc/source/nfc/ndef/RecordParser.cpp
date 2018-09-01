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

#include "nfc/ndef/RecordParser.h"

namespace mbed {
namespace nfc {
namespace ndef {

bool RecordParserChain::parse(const Record &record)
{
    RecordParser *current_parser = _parsers;
    while (current_parser) {
        if (current_parser->parse(record)) {
            return true;
        }
        current_parser = current_parser->_next_parser;
    }
    return false;
}

void RecordParserChain::set_next_parser(RecordParser *parser)
{
    if (!_parsers) {
        _parsers = parser;
    } else {
        RecordParser *current_parser = _parsers;
        while (current_parser->_next_parser) {
            current_parser = current_parser->_next_parser;
        }
        current_parser->_next_parser = parser;
    }
}

} // namespace ndef
} // namespace nfc
} // namespace mbed
