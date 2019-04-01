/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "fake_lhci_drv.h"
#include "chci_tr.h"
#include "chci_api.h"
#include "hci_defs.h"
#include "wsf_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

uint16_t FakeChciTrRead(uint8_t prot, uint8_t hci_type, uint16_t len, uint8_t *pData)
{
    uint8_t controller_type;
    switch (hci_type) {
    case HCI_CMD_TYPE:
        controller_type = CHCI_TR_TYPE_CMD;
        break;
    case HCI_ACL_TYPE:
        controller_type = CHCI_TR_TYPE_DATA;
        break;
    case HCI_ISO_TYPE:
        controller_type = CHCI_TR_TYPE_ISO;
        break;
    default:
        /* should never happen */
        WSF_ASSERT(false);
        return 0;
        break;
    }

    chciTrRecv(prot, controller_type, pData);
    return len;
}

uint16_t FakeChciTrWrite(uint8_t prot, uint8_t controller_type, uint16_t len, uint8_t *pData)
{
    uint8_t hci_type;
    switch (controller_type) {
    case CHCI_TR_TYPE_EVT:
        hci_type = HCI_EVT_TYPE;
        break;
    case CHCI_TR_TYPE_DATA:
        hci_type = HCI_ACL_TYPE;
        break;
    case CHCI_TR_TYPE_ISO:
        hci_type = HCI_ISO_TYPE;
        break;
    default:
        /* should never happen */
        WSF_ASSERT(false);
        return 0;
        break;
    }

    return ControllerToHostWrite(prot, hci_type, len, pData);
}

#ifdef __cplusplus
};
#endif
