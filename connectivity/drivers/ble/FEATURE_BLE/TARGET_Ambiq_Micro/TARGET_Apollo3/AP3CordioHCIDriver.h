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

#ifndef APOLLO3_CORDIO_HCI_DRIVER_H_
#define APOLLO3_CORDIO_HCI_DRIVER_H_

#include "CordioHCIDriver.h"
#include "am_mcu_apollo.h"

namespace ble
{
    class AP3CordioHCIDriver : public CordioHCIDriver
    {
    public:
        AP3CordioHCIDriver(
            CordioHCITransportDriver &transport_driver
            /* specific constructor arguments*/);

        virtual ~AP3CordioHCIDriver();

        virtual void do_initialize();

        virtual void do_terminate();

        virtual ble::buf_pool_desc_t get_buffer_pool_description();

    private:
        void **_ptr_to_handle;
        am_hal_ble_config_t _ble_config;
    };
} // namespace ble

#endif /* APOLLO3_CORDIO_HCI_TRANSPORT_DRIVER_H_ */
