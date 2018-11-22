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
#include "chci_tr_serial.h"
#include "chci_drv.h"
#include "platform_nordic.h"
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

uint16_t NRFCordioHCITransportDriver::write(uint8_t type, uint16_t len, uint8_t *pData)
{
    chciTrSerialRxIncoming(&type, 1);
    chciTrSerialRxIncoming(pData, len);
    return len;
}

extern "C" void chciDrvInit(void)
{
    // No-op
}

// Callback from Cordio stack
extern "C" uint16_t chciDrvWrite(uint8_t prot, uint8_t type, uint16_t len, uint8_t *pData)
{
    uint8_t ctype = (type == CHCI_TR_TYPE_EVT) ? HCI_EVT_TYPE : HCI_ACL_TYPE;
    CordioHCITransportDriver::on_data_received(&ctype, 1);
    CordioHCITransportDriver::on_data_received(pData, len);
    return len;
}
