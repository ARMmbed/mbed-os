/* mbed Microcontroller Library
 * Copyright (c) 2018-2020 ARM Limited
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

#if defined(DEVICE_USBDEVICE) && DEVICE_USBDEVICE

#include "USBPhyHw_MCUXpresso.h"
#include "USBEndpoints_MCUXpresso.h"
#include "fsl_clock_config.h"

static USBPhyHw *instance;

static volatile int epComplete = 0;

static uint32_t usb_irq = 0;

usb_device_handle g_deviceHandle;

static USB_Type * const usb_addrs[] = USB_BASE_PTRS;

// Convert physical endpoint number to register bit
#define EP(endpoint) (1<<(endpoint))

// Conversion macros
#define PHY_TO_LOG(endpoint)    ((endpoint)>>1)
#define DESC_TO_LOG(endpoint)   ((endpoint) & 0xF)
#define DESC_TO_PHY(endpoint) ((((endpoint)&0x0F)<<1) | (((endpoint) & 0x80) ? 1:0))
#define PHY_TO_DESC(endpoint) (((endpoint)>>1)|(((endpoint)&1)?0x80:0))

// Get endpoint direction
#define DESC_EP_IN(endpoint)     ((endpoint) & 0x80U ? true : false)
#define DESC_EP_OUT(endpoint)    ((endpoint) & 0x80U ? false : true)

#define TX    1
#define RX    0
#define ODD   0
#define EVEN  1
// this macro waits a physical endpoint number
#define EP_BDT_IDX(ep, dir, odd) (((ep * 4) + (2 * dir) + (1 *  odd)))

uint8_t *endpoint_buffer[NUMBER_OF_PHYSICAL_ENDPOINTS * 2];
uint16_t endpoint_packet_size[NUMBER_OF_PHYSICAL_ENDPOINTS];
USB_CONTROLLER_DATA uint32_t ep0_buffer[2][MAX_PACKET_SIZE_EP0 / 4];
USB_CONTROLLER_DATA uint32_t ep1_buffer[2][MAX_PACKET_SIZE_EP1 / 4];
USB_CONTROLLER_DATA uint32_t ep2_buffer[2][MAX_PACKET_SIZE_EP2 / 4];
USB_CONTROLLER_DATA uint32_t ep3_buffer[2][MAX_PACKET_SIZE_EP3 / 4];

static uint8_t set_addr = 0;

extern "C" uint32_t USB_DeviceGetIrqNumber(void);
extern "C" void USB_DeviceClockInit(void);

static uint32_t frameNumber()
{
    return (usb_addrs[USB_INSTANCE]->FRINDEX >> 3);
}

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

usb_status_t USBPhy_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    switch (event) {
        // reset interrupt
        case kUSB_DeviceEventBusReset:
            instance->endpoint_add(EP0OUT, MAX_PACKET_SIZE_EP0, USB_EP_TYPE_CTRL);
            instance->endpoint_add(EP0IN, MAX_PACKET_SIZE_EP0, USB_EP_TYPE_CTRL);

            memset(instance->read_buffers, 0, sizeof(instance->read_buffers));
            memset(instance->read_sizes, 0, sizeof(instance->read_sizes));

            // reset bus for USBDevice layer
            instance->events->reset();
            break;
        case kUSB_DeviceEventResume:
            instance->events->suspend(false);
            break;
        case kUSB_DeviceEventSuspend:
            instance->events->suspend(true);
            break;
        default:
            break;
    }

    return kStatus_USB_Success;
}

usb_status_t USBPhy_EndpointCallback(usb_device_handle handle, usb_device_endpoint_callback_message_struct_t *message,
                                     void *callbackParam)
{
    usb_ep_t endpoint = (usb_ep_t )((long)callbackParam);
    int phy_ep = DESC_TO_PHY(endpoint);
    uint8_t state;

    if (message->length == USB_UNINITIALIZED_VAL_32) {
        return kStatus_USB_Success;
    }

    /* Update the read size to what was actually read */
    instance->read_sizes[phy_ep] = message->length;

    // setup packet
    if (message->isSetup) {
        memcpy(&ep0_buffer[0][0], message->buffer, 8U);

        // EP0 SETUP event (SETUP data received)
        instance->events->ep0_setup();
    } else {
        if (DESC_EP_IN(endpoint)) {
            if ((endpoint & USB_ENDPOINT_NUMBER_MASK) == 0) {
                instance->events->ep0_in();
                if (set_addr == 1) {
                    state = (uint8_t)kUSB_DeviceStateAddress;
                    USB_DeviceSetStatus(g_deviceHandle, kUSB_DeviceStatusAddress, &state);
                    set_addr = 0;
                }
            } else {
                epComplete |= EP(phy_ep);
                instance->events->in(endpoint);
            }
        } else {
            if ((endpoint & USB_ENDPOINT_NUMBER_MASK) == 0) {
                instance->events->ep0_out();
             } else {
                epComplete |= EP(phy_ep);
                instance->events->out(endpoint);
            }
        }
    }

    return kStatus_USB_Success;
}

