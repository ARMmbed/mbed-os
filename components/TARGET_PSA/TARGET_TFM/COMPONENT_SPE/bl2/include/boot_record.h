/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __BOOT_RECORD_H__
#define __BOOT_RECORD_H__

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

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
                             uint8_t        minor_type,
                             size_t         size,
                             const uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif /* __BOOT_RECORD_H__ */
