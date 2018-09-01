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

#ifndef MBED_NFC_REMOTE_ENDPOINT_H
#define MBED_NFC_REMOTE_ENDPOINT_H

#include <stdint.h>

#include "NFCDefinitions.h"

namespace mbed {
namespace nfc {

/**
 * @addtogroup nfc
 * @{
 */

class NFCController;

/**
 * This is the base class for all remote endpoints (initiators and targets)
 * addressable over the air interface.
 */
class NFCRemoteEndpoint {
public:
    /**
     * Create a NFCRemoteEndpointinstance
     * @param[in] controller the NFCController instance that detected this endpoint
     */
    NFCRemoteEndpoint(NFCController *controller);

    /**
     * Destructor
     */
    virtual ~NFCRemoteEndpoint();

    /**
     * The NFCRemoteEndpoint base delegate.
     */
    struct Delegate {
        /**
         * This method is called when the endpoint is connected
         */
        virtual void on_connected() {}

        /**
         * This method is called when the endpoint is lost (air interface link disconnnected)
         */
        virtual void on_disconnected() {}

    protected:
        ~Delegate() {}
    };

    /**
     * Connect the remote endpoint
     *
     * @return NFC_OK or an error code
     */
    virtual nfc_err_t connect() = 0;

    /**
     * Disconnect the remote endpoint
     *
     * @return NFC_OK or an error code
     */
    virtual nfc_err_t disconnect() = 0;

    /**
     * Check if the endpoint is connected.
     * @return whether the endpoint is connected
     */
    virtual bool is_connected() const = 0;

    /**
     * Check if the endpoint is disconnected/lost.
     * @return whether the endpoint has been disconnected
     */
    virtual bool is_disconnected() const = 0;

    /**
     * Get the list of RF protocols supported and activated over the air interface.
     * @return a bitmask of activated protocols
     */
    virtual nfc_rf_protocols_bitmask_t rf_protocols() = 0;

protected:
    /**
     * Mark endpoint as connected
     */
    virtual void connected() = 0;

    /**
     * Mark endpoint as disconnected
     */
    virtual void disconnected() = 0;

    /**
     * Retrieve NFCController instance
     * @return a pointer to the NFController instance that created this endpoint.
     */
    NFCController *nfc_controller();

    /**
     * Retrieve NFCController instance
     * @return a pointer to the NFController instance that created this endpoint.
     */
    const NFCController *nfc_controller() const;

private:
    NFCController *_controller;
};

/**
 * @}
 */

} // namespace nfc
} // namespace mbed

#endif
