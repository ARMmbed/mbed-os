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

#ifndef CORDIO_PAL_ATT_CLIENT_
#define CORDIO_PAL_ATT_CLIENT_

#include "CordioGattServer.h"
#include "ble/pal/AttClient.h"
#include "ble/pal/AttClientToGattClientAdapter.h"
#include "ble/pal/SimpleAttServerMessage.h"
#include "att_api.h"
#include "att_defs.h"
#include "ble/pal/PalGap.h"
#include "CordioPalGap.h"
#include "PalGattClient.h"
#include "CordioBLE.h"

namespace ble {
namespace pal {
namespace vendor {
namespace cordio {

class CordioAttClient : public ::ble::pal::AttClient<CordioAttClient> {

public:
    CordioAttClient() : ::ble::pal::AttClient<CordioAttClient>(), _local_sign_counter(0) { }

    ~CordioAttClient() { }

    /**
     * @see ble::pal::AttClient::exchange_mtu_request
     */
    ble_error_t exchange_mtu_request_(connection_handle_t connection)
    {
        AttcMtuReq(connection, pAttCfg->mtu);
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::GattClient::get_mtu_size
     */
    ble_error_t get_mtu_size_(
        connection_handle_t connection_handle,
        uint16_t& mtu_size
    ) {
        mtu_size = AttGetMtu(connection_handle);
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::AttClient::find_information_request
     */
    ble_error_t find_information_request_(
        connection_handle_t connection_handle,
        attribute_handle_range_t discovery_range
    ) {
        AttcFindInfoReq(
            connection_handle,
            discovery_range.begin,
            discovery_range.end,
            false
        );
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::AttClient::find_by_type_value_request
     */
    ble_error_t find_by_type_value_request_(
        connection_handle_t connection_handle,
        attribute_handle_range_t discovery_range,
        uint16_t type,
        const Span<const uint8_t>& value
    ) {
        AttcFindByTypeValueReq(
            connection_handle,
            discovery_range.begin,
            discovery_range.end,
            type,
            value.size(),
            const_cast<uint8_t*>(value.data()),
            false
        );
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::AttClient::read_by_type_request
     */
    ble_error_t read_by_type_request_(
        connection_handle_t connection_handle,
        attribute_handle_range_t read_range,
        const UUID& type
    ) {
        AttcReadByTypeReq(
            connection_handle,
            read_range.begin,
            read_range.end,
            type.getLen(),
            const_cast<uint8_t*>(type.getBaseUUID()),
            false
        );
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::AttClient::read_request
     */
    ble_error_t read_request_(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle
    ) {
        AttcReadReq(connection_handle, attribute_handle);
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::AttClient::read_blob_request
     */
    ble_error_t read_blob_request_(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        uint16_t offset
    ) {
        AttcReadLongReq(
            connection_handle,
            attribute_handle,
            offset,
            false
        );
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::AttClient::read_multiple_request
     */
    ble_error_t read_multiple_request_(
        connection_handle_t connection_handle,
        const Span<const attribute_handle_t>& attribute_handles
    ) {
        AttcReadMultipleReq(
            connection_handle,
            attribute_handles.size(),
            const_cast<uint16_t*>(attribute_handles.data())
        );
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::AttClient::read_by_group_type_request
     */
    ble_error_t read_by_group_type_request_(
        connection_handle_t connection_handle,
        attribute_handle_range_t read_range,
        const UUID& group_type
    ) {
        AttcReadByGroupTypeReq(
            connection_handle,
            read_range.begin,
            read_range.end,
            group_type.getLen(),
            const_cast<uint8_t*>(group_type.getBaseUUID()),
            false
        );
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::AttClient::write_request
     */
    ble_error_t write_request_(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        const Span<const uint8_t>& value
    ) {
        AttcWriteReq(
            connection_handle,
            attribute_handle,
            value.size(),
            const_cast<uint8_t*>(value.data())
        );
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::AttClient::write_command
     */
    ble_error_t write_command_(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        const Span<const uint8_t>& value
    ) {
        AttcWriteCmd(
            connection_handle,
            attribute_handle,
            value.size(),
            const_cast<uint8_t*>(value.data())
        );
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::AttClient::signed_write_command
     */
    ble_error_t signed_write_command_(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        const Span<const uint8_t>& value
    ) {
        AttcSignedWriteCmd(
            connection_handle,
            attribute_handle,
            _local_sign_counter,
            value.size(),
            const_cast<uint8_t*>(value.data())
        );
        _local_sign_counter++;
        return BLE_ERROR_NONE;
    }

    /**
     * Initialises the counter used to sign messages. Counter will be incremented every
     * time a message is signed.
     *
     * @param sign_counter initialise the signing counter to this value
     */
    void set_sign_counter_(
        sign_count_t sign_counter
    ) {
        _local_sign_counter = sign_counter;
    }

    /**
     * @see ble::pal::AttClient::prepare_write_request
     */
    ble_error_t prepare_write_request_(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        uint16_t offset,
        const Span<const uint8_t>& value
    ) {
        AttcPrepareWriteReq(
            connection_handle,
            attribute_handle,
            offset,
            value.size(),
            const_cast<uint8_t*>(value.data()),
            false,
            false
        );
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::AttClient::execute_write_request
     */
    ble_error_t execute_write_request_(
        connection_handle_t connection_handle,
        bool execute
    ) {
        AttcExecuteWriteReq(
            connection_handle,
            execute
        );
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::AttClient::initialize
     */
    ble_error_t initialize_()
    {
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::AttClient::terminate
     */
    ble_error_t terminate_()
    {
        return BLE_ERROR_NONE;
    }

    // singleton of the ARM Cordio client
    static CordioAttClient& get_client()
    {
        static CordioAttClient _client;
        return _client;
    }

private:
    // convert an array of byte to an uint16_t
    static uint16_t to_uint16_t(const uint8_t* array)
    {
        uint16_t result;
        memcpy(&result, array, sizeof(result));
        return result;
    }

    /**
     * Type of an event handler.
     * @param The event to handle
     * @return true if the event has been handled and false otherwise.
     */
    typedef bool (*event_handler_t)(const attEvt_t*);

public:
    /**
     * Callback which handle attEvt_t and forward them to on_server_event.
     */
    static void att_client_handler(const attEvt_t* event);

private:
    /**
     * Event handler generator.
     * @tparam Description of an event converter. It contains two static member
     * function:
     *   - bool can_convert(const attEvt_t* event): return true if the event can
     *   be converted by the converter
     *   - <undefined> convert(const attEvt_t* event): return the
     *   AttServerMessage converted from event.
     * @param event
     * @return
     */
    template<typename T>
    static bool event_handler(const attEvt_t* event)
    {
        if (T::can_convert(event)) {
            generated_handler(event, T::convert);
            return true;
        }
        return false;
    }

    static bool timeout_event_handler(const attEvt_t* event)
    {
        if(event->hdr.status != ATT_ERR_TIMEOUT) {
            return false;
        }

        get_client().on_transaction_timeout(event->hdr.param);
        return true;
    }

    template<typename ResultType>
    static void generated_handler(
        const attEvt_t* event, ResultType (*convert)(const attEvt_t*)
    ) {
        get_client().on_server_event(
            event->hdr.param,
            convert(event)
        );
    }

    /**
     * Traits defining can_convert for non ErrorResponse events.
     */
    template<uint8_t RequestID>
    struct ResponseConverter {
        static bool can_convert(const attEvt_t* event)
        {
            if(event->hdr.status == ATT_SUCCESS && event->hdr.event == RequestID) {
                return true;
            }
            return false;
        }
    };

    /**
     * Converter for an AttErrorResponse.
     */
    struct ErrorResponseConverter {
        static bool can_convert(const attEvt_t* event)
        {
            if(event->hdr.status != ATT_SUCCESS) {
                return true;
            }
            return false;
        }

        static AttErrorResponse convert(const attEvt_t* event)
        {
            return AttErrorResponse(
                static_cast<AttributeOpcode::Code>(event->hdr.event * 2),
                event->handle,
                event->hdr.status
            );
        }
    };

    /**
     * Converter for a SimpleAttFindInformationResponse.
     */
    struct FindInformationResponseConverter : ResponseConverter<ATTC_FIND_INFO_RSP> {
        static SimpleAttFindInformationResponse convert(const attEvt_t* event)
        {
            return SimpleAttFindInformationResponse(
                static_cast<SimpleAttFindInformationResponse::Format>(event->pValue[0]),
                make_const_Span(
                    event->pValue + 1,
                    event->valueLen - 1
                )
            );
        }
    };

    /**
     * Converter for a SimpleAttFindByTypeValueResponse.
     */
    struct FindByTypeValueResponseConverter : ResponseConverter<ATTC_FIND_BY_TYPE_VALUE_RSP> {
        static SimpleAttFindByTypeValueResponse convert(const attEvt_t* event)
        {
            return SimpleAttFindByTypeValueResponse(
                make_const_Span(
                    event->pValue,
                    event->valueLen
                )
            );
        }
    };

    /**
     * Converter for a SimpleAttReadByTypeResponse.
     */
    struct ReadByTypeResponseConverter : ResponseConverter<ATTC_READ_BY_TYPE_RSP> {
        static SimpleAttReadByTypeResponse convert(const attEvt_t* event)
        {
            return SimpleAttReadByTypeResponse(
                event->pValue[0],
                make_const_Span(
                    event->pValue + 1,
                    event->valueLen - 1
                )
            );
        }
    };

    /**
     * Converter for a AttReadResponse.
     */
    struct ReadResponseConverter : ResponseConverter<ATTC_READ_RSP> {
        static AttReadResponse convert(const attEvt_t* event)
        {
            return AttReadResponse(
                make_const_Span(
                    event->pValue,
                    event->valueLen
                )
            );
        }
    };

    /**
     * Converter for a AttReadBlobResponse.
     */
    struct ReadBlobResponseConverter : ResponseConverter<ATTC_READ_LONG_RSP> {
        static AttReadBlobResponse convert(const attEvt_t* event)
        {
            return AttReadBlobResponse(
                make_const_Span(
                    event->pValue,
                    event->valueLen
                )
            );
        }
    };

    /**
     * Converter for a AttReadMultipleResponse.
     */
    struct ReadMultipleResponseConverter : ResponseConverter<ATTC_READ_MULTIPLE_RSP> {
        static AttReadMultipleResponse convert(const attEvt_t* event)
        {
            return AttReadMultipleResponse(
                make_const_Span(
                    event->pValue,
                    event->valueLen
                )
            );
        }
    };

    /**
     * Converter for a SimpleAttReadByGroupTypeResponse.
     */
    struct ReadBygroupTypeResponseConverter : ResponseConverter<ATTC_READ_BY_GROUP_TYPE_RSP> {
        static SimpleAttReadByGroupTypeResponse convert(const attEvt_t* event)
        {
            return SimpleAttReadByGroupTypeResponse(
                event->pValue[0],
                make_const_Span(
                    event->pValue + 1,
                    event->valueLen - 1
                )
            );
        }
    };

    /**
     * Converter for a AttWriteResponse.
     */
    struct WriteResponseConverter : ResponseConverter<ATTC_WRITE_RSP> {
        static AttWriteResponse convert(const attEvt_t* event)
        {
            return AttWriteResponse();
        }
    };

    /**
     * Converter for a AttPrepareWriteResponse.
     */
    struct PrepareWriteResponseConverter : ResponseConverter<ATTC_PREPARE_WRITE_RSP> {
        static AttPrepareWriteResponse convert(const attEvt_t* event)
        {
            // WARNING: Not sure if correct, the stack erase the length parameter
            return AttPrepareWriteResponse(
                event->handle,
                to_uint16_t(event->pValue + 2),
                // FIXME: the stack set the lenght to 0, the data won't be seen ...
                make_const_Span(
                    event->pValue + 4,
                    event->valueLen
                )
            );
        }
    };

    /**
     * Converter for a AttExecuteWriteResponse.
     */
    struct ExecuteWriteResponseConverter : ResponseConverter<ATTC_EXECUTE_WRITE_RSP> {
        static AttExecuteWriteResponse convert(const attEvt_t* event)
        {
            return AttExecuteWriteResponse();
        }
    };

    /**
     * Converter for a AttHandleValueNotification.
     */
    struct HandleValueNotificationConverter : ResponseConverter<ATTC_HANDLE_VALUE_NTF> {
        static AttHandleValueNotification convert(const attEvt_t* event)
        {
            return AttHandleValueNotification(
                event->handle,
                make_const_Span(
                    event->pValue,
                    event->valueLen
                )
            );
        }
    };

    /**
     * Converter for a AttHandleValueIndication.
     */
    struct HandleValueIndicationConverter : ResponseConverter<ATTC_HANDLE_VALUE_IND> {
        static AttHandleValueIndication convert(const attEvt_t* event)
        {
            return AttHandleValueIndication(
                event->handle,
                make_const_Span(
                    event->pValue,
                    event->valueLen
                )
            );
        }
    };
private:
    sign_count_t _local_sign_counter;
};

template<class EH>
struct CordioGattClient : pal::AttClientToGattClientAdapter<CordioAttClient, EH> {
    CordioGattClient(CordioAttClient& att_client) :
        pal::AttClientToGattClientAdapter<CordioAttClient, EH>(att_client)
    { }

    typedef EH EventHandler;
};

} // cordio
} // vendor
} // pal
} // ble

#endif /* CORDIO_PAL_ATT_CLIENT_ */
