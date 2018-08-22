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
 * \file nfc_transport.c
 * \copyright Copyright (c) ARM Ltd 2013-2018
 * \author Donatien Garnier
 * \details Transport layer
 */

/** \addtogroup Implementation
 *  @{
 *  \name Transport
 *  @{
 */

#include "stack/nfc_errors.h"
#include "nfc_transport.h"

/** Initialize transport with a specific implementation
 * \param pTransport pointer to a nfc_transport_t structure to initialize
 * \param write transport write function
 * \param read transport read function
 * \param pUser parameter that will be passed to any of the above functions
 */
void nfc_transport_init(nfc_transport_t *pTransport, nfc_transport_write_fn_t write, nfc_transport_read_fn_t read, void *pUser)
{
    pTransport->write = write;
    pTransport->read = read;
    pTransport->pUser = pUser;
}




/**
 * @}
 * @}
 * */

