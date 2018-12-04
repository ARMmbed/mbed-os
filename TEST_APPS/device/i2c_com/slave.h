/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#ifndef TEST_APPS_DEVICE_I2C_COM_SLAVE_H_
#define TEST_APPS_DEVICE_I2C_COM_SLAVE_H_

#include <stdint.h>

struct TransferConfig {
    bool result;
    uint32_t write_size;
    uint32_t write_size_resulting;
    uint32_t read_size;
    uint32_t read_size_resulting;
    uint32_t iterations;
    uint16_t address;
};

bool slave_transfer(uint32_t write_size, uint32_t write_size_resulting, uint32_t read_size, uint32_t read_size_resulting, uint16_t address, uint32_t iterations);
bool slave_init(uint32_t frequency);
bool slave_deinit();
void slave_finish();


#endif /* TEST_APPS_DEVICE_I2C_COM_SLAVE_H_ */