void USBPhyHw::init(USBPhyEvents *events)
{
    if (this->events == NULL) {
        sleep_manager_lock_deep_sleep();
    }

    this->events = events;

    usb_irq = USB_DeviceGetIrqNumber();

    // Disable IRQ
    NVIC_DisableIRQ((IRQn_Type)usb_irq);

    // Attach IRQ
    instance = this;

    USB_DeviceClockInit();

    if (kStatus_USB_Success != USB_DeviceInit(CONTROLLER_ID, USBPhy_DeviceCallback, &g_deviceHandle)) {
        return;
    }

    /* Allocate control endpoint buffers */
    endpoint_buffer[EP_BDT_IDX(0, TX, ODD)] = (uint8_t*)ep0_buffer[TX];
    endpoint_buffer[EP_BDT_IDX(0, RX, ODD)] = (uint8_t*)ep0_buffer[RX];
    endpoint_buffer[EP_BDT_IDX(1, TX, ODD)] = (uint8_t*)ep1_buffer[TX];
    endpoint_buffer[EP_BDT_IDX(1, RX, ODD)] = (uint8_t*)ep1_buffer[RX];
    endpoint_buffer[EP_BDT_IDX(2, TX, ODD)] = (uint8_t*)ep2_buffer[TX];
    endpoint_buffer[EP_BDT_IDX(2, RX, ODD)] = (uint8_t*)ep2_buffer[RX];
    endpoint_buffer[EP_BDT_IDX(3, TX, ODD)] = (uint8_t*)ep3_buffer[TX];
    endpoint_buffer[EP_BDT_IDX(3, RX, ODD)] = (uint8_t*)ep3_buffer[RX];
    endpoint_packet_size[0] = sizeof (ep0_buffer[TX]);
    endpoint_packet_size[1] = sizeof (ep0_buffer[RX]);
    endpoint_packet_size[2] = sizeof (ep1_buffer[TX]);
    endpoint_packet_size[3] = sizeof (ep1_buffer[RX]);
    endpoint_packet_size[4] = sizeof (ep2_buffer[TX]);
    endpoint_packet_size[5] = sizeof (ep2_buffer[RX]);
    endpoint_packet_size[6] = sizeof (ep3_buffer[TX]);
    endpoint_packet_size[7] = sizeof (ep3_buffer[RX]);

    NVIC_SetVector((IRQn_Type)usb_irq, (uint32_t)&_usbisr);
    NVIC_EnableIRQ((IRQn_Type)usb_irq);
}

void USBPhyHw::deinit()
{
    USB_DeviceDeinit(g_deviceHandle);

    disconnect();
    NVIC_DisableIRQ((IRQn_Type)usb_irq);

    if (events != NULL) {
        sleep_manager_unlock_deep_sleep();
    }
    events = NULL;
}

bool USBPhyHw::powered()
{
    return true;
}

void USBPhyHw::connect()
{
    USB_DeviceRun(g_deviceHandle);
}

