/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_ERROR_HIST_H
#define MBED_ERROR_HIST_H

#ifndef MBED_CONF_PLATFORM_ERROR_HIST_SIZE
#define MBED_CONF_PLATFORM_ERROR_HIST_SIZE  4
#else
#if MBED_CONF_PLATFORM_ERROR_HIST_SIZE == 0
#define MBED_CONF_PLATFORM_ERROR_HIST_SIZE  1
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** \ingroup mbed-os-internal */
/** \addtogroup platform-internal-api */
/** @{*/

/*
 * Puts/Adds an error entry into the error history list
 *
 * @param  error_ctx            pointer to the mbed_error_ctx struct with the error context
 * @return                      0 or MBED_SUCCESS on success.
 *                              MBED_ERROR_WRITE_FAILED if writing to file failed
 *                              MBED_ERROR_INVALID_ARGUMENT if path is not valid
 *
 *
 */
mbed_error_status_t mbed_error_hist_put(mbed_error_ctx *error_ctx);

/*
 * Reads the error entry from the error list with the specified index
 *
 * @param  index                Index of the error context to be retrieved. It starts from 0 and 0 is the oldest.
 * @param  error_ctx            pointer to the mbed_error_ctx struct where the error context will be filled, this should be allocated by the caller
 * @return                      0 or MBED_SUCCESS on success.
 *                              MBED_ERROR_WRITE_FAILED if writing to file failed
 *                              MBED_ERROR_INVALID_ARGUMENT if path is not valid
 *
 *
 */
mbed_error_status_t mbed_error_hist_get(int index, mbed_error_ctx *error_ctx);

/*
 * Gets a reference to the next error entry in the error log where in the error ctx can be filled in.
 * Its like reserving the next error entry to fill in the error info
 *
 * @return                      Returns the pointer to the next error ctx entry
 *
 *
 */
mbed_error_ctx *mbed_error_hist_get_entry(void);

/*
 * Reads the last(latest) error entry from the error history
 *
 * @param  error_ctx            pointer to the mbed_error_ctx struct where the error context will be filled, this should be allocated by the caller
 * @return                      0 or MBED_SUCCESS on success.
 *                              MBED_ERROR_WRITE_FAILED if writing to file failed
 *                              MBED_ERROR_INVALID_ARGUMENT if path is not valid
 *
 *
 */
mbed_error_status_t mbed_error_hist_get_last_error(mbed_error_ctx *error_ctx);

/*
 * Returns the number of error entries in the error history list
 *
 * @return                      Number of entries in the history list
 *
 *
 */
int mbed_error_hist_get_count(void);

/*
 * Resets the error log by resetting the number of errors to 0 and clears all previous errors in the history list
 *
 * @return                      0 or MBED_SUCCESS on success.
 *                              MBED_ERROR_WRITE_FAILED if writing to file failed
 *                              MBED_ERROR_INVALID_ARGUMENT if path is not valid
 *
 *
 */
mbed_error_status_t mbed_error_hist_reset(void);

/*
 * Saves the error log information to a file
 *
 * @param  path                 path to the file in the filesystem
 * @return                      0 or MBED_SUCCESS on success.
 *                              MBED_ERROR_WRITE_FAILED if writing to file failed
 *                              MBED_ERROR_INVALID_ARGUMENT if path is not valid
 *
 * @note                        Filesystem support is required in order for this function to work.
 *
 */
mbed_error_status_t mbed_save_error_hist(const char *path);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
