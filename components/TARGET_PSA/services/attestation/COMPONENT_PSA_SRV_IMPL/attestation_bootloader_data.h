/*
* Copyright (c) 2018-2019 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef __ATTESTATION_BOOTLOADER_DATA_H__
#define __ATTESTATION_BOOTLOADER_DATA_H__

#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

/* Temp Shared data area between bootloader and runtime firmware */
extern const uint8_t temp_ram_page_data[];

#define S_RAM_ALIAS_BASE  (temp_ram_page_data)

#define BOOT_TFM_SHARED_DATA_BASE S_RAM_ALIAS_BASE

extern const uint8_t impl_id_data[];

/* Example verification service URL for initial attestation token - temporary data*/
static const char verification_service_url[] = "www.mbed.com";
/* Example profile definition document for initial attestation token - temporary data*/
static const char attestation_profile_definition[] = "psa-attest.md";

#ifdef __cplusplus
}
#endif

#endif /* __ATTESTATION_BOOTLOADER_DATA_H__ */
