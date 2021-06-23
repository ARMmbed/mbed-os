/*
 * Copyright (c) 2017, 2019-2020, Pelion and affiliates.
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

#ifndef _NS_FILE_SYSTEM_H_
#define _NS_FILE_SYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file ns_file_system.h
 * \brief Nanostack file system API.
 */

/**
 * \brief Set file system root path.
 *
 *  Allow stack to store information to the location provided.
 *  Setting root path to NULL will prevent file system usage.
 *
 * \param root_path Path to location where networking files can be stored. Path must exists in the file system
 *   and path must end to "/" character. Path can be NULL to disable file system usage.
 *
 * \return 0 in success, negative value in case of error.
 *
 */

int ns_file_system_set_root_path(const char *root_path);

/**
 * \brief Get file system root path.
 *
 * \return Root path to stack storage location.
 *
 */
char *ns_file_system_get_root_path(void);

/**
 * \brief NS file handle
 *
 */
typedef void *NS_FILE;

/**
 * File open callback
 *
 * Depending on underlying file system file open for read for non-existing
 * files can return success. In that case file read will fail.
 *
 * \param filename filename
 * \param mode can be either "r" or "w"
 *
 * \return file handle
 * \return NULL on error
 *
 */
typedef NS_FILE(*ns_file_open)(const char *filename, const char *mode);

/**
 * File close callback
 *
 * \param handle file handle
 *
 * \return 0 on success
 * \return < 0 in case of errors
 *
 */
typedef int (*ns_file_close)(NS_FILE *handle);

/**
 * File remove callback
 *
 * \param filename filename
 *
 * \return 0 on success
 * \return < 0 in case of errors
 *
 */
typedef int (*ns_file_remove)(const char *filename);

/**
 * File write callback
 *
 * Write is not stream write. The whole file is written from start to end
 * and if function is called again, previous file content is replaced with
 * new content.
 *
 * \param handle file handle
 * \param buffer buffer
 * \param buffer buffer size
 *
 * \return bytes written
 *
 */
typedef size_t (*ns_file_write)(NS_FILE *handle, const void *buffer, size_t size);

/**
 * File read callback
 *
 * Read is not stream read. The whole file is read from start to end
 * and if function is called again, read is started from start again.
 *
 * \param handle file handle
 * \param buffer buffer
 * \param size buffer size
 *
 * \return bytes written
 *
 */
typedef size_t (*ns_file_read)(NS_FILE *handle, void *buffer, size_t size);

/**
 * File size callback
 *
 * Reads file size.
 *
 * \param handle file handle
 * \param size file size
 *
 * \return 0 on success
 * \return < 0 in case of reading file size is not supported
 *
 */
typedef int (*ns_file_size)(NS_FILE *handle, size_t *size);

/**
 * File callbacks set
 *
 * Sets file handling callbacks to nanostack.
 *
 * \param open file open callback
 * \param close file close callback
 * \param remove file remove callback
 * \param write file write callback
 * \param read file read callback
 * \param size file size callback
 *
 */
void ns_file_system_callbacks_set(ns_file_open open, ns_file_close close, ns_file_remove remove, ns_file_write write, ns_file_read read, ns_file_size size);

#ifdef __cplusplus
}
#endif

#endif /* _NS_FILE_SYSTEM_H_ */
