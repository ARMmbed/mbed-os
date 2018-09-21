/*
 * Copyright (c) 2014-2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * \file pn512_transceive.h
 * \copyright Copyright (c) ARM Ltd 2014
 * \author Donatien Garnier
 */

#ifndef PN512_TRANSCEIVE_H_
#define PN512_TRANSCEIVE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stack/nfc_common.h"
#include "pn512.h"

void pn512_transceive_hw(pn512_t *pPN512, pn512_transceive_mode_t mode, pn512_cb_t cb);

void pn512_transceive_hw_tx_task(uint32_t events, void *pUserData);
void pn512_transceive_hw_tx_iteration(pn512_t *pPN512, bool start);

void pn512_transceive_hw_rx_start(pn512_t *pPN512);
void pn512_transceive_hw_rx_task(uint32_t events, void *pUserData);


#ifdef __cplusplus
}
#endif

#endif /* PN512_TRANSCEIVE_H_ */
