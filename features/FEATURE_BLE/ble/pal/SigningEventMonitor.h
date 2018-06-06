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

#ifndef MBED_BLE_SIGNING_EVENT_MONITOR
#define MBED_BLE_SIGNING_EVENT_MONITOR

#include "ble/BLETypes.h"

namespace ble {
namespace pal {

/**
 * Implemented by classes that need to be notified of signing events.
 * Notification is done by calling functions in the passed in event handler
 */
class SigningEventMonitor {
public:
    /**
     * Implemented by classes that are reacting to signing events.
     */
    class EventHandler {
    public:
        /**
         * Set new signed write peer counter.
         *
         * @param[in] connection connection handle
         * @param[in] sign_coutner counter received from peer
         */
        virtual void on_signed_write_received(
            connection_handle_t connection,
            uint32_t sign_coutner
        ) = 0;

        /**
         * Indicate that signed data was rejected due to verification failure. This could
         * be due to an invalid CSRK key.
         *
         * @param[in] connection connection handle
         */
        virtual void on_signed_write_verification_failure(
            connection_handle_t connection
        ) = 0;

        /**
         * Notify a new signed write cmd was executed.
         */
        virtual void on_signed_write() = 0;
    };

    /**
     * Register a handler for singing events to be used internally and serviced first.
     *
     * @param[in] signing_event_handler Event handler being registered.
     */
    virtual void set_signing_event_handler(EventHandler *signing_event_handler) = 0;
};

} // namespace pal
} // namespace ble

#endif /* MBED_BLE_SIGNING_EVENT_MONITOR */
