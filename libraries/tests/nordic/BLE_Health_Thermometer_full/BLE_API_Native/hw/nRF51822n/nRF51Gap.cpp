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
  
#include "nRF51Gap.h"
#include "mbed.h"

#include "common/common.h"
#include "ble_advdata.h"

/**************************************************************************/
/*!
    @brief  Sets the advertising parameters and payload for the device

    @param[in]  params
                Basic advertising details, including the advertising
                delay, timeout and how the device should be advertised
    @params[in] advData
                The primary advertising data payload
    @params[in] scanResponse
                The optional Scan Response payload if the advertising
                type is set to \ref GapAdvertisingParams::ADV_SCANNABLE_UNDIRECTED
                in \ref GapAdveritinngParams
            
    @returns    \ref ble_error_t
    
    @retval     BLE_ERROR_NONE
                Everything executed properly

    @retval     BLE_ERROR_BUFFER_OVERFLOW
                The proposed action would cause a buffer overflow.  All
                advertising payloads must be <= 31 bytes, for example.
                
    @retval     BLE_ERROR_NOT_IMPLEMENTED
                A feature was requested that is not yet supported in the
                nRF51 firmware or hardware.

    @retval     BLE_ERROR_PARAM_OUT_OF_RANGE
                One of the proposed values is outside the valid range.

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF51Gap::setAdvertisingData(GapAdvertisingData & advData, GapAdvertisingData & scanResponse)
{
    /* Make sure we don't exceed the advertising payload length */
    if (advData.getPayloadLen() > GAP_ADVERTISING_DATA_MAX_PAYLOAD)
    {
        return BLE_ERROR_BUFFER_OVERFLOW;
    }
    
    /* Make sure we have a payload! */
    if (advData.getPayloadLen() == 0)
    {
      return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    /* Check the scan response payload limits */
    //if ((params.getAdvertisingType() == GapAdvertisingParams::ADV_SCANNABLE_UNDIRECTED))
    //{
    //    /* Check if we're within the upper limit */
    //    if (advData.getPayloadLen() > GAP_ADVERTISING_DATA_MAX_PAYLOAD)
    //    {
    //        return BLE_ERROR_BUFFER_OVERFLOW;
    //    }
    //    /* Make sure we have a payload! */
    //    if (advData.getPayloadLen() == 0)
    //    {
    //        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    //    }
    //}
    
    /* Send advertising data! */
    ASSERT( ERROR_NONE == sd_ble_gap_adv_data_set(advData.getPayload(), advData.getPayloadLen(),
                                                  scanResponse.getPayload(), scanResponse.getPayloadLen()), BLE_ERROR_PARAM_OUT_OF_RANGE);

	/* Make sure the GAP Service appearance value is aligned with the appearance from GapAdvertisingData */	
	ASSERT( ERROR_NONE == sd_ble_gap_appearance_set(advData.getAppearance()), BLE_ERROR_PARAM_OUT_OF_RANGE);

    /* ToDo: Perform some checks on the payload, for example the Scan Response can't */
    /* contains a flags AD type, etc. */

    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Starts the BLE HW, initialising any services that were
            added before this function was called.
            
    @note   All services must be added before calling this function!
            
    @returns    ble_error_t
    
    @retval     BLE_ERROR_NONE
                Everything executed properly
                
    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF51Gap::startAdvertising(GapAdvertisingParams & params)
{
      /* Make sure we support the advertising type */
    if (params.getAdvertisingType() == GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED)
    {
        /* ToDo: This requires a propery security implementation, etc. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /* Check interval range */
    if (params.getAdvertisingType() == GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED)
    {
        /* Min delay is slightly longer for unconnectable devices */
        if ((params.getInterval() < GAP_ADV_PARAMS_INTERVAL_MIN_NONCON) ||
            (params.getInterval() > GAP_ADV_PARAMS_INTERVAL_MAX))
        {
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        }
    }
    else
    {
        if ((params.getInterval() < GAP_ADV_PARAMS_INTERVAL_MIN) ||
            (params.getInterval() > GAP_ADV_PARAMS_INTERVAL_MAX))
        {
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        }
    }

    /* Check timeout is zero for Connectable Directed */
    if ((params.getAdvertisingType() == GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) &&
        (params.getTimeout() != 0))
    {
        /* Timeout must be 0 with this type, although we'll never get here */
        /* since this isn't implemented yet anyway */
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    /* Check timeout for other advertising types */
    if ((params.getAdvertisingType() != GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) &&
        (params.getTimeout() > GAP_ADV_PARAMS_TIMEOUT_MAX))
    {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    /* ToDo: Start Advertising */
    ble_gap_adv_params_t adv_para = { 0 };

    adv_para.type        = params.getAdvertisingType() ;
    adv_para.p_peer_addr = NULL                        ; // Undirected advertisement
    adv_para.fp          = BLE_GAP_ADV_FP_ANY          ;
    adv_para.p_whitelist = NULL                        ;
    adv_para.interval    = params.getInterval()        ; // advertising interval (in units of 0.625 ms)
    adv_para.timeout     = params.getTimeout()         ;

    ASSERT( ERROR_NONE == sd_ble_gap_adv_start(&adv_para), BLE_ERROR_PARAM_OUT_OF_RANGE);
		
		state.advertising = 1;

    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Stops the BLE HW and disconnects from any devices
            
    @returns    ble_error_t
    
    @retval     BLE_ERROR_NONE
                Everything executed properly
                
    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF51Gap::stopAdvertising(void)
{
    /* ToDo: Stop Advertising */

    /* ToDo: Check response */
    wait(0.1);
	
	  state.advertising = 0;

    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Disconnects if we are connected to a central device
            
    @returns    ble_error_t
    
    @retval     BLE_ERROR_NONE
                Everything executed properly
                
    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF51Gap::disconnect(void)
{
	/* ToDo: Disconnect if we are connected to a central device */
	
	  state.advertising = 0;
	  state.connected = 0;
	
    return BLE_ERROR_NONE;
}
