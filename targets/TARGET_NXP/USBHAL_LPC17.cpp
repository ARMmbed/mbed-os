/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#if defined(DEVICE_USBDEVICE) && DEVICE_USBDEVICE && \
    (defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC2460))

#include "USBEndpoints_LPC17_LPC23.h"
#include "USBPhyHw.h"
#include "usb_phy_api.h"


// Get endpoint direction
#define IN_EP(endpoint)     ((endpoint) & 0x80U ? true : false)
#define OUT_EP(endpoint)    ((endpoint) & 0x80U ? false : true)

// Convert physical endpoint number to register bit
#define EP(endpoint) (1UL<<DESC_TO_PHY(endpoint))

// Check if this is an isochronous endpoint
#define ISO_EP(endpoint)        ((((endpoint) & 0xF) == 3) || (((endpoint) & 0xF) == 6) || (((endpoint) & 0xF) == 9) || (((endpoint) & 0xF) == 12))

#define DESC_TO_PHY(endpoint) ((((endpoint)&0x0F)<<1) | (((endpoint) & 0x80) ? 1:0))
#define PHY_TO_DESC(endpoint) (((endpoint)>>1)|(((endpoint)&1)?0x80:0))

// Power Control for Peripherals register
#define PCUSB      (1UL<<31)

// USB Clock Control register
#define DEV_CLK_EN (1UL<<1)
#define AHB_CLK_EN (1UL<<4)

// USB Clock Status register
#define DEV_CLK_ON (1UL<<1)
#define AHB_CLK_ON (1UL<<4)

// USB Device Interupt registers
#define FRAME      (1UL<<0)
#define EP_FAST    (1UL<<1)
#define EP_SLOW    (1UL<<2)
#define DEV_STAT   (1UL<<3)
#define CCEMPTY    (1UL<<4)
#define CDFULL     (1UL<<5)
#define RxENDPKT   (1UL<<6)
#define TxENDPKT   (1UL<<7)
#define EP_RLZED   (1UL<<8)
#define ERR_INT    (1UL<<9)

// USB Control register
#define RD_EN (1<<0)
#define WR_EN (1<<1)
#define LOG_ENDPOINT(endpoint) ((DESC_TO_PHY(endpoint)>>1)<<2)

// USB Receive Packet Length register
#define DV      (1UL<<10)
#define PKT_RDY (1UL<<11)
#define PKT_LNGTH_MASK (0x3ff)

// Serial Interface Engine (SIE)
#define SIE_WRITE   (0x01)
#define SIE_READ    (0x02)
#define SIE_COMMAND (0x05)
#define SIE_CMD_CODE(phase, data) ((phase<<8)|(data<<16))

// SIE Command codes
#define SIE_CMD_SET_ADDRESS        (0xD0)
#define SIE_CMD_CONFIGURE_DEVICE   (0xD8)
#define SIE_CMD_SET_MODE           (0xF3)
#define SIE_CMD_READ_FRAME_NUMBER  (0xF5)
#define SIE_CMD_READ_TEST_REGISTER (0xFD)
#define SIE_CMD_SET_DEVICE_STATUS  (0xFE)
#define SIE_CMD_GET_DEVICE_STATUS  (0xFE)
#define SIE_CMD_GET_ERROR_CODE     (0xFF)
#define SIE_CMD_READ_ERROR_STATUS  (0xFB)

#define SIE_CMD_SELECT_ENDPOINT(endpoint)                 (0x00+DESC_TO_PHY(endpoint))
#define SIE_CMD_SELECT_ENDPOINT_CLEAR_INTERRUPT(endpoint) (0x40+DESC_TO_PHY(endpoint))
#define SIE_CMD_SET_ENDPOINT_STATUS(endpoint)             (0x40+DESC_TO_PHY(endpoint))

#define SIE_CMD_CLEAR_BUFFER    (0xF2)
#define SIE_CMD_VALIDATE_BUFFER (0xFA)

// SIE Device Status register
#define SIE_DS_CON    (1<<0)
#define SIE_DS_CON_CH (1<<1)
#define SIE_DS_SUS    (1<<2)
#define SIE_DS_SUS_CH (1<<3)
#define SIE_DS_RST    (1<<4)

