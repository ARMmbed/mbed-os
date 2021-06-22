/*
 * Copyright (c) 2015-2020, Pelion and affiliates.
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
#ifndef _NS_CRC_H_
#define _NS_CRC_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/// @todo documentation

/** same as crc16_ibm */
uint16_t crc16_ansi(uint8_t *message, int nBytes);
uint16_t crc16_ccitt(uint8_t *message, int nBytes);

/**
 *  @param data         data which crc will be calculate
 *  @param data_length  Length of data pointer
 *  @param polynomial   Polynomial which will be used to calculate CRC, POLYNOMIAL_CRC15_CCIT, POLYNOMIAL_CRC15_ANSI
 *  @return Calculated 16bit CRC value
 */
uint16_t crc16_calc(uint8_t *data, uint16_t data_length, uint16_t polynomial);

#ifdef __cplusplus
}
#endif
#endif /* _NS_CRC_H_ */
