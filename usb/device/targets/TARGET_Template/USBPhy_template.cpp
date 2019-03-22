/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#include "USBPhyHw.h"

static USBPhyHw *instance;

USBPhy *get_usb_phy()
{
    static USBPhyHw usbphy;
    return &usbphy;
}

USBPhyHw::USBPhyHw(): events(NULL)
{

}

USBPhyHw::~USBPhyHw()
{

}

void USBPhyHw::init(USBPhyEvents *events)
{
    this->events = events;

    // Disable IRQ
    NVIC_DisableIRQ(USB_IRQn);

    // TODO -  Setup clocks

    // TODO - Enable USB module

    // Enable IRQ
    NVIC_SetVector(USB_IRQn, (uint32_t)&_usbisr);
    NVIC_EnableIRQ(USB_IRQn);
}

void USBPhyHw::deinit()
{
    // Disconnect and disable interrupt
    disconnect();
    NVIC_DisableIRQ(USB_IRQn);
}

bool USBPhyHw::powered()
{
    // TODO - return true if powered false otherwise. Devices which don't support
    //    this should always return true
    return true;
}

void USBPhyHw::connect()
{
    // TODO - Enable endpoint interrupts

    // TODO - Enable pullup on D+
}

void USBPhyHw::disconnect()
{
    // TODO - Disable all endpoints

    // TODO - Clear all endpoint interrupts

    // TODO - Disable pullup on D+
}

void USBPhyHw::configure()
{
    // TODO - set device to configured. Most device will not need this
}

void USBPhyHw::unconfigure()
{
    // TODO - set device to unconfigured. Most device will not need this
}

void USBPhyHw::sof_enable()
{
    // TODO - Enable SOF interrupt
}

void USBPhyHw::sof_disable()
{
    // TODO - Disable SOF interrupt
}

void USBPhyHw::set_address(uint8_t address)
{
    // TODO - set the device address. Address must take effect
    //        after the status phase of the current transfer
}

void USBPhyHw::remote_wakeup()
{
    // TODO - Sent remote wakeup over USB lines (if supported)
}

const usb_ep_table_t *USBPhyHw::endpoint_table()
{
    // TODO - Update the endpoint table for what your device supports

    static const usb_ep_table_t template_table = {
        4096 - 32 * 4, // 32 words for endpoint buffers
        // +3 based added to interrupt and isochronous to ensure enough
        // space for 4 byte alignment
        {
            {USB_EP_ATTR_ALLOW_CTRL | USB_EP_ATTR_DIR_IN_AND_OUT, 1, 0},
            {USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT,  1, 3},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_DIR_IN_AND_OUT, 2, 0},
            {USB_EP_ATTR_ALLOW_ISO | USB_EP_ATTR_DIR_IN_AND_OUT,  1, 3},
            {USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT,  1, 3},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_DIR_IN_AND_OUT, 2, 0},
            {USB_EP_ATTR_ALLOW_ISO | USB_EP_ATTR_DIR_IN_AND_OUT,  1, 3},
            {USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT,  1, 3},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_DIR_IN_AND_OUT, 2, 0},
            {USB_EP_ATTR_ALLOW_ISO | USB_EP_ATTR_DIR_IN_AND_OUT,  1, 3},
            {USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT,  1, 3},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_DIR_IN_AND_OUT, 2, 0},
            {USB_EP_ATTR_ALLOW_ISO | USB_EP_ATTR_DIR_IN_AND_OUT,  1, 3},
            {USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT,  1, 3},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_DIR_IN_AND_OUT, 2, 0},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_DIR_IN_AND_OUT, 2, 0}
        }
    };
    return &lpc_table;
}

uint32_t USBPhyHw::ep0_set_max_packet(uint32_t max_packet)
{
    // TODO - set endpoint 0 size and return this size
    return 64;
}

// read setup packet
void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size)
{
    // TODO - read up to size bytes of the setup packet
}

void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
    // TODO - setup data buffer to receive next endpoint 0 OUT packet
}

uint32_t USBPhyHw::ep0_read_result()
{
    // TODO - return the size of the last OUT packet received on endpoint 0
    return 0;
}

void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
    // TODO - start transferring buffer on endpoint 0 IN
}

void USBPhyHw::ep0_stall()
{
    // TODO - protocol stall endpoint 0. This stall must be automatically
    //        cleared by the next setup packet
}

bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type)
{
    // TODO - enable this endpoint

    return true;
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    // TODO - disable and remove this endpoint
}

void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    // TODO - stall this endpoint until it is explicitly cleared
}

void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    // TODO - unstall this endpoint
}

bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    // TODO - setup data buffer to receive next endpoint OUT packet and return true if successful
    return true;
}

uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    // TODO - return the size of the last OUT packet received on endpoint
}

bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    // TODO - start transferring buffer on endpoint IN

    return true;
}

void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
    // TODO - stop the current transfer on this endpoint and don't call the IN or OUT callback
}

void USBPhyHw::process()
{
    // TODO - update register for your mcu

    uint8_t stat = USB0->STAT;

    USB0->STAT = stat; // Clear pending interrupts

    // reset interrupt
    if (stat & USB_STAT_RESET_MASK) {

        // TODO - disable all endpoints

        // TODO - clear all endpoint interrupts

        // TODO - enable control endpoint

        // reset bus for USBDevice layer
        events->reset();

        // Re-enable interrupt
        NVIC_ClearPendingIRQ(USB_IRQn);
        NVIC_EnableIRQ(USB_IRQn);
        return;
    }

    // power applied
    if (stat & USB_STAT_POWERED) {
        events->powered(true);
    }

    // power lost
    if (stat & USB_STAT_UNPOWERED) {
        events->powered(false);
    }

    // sleep interrupt
    if (stat & USB_STAT_SUSPEND_MASK) {
        events->suspend(true);
    }

    // resume interrupt
    if (stat & USB_STAT_RESUME_MASK) {
        events->suspend(false);
    }

    // sof interrupt
    if (stat & USB_STAT_SOF_MASK) {
        // SOF event, read frame number
        events->sof(USB0->FRAME);
    }

    // endpoint interrupt
    if (stat & USB_STAT_EP_MASK) {
        uint32_t ep_pending = USB->EP;

        // TODO - call endpoint 0 IN callback if pending
        events->ep0_in();

        // TODO - call endpoint 0 OUT callback if pending
        events->ep0_out();

        // TODO - call endpoint 0 SETUP callback if pending
        events->ep0_setup();

        for (int i = 0; i < 16; i++) {
            // TODO - call endpoint i IN callback if pending
            events->in(0x80 | i);
        }

        for (int i = 0; i < 16; i++) {
            // TODO - call endpoint i OUT callback if pending
            events->out();
        }

        USB->EP = ep_pending; // clear pending
    }

    // Re-enable interrupt
    NVIC_ClearPendingIRQ(USB_IRQn);
    NVIC_EnableIRQ(USB_IRQn);
}

void USBPhyHw::_usbisr(void)
{
    NVIC_DisableIRQ(USB_IRQn);
    instance->events->start_process();
}
