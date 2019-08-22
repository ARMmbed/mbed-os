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
    (defined(TARGET_KL25Z) | defined(TARGET_KL43Z) | \
     defined(TARGET_KL46Z) | defined(TARGET_K20D50M) | \
     defined(TARGET_K64F) | defined(TARGET_K22F) | defined(TARGET_K82F) | \
     defined(TARGET_TEENSY3_1))

#if defined(TARGET_KSDK2_MCUS)
#include "fsl_common.h"
#endif
#include "USBPhyHw.h"
#include "USBEndpoints_Kinetis.h"
#include "mbed_critical.h"

static USBPhyHw *instance;

static volatile int epComplete = 0;

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

#define BD_OWN_MASK        (1<<7)
#define BD_DATA01_MASK     (1<<6)
#define BD_KEEP_MASK       (1<<5)
#define BD_NINC_MASK       (1<<4)
#define BD_DTS_MASK        (1<<3)
#define BD_STALL_MASK      (1<<2)

#define TX    1
#define RX    0
#define ODD   0
#define EVEN  1
// this macro waits a physical endpoint number
#define EP_BDT_IDX(ep, dir, odd) (((ep * 4) + (2 * dir) + (1 *  odd)))

#define SETUP_TOKEN    0x0D
#define IN_TOKEN       0x09
#define OUT_TOKEN      0x01
#define TOK_PID(idx)   ((bdt[idx].info >> 2) & 0x0F)

// for each endpt: 8 bytes
typedef struct BDT {
    uint8_t   info;       // BD[0:7]
    uint8_t   dummy;      // RSVD: BD[8:15]
    uint16_t  byte_count; // BD[16:32]
    uint32_t  address;    // Addr
} BDT;

typedef enum {
    CTRL_XFER_READY,
    CTRL_XFER_IN,
    CTRL_XFER_NONE,
    CTRL_XFER_OUT
} ctrl_xfer_t;

// there are:
//    * 4 bidirectionnal endpt -> 8 physical endpt
//    * as there are ODD and EVEN buffer -> 8*2 bdt
MBED_ALIGN(512) BDT bdt[NUMBER_OF_PHYSICAL_ENDPOINTS * 2];  // 512 bytes aligned!

uint8_t *endpoint_buffer[NUMBER_OF_PHYSICAL_ENDPOINTS * 2];
uint8_t ep0_buffer[2][MAX_PACKET_SIZE_EP0];
uint8_t ep1_buffer[2][MAX_PACKET_SIZE_EP1];
uint8_t ep2_buffer[2][MAX_PACKET_SIZE_EP2];
uint8_t ep3_buffer[2][MAX_PACKET_SIZE_EP3];

static bool setup_suspend = false;
static uint8_t set_addr = 0;
static uint8_t addr = 0;
static ctrl_xfer_t ctrl_xfer = CTRL_XFER_READY;

static uint32_t Data1  = 0x55555555;

static uint32_t frameNumber()
{
    return ((USB0->FRMNUML | (USB0->FRMNUMH << 8)) & 0x07FF);
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
    if (this->events == NULL) {
        sleep_manager_lock_deep_sleep();
    }
    this->events = events;

    // Disable IRQ
    NVIC_DisableIRQ(USB0_IRQn);

#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
    MPU->CESR = 0;
#endif

#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU->CESR = 0;
#endif

#if defined(TARGET_KL43Z) || defined(TARGET_K22F) || defined(TARGET_K64F) || defined(TARGET_K82F)
    // enable USBFS clock
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcIrc48M, 48000000U);
#else
    // choose usb src as PLL
    SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;
    SIM->SOPT2 |= (SIM_SOPT2_USBSRC_MASK | (1 << SIM_SOPT2_PLLFLLSEL_SHIFT));

    // enable OTG clock
    SIM->SCGC4 |= SIM_SCGC4_USBOTG_MASK;
