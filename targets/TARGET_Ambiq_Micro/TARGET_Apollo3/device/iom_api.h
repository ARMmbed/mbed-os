/*
Copyright (c) 2020 SparkFun Electronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MBED_IOM_API_H
#define MBED_IOM_API_H

#include "hal/dma_api.h"
#include "hal/buffer.h"

#include "am_mcu_apollo.h"

#include "objects_iom.h"

/** Asynch IOM HAL structure
 */
typedef struct {
    struct iom_s iom;        /**< Target specific SPI structure */
    // struct buffer_s tx_buff; /**< Tx buffer */
    // struct buffer_s rx_buff; /**< Rx buffer */
} iom_t;

void iom_init(iom_t* obj);
void iom_deinit(iom_t* obj);

#endif // MBED_IOM_API_H
