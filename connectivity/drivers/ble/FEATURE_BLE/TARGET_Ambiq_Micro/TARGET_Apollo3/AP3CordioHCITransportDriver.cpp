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

#define PRINT_DEBUG_HCI 0

using namespace ble;

#ifndef USE_AMBIQ_DRIVER
static uint8_t ample_buffer[256];
void *ble_handle = NULL;
#endif

AP3CordioHCITransportDriver::~AP3CordioHCITransportDriver() {}

void AP3CordioHCITransportDriver::initialize()
{
#ifdef USE_AMBIQ_DRIVER
    wsfHandlerId_t handlerId = WsfOsSetNextHandler(HciDrvHandler);
    HciDrvHandlerInit(handlerId);
#else
    am_hal_ble_initialize(0, &handle);
    ble_handle = handle;
#endif
}

void AP3CordioHCITransportDriver::terminate()
{
#ifdef USE_AMBIQ_DRIVER
#else
    am_hal_ble_deinitialize(handle);
    handle = NULL;
    ble_handle = NULL;
#endif
}

uint16_t AP3CordioHCITransportDriver::write(uint8_t packet_type, uint16_t len, uint8_t *data)
{
#if PRINT_DEBUG_HCI
    printf("sent tx packet_type: %02X data: ", packet_type);
    for (int i = 0; i < len; i++)
    {
        printf(" %02X", data[i]);
    }
    printf("\r\n");
#endif

    //Temporary workaround, random address not working, suppress it.
    if (data[0] == 0x06 && data[1] == 0x20)
    {
#if PRINT_DEBUG_HCI
        printf("LE Set Advertising Params\r\n");
#endif
        data[8] = 0;
    }

    uint16_t retLen = 0;
#ifdef USE_AMBIQ_DRIVER
    retLen = ap3_hciDrvWrite(packet_type, len, data);
#else
    if (handle)
    {
        uint16_t retVal = (uint16_t)am_hal_ble_blocking_hci_write(handle, packet_type, (uint32_t *)data, (uint16_t)len);
        if (retVal == AM_HAL_STATUS_SUCCESS)
        {
            retLen = len;
        }
    }
#endif

#if CORDIO_ZERO_COPY_HCI
    WsfMsgFree(data);
#endif

    return retLen;
}

#ifdef USE_AMBIQ_DRIVER
//Ugly Mutlifile implementation
void CordioHCITransportDriver_on_data_received(uint8_t *data, uint16_t len)
{
#if PRINT_DEBUG_HCI
    printf("data rx: ");
    for (int i = 0; i < len; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\r\n");
#endif
    CordioHCITransportDriver::on_data_received(data, len);
}
#else
extern "C" void HciDrvIntService(void)
{
    uint32_t status = am_hal_ble_int_status(ble_handle, false);
    if (status & AM_HAL_BLE_INT_BLECIRQ)
    {
        uint32_t len = 0;
        am_hal_ble_blocking_hci_read(ble_handle, (uint32_t *)ample_buffer, &len);
        CordioHCITransportDriver::on_data_received(ample_buffer, len);
    }
    am_hal_ble_int_clear(ble_handle, 0xFFFF);
}
#endif
