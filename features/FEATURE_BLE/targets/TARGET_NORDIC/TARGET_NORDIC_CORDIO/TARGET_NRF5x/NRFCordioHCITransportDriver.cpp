/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#include <stddef.h>
#include <string.h>

#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_cs.h"
#include "wsf_math.h"
#include "chci_api.h"
#include "chci_tr.h"
#include "fake_lhci_drv.h"
#include "hci_defs.h"
#include <string.h>

#include "NRFCordioHCITransportDriver.h"

using namespace ble::vendor::nordic;
using namespace ble::vendor::cordio;

NRFCordioHCITransportDriver::~NRFCordioHCITransportDriver() { }

void NRFCordioHCITransportDriver::initialize()
{

}

void NRFCordioHCITransportDriver::terminate()
{

}

uint16_t NRFCordioHCITransportDriver::write(uint8_t hci_type, uint16_t len, uint8_t *pData)
{
    /* ownership of the WSF buffer is transferred to the controller (zero copy HCI) */
    return FakeChciTrRead(CHCI_TR_PROT_BLE, hci_type, len, pData);
}

extern "C" void chciDrvInit(void)
{
    // No-op
}

// Callback from Cordio stack
extern "C" uint16_t ControllerToHostWrite(uint8_t prot, uint8_t hci_type, uint16_t len, uint8_t *pData)
{
    WSF_ASSERT(prot == CHCI_TR_PROT_BLE);

    CordioHCITransportDriver::on_data_received(&hci_type, 1);
    CordioHCITransportDriver::on_data_received(pData, len);

    return len;
}
