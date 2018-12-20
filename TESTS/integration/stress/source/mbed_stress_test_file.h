/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**  Initialize and format file system
 *
 * Get default Block Device and File system instances
 * Initialize and format file system
*/
void mbed_stress_test_format_file(void);

/** Open and write file
 *
 * Writes datafile from memory into a filesystem datafile with specified block size
 */
void mbed_stress_test_write_file(const char *file, size_t offset, const unsigned char *data, size_t data_length, size_t buffer_size);

/** Open and compare file
 *
 * Open previously written datafile from filesystem and read with specified block size
 * Compare that file from filesystem equals to file loaded in memory
 */
void mbed_stress_test_compare_file(const char *file, size_t offset, const unsigned char *data, size_t data_length, size_t buffer_size);
