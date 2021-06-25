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

#if defined(DEVICE_USBDEVICE) && (DEVICE_USBDEVICE) && \
    (defined(TARGET_MAX32620) || defined(TARGET_MAX32625) || defined(TARGET_MAX32630))

#include "mbed.h"
#include "USBPhyHw.h"
#include "usb_phy_api.h"
#include "USBEndpoints_Maxim.h"

#include "usb_regs.h"
#include "clkman_regs.h"

#if defined(TARGET_MAX32625) || defined(TARGET_MAX32630)
#include "pwrman_regs.h"
#endif

#define CONNECT_INTS    (MXC_F_USB_DEV_INTEN_BRST | MXC_F_USB_DEV_INTEN_SETUP | MXC_F_USB_DEV_INTEN_EP_IN | MXC_F_USB_DEV_INTEN_EP_OUT | MXC_F_USB_DEV_INTEN_DMA_ERR)

typedef struct {
    volatile uint32_t buf0_desc;
    volatile uint32_t buf0_address;
    volatile uint32_t buf1_desc;
    volatile uint32_t buf1_address;
} ep_buffer_t;

typedef struct {
    ep_buffer_t out_buffer;
    ep_buffer_t in_buffer;
} ep0_buffer_t;

typedef struct {
    ep0_buffer_t ep0;
    ep_buffer_t ep[MXC_USB_NUM_EP - 1];
} ep_buffer_descriptor_t;

// control packet state
static enum {
    CTRL_NONE = 0,
    CTRL_SETUP,
    CTRL_OUT,
    CTRL_IN,
} control_state;

// Static storage for endpoint buffer descriptor table. Must be 512 byte aligned for DMA.
MBED_ALIGN(512)
ep_buffer_descriptor_t ep_buffer_descriptor;

// static storage for temporary data buffers. Must be 32 byte aligned.
MBED_ALIGN(4)
static uint8_t aligned_buffer[NUMBER_OF_LOGICAL_ENDPOINTS][MXC_USB_MAX_PACKET];

static uint8_t* read_buf_addr[NUMBER_OF_LOGICAL_ENDPOINTS];

static USBPhyHw *instance;

static ep_buffer_t *get_desc(uint8_t endpoint)
{
    uint8_t epnum = EP_NUM(endpoint);
    ep_buffer_t *desc;

    if (epnum == 0) {
        if (IN_EP(endpoint)) {
            desc = &ep_buffer_descriptor.ep0.in_buffer;
        } else {
            desc = &ep_buffer_descriptor.ep0.out_buffer;
        }
    } else {
        desc = &ep_buffer_descriptor.ep[epnum - 1];
    }

    return desc;
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

void USBPhyHw::init(USBPhyEvents *events)
{
    this->events = events;

    // Disable IRQ
    NVIC_DisableIRQ(USB_IRQn);

    // Enable the USB clock
#if defined(TARGET_MAX32620)
    MXC_CLKMAN->clk_ctrl |= MXC_F_CLKMAN_CLK_CTRL_USB_CLOCK_ENABLE;
#elif defined(TARGET_MAX32625) || defined(TARGET_MAX32630)
    MXC_PWRMAN->pwr_rst_ctrl |= MXC_F_PWRMAN_PWR_RST_CTRL_USB_POWERED;
    MXC_CLKMAN->clk_ctrl |= MXC_F_CLKMAN_CLK_CTRL_USB_CLOCK_ENABLE;
#endif

    // reset the device
    MXC_USB->cn = 0;
    MXC_USB->cn = MXC_F_USB_CN_USB_EN;
    MXC_USB->dev_inten = 0;
    MXC_USB->dev_cn = 0;
    MXC_USB->dev_cn = MXC_F_USB_DEV_CN_URST;
    MXC_USB->dev_cn = 0;

    // clear driver state
    control_state = CTRL_NONE;

    // set the descriptor location
    MXC_USB->ep_base = (uint32_t)&ep_buffer_descriptor;

    // enable VBUS interrupts
    MXC_USB->dev_inten = MXC_F_USB_DEV_INTEN_NO_VBUS | MXC_F_USB_DEV_INTEN_VBUS;

    // Attach IRQ handler and Enable IRQ
    instance = this;
    NVIC_SetVector(USB_IRQn, (uint32_t)&_usbisr);
    NVIC_EnableIRQ(USB_IRQn);
}

void USBPhyHw::deinit()
{
    // Disconnect and disable interrupt
    MXC_USB->dev_cn = MXC_F_USB_DEV_CN_URST;
    MXC_USB->dev_cn = 0;
    MXC_USB->cn = 0;
    disconnect();
    NVIC_DisableIRQ(USB_IRQn);
}

bool USBPhyHw::powered()
{
    return true;
}

void USBPhyHw::connect()
{
    // enable interrupts
    MXC_USB->dev_inten |= CONNECT_INTS;

    // allow interrupts on ep0
    MXC_USB->ep[0] |= MXC_F_USB_EP_INT_EN;

    // pullup enable
    MXC_USB->dev_cn |= (MXC_F_USB_DEV_CN_CONNECT | MXC_F_USB_DEV_CN_FIFO_MODE);
}

void USBPhyHw::disconnect()
{
     // disable interrupts
    MXC_USB->dev_inten &= ~CONNECT_INTS;

    // disable pullup
    MXC_USB->dev_cn &= ~MXC_F_USB_DEV_CN_CONNECT;
}

void USBPhyHw::configure()
{
    // Do nothing
}

void USBPhyHw::unconfigure()
{
    // reset endpoints
    for (int i = 0; i < MXC_USB_NUM_EP; i++) {
        // Disable endpoint and clear the data toggle
        MXC_USB->ep[i] &= ~MXC_F_USB_EP_DIR;
        MXC_USB->ep[i] |= MXC_F_USB_EP_DT;
    }
}

void USBPhyHw::sof_enable()
{
    // Do nothing
}

void USBPhyHw::sof_disable()
{
    // Do nothing
}

void USBPhyHw::set_address(uint8_t address)
{
    // Do nothing
}

void USBPhyHw::remote_wakeup()
{
    // Do nothing
}

const usb_ep_table_t *USBPhyHw::endpoint_table()
{
    static const usb_ep_table_t endpoint_table = {
        4096 - 32 * 4, // 32 words for endpoint buffers
        // +3 based added to interrupt and isochronous to ensure enough
        // space for 4 byte alignment
        {
            {USB_EP_ATTR_ALLOW_CTRL | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_DIR_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_DIR_IN, 0, 0},
            {USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN, 0, 0},
            {USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
        }
    };
    return &endpoint_table;
}

uint32_t USBPhyHw::ep0_set_max_packet(uint32_t max_packet)
{
    return MAX_PACKET_SIZE_EP0;
}

void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size)
{
    uint32_t *ptr32 = (uint32_t*)buffer;
    
    // read setup packet
    ptr32[0] = (uint32_t)MXC_USB->setup0;
    ptr32[1] = (uint32_t)MXC_USB->setup1;
}

void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
    // Setup data buffer to receive next endpoint 0 OUT packet
    if (control_state == CTRL_IN) {
        // This is the status stage. ACK.
        MXC_USB->ep[0] |= MXC_F_USB_EP_ST_ACK;
        control_state = CTRL_NONE;
        return;
    }

    control_state = CTRL_OUT;

    endpoint_read(EP0OUT, data, size);
}

