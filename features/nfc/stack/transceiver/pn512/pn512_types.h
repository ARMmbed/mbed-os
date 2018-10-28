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
 * \file pn512_types.h
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#ifndef TRANSCEIVER_PN512_PN512_TYPES_H_
#define TRANSCEIVER_PN512_PN512_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stack/nfc_common.h"

typedef struct __pn512 pn512_t;

typedef struct __pn512_registers {
    int8_t registers_page;
} pn512_registers_t;



#ifdef __cplusplus
}
#endif

#endif /* TRANSCEIVER_PN512_PN512_TYPES_H_ */
