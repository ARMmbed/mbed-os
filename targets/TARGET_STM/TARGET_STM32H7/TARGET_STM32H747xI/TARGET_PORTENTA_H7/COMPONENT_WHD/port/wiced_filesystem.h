/*
 * Copyright 2020 Arduino SA
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

/** @file
 *  Provides wiced fs porting to generic mbed APIs
 */

#pragma once

#include "whd_config.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************
 *                   Enumerations
 ******************************************************/

typedef enum {
    WICED_FILESYSTEM_OPEN_FOR_READ,      /** Specifies read access to the object. Data can be read from the file - equivalent to "r" or "rb" */
    WICED_FILESYSTEM_OPEN_FOR_WRITE,     /** Specifies read/write access to the object. Data can be written to the file - equivalent to "r+" or "rb+" or "r+b" */
    WICED_FILESYSTEM_OPEN_WRITE_CREATE,  /** Opens for read/write access, creates it if it doesn't exist */
    WICED_FILESYSTEM_OPEN_ZERO_LENGTH,   /** Opens for read/write access, Truncates file to zero length if it exists, or creates it if it doesn't - equivalent to "w+", "wb+" or "w+b" */
    WICED_FILESYSTEM_OPEN_APPEND,        /** Opens for read/write access, places the current location at the end of the file ready for appending - equivalent to "a", "ab" */
    WICED_FILESYSTEM_OPEN_APPEND_CREATE, /** Opens for read/write access, creates it if it doesn't exist, and places the current location at the end of the file ready for appending  - equivalent to "a+", "ab+" or "a+b" */
} wiced_filesystem_open_mode_t;

typedef enum {
    WICED_FILESYSTEM_SEEK_SET = SEEK_SET,      /* Offset from start of file */
    WICED_FILESYSTEM_SEEK_CUR = SEEK_CUR,      /* Offset from current position in file */
    WICED_FILESYSTEM_SEEK_END = SEEK_END,      /* Offset from end of file */
} wiced_filesystem_seek_type_t;

typedef enum {
    WICED_SUCCESS = 0,
    WICED_ERROR = 1
} wiced_result_t;

/******************************************************
 *                 Type Definitions
 ******************************************************/

/**
 * File-system Handle Structure
 */
typedef int wiced_filesystem_t;

/**
 * File Handle Structure
 *
 */
typedef int wiced_file_t;

/**
 * File-system type Handle Structure
 */
typedef int wiced_filesystem_handle_type_t;

/**
 * Initialise the BlockDevice and filesystem module
 *
 * Initialises the BlockDevice and filesystem module before mounting a physical device.
 *
 * @return WICED_SUCCESS on success
 */
wiced_result_t wiced_filesystem_init(void);

/**
 * Open a file for reading or writing
 *
 * @param[in]  fs_handle       - The filesystem handle to use - obtained from wiced_filesystem_mount
 * @param[out] file_handle_out - a pointer to a wiced_file_t structure which will receive the
 *                               file handle after it is opened
 * @param[in]  filename        - The filename of the file to open
 * @param[in]  mode            - Specifies read or write access
 *
 * @return WICED_SUCCESS on success
 */
wiced_result_t wiced_filesystem_file_open(wiced_filesystem_t *fs_handle, wiced_file_t *file_handle_out, const char *filename, wiced_filesystem_open_mode_t mode);

/**
 * Seek to a location within a file
 *
 * This is similar to the fseek() in ISO C.
 *
 * @param[in] file_handle - The file handle on which to perform the seek.
 *                          Must have been previously opened with wiced_filesystem_fopen.
 * @param[in] offset      - The offset in bytes
 * @param[in] whence      - WICED_FILESYSTEM_SEEK_SET = Offset from start of file
 *                          WICED_FILESYSTEM_SEEK_CUR = Offset from current position in file
 *                          WICED_FILESYSTEM_SEEK_END = Offset from end of file
 *
 * @return WICED_SUCCESS  on success
 */
wiced_result_t wiced_filesystem_file_seek(wiced_file_t *file_handle, int64_t offset, wiced_filesystem_seek_type_t whence);

/**
 * Reads data from a file into a memory buffer
 *
 * @param[in] file_handle          - the file handle to read from
 * @param[out] data                - A pointer to the memory buffer that will
 *                                   receive the data that is read
 * @param[in] bytes_to_read        - the number of bytes to read
 * @param[out] returned_item_count - the number of items successfully read.
 *
 * @return WICED_SUCCESS  on success
 */
wiced_result_t wiced_filesystem_file_read(wiced_file_t *file_handle, void *data, uint64_t bytes_to_read, uint64_t *returned_bytes_count);

/**
 * Close a file
 *
 * This is similar to the fclose() in ISO C.
 *
 * @param[in] file_handle - the file handle to close
 *
 * @return WICED_SUCCESS = success
 */
wiced_result_t wiced_filesystem_file_close(wiced_file_t *file_handle);

#ifdef __cplusplus
} /*extern "C" */
#endif
