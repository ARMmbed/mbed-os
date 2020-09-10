/* 
 * Copyright (c) 2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef APOLLO3_CORDIO_HCI_TRANSPORT_DRIVER_H_
#define APOLLO3_CORDIO_HCI_TRANSPORT_DRIVER_H_

#include "CordioHCITransportDriver.h"

#define AP3_STUPID_DEF_OF_BLECIRQ_BIT 0x00000080 // AM_BLEIF_INT_BLECIRQ

namespace ble
{
    class AP3CordioHCITransportDriver : public CordioHCITransportDriver
    {
    public:
        //AP3CordioHCITransportDriver(/* specific constructor arguments*/);

        virtual ~AP3CordioHCITransportDriver();

        virtual void initialize();

        virtual void terminate();

        virtual uint16_t write(uint8_t packet_type, uint16_t len, uint8_t *data);

        void *handle;

    private:
        // private driver declarations
    };
} // namespace ble

extern "C" void CordioHCITransportDriver_on_data_received(uint8_t *data, uint16_t len);

#endif /* APOLLO3_CORDIO_HCI_TRANSPORT_DRIVER_H_ */
