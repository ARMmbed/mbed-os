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
  
#include "mbed.h"
#include "nRF51822n.h"

#include "btle/btle.h"

/**************************************************************************/
/*!
    @brief  Constructor
*/
/**************************************************************************/
nRF51822n::nRF51822n(void)
{
}

/**************************************************************************/
/*!
    @brief  Destructor
*/
/**************************************************************************/
nRF51822n::~nRF51822n(void)
{
}

/**************************************************************************/
/*!
    @brief  Initialises anything required to start using BLE
            
    @returns    ble_error_t
    
    @retval     BLE_ERROR_NONE
                Everything executed properly
                
    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF51822n::init(void)
{
  /* ToDo: Clear memory contents, reset the SD, etc. */
  btle_init();

  return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Resets the BLE HW, removing any existing services and
            characteristics
            
    @returns    ble_error_t
    
    @retval     BLE_ERROR_NONE
                Everything executed properly
                
    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF51822n::reset(void)
{
    wait(0.5);
    
    /* Wait for the radio to come back up */
    wait(1);
    
    return BLE_ERROR_NONE;
}
