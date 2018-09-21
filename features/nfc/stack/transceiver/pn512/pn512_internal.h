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
 * \file pn512_internal.h
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 */

#ifndef PN512_INTERNAL_H_
#define PN512_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stack/nfc_common.h"
#include "transceiver/transceiver_internal.h"

#include "pn512.h"
#include "pn512_callback.h"

//Public
void pn512_set_protocols(nfc_transceiver_t *pTransceiver, nfc_tech_t initiators, nfc_tech_t targets, polling_options_t options);
void pn512_poll(nfc_transceiver_t *pTransceiver);
void pn512_set_crc(nfc_transceiver_t *pTransceiver, bool crc_out, bool crc_in);
void pn512_set_timeout(nfc_transceiver_t *pTransceiver, int timeout);
void pn512_set_transceive_options(nfc_transceiver_t *pTransceiver, bool transmit, bool receive, bool repoll);
void pn512_set_transceive_framing(nfc_transceiver_t *pTransceiver, nfc_framing_t framing);
void pn512_set_write(nfc_transceiver_t *pTransceiver, ac_buffer_t *pWriteBuf);
ac_buffer_t *pn512_get_read(nfc_transceiver_t *pTransceiver);
size_t pn512_get_last_byte_length(nfc_transceiver_t *pTransceiver);
void pn512_set_last_byte_length(nfc_transceiver_t *pTransceiver, size_t lastByteLength);
void pn512_set_first_byte_align(nfc_transceiver_t *pTransceiver, size_t firstByteAlign);
void pn512_abort(nfc_transceiver_t *pTransceiver);
void pn512_transceive(nfc_transceiver_t *pTransceiver);
void pn512_close(nfc_transceiver_t *pTransceiver);
void pn512_sleep(nfc_transceiver_t *pTransceiver, bool sleep);

void pn512_transceiver_callback(pn512_t *pPN512, nfc_err_t ret);

static inline void pn512_rf_callback(pn512_t *pPN512, nfc_err_t ret)
{
    pPN512->rf.cb(pPN512, ret);
}

static inline void pn512_poll_callback(pn512_t *pPN512, nfc_err_t ret)
{
    pPN512->poll.cb(pPN512, ret);
}

static inline void pn512_anticollision_callback(pn512_t *pPN512, nfc_err_t ret)
{
    pPN512->anticollision.cb(pPN512, ret);
}

static inline void pn512_transceive_callback(pn512_t *pPN512, nfc_err_t ret)
{
    pPN512->transceive.cb(pPN512, ret);
}



#ifdef __cplusplus
}
#endif

#endif /* PN512_INTERNAL_H_ */
