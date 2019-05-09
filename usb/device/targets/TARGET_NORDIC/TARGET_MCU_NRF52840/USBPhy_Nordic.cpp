/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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

#include "platform/mbed_critical.h"
#include "platform/mbed_assert.h"

#define MAX_PACKET_SIZE_SETUP NRF_DRV_USBD_EPSIZE
#define MAX_PACKET_NON_ISO    NRF_DRV_USBD_EPSIZE
#define MAX_PACKET_ISO        NRF_DRV_USBD_ISOSIZE
#define ENDPOINT_NON_ISO      (USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT)

#define IS_IN_EP(ep)    (ep & 0x80)  // Checks if the given endpoint is an IN endpoint (MSB set)
#define IS_OUT_EP(ep)   (ep & ~0x80) // Checks if the given endpoint is an OUT endpoint (MSB clear)

// If this bit is set in setup.bmRequestType, the setup transfer
// is DEVICE->HOST (IN transfer)
// if it is clear, the transfer is HOST->DEVICE (OUT transfer)
#define SETUP_TRANSFER_DIR_MASK 0x80

// Debugging flag for tracking USB events
#define USBD_DEBUG 0

// Nordic USBD driver IRQ handler
extern "C" void USBD_IRQHandler(void);

// Internal USBD driver IRQ handler
void USBD_HAL_IRQHandler(void);

static USBPhyHw *instance = 0;

static volatile bool virtual_status_xfer_event;

static void usbd_event_handler(nrf_drv_usbd_evt_t const *const p_event);
static void power_usb_event_handler(nrfx_power_usb_evt_t event);

USBPhy *get_usb_phy()
{
    static USBPhyHw usbphy;
    return &usbphy;
}

USBPhyHw::USBPhyHw() :
    events(NULL), sof_enabled(false), connect_enabled(false),
    usb_event_type(USB_HW_EVENT_NONE),
    usb_power_event(NRFX_POWER_USB_EVT_REMOVED)
{

}

USBPhyHw::~USBPhyHw()
{

}

void USBPhyHw::init(USBPhyEvents *events)
{

    // Disable the USBD interrupts
    // Interrupts will be reenabled by the Nordic driver
    NRFX_IRQ_DISABLE(USBD_IRQn);

    if (this->events == NULL) {
        sleep_manager_lock_deep_sleep();
    }

    this->events = events;

    ret_code_t ret;

    // Initialize power module to track USB Power events
    ret = nrfx_power_init(NULL);
    MBED_ASSERT(ret == NRF_SUCCESS);


    // Register callback for USB Power events
    static const nrfx_power_usbevt_config_t config = {
        .handler = power_usb_event_handler
    };

    nrfx_power_usbevt_init(&config);

    // Initialize USB Device driver
    ret = nrf_drv_usbd_init(usbd_event_handler);
    MBED_ASSERT(ret == NRF_SUCCESS);

    /* Configure selected size of the packed on EP0 */
    nrf_drv_usbd_ep_max_packet_size_set(NRF_DRV_USBD_EPOUT0, MAX_PACKET_SIZE_SETUP);
    nrf_drv_usbd_ep_max_packet_size_set(NRF_DRV_USBD_EPIN0, MAX_PACKET_SIZE_SETUP);

    // Store a reference to this instance
    instance = this;

    virtual_status_xfer_event = false;

    /*
     * Configure ISOIN endpoint to respond with ZLP when
     * no data is ready to be sent
     */
    NRF_USBD->ISOINCONFIG |= 0x01; // set RESPONSE to 1 (respond with ZLP)

    // Set up the IRQ handler
    NVIC_SetVector(USBD_IRQn, (uint32_t)USBD_HAL_IRQHandler);

    // Enable the power events
    nrfx_power_usbevt_enable();

}

void USBPhyHw::deinit()
{
    // Disconnect and disable interrupt
    disconnect();

    // Disable the USB Device driver
    ret_code_t ret = nrf_drv_usbd_uninit();
    MBED_ASSERT(ret == NRF_SUCCESS);

    // Disable the power peripheral driver
    nrfx_power_uninit();

    if (this->events != NULL) {
        sleep_manager_unlock_deep_sleep();
    }

    this->events = NULL;

    // Clear the instance pointer
    instance = 0;
}

bool USBPhyHw::powered()
{
    if (nrfx_power_usbstatus_get() == NRFX_POWER_USB_STATE_CONNECTED
            || nrfx_power_usbstatus_get() == NRFX_POWER_USB_STATE_READY) {
        return true;
    } else {
        return false;
    }
}

void USBPhyHw::connect()
{

    // To save power, we only enable the USBD peripheral
    // when there's actually VBUS detected

    // So flag that the USB stack is ready to connect
    this->connect_enabled = true;

    // If VBUS is already available, enable immediately
    if (nrfx_power_usbstatus_get() == NRFX_POWER_USB_STATE_CONNECTED) {
        // Enabling USB will cause NRF_DRV_POWER_USB_EVT_READY
        // to occur, which will start the USBD peripheral
        // when the internal regulator has settled
        if (!nrf_drv_usbd_is_enabled()) {
            nrf_drv_usbd_enable();
        }

        if (nrfx_power_usbstatus_get() == NRFX_POWER_USB_STATE_READY
                && !nrf_drv_usbd_is_started()) {
            nrf_drv_usbd_start(true);
        }
    }
}

void USBPhyHw::disconnect()
{

    this->connect_enabled = false;

    if (nrf_drv_usbd_is_started()) {
        nrf_drv_usbd_stop();
    }
    if (nrf_drv_usbd_is_enabled()) {
        nrf_drv_usbd_disable();
    }
}

void USBPhyHw::configure()
{
    // Not needed
}

void USBPhyHw::unconfigure()
{
    // Remove all endpoints (except control, obviously)
    nrf_drv_usbd_ep_default_config();
}

void USBPhyHw::sof_enable()
{
    // TODO - Enable SOF interrupt
    // Can this safely be done if
    // nrf_drv_usbd_start is called with SoF enabled?
    // For now just mask the interrupt with a boolean flag
    sof_enabled = true;
}

void USBPhyHw::sof_disable()
{
    // TODO - Disable SOF interrupt
    // Can this safely be done if
    // nrf_drv_usbd_start is called with SoF enabled?
    sof_enabled = false;
}

void USBPhyHw::set_address(uint8_t address)
{
    // nothing to do, handled by hardware; but don't STALL
}

void USBPhyHw::remote_wakeup()
{
    // Not supported(?)
}

