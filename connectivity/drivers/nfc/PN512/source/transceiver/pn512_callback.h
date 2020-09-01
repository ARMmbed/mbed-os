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
 * \file pn512_callback.h
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#ifndef PN512_CALLBACK_H_
#define PN512_CALLBACK_H_

#include "stack/nfc_common.h"
#include "pn512_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*pn512_cb_t)(pn512_t *pPN512, nfc_err_t ret);

#ifdef __cplusplus
}
#endif

#endif /* PN512_CALLBACK_H_ */
