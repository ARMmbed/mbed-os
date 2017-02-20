/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef NXP_NXPATTCLIENT_H_
#define NXP_NXPATTCLIENT_H_

#include <algorithm>
#include "ble/pal/AttClient.h"
#include "att_params.h"
#include "att_interface.h"

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

namespace ble {
namespace pal {
namespace nxp {

/**
 * Implementation of ble::pal::AttClient for NXP targets.
 */
class NXPAttClient: public AttClient {
public:
    NXPAttClient() : AttClient() { }
    virtual ~NXPAttClient() { }

    /**
     * @see ble::pal::AttClient::exchange_mtu_request
     */
    virtual ble_error_t exchange_mtu_request(connection_handle_t connection) {
        // N/A, the MTU is limited to the default value ...
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * @see ble::pal::AttClient::get_max_rx_mtu_supported
     */
    virtual uint16_t get_max_rx_mtu_supported() {
        return gAttMaxMtu_c;
    }

    /**
     * @see ble::pal::AttClient::find_information_request
     */
    virtual ble_error_t find_information_request(
        connection_handle_t connection_handle,
        attribute_handle_range_t discovery_range
    ) {
        attFindInformationRequestParams_t req = {
            discovery_range.begin,
            discovery_range.end
        };

        return _error_converter << AttClient_SendFindInformationRequest(
            connection_handle, &req
        );
    }

    /**
     * @see ble::pal::AttClient::find_by_type_value_request
     */
    virtual ble_error_t find_by_type_value_request(
        connection_handle_t connection_handle,
        attribute_handle_range_t discovery_range,
        uint16_t type,
        const array_view<const uint8_t>& value
    ) {
        if (value.size() > COUNT_OF(((attFindByTypeValueRequestParams_t*) 0)->attributeValue)) {
            return BLE_ERROR_INVALID_PARAM;
        }

        attFindByTypeValueRequestParams_t req = {
            discovery_range.begin,
            discovery_range.end,
            type,
            { 0 },
            value.size()
        };
        memcpy(req.attributeValue, value.data(), value.size());

        return _error_converter << AttClient_SendFindByTypeValueRequest(
            connection_handle, &req
        );
    }

    /**
     * @see ble::pal::AttClient::read_by_type_request
     */
    virtual ble_error_t read_by_type_request(
        connection_handle_t connection_handle,
        attribute_handle_range_t read_range,
        const UUID& type
    ) {
        attReadByTypeRequestParams_t req = {
            read_range.begin,
            read_range.end
        };

        if (type.getLen() == 2) {
            req.format = gAttUuid16BitFormat_c;
            req.attributeType.uuid16 = type.getShortUUID();
        } else {
            req.format = gAttUuid128BitFormat_c;
            memcpy(req.attributeType.uuid128, type.getBaseUUID(), type.getLen());
        }

        return _error_converter << AttClient_SendReadByTypeRequest(
            connection_handle, &req
        );
    }

    /**
     * @see ble::pal::AttClient::read_request
     */
    virtual ble_error_t read_request(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle
    ) {
        attReadRequestParams_t req = {
            attribute_handle
        };

        return _error_converter << AttClient_SendReadRequest(
            connection_handle, &req
        );
    }

    /**
     * @see ble::pal::AttClient::read_blob_request
     */
    virtual ble_error_t read_blob_request(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        uint16_t offset
    ) {
        attReadBlobRequestParams_t req = {
            attribute_handle,
            offset
        };

        return _error_converter << AttClient_SendReadBlobRequest(
            connection_handle, &req
        );
    }

    /**
     * @see ble::pal::AttClient::read_multiple_request
     */
    virtual ble_error_t read_multiple_request(
        connection_handle_t connection_handle,
        const array_view<const attribute_handle_t>& attribute_handles
    ) {
        if (attribute_handles.size() > COUNT_OF(((attReadMultipleRequestParams_tag*)0)->listOfHandles)) {
            return BLE_ERROR_INVALID_PARAM;
        }

        attReadMultipleRequestParams_t req;
        std::copy(attribute_handles.data(), attribute_handles.data() + 6, req.listOfHandles);
        req.handleCount = attribute_handles.size();

        return _error_converter << AttClient_SendReadMultipleRequest(
            connection_handle, &req
        );
    }

    /**
     * @see ble::pal::AttClient::read_by_group_type_request
     */
    virtual ble_error_t read_by_group_type_request(
        connection_handle_t connection_handle,
        attribute_handle_range_t read_range,
        const UUID& group_type
    ) {
        attReadByGroupTypeRequestParams_t req = {
            read_range.begin,
            read_range.end
        };

        if (group_type.getLen() == 2) {
            req.format = gAttUuid16BitFormat_c;
            req.attributeType.uuid16 = group_type.getShortUUID();
        } else {
            req.format = gAttUuid128BitFormat_c;
            memcpy(req.attributeType.uuid128, group_type.getBaseUUID(), group_type.getLen());
        }

        return _error_converter << AttClient_SendReadByGroupTypeRequest(
            connection_handle, &req
        );
    }

    /**
     * @see ble::pal::AttClient::write_request
     */
    virtual ble_error_t write_request(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        const array_view<const uint8_t>& value
    ) {
        if (value.size() > COUNT_OF(((attWriteRequestAndCommandParams_t*)0)->attributeValue)) {
            return BLE_ERROR_INVALID_PARAM;
        }

        attWriteRequestAndCommandParams_t req = {
            attribute_handle
        };
        memcpy(req.attributeValue, value.data(), value.size());
        req.attributeLength = value.size();

        return _error_converter << AttClient_SendWriteRequest(
            connection_handle, &req
        );
    }

    /**
     * @see ble::pal::AttClient::write_command
     */
    virtual ble_error_t write_command(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        const array_view<const uint8_t>& value
    ) {
        if (value.size() > COUNT_OF(((attWriteRequestAndCommandParams_t*)0)->attributeValue)) {
            return BLE_ERROR_INVALID_PARAM;
        }

        attWriteRequestAndCommandParams_t req = {
            attribute_handle
        };
        memcpy(req.attributeValue, value.data(), value.size());
        req.attributeLength = value.size();

        return _error_converter << AttClient_SendWriteCommand(
            connection_handle, &req
        );
    }

    /**
     * @see ble::pal::AttClient::signed_write_command
     */
    virtual ble_error_t signed_write_command(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        const array_view<const uint8_t>& value
    ) {
        if (value.size() > COUNT_OF(((attSignedWriteCommandParams_tag*)0)->attributeValue)) {
            return BLE_ERROR_INVALID_PARAM;
        }

        attSignedWriteCommandParams_t req = {
                attribute_handle
        };
        memcpy(req.attributeValue, value.data(), value.size());
        req.attributeLength = value.size();

        // FIXME: compute the authentication signature
//    	memcpy(req.authenticationSignature, authentication_signature, sizeof(authentication_signature));

        return _error_converter << AttClient_SendSignedWriteCommand(
            connection_handle, &req
        );
    }

    /**
     * @see ble::pal::AttClient::prepare_write_request
     */
    virtual ble_error_t prepare_write_request(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        uint16_t offset,
        const array_view<const uint8_t>& value
    ) {
        if (value.size() > COUNT_OF(((attPrepareWriteRequestResponseParams_t*)0)->attributeValue)) {
            return BLE_ERROR_INVALID_PARAM;
        }

        attPrepareWriteRequestResponseParams_t req = {
            attribute_handle,
            offset
        };
        memcpy(req.attributeValue, value.data(), value.size());
        req.attributeLength = value.size();

        return _error_converter << AttClient_SendPrepareWriteRequest(
            connection_handle, &req
        );
    }

    /**
     * @see ble::pal::AttClient::execute_write_request
     */
    virtual ble_error_t execute_write_request(
        connection_handle_t connection_handle,
        bool execute
    ) {
        attExecuteWriteRequestParams_t req = { execute ? gAttExecuteWriteRequestImmediately : gAttExecuteWriteRequestCancel };

        return _error_converter << AttClient_SendExecuteWriteRequest(
            connection_handle, &req
        );
    }

    /**
     * @see ble::pal::AttClient::initialize
     */
    virtual ble_error_t initialize() {
        // TODO
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::AttClient::terminate
     */
    virtual ble_error_t terminate() {
        // TODO
        return BLE_ERROR_NONE;
    }

private:

    /**
     * Placeholder used to convert bleResult_t into ble_error_t.
     * The operator << of this instance is overloaded and handle the conversion.
     * It become possible to write code like:
     * @code
     * ble_error_t some_function() {
     *   return _error_converter << AttClient_Something();
     * }
     * @endcode
     */
    static struct {
        ble_error_t operator<<(bleResult_t result) {
            switch (result) {
                case gBleSuccess_c: return BLE_ERROR_NONE;
                // TODO map other type of errors
                default: return BLE_ERROR_UNSPECIFIED;
            }
        }
    } _error_converter;
};

} /* namespace nxp */
} /* namespace pal */
} /* namespace ble */

#endif /* NXP_NXPATTCLIENT_H_ */