uint32_t USBPhyHw::ep0_read_result()
{
    // Return the size of the last OUT packet received on endpoint 0
    return endpoint_read_result(EP0OUT);
}

void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
    // Start transferring buffer on endpoint 0 IN
    if ((size == 0) && (control_state != CTRL_IN)) {
        // This is a status stage ACK. Handle in hardware.
        MXC_USB->ep[0] |= MXC_F_USB_EP_ST_ACK;
        control_state = CTRL_NONE;
        return;
    }

    control_state = CTRL_IN;

    endpoint_write(EP0IN, buffer, size);
}

void USBPhyHw::ep0_stall()
{
    endpoint_stall(EP0OUT);
}

bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type)
{
    uint8_t epnum = EP_NUM(endpoint);
    uint32_t ep_ctrl = 0;

    if (epnum >= NUMBER_OF_PHYSICAL_ENDPOINTS) {
        return false;
    }
    
    if (IN_EP(endpoint)) {
        ep_ctrl = MXC_S_USB_EP_DIR_IN;
    } else {
        ep_ctrl = MXC_S_USB_EP_DIR_OUT;
    }

    ep_ctrl |= (MXC_F_USB_EP_DT | MXC_F_USB_EP_INT_EN);

    MXC_USB->ep[epnum] = ep_ctrl;

    return true;
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    uint8_t epnum = EP_NUM(endpoint);
    // Disable and remove this endpoint
    MXC_USB->ep[epnum] &= ~MXC_F_USB_EP_DIR;
    MXC_USB->ep[epnum] |= MXC_F_USB_EP_DT;
}

void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    // Stall this endpoint until it is explicitly cleared
    uint8_t epnum = EP_NUM(endpoint);

    if (epnum == 0) {
        MXC_USB->ep[epnum] |= MXC_F_USB_EP_ST_STALL;
    }

    MXC_USB->ep[epnum] |= MXC_F_USB_EP_STALL;
}

void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    // Unstall this endpoint
    MXC_USB->ep[EP_NUM(endpoint)] &= ~MXC_F_USB_EP_STALL;
}

bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    // Setup data buffer to receive next endpoint OUT packet and return true if successful
    uint8_t epnum = EP_NUM(endpoint);

    if ((epnum >= NUMBER_OF_PHYSICAL_ENDPOINTS) ||
        IN_EP(endpoint) ||
        (size > MAX_PACKET_SIZE_EP0)) {
        return false;
    }
    
    uint32_t mask = (1 << epnum);
    if (MXC_USB->out_owner & mask) {
        return false;
    }

    read_buf_addr[epnum] = data;

    ep_buffer_t *desc = get_desc(endpoint);
    desc->buf0_desc = size;
    desc->buf0_address = (uint32_t)aligned_buffer[epnum];

    MXC_USB->out_owner = mask;

    return true;
}

uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    uint32_t size = 0;
    uint8_t epnum = EP_NUM(endpoint);

    if ((epnum >= NUMBER_OF_PHYSICAL_ENDPOINTS) || IN_EP(endpoint)) {
        return 0;
    }

    uint32_t mask = (1 << epnum);
    if (MXC_USB->out_owner & mask) {
        return 0;
    }

    // get the packet length and contents
    ep_buffer_t *desc = get_desc(endpoint);
    size = desc->buf0_desc;
    memcpy(read_buf_addr[epnum], aligned_buffer[epnum], size);

    return size;
}

bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    // Start transferring buffer on endpoint IN
    uint8_t epnum = EP_NUM(endpoint);

    if ((epnum >= NUMBER_OF_PHYSICAL_ENDPOINTS) || 
        OUT_EP(endpoint) || 
        (size > MAX_PACKET_SIZE_EP0)) {
        return false;
    }

    uint32_t mask = (1 << epnum);
    if (MXC_USB->in_owner & mask) {
        return false;
    }

    memcpy(aligned_buffer[epnum], data, size);

    ep_buffer_t *desc = get_desc(endpoint);
    desc->buf0_desc = size;
    desc->buf0_address = (uint32_t)aligned_buffer[epnum];

    // start the DMA
    MXC_USB->in_owner = mask;

    return true;
}

void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
    // Stop the current transfer on this endpoint and don't call the IN or OUT callback
    ep_buffer_t *desc = get_desc(endpoint);
    desc->buf0_desc = 0;
    desc->buf0_address = 0;
}

void USBPhyHw::process()
{
    // get and clear irqs
    uint32_t irq_flags = MXC_USB->dev_intfl;
    MXC_USB->dev_intfl = irq_flags;

    // process only enabled interrupts
    irq_flags &= MXC_USB->dev_inten;

    // suspend 
    if (irq_flags & MXC_F_USB_DEV_INTFL_SUSP) {
        events->suspend(true);
    }

    // bus reset
    if (irq_flags & MXC_F_USB_DEV_INTFL_BRST) {

        // reset endpoints
        for (int i = 0; i < NUMBER_OF_LOGICAL_ENDPOINTS; i++) {
            // Disable endpoint and clear the data toggle
            MXC_USB->ep[i] &= ~MXC_F_USB_EP_DIR;
            MXC_USB->ep[i] |= MXC_F_USB_EP_DT;
        }

        events->reset();

        // clear driver state
        control_state = CTRL_NONE;

    }

    // power applied
    if (irq_flags & MXC_F_USB_DEV_INTFL_VBUS) {
        events->power(true);
    }

    // power lost
    if (irq_flags & MXC_F_USB_DEV_INTFL_NO_VBUS) {
        events->power(false);
    }
    
    // Setup packet
    if (irq_flags & MXC_F_USB_DEV_INTFL_SETUP) {
        events->ep0_setup();
    }

    // IN packets
    if (irq_flags & MXC_F_USB_DEV_INTFL_EP_IN) {
        // get and clear IN irqs
        uint32_t in_irqs = MXC_USB->in_int;
        MXC_USB->in_int = in_irqs;

        if (in_irqs & 1) {
            events->ep0_in();
        }

        for (uint8_t epnum = 1; epnum < NUMBER_OF_LOGICAL_ENDPOINTS; epnum++) {
            uint32_t irq_mask = (1 << epnum);
            if (in_irqs & irq_mask) {
                uint8_t endpoint = epnum | DIR_IN;
                events->in(endpoint);
            }
        }
    }

    // OUT packets
    if (irq_flags & MXC_F_USB_DEV_INTFL_EP_OUT) {
        // get and clear OUT irqs
        uint32_t out_irqs = MXC_USB->out_int;
        MXC_USB->out_int = out_irqs;

        if (out_irqs & 1) {
            events->ep0_out();
        }

        for (uint8_t epnum = 1; epnum < NUMBER_OF_LOGICAL_ENDPOINTS; epnum++) {
            uint32_t irq_mask = (1 << epnum);
            if (out_irqs & irq_mask) {
                uint8_t endpoint = epnum | DIR_OUT;
                events->out(endpoint);
            }
        }
    }

    // Re-enable interrupt
    NVIC_ClearPendingIRQ(USB_IRQn);
    NVIC_EnableIRQ(USB_IRQn);
}

void USBPhyHw::_usbisr(void)
{
    NVIC_DisableIRQ(USB_IRQn);
    // Handle interrupts
    instance->events->start_process();
}

#endif
