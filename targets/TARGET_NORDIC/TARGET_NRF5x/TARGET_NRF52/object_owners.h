/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef OBJECT_OWNERS_H
#define OBJECT_OWNERS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief      Set instance owner for the SPI/I2C peripheral.
 *
 * @param[in]  instance  The instance.
 * @param      object    The object.
 */
void object_owner_spi2c_set(int instance, void *object);

/**
 * @brief      Get instance owner for the SPI/I2C peripheral.
 *
 * @param[in]  instance  The instance.
 *
 * @return     Pointer to the object owning the instance.
 */
void * object_owner_spi2c_get(int instance);

#ifdef __cplusplus
}
#endif

#endif
