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

#include "GapAdvertisingData.h"

/**************************************************************************/
/*!
    \brief  Creates a new GapAdvertisingData instance

    \par EXAMPLE

    \code

    \endcode
*/
/**************************************************************************/
GapAdvertisingData::GapAdvertisingData(void)
{
    memset(_payload, 0, GAP_ADVERTISING_DATA_MAX_PAYLOAD);
    _payloadLen = 0;
    _appearance = GENERIC_TAG;
}

/**************************************************************************/
/*!
    Destructor
*/
/**************************************************************************/
GapAdvertisingData::~GapAdvertisingData(void)
{
}

/**************************************************************************/
/*!
    \brief  Adds advertising data based on the specified AD type (see
            DataType)

    \args[in]   advDataType The Advertising 'DataType' to add
    \args[in]   payload     Pointer to the payload contents
    \args[in]   len         Size of the payload in bytes
    
    \returns    ble_error_t
    
    \retval     BLE_ERROR_NONE
                Everything executed properly
                
    \retval     BLE_ERROR_BUFFER_OVERFLOW
                The specified data would cause the advertising buffer
                to overflow
    
    \par EXAMPLE

    \code

    \endcode
*/
/**************************************************************************/
ble_error_t GapAdvertisingData::addData(DataType advDataType, uint8_t * payload, uint8_t len)
{
    /* ToDo: Check if an AD type already exists and if the existing */
    /*       value is exclusive or not (flags, etc.) */
    
    /* Make sure we don't exceed the 31 byte payload limit */
    if (_payloadLen + len + 2 >= GAP_ADVERTISING_DATA_MAX_PAYLOAD)
        return BLE_ERROR_BUFFER_OVERFLOW;

    /* Field length */
    memset(&_payload[_payloadLen], len+1, 1);
    _payloadLen++;
    
    /* Field ID */
    memset(&_payload[_payloadLen], (uint8_t)advDataType, 1); 
    _payloadLen++;
       
    /* Payload */
    memcpy(&_payload[_payloadLen], payload, len);
    _payloadLen += len;
    
    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    \brief  Helper function to add APPEARANCE data to the advertising
            payload

    \args[in]   appearance  The APPEARANCE value to add
    
    \returns    ble_error_t
    
    \retval     BLE_ERROR_NONE
                Everything executed properly
                
    \retval     BLE_ERROR_BUFFER_OVERFLOW
                The specified data would cause the advertising buffer
                to overflow
    
    \par EXAMPLE

    \code

    \endcode
*/
/**************************************************************************/
ble_error_t GapAdvertisingData::addAppearance(Appearance appearance)
{
    _appearance = appearance;
    return addData(GapAdvertisingData::APPEARANCE, (uint8_t*)&appearance, 2);
}

/**************************************************************************/
/*!
    \brief  Helper function to add FLAGS data to the advertising
            payload

    \args[in]   flag  The FLAGS value to add

                \par LE_LIMITED_DISCOVERABLE
                The peripheral is discoverable for a limited period of
                time

                \par LE_GENERAL_DISCOVERABLE
                The peripheral is permanently discoverable
                
                \par BREDR_NOT_SUPPORTED
                This peripheral is a Bluetooth Low Energy only device
                (no EDR support)

    \returns    ble_error_t
    
    \retval     BLE_ERROR_NONE
                Everything executed properly
                
    \retval     BLE_ERROR_BUFFER_OVERFLOW
                The specified data would cause the advertising buffer
                to overflow
    
    \par EXAMPLE

    \code

    \endcode
*/
/**************************************************************************/
ble_error_t GapAdvertisingData::addFlags(Flags flag)
{
    return addData(GapAdvertisingData::FLAGS, (uint8_t*)&flag, 1);
}

/**************************************************************************/
/*!
    \brief  Helper function to add TX_POWER_LEVEL data to the
            advertising payload

    \args[in]   flag  The TX_POWER_LEVEL value to add
    
    \returns    ble_error_t
    
    \retval     BLE_ERROR_NONE
                Everything executed properly
                
    \retval     BLE_ERROR_BUFFER_OVERFLOW
                The specified data would cause the advertising buffer
                to overflow
    
    \par EXAMPLE

    \code

    \endcode
*/
/**************************************************************************/
ble_error_t GapAdvertisingData::addTxPower(int8_t txPower)
{
    /* ToDo: Basic error checking to make sure txPower is in range */
    return addData(GapAdvertisingData::TX_POWER_LEVEL, (uint8_t*)&txPower, 1);
}

/**************************************************************************/
/*!
    \brief Clears the payload and resets the payload length counter
*/
/**************************************************************************/
void GapAdvertisingData::clear(void)
{
    memset(&_payload, 0, GAP_ADVERTISING_DATA_MAX_PAYLOAD);
    _payloadLen = 0;
}

/**************************************************************************/
/*!
    \brief Returns a pointer to the the current payload
    
    \returns    A pointer to the payload
*/
/**************************************************************************/
uint8_t * GapAdvertisingData::getPayload(void)
{
    return (_payloadLen > 0) ? _payload : NULL;
}

/**************************************************************************/
/*!
    \brief Returns the current payload length (0..31 bytes)
    
    \returns    The payload length in bytes
*/
/**************************************************************************/
uint8_t GapAdvertisingData::getPayloadLen(void)
{
    return _payloadLen;
}

/**************************************************************************/
/*!
    \brief Returns the 16-bit appearance value for this device
    
    \returns    The 16-bit appearance value
*/
/**************************************************************************/
uint16_t GapAdvertisingData::getAppearance(void)
{
    return (uint16_t)_appearance;
}
