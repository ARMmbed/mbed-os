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

#ifndef HCI_MBED_OS_ADAPTATION_H_
#define HCI_MBED_OS_ADAPTATION_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Write data to the driver.
 *
 * @param type HCI packet type.
 * @param len Number of bytes to write
 * @param pData Pointer to the data to write.
 * @return the actual number of data bytes written,
 *
 * @note type parameter allows the driver layer to prepend the data with a
 * header on the same write transaction.
 * @note mbed os wrapper of hciDrvWrite
 */
uint16_t hci_mbed_os_drv_write(uint8_t type, uint16_t len, uint8_t *pData);

/**
 * Start the hci reset sequence.
 * @note stable wrapper around hciCoreResetStart.
 * @note definition provided in mbed OS.
 */
void hci_mbed_os_start_reset_sequence(void);

/**
 * Handle hci messages during the reset sequence.
 * @param msg: The HCI message to handle.
 * @note stable wrapper of hciCoreResetSequence.
 * @note definition provided in mbed os.
 */
void hci_mbed_os_handle_reset_sequence(uint8_t* msg);

/**
 * Signal to the stack that the reset sequence is done.
 * @note shall be called from mbed OS when the reset sequence has been done.
 */
void hci_mbed_os_signal_reset_sequence_done(void);

#ifdef __cplusplus
};
#endif

#endif /* HCI_MBED_OS_ADAPTATION_H_ */