#endif

    // Attach IRQ
    instance = this;

    // USB Module Configuration
    // Set BDT Base Register
    USB0->BDTPAGE1 = (uint8_t)((uint32_t)bdt >> 8);
    USB0->BDTPAGE2 = (uint8_t)((uint32_t)bdt >> 16);
    USB0->BDTPAGE3 = (uint8_t)((uint32_t)bdt >> 24);

    // Clear interrupt flag
    USB0->ISTAT = 0xff;

    // USB Interrupt Enablers
    USB0->INTEN |= USB_INTEN_TOKDNEEN_MASK |
                   USB_INTEN_ERROREN_MASK  |
                   USB_INTEN_USBRSTEN_MASK;

    // Disable weak pull downs
    USB0->USBCTRL &= ~(USB_USBCTRL_PDE_MASK | USB_USBCTRL_SUSP_MASK);

    USB0->USBTRC0 |= 0x40;

    /* Allocate control endpoint buffers */
    endpoint_buffer[EP_BDT_IDX(0, TX, ODD)] = ep0_buffer[TX];
    endpoint_buffer[EP_BDT_IDX(0, RX, ODD)] = ep0_buffer[RX];
    endpoint_buffer[EP_BDT_IDX(1, TX, ODD)] = ep1_buffer[TX];
    endpoint_buffer[EP_BDT_IDX(1, RX, ODD)] = ep1_buffer[RX];
    endpoint_buffer[EP_BDT_IDX(2, TX, ODD)] = ep2_buffer[TX];
    endpoint_buffer[EP_BDT_IDX(2, RX, ODD)] = ep2_buffer[RX];
    endpoint_buffer[EP_BDT_IDX(3, TX, ODD)] = ep3_buffer[TX];
    endpoint_buffer[EP_BDT_IDX(3, RX, ODD)] = ep3_buffer[RX];

    NVIC_SetVector(USB0_IRQn, (uint32_t)&_usbisr);
    NVIC_EnableIRQ(USB0_IRQn);
}

void USBPhyHw::deinit()
{
    disconnect();
    NVIC_DisableIRQ(USB0_IRQn);
    USB0->INTEN = 0;

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
    // enable USB
    USB0->CTL |= USB_CTL_USBENSOFEN_MASK;
    // Pull up enable
    USB0->CONTROL |= USB_CONTROL_DPPULLUPNONOTG_MASK;
}

void USBPhyHw::disconnect()
{
    // disable all endpoints to prevent them from nacking when disconnected
    for (int i = 0; i < 16; i++) {
        USB0->ENDPOINT[i].ENDPT = 0x00;
    }
    // disable USB
    USB0->CTL &= ~USB_CTL_USBENSOFEN_MASK;
    // Pull up disable
    USB0->CONTROL &= ~USB_CONTROL_DPPULLUPNONOTG_MASK;

    while (USB0->ISTAT) {
        USB0->ISTAT = 0xFF;
    }
}

void USBPhyHw::configure()
{
    // not needed
}

void USBPhyHw::unconfigure()
{
    // not needed
}

void USBPhyHw::sof_enable()
{
    USB0->INTEN |= USB_INTEN_SOFTOKEN_MASK;
}

void USBPhyHw::sof_disable()
{
    USB0->INTEN &= ~USB_INTEN_SOFTOKEN_MASK;
}

void USBPhyHw::set_address(uint8_t address)
{
    // we don't set the address now otherwise the usb controller does not ack
    // we set a flag instead
    // see usbisr when an IN token is received
    set_addr = 1;
    addr = address;
}

void USBPhyHw::remote_wakeup()
{
    // TODO
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
    return MAX_PACKET_SIZE_EP0;
}

// read setup packet
void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size)
{
    uint32_t sz;
    endpoint_read_result_core(EP0OUT, buffer, size, &sz);
}

void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
    if (ctrl_xfer == CTRL_XFER_READY) {
        // Transfer is done so ignore call
        return;
    }
    if (ctrl_xfer == CTRL_XFER_IN) {
        ctrl_xfer = CTRL_XFER_READY;
        // Control transfer with a data IN stage.
        // The next packet received will be the status packet - an OUT packet using DATA1
        //
        // PROBLEM:
        // If a Setup packet is received after status packet of
        // a Control In transfer has been received in the RX buffer
        // but before the processor has had a chance the prepare
        // this buffer for the Setup packet, the Setup packet
        // will be dropped.
        //
        // WORKAROUND:
        // Set data toggle to DATA0 so if the status stage of a
        // Control In transfer arrives it will be ACKed by hardware
        // but will be discarded without filling the RX buffer.
        // This allows a subsequent SETUP packet to be stored
        // without any processor intervention.
        Data1 &= ~1UL;  // set DATA0
        endpoint_read_core(EP0OUT, MAX_PACKET_SIZE_EP0);
    } else {
        endpoint_read(EP0OUT, data, size);
    }

    // Clear suspend after the setup stage
    if (setup_suspend) {
        USB0->CTL &= ~USB_CTL_TXSUSPENDTOKENBUSY_MASK;
        setup_suspend = false;
    }
}

uint32_t USBPhyHw::ep0_read_result()
{
    return endpoint_read_result(EP0OUT);
}

