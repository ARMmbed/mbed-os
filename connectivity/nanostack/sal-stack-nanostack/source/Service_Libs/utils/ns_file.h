/*
 * Copyright (c) 2017, 2020, Pelion and affiliates.
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

#ifndef _NS_FILE_H_
#define _NS_FILE_H_

/**
 * \file ns_file.h
 * \brief Nanostack file handling API.
 */

#include "ns_file_system.h"

/**
 * File open
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
NS_FILE ns_fopen(const char *filename, const char *mode);

/**
 * File close
 *
 * \param handle file handle
 *
 * \return 0 on success
 * \return < 0 in case of errors
 *
 */
int ns_fclose(NS_FILE *ns_handle);

/**
 * File remove
 *
 * \param filename filename
 *
 * \return 0 on success
 * \return < 0 in case of errors
 *
 */
int ns_fremove(const char *filename);

/**
 * File write
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
size_t ns_fwrite(NS_FILE *ns_handle, const void *buffer, size_t size);

/**
 * File read
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
size_t ns_fread(NS_FILE *ns_handle, void *buffer, size_t size);

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
int ns_fsize(NS_FILE *ns_handle, size_t *size);

#endif /* _NS_FILE_SYSTEM_H_ */