// SIE Device Set Address register
#define SIE_DSA_DEV_EN  (1<<7)

// SIE Configue Device register
#define SIE_CONF_DEVICE (1<<0)

// Select Endpoint register
#define SIE_SE_FE       (1<<0)
#define SIE_SE_ST       (1<<1)
#define SIE_SE_STP      (1<<2)
#define SIE_SE_PO       (1<<3)
#define SIE_SE_EPN      (1<<4)
#define SIE_SE_B_1_FULL (1<<5)
#define SIE_SE_B_2_FULL (1<<6)

// Set Endpoint Status command
#define SIE_SES_ST      (1<<0)
#define SIE_SES_DA      (1<<5)
#define SIE_SES_RF_MO   (1<<6)
#define SIE_SES_CND_ST  (1<<7)


static USBPhyHw *instance;

static uint32_t opStarted;

static const usb_ep_t ISO_EPS[] = {
    0x03, 0x83,
    0x06, 0x86,
    0x09, 0x89,
    0x0C, 0x8C
};

static void SIECommand(uint32_t command)
{
    // The command phase of a SIE transaction
    LPC_USB->USBDevIntClr = CCEMPTY;
    LPC_USB->USBCmdCode = SIE_CMD_CODE(SIE_COMMAND, command);
    while (!(LPC_USB->USBDevIntSt & CCEMPTY));
}

static void SIEWriteData(uint8_t data)
{
    // The data write phase of a SIE transaction
    LPC_USB->USBDevIntClr = CCEMPTY;
    LPC_USB->USBCmdCode = SIE_CMD_CODE(SIE_WRITE, data);
    while (!(LPC_USB->USBDevIntSt & CCEMPTY));
}

static uint8_t SIEReadData(uint32_t command)
{
    // The data read phase of a SIE transaction
    LPC_USB->USBDevIntClr = CDFULL;
    LPC_USB->USBCmdCode = SIE_CMD_CODE(SIE_READ, command);
    while (!(LPC_USB->USBDevIntSt & CDFULL));
    return (uint8_t)LPC_USB->USBCmdData;
}

static void SIEsetDeviceStatus(uint8_t status)
{
    // Write SIE device status register
    SIECommand(SIE_CMD_SET_DEVICE_STATUS);
    SIEWriteData(status);
}

static uint8_t SIEgetDeviceStatus(void)
{
    // Read SIE device status register
    SIECommand(SIE_CMD_GET_DEVICE_STATUS);
    return SIEReadData(SIE_CMD_GET_DEVICE_STATUS);
}

void SIEsetAddress(uint8_t address, bool enable = true)
{
    // Write SIE device address register
    SIECommand(SIE_CMD_SET_ADDRESS);
    SIEWriteData((address & 0x7f) | (enable ? SIE_DSA_DEV_EN : 0));
}

static uint8_t SIEselectEndpoint(uint8_t endpoint)
{
    // SIE select endpoint command
    SIECommand(SIE_CMD_SELECT_ENDPOINT(endpoint));
    return SIEReadData(SIE_CMD_SELECT_ENDPOINT(endpoint));
}

static uint8_t SIEclearBuffer(void)
{
    // SIE clear buffer command
    SIECommand(SIE_CMD_CLEAR_BUFFER);
    return SIEReadData(SIE_CMD_CLEAR_BUFFER);
}

static void SIEvalidateBuffer(void)
{
    // SIE validate buffer command
    SIECommand(SIE_CMD_VALIDATE_BUFFER);
}

static void SIEsetEndpointStatus(uint8_t endpoint, uint8_t status)
{
    // SIE set endpoint status command
    SIECommand(SIE_CMD_SET_ENDPOINT_STATUS(endpoint));
    SIEWriteData(status);
}

static uint16_t SIEgetFrameNumber(void) __attribute__((unused));
static uint16_t SIEgetFrameNumber(void)
{
    // Read current frame number
    uint16_t lowByte;
    uint16_t highByte;

    SIECommand(SIE_CMD_READ_FRAME_NUMBER);
    lowByte = SIEReadData(SIE_CMD_READ_FRAME_NUMBER);
    highByte = SIEReadData(SIE_CMD_READ_FRAME_NUMBER);

    return (highByte << 8) | lowByte;
}

