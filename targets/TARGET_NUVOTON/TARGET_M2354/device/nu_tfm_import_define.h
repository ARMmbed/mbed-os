/*
 * Copyright (c) 2020, Nuvoton Technology Corporation
 *
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

#ifndef __NU_TFM_IMPORT_DEFINE_H__
#define __NU_TFM_IMPORT_DEFINE_H__

/* Update NU_TFM_S_BL2 and friends on redoing TF-M import
 *
 * We expect NU_TFM_S_BL2 and friends are passed along from build tool, esp. Mbed CLI2,
 * If not, NU_TFM_S_BL2 and friends must update manually. */

#ifndef NU_TFM_S_BL2
#define NU_TFM_S_BL2                    1
#endif

#ifndef NU_TFM_S_MCUBOOT_IMAGE_NUMBER
#define NU_TFM_S_MCUBOOT_IMAGE_NUMBER   2
#endif

#ifndef NU_TFM_S_UPDATE_STAGE_SDH
#define NU_TFM_S_UPDATE_STAGE_SDH       1
#endif

#ifndef NU_TFM_S_UPDATE_STAGE_FLASH
#define NU_TFM_S_UPDATE_STAGE_FLASH     0
#endif

#ifndef NU_TFM_S_REGION_DEFS_H_PATH
#define NU_TFM_S_REGION_DEFS_H_PATH     "../TARGET_TFM/TARGET_NU_M2354/COMPONENT_TFM_S_FW/partition/region_defs.h"
#endif

#ifndef NU_TFM_S_PARTITION_MCU_H_PATH
#define NU_TFM_S_PARTITION_MCU_H_PATH   "../TARGET_TFM/TARGET_NU_M2354/COMPONENT_TFM_S_FW/partition/partition_M2354_im.h"
#endif

/* TF-M exported partition/ header files depend on below defines */
#if NU_TFM_S_BL2
#define BL2
#endif
#define MCUBOOT_IMAGE_NUMBER    NU_TFM_S_MCUBOOT_IMAGE_NUMBER
#define NU_UPDATE_STAGE_SDH     NU_TFM_S_UPDATE_STAGE_SDH
#define NU_UPDATE_STAGE_FLASH   NU_TFM_S_UPDATE_STAGE_FLASH

#endif  /* __NU_TFM_IMPORT_DEFINE_H__ */
