/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
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

/* Temporary Implementation ID data: mandatory claim represents the original
** implementation signer of the attestation key and identifies the contract
** between the report and verification */
#define TEMP_IMPL_ID_DATA_SIZE (32u)

#define TEMP_IMPL_ID_DATA  0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, \
                           0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, \
                           0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, \
                           0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF
                           
extern uint8_t impl_id_data[];

#ifdef __cplusplus
}
#endif

#endif /* __ATTESTATION_BOOTLOADER_DATA_H__ */
