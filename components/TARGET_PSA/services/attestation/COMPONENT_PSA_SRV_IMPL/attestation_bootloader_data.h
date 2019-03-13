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

#define IMPL_ID_DATA_SIZE  S_IMPL_ID_DATA_SIZE
#define VERIFICATION_SERVICE_DATA_SIZE  S_VERIFICATION_SERVICE_DATA_SIZE
#define PROFILE_DEFINITION_DATA_SIZE  S_PROFILE_DEFINITION_DATA_SIZE

#if (DUMMY_ATTESTATION == 1)
extern const uint8_t temp_ram_page_data[];
extern const uint8_t temp_impl_id_data[IMPL_ID_DATA_SIZE];
extern const char temp_profile_def_data[PROFILE_DEFINITION_DATA_SIZE];
extern const char temp_verification_service_data[VERIFICATION_SERVICE_DATA_SIZE];
#endif

/* Shared data area between bootloader and runtime firmware */
#define S_RAM_ALIAS_BASE  SHARED_DATA_BASE

#define BOOT_TFM_SHARED_DATA_BASE ((uint8_t *)S_RAM_ALIAS_BASE)

#define IMPL_ID_DATA  ((uint8_t *)S_IMPL_ID_DATA)

/* Example verification service URL for initial attestation token - temporary data*/
#define VERIFICATION_SERVICE_DATA  ((char *)S_VERIFICATION_SERVICE_DATA)

/* Example profile definition document for initial attestation token - temporary data*/
#define PROFILE_DEFINITION_DATA  ((char *)S_PROFILE_DEFINITION_DATA)

#ifdef __cplusplus
}
#endif

#endif /* __ATTESTATION_BOOTLOADER_DATA_H__ */
