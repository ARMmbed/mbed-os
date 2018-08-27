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

#ifndef NFC_NDEF_RECORDPARSER_H_
#define NFC_NDEF_RECORDPARSER_H_

#include <stddef.h>

#include "nfc/ndef/Record.h"

namespace mbed {
namespace nfc {
namespace ndef {

/**
 * @addtogroup nfc
 * @{
 */

/**
 * Parse a record.
 */
struct RecordParser {
    /**
     * Construct a record parser.
     */
    RecordParser() : _next_parser(NULL) { }

    /**
     * Parse the record in input.
     * @param record The NDEF record to parse.
     * @return true if decoding has succeeded and false otherwise.
     */
    virtual bool parse(const Record &record) = 0;

protected:
    /**
     * Protected non virtual destructor.
     * RecordParser subclasses are not meant to be destroyed as RecordParser's.
     */
    ~RecordParser() { }

private:
    friend class RecordParserChain;
    RecordParser *_next_parser;
};


/**
 * GenericRecordParser.
 *
 * @tparam ParserImplementation the implementation type of the parser.
 * It must provides A decoding function named do_parse that accept a const
 * reference to a record and a reference to the type parsed and return a boolean
 * status that indicates the result of the parsing operation.
 *
 * @tparam ParsingResult Type produced by the parsing operation when successful.
 */
template<typename ParserImplementation, typename ParsingResult>
struct GenericRecordParser : public RecordParser {

    /**
     * Handle that receives parsed values.
     */
    struct Delegate {
        /**
         * Called when a record has been parsed and converted into a value_type.
         *
         * @param object_parsed The record in its parsed form.
         * @param id The RecordId associated with the object parsed.
         */
        virtual void on_record_parsed(const ParsingResult &object_parsed, const RecordID &id) = 0;

    protected:
        ~Delegate() { }
    };

    /**
     * Construct a record parser.
     */
    GenericRecordParser() : _delegate(NULL) { }

    /**
     * @see RecordParser::parse
     */
    virtual bool parse(const Record &record)
    {
        ParsingResult parsed_value;
        if (static_cast<ParserImplementation *>(this)->do_parse(record, parsed_value)) {
            if (_delegate) {
                _delegate->on_record_parsed(parsed_value, record.id);
            }
            return true;
        }
        return false;
    }

    /**
     * Set the delegate that processes record parser.
     *
     * @param delegate The delegate to set.
     */
    void set_delegate(Delegate *delegate)
    {
        _delegate = delegate;
    }

protected:
    /**
     * Protected non virtual destructor.
     */
    ~GenericRecordParser() { }

private:
    Delegate *_delegate;
};


/**
 * Record parser chain.
 */
struct RecordParserChain {
    /**
     * Construct a parser chain.
     */
    RecordParserChain() : _parsers(NULL) { }

    /**
     * Destroy a parser chain.
     */
    ~RecordParserChain() { }

    /**
     * Parse a record.
     * @param record The record to parse.
     * @return true if the record has been parsed and false otherwise.
     */
    bool parse(const Record &record);

    /**
     * Add a parser at the end of the parser list.
     * @param parser The parser to add.
     */
    void set_next_parser(RecordParser *parser);

private:
    RecordParser *_parsers;
};

/**
 * @}
 */

} // namespace ndef
} // namespace nfc
} // namespace mbed


#endif /* NFC_NDEF_RECORDPARSER_H_ */
