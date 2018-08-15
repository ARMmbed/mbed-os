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

#ifndef MBED_ISO7816_APP_H
#define MBED_ISO7816_APP_H

#include <stdint.h>
#include <stddef.h>

#include "NFCDefinitions.h"

namespace mbed {
namespace nfc {

    /**
     * @addtogroup nfc
     * @{
     */

    class Type4RemoteInitiator;
    
    /**
     * This base class represents an ISO7816-4 application.
     */ 
    class ISO7816App {
        /**
         * This class describes an ISO7816-4 exchange (Command/Response).
         */ 
        class Exchange {
        public:
            /**
             * A Command APDU (Application Protocol Data Unit) .
             */
            struct CAPDU
            {
                uint8_t cla; ///< CLA - Instruction class
                uint8_t ins; ///< INS - Instruction code
                uint8_t p1; ///< P1 - First parameter
                uint8_t p2; ///< P2 - Second parameter
                ac_buffer_t dataIn; ///< Command data
                size_t maxRespLength; ///< Maximum response length
            };

            /**
             * A Response APDU (Application Protocol Data Unit) 
             */
            struct RAPDU
            {
                ac_buffer_t dataOut; ///< Response data
                uint16_t sw; ///< Status word
            };

            /**
             * Access Command APDU.
             *
             *  @return a const reference to the Command ADPU
             */
            const CAPDU& command() const;

            /**
             * Access Response APDU.
             *
             *  @return a mutable reference to the Command ADPU
             */
            RAPDU& response();

            /**
             * Respond to command.
             * 
             * @return NFC_OK if the response could be sent back successfully, or an error code otherwise
             */ 
            nfc_err_t respond();

        private:
            CAPDU _command;
            RAPDU _response;
            ISO7816App* _iso7816_app;
        };

        /** 
         * Construct ISO7816 app instance
         */ 
        ISO7816App();

    private:
        friend class Type4RemoteInitiator;

        /**
         * Retrieve the application's identifier (AID).
         * AIDs are composed of a RID (Registered Application Provider Identifier) that needs to be registered and a custom suffix.
         * 
         * @return a pointer to a const buffer containing the application's identifier (AID).
         */ 
        virtual const ac_buffer_t* get_aid() const = 0;

        /**
         * Called when the application is selected and before any exchange is performed.
         */ 
        virtual void on_selected() = 0;

        /**
         * Called when the application is deselected (or link is lost).
         */ 
        virtual void on_deselected() = 0;

        /**
         * Called when an exchange is performed.
         * The application must respond using the respond() method of the Exchange class.
         * 
         * @param[in] exchange an instance of the Exchange class populated with the C-APDU which was received
         */ 
        virtual void on_exchange(Exchange* exchange) = 0;

        nfc_tech_iso7816_app_t iso7816_app;
    };

    /**
     * @}
     */

} // namespace nfc
} // namespace mbed

#endif
