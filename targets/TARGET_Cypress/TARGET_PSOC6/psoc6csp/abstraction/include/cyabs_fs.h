/***************************************************************************//**
* \file cyabs_fs.h
*
* \brief
* Basic file system abstraction layer. This API provides convenience methods
* for reading and writing values.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
*******************************************************************************/

/**
* \addtogroup group_abstraction_fs File system abstraction
* \ingroup group_abstraction
* \{
* Basic file system abstraction layer. This API provides convenience methods
* for reading and writing values.
*
* \defgroup group_abstraction_fs_macros Macros
* \defgroup group_abstraction_fs_data_structures Data Structures
* \defgroup group_abstraction_fs_functions Functions
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_abstraction_fs_macros
* \{
*/

#define CY_O_RDONLY             (1 << 0) /**< TODO */
#define CY_O_WRONLY             (1 << 1) /**< TODO */
#define CY_O_RDWR               (1 << 2) /**< TODO */
#define CY_O_APPEND             (1 << 3) /**< TODO */
#define CY_O_CREAT              (1 << 4) /**< TODO */

/** \} group_abstraction_fs_macros */

/**
* \addtogroup group_abstraction_fs_data_structures
* \{
*/

typedef uint32_t cy_handle_t; /**< Resource handle */

/** \} group_abstraction_fs_data_structures */


/**
* \addtogroup group_abstraction_fs_functions
* \{
*/

/**
  * \brief open or create a file and return a handle
  * \param path the path name of the file to open
  * \param oflag the mode to use when opening the file
  * \param handle pointer to location re receive handle
  * \returns CY_RSLT_SUCCESS if successful, otherwise error code
  */
cy_rslt_t cy_fs_open(const char *path, uint32_t oflag, cy_handle_t *handle) ;

/**
 * \brief close an open file
 * \param handle a file handle to an open file
 * \returns CY_RSLT_SUCCESS if successful, otherwise error code
 */
 cy_rslt_t cy_fs_close(cy_handle_t handle) ;

/**
 * \brief read data from a file
 * \param handle a file handle open for reading or read/write
 * \param buf the buffer for the read data
 * \param nbyte the size of the buffer in bytes, the number of bytes read on return
 * \returns CY_RSLT_SUCCESS if successful, otherwise error code
 */
cy_rslt_t cy_fs_read(cy_handle_t handle, void *buf, size_t *nbyte) ;

/**
 * \brief write data to a file
 * \param handle a file handle open for writing or read/write
 * \param buf the buffer for the data to write
 * \param nbyte the size of the buffer in bytes, the number of bytes written on return
 * \returns CY_RSLT_SUCCESS if successful, otherwise error code
 */
cy_rslt_t cy_fs_write(cy_handle_t handle, const void *buf, size_t *nbyte) ;

/** \} group_abstraction_fs_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_abstraction_fs */