const usb_ep_table_t *USBPhyHw::endpoint_table()
{

    static const usb_ep_table_t template_table = {
        1536, // 64 bytes per bulk/int endpoint pair (8), 1023 bytes for iso endpoint pair (1)
        {
            { USB_EP_ATTR_ALLOW_CTRL    | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { ENDPOINT_NON_ISO          | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { ENDPOINT_NON_ISO          | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { ENDPOINT_NON_ISO          | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { ENDPOINT_NON_ISO          | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { ENDPOINT_NON_ISO          | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { ENDPOINT_NON_ISO          | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { ENDPOINT_NON_ISO          | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { USB_EP_ATTR_ALLOW_ISO     | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { 0                         | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { 0                         | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { 0                         | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { 0                         | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { 0                         | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { 0                         | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
            { 0                         | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
        }
    };
    return &template_table;
}

uint32_t USBPhyHw::ep0_set_max_packet(uint32_t max_packet)
{
    disable_usb_interrupts();

    if (max_packet > MAX_PACKET_SIZE_SETUP) {
        max_packet = MAX_PACKET_SIZE_SETUP;
    }

    nrf_drv_usbd_ep_max_packet_size_set(NRF_DRV_USBD_EPOUT0, max_packet);
    nrf_drv_usbd_ep_max_packet_size_set(NRF_DRV_USBD_EPIN0, max_packet);

    enable_usb_interrupts();

    return max_packet;
}

// read setup packet
void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size)
{

    disable_usb_interrupts();

    if (size > sizeof(this->setup_buf)) {
        size = sizeof(this->setup_buf);
    }
    memcpy(buffer, &this->setup_buf, size);

    enable_usb_interrupts();
}

void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{

    // Check for status stage
    if (data == NULL && size == 0) {
        // If the data stage transfer direction was OUT
        if (setup_buf.bmRequestType & SETUP_TRANSFER_DIR_MASK) {
            // This is the status stage -- trigger the status task and notify the Mbed stack
            // Don't trigger status stage unless endpoint is not busy!
            // (Causes an undocumented hardware-initiated stall on the control endpoint)
            if (nrf_drv_usbd_ep_is_busy(NRF_DRV_USBD_EPIN0)) {
                nrf_usbd_shorts_enable(NRF_USBD_SHORT_EP0DATADONE_EP0STATUS_MASK);
            } else {
                nrf_usbd_task_trigger(NRF_USBD_TASK_EP0STATUS);
            }

            virtual_status_xfer_event = true;

            // Trigger an interrupt to process the virtual status event
            NRFX_IRQ_PENDING_SET(USBD_IRQn);

            return;
        }
    }

    nrf_drv_usbd_transfer_t *transfer = get_transfer_buffer((usb_ep_t)(NRF_DRV_USBD_EPOUT0));
    memset(transfer, 0, sizeof(nrf_drv_usbd_transfer_t));
    transfer->p_data.rx = data;
    transfer->size = size;

    nrf_drv_usbd_setup_data_clear(); // tell the hardware to receive another OUT packet

    ret_code_t ret = nrf_drv_usbd_ep_transfer(NRF_DRV_USBD_EPOUT0, transfer);
    MBED_ASSERT(ret == NRF_SUCCESS);
}

uint32_t USBPhyHw::ep0_read_result()
{
    return nrf_drv_usbd_epout_size_get(NRF_DRV_USBD_EPOUT0);
}

void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{

    // Check for status stage
    if (buffer == NULL && size == 0) {
        // If the requested size was 0 OR the data stage transfer direction was OUT
        if (setup_buf.wLength == 0
                || ((setup_buf.bmRequestType & SETUP_TRANSFER_DIR_MASK) == 0)) {

            // This is the status stage -- trigger the status task and notify the Mbed stack

            // Don't trigger status stage unless endpoint is not busy!
            // (Causes an undocumented hardware-initiated stall on the control endpoint)
            if (nrf_drv_usbd_ep_is_busy(NRF_DRV_USBD_EPOUT0)) {
                nrf_usbd_shorts_enable(NRF_USBD_SHORT_EP0DATADONE_EP0STATUS_MASK);
            } else {
                nrf_usbd_task_trigger(NRF_USBD_TASK_EP0STATUS);
            }

            virtual_status_xfer_event = true;

            // Trigger an interrupt to process the virtual status event
            NRFX_IRQ_PENDING_SET(USBD_IRQn);

            return;
        }
    }

    nrf_drv_usbd_transfer_t *transfer = get_transfer_buffer(NRF_DRV_USBD_EPIN0);
    memset(transfer, 0, sizeof(nrf_drv_usbd_transfer_t));
    transfer->p_data.tx = buffer;
    transfer->size = size;

    if (size == 0) {
        transfer->flags |= NRF_DRV_USBD_TRANSFER_ZLP_FLAG;
    }

    ret_code_t ret = nrf_drv_usbd_ep_transfer(NRF_DRV_USBD_EPIN0, transfer);
    MBED_ASSERT(ret == NRF_SUCCESS);
}

void USBPhyHw::ep0_stall()
{
    // Note: This stall must be automatically cleared by the next setup packet
    nrf_drv_usbd_setup_stall();
}

bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type)
{
    nrf_drv_usbd_ep_t nrf_ep = get_nordic_endpoint(endpoint);
    nrf_drv_usbd_ep_enable(nrf_ep);
    nrf_drv_usbd_ep_max_packet_size_set(nrf_ep, max_packet);
    return nrf_drv_usbd_ep_enable_check(nrf_ep);
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    nrf_drv_usbd_ep_t nrf_ep = get_nordic_endpoint(endpoint);
    // Reset data toggle for bulk/interrupt endpoints
    if (nrf_ep != NRF_DRV_USBD_EPOUT8 && nrf_ep != NRF_DRV_USBD_EPIN8) {
        nrf_drv_usbd_ep_dtoggle_clear(nrf_ep);
    }

    nrf_drv_usbd_ep_disable(nrf_ep);
}

void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    nrf_drv_usbd_ep_stall(get_nordic_endpoint(endpoint));
}

void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    nrf_drv_usbd_ep_t ep = get_nordic_endpoint(endpoint);

    // Unstall may be called on an endpoint that isn't stalled
    if (nrf_drv_usbd_ep_stall_check(ep)) {
        nrf_drv_usbd_ep_stall_clear(ep);
    }

    // Clear data toggle
    nrf_drv_usbd_ep_dtoggle_clear(ep);

    /*
     * This is a somewhat hacky fix to fully "unload"
     * an IN endpoint after a buffer has been
     * transferred via EasyDMA...
     */

    nrf_drv_usbd_ep_disable(ep);
    nrf_drv_usbd_ep_enable(ep);
}

bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    nrf_drv_usbd_transfer_t *transfer = get_transfer_buffer(endpoint);
    memset(transfer, 0, sizeof(nrf_drv_usbd_transfer_t));
    transfer->p_data.rx = data;
    transfer->size = size;

    ret_code_t ret = nrf_drv_usbd_ep_transfer(get_nordic_endpoint(endpoint), transfer);
    return (ret == NRF_SUCCESS);
}

uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    return nrf_drv_usbd_epout_size_get(get_nordic_endpoint(endpoint));
}

bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    nrf_drv_usbd_transfer_t *transfer = get_transfer_buffer(endpoint);
    memset(transfer, 0, sizeof(nrf_drv_usbd_transfer_t));
    transfer->p_data.tx = data;
    transfer->size = size;

    // If this is a zero-length-packet (ZLP)
    // Set the ZLP flag
    if (size == 0) {
        transfer->flags |= NRF_DRV_USBD_TRANSFER_ZLP_FLAG;
    }

    ret_code_t ret = nrf_drv_usbd_ep_transfer(get_nordic_endpoint(endpoint), transfer);
    return (ret == NRF_SUCCESS);
}

void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
    nrf_drv_usbd_ep_abort(get_nordic_endpoint(endpoint));
}

void USBPhyHw::process()
{

    if (usb_event_type == USB_HW_EVENT_USBD) {

        // Process regular USBD events
        switch (usb_event.type) {
            case NRF_DRV_USBD_EVT_SUSPEND:
                events->suspend(true);
                break;
            case NRF_DRV_USBD_EVT_RESUME:
                events->suspend(false);
                break;
            case NRF_DRV_USBD_EVT_WUREQ:
                break;
            case NRF_DRV_USBD_EVT_RESET:
                this->_reset();
                events->reset();
                break;
            case NRF_DRV_USBD_EVT_SOF:
                if (sof_enabled) {
                    events->sof(usb_event.data.sof.framecnt);
                }
                break;
            case NRF_DRV_USBD_EVT_EPTRANSFER:
                if (usb_event.data.eptransfer.status == NRF_USBD_EP_OK) {
                    if (!nrf_drv_usbd_ep_stall_check(usb_event.data.eptransfer.ep)) {
                        if (IS_IN_EP(usb_event.data.eptransfer.ep)) {
                            if ((usb_event.data.eptransfer.ep & 0x7F) == 0) {
                                events->ep0_in();
                            } else {
                                events->in((usb_ep_t) usb_event.data.eptransfer.ep);
                            }
                        } else {
                            if ((usb_event.data.eptransfer.ep & 0x7F) == 0) {
                                events->ep0_out();
                            } else {
                                events->out((usb_ep_t) usb_event.data.eptransfer.ep);
                            }
                        }
                    }
                }
                break;
            case NRF_DRV_USBD_EVT_SETUP: {
                nrf_drv_usbd_ep_stall_clear(NRF_DRV_USBD_EPIN0);
                nrf_drv_usbd_ep_stall_clear(NRF_DRV_USBD_EPOUT0);

                // Copy the setup packet into the internal buffer
                nrf_drv_usbd_setup_get(&setup_buf);

                // Notify the Mbed stack
                events->ep0_setup();
            }
            break;
            default:
                break;

        }
    } else if (usb_event_type == USB_HW_EVENT_POWER) {
        // Process USB power-related events
        switch (usb_power_event) {
            case NRFX_POWER_USB_EVT_DETECTED:
                if (this->connect_enabled) {
                    if (!nrf_drv_usbd_is_enabled()) {
                        nrf_drv_usbd_enable();
                    }
                    events->power(true);
                }
                break;
            case NRFX_POWER_USB_EVT_REMOVED:
                events->power(false);
                break;
            case NRFX_POWER_USB_EVT_READY:
                if (!nrf_drv_usbd_is_started()) {
                    nrf_drv_usbd_start(true);
                }
                break;
            default:
                ASSERT(false);
        }
    } else if (usb_event_type == USB_HW_EVENT_VIRTUAL_STATUS) {
        // Notify Mbed stack of status stage transfer completion
        if (setup_buf.bmRequestType & SETUP_TRANSFER_DIR_MASK) { // DATA IN transfer, Status OUT transfer
            events->ep0_out();
        } else {                                                 // DATA OUT transfer, Status IN transfer
            events->ep0_in();
        }
    }

    // Unflag the event type
    usb_event_type = USB_HW_EVENT_NONE;

    // Re-enable interrupt
    enable_usb_interrupts();
}

void USBPhyHw::_usb_event_handler(
    nrf_drv_usbd_evt_t const *const p_event)
{
    disable_usb_interrupts();
    // Copy the event data into internal memory
    memcpy(&instance->usb_event, p_event, sizeof(instance->usb_event));
    // Tell the upper layers of the stack to process the event
    instance->usb_event_type = USB_HW_EVENT_USBD;
    instance->events->start_process();
}

void USBPhyHw::_usb_power_event_handler(nrfx_power_usb_evt_t event)
{
    disable_usb_interrupts();
    // Copy the event data into internal memory
    instance->usb_power_event = event;
    // Tell the upper layers of the stack to process the event
    instance->usb_event_type = USB_HW_EVENT_POWER;
    instance->events->start_process();
}

void USBPhyHw::_usb_virtual_status_event_handler(void)
{
    disable_usb_interrupts();

    // Tell the upper layers of the stack to process the event
    instance->usb_event_type = USB_HW_EVENT_VIRTUAL_STATUS;
    instance->events->start_process();
}

nrf_drv_usbd_transfer_t *USBPhyHw::get_transfer_buffer(usb_ep_t endpoint)
{
    // Index is base endpoint number * 2 (output), add 1 for input endpoints
    return &transfer_buf[(((endpoint & 0x7F) << 1) + ((endpoint & 0x80) >> 7))];
}

nrf_drv_usbd_ep_t USBPhyHw::get_nordic_endpoint(usb_ep_t endpoint)
{
    return (nrf_drv_usbd_ep_t) endpoint;
}

void USBPhyHw::_reset(void)
{
    // Disable all endpoints except for control endpoints
    nrf_drv_usbd_ep_default_config();

    nrf_drv_usbd_setup_clear();

    usb_event_type = USB_HW_EVENT_NONE;

    // Clear all endpoint interrupts
    NRFX_IRQ_PENDING_CLEAR(USBD_IRQn);
    nrf_usbd_event_clear((nrf_usbd_event_t)0x01FFFFFF);
}

void USBPhyHw::enable_usb_interrupts(void)
{
    // Enable USB and USB-related power interrupts
    NRFX_IRQ_ENABLE(USBD_IRQn);
    nrfx_power_usbevt_enable();
}

void USBPhyHw::disable_usb_interrupts(void)
{
    // Disable USB and USB-related power interrupts
    NRFX_IRQ_DISABLE(USBD_IRQn);
    nrfx_power_usbevt_disable();
}

static void power_usb_event_handler(nrfx_power_usb_evt_t event)
{
    if (instance) {
        // Pass the event on to the USBPhyHW instance
        instance->_usb_power_event_handler(event);
    }
}

static void usbd_event_handler(nrf_drv_usbd_evt_t const *const p_event)
{
    if (instance) {
        // Pass the event on to the USBPhyHW instance
        instance->_usb_event_handler(p_event);
    }
}

void USBD_HAL_IRQHandler(void)
{
    // Process the virtual status stage transfer event
    if (virtual_status_xfer_event) {
        if (instance) {
            instance->_usb_virtual_status_event_handler();
        }

        virtual_status_xfer_event = false;

    }
    // Call Nordic driver IRQ handler
    USBD_IRQHandler();
}
