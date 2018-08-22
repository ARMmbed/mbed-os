/*
 * Copyright (c) 2015-2018, ARM Limited, All Rights Reserved
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
 * \file transceiver_internal.h
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#ifndef TRANSCEIVER_INTERNAL_H_
#define TRANSCEIVER_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stack/nfc_common.h"
#include "transceiver.h"

static inline void transceiver_callback(nfc_transceiver_t *pTransceiver, nfc_err_t ret)
{
    pTransceiver->cb(pTransceiver, ret, pTransceiver->pUserData);
}

#ifdef __cplusplus
}
#endif

#endif /* TRANSCEIVER_INTERNAL_H_ */
