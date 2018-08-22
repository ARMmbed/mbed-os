/*
 * Copyright (c) 2013-2018, ARM Limited, All Rights Reserved
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
 * \file transceiver.c
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 * \details Transceiver
 */

/** \addtogroup Transceiver
 *  @{
 *  \name Initialization
 *  @{
 */

#include "transceiver.h"

/** Initialize nfc_transceiver_t structure
 * \param pTransceiver pointer to nfc_transceiver_t structure to initialize
 * \param pTransport pointer to already initialized nfc_transport_t structure
 * \param pImpl pointer to the structure implementing the transceiver interface (eg pn512_t or pn532_t)
 */
void transceiver_init(nfc_transceiver_t *pTransceiver, nfc_transport_t *pTransport, nfc_scheduler_timer_t *pTimer)
{
    pTransceiver->pTransport = pTransport;
    nfc_scheduler_init(&pTransceiver->scheduler, pTimer);
}


/**
 * @}
 * @}
 * */
