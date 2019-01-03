/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
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

#ifndef MBED_DIRECT_ACCESS_DEVICEKEY_H
#define MBED_DIRECT_ACCESS_DEVICEKEY_H

#include "FlashIAP.h"
#include <stdio.h>
/**
 * @brief Returns devicekey directly from internal FlashIAP by given tdb internal position
 *
 * @param[in]  tdb_start_offset     FlashIAP Start address offset of tdb store
 * @param[in]  tdb_end_offset       FlashIAP End address offset of tdb store
 * @param[in]  data_buf             Buffer to store devicekey data
 * @param[in]  data_buf_size        Max expected size of buffer to store the devicekey data
 * @param[out] actual_data_size_ptr Actual retrieved devicekey size
 *
 * @returns MBED_ERROR_INVALID_ARGUMENT         Illegal argument given
 *          MBED_ERROR_INVALID_SIZE             data_buf_size is too small for retrieving actual data size
 *          MBED_ERROR_INVALID_DATA_DETECTED    devicekey data invalid crc
 *          MBED_ERROR_ITEM_NOT_FOUND           devicekey data not found
 *          MBED_ERROR_FAILED_OPERATION         internal flash failure
 *          MBED_ERROR_READ_FAILED              internal flash read failed
 *          MBED_SUCCESS                        successful
 */
int direct_access_to_devicekey(uint32_t tdb_start_offset, uint32_t tdb_end_offset, void *data_buf,
                               size_t data_buf_size, size_t *actual_data_size_ptr);

#endif /* MBED_DIRECT_ACCESS_DEVICEKEY_H */
