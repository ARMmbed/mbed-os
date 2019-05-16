/* Copyright (c) 2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SMARTPOSTER_H_
#define SMARTPOSTER_H_

#include "nfc/ndef/common/Text.h"
#include "nfc/ndef/common/URI.h"
#include "nfc/ndef/common/Mime.h"
#include "nfc/ndef/MessageBuilder.h"

/**
 * Smart poster object.
 *
 * A smart poster is one of the basic use case of NFC. It encapsulates a URI to
 * a resource and meta-data of the resource.
 *
 * Meta-data are optional, they can be:
 *   - title: name of the resource
 *   - icon: image/media associated to the resource
 *   - action: Action the peer should execute upon reception of the smart poster
 *   - size: The size of the resource.
 *   - type: Mime type of the resource.
 *
 * @note It obeys to value semantic and can be copied around.
 */
class SmartPoster {
public:
    typedef mbed::nfc::ndef::common::Mime Mime;
    typedef mbed::nfc::ndef::common::Text Text;
    typedef mbed::nfc::ndef::common::URI URI;
    typedef mbed::nfc::ndef::MessageBuilder MessageBuilder;

    /**
     * Type of actions that should be executed upon smart poster reception.
     */
    enum action_t {
        EXECUTE, //!< EXECUTE
        SAVE,   //!< SAVE
        EDIT    //!< EDIT
    };

    /**
     * Construct a smart poster.
     *
     * @param uri The URI to the resource.
     */
    SmartPoster(const URI &uri);

    /**
     * Set the title of the resource.
     *
     * @param text The title of the resource to set.
     */
    void set_title(const Text &text);

    /**
     * Set the icon of the resource.
     *
     * @param icon The icon to set.
     */
    void set_icon(const Mime &icon);

    /**
     * Set the action to trigger upon smart poster reception.
     *
     * @param action The action to do upon reception.
     */
    void set_action(action_t action);

    /**
     * Set the size of the resource.
     *
     * @param size The size of the resource.
     */
    void set_resource_size(uint32_t size);

    /**
     * Set the type of the resource.
     *
     * @param resource_type The type of the resource pointed by the URI.
     */
    void set_resource_type(mbed::Span<const uint8_t> &resource_type);

    /**
     * Append the smart poster as a ndef record.
     *
     * @param ndef_builder The message builder where the record is appended.
     * @param is_last_record Indicates if this message is the last one.
     *
     * @return true if the message has been appended to the builder or false
     * otherwise.
     */
    bool append_record(MessageBuilder &ndef_builder, bool is_last_record) const;

private:
    void append_uri(MessageBuilder &builder) const;
    size_t get_uri_record_size() const;

    void append_title(MessageBuilder &builder) const;
    size_t get_title_record_size() const;

    void append_icon(MessageBuilder &builder) const;
    size_t get_icon_record_size() const;

    void append_action(MessageBuilder &builder) const;
    size_t get_action_record_size() const;

    void append_resource_size(MessageBuilder &builder) const;
    size_t get_resource_size_record_size() const;

    void append_type(MessageBuilder &builder) const;
    size_t get_type_record_size() const;

    URI _uri;
    Text _title;
    Mime _icon;
    action_t _action;
    uint32_t _resource_size;
    Text _type;

    bool _action_set : 1;
    bool _resource_size_set : 1;
};

#endif /* SMARTPOSTER_H_ */
