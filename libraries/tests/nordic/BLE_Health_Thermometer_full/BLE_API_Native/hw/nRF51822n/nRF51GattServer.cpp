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
  
#include "nRF51GattServer.h"
#include "mbed.h"

#include "common/common.h"
#include "btle/custom/custom_helper.h"

/**************************************************************************/
/*!
    @brief  Adds a new service to the GATT table on the peripheral
            
    @returns    ble_error_t
    
    @retval     BLE_ERROR_NONE
                Everything executed properly
                
    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF51GattServer::addService(GattService & service)
{
    /* ToDo: Make sure we don't overflow the array, etc. */
    /* ToDo: Make sure this service UUID doesn't already exist (?) */
    /* ToDo: Basic validation */
    
    /* Add the service to the nRF51 */
    ble_uuid_t uuid;

    if (service.primaryServiceID.type == UUID::UUID_TYPE_SHORT)
    {
      /* 16-bit BLE UUID */
      uuid.type = BLE_UUID_TYPE_BLE;
    }
    else
    {
      /* 128-bit Custom UUID */
      uuid.type = custom_add_uuid_base( service.primaryServiceID.base );
    }

    uuid.uuid = service.primaryServiceID.value;

    ASSERT( ERROR_NONE == sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &uuid, &service.handle), BLE_ERROR_PARAM_OUT_OF_RANGE );

    /* Add characteristics to the service */
    for (uint8_t i = 0; i < service.characteristicCount; i++)
    {
      GattCharacteristic * p_char = service.characteristics[i];

      uuid.uuid = p_char->uuid;
      ASSERT ( ERROR_NONE == custom_add_in_characteristic(service.handle, &uuid, p_char->properties,
                                                          NULL, p_char->lenMin, p_char->lenMax, &nrfCharacteristicHandles[characteristicCount]), BLE_ERROR_PARAM_OUT_OF_RANGE );
        
      /* Update the characteristic handle */
      p_char->handle = characteristicCount;
      p_characteristics[characteristicCount++] = p_char;
    }

    serviceCount++;
    
    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Reads the value of a characteristic, based on the service
            and characteristic index fields

    @param[in]  charHandle
                The handle of the GattCharacteristic to read from
    @param[in]  buffer
                Buffer to hold the the characteristic's value
                (raw byte array in LSB format)
    @param[in]  len
                The number of bytes read into the buffer
            
    @returns    ble_error_t
    
    @retval     BLE_ERROR_NONE
                Everything executed properly
                
    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF51GattServer::readValue(uint16_t charHandle, uint8_t buffer[], uint16_t len)
{
    ASSERT( ERROR_NONE == sd_ble_gatts_value_get(nrfCharacteristicHandles[charHandle].value_handle, 0, &len, buffer), BLE_ERROR_PARAM_OUT_OF_RANGE);
    
    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Updates the value of a characteristic, based on the service
            and characteristic index fields

    @param[in]  charHandle
                The handle of the GattCharacteristic to write to
    @param[in]  buffer
                Data to use when updating the characteristic's value
                (raw byte array in LSB format)
    @param[in]  len
                The number of bytes in buffer
            
    @returns    ble_error_t
    
    @retval     BLE_ERROR_NONE
                Everything executed properly
                
    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF51GattServer::updateValue(uint16_t charHandle, uint8_t buffer[], uint16_t len)
{
  if ((p_characteristics[charHandle]->properties & (GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)) &&
      (m_connectionHandle != BLE_CONN_HANDLE_INVALID) )
  {
    /* HVX update for the characteristic value */
    ble_gatts_hvx_params_t hvx_params;

    hvx_params.handle = nrfCharacteristicHandles[charHandle].value_handle;
    hvx_params.type   = (p_characteristics[charHandle]->properties & GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)  ? BLE_GATT_HVX_NOTIFICATION : BLE_GATT_HVX_INDICATION;
    hvx_params.offset = 0;
    hvx_params.p_data = buffer;
    hvx_params.p_len  = &len;

    error_t error = (error_t) sd_ble_gatts_hvx(m_connectionHandle, &hvx_params);

    /* ERROR_INVALID_STATE, ERROR_BUSY, ERROR_GATTS_SYS_ATTR_MISSING and ERROR_NO_TX_BUFFERS the ATT table has been updated. */
    if ( (error != ERROR_NONE                      ) && (error != ERROR_INVALID_STATE) &&
         (error != ERROR_BLE_NO_TX_BUFFERS         ) && (error != ERROR_BUSY         ) &&
         (error != ERROR_BLEGATTS_SYS_ATTR_MISSING ) )
    {
      ASSERT_INT( ERROR_NONE, sd_ble_gatts_value_set(nrfCharacteristicHandles[charHandle].value_handle, 0, &len, buffer), BLE_ERROR_PARAM_OUT_OF_RANGE );
    }
  } else
  {
    ASSERT_INT( ERROR_NONE, sd_ble_gatts_value_set(nrfCharacteristicHandles[charHandle].value_handle, 0, &len, buffer), BLE_ERROR_PARAM_OUT_OF_RANGE );
  }
    
  return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Callback handler for events getting pushed up from the SD
*/
/**************************************************************************/
void nRF51GattServer::hwCallback(ble_evt_t * p_ble_evt)
{
  uint16_t handle_value;
  GattServerEvents::gattEvent_t event;

  switch (p_ble_evt->header.evt_id)
  {
    case BLE_GATTS_EVT_WRITE:
      /* There are 2 use case here: Values being updated & CCCD (indicate/notify) enabled */
      
      /* 1.) Handle CCCD changes */
      handle_value = p_ble_evt->evt.gatts_evt.params.write.handle;
      for(uint8_t i=0; i<characteristicCount; i++)
      {
        if ( (p_characteristics[i]->properties & (GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)) &&
             (nrfCharacteristicHandles[i].cccd_handle == handle_value) )
        {
          uint16_t cccd_value = (p_ble_evt->evt.gatts_evt.params.write.data[1] << 8) | p_ble_evt->evt.gatts_evt.params.write.data[0]; /* Little Endian but M0 may be mis-aligned */

          if ( ((p_characteristics[i]->properties & GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE) && (cccd_value & BLE_GATT_HVX_INDICATION  )) ||
               ((p_characteristics[i]->properties & GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY  ) && (cccd_value & BLE_GATT_HVX_NOTIFICATION)))
          {
            event = GattServerEvents::GATT_EVENT_UPDATES_ENABLED;
          } else
          {
            event = GattServerEvents::GATT_EVENT_UPDATES_DISABLED;
          }

          handleEvent(event, i);
          return;
        }
      }

      /* 2.) Changes to the characteristic value will be handled with other events below */
      event = GattServerEvents::GATT_EVENT_DATA_WRITTEN;
      break;

    case BLE_GATTS_EVT_HVC:
      /* Indication confirmation received */
      event        = GattServerEvents::GATT_EVENT_CONFIRMATION_RECEIVED;
      handle_value = p_ble_evt->evt.gatts_evt.params.hvc.handle;
      break;

    default: 
      return;
  }

  /* Find index (charHandle) in the pool */
  for(uint8_t i=0; i<characteristicCount; i++)
  {
    if (nrfCharacteristicHandles[i].value_handle == handle_value)
    {
      handleEvent(event, i);
      break;
    }
  }
}
