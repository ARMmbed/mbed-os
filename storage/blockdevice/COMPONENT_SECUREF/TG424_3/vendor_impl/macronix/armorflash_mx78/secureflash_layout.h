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
#ifndef _SECUREFLASH_LAYOUT_H_
#define _SECUREFLASH_LAYOUT_H_

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Secure Flash name string. */
#define SECURE_FLASH_NAME            "mx78x64"
#define SECURE_FLASH_SECURITY_STORAGE_CAP (1)
#define SECURE_FLASH_SECTOR_SIZE     (0x1000) /*!< The size of secure Flash's sector */
#define SECURE_FLASH_ERASED_VALUE    (0xFF)   /*!< The erase value of secure Flash */
#define SECURE_FLASH_PROGRAM_UNIT    (0x01)   /*!< The program unit of secure Flash */
#define SECURE_FLASH_READ_UNIT       (0x01)   /*!< The read unit of secure Flash */
#define SECURE_FLASH_PROGRAM_SIZE    (0x100)  /*!< The max data size of each secure program packet */
#define SECURE_FLASH_READ_SIZE       (0x100)  /*!< The max data size of each secure read packet */
#define SECURE_FLASH_SIZE            (0x800000)
#define SECURE_FLASH_ZONE_SIZE       (0x80000)
#define SECURE_FLASH_ZONE_NUM        (SECURE_FLASH_SIZE / SECURE_FLASH_ZONE_SIZE)

#define SECURE_FLASH_MAX_PUF_SIZE    (0x20)
#define SECURE_FLASH_MAX_TRNG_SIZE   (0x20)
#define SECURE_FLASH_MAX_MC_SIZE     (4)
/** \brief If multi-client(or multi-application) isolation is defined,
 *         the number of clients,
 *         and the layout of secure Flash memory region should be defined.
 *         Here takes four applications for example.
 */
#if defined(MULTI_CLIENT_ISOLATION) /*!< Enable multi-client isolation */
/* multi-client secure flash layout*/
/* Negative client id stands for clients from nspe;
 * Positive client id stands for clients from spe */
#define SECURE_FLASH_CLIENT_NUM (3)
#define SECURE_FLASH_CLIENT0_ID                (-2) /*!< Client id 0:0xfffffffe */
#define SECURE_FLASH_CLIENT0_AREA_START_ADDR   (0)  /*!< Start address of security memory region allocated for Client id 0 */
#define SECURE_FLASH_CLIENT0_AREA_SIZE         (SECURE_FLASH_ZONE_SIZE) /*!< The size of security memory region allocated for Client id 0 */
#define SECURE_FLASH_CLIENT0_SECTORS_PER_BLOCK (4)
#if (SECURE_FLASH_CLIENT_NUM > 1)
#define SECURE_FLASH_CLIENT1_ID                (-3) /*!< Client id 1:0xfffffffd */
#define SECURE_FLASH_CLIENT1_AREA_START_ADDR   (SECURE_FLASH_CLIENT0_AREA_START_ADDR + SECURE_FLASH_CLIENT0_AREA_SIZE) /*!< Start address of security memory region allocated for Client id 1 */
#define SECURE_FLASH_CLIENT1_AREA_SIZE         (SECURE_FLASH_ZONE_SIZE) /*!< The size of security memory region allocated for Client id 1 */
#define SECURE_FLASH_CLIENT1_SECTORS_PER_BLOCK (4)
#endif
#if (SECURE_FLASH_CLIENT_NUM > 2)
#define SECURE_FLASH_CLIENT2_ID                (3002) /*!< Client id 2:0xbba */
#define SECURE_FLASH_CLIENT2_AREA_START_ADDR   (SECURE_FLASH_CLIENT1_AREA_START_ADDR + SECURE_FLASH_CLIENT1_AREA_SIZE) /*!< Start address of security memory region allocated for Client id 2 */
#define SECURE_FLASH_CLIENT2_AREA_SIZE         (SECURE_FLASH_ZONE_SIZE) /*!< The size of security memory region allocated for Client id 2 */
#define SECURE_FLASH_CLIENT2_SECTORS_PER_BLOCK (4)
#endif
#if (SECURE_FLASH_CLIENT_NUM > 3)
#define SECURE_FLASH_CLIENT3_ID                (3006) /*!< Client id 3:0xbbe */
#define SECURE_FLASH_CLIENT3_AREA_START_ADDR   (SECURE_FLASH_CLIENT2_AREA_START_ADDR + SECURE_FLASH_CLIENT2_AREA_SIZE) /*!< Start address of security memory region allocated for Client id 3 */
#define SECURE_FLASH_CLIENT3_AREA_SIZE         (SECURE_FLASH_ZONE_SIZE) /*!< The size of security memory region allocated for Client id 3 */
#define SECURE_FLASH_CLIENT3_SECTORS_PER_BLOCK (4)
#endif
#endif
/* Multi client isolation disabled */
#define SECURE_FLASH_START_ADDR                (0) /*!< Start address of secure Flash's security memory region */
#define SECURE_FLASH_DEFAULT_CLIENT_AREA_SIZE  (SECURE_FLASH_ZONE_SIZE *2) /*!< Default client area size equals secure zone size */
#define SECURE_FLASH_SECTORS_PER_BLOCK         (4) /*!< The number of sectors of per secure Flash block */

#ifdef __cplusplus
}
#endif

#endif /* _SECUREFLASH_LAYOUT_H_ */
