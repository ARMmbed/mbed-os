/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#ifndef INCLUDED_WWD_BUFFER_H_
#define INCLUDED_WWD_BUFFER_H_


#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 *             Constants
 ******************************************************/

/******************************************************
 *             Structures
 ******************************************************/

typedef struct
{
    uint16_t size;
    uint16_t offset;
    uint8_t *ptr;
} wiced_buffer_impl_t;

typedef wiced_buffer_impl_t *wiced_buffer_t;

typedef void wiced_buffer_fifo_t;

/******************************************************
 *             Function declarations
 ******************************************************/

/******************************************************
 *             Global variables
 ******************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WWD_BUFFER_H_ */
