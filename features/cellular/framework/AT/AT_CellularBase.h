/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
#ifndef AT_CELLULAR_BASE_H_
#define AT_CELLULAR_BASE_H_

#include "ATHandler.h"

namespace mbed {

/**
 *  Class AT_CellularBase
 *
 *  A base class for AT-classes.
 */
class AT_CellularBase {
public:
    AT_CellularBase(ATHandler &at);
    /** Getter for at handler. Common method for all AT-classes.
     *
     *  @return reference to ATHandler
     */
    ATHandler &get_at_handler();

    /** Gets the device error that happened when using AT commands/responses. This is at error
     *  returned by the device. Returned CME/CMS errors can be found from 3gpp documents 27007 and 27005.
     *
     *  @return at error (CME/CMS) while communicating with the device
     */
    device_err_t get_device_error() const;

    enum CellularProperty {
        PROPERTY_C_EREG,            // AT_CellularNetwork::RegistrationMode. What support modem has for this registration type.
        PROPERTY_C_GREG,            // AT_CellularNetwork::RegistrationMode. What support modem has for this registration type.
        PROPERTY_C_REG,             // AT_CellularNetwork::RegistrationMode. What support modem has for this registration type.
        PROPERTY_AT_CGSN_WITH_TYPE, // 0 = not supported, 1 = supported. AT+CGSN without type is likely always supported similar to AT+GSN.
        PROPERTY_AT_CGDATA,         // 0 = not supported, 1 = supported. Alternative is to support only ATD*99***<cid>#
        PROPERTY_AT_CGAUTH,         // 0 = not supported, 1 = supported. APN authentication AT commands supported
        PROPERTY_IPV4_STACK,        // 0 = not supported, 1 = supported
        PROPERTY_IPV6_STACK,        // 0 = not supported, 1 = supported
        PROPERTY_IPV4V6_STACK,      // 0 = not supported, 1 = supported
        PROPERTY_MAX
    };

    /** Cellular module need to define an array of cellular properties which defines module supported property values.
     *
     *  @param property_array array of module properties
     */
    static void set_cellular_properties(const intptr_t *property_array);

protected:

    static const intptr_t *_property_array;

    ATHandler &_at;

    /** Get value for the given key.
     *
     *  @param key  key for value to be fetched
     *  @return     property value for the given key. Value type is defined in enum CellularProperty
     */
    static intptr_t get_property(CellularProperty key);
};

} // namespace mbed

#endif /* AT_CELLULAR_BASE_H_ */
