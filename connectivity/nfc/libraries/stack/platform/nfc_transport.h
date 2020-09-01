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
 * \file nfc_transport.h
 * \copyright Copyright (c) ARM Ltd 2013-2018
 * \author Donatien Garnier
 */

/** \addtogroup Implementation
 *  @{
 *  \name Transport
 *  @{
 */

#ifndef NFC_TRANSPORT_H_
#define NFC_TRANSPORT_H_

#include "stack/nfc_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Function called to write a register's value
 * \param address address of the register to write to
 * \param outBuf buffer to write
 * \param outLen buffer's length
 * \param pUser parameter passed to the nfc_transport_init function
 */
typedef void (*nfc_transport_write_fn_t)(uint8_t address, const uint8_t *outBuf, size_t outLen, void *pUser);

/** Function called to read a register's value
 * \param address address to read packet from
 * \param outBuf buffer to read
 * \param outLen buffer's length
 * \param pUser parameter passed to the nfc_transport_init function
 */
typedef void (*nfc_transport_read_fn_t)(uint8_t address, uint8_t *inBuf, size_t inLen, void *pUser);

typedef struct __transport {
    nfc_transport_write_fn_t write;
    nfc_transport_read_fn_t read;
    void *pUser;
} nfc_transport_t;

void nfc_transport_init(nfc_transport_t *pTransport, nfc_transport_write_fn_t write, nfc_transport_read_fn_t read, void *pUser);

static inline void nfc_transport_write(nfc_transport_t *pTransport, uint8_t address, const uint8_t *outBuf, size_t outLen)
{
    pTransport->write(address, outBuf, outLen, pTransport->pUser);
}

static inline void nfc_transport_read(nfc_transport_t *pTransport, uint8_t address, uint8_t *inBuf, size_t inLen)
{
    pTransport->read(address, inBuf, inLen, pTransport->pUser);
}

#ifdef __cplusplus
}
#endif

#endif /* NFC_TRANSPORT_H_ */


/**
 * @}
 * @}
 * */

