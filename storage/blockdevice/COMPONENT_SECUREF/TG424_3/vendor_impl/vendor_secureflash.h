/*
 * Copyright (c) 2020-2023 Macronix International Co. LTD. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _VENDOR_SECUREFLASH_H_
#define _VENDOR_SECUREFLASH_H_

#include "vendor_secureflash_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const secure_flash_info_t macronix_mx78_info;

const secure_flash_info_t *flash_info[] = {
    &macronix_mx78_info,
    /* Add secure flash info here */
};

#ifdef __cplusplus
}
#endif

#endif /* _VENDOR_SECUREFLASH_H_ */