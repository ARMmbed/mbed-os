/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#include "cy_usb_phy_hw.h"
#include "mbed_assert.h"
#include "mbed_power_mgmt.h"

#if defined(DEVICE_USBDEVICE)

#define USB_EP_ATTR_DATA_EP             (USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_ALLOW_ISO)
#define USB_EP_ATTR_NON_ISO             (USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT)
#define USB_DEV_EP_NON_ISOC_MAX_PACKET  (64U)
#define USB_DEV_EP_ISOC_MAX_PACKET      (128U)
#define USB_DEV_IN_DIR                  (0x80U)

// Converts endpoint to the bit mask
#define USB_DEV_EP_BIT(endpoint)         (1U << CYHAL_USB_DEV_GET_EP_IDX(endpoint))

// Callbacks to notify stack about event occurrence
static void usb_dev_bus_reset_callback(void);
static void usb_dev_ep0_setup_callback(void);
static void usb_dev_ep0_in_callback(void);
static void usb_dev_ep0_out_callback(void);
static void usb_dev_sof_callback(uint32_t frame_number);
static void usb_dev_endpoint_callback(cyhal_usb_dev_ep_t endpoint);

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
    // Get access to the CYHAL USB object
    cyhal_usb_dev_t *hal_obj = &obj;

    // Disable IRQ
    cyhal_usb_dev_irq_enable(hal_obj, false);

    // Initialize instance to access class data
    instance = this;

    if (this->events == NULL) {
        sleep_manager_lock_deep_sleep();
    }

    // Store events
    instance->events = events;

    // Initialize USB Device (CYHAL find required resources).
    cy_rslt_t ret = cyhal_usb_dev_init(hal_obj, USBDP, USBDM, NULL);
    MBED_ASSERT(CY_RSLT_SUCCESS == ret);

    // Hook device handlers to be called by driver
    cyhal_usb_dev_register_event_callback(hal_obj, CYHAL_USB_DEV_EVENT_BUS_RESET, &usb_dev_bus_reset_callback);
    cyhal_usb_dev_register_event_callback(hal_obj, CYHAL_USB_DEV_EVENT_EP0_SETUP, &usb_dev_ep0_setup_callback);
    cyhal_usb_dev_register_event_callback(hal_obj, CYHAL_USB_DEV_EVENT_EP0_IN,    &usb_dev_ep0_in_callback);
    cyhal_usb_dev_register_event_callback(hal_obj, CYHAL_USB_DEV_EVENT_EP0_OUT,   &usb_dev_ep0_out_callback);

    // Hook SOF callback
    cyhal_usb_dev_register_sof_callback(hal_obj, &usb_dev_sof_callback);

    // Divide hardware buffer (512 bytes) statically accordingly to the lpc_table
    // Only 6 endpoint out of 8 (supported by hardware) are supported because of buffer space limitation
    (void) cyhal_usb_dev_endpoint_add(hal_obj, true, false, 1U, USB_DEV_EP_NON_ISOC_MAX_PACKET, CYHAL_USB_DEV_EP_TYPE_BULK);
    (void) cyhal_usb_dev_endpoint_add(hal_obj, true, false, 2U, USB_DEV_EP_NON_ISOC_MAX_PACKET, CYHAL_USB_DEV_EP_TYPE_BULK);
    (void) cyhal_usb_dev_endpoint_add(hal_obj, true, false, 3U, USB_DEV_EP_NON_ISOC_MAX_PACKET, CYHAL_USB_DEV_EP_TYPE_BULK);
    (void) cyhal_usb_dev_endpoint_add(hal_obj, true, false, 4U, USB_DEV_EP_NON_ISOC_MAX_PACKET, CYHAL_USB_DEV_EP_TYPE_BULK);
    (void) cyhal_usb_dev_endpoint_add(hal_obj, true, false, 5U, USB_DEV_EP_ISOC_MAX_PACKET,     CYHAL_USB_DEV_EP_TYPE_ISO);
    (void) cyhal_usb_dev_endpoint_add(hal_obj, true, false, 6U, USB_DEV_EP_ISOC_MAX_PACKET,     CYHAL_USB_DEV_EP_TYPE_ISO);

    // Clear IN and OUT data endpoint event masks
    in_event_mask  = 0;
    out_event_mask = 0;

    // Configure interrupt and hook interrupt handler
    cyhal_usb_dev_register_irq_callback(hal_obj, (cyhal_usb_dev_irq_callback_t)&_usbisr);
    cyhal_usb_dev_irq_enable(hal_obj, true);
}

static void usb_dev_bus_reset_callback(void)
{
    instance->events->reset();
}

static void usb_dev_ep0_setup_callback(void)
{
    // Process endpoint 0 SETUP packet (Setup stages)
    instance->events->ep0_setup();
}

static void usb_dev_ep0_in_callback(void)
{
    // Process endpoint 0 IN packet (Data stage)
    instance->events->ep0_in();
}

static void usb_dev_ep0_out_callback(void)
{
    // Process endpoint 0 OUT packet (Data stages)
    instance->events->ep0_out();
}

static void usb_dev_sof_callback(uint32_t frame_number)
{
    // Calls SOF packet callback
    instance->events->sof(frame_number);
}

static void usb_dev_endpoint_callback(cyhal_usb_dev_ep_t endpoint)
{
    if (CYHAL_USB_DEV_IS_IN_EP(endpoint)) {
        instance->in_event_mask  |= USB_DEV_EP_BIT(endpoint);
    } else {
        instance->out_event_mask |= USB_DEV_EP_BIT(endpoint);
    }
}

void USBPhyHw::usb_dev_execute_ep_callbacks(void)
{
    if ((0 != in_event_mask) || (0 != out_event_mask)) {
        uint32_t mask;
        uint32 endpoint;

        // USBPhy must send IN endpoint events before OUT endpoint events if both are pending.

        // Notify IN endpoint transfer complete events
        endpoint = 1U;
        mask = (uint32_t) in_event_mask;
        while (0 != mask) {
            if (0 != (mask & 0x1U)) {
                events->in(USB_DEV_IN_DIR | endpoint);
            }

            mask >>= 1U;
            ++endpoint;
        }

        // Notify OUT endpoint transfer complete events
        endpoint = 1U;
        mask = (uint32_t) out_event_mask;
        while (0 != mask) {
            if (0 != (mask & 0x1U)) {
                events->out(endpoint);
            }

            mask >>= 1U;
            ++endpoint;
        }

        out_event_mask = 0;
        in_event_mask  = 0;
    }
}

void USBPhyHw::deinit()
{
    cyhal_usb_dev_free(&obj);

    if (events != NULL) {
        sleep_manager_unlock_deep_sleep();
    }

    events = NULL;
}

bool USBPhyHw::powered()
{
    // Not implemented for this target
    return true;
}

void USBPhyHw::connect()
{
    cyhal_usb_dev_connect(&obj);
}

void USBPhyHw::disconnect()
{
    cyhal_usb_dev_disconnect(&obj);
}

void USBPhyHw::configure()
{
    cyhal_usb_dev_set_configured(&obj);
}

void USBPhyHw::unconfigure()
{
    cyhal_usb_dev_set_unconfigured(&obj);
}

void USBPhyHw::sof_enable()
{
    cyhal_usb_dev_sof_enable(&obj, true);
}

void USBPhyHw::sof_disable()
{
    cyhal_usb_dev_sof_enable(&obj, false);
}

void USBPhyHw::set_address(uint8_t address)
{
    cyhal_usb_dev_set_address(&obj, address);
}

void USBPhyHw::remote_wakeup()
{
    // Not implemented for this target
}

void USBPhyHw::suspend(bool suspended)
{
    // Not implemented for this target
    (void) suspended;
}

const usb_ep_table_t *USBPhyHw::endpoint_table()
{
    static const usb_ep_table_t lpc_table = {
        512,
        // CY USB IP has hardware buffer of 512 bytes that is shared among 8 data endpoint.
        // The buffer has static allocation as follows:
        // - 4 endpoints of 64 that supports BULK and INT.
        // - 2 endpoints of 128 that support ISOC, BULK and INT.
        // The static allocation of max packet for BULK and INT allows to handle change interface
        // alternates properly if endpoint size is changed (the endpoint is not overlapped with
        // endpoints of neighborhood interface).
        // The CY USB IP has separate endpoint 0 hardware buffer of 8 bytes.
        {
            {USB_EP_ATTR_ALLOW_CTRL | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_NON_ISO    | USB_EP_ATTR_DIR_IN_OR_OUT,  0, 0},
            {USB_EP_ATTR_NON_ISO    | USB_EP_ATTR_DIR_IN_OR_OUT,  0, 0},
            {USB_EP_ATTR_NON_ISO    | USB_EP_ATTR_DIR_IN_OR_OUT,  0, 0},
            {USB_EP_ATTR_NON_ISO    | USB_EP_ATTR_DIR_IN_OR_OUT,  0, 0},
            {USB_EP_ATTR_DATA_EP    | USB_EP_ATTR_DIR_IN_OR_OUT,  0, 0},
            {USB_EP_ATTR_DATA_EP    | USB_EP_ATTR_DIR_IN_OR_OUT,  0, 0},
            {0, 0, 0},
            {0, 0, 0},
        }
    };

    return &lpc_table;
}

uint32_t USBPhyHw::ep0_set_max_packet(uint32_t max_packet)
{
    // Ignore max packet because endpoint 0 has dedicated hardware buffer
    (void) max_packet;

    return cyhal_usb_dev_ep0_get_max_packet(&obj);
}

void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size)
{
    cyhal_usb_dev_ep0_setup_read_result(&obj, buffer, size);
}

void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
    cyhal_usb_dev_ep0_read(&obj, data, size);
}

uint32_t USBPhyHw::ep0_read_result()
{
    return cyhal_usb_dev_ep0_read_result(&obj);
}

void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
    cyhal_usb_dev_ep0_write(&obj, buffer, size);
}

void USBPhyHw::ep0_stall()
{
    cyhal_usb_dev_ep0_stall(&obj);
}

bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type)
{
    bool result = false;

    // The endpoint type is USB spec defined therefore it is safe to cast into the different type used for the same
    result = (CY_RSLT_SUCCESS == cyhal_usb_dev_endpoint_add(&obj, false, true, endpoint, max_packet, (cyhal_usb_dev_ep_type_t) type));

    if (result) {
        // Hook endpoint callback to get a notification about transfer completion events
        cyhal_usb_dev_register_endpoint_callback(&obj, endpoint, &usb_dev_endpoint_callback);
    }

    return result;
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    (void) cyhal_usb_dev_endpoint_remove(&obj, endpoint);

    // Clear endpoint masks
    in_event_mask  &= ~USB_DEV_EP_BIT(endpoint);
    out_event_mask &= ~USB_DEV_EP_BIT(endpoint);
}

void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    cy_rslt_t ret = cyhal_usb_dev_endpoint_stall(&obj, endpoint);
    MBED_ASSERT(CY_RSLT_SUCCESS == ret);
}

void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    cy_rslt_t ret = cyhal_usb_dev_endpoint_unstall(&obj, endpoint);
    MBED_ASSERT(CY_RSLT_SUCCESS == ret);
}

bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    return (CY_RSLT_SUCCESS == cyhal_usb_dev_endpoint_read(&obj, endpoint, data, size));
}

uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    uint32_t actSize = 0;

    // The read result return actual size zero in case of failure
    cy_rslt_t ret = cyhal_usb_dev_endpoint_read_result(&obj, endpoint, &actSize);
    MBED_ASSERT(CY_RSLT_SUCCESS == ret);

    return actSize;
}

bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    return (CY_RSLT_SUCCESS == cyhal_usb_dev_endpoint_write(&obj, endpoint, data, size));
}

void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
    cy_rslt_t ret = cyhal_usb_dev_endpoint_abort(&obj, endpoint);
    MBED_ASSERT(CY_RSLT_SUCCESS == ret);
}

void USBPhyHw::process()
{
    // Process interrupt
    cyhal_usb_dev_process_irq(&obj);

    // Calls data endpoint IN and OUT event in the required order
    usb_dev_execute_ep_callbacks();

    // Enable interrupt after interrupt processing completed
    cyhal_usb_dev_irq_enable(&obj, true);
}

void USBPhyHw::_usbisr(void)
{
    cyhal_usb_dev_t *hal_obj = &(instance->obj);

    // Disable interrupt before start interrupt processing
    cyhal_usb_dev_irq_enable(hal_obj, false);
    instance->events->start_process();
}

#endif /* (DEVICE_USBDEVICE) */
