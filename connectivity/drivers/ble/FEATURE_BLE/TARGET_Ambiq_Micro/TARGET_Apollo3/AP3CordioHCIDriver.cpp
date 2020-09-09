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

#include "AP3CordioHCIDriver.h"
#include "AP3CordioHCITransportDriver.h"
#include "am_mcu_apollo.h"
#include "stdio.h"
#include <cstring>

#include "wsf_types.h"
#include "wsf_timer.h"
#include "bstream.h"
#include "wsf_msg.h"
#include "wsf_cs.h"

#include "hci_drv_apollo3.h"

using namespace ble;

AP3CordioHCIDriver::AP3CordioHCIDriver(CordioHCITransportDriver &transport_driver)
    : CordioHCIDriver(transport_driver)
{
    AP3CordioHCITransportDriver *p_trspt_drv = (AP3CordioHCITransportDriver *)&transport_driver;
    _ptr_to_handle = &p_trspt_drv->handle;
}

AP3CordioHCIDriver::~AP3CordioHCIDriver() {}

void AP3CordioHCIDriver::do_initialize()
{
#ifdef USE_AMBIQ_DRIVER
    HciDrvRadioBoot(true);
#else
    MBED_ASSERT(*_ptr_to_handle);
    _ble_config = am_hal_ble_default_config;
    MBED_ASSERT(am_hal_ble_power_control(*_ptr_to_handle, AM_HAL_BLE_POWER_ACTIVE) == AM_HAL_STATUS_SUCCESS);
    MBED_ASSERT(am_hal_ble_config(*_ptr_to_handle, &_ble_config) == AM_HAL_STATUS_SUCCESS);
    MBED_ASSERT(am_hal_ble_boot(*_ptr_to_handle) == AM_HAL_STATUS_SUCCESS);
    MBED_ASSERT(am_hal_ble_tx_power_set(*_ptr_to_handle, 0x0F) == AM_HAL_STATUS_SUCCESS);
    MBED_ASSERT(am_hal_ble_sleep_set(*_ptr_to_handle, false) == AM_HAL_STATUS_SUCCESS);
    am_hal_ble_int_enable(*_ptr_to_handle, (AP3_STUPID_DEF_OF_BLECIRQ_BIT | AM_HAL_BLE_INT_ICMD | AM_HAL_BLE_INT_BLECSSTAT));
    NVIC_EnableIRQ(BLE_IRQn);
#endif
}
void AP3CordioHCIDriver::do_terminate()
{
#ifdef USE_AMBIQ_DRIVER
    HciDrvRadioShutdown();
#else
    am_hal_ble_power_control(*_ptr_to_handle, AM_HAL_BLE_POWER_OFF);
#endif
}

ble::buf_pool_desc_t AP3CordioHCIDriver::get_buffer_pool_description()
{
    static union {
        uint8_t buffer[9000];
        uint64_t align;
    };
    static const wsfBufPoolDesc_t pool_desc[] = {
        {16, 64},
        {32, 64},
        {64, 32},
        {128, 16},
        {272, 4}};
    return buf_pool_desc_t(buffer, pool_desc);
}

ble::CordioHCIDriver &ble_cordio_get_hci_driver()
{
    static AP3CordioHCITransportDriver transport_driver;

    static AP3CordioHCIDriver hci_driver(transport_driver);

    return hci_driver;
}
