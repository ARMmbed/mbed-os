/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __BOOT_RECORD_H__
#define __BOOT_RECORD_H__

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../ext/mcuboot/bootutil/include/bootutil/image.h"
#include "../ext/mcuboot/include/flash_map/flash_map.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \enum shared_data_err_t
 *
 * \brief Return values for adding data entry to shared memory area
 */
enum shared_memory_err_t {
    SHARED_MEMORY_OK = 0,
    SHARED_MEMORY_OVERFLOW = 1,
    SHARED_MEMORY_OVERWRITE = 2,

    /* This is used to force the maximum size */
    TLV_TYPE_MAX = INT_MAX
};

/*!
 * \enum boot_status_err_t
 *
 * \brief Return values for saving boot status information to shared memory are
 */
enum boot_status_err_t {
    BOOT_STATUS_OK,
    BOOT_STATUS_ERROR,
};

/*!
 * \brief Add a data item to the shared data area between bootloader and
 *        runtime SW
 *
 * \param[in] major_type  TLV major type, identify consumer
 * \param[in] minor_type  TLV minor type, identify TLV type
 * \param[in] size        length of added data
 * \param[in] data        pointer to data
 *
 * \return Returns error code as specified in \ref shared_memory_err_t
 */
enum shared_memory_err_t
boot_add_data_to_shared_area(uint8_t        major_type,
                             uint16_t       minor_type,
                             size_t         size,
                             const uint8_t *data);

/*!
 * \brief Add an image's all boot status information to the shared data area
 *        between bootloader and runtime SW
 *
 * \param[in]  sw_module  Identifier of the SW component
 * \param[in]  hdr        Pointer to the image header stored in RAM
 * \param[in]  fap        Pointer to the flash area where image is stored
 *
 * \return Returns error code as specified in \ref boot_status_err_t
 */
enum boot_status_err_t
boot_save_boot_status(uint8_t sw_module,
                      const struct image_header *hdr,
                      const struct flash_area *fap);

#ifdef __cplusplus
}
#endif

#endif /* __BOOT_RECORD_H__ */
