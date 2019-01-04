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


#include "SmartPoster.h"

#include "nfc/ndef/common/Text.h"
#include "nfc/ndef/common/URI.h"
#include "nfc/ndef/common/Mime.h"
#include "nfc/ndef/MessageBuilder.h"
#include "nfc/ndef/common/util.h"

using mbed::Span;

using mbed::nfc::ndef::MessageBuilder;
using mbed::nfc::ndef::RecordType;
using mbed::nfc::ndef::Record;
using mbed::nfc::ndef::RecordID;
using mbed::nfc::ndef::RecordPayload;
using mbed::nfc::ndef::common::span_from_cstr;
using mbed::nfc::ndef::common::Mime;
using mbed::nfc::ndef::common::Text;
using mbed::nfc::ndef::common::URI;

// todo: this class probably needs to be in the nfc module itself

namespace {
static RecordType smart_poster_record_type()
{
    return RecordType(RecordType::well_known_type, span_from_cstr("Sp"));
}

static RecordType action_record_type()
{
    return RecordType(RecordType::well_known_type, span_from_cstr("act"));
}

static RecordType size_record_type()
{
    return RecordType(RecordType::well_known_type, span_from_cstr("s"));
}

static RecordType type_record_type()
{
    return RecordType(RecordType::well_known_type, span_from_cstr("T"));
}

static size_t compute_record_size(const RecordType &type,
                                  const RecordPayload &payload)
{
    return MessageBuilder::compute_record_size(
               Record(type, payload, RecordID(), false, false));
}

} // end of anonymous namespace

SmartPoster::SmartPoster(const URI &uri) :
    _uri(uri), _action(), _resource_size(0), _action_set(false), _resource_size_set(
        false)
{
}

void SmartPoster::set_title(const Text &text)
{
    _title = text;
}

void SmartPoster::set_icon(const Mime &icon)
{
    _icon = icon;
}

void SmartPoster::set_action(action_t action)
{
    _action = action;
    _action_set = true;
}

void SmartPoster::set_resource_size(uint32_t size)
{
    _resource_size = size;
    _resource_size_set = true;
}

void SmartPoster::set_resource_type(Span<const uint8_t> &type)
{
    _type.set_text(Text::UTF8, Span<const uint8_t>(), type);
}

bool SmartPoster::append_record(MessageBuilder &ndef_builder,
                                bool is_last_record) const
{
    if (_uri.get_uri_field().empty()) {
        return false;
    }

    struct PayloadBuilder: MessageBuilder::PayloadBuilder {
        PayloadBuilder(const SmartPoster &sp) :
            sp(sp)
        {
        }

        virtual size_t size() const
        {
            return sp.get_uri_record_size() + sp.get_title_record_size()
                   + sp.get_icon_record_size() + sp.get_action_record_size()
                   + sp.get_resource_size_record_size()
                   + sp.get_type_record_size();
        }

        virtual void build(const Span<uint8_t> &buffer) const
        {
            MessageBuilder smart_poster_builder(buffer);
            sp.append_title(smart_poster_builder);
            sp.append_icon(smart_poster_builder);
            sp.append_resource_size(smart_poster_builder);
            sp.append_type(smart_poster_builder);
            sp.append_action(smart_poster_builder);
            sp.append_uri(smart_poster_builder);
        }

        const SmartPoster &sp;
    };

    bool result = ndef_builder.append_record(smart_poster_record_type(),
                                             PayloadBuilder(*this), is_last_record);
    return result;
}

void SmartPoster::append_uri(MessageBuilder &builder) const
{
    _uri.append_as_record(builder, true);
}

size_t SmartPoster::get_uri_record_size() const
{
    return _uri.get_record_size();
}

void SmartPoster::append_title(MessageBuilder &builder) const
{
    if (_title.get_text().empty()) {
        return;
    }
    _title.append_as_record(builder);
}

size_t SmartPoster::get_title_record_size() const
{
    if (_title.get_text().empty()) {
        return 0;
    }

    return _title.get_record_size();
}

void SmartPoster::append_icon(MessageBuilder &builder) const
{
    if (_icon.get_mime_content().empty()) {
        return;
    }
    _icon.append_as_record(builder);
}

size_t SmartPoster::get_icon_record_size() const
{
    if (_icon.get_mime_content().empty()) {
        return 0;
    }

    return _icon.get_record_size();
}

void SmartPoster::append_action(MessageBuilder &builder) const
{
    if (!_action_set) {
        return;
    }

    const uint8_t action_value[1] = { _action };
    builder.append_record(action_record_type(), action_value);
}

size_t SmartPoster::get_action_record_size() const
{
    if (!_action_set) {
        return 0;
    }

    const uint8_t action_value[1] = { _action };

    return compute_record_size(action_record_type(), action_value);
}

void SmartPoster::append_resource_size(MessageBuilder &builder) const
{
    if (!_resource_size_set) {
        return;
    }

    uint8_t value[4];
    std::reverse_copy(&_resource_size, &_resource_size + 4, value);

    builder.append_record(size_record_type(), value);
}

size_t SmartPoster::get_resource_size_record_size() const
{
    if (!_resource_size_set) {
        return 0;
    }

    uint8_t value[4];

    return compute_record_size(size_record_type(), value);
}

void SmartPoster::append_type(MessageBuilder &builder) const
{
    if (_type.get_text().empty()) {
        return;
    }

    builder.append_record(type_record_type(), _type.get_text());
}

size_t SmartPoster::get_type_record_size() const
{
    if (_type.get_text().empty()) {
        return 0;
    }

    return compute_record_size(type_record_type(), _type.get_text());
}

