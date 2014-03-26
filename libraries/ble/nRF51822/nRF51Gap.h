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
  
#ifndef __NRF51822_GAP_H__
#define __NRF51822_GAP_H__

#include "mbed.h"
#include "blecommon.h"
#include "GapAdvertisingParams.h"
#include "GapAdvertisingData.h"
#include "hw/Gap.h"

/**************************************************************************/
/*!
    \brief

*/
/**************************************************************************/
class nRF51Gap : public Gap
{
    public:
        static nRF51Gap& getInstance()
        {
            static nRF51Gap m_instance;
            return m_instance;
        }

        /* Functions that must be implemented from Gap */
        virtual ble_error_t setAdvertisingData(GapAdvertisingData &, GapAdvertisingData &);
        virtual ble_error_t startAdvertising(GapAdvertisingParams &);
        virtual ble_error_t stopAdvertising(void);
        virtual ble_error_t disconnect(void);
        
    private:
        nRF51Gap() { };
        nRF51Gap(nRF51Gap const&);
        void operator=(nRF51Gap const&);
};

#endif
