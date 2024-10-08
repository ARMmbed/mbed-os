/*
 * Copyright (c) 2022-2023 Macronix International Co. LTD. All rights reserved.
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
#ifndef _PLATFORM_IMPL_H_
#define _PLATFORM_IMPL_H_

#ifdef __cplusplus
extern "C" {
#endif

int32_t plat_store_secure_flash_provision_info(uint8_t *buffer, int size);
int32_t plat_get_secure_flash_provision_info(uint8_t *buffer, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* _PLATFORM_IMPL_H_ */