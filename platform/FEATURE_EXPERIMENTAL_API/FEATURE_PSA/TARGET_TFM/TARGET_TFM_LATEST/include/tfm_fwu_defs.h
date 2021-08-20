/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef TFM_FWU_BOOTLOADER_DEFS_H
#define TFM_FWU_BOOTLOADER_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Image type identities. When porting a specific bootloader to FWU partition,
 * the bootloader specific image types can be defined here.
 */
#define FWU_IMAGE_TYPE_NONSECURE        0x01U
#define FWU_IMAGE_TYPE_SECURE           0x02U
#define FWU_IMAGE_TYPE_FULL             0x03U

#ifdef __cplusplus
}
#endif
#endif /* TFM_FWU_BOOTLOADER_DEFS_H */
