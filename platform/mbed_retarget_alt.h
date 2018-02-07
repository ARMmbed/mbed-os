/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
 *
 * Alternative (custom) stdio retarget interface declaration
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
 *
 */

#ifndef RETARGET_ALT_H
#define RETARGET_ALT_H

#if __cplusplus
extern "C" {
#endif
    /** Alternative stdio initialization interface function
     *
     * @param fh    The file handle of the stdio operation
     *              (0 = stdin, 1 = stdout, 2 = stderr)
     */
    void retarget_stdio_init_alt(unsigned int fh);
    
    /** Alternative stdio write interface function
     *
     * @param fh       The file handle of the stdio operation
     * @param buffer   The buffer containing the data to write
     * @param length   The length of the valid data in the buffer
     *
     * @return the number of bytes written or negative errno on failure
     *         (e.g. -EIO from errno.h)
     */
    int retarget_stdio_write_alt(unsigned int fh, const unsigned char *buffer, unsigned int length);
    
    /** Alternative stdio read interface function
     *
     *  This shall be a blocking function not returning before at least one
     *  character has been read.
     *
     * @param fh       The file handle of the stdio operation
     * @param buffer   The buffer for the read data
     * @param length   The size of the buffer
     *
     * @return the number of bytes read or negative errno on failure
     *         (e.g. -EIO from errno.h)
     */
    int retarget_stdio_read_alt(unsigned int fh, unsigned char *buffer, unsigned int length);
#if __cplusplus
}
#endif

#endif /* RETARGET_ALT_H */
