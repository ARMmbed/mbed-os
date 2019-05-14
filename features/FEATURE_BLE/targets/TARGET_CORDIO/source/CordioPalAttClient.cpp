/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#include "CordioPalAttClient.h"
#include "CordioBLE.h"

namespace ble {
namespace pal {
namespace vendor {
namespace cordio {

void CordioAttClient::att_client_handler(const attEvt_t* event)
{
#if BLE_FEATURE_GATT_CLIENT
    if (event->hdr.status == ATT_SUCCESS && event->hdr.event == ATT_MTU_UPDATE_IND) {
        ble::vendor::cordio::BLE& ble = ble::vendor::cordio::BLE::deviceInstance();
        impl::PalGattClientImpl::EventHandler *handler = ble.getPalGattClient().get_event_handler();
        if (handler) {
            handler->on_att_mtu_change(event->hdr.param, event->mtu);
        }
    } else if (event->hdr.event == ATTC_WRITE_CMD_RSP) {
        ble::vendor::cordio::BLE& ble = ble::vendor::cordio::BLE::deviceInstance();
        impl::PalGattClientImpl::EventHandler *handler = ble.getPalGattClient().get_event_handler();
        if (handler) {
            handler->on_write_command_sent(
                event->hdr.param,
                event->handle,
                event->hdr.status
            );
        }
    } else {
        // all handlers are stored in a static array
        static const event_handler_t handlers[] = {
            &timeout_event_handler,
            &event_handler<ErrorResponseConverter>,
            &event_handler<FindInformationResponseConverter>,
            &event_handler<FindByTypeValueResponseConverter>,
            &event_handler<ReadByTypeResponseConverter>,
            &event_handler<ReadResponseConverter>,
            &event_handler<ReadBlobResponseConverter>,
            &event_handler<ReadMultipleResponseConverter>,
            &event_handler<ReadBygroupTypeResponseConverter>,
            &event_handler<WriteResponseConverter>,
            &event_handler<PrepareWriteResponseConverter>,
            &event_handler<ExecuteWriteResponseConverter>,
            &event_handler<HandleValueIndicationConverter>,
            &event_handler<HandleValueNotificationConverter>
        };

        // event->hdr.param: connection handle
        // event->header.event: opcode from the request
        // event->header.status: success or error code ...
        // event->pValue: starting after opcode for response; starting after opcode + handle for server initiated responses.
        // event->handle: handle for server initiated responses

        // traverse all handlers and execute them with the event in input.
        // exit if an handler has handled the event.
        for(size_t i = 0; i < (sizeof(handlers)/sizeof(handlers[0])); ++i) {
            if (handlers[i](event)) {
                return;
            }
        }
    }
#endif // BLE_FEATURE_GATT_CLIENT

#if BLE_FEATURE_GATT_SERVER
    // pass events not handled to the server side
    ble::vendor::cordio::GattServer::getInstance().att_cb(event);
#endif // BLE_FEATURE_GATT_SERVER
}

} // cordio
} // vendor
} // pal
} // ble
