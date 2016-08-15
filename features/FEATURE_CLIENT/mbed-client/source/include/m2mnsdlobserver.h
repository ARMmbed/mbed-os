/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
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
#ifndef M2M_NSDL_OBSERVER_H
#define M2M_NSDL_OBSERVER_H

#include "include/nsdllinker.h"

//FORWARD DECLARATION
class M2MSecurity;
class M2MServer;

/**
 * @brief Observer class for informing NSDL callback to the state machine
 */

class M2MNsdlObserver
{

public :

    /**
    * @brief Informs that coap message is ready.
    * @param data_ptr, Data object of coap message.
    * @param data_len, Length of the data object.
    * @param address_ptr, Address structure of the server.
    */
    virtual void coap_message_ready(uint8_t *data_ptr,
                                    uint16_t data_len,
                                    sn_nsdl_addr_s *address_ptr) = 0;

    /**
    * @brief Informs that client is registered successfully.
    * @param server_object, Server object associated with
    * registered server.
    */
    virtual void client_registered(M2MServer *server_object) = 0;

    /**
    * @brief Informs that client registration is updated successfully.
    * @param server_object, Server object associated with
    * registered server.
    */
    virtual void registration_updated(const M2MServer &server_object) = 0;

    /**
    * @brief Informs that some error occured during
    * registration.
    * @param error_code, Error code for registration error
    * @param retry, Indicates state machine to re-establish connection
    */
    virtual void registration_error(uint8_t error_code, bool retry = false) = 0;

    /**
    * @brief Informs that client is unregistered successfully.
    */
    virtual void client_unregistered() = 0;

    /**
    * @brief Informs that client bootstrapping is done.
    * @param security_object, M2MSecurity Object which contains information about
    * LWM2M server fetched from bootstrap server.
    */
    virtual void bootstrap_done(M2MSecurity *security_object) = 0;

    /**
    * @brief Informs that some error occured during
    * bootstrapping.
    */
    virtual void bootstrap_error() = 0;

    /**
    * @brief Informs that received data has been processed.
    */
    virtual void coap_data_processed() = 0;

    /**
     * @brief Callback informing that the value of the resource object is updated by server.
     * @param base Object whose value is updated.
     */
    virtual void value_updated(M2MBase *base) = 0;
};
#endif // M2M_NSDL_OBSERVER_H