void USBPhyHw::disconnect()
{
    // disable all endpoints to prevent them from nacking when disconnected
    for (uint8_t i = 0; i < (USB_DEVICE_CONFIG_ENDPOINTS); i++) {
        USB_DeviceDeinitEndpoint(g_deviceHandle,
                                 (i & USB_ENDPOINT_NUMBER_MASK) |
                                 (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT));
        USB_DeviceDeinitEndpoint(g_deviceHandle,
                                 (i & USB_ENDPOINT_NUMBER_MASK) |
                                 (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT));

    }

    USB_DeviceStop(g_deviceHandle);
}

void USBPhyHw::configure()
{

}

void USBPhyHw::unconfigure()
{

}

void USBPhyHw::sof_enable()
{
    usb_addrs[USB_INSTANCE]->USBINTR |= USBHS_USBINTR_SRE_MASK;
}

void USBPhyHw::sof_disable()
{
    usb_addrs[USB_INSTANCE]->USBINTR &= ~USBHS_USBINTR_SRE_MASK;
}

void USBPhyHw::set_address(uint8_t address)
{
    uint8_t state;

    // we don't set the address now, we set a flag instead.
    // see usbisr when an IN token is received
    set_addr = 1;

    state = address & 0xFFU;

    USB_DeviceSetStatus(g_deviceHandle, kUSB_DeviceStatusAddress, &state);
}

void USBPhyHw::remote_wakeup()
{

}

#define ALLOW_BULK_OR_INT_ENDPOINTS (USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT)
#define ALLOW_NO_ENDPOINTS 0

const usb_ep_table_t *USBPhyHw::endpoint_table()
{
    static const usb_ep_table_t endpoint_table = {
        1, // No cost per endpoint - everything allocated up front
        {
            {USB_EP_ATTR_ALLOW_CTRL | USB_EP_ATTR_DIR_IN_AND_OUT,       0, 0},
            {ALLOW_BULK_OR_INT_ENDPOINTS | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
            {ALLOW_BULK_OR_INT_ENDPOINTS | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
            {USB_EP_ATTR_ALLOW_ISO | USB_EP_ATTR_DIR_IN_AND_OUT,        0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN_AND_OUT,           0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN_AND_OUT,           0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN_AND_OUT,           0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN_AND_OUT,           0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN_AND_OUT,           0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN_AND_OUT,           0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN_AND_OUT,           0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN_AND_OUT,           0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN_AND_OUT,           0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN_AND_OUT,           0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN_AND_OUT,           0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN_AND_OUT,           0, 0}
        }
    };
    return &endpoint_table;
}

uint32_t USBPhyHw::ep0_set_max_packet(uint32_t max_packet)
{
    return USB_CONTROL_MAX_PACKET_SIZE;
}

// read setup packet
void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size)
{
    memcpy(buffer, &ep0_buffer[0][0], 8U);
}

void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
    endpoint_read(EP0OUT, data, size);
}

uint32_t USBPhyHw::ep0_read_result()
{
    return endpoint_read_result(EP0OUT);
}

void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
    endpoint_write(EP0IN, buffer, size);
}

void USBPhyHw::ep0_stall()
{
    endpoint_stall(EP0IN);
    endpoint_stall(EP0OUT);
}

bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type)
{
    usb_device_endpoint_init_struct_t epInit;
    usb_device_endpoint_callback_struct_t epCallback;

    memset(&epInit, 0, sizeof(epInit));
    memset(&epCallback, 0, sizeof(epCallback));

    if (DESC_TO_PHY(endpoint) > NUMBER_OF_PHYSICAL_ENDPOINTS - 1) {
        return false;
    }

    epInit.transferType = (uint8_t)type;
    epInit.maxPacketSize = (uint16_t)max_packet;
    epInit.endpointAddress = endpoint;
    if (type == USB_EP_TYPE_CTRL) {
        epInit.zlt = 1;
    }

    epCallback.callbackFn = USBPhy_EndpointCallback;
    epCallback.callbackParam = (void *)((long)endpoint);

    if (USB_DeviceInitEndpoint(g_deviceHandle, &epInit, &epCallback) != kStatus_USB_Success) {
        return false;
    }

    return true;
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    USB_DeviceDeinitEndpoint(g_deviceHandle, endpoint);
}