static void SIEconfigureDevice(void)
{
    // SIE Configure device command
    SIECommand(SIE_CMD_CONFIGURE_DEVICE);
    SIEWriteData(SIE_CONF_DEVICE);
}

static void SIEunconfigureDevice(void)
{
    // SIE Configure device command
    SIECommand(SIE_CMD_CONFIGURE_DEVICE);
    SIEWriteData(0);
}

static void SIEconnect(void)
{
    // Connect USB device
    uint8_t status = SIEgetDeviceStatus();
    SIEsetDeviceStatus(status | SIE_DS_CON);
}


static void SIEdisconnect(void)
{
    // Disconnect USB device
    uint8_t status = SIEgetDeviceStatus();
    SIEsetDeviceStatus(status & ~SIE_DS_CON);
}


static uint8_t selectEndpointClearInterrupt(uint8_t endpoint)
{
    // Implemented using using EP_INT_CLR.
    LPC_USB->USBEpIntClr = EP(endpoint);
    while (!(LPC_USB->USBDevIntSt & CDFULL));
    return (uint8_t)LPC_USB->USBCmdData;
}


static void enableEndpointEvent(uint8_t endpoint)
{
    // Route endpoint events to USBEpIntSt so they trigger an interrupt
    LPC_USB->USBEpIntEn |= EP(endpoint);
}

// Do not use disableEndpointEvent. If an endpoint's event is disabled
// and a transfer occurs on that endpoint then that endpoint will enter
// a bad state. Future transfers on that endpoint will not trigger an
// interrupt even if the endpoint event is enabled again or the
// endpoint is reinitialized
/*
static void disableEndpointEvent(uint8_t endpoint) __attribute__((unused));
static void disableEndpointEvent(uint8_t endpoint)
{
    // Don't set endpoint interrupt to pending in USBEpIntSt when an event occurs.
    // Instead route them to USBDMARSt so they can be ignored.
    LPC_USB->USBEpIntEn &= ~EP(endpoint);
}
*/


static uint32_t endpointReadcore(uint8_t endpoint, uint8_t *buffer, uint32_t size)
{
    // Read from an OUT endpoint
    uint32_t actual_size;
    uint32_t i;
    uint32_t data = 0;
    uint8_t offset;

    LPC_USB->USBCtrl = LOG_ENDPOINT(endpoint) | RD_EN;
    while (!(LPC_USB->USBRxPLen & PKT_RDY));

    actual_size = LPC_USB->USBRxPLen & PKT_LNGTH_MASK;

    offset = 0;

    if (actual_size > 0) {
        for (i = 0; i < actual_size; i++) {
            if (offset == 0) {
                // Fetch up to four bytes of data as a word
                data = LPC_USB->USBRxData;
            }

            // extract a byte
            if (size) {
                *buffer = (data >> offset) & 0xff;
                buffer++;
                size--;
            }

            // move on to the next byte
            offset = (offset + 8) % 32;
        }
    } else {
        (void)LPC_USB->USBRxData;
    }

    LPC_USB->USBCtrl = 0;

    return actual_size;
}

static void endpointWritecore(uint8_t endpoint, uint8_t *buffer, uint32_t size)
{
    // Write to an IN endpoint
    uint32_t temp, data;
    uint8_t offset;

    LPC_USB->USBCtrl = LOG_ENDPOINT(endpoint) | WR_EN;

    LPC_USB->USBTxPLen = size;
    offset = 0;
    data = 0;

    if (size > 0) {
        do {
            // Fetch next data byte into a word-sized temporary variable
            temp = *buffer++;

            // Add to current data word
            temp = temp << offset;
            data = data | temp;

            // move on to the next byte
            offset = (offset + 8) % 32;
            size--;

            if ((offset == 0) || (size == 0)) {
                // Write the word to the endpoint
                LPC_USB->USBTxData = data;
                data = 0;
            }
        } while (size > 0);
    } else {
        LPC_USB->USBTxData = 0;
    }

    // Clear WR_EN to cover zero length packet case
    LPC_USB->USBCtrl = 0;

    SIEselectEndpoint(endpoint);
    SIEvalidateBuffer();
}

