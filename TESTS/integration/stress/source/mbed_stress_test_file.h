/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

void mbed_stress_test_format_file(void);

void mbed_stress_test_write_file(const char* file, size_t offset, const unsigned char* data, size_t data_length, size_t buffer_size);

void mbed_stress_test_compare_file(const char* file, size_t offset, const unsigned char* data, size_t data_length, size_t buffer_size);