void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    USB_DeviceStallEndpoint(g_deviceHandle, endpoint);
}

void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    USB_DeviceUnstallEndpoint(g_deviceHandle, endpoint);
}

bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    uint8_t log = DESC_TO_PHY(endpoint);

    read_buffers[log] = data;
    read_sizes[log] = size;
    return endpoint_read_core(endpoint, size);
}

bool USBPhyHw::endpoint_read_core(usb_ep_t endpoint, uint32_t max_packet)
{
    uint8_t log_endpoint = DESC_TO_LOG(endpoint);
    uint8_t *buf;

    buf = &endpoint_buffer[EP_BDT_IDX(log_endpoint, RX, ODD)][0];

    if (USB_DeviceRecvRequest(g_deviceHandle, endpoint, buf, (max_packet > endpoint_packet_size[log_endpoint] ? endpoint_packet_size[log_endpoint] : max_packet)) != kStatus_USB_Success) {
        return false;
    }

    return true;
}

uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    uint8_t log = DESC_TO_PHY(endpoint);

    uint32_t bytes_read = 0;
    endpoint_read_result_core(endpoint, read_buffers[log], read_sizes[log], &bytes_read);
    read_buffers[log] = NULL;
    read_sizes[log] = 0;

    return bytes_read;
}


bool USBPhyHw::endpoint_read_result_core(usb_ep_t endpoint, uint8_t *data, uint32_t size, uint32_t *bytes_read)
{
    uint32_t n, idx;
    uint8_t *ep_buf;

    uint32_t log_endpoint = DESC_TO_LOG(endpoint);

    if (DESC_TO_PHY(endpoint) > NUMBER_OF_PHYSICAL_ENDPOINTS - 1) {
        return false;
    }

    // if read on a IN endpoint -> error
    if (DESC_EP_IN(endpoint)) {
        return false;
    }

    idx = EP_BDT_IDX(log_endpoint, RX, 0);

    if ((log_endpoint != 0) && !(epComplete & EP(DESC_TO_PHY(endpoint)))) {
        return false;
    }

    ep_buf = endpoint_buffer[idx];

    for (n = 0; n < read_sizes[DESC_TO_PHY(endpoint)]; n++) {
        data[n] = ep_buf[n];
    }

    *bytes_read = read_sizes[DESC_TO_PHY(endpoint)];

    epComplete &= ~EP(DESC_TO_PHY(endpoint));
    return true;
}

bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    uint32_t idx, n;
    uint8_t *ep_buf;

    if (DESC_TO_PHY(endpoint) > NUMBER_OF_PHYSICAL_ENDPOINTS - 1) {
        return false;
    }

    // if write on a OUT endpoint -> error
    if (DESC_EP_OUT(endpoint)) {
        return false;
    }

    idx = EP_BDT_IDX(DESC_TO_LOG(endpoint), TX, 0);

    ep_buf = endpoint_buffer[idx];

    if (size > endpoint_packet_size[DESC_TO_LOG(endpoint)]) {
        size = endpoint_packet_size[DESC_TO_LOG(endpoint)];
    }
    for (n = 0; n < size; n++) {
        ep_buf[n] = data[n];
    }

    if (USB_DeviceSendRequest(g_deviceHandle, endpoint, ep_buf, size) != kStatus_USB_Success) {
        return false;
    }

    return true;
}

void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
    USB_DeviceCancel(g_deviceHandle, endpoint);
}

void USBPhyHw::process()
{
    uint32_t istat = usb_addrs[USB_INSTANCE]->USBSTS & usb_addrs[USB_INSTANCE]->USBINTR;

    // SOF interrupt
    if (istat & USBHS_USBSTS_SRI_MASK) {
        // SOF event, read frame number
        events->sof(frameNumber());
    }

    USB_DeviceEhciIsrFunction(g_deviceHandle);

    NVIC_ClearPendingIRQ((IRQn_Type)usb_irq);
    NVIC_EnableIRQ((IRQn_Type)usb_irq);
}

void USBPhyHw::_usbisr(void)
{
    NVIC_DisableIRQ((IRQn_Type)usb_irq);
    instance->events->start_process();
}

#endif