USBPhy *get_usb_phy()
{
    static USBPhyHw usbphy;
    return &usbphy;
}

USBPhyHw::USBPhyHw(void): events(NULL)
{

}

USBPhyHw::~USBPhyHw(void)
{

}

void USBPhyHw::init(USBPhyEvents *events)
{
    if (this->events == NULL) {
        sleep_manager_lock_deep_sleep();
    }
    this->events = events;

    // Disable IRQ
    NVIC_DisableIRQ(USB_IRQn);

    memset(read_buffers, 0, sizeof(read_buffers));
    memset(read_sizes, 0, sizeof(read_sizes));

    // Enable power to USB device controller
    LPC_SC->PCONP |= PCUSB;

    // Enable USB clocks
    LPC_USB->USBClkCtrl |= DEV_CLK_EN | AHB_CLK_EN;
    while (LPC_USB->USBClkSt != (DEV_CLK_ON | AHB_CLK_ON));

    // Configure pins P0.29 and P0.30 to be USB D+ and USB D-
    LPC_PINCON->PINSEL1 &= 0xc3ffffff;
    LPC_PINCON->PINSEL1 |= 0x14000000;

    // Disconnect USB device
    SIEdisconnect();

    // Configure pin P2.9 to be Connect
    LPC_PINCON->PINSEL4 &= 0xfffcffff;
    LPC_PINCON->PINSEL4 |= 0x00040000;

    // Connect must be low for at least 2.5uS
    ThisThread::sleep_for(300);

    // Disable control endpoints
    SIEsetEndpointStatus(EP0IN, SIE_SES_DA);
    SIEsetEndpointStatus(EP0OUT, SIE_SES_DA);

    // Set the maximum packet size for the control endpoints
    endpoint_add(EP0IN, MAX_PACKET_SIZE_EP0, USB_EP_TYPE_CTRL);
    endpoint_add(EP0OUT, MAX_PACKET_SIZE_EP0, USB_EP_TYPE_CTRL);

    // Map interrupts to USBEpIntSt
    enableEndpointEvent(EP0IN);
    enableEndpointEvent(EP0OUT);

    // Attach IRQ
    instance = this;
    NVIC_SetVector(USB_IRQn, (uint32_t)&_usbisr);

    // Enable interrupts for device events and EP0
    LPC_USB->USBDevIntEn = EP_SLOW | DEV_STAT | FRAME;

    NVIC_EnableIRQ(USB_IRQn);
}

void USBPhyHw::deinit()
{
    // Ensure device disconnected
    SIEdisconnect();
    // Disable USB interrupts
    NVIC_DisableIRQ(USB_IRQn);
    events = NULL;
    opStarted = 0;

    if (events != NULL) {
        sleep_manager_unlock_deep_sleep();
    }
    events = NULL;
}

bool USBPhyHw::powered()
{
    return true;
}

void USBPhyHw::connect(void)
{
    // Enable control endpoints
    SIEsetEndpointStatus(EP0IN, 0);
    SIEsetEndpointStatus(EP0OUT, 0);

    // Connect USB device
    SIEconnect();
}

void USBPhyHw::disconnect(void)
{
    // Disable control endpoints
    SIEsetEndpointStatus(EP0IN, SIE_SES_DA);
    SIEsetEndpointStatus(EP0OUT, SIE_SES_DA);

    if (LPC_USB->USBEpIntSt & EP(EP0IN)) {
        selectEndpointClearInterrupt(EP0IN);
    }
    if (LPC_USB->USBEpIntSt & EP(EP0OUT)) {
        selectEndpointClearInterrupt(EP0OUT);
    }

    // Turn off USB nacking
    SIEsetAddress(0, false);

    // Disconnect USB device
    SIEdisconnect();

    // Reset all started operations
    opStarted = 0;
}

void USBPhyHw::configure(void)
{
    SIEconfigureDevice();
}

void USBPhyHw::unconfigure(void)
{
    SIEunconfigureDevice();
}

void USBPhyHw::sof_enable()
{
    //TODO
}

void USBPhyHw::sof_disable()
{
    //TODO
}

void USBPhyHw::set_address(uint8_t address)
{
    SIEsetAddress(address);
}

uint32_t USBPhyHw::ep0_set_max_packet(uint32_t max_packet)
{
    return MAX_PACKET_SIZE_EP0;
}

