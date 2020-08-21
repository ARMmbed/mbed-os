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

#ifndef CUSTOM_CHCI_TR_H_
#define CUSTOM_CHCI_TR_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Provide this callback in your HCI driver.
 *
 * @param prot Must be CHCI_TR_PROT_BLE.
 * @param hci_type HCI type, HCI_ACL_TYPE or HCI_CMD_TYPE.
 * @param len Buffer length.
 * @param pData Data to be sent.
 * @return Number of bytes processed.
 */
uint16_t ControllerToHostWrite(uint8_t prot, uint8_t hci_type, uint16_t len, uint8_t *pData);

/**
 * Send bytes from host to controller.
 *
 * @param prot Protocol, must be CHCI_TR_PROT_BLE.
 * @param hci_type HCI type, HCI_ACL_TYPE or HCI_CMD_TYPE.
 * @param len Buffer length.
 * @param pData Data to be sent.
 * @return Number of bytes processed.
 */
uint16_t CustomChciTrRead(uint8_t prot, uint8_t hci_type, uint16_t len, uint8_t *pData);

/**
 * Send bytes from controller to host.
 *
 * @param prot Protocol, must be CHCI_TR_PROT_BLE.
 * @param controller_type Controller type, CHCI_TR_TYPE_DATA or CHCI_TR_TYPE_EVT.
 * @param len Buffer length.
 * @param pData Data to be sent.
 * @return Number of bytes processed.
 */
uint16_t CustomChciTrWrite(uint8_t prot, uint8_t controller_type, uint16_t len, uint8_t *pData);

void CustomChciTrInit(uint16_t maxAclLen, uint16_t maxIsoSduLen);

#ifdef __cplusplus
};
#endif

#endif /* CUSTOM_CHCI_TR_H_ */