void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
    if (ctrl_xfer == CTRL_XFER_READY) {
        // Transfer is done so ignore call
        return;
    }
    if ((ctrl_xfer == CTRL_XFER_NONE) || (ctrl_xfer == CTRL_XFER_OUT)) {
        // Prepare for next setup packet
        endpoint_read_core(EP0OUT, MAX_PACKET_SIZE_EP0);
        ctrl_xfer = CTRL_XFER_READY;
    }
    endpoint_write(EP0IN, buffer, size);

    // Clear suspend after the setup stage
    if (setup_suspend) {
        USB0->CTL &= ~USB_CTL_TXSUSPENDTOKENBUSY_MASK;
        setup_suspend = false;
    }
}

void USBPhyHw::ep0_stall()
{
    if (ctrl_xfer == CTRL_XFER_READY) {
        // Transfer is done so ignore call
        return;
    }
    ctrl_xfer = CTRL_XFER_READY;

    // Clear suspend after the setup stage
    if (setup_suspend) {
        USB0->CTL &= ~USB_CTL_TXSUSPENDTOKENBUSY_MASK;
        setup_suspend = false;
    }

    core_util_critical_section_enter();
    endpoint_stall(EP0OUT);
    // Prepare for next setup packet
    // Note - time between stalling and setting up the endpoint
    //      must be kept to a minimum to prevent a dropped SETUP
    //      packet.
    endpoint_read_core(EP0OUT, MAX_PACKET_SIZE_EP0);
    core_util_critical_section_exit();
}

bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type)
{
    uint32_t handshake_flag = 0;
    uint8_t *buf;

    if (DESC_TO_PHY(endpoint) > NUMBER_OF_PHYSICAL_ENDPOINTS - 1) {
        return false;
    }

    uint32_t log_endpoint = DESC_TO_LOG(endpoint);

    if (type != USB_EP_TYPE_ISO) {
        handshake_flag = USB_ENDPT_EPHSHK_MASK;
    }

    if (DESC_EP_IN(endpoint)) {
        buf = &endpoint_buffer[EP_BDT_IDX(log_endpoint, TX, ODD)][0];
    } else {
        buf = &endpoint_buffer[EP_BDT_IDX(log_endpoint, RX, ODD)][0];
    }

    // IN endpt -> device to host (TX)
    if (DESC_EP_IN(endpoint)) {
        bdt[EP_BDT_IDX(log_endpoint, TX, ODD)].address = (uint32_t) buf;
        bdt[EP_BDT_IDX(log_endpoint, TX, ODD)].info       = 0;
        bdt[EP_BDT_IDX(log_endpoint, TX, EVEN)].address = 0;
        USB0->ENDPOINT[log_endpoint].ENDPT |= handshake_flag |        // ep handshaking (not if iso endpoint)
                                              USB_ENDPT_EPTXEN_MASK;  // en TX (IN) tran
    }
    // OUT endpt -> host to device (RX)
    else {
        bdt[EP_BDT_IDX(log_endpoint, RX, ODD)].byte_count = max_packet;
        bdt[EP_BDT_IDX(log_endpoint, RX, ODD)].address    = (uint32_t) buf;
        bdt[EP_BDT_IDX(log_endpoint, RX, ODD)].info       = BD_DTS_MASK;
        bdt[EP_BDT_IDX(log_endpoint, RX, EVEN)].info       = 0;
        if (log_endpoint == 0) {
            // Prepare for setup packet
            bdt[EP_BDT_IDX(log_endpoint, RX, ODD)].info |= BD_OWN_MASK;
        }
        USB0->ENDPOINT[log_endpoint].ENDPT |= handshake_flag |        // ep handshaking (not if iso endpoint)
                                              USB_ENDPT_EPRXEN_MASK;  // en RX (OUT) tran.
    }

    // First transfer will be a DATA0 packet
    Data1 &= ~(1 << DESC_TO_PHY(endpoint));

    return true;
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    USB0->ENDPOINT[DESC_TO_LOG(endpoint)].ENDPT = 0;
}

void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    USB0->ENDPOINT[DESC_TO_LOG(endpoint)].ENDPT |= USB_ENDPT_EPSTALL_MASK;
}

void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    // Next transfer will be a DATA0 packet
    Data1 &= ~(1 << DESC_TO_PHY(endpoint));
    USB0->ENDPOINT[DESC_TO_LOG(endpoint)].ENDPT &= ~USB_ENDPT_EPSTALL_MASK;
}

bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    uint8_t log = DESC_TO_LOG(endpoint);

    read_buffers[log] = data;
    read_sizes[log] = size;
    return endpoint_read_core(endpoint, size);
}