void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size)
{
    endpointReadcore(EP0OUT, buffer, size);
}

void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
    read_buffers[EP0OUT] = data;
    read_sizes[EP0OUT] = size;
    SIEselectEndpoint(EP0OUT);
    SIEclearBuffer();
}

uint32_t USBPhyHw::ep0_read_result()
{
    uint32_t size = endpointReadcore(EP0OUT, read_buffers[EP0OUT], read_sizes[EP0OUT]);
    read_buffers[EP0OUT] = NULL;
    read_sizes[EP0OUT] = 0;
    return size;
}

void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
    endpointWritecore(EP0IN, buffer, size);
}

void USBPhyHw::ep0_stall(void)
{
    // This will stall both control endpoints
    endpoint_stall(EP0OUT);
}

bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    opStarted |= EP(endpoint);
    read_buffers[endpoint] = data;
    read_sizes[endpoint] = size;
    uint8_t status = SIEselectEndpoint(endpoint);
    if (status & ((1 << 5) | (1 << 6))) {
        // If any buffer has data then set the interrupt flag
        LPC_USB->USBEpIntSet = EP(endpoint);
    }
    return true;
}

uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    opStarted &= ~EP(endpoint);

    uint32_t bytesRead = endpointReadcore(endpoint, read_buffers[endpoint], read_sizes[endpoint]);
    read_buffers[endpoint] = NULL;
    read_sizes[endpoint] = 0;

    // Don't clear isochronous endpoints
    if (!ISO_EP(endpoint)) {
        SIEselectEndpoint(endpoint);
        SIEclearBuffer();
    }

    return bytesRead;
}

bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    opStarted |= EP(endpoint);

    endpointWritecore(endpoint, data, size);
    return true;
}

void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
    opStarted &= ~EP(endpoint);

    // Clear out transfer buffers since the transfer has been aborted
    if (OUT_EP(endpoint)) {
        read_buffers[endpoint] = NULL;
        read_sizes[endpoint] = 0;
    }
}

bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t maxPacket, usb_ep_type_t type)
{
    // Realise an endpoint
    LPC_USB->USBDevIntClr = EP_RLZED;
    LPC_USB->USBReEp |= EP(endpoint);
    LPC_USB->USBEpInd = DESC_TO_PHY(endpoint);
    LPC_USB->USBMaxPSize = maxPacket;

    while (!(LPC_USB->USBDevIntSt & EP_RLZED));
    LPC_USB->USBDevIntClr = EP_RLZED;

    // Map interrupts to USBEpIntSt
    enableEndpointEvent(endpoint);

    // Enable this endpoint
    SIEsetEndpointStatus(endpoint, 0);

    return true;
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    // Unrealise an endpoint

    opStarted &= ~EP(endpoint);

    // Disable this endpoint
    SIEsetEndpointStatus(endpoint, SIE_SES_DA);

    // Clear the given interrupt bit in USBEpIntSt if it is set
    if (LPC_USB->USBEpIntSt & EP(endpoint)) {
        selectEndpointClearInterrupt(endpoint);
    }

    LPC_USB->USBDevIntClr = EP_RLZED;
    LPC_USB->USBReEp &= ~EP(endpoint);

    while (!(LPC_USB->USBDevIntSt & EP_RLZED));
    LPC_USB->USBDevIntClr = EP_RLZED;
}

void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    // Stall an endpoint
    if ((endpoint == EP0IN) || (endpoint == EP0OUT)) {
        // Conditionally stall both control endpoints
        SIEsetEndpointStatus(EP0OUT, SIE_SES_CND_ST);
    } else {
        SIEsetEndpointStatus(endpoint, SIE_SES_ST);
    }
}

void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    // Unstall an endpoint. The endpoint will also be reinitialised
    SIEsetEndpointStatus(endpoint, 0);
}

void USBPhyHw::remote_wakeup(void)
{
    // Remote wakeup
    uint8_t status;

    // Enable USB clocks
    LPC_USB->USBClkCtrl |= DEV_CLK_EN | AHB_CLK_EN;
    while (LPC_USB->USBClkSt != (DEV_CLK_ON | AHB_CLK_ON));

    status = SIEgetDeviceStatus();
    SIEsetDeviceStatus(status & ~SIE_DS_SUS);
}

