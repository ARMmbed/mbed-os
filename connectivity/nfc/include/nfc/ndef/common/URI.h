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

#ifndef NFC_COMMON_URI_H_
#define NFC_COMMON_URI_H_

#include <stdint.h>
#include <string.h>

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
 * Model the well known type URI.
 */
class URI {
public:
    /**
     * Identifier codes
     */
    enum uri_identifier_code_t {
        NA = 0x00, /// Not applicable
        HTTP_WWW = 0x01, /// http://www.
        HTTPS_WWW = 0x02, /// https://www.
        HTTP = 0x03, /// http://
        HTTPS = 0x04, /// https://
        TEL = 0x05, /// tel:
        MAILTO = 0x06, /// mailto:
        FTP_ANONYMOUS = 0x07, /// ftp://anonymous:anonymous@
        FTP_FTP = 0x08, /// ftp://ftp.
        FTPS = 0x09, /// ftps://
        SFTP = 0x0A, /// sftp://
        SMB = 0x0B, /// smb://
        NFS = 0x0C, /// nfs://
        FTP = 0x0D, /// ftp://
        DAV = 0x0E, /// dav://
        NEWS = 0x0F, /// news:
        TELNET = 0x10, /// telnet://
        IMAP = 0x11, /// imap:
        RSTP = 0x12, /// rstp://
        URN = 0x13, /// urn:
        POP = 0x14, /// pop:
        SIP = 0x15, /// sip:
        SIPS = 0x16, /// sips:
        TFTP = 0x17, /// tftp:
        BTSPP = 0x18, /// btspp://
        BTL2CAP = 0x19, /// btl2cap://
        BTGOEP = 0x1A, /// btgoep://
        TCPOBEX = 0x1B, /// tcpobex://
        IRDAOBEX = 0x1C, /// irdaobex://
        FILE = 0x1D, /// file://
        URN_EPC_ID = 0x1E, /// urn:epc:id:
        URN_EPC_TAG = 0x1F, /// urn:epc:tag:
        URN_EPC_PAT = 0x20, /// urn:epc:pat:
        URN_EPC_RAW = 0x21, /// urn:epc:raw:
        URN_EPC = 0x22, /// urn:epc:
        URN_NFC = 0x23, /// urn:nfc:
    };

    /**
     * Construct an empty URI object.
     */
    URI();

    /**
     * Construct a URI from an id and a uri field.
     *
     * @param id The code of the URI prefix.
     * @param uri_field The URI itself.
     *
     * @note To remove the NULL terminator of the C-string of the uri_field
     * parameter, you can use the utility function span_from_cstr.
     */
    URI(uri_identifier_code_t id, const Span<const uint8_t> &uri_field);

    /**
     * Construct a URI from another URI.
     * @param to_copy The uri copied.
     */
    URI(const URI &to_copy);

    /**
     * Destroy a URI object.
     */
    ~URI();

    /**
     * Replace the content by the one of an existing URI.
     * @param to_copy The URI to copy.
     * @return a reference to this object
     */
    URI &operator=(const URI &to_copy);

    /**
     * Replace the value of the URI.
     *
     * @param id The ID of the URI
     * @param uri_field A buffer containing the value of the URI field.
     *
     * @note To remove the NULL terminator of the C-string of the uri_field
     * parameter, you can use the utility function span_from_cstr.
     */
    void set_uri(
        uri_identifier_code_t id,
        const Span<const uint8_t> &uri_field
    );

    /**
     * Return the id of the uri.
     * @return The id of the uri.
     */
    uri_identifier_code_t get_id() const;

    /**
     * Return the current value of the uri field.
     * @return The value of the uri field.
     */
    Span<const uint8_t> get_uri_field() const;

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

    /**
     * Equal operator between two URIs
     * @param lhs The URI on the left hand side
     * @param rhs The URI on the right hand side
     * @return true if lhs equals rhs or false.
     */
    friend bool operator==(const URI &lhs, const URI &rhs)
    {
        if (lhs._uri_size != rhs._uri_size) {
            return false;
        }

        return memcmp(lhs._uri, rhs._uri, lhs._uri_size) == 0;
    }

    friend bool operator!=(const URI &lhs, const URI &rhs)
    {
        return !(lhs == rhs);
    }

private:
    friend class URIParser;

    void move_data(uint8_t *text, size_t size);

    uint8_t *_uri;
    size_t _uri_size;
};

/**
 * Parser of a URI.
 */
class URIParser : public GenericRecordParser<URIParser, URI> {
public:
    bool do_parse(const Record &record, URI &uri);
};

/**
 * @}
 */

} // namespace common
} // namespace ndef
} // namespace nfc
} // namespace mbed

#endif /* NFC_COMMON_URI_H_ */
