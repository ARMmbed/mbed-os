/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifdef __cplusplus
}
#endif

#endif /* _NS_FILE_SYSTEM_H_ */