bool USBPhyHw::endpoint_read_core(usb_ep_t endpoint, uint32_t max_packet)
{
    uint8_t log_endpoint = DESC_TO_LOG(endpoint);

    uint32_t idx = EP_BDT_IDX(log_endpoint, RX, 0);
    bdt[idx].byte_count = max_packet;
    if ((Data1 >> DESC_TO_PHY(endpoint)) & 1) {
        bdt[idx].info = BD_OWN_MASK | BD_DTS_MASK | BD_DATA01_MASK;
    } else {
        bdt[idx].info = BD_OWN_MASK | BD_DTS_MASK;
    }

    Data1 ^= (1 << DESC_TO_PHY(endpoint));
    return true;
}

uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    uint8_t log = DESC_TO_LOG(endpoint);

    uint32_t bytes_read = 0;
    endpoint_read_result_core(endpoint, read_buffers[log], read_sizes[log], &bytes_read);
    read_buffers[log] = NULL;
    read_sizes[log] = 0;
    return bytes_read;
}


bool USBPhyHw::endpoint_read_result_core(usb_ep_t endpoint, uint8_t *data, uint32_t size, uint32_t *bytes_read)
{
    uint32_t n, sz, idx, setup = 0;
    uint8_t not_iso;
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
    sz  = bdt[idx].byte_count;
    not_iso = USB0->ENDPOINT[log_endpoint].ENDPT & USB_ENDPT_EPHSHK_MASK;

    //for isochronous endpoint, we don't wait an interrupt
    if ((log_endpoint != 0) && not_iso && !(epComplete & EP(DESC_TO_PHY(endpoint)))) {
        return false;
    }

    if ((log_endpoint == 0) && (TOK_PID(idx) == SETUP_TOKEN)) {
        setup = 1;
    }

    ep_buf = endpoint_buffer[idx];

    for (n = 0; n < sz; n++) {
        data[n] = ep_buf[n];
    }

    if (setup) {
        // Record the setup type
        if ((data[6] == 0) && (data[7] == 0))  {
            ctrl_xfer = CTRL_XFER_NONE;
        } else {
            uint8_t in_xfer = (data[0] >> 7) & 1;
            ctrl_xfer = in_xfer ? CTRL_XFER_IN : CTRL_XFER_OUT;
        }
    }

    *bytes_read = sz;

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
    bdt[idx].byte_count = size;

    ep_buf = endpoint_buffer[idx];

    for (n = 0; n < size; n++) {
        ep_buf[n] = data[n];
    }

    if ((Data1 >> DESC_TO_PHY(endpoint)) & 1) {
        bdt[idx].info = BD_OWN_MASK | BD_DTS_MASK | BD_DATA01_MASK;
    } else {
        bdt[idx].info = BD_OWN_MASK | BD_DTS_MASK;
    }

    Data1 ^= (1 << DESC_TO_PHY(endpoint));

    return true;
}

void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
    uint8_t dir = DESC_EP_IN(endpoint) ? TX : RX;
    uint32_t idx = EP_BDT_IDX(DESC_TO_LOG(endpoint), dir, 0);
    bdt[idx].info &= ~BD_OWN_MASK;
}

