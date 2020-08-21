/* Copyright (c) 2019 Unisoc Communications Inc.
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

#ifndef _WLAND_FLASH_WP_H_
#define _WLAND_FLASH_WP_H_

#ifdef __cplusplus
extern "C" {
#endif
extern void flash_wrtie_protect_all();
extern void flash_wrtie_protect_none();
extern void flash_wrtie_protect(unsigned int offset);
extern void rda5981_flash_init();
#ifdef __cplusplus
}
#endif

#endif

