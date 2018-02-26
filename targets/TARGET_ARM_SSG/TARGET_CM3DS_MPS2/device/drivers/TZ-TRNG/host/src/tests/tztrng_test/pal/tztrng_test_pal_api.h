/******************************************************************************
 * Copyright (c) 2017-2017, ARM, All Rights Reserved                           *
 * SPDX-License-Identifier: Apache-2.0                                         *
 *                                                                             *
 * Licensed under the Apache License, Version 2.0 (the "License");             *
 * you may not use this file except in compliance with the License.            *
 *                                                                             *
 * You may obtain a copy of the License at                                     *
 * http://www.apache.org/licenses/LICENSE-2.0                                  *
 *                                                                             *
 * Unless required by applicable law or agreed to in writing, software         *
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT   *
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.            *
 *                                                                             *
 * See the License for the specific language governing permissions and         *
 * limitations under the License.                                              *
 ******************************************************************************/

#ifndef _TZTRNG_TEST_HAL__API_H_
#define _TZTRNG_TEST_HAL__API_H_

/**
 * @brief Undo actions that were done while getting the hardware register address
 * Finalize the use of any resource.
 * Should be called before exiting the application.
 *
 * @param regBase		The address to which the hardware base register was mapped to.
 */
void tztrngTest_pal_unmapCcRegs(unsigned long regBase);

/**
 * This function should be called before calling tztrng API.
 * This function maps the HW base register to application memory.
 * In Linux will be mapped to virtual mempry.
 * In freeRTOS mapping in not needed since we have access to memory's physical address.
 *
 * @param regBase		HW_BASE_REG. The offset of the HW base register.
 *
 * @return 				An address to which the HW_BASE_REG was mapped to.
 */
unsigned long tztrngTest_pal_mapCcRegs(unsigned long regBase);

/**
 * @brief This function allocates "size" bytes.
 *
 * @param[in] size in bytes. *
 * @param[out]
 *
 * @return pointer to the allocated memory.
 */
void *tztrngTest_pal_malloc(size_t size);

/**
 * @brief This function allocates "size" bytes.
 *
 * @param[in] size in bytes. *
 * @param[out]
 *
 * @return pointer to the allocated memory.
 */
void tztrngTest_pal_free(void *pvAddress);

/**
 * Dump the data from large_buf to OS specific output.
 *
 * @param large_buf
 * @param outputLen
 *
 * @return
 */
int tztrngTest_pal_dumpData(unsigned char *large_buf, size_t outputLen);

#endif //_TZTRNG_TEST_HAL__API_H_
