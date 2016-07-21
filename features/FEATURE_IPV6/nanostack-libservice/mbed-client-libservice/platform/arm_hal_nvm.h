/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Nanostack NVM API.
 *
 * After NVM initialization (platform_nvm_init(...)) a user can:
 * -create key to the NVM by using method platform_nvm_key_create(...)
 * -write data to the NVM by using method platform_nvm_write(...)
 * -read data from the NVM by using method platform_nvm_read(...)
 * -delete the created key by using platform_nvm_key_delete(...)
 * -store changed data to the underlying backing store by using method platform_nvm_flush(...).
 *
 * This NVM API is asynchronous. If API function returns PLATFORM_NVM_OK then provided callback function will be
 * called once operation is completed. Callback function carries status parameter that indicates status of the
 * operation. If platform API function returns error code then callback will not be called. A new operation can not
 * be started until the previous operation has completed.
 *
 */

#ifndef _PLATFORM_NVM_H_
#define _PLATFORM_NVM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Enumeration for nvm API function return values */
typedef enum {
    PLATFORM_NVM_OK,
    PLATFORM_NVM_KEY_NOT_FOUND,
    PLATFORM_NVM_ERROR
} platform_nvm_status;

/** \brief Client registered API callback type.
 *
 * \param status operation completion status
 * \param context client provided context that was used when requesting operation
 */
typedef void (nvm_callback)(platform_nvm_status status, void *context);

/** \brief Initialize NVM. Allocates module static resources and initializes underlying NMV.
 *
 * \param callback callback function that will be called once initialization is completed.
 * \param context A client registered context that will be supplied as an argument to the callback when called.
 *
 * \return NVM_OK if initialization is in progress and callback will be called.
 * \return NVM_ERROR if initialization failed and callback will not be called.
  */
platform_nvm_status platform_nvm_init(nvm_callback *callback, void *context);

/** \brief Free resources reserved by NVM init.
 *
 * \param callback callback function that will be called once deinitialization is completed.
 * \param context A client registered context that will be supplied as an argument to the callback when called.
 *
 * \return NVM_OK if initialization is in progress and callback will be called.
 * \return NVM_ERROR if initialization failed and callback will not be called.
  */
platform_nvm_status platform_nvm_finalize(nvm_callback *callback, void *context);

/** \brief Create key to the NMV. If the key exists then the key will be recreated with new length.
 *
 * \param callback callback function that will be called once key creation is finished
 * \param key_name name of the key to be created
 * \param value_len length of data reserved for the key
 * \param flags reserved for future use
 * \param context A client registered context that will be supplied as an argument to the callback when called
 *
 * \return NVM_OK if key creation is in progress and callback will be called.
 * \return NVM_ERROR if key creation failed and callback will not be called.
 * \return Provided callback function will be called once operation is completed.
 */
platform_nvm_status platform_nvm_key_create(nvm_callback *callback, const char *key_name, uint16_t value_len, uint32_t flags, void *context);

/** \brief Delete key from NMV.
 *
 * \param callback callback function that will be called once key creation is finished
 * \param key_name name of the key to be deleted
 * \param context A client registered context that will be supplied as an argument to the callback when called
 *
 * \return NVM_OK if key creation is in progress and callback will be called.
 * \return NVM_ERROR if key creation failed and callback will not be called.
 * \return Provided callback function will be called once operation is completed.
 */
platform_nvm_status platform_nvm_key_delete(nvm_callback *callback, const char *key_name, void *context);

/** \brief Write data to the NVM. Data will be truncated if the key does not have enough space for the data.
 *
 * \param callback callback function that will be called once writing is complete
 * \param key_name name of the key where data will be written
 * \param data buffer to data to be write. Data must be valid until callback is called.
 * \param data_len [IN] length of data in bytes. [OUT] number of bytes written. Argument must be valid until a callback is called.
 * \param context A client registered context that will be supplied as an argument to the callback when called.
 *
 * \return NVM_OK if data writing is in progress and callback will be called.
 * \return NVM_ERROR if data writing failed and callback will not be called.
 * \return Provided callback function will be called once operation is completed.
 */
platform_nvm_status platform_nvm_write(nvm_callback *callback, const char *key_name, const void *data, uint16_t *data_len, void *context);

/** \brief Read key value from the NVM.
 *
 * \param callback callback function that will be called once reading is complete
 * \param key_name name of the key whose data will be read
 * \param buf buffer where data will be copied. Argument must be valid until a callback is called.
 * \param buf_len [IN] provided buffer length in bytes. [OUT] bytes read. Argument must be valid until callback is called.
 * \param context A client registered context that will be supplied as an argument to the callback when called.
 *
 * \return NVM_OK if data reading is in progress and callback will be called.
 * \return NVM_ERROR if data reading failed and callback will not be called.
 * \return Provided callback function will be called once operation is completed.
 */
platform_nvm_status platform_nvm_read(nvm_callback *callback, const char *key_name, void *buf, uint16_t *buf_len, void *context);

/** \brief Store changed data to the backing store. This operation will write changed data to SRAM/Flash.
 *
 * \param callback callback function that will be called once flushing is complete
 * \param context A client registered context that will be supplied as an argument to the callback when called.
 *
 * \return NVM_OK if data flushing is in progress and callback will be called.
 * \return NVM_ERROR if data flushing failed and callback will not be called.
 * \return Provided callback function will be called once operation is completed.
 */
platform_nvm_status platform_nvm_flush(nvm_callback *callback, void *context);

#ifdef __cplusplus
}
#endif
#endif /* _PLATFORM_NVM_H_ */