const usb_ep_table_t *USBPhyHw::endpoint_table()
{
    static const usb_ep_table_t lpc_table = {
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

void USBPhyHw::_usbisr(void)
{
    NVIC_DisableIRQ(USB_IRQn);
    instance->events->start_process();
}

void USBPhyHw::process(void)
{
    uint8_t devStat;

    if (LPC_USB->USBDevIntSt & FRAME) {
        // Start of frame event
        events->sof(SIEgetFrameNumber());
        // Clear interrupt status flag
        LPC_USB->USBDevIntClr = FRAME;

        // There is no ISO interrupt, instead a packet is transferred every SOF
        for (uint32_t i = 0; i < sizeof(ISO_EPS) / sizeof(ISO_EPS[0]); i++) {
            uint8_t endpoint = ISO_EPS[i];
            if (opStarted & EP(endpoint)) {
                opStarted &= ~EP(endpoint);
                if (IN_EP(endpoint)) {
                    events->in(endpoint);
                } else {
                    events->out(endpoint);
                }
            }
        }

    }

    if (LPC_USB->USBDevIntSt & DEV_STAT) {
        // Device Status interrupt
        // Must clear the interrupt status flag before reading the device status from the SIE
        LPC_USB->USBDevIntClr = DEV_STAT;

        // Read device status from SIE
        devStat = SIEgetDeviceStatus();
        //printf("devStat: %d\r\n", devStat);

        if (devStat & SIE_DS_SUS_CH) {
            // Suspend status changed
            if ((devStat & SIE_DS_SUS) != 0) {
                events->suspend(false);
            }
        }

        if (devStat & SIE_DS_RST) {
            // Bus reset
            if ((devStat & SIE_DS_SUS) == 0) {
                events->suspend(true);
            }
            memset(read_buffers, 0, sizeof(read_buffers));
            memset(read_sizes, 0, sizeof(read_sizes));
            opStarted = 0;
            events->reset();
        }
    }

    if (LPC_USB->USBDevIntSt & EP_SLOW) {
        // (Slow) Endpoint Interrupt

        // Process IN packets before SETUP packets
        // Note - order of OUT and SETUP does not matter as OUT packets
        //        are clobbered by SETUP packets and thus ignored.
        //
        // A SETUP packet can arrive at any time where as an IN packet is
        // only sent after calling EP0write and an OUT packet after EP0read.
        // The functions EP0write and EP0read are called only in response to
        // a setup packet or IN/OUT packets sent in response to that
        // setup packet. Therefore, if an IN or OUT packet is pending
        // at the same time as a SETUP packet, the IN or OUT packet belongs
        // to the previous control transfer and should either be processed
        // before the SETUP packet (in the case of IN) or dropped (in the
        // case of OUT as SETUP clobbers the OUT data).
        if (LPC_USB->USBEpIntSt & EP(EP0IN)) {
            selectEndpointClearInterrupt(EP0IN);
            events->ep0_in();
        }

        // Process each endpoint interrupt
        if (LPC_USB->USBEpIntSt & EP(EP0OUT)) {
            if (selectEndpointClearInterrupt(EP0OUT) & SIE_SE_STP) {
                // this is a setup packet
                events->ep0_setup();
            } else {
                events->ep0_out();
            }
        }

        for (uint8_t num = 2; num < 16 * 2; num++) {
            uint8_t endpoint = PHY_TO_DESC(num);
            if (LPC_USB->USBEpIntSt & EP(endpoint)) {
                selectEndpointClearInterrupt(endpoint);
                if (ISO_EP(endpoint)) {
                    // Processing for ISO endpoints done in FRAME handling
                    continue;
                }
                if (opStarted & EP(endpoint)) {
                    opStarted &= ~EP(endpoint);
                    if (IN_EP(endpoint)) {
                        events->in(endpoint);
                    } else {
                        events->out(endpoint);
                    }
                }
            }
        }

        LPC_USB->USBDevIntClr = EP_SLOW;
    }

    NVIC_ClearPendingIRQ(USB_IRQn);
    NVIC_EnableIRQ(USB_IRQn);
}

#endif
