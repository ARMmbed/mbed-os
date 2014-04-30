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
  
#ifndef __NRF51822_H__
#define __NRF51822_H__

#define NRF51
#define DEBUG_NRF_USER
#define BLE_STACK_SUPPORT_REQD
#define BOARD_PCA10001

#include "mbed.h"
#include "blecommon.h"
#include "hw/BLEDevice.h"
#include "nRF51Gap.h"
#include "nRF51GattServer.h"

/**************************************************************************/
/*!
    \brief

*/
/**************************************************************************/
class nRF51822n : public BLEDevice
{
    public:
        nRF51822n(void);
        virtual ~nRF51822n(void);

    	virtual Gap&        getGap()        { return nRF51Gap::getInstance(); };
        virtual GattServer& getGattServer() { return nRF51GattServer::getInstance(); };
        virtual ble_error_t init(void);
        virtual ble_error_t reset(void);
};

#endif
