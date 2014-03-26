/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
  
#include <stdio.h>
#include <string.h>

#include "GattCharacteristic.h"

/**************************************************************************/
/*!
    @brief  Creates a new GattCharacteristic using the specified 16-bit
            UUID, value length, and properties
            
    @note   The UUID value must be unique in the service and is normally >1

    @param[in]  id
                The 16-bit UUID to use for this characteristic
    @param[in]  minLen
                The min length in bytes of this characteristic's value
    @param[in]  maxLen
                The max length in bytes of this characteristic's value
    @param[in]  props
                The 8-bit bit field containing the characteristic's
                properties

    @section EXAMPLE

    @code

    // UUID = 0x2A19, Min length 2, Max len = 2, Properties = write
    GattCharacteristic c = GattCharacteristic( 0x2A19, 2, 2, BLE_GATT_CHAR_PROPERTIES_WRITE );
   
    @endcode
*/
/**************************************************************************/
GattCharacteristic::GattCharacteristic(uint16_t id, uint16_t minLen, uint16_t maxLen, uint8_t props)
{
    uuid = id;
    memcpy(&properties, &props, 1);
    lenMin = minLen;
    lenMax = maxLen;
//    handle = 0;
}

/**************************************************************************/
/*!
    Destructor
*/
/**************************************************************************/
GattCharacteristic::~GattCharacteristic(void)
{
}