void USBPhyHw::process()
{
    uint8_t i;
    uint8_t istat = USB0->ISTAT & USB0->INTEN;

    // reset interrupt
    if (istat & USB_ISTAT_USBRST_MASK) {
        // disable all endpt
        for (i = 0; i < 16; i++) {
            USB0->ENDPOINT[i].ENDPT = 0x00;
        }

        // enable control endpoint
        setup_suspend = false;
        endpoint_add(EP0OUT, MAX_PACKET_SIZE_EP0, USB_EP_TYPE_CTRL);
        endpoint_add(EP0IN, MAX_PACKET_SIZE_EP0, USB_EP_TYPE_CTRL);

        Data1 = 0x55555555;
        USB0->CTL |=  USB_CTL_ODDRST_MASK;
        USB0->CTL &= ~USB_CTL_TXSUSPENDTOKENBUSY_MASK;

        USB0->ISTAT   =  0xFF;  // clear all interrupt status flags
        USB0->ERRSTAT =  0xFF;  // clear all error flags
        USB0->ERREN   =  0xFF;  // enable error interrupt sources
        USB0->ADDR    =  0x00;  // set default address

        memset(read_buffers, 0, sizeof(read_buffers));
        memset(read_sizes, 0, sizeof(read_sizes));

        // reset bus for USBDevice layer
        events->reset();

        NVIC_ClearPendingIRQ(USB0_IRQn);
        NVIC_EnableIRQ(USB0_IRQn);
        return;
    }

    // resume interrupt
    if (istat & USB_ISTAT_RESUME_MASK) {
        USB0->ISTAT = USB_ISTAT_RESUME_MASK;
        events->suspend(false);
    }

    // SOF interrupt
    if (istat & USB_ISTAT_SOFTOK_MASK) {
        USB0->ISTAT = USB_ISTAT_SOFTOK_MASK;
        // SOF event, read frame number
        events->sof(frameNumber());
    }

    // stall interrupt
    if (istat & 1 << 7) {
        if (USB0->ENDPOINT[0].ENDPT & USB_ENDPT_EPSTALL_MASK) {
            USB0->ENDPOINT[0].ENDPT &= ~USB_ENDPT_EPSTALL_MASK;
        }
        USB0->ISTAT = USB_ISTAT_STALL_MASK;
    }

    // token interrupt
    if (istat & USB_ISTAT_TOKDNE_MASK) {
        uint32_t num  = (USB0->STAT >> 4) & 0x0F;
        uint32_t dir  = (USB0->STAT >> 3) & 0x01;
        uint32_t ev_odd = (USB0->STAT >> 2) & 0x01;
        int phy_ep = (num << 1) | dir;

        bool tx_en = (USB0->ENDPOINT[PHY_TO_LOG(phy_ep)].ENDPT & USB_ENDPT_EPTXEN_MASK) ? true : false;
        bool rx_en = (USB0->ENDPOINT[PHY_TO_LOG(phy_ep)].ENDPT & USB_ENDPT_EPRXEN_MASK) ? true : false;

        // setup packet
        if (tx_en && (num == 0) && (TOK_PID((EP_BDT_IDX(num, dir, ev_odd))) == SETUP_TOKEN)) {
            setup_suspend = true;
            Data1 |= 0x02 | 0x01; // set DATA1 for TX and RX
            bdt[EP_BDT_IDX(0, TX, EVEN)].info &= ~BD_OWN_MASK;
            bdt[EP_BDT_IDX(0, TX, ODD)].info  &= ~BD_OWN_MASK;

            // EP0 SETUP event (SETUP data received)
            events->ep0_setup();

        } else {
            // OUT packet
            if (rx_en && (TOK_PID((EP_BDT_IDX(num, dir, ev_odd))) == OUT_TOKEN)) {
                if (num == 0) {
                    events->ep0_out();
                } else {
                    epComplete |= EP(phy_ep);
                    events->out(PHY_TO_DESC(phy_ep));
                }
            }

            // IN packet
            if (tx_en && (TOK_PID((EP_BDT_IDX(num, dir, ev_odd))) == IN_TOKEN)) {
                if (num == 0) {
                    events->ep0_in();
                    if (set_addr == 1) {
                        USB0->ADDR = addr & 0x7F;
                        set_addr = 0;
                    }
                } else {
                    epComplete |= EP(phy_ep);
                    events->in(PHY_TO_DESC(phy_ep));
                }
            }
        }

        USB0->ISTAT = USB_ISTAT_TOKDNE_MASK;
    }

    // sleep interrupt
    if (istat & 1 << 4) {
        USB0->ISTAT = USB_ISTAT_SLEEP_MASK;
        events->suspend(true);
    }

    // error interrupt
    if (istat & USB_ISTAT_ERROR_MASK) {
        USB0->ERRSTAT = 0xFF;
        USB0->ISTAT = USB_ISTAT_ERROR_MASK;
    }

    // Check if the suspend condition should be removed here
    // 1. Don't attempt to clear USB_CTL_TXSUSPENDTOKENBUSY_MASK if it isn't set. This
    //      is to avoid potential race conditions.
    // 2. If a setup packet is being processed then remove suspend on the next control transfer rather than here
    // 3. Process all pending packets before removing suspend
    bool suspended = (USB0->CTL & USB_CTL_TXSUSPENDTOKENBUSY_MASK) != 0;
    if (suspended && !setup_suspend && ((USB0->ISTAT & USB_ISTAT_TOKDNE_MASK) == 0)) {
        USB0->CTL &= ~USB_CTL_TXSUSPENDTOKENBUSY_MASK;
    }

    NVIC_ClearPendingIRQ(USB0_IRQn);
    NVIC_EnableIRQ(USB0_IRQn);
}

void USBPhyHw::_usbisr(void)
{
    NVIC_DisableIRQ(USB0_IRQn);
    instance->events->start_process();
}

#endif
