/* mbed Microcontroller Library
 * Copyright (c) 2019-2020 ARM Limited
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
#include "USBEndpoints_Nuvoton.h"
#include "mbed_critical.h"

static USBPhyHw *instance;

#if defined (TARGET_M451)
#undef  MBED_CONF_TARGET_USB_DEVICE_HSUSBD
#define MBED_CONF_TARGET_USB_DEVICE_HSUSBD 0  /* USB 1.1 Only */
#elif defined (TARGET_M480)
#define USBD_SET_ADDRESS         0x05ul
#elif defined (TARGET_M2351) || defined(TARGET_M261)
#undef  MBED_CONF_TARGET_USB_DEVICE_HSUSBD
#define MBED_CONF_TARGET_USB_DEVICE_HSUSBD 0  /* USB 1.1 Only */
#define USBD_SET_ADDRESS         0x05ul
#elif defined (TARGET_NANO100)
#undef  MBED_CONF_TARGET_USB_DEVICE_HSUSBD
#define MBED_CONF_TARGET_USB_DEVICE_HSUSBD 0  /* USB 1.1 Only */
#elif defined (TARGET_NUC472)
#define HSUSBD USBD 
#undef  MBED_CONF_TARGET_USB_DEVICE_HSUSBD
#define MBED_CONF_TARGET_USB_DEVICE_HSUSBD 1  /* USB 2.0 Only */
#endif

#if defined(DEVICE_USBDEVICE) && (DEVICE_USBDEVICE == 1)

extern "C" void USBD_IRQHandler(void);

void chip_config(void)
{
#if defined(TARGET_M451)
    /* Enable USBD module clock */
    CLK_EnableModuleClock(USBD_MODULE);

    CLK_SetModuleClock(USBD_MODULE, 0, CLK_CLKDIV0_USB(3));

    /* Enable USB LDO33 */
    SYS->USBPHY = SYS_USBPHY_LDO33EN_Msk;

#elif defined(TARGET_M480)
#if (MBED_CONF_TARGET_USB_DEVICE_HSUSBD == 0)
    /* Configure USB to Device mode */
    SYS->USBPHY = (SYS->USBPHY & ~SYS_USBPHY_USBROLE_Msk) | SYS_USBPHY_USBROLE_STD_USBD;
    
    /* Enable USB PHY */
    SYS->USBPHY |= SYS_USBPHY_USBEN_Msk | SYS_USBPHY_SBO_Msk;

    /* Select IP clock source */
    CLK->CLKDIV0 = (CLK->CLKDIV0 & ~CLK_CLKDIV0_USBDIV_Msk) | CLK_CLKDIV0_USB(4);

    /* Enable IP clock */
    CLK_EnableModuleClock(USBD_MODULE);

    /* Configure pins for USB 1.1 port: VBUS/D+/D-/ID */
    pin_function(PA_12, SYS_GPA_MFPH_PA12MFP_USB_VBUS);
    pin_function(PA_13, SYS_GPA_MFPH_PA13MFP_USB_D_N);
    pin_function(PA_14, SYS_GPA_MFPH_PA14MFP_USB_D_P);
    pin_function(PA_15, (int) SYS_GPA_MFPH_PA15MFP_USB_OTG_ID);
#else
    /* Configure HSUSB to Device mode */
    SYS->USBPHY = (SYS->USBPHY & ~SYS_USBPHY_HSUSBROLE_Msk) | SYS_USBPHY_HSUSBROLE_STD_USBD;
    /* Enable HSUSB PHY */
    SYS->USBPHY = (SYS->USBPHY & ~(SYS_USBPHY_HSUSBEN_Msk | SYS_USBPHY_HSUSBACT_Msk)) | SYS_USBPHY_HSUSBEN_Msk;
    /* Delay >10 us and then switch HSUSB PHY from reset state to active state */
    wait_us(10);
    SYS->USBPHY |= SYS_USBPHY_HSUSBACT_Msk;

    /* Enable USBD module clock */
    CLK_EnableModuleClock(HSUSBD_MODULE);
#endif

#elif defined (TARGET_M2351) || defined(TARGET_M261)

    /* Select USBD */
    SYS->USBPHY = (SYS->USBPHY & ~SYS_USBPHY_USBROLE_Msk) | SYS_USBPHY_OTGPHYEN_Msk | SYS_USBPHY_SBO_Msk;

    /* Enable IP clock */
    CLK_EnableModuleClock(USBD_MODULE);

    /* Select IP clock source */
    CLK_SetModuleClock(USBD_MODULE, CLK_CLKSEL0_USBSEL_HIRC48, CLK_CLKDIV0_USB(1));

    /* USBD multi-function pins for VBUS, D+, D-, and ID pins */
    SYS->GPA_MFPH &= ~(SYS_GPA_MFPH_PA12MFP_Msk | SYS_GPA_MFPH_PA13MFP_Msk | SYS_GPA_MFPH_PA14MFP_Msk | SYS_GPA_MFPH_PA15MFP_Msk);
    SYS->GPA_MFPH |= (SYS_GPA_MFPH_PA12MFP_USB_VBUS | SYS_GPA_MFPH_PA13MFP_USB_D_N | SYS_GPA_MFPH_PA14MFP_USB_D_P | SYS_GPA_MFPH_PA15MFP_USB_OTG_ID);


#elif defined (TARGET_NANO100)

    /* Select IP clock source */
    CLK_SetModuleClock(USBD_MODULE, 0, CLK_USB_CLK_DIVIDER(2));
    /* Enable IP clock */
    CLK_EnableModuleClock(USBD_MODULE);

#elif defined (TARGET_NUC472)

    /* Enable USBD module clock */
    CLK_EnableModuleClock(USBD_MODULE);

    /* Enable USB PHY's LDO33. Run as USB device. */
    SYS->USBPHY = SYS_USBPHY_USBROLE_OTG_V33_EN | SYS_USBPHY_USBROLE_STD_USBD;

#endif
}

#if (MBED_CONF_TARGET_USB_DEVICE_HSUSBD == 0)

// Conversion macros
#define DESC_TO_LOG(endpoint)   ((endpoint) & 0xF)
#define NU_EPH2EPL(ep)           ep
#define NU_EPL2EPH(ep)           ep
#define HW_TO_DESC(endpoint) (endpoint|(((endpoint&1)?0x0:0x80)))

/* Global variables for Control Pipe */
#if defined(TARGET_M2351) || defined(TARGET_M261)
extern uint8_t g_USBD_au8SetupPacket[];        /*!< Setup packet buffer */
uint8_t* g_usbd_SetupPacket=g_USBD_au8SetupPacket;
#else
extern uint8_t g_usbd_SetupPacket[];        /*!< Setup packet buffer */
#endif

#define CEP_BUF_BASE 8
#define MAX_CEP_SIZE 64

static volatile uint32_t s_ep_compl = 0;
static volatile uint32_t s_ep_buf_ind = 8;
static volatile uint32_t s_ep0_max_packet_size = MAX_CEP_SIZE;
static volatile uint8_t s_usb_addr = 0;
static volatile uint8_t s_ep_data_bit[NUMBER_OF_PHYSICAL_ENDPOINTS] = {1};
static volatile uint8_t s_ep_mxp[NUMBER_OF_PHYSICAL_ENDPOINTS] = {0};
volatile uint8_t s_ep_valid[NUMBER_OF_PHYSICAL_ENDPOINTS] = {0};
extern volatile uint8_t *g_usbd_CtrlInPointer;
extern volatile uint32_t g_usbd_CtrlInSize;
extern volatile uint8_t *g_usbd_CtrlOutPointer;
extern volatile uint32_t g_usbd_CtrlOutSize;
extern volatile uint32_t g_usbd_CtrlOutSizeLimit;
extern volatile uint32_t g_usbd_UsbConfig;
extern volatile uint32_t g_usbd_CtrlMaxPktSize;
extern volatile uint32_t g_usbd_UsbAltInterface;
volatile uint32_t g_usbd_CepTransferLen = 0;
volatile uint32_t frame_cnt = 0;


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

/* 
  Initialize this USBPhy instance.
  This function must be called before calling any other functions of this class, unless specifically noted. 
*/
void USBPhyHw::init(USBPhyEvents *events)
{
    if (this->events == NULL) {
        sleep_manager_lock_deep_sleep();
    }
    this->events = events;

    SYS_UnlockReg();

    s_ep_buf_ind = 0;

    chip_config();
    
    /* Initial USB engine */
#if defined (TARGET_NANO100)
    USBD->CTL = 0x29f;
    USBD->PDMA |= USBD_PDMA_BYTEM_Msk;
#else
    USBD->ATTR = 0x7D0;
#endif
    /* Set SE0 (disconnect) */
    USBD_SET_SE0();

    NVIC_SetVector(USBD_IRQn, (uint32_t) &USBD_IRQHandler);
    NVIC_EnableIRQ(USBD_IRQn);

    instance = this;
}

void USBPhyHw::deinit()
{
    NVIC_DisableIRQ(USBD_IRQn);
    USBD_SET_SE0();
    USBD_DISABLE_PHY();

    if (events != NULL)
    {
        sleep_manager_unlock_deep_sleep();
    }
    events = NULL;
}

/* 
  Check if USB power is present.
  Devices which don't support checking the USB power state must always return true. 
*/
bool USBPhyHw::powered()
{
    return true;
}

void EpInit(void);

void USBPhyHw::connect()
{
    int volatile i = 0;
    memset(read_buffers, 0, sizeof(read_buffers));
    memset(read_sizes, 0, sizeof(read_sizes));

    s_ep_compl = 0;

    EpInit();
    /* Disable software-disconnect function */
    USBD_CLR_SE0();

    /* Clear USB-related interrupts before enable interrupt */
    USBD_CLR_INT_FLAG(USBD_INT_BUS | USBD_INT_USB | USBD_INT_FLDET | USBD_INT_WAKEUP);

    /* Enable USB-related interrupts. */
    USBD_ENABLE_INT(USBD_INT_BUS | USBD_INT_USB | USBD_INT_FLDET | USBD_INT_WAKEUP);
}

/* Make the USB phy visible to the USB host.
   Enable either the D+ or D- pullup so the host can detect the presence of this device. */
void USBPhyHw::disconnect()
{
    uint32_t volatile i = 0;
#if 1
#if defined (TARGET_NANO100)
    SYS->IPRST_CTL2 = SYS_IPRST_CTL2_USBD_RST_Msk;
#else
    SYS->IPRST1 =  SYS_IPRST1_USBDRST_Msk;
#endif
    for(i=0;i<1000;i++)
        ;

#if defined (TARGET_NANO100)
    SYS->IPRST_CTL2 = 0;
#else
    SYS->IPRST1 =  0;
#endif

    s_ep_buf_ind = 0;
    
    /* Initial USB engine */
#if defined (TARGET_NANO100)
    USBD->CTL = 0x29f;
    USBD->PDMA |= USBD_PDMA_BYTEM_Msk;
#else
    USBD->ATTR = 0x7D0;
#endif
#endif
    /* Set SE0 (disconnect) */
    USBD_SET_SE0();

    memset(read_buffers, 0, sizeof(read_buffers));
    memset(read_sizes, 0, sizeof(read_sizes));

    frame_cnt = 0;

    EpInit();

    /* Clear USB-related interrupts before disable interrupt */
    USBD_CLR_INT_FLAG(USBD_INT_BUS | USBD_INT_USB | USBD_INT_FLDET | USBD_INT_WAKEUP);

    /* Disable USB-related interrupts. */
    USBD->INTEN = 0;
}


void EpInit(void)
{
    uint32_t volatile i = 0;

#if defined (TARGET_NANO100)
    USBD->BUFSEG = 0;
#else
    USBD->STBUFSEG = 0;
#endif

    memset((void *)s_ep_valid, 0, sizeof(s_ep_valid));

    /* EP0 ==> control IN endpoint, address 0 */
    USBD_CONFIG_EP(EP0, USBD_CFG_CSTALL | USBD_CFG_EPMODE_IN | 0);

    /* Buffer range for EP0 */
    USBD_SET_EP_BUF_ADDR(EP0, CEP_BUF_BASE);

    /* EP1 ==> control OUT endpoint, address 0 */
    USBD_CONFIG_EP(EP1, USBD_CFG_CSTALL | USBD_CFG_EPMODE_OUT | 0);

    /* Buffer range for EP1 */
    USBD_SET_EP_BUF_ADDR(EP1, CEP_BUF_BASE);

    s_ep_buf_ind = CEP_BUF_BASE + MAX_CEP_SIZE;

    for(i=2;i<NUMBER_OF_PHYSICAL_ENDPOINTS;i++)
    {
        USBD_SET_EP_BUF_ADDR(i, s_ep_buf_ind);
        s_ep_buf_ind += 64;
    }
}

/* Set this device to the configured state.
   Enable added endpoints if they are not enabled already. */
void USBPhyHw::configure()
{
    // not needed
}

/* 
   Leave the configured state.
   This is a notification to the USBPhy indicating that the device is leaving the configured state. The USBPhy can disable all endpoints other than endpoint 0. 
*/
void USBPhyHw::unconfigure()
{

}

/* Enable the start of frame interrupt. */
void USBPhyHw::sof_enable()
{

}

/* Disable the start of frame interrupt. */
void USBPhyHw::sof_disable()
{

}

/* Set the USBPhy's address. */
void USBPhyHw::set_address(uint8_t address)
{
    s_usb_addr = address;
}

/* Wake upstream devices */
void USBPhyHw::remote_wakeup()
{
#if defined (TARGET_NANO100)
    USBD->CTL |= USBD_CTL_RWAKEUP_Msk;
#else
    USBD->ATTR |= USBD_ATTR_RWAKEUP_Msk;
#endif
}

/*
  Get the endpoint table.
  This function returns a table which describes the endpoints can be used, the functionality of those endpoints and the resource cost.
*/
const usb_ep_table_t *USBPhyHw::endpoint_table()
{
    static const usb_ep_table_t endpoint_table = {
        1, // No cost per endpoint - everything allocated up front
        {
            {USB_EP_ATTR_ALLOW_CTRL | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_CTRL | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_IN ,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_OUT,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_IN ,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_OUT,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_IN ,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_OUT,         0, 0},
#if (!TARGET_M451)
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_IN ,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_OUT,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_IN ,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_OUT,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_IN ,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_OUT,         0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN,             0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_OUT,            0, 0},
#endif
        }
    };
    return &endpoint_table;
}
/*
  Set wMaxPacketSize of endpoint 0.
 */
uint32_t USBPhyHw::ep0_set_max_packet(uint32_t max_packet)
{
    s_ep0_max_packet_size = max_packet;
    return s_ep0_max_packet_size;
}

/* Read the contents of the SETUP packet. */
void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size)
{
    USBD_MemCopy(g_usbd_SetupPacket, (uint8_t *)USBD_BUF_BASE, 8);
    if (buffer)
        USBD_MemCopy(buffer, g_usbd_SetupPacket, 8);
    USBD_SET_PAYLOAD_LEN(EP1, s_ep0_max_packet_size);
}

/*
  Start receiving a packet of up to wMaxPacketSize on endpoint 0.
*/
void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
    read_buffers[0] = data;
    read_sizes[0] = size;
}
/*
  Read the contents of a received packet.
*/
uint32_t USBPhyHw::ep0_read_result()
{
    uint32_t i;
    uint8_t *buf = (uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP1));
    uint8_t *buffer = read_buffers[0];
    uint32_t ceprxcnt = USBD_GET_PAYLOAD_LEN(EP1);
    if(buffer)
    {
        for (i = 0; i < ceprxcnt; i ++)
            buffer[i] = buf[i];
    }
    USBD_SET_PAYLOAD_LEN(EP1, read_sizes[0]);
    if(read_sizes[0] == 0)
        ceprxcnt = 0;
    read_buffers[0] = 0;
    read_sizes[0] = 0;
    return ceprxcnt;
}

/* Write a packet on endpoint 0. */
void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
    if (buffer && size)
    {
        if(s_ep_data_bit[0] & 1)
            USBD_SET_DATA1(EP0);
        else
            USBD_SET_DATA0(EP0);
        s_ep_data_bit[0]++;
        USBD_MemCopy((uint8_t *)USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0), buffer, size);
        USBD_SET_PAYLOAD_LEN(EP0, size);
        if(size < s_ep0_max_packet_size)
            s_ep_data_bit[0] = 1;
    }
    else
    {
        s_ep_data_bit[0] = 1;
        USBD_SET_DATA1(EP0);
        wait_us(500);
        USBD_SET_PAYLOAD_LEN(EP0, 0);

        if((USBD->EPSTS & 0xF) == 0x00)    /* NAK */
            USBD_SET_PAYLOAD_LEN(EP0, 0);
    }
}

void stallEndpoint(uint8_t endpoint)
{
    uint32_t ep_logic_index = DESC_TO_LOG(endpoint);
    if (ep_logic_index >= NUMBER_OF_LOGICAL_ENDPOINTS)
        return;

    USBD_SetStall(ep_logic_index);
}

/* Protocol stall on endpoint 0.
   Stall all IN and OUT packets on endpoint 0 until a setup packet is received.
   Note The stall is cleared automatically when a setup packet is received 
*/
void USBPhyHw::ep0_stall()
{
    stallEndpoint(0);
    stallEndpoint(1);
}

/* Configure and enable an endpoint. */
/*
   endpoint	Endpoint to configure and enable
   max_packet	The maximum packet size that can be sent or received
   type	The type of endpoint this should be configured as - USB_EP_TYPE_BULK, USB_EP_TYPE_INT or USB_EP_TYPE_ISO
   This function cannot be used to configure endpoint 0. That must be done with ep0_set_max_packet
*/
bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type)
{
    uint32_t ep_type = 0;
    uint32_t ep_logic_index = DESC_TO_LOG(endpoint);
    uint32_t ep_hw_index = NU_EPL2EPH(ep_logic_index);
    uint32_t ep_dir = (endpoint & 0x80) ? USBD_CFG_EPMODE_IN : USBD_CFG_EPMODE_OUT;

    if(type == USB_EP_TYPE_ISO)
        ep_type = USBD_CFG_TYPE_ISO;

    USBD_CONFIG_EP(ep_hw_index, ep_dir | ep_type | ep_logic_index);

    /* Buffer range */
//    USBD_SET_EP_BUF_ADDR(ep_hw_index, s_ep_buf_ind);

    s_ep_mxp[ep_logic_index] = max_packet;
//    s_ep_buf_ind += max_packet;
    s_ep_valid[ep_logic_index] = 1;
    return true;
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{

}

/*
  Perform a functional stall on the given endpoint.
  Set the HALT feature for this endpoint so that all further communication is aborted. 
*/
void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    USBD_SetStall(DESC_TO_LOG(endpoint));
}

/*
   Unstall the endpoint.
   Clear the HALT feature on this endpoint so communication can resume. 
 */
void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    uint32_t ep_logic_index = DESC_TO_LOG(endpoint);
    uint32_t ep_hw_index = NU_EPL2EPH(ep_logic_index);
    if (ep_logic_index >= NUMBER_OF_LOGICAL_ENDPOINTS)
        return;
    USBD_ClearStall(ep_logic_index);
    USBD_SET_DATA0(ep_hw_index);
    s_ep_compl &= ~(1 << ep_logic_index);
    s_ep_valid[ep_logic_index] = 1;
}

/* Start a read on the given endpoint. */
bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    /* Store the buffer address & length */
    uint32_t ep_logic_index = DESC_TO_LOG(endpoint);
    uint32_t ep_hw_index = NU_EPL2EPH(ep_logic_index);
    USBD_SET_PAYLOAD_LEN(ep_hw_index,s_ep_mxp[ep_logic_index]);
    read_buffers[ep_logic_index] = data;
    read_sizes[ep_logic_index] = size;
    return true;
}

/* 
  Finish a read on the given endpoint 
  Returns The number of bytes received
*/
uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    uint8_t ep_logic_index = DESC_TO_LOG(endpoint);
    uint32_t bytes_read = 0;
    endpoint_read_result_core(endpoint, read_buffers[ep_logic_index], read_sizes[ep_logic_index], &bytes_read);
    read_buffers[ep_logic_index] = NULL;
    read_sizes[ep_logic_index] = 0;
    return bytes_read;
}

bool USBPhyHw::endpoint_read_result_core(usb_ep_t endpoint, uint8_t *data, uint32_t size, uint32_t *bytes_read)
{
    uint32_t i;
    uint8_t ep_logic_index = DESC_TO_LOG(endpoint);
    uint32_t ep_hw_index = NU_EPL2EPH(ep_logic_index);
    uint8_t *buf = (uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(ep_hw_index));
    uint32_t eprxcnt = USBD_GET_PAYLOAD_LEN(ep_hw_index);
    for (i = 0; i < eprxcnt; i ++)
        data[i] = buf[i];
    *bytes_read = eprxcnt;
    return true;
}

/*
  Start a write on the given endpoint.
  true if the data was prepared for transmit, false otherwise
*/
bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    uint32_t ep_logic_index = DESC_TO_LOG(endpoint);
    uint32_t ep_hw_index = NU_EPL2EPH(ep_logic_index);

    if(ep_logic_index == 0)
        return false;
    else
    {
        uint8_t *buf;
        uint32_t i=0;
        if(s_ep_compl & (1 << ep_logic_index))
            return false;

        s_ep_compl |= (1 << ep_logic_index);

        buf = (uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(ep_hw_index));

        for(i=0; i<size; i++)
            buf[i] = data[i];

        /* Set transfer length and trigger IN transfer */
        USBD_SET_PAYLOAD_LEN(ep_hw_index, size);
    }
    return true;
}

/* Abort the current transfer if it has not yet been sent. */
void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
    uint32_t ep_logic_index = DESC_TO_LOG(endpoint);
    uint32_t ep_hw_index = NU_EPL2EPH(ep_logic_index);
    USBD_STOP_TRANSACTION(ep_hw_index);
    s_ep_compl &= ~(1 << ep_logic_index);
    s_ep_valid[ep_logic_index] = 0;
}

/* 
  Callback used for performing USB processing.
  USBPhy processing should be triggered by calling USBPhyEvents::start_process and done inside process. All USBPhyEvents callbacks aside from USBPhyEvents::start_process must be called in the context of process
*/
void USBPhyHw::process()
{
    uint32_t u32IntSts = USBD_GET_INT_FLAG();
    uint32_t u32State = USBD_GET_BUS_STATE();

#if defined (TARGET_NANO100)
    if(u32IntSts & USBD_INTSTS_FLD_STS_Msk)
#else
    if(u32IntSts & USBD_INTSTS_VBDETIF_Msk)
#endif
    {
        /* Floating detect */
#if defined (TARGET_NANO100)
        USBD_CLR_INT_FLAG(USBD_INTSTS_FLD_STS_Msk);
#else
        USBD_CLR_INT_FLAG(USBD_INTSTS_VBDETIF_Msk);
#endif
        if(USBD_IS_ATTACHED())
        {
            /* USB Plug In */
            USBD_ENABLE_USB();
        } else {
            /* USB Un-plug */
            USBD_DISABLE_USB();
          
            disconnect();
        }
    }

#if defined (TARGET_NANO100)
    if(u32IntSts & USBD_INTSTS_BUS_STS_Msk)
#else
    if(u32IntSts & USBD_INTSTS_BUSIF_Msk)
#endif
    {
        /* Clear event flag */
#if defined (TARGET_NANO100)
        USBD_CLR_INT_FLAG(USBD_INTSTS_BUS_STS_Msk);
        if(u32State & USBD_BUSSTS_USBRST_Msk)
#else
        USBD_CLR_INT_FLAG(USBD_INTSTS_BUSIF_Msk);
        if(u32State & USBD_ATTR_USBRST_Msk)
#endif
        {
            /* Bus reset */
            USBD_ENABLE_USB();
            
            USBD_SwReset();

            connect();
            events->reset();
        }

#if defined (TARGET_NANO100)
        if(u32State & USBD_BUSSTS_SUSPEND_Msk)
#else
        if(u32State & USBD_ATTR_SUSPEND_Msk)
#endif
        {
            /* Enable USB but disable PHY */
            USBD_DISABLE_PHY();
        }
#if defined (TARGET_NANO100)
        if(u32State & USBD_BUSSTS_RESUME_Msk)
#else
        if(u32State & USBD_ATTR_RESUME_Msk)
#endif
        {
            /* Enable USB and enable PHY */
            USBD_ENABLE_USB();
        }
    }

#if defined (TARGET_NANO100)
    if(u32IntSts & USBD_INTSTS_USB_STS_Msk)
#else
    if(u32IntSts & USBD_INTSTS_USBIF_Msk)
#endif
    {
        /* USB event */
        if(u32IntSts & USBD_INTSTS_SETUP_Msk)
        {
            /* Setup packet */
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_SETUP_Msk);
            /* Clear the data IN/OUT ready flag of control end-points */
            USBD_STOP_TRANSACTION(EP0);
            USBD_STOP_TRANSACTION(EP1);
            events->ep0_setup();
        }

        /* EP events */
        if(u32IntSts & USBD_INTSTS_EP0)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP0);
            /* Control IN */
            events->ep0_in();
            /* In ACK for Set address */
#if defined(TARGET_M480) || defined(TARGET_M451)
            if((g_usbd_SetupPacket[0] == REQ_STANDARD) && (g_usbd_SetupPacket[1] == USBD_SET_ADDRESS))
#else
            if((g_usbd_SetupPacket[0] == REQ_STANDARD) && (g_usbd_SetupPacket[1] == SET_ADDRESS))
#endif
            {
                if((USBD_GET_ADDR() != s_usb_addr) && (USBD_GET_ADDR() == 0))
                {
                    USBD_SET_ADDR(s_usb_addr);
                }
            }
        }
        if(u32IntSts & USBD_INTSTS_EP1)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP1);

            /* Control OUT */
            events->ep0_out();
        }

        uint32_t gintsts_epx = (u32IntSts >> 18) & 0x3F;
        uint32_t ep_hw_index = 2;
        while (gintsts_epx)
        {
            if(gintsts_epx & 0x01)
            {
                uint32_t ep_status;
#if defined(TARGET_M451)
                ep_status = (USBD->EPSTS >> (ep_hw_index * 3 + 8)) & 0x7;
#elif defined(TARGET_NANO100)
                if(ep_hw_index < 6)
                    ep_status = (USBD->EPSTS >> (ep_hw_index * 4 + 8)) & 0xF;
                else
                    ep_status = (USBD->EPSTS2 >> ((ep_hw_index - 6) * 4)) & 0x7;
#elif defined(TARGET_M480) || defined(TARGET_M2351) || defined(TARGET_M261)
                if(ep_hw_index < 8)
                    ep_status = (USBD->EPSTS0 >> (ep_hw_index * 4)) & 0xF;
                else
                    ep_status = (USBD->EPSTS1 >> ((ep_hw_index - 8) * 4)) & 0xF;
#endif
                /* Clear event flag */
                USBD_CLR_INT_FLAG(1 << (ep_hw_index + 16));

                if(ep_status == 0x02 || ep_status == 0x06 || (ep_status == 0x07 && (ep_hw_index & 0x01) == 1))
                {   /* RX */
                    s_ep_compl &= ~(1 << (NU_EPH2EPL(ep_hw_index)));
                    if(s_ep_valid[NU_EPH2EPL(ep_hw_index)])
                        events->out(HW_TO_DESC(ep_hw_index));
                    s_ep_valid[NU_EPH2EPL(ep_hw_index)] = 1;
                }
                else if(ep_status == 0x00 || ep_status == 0x07)
                {   /* TX */
                    s_ep_compl &= ~(1 << (NU_EPH2EPL(ep_hw_index)));
                    if(s_ep_valid[NU_EPH2EPL(ep_hw_index)])
                        events->in(HW_TO_DESC(ep_hw_index));
                    s_ep_valid[NU_EPH2EPL(ep_hw_index)] = 1;
                }
            }
            gintsts_epx = gintsts_epx >> 1;
            ep_hw_index++;
        }
    }
}

extern "C" void USBD_IRQHandler(void)
{
    instance->events->start_process();
}
#else
/* USB 2.0 Engine */
static volatile int epComplete = 0;

// Conversion macros
#define DESC_TO_LOG(endpoint)   ((endpoint) & 0xF)
#define NU_EPH2EPL(ep)          ((ep) + 1)
#define NU_EPL2EPH(ep)          (ep - 1)

#define EP_DIR_Msk        0x80
#define EP_DIR_OUT        0x00
#define EP_DIR_IN         0x80

#if defined (TARGET_M480)
#define HSUSBD_GET_EP_MAX_PAYLOAD(ep)     HSUSBD->EP[ep].EPMPS
#define HSUSBD_GET_EP_DATA_COUNT(ep)      (HSUSBD->EP[ep].EPDATCNT & 0xFFFFF)
#define HSUSBD_SET_EP_SHORT_PACKET(ep)    HSUSBD->EP[ep].EPRSPCTL = ((HSUSBD->EP[ep].EPRSPCTL & 0x10) | 0x40)
#define HSUSBD_GET_EP_INT_EN(ep)          HSUSBD->EP[ep].EPINTEN
#elif defined (TARGET_NUC472)
#define USBD_GET_EP_MAX_PAYLOAD(ep)       *((__IO uint32_t *) ((uint32_t)&USBD->EPAMPS + (uint32_t)(ep*0x28)))
#define USBD_GET_EP_DATA_COUNT(ep)        *((__IO uint32_t *) ((uint32_t)&USBD->EPADATCNT + (uint32_t)(ep*0x28)))
#define USBD_SET_EP_SHORT_PACKET(ep)      *((__IO uint32_t *) ((uint32_t)&USBD->EPARSPCTL + (uint32_t)(ep*0x28))) = ((*((__IO uint32_t *)((uint32_t)&USBD->EPARSPCTL+(uint32_t)(ep*0x28))) & 0x10) | 0x40)
#define USBD_SET_EP_BUF_FLUSH(ep)         *((__IO uint32_t *) ((uint32_t)&USBD->EPARSPCTL + (uint32_t)(ep*0x28))) = USBD_EPRSPCTL_FLUSH_Msk
#define USBD_GET_EP_INT_EN(ep)            *((__IO uint32_t *) ((uint32_t)&USBD->EPAINTEN  + (uint32_t)(ep*0x28)))
#define USBD_GET_EP_INT(ep)               *((__IO uint32_t *) ((uint32_t)&USBD->EPAINTSTS + (uint32_t)(ep*0x28)))
#define USBD_CLR_EP_INT(ep, intr)        (*((__IO uint32_t *) ((uint32_t)&USBD->EPAINTSTS + (uint32_t)(ep*0x28))) = (intr)) /*!<Enable EPx Interrupt  \hideinitializer */
#endif

static volatile uint32_t s_ep_compl = 0;
static volatile uint32_t s_ep_buf_ind = 0;
static volatile uint32_t s_ep0_max_packet_size = 64;
static volatile uint8_t s_usb_addr = 0;

#if defined (TARGET_M480)
static volatile S_HSUSBD_CMD_T s_setup;
#elif defined (TARGET_NUC472)
static volatile S_USBD_CMD_T s_setup;
#endif

static volatile uint16_t s_ctrlin_packetsize;
static volatile uint8_t *g_usbd_CtrlInPointer = 0;
static uint32_t g_usbd_CtrlMaxPktSize = 64;
static volatile uint32_t g_usb_CtrlInSize = 0;
static uint32_t g_usb_ShortPacket = 0;
static uint32_t gEpReadCnt = 0;
static uint8_t set_addr = 0;

#if defined (TARGET_NUC472)
void USBD_CtrlInput(void)
{
    unsigned volatile i;
    uint32_t volatile count;

    if(g_usb_CtrlInSize >= g_usbd_CtrlMaxPktSize)
    {  
        for (i=0; i<g_usbd_CtrlMaxPktSize; i++)
            USBD->CEPDAT_BYTE = *(uint8_t *)(g_usbd_CtrlInPointer++);
        USBD->CEPTXCNT = g_usbd_CtrlMaxPktSize;
        g_usb_CtrlInSize -= g_usbd_CtrlMaxPktSize;
    }
    else
    {
        for (i=0; i<g_usb_CtrlInSize; i++)
            USBD->CEPDAT_BYTE = *(uint8_t *)(g_usbd_CtrlInPointer++);
        USBD->CEPTXCNT = g_usb_CtrlInSize;
        g_usbd_CtrlInPointer = 0;
        g_usb_CtrlInSize = 0;
    }
}
#elif defined (TARGET_M480)
void HSUSBD_CtrlInput(void)
{
    unsigned volatile i;
    uint32_t volatile count;

    /*************************** Enable Buffer ***************************/
    HSUSBD_SetEpBufAddr(CEP, 0, s_ep0_max_packet_size);
    if(g_usb_CtrlInSize >= g_usbd_CtrlMaxPktSize)
    {
        for (i=0; i<g_usbd_CtrlMaxPktSize; i++)
            HSUSBD->CEPDAT_BYTE = *(uint8_t *)(g_usbd_CtrlInPointer++);
        HSUSBD->CEPTXCNT = g_usbd_CtrlMaxPktSize;
        g_usb_CtrlInSize -= g_usbd_CtrlMaxPktSize;
    }
    else 
    {
        for (i=0; i<g_usb_CtrlInSize; i++)
            HSUSBD->CEPDAT_BYTE = *(uint8_t *)(g_usbd_CtrlInPointer++);
        HSUSBD->CEPTXCNT = g_usb_CtrlInSize;
        g_usbd_CtrlInPointer = 0;
        g_usb_CtrlInSize = 0;
    }
}
#endif

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

/* 
  Initialize this USBPhy instance.
  This function must be called before calling any other functions of this class, unless specifically noted. 
*/
void USBPhyHw::init(USBPhyEvents *events)
{
    if (this->events == NULL)
        sleep_manager_lock_deep_sleep();

    this->events = events;

    SYS_UnlockReg();

    s_ep_buf_ind = 0;

    chip_config();

#if defined (TARGET_NUC472)
    
    USBD_ENABLE_PHY();
    
    while (1)
    {
        USBD->EPAMPS = 0x20;
        if (USBD->EPAMPS == 0x20)
            break;
    }

    /* Set SE0 (disconnect) */
    USBD_SET_SE0();

    NVIC_SetVector(USBD_IRQn, (uint32_t) &USBD_IRQHandler);
    NVIC_EnableIRQ(USBD_IRQn);
    
#elif defined (TARGET_M480)
    HSUSBD_ENABLE_PHY();

    while (1)
    {
        HSUSBD->EP[0].EPMPS = 0x20;
        if (HSUSBD->EP[0].EPMPS == 0x20)
            break;
    }

    /* Set SE0 (disconnect) */
    HSUSBD_SET_SE0();

    NVIC_SetVector(USBD20_IRQn, (uint32_t) &USBD_IRQHandler);
    NVIC_EnableIRQ(USBD20_IRQn);
#endif

    instance = this;
}

void USBPhyHw::deinit()
{
    disconnect();

#if defined (TARGET_NUC472)

    NVIC_DisableIRQ(USBD_IRQn);
    
    USBD_SET_SE0();
    USBD_DISABLE_PHY();
    
#elif defined (TARGET_M480)

    NVIC_DisableIRQ(USBD20_IRQn);

    HSUSBD_SET_SE0();
    HSUSBD_DISABLE_PHY();
    
#endif

    if (events != NULL)
    {
        sleep_manager_unlock_deep_sleep();
    }
    events = NULL;
}

/* 
  Check if USB power is present.
  Devices which don't support checking the USB power state must always return true. 
*/
bool USBPhyHw::powered()
{
    return true;
}

void USBPhyHw::connect()
{
    memset(read_buffers, 0, sizeof(read_buffers));
    memset(read_sizes, 0, sizeof(read_sizes));

#if defined (TARGET_NUC472)
    USBD_ResetDMA();
    USBD_SET_ADDR(0);

    /**
      * Control Transfer Packet Size Constraints
      * low-speed: 8
      * full-speed: 8, 16, 32, 64
      * high-speed: 64
      */
    /* Control endpoint */
    USBD_SetEpBufAddr(CEP, 0, s_ep0_max_packet_size);
    s_ep_buf_ind = s_ep0_max_packet_size;

    /* Enable USB/CEP interrupt */
    USBD_ENABLE_USB_INT(USBD_GINTEN_USBIE_Msk | USBD_GINTEN_CEPIE_Msk);
    USBD_ENABLE_CEP_INT(USBD_CEPINTEN_SETUPPKIEN_Msk| USBD_CEPINTEN_STSDONEIEN_Msk);

    /* Enable BUS interrupt */
    USBD_ENABLE_BUS_INT(
        USBD_BUSINTEN_RESUMEIEN_Msk |
        USBD_BUSINTEN_RSTIEN_Msk |
        USBD_BUSINTEN_VBUSDETIEN_Msk
    );

    /* Clear SE0 (connect) */
    USBD_CLR_SE0();

#elif defined (TARGET_M480)
    HSUSBD_ResetDMA();
    HSUSBD_SET_ADDR(0);

    /**
      * Control Transfer Packet Size Constraints
      * low-speed: 8
      * full-speed: 8, 16, 32, 64
      * high-speed: 64
      */
    /* Control endpoint */
    /*************************** Disable Buffer ***************************/
    HSUSBD_SetEpBufAddr(CEP, 0, 1);
    s_ep_buf_ind = s_ep0_max_packet_size;

    /* Enable USB/CEP interrupt */
    HSUSBD_ENABLE_USB_INT(HSUSBD_GINTEN_USBIEN_Msk | HSUSBD_GINTEN_CEPIEN_Msk);
    HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPTKIEN_Msk|HSUSBD_CEPINTEN_SETUPPKIEN_Msk|HSUSBD_CEPINTEN_STSDONEIEN_Msk);

    /* Enable BUS interrupt */
    HSUSBD_ENABLE_BUS_INT(
        HSUSBD_BUSINTEN_RESUMEIEN_Msk |
        HSUSBD_BUSINTEN_RSTIEN_Msk |
        HSUSBD_BUSINTEN_VBUSDETIEN_Msk
    );

    /* Clear SE0 (connect) */
    HSUSBD_CLR_SE0();
#endif
}

/* Make the USB phy visible to the USB host.
   Enable either the D+ or D- pullup so the host can detect the presence of this device. */
void USBPhyHw::disconnect()
{
    /* Set SE0 (disconnect) */
#if defined (TARGET_NUC472)
    USBD_SET_SE0();
#elif defined (TARGET_M480)
    HSUSBD_SET_SE0();
#endif
}

/* Set this device to the configured state.
   Enable added endpoints if they are not enabled already. */
void USBPhyHw::configure()
{
    // not needed
}

/* 
   Leave the configured state.
   This is a notification to the USBPhy indicating that the device is leaving the configured state. The USBPhy can disable all endpoints other than endpoint 0. 
*/
void USBPhyHw::unconfigure()
{
    s_ep_buf_ind = s_ep0_max_packet_size;
}

/* Enable the start of frame interrupt. */
void USBPhyHw::sof_enable()
{
#if defined (TARGET_NUC472)
    USBD->BUSINTEN |= USBD_BUSINTEN_SOFIEN_Msk;
#elif defined (TARGET_M480)
    HSUSBD->BUSINTEN |= HSUSBD_BUSINTEN_SOFIEN_Msk;
#endif
}

/* Disable the start of frame interrupt. */
void USBPhyHw::sof_disable()
{
#if defined (TARGET_NUC472)
    USBD->BUSINTEN &= ~USBD_BUSINTEN_SOFIEN_Msk;
#elif defined (TARGET_M480)
    HSUSBD->BUSINTEN &= ~HSUSBD_BUSINTEN_SOFIEN_Msk;
#endif
}

/* Set the USBPhy's address. */
void USBPhyHw::set_address(uint8_t address)
{
    s_usb_addr = address;
}

/* Wake upstream devices */
void USBPhyHw::remote_wakeup()
{
#if defined (TARGET_NUC472)
    USBD->OPER |= USBD_OPER_RESUMEEN_Msk;
#elif defined (TARGET_M480)
    HSUSBD->OPER |= HSUSBD_OPER_RESUMEEN_Msk;
#endif
}

/*
  Get the endpoint table.
  This function returns a table which describes the endpoints can be used, the functionality of those endpoints and the resource cost.
*/
const usb_ep_table_t *USBPhyHw::endpoint_table()
{
    static const usb_ep_table_t endpoint_table = {
        1, // No cost per endpoint - everything allocated up front
        {
            {USB_EP_ATTR_ALLOW_CTRL | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_IN ,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_OUT,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_IN ,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_OUT,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_IN ,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_OUT,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_IN ,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_OUT,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_IN ,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_OUT,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_IN ,         0, 0},
            {ALLOW_ALL_EXCEPT_CRTL | USB_EP_ATTR_DIR_OUT,         0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN,             0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_OUT,            0, 0},
            {ALLOW_NO_ENDPOINTS | USB_EP_ATTR_DIR_IN,             0, 0}
        }
    };
    return &endpoint_table;
}
/*
  Set wMaxPacketSize of endpoint 0.
 */
uint32_t USBPhyHw::ep0_set_max_packet(uint32_t max_packet)
{
    s_ep0_max_packet_size = max_packet;
    return s_ep0_max_packet_size;
}

/* Read the contents of the SETUP packet. */
void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size)
{
#if defined (TARGET_NUC472)
    *((uint16_t *) (buffer + 0)) = (uint16_t) USBD->SETUP1_0;
    *((uint16_t *) (buffer + 2)) = (uint16_t) USBD->SETUP3_2;
    *((uint16_t *) (buffer + 4)) = (uint16_t) USBD->SETUP5_4;
    *((uint16_t *) (buffer + 6)) = (uint16_t) USBD->SETUP7_6;

    s_setup.bmRequestType = (uint8_t) (USBD->SETUP1_0 & 0xff);
    s_setup.bRequest = (int8_t) (USBD->SETUP1_0 >> 8) & 0xff;
    s_setup.wValue = (uint16_t) USBD->SETUP3_2;
    s_setup.wIndex = (uint16_t) USBD->SETUP5_4;
    s_setup.wLength = (uint16_t) USBD->SETUP7_6;
#elif defined (TARGET_M480)
    *((uint16_t *) (buffer + 0)) = (uint16_t) HSUSBD->SETUP1_0;
    *((uint16_t *) (buffer + 2)) = (uint16_t) HSUSBD->SETUP3_2;
    *((uint16_t *) (buffer + 4)) = (uint16_t) HSUSBD->SETUP5_4;
    *((uint16_t *) (buffer + 6)) = (uint16_t) HSUSBD->SETUP7_6;

    s_setup.bmRequestType = (uint8_t) (HSUSBD->SETUP1_0 & 0xff);
    s_setup.bRequest = (int8_t) (HSUSBD->SETUP1_0 >> 8) & 0xff;
    s_setup.wValue = (uint16_t) HSUSBD->SETUP3_2;
    s_setup.wIndex = (uint16_t) HSUSBD->SETUP5_4;
    s_setup.wLength = (uint16_t) HSUSBD->SETUP7_6;
#endif
}

/*
  Start receiving a packet of up to wMaxPacketSize on endpoint 0.
*/
void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
#if defined (TARGET_NUC472)
    if (s_setup.wLength && ! (s_setup.bmRequestType & 0x80))
    {
        /* Control OUT */
        read_buffers[0] = data;
        read_sizes[0] = size;
        USBD_ENABLE_CEP_INT(USBD_CEPINTEN_SETUPPKIEN_Msk | USBD_CEPINTEN_RXPKIEN_Msk);
    }
    else
    { 
        /* Status stage */
        USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_STSDONEIF_Msk);
        USBD_SET_CEP_STATE(USB_CEPCTL_NAKCLR);
        USBD_ENABLE_CEP_INT(USBD_CEPINTEN_STSDONEIEN_Msk|USBD_CEPINTEN_INTKIEN_Msk);
    }
#elif defined (TARGET_M480)
    if (s_setup.wLength && ! (s_setup.bmRequestType & 0x80))
    {
        /* Control OUT */
        read_buffers[0] = data;
        read_sizes[0] = size;
        HSUSBD->CEPINTSTS = HSUSBD_CEPINTSTS_NAKIF_Msk;
        HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPTKIEN_Msk|HSUSBD_CEPINTEN_SETUPPKIEN_Msk | HSUSBD_CEPINTEN_RXPKIEN_Msk | HSUSBD_CEPINTEN_NAKIEN_Msk);
    }
    else
    {
        /* Status stage */
        HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
        HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
        HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk|HSUSBD_CEPINTEN_INTKIEN_Msk);
        /*************************** Disable Buffer ***************************/
        HSUSBD_SetEpBufAddr(CEP, 0, 1);
    }
#endif
}
/*
  Read the contents of a received packet.
*/
uint32_t USBPhyHw::ep0_read_result()
{
    uint32_t i;
    uint8_t *ep0_data = read_buffers[0];
#if defined (TARGET_NUC472)
    uint32_t ceprxcnt = USBD->CEPRXCNT;
    
    for (i = 0; i < ceprxcnt; i ++)
        ep0_data[i] = USBD->CEPDAT_BYTE;
#elif defined (TARGET_M480)
    uint32_t ceprxcnt = HSUSBD->CEPRXCNT;
    
    for (i = 0; i < ceprxcnt; i ++)
        ep0_data[i] = HSUSBD->CEPDAT_BYTE;
#endif
    return ceprxcnt;
}

/* Write a packet on endpoint 0. */
void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
#if defined (TARGET_NUC472)
    if (buffer && size)
    {
        g_usbd_CtrlInPointer = buffer;
        g_usb_CtrlInSize = size;
        USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_INTKIF_Msk);
        USBD_ENABLE_CEP_INT(USBD_CEPINTEN_INTKIEN_Msk);
    }
    else
    {
        /* Status stage */
        USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_STSDONEIF_Msk);
        USBD_SET_CEP_STATE(USB_CEPCTL_NAKCLR);
        USBD_ENABLE_CEP_INT(USBD_CEPINTEN_STSDONEIEN_Msk);
    }
#elif defined (TARGET_M480)
    if (buffer && size)
    {
        g_usbd_CtrlInPointer = buffer;
        g_usb_CtrlInSize = size;
        HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
        HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_INTKIEN_Msk);
    }
    else
    {
        /* Status stage */
        HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
        HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
        HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
    }
#endif
}

void stallEndpoint(uint8_t endpoint)
{
    uint32_t ep_hw_index = NU_EPL2EPH(endpoint);
    if (ep_hw_index >= NUMBER_OF_PHYSICAL_ENDPOINTS)
        return;
#if defined (TARGET_NUC472)
    USBD_SetStall(ep_hw_index);
#elif defined (TARGET_M480)
    HSUSBD_SetStall(ep_hw_index);
#endif
}

/* Protocol stall on endpoint 0.
   Stall all IN and OUT packets on endpoint 0 until a setup packet is received.
   Note The stall is cleared automatically when a setup packet is received 
*/
void USBPhyHw::ep0_stall()
{
#if defined (TARGET_NUC472)
    USBD_SetStall(0);
#elif defined (TARGET_M480)
    HSUSBD_SetStall(0);
#endif
}

/* Configure and enable an endpoint. */
/*
   endpoint	Endpoint to configure and enable
   max_packet	The maximum packet size that can be sent or received
   type	The type of endpoint this should be configured as - USB_EP_TYPE_BULK, USB_EP_TYPE_INT or USB_EP_TYPE_ISO
   This function cannot be used to configure endpoint 0. That must be done with ep0_set_max_packet
*/
bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type)
{
    uint32_t ep_type;
    uint32_t ep_hw_index = NU_EPL2EPH(DESC_TO_LOG(endpoint));

#if defined (TARGET_NUC472)
    USBD_SetEpBufAddr(ep_hw_index, s_ep_buf_ind, max_packet);
    s_ep_buf_ind += max_packet;
    USBD_SET_MAX_PAYLOAD(ep_hw_index, max_packet);
    switch (type) 
    {
        case USB_EP_TYPE_INT:
            ep_type = USB_EP_CFG_TYPE_INT;
            break;

        case USB_EP_TYPE_ISO:
            ep_type = USB_EP_CFG_TYPE_ISO;
            break;

        default:
            ep_type = USB_EP_CFG_TYPE_BULK;
    }

    uint32_t ep_dir = ((endpoint & EP_DIR_Msk) == EP_DIR_IN) ? USB_EP_CFG_DIR_IN : USB_EP_CFG_DIR_OUT;
    USBD_ConfigEp(ep_hw_index, DESC_TO_LOG(endpoint), ep_type, ep_dir);

    /* Enable USB/EPX interrupt */
    // NOTE: Require USBD_GINTEN_EPAIE_Pos, USBD_GINTEN_EPBIE_Pos, ... USBD_GINTEN_EPLIE_Pos to be consecutive.
    USBD_ENABLE_USB_INT(USBD->GINTEN | USBD_GINTEN_USBIE_Msk |
                          USBD_GINTEN_CEPIE_Msk |
                          1 << (ep_hw_index  + USBD_GINTEN_EPAIE_Pos));

    if (ep_dir != 0)
        USBD_ENABLE_EP_INT(ep_hw_index, USBD_EPINTEN_TXPKIEN_Msk);
#elif defined (TARGET_M480)
    HSUSBD_SetEpBufAddr(ep_hw_index, s_ep_buf_ind, max_packet);
    s_ep_buf_ind += max_packet;
    HSUSBD_SET_MAX_PAYLOAD(ep_hw_index, max_packet);
    switch (type)
    {
        case USB_EP_TYPE_INT:
            ep_type = HSUSBD_EP_CFG_TYPE_INT;
            break;

        case USB_EP_TYPE_ISO:
            ep_type = HSUSBD_EP_CFG_TYPE_ISO;
            break;

        default:
            ep_type = HSUSBD_EP_CFG_TYPE_BULK;
    }

    uint32_t ep_dir = ((endpoint & EP_DIR_Msk) == EP_DIR_IN) ? HSUSBD_EP_CFG_DIR_IN : HSUSBD_EP_CFG_DIR_OUT;
    HSUSBD_ConfigEp(ep_hw_index, DESC_TO_LOG(endpoint), ep_type, ep_dir);

    /* Enable USB/EPX interrupt */
    // NOTE: Require USBD_GINTEN_EPAIE_Pos, USBD_GINTEN_EPBIE_Pos, ... USBD_GINTEN_EPLIE_Pos to be consecutive.
    HSUSBD_ENABLE_USB_INT(HSUSBD->GINTEN | HSUSBD_GINTEN_USBIEN_Msk |
                          HSUSBD_GINTEN_CEPIEN_Msk |
                          1 << (ep_hw_index  + HSUSBD_GINTEN_EPAIEN_Pos));
#endif
     return true;
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    uint32_t ep_hw_index = NU_EPL2EPH(DESC_TO_LOG(endpoint));
#if defined (TARGET_NUC472)
    *((__IO uint32_t *) ((uint32_t)&USBD->EPACFG + (uint32_t)(ep_hw_index*0x28))) = (*((__IO uint32_t *)((uint32_t)&USBD->EPACFG+(uint32_t)(ep_hw_index*0x28))) & ~USB_EP_CFG_VALID);
#elif defined (TARGET_M480)
    HSUSBD->EP[ep_hw_index].EPCFG = HSUSBD->EP[ep_hw_index].EPCFG  & ~HSUSBD_EP_CFG_VALID;
#endif
}

/*
  Perform a functional stall on the given endpoint.
  Set the HALT feature for this endpoint so that all further communication is aborted. 
*/
void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    uint8_t ep_logic_index = DESC_TO_LOG(endpoint);
    uint32_t ep_hw_index = NU_EPL2EPH(ep_logic_index);
#if defined (TARGET_NUC472)
    if(ep_logic_index == 0)
        USBD_SetStall(0);
    else
        USBD_SetStall(ep_logic_index);
#elif defined (TARGET_M480)
    if(ep_logic_index == 0)
        HSUSBD_SetEpStall(CEP);
    else
        HSUSBD_SetEpStall(ep_hw_index);
#endif
}

/*
   Unstall the endpoint.
   Clear the HALT feature on this endpoint so communication can resume. 
 */
void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    uint8_t ep_logic_index = DESC_TO_LOG(endpoint);
    uint32_t ep_hw_index = NU_EPL2EPH(ep_logic_index);
#if defined (TARGET_NUC472)
    if(ep_logic_index != 0)
        USBD_ClearStall(ep_logic_index);
#elif defined (TARGET_M480)
    if(ep_logic_index != 0)
        HSUSBD_ClearEpStall(ep_hw_index);
#endif
}

/* Start a read on the given endpoint. */
bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
/* Store the buffer address & length */
    uint8_t ep_logic_index = DESC_TO_LOG(endpoint);
    uint32_t ep_hw_index = NU_EPL2EPH(ep_logic_index);
    read_buffers[ep_logic_index] = data;
    read_sizes[ep_logic_index] = size;
#if defined (TARGET_NUC472)    
    USBD_ENABLE_EP_INT(ep_hw_index, USBD_GET_EP_INT_EN(ep_hw_index) | USBD_EPINTEN_RXPKIEN_Msk);
#elif defined (TARGET_M480)
    HSUSBD->EP[ep_hw_index].EPINTEN |= HSUSBD_EPINTEN_RXPKIEN_Msk;
#endif
    return true;
}

/* 
  Finish a read on the given endpoint 
  Returns The number of bytes received
*/
uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    uint32_t ep_logic_index = DESC_TO_LOG(endpoint);
    uint32_t bytes_read = 0;
    endpoint_read_result_core(endpoint, read_buffers[ep_logic_index], read_sizes[ep_logic_index], &bytes_read);
    read_buffers[ep_logic_index] = 0;
    read_sizes[ep_logic_index] = 0;
    return bytes_read;
}

bool USBPhyHw::endpoint_read_result_core(usb_ep_t endpoint, uint8_t *data, uint32_t size, uint32_t *bytes_read)
{
    uint32_t ep_logic_index = DESC_TO_LOG(endpoint);
    uint32_t ep_hw_index = NU_EPL2EPH(ep_logic_index);
    uint8_t __attribute__((aligned(4))) tmp_buffer[4];
    uint32_t volatile buffer, len, i;
#if defined (TARGET_NUC472)
    uint32_t mps = USBD_GET_EP_MAX_PAYLOAD(ep_hw_index);

    gEpReadCnt = USBD_GET_EP_DATA_COUNT(ep_hw_index);
#elif defined (TARGET_M480)
    uint32_t mps = HSUSBD_GET_EP_MAX_PAYLOAD(ep_hw_index);
    
    gEpReadCnt = HSUSBD_GET_EP_DATA_COUNT(ep_hw_index);
#endif

    if(gEpReadCnt == 0)
    {
        *bytes_read = 0;
        return true;
    }

    buffer = (uint32_t)data;

    if(buffer % 4)
    {
        len = 4 - (buffer % 4);
        if(gEpReadCnt <= len)
            len = gEpReadCnt;

        while(1)
        {
#if defined (TARGET_NUC472)
            if (!(USBD->DMACTL & USBD_DMACTL_DMAEN_Msk))
                break;
            else if (!USBD_IS_ATTACHED())
                break;
#elif defined (TARGET_M480)
            if (!(HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk))
                break;
            else if (!HSUSBD_IS_ATTACHED())
                break;
#endif
        }
#if defined (TARGET_NUC472)
        USBD_SET_DMA_LEN(len);
        USBD_SET_DMA_ADDR((uint32_t)tmp_buffer);
        USBD_SET_DMA_WRITE(ep_logic_index);
        USBD_ENABLE_DMA();
#elif defined (TARGET_M480)
        HSUSBD_SET_DMA_LEN(len);
        HSUSBD_SET_DMA_ADDR((uint32_t)tmp_buffer);
        HSUSBD_SET_DMA_WRITE(ep_logic_index);
        HSUSBD_ENABLE_DMA();
#endif

        while(1)
        {
#if defined (TARGET_NUC472)
            if (!(USBD->DMACTL & USBD_DMACTL_DMAEN_Msk))
                break;
            else if (!USBD_IS_ATTACHED())
                break;
#elif defined (TARGET_M480)
            if (!(HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk))
                break;
            else if (!HSUSBD_IS_ATTACHED())
                break;
#endif
        }
        for(i=0;i<len;i++)
            data[i] = tmp_buffer[i];

        buffer = buffer + len;
        len = gEpReadCnt - len;
    }
    else
        len = gEpReadCnt;

    if(len)
    {
#if defined (TARGET_NUC472)
        USBD_SET_DMA_LEN(len);
        USBD_SET_DMA_ADDR(buffer);
        USBD_SET_DMA_WRITE(ep_logic_index);
        USBD_ENABLE_DMA();
        while(1)
        {
            if (!(USBD->DMACTL & USBD_DMACTL_DMAEN_Msk))
                break;
            else if (!USBD_IS_ATTACHED())
                break;
        }
#elif defined (TARGET_M480)
        HSUSBD_SET_DMA_LEN(len);
        HSUSBD_SET_DMA_ADDR(buffer);
        HSUSBD_SET_DMA_WRITE(ep_logic_index);
        HSUSBD_ENABLE_DMA();
        while(1)
        {
            if (!(HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk))
                break;
            else if (!HSUSBD_IS_ATTACHED())
                break;
        }
#endif

    }
    *bytes_read = gEpReadCnt;
    return true;
}

/*
  Start a write on the given endpoint.
  true if the data was prepared for transmit, false otherwise
*/
bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
   uint32_t ep_logic_index = DESC_TO_LOG(endpoint);
   uint32_t ep_hw_index = NU_EPL2EPH(ep_logic_index);
   uint8_t __attribute__((aligned(4))) tmp_buffer[4];

    if(ep_logic_index == 0)
        return false;
    else
    {
#if defined (TARGET_NUC472)
        uint32_t mps = USBD_GET_EP_MAX_PAYLOAD(ep_hw_index);
#elif defined (TARGET_M480)
        uint32_t mps = HSUSBD_GET_EP_MAX_PAYLOAD(ep_hw_index);
#endif

        uint32_t buffer, len, i;
        if (size > mps)
            return false;

#if defined (TARGET_NUC472)
        if(USBD_GET_EP_DATA_COUNT(ep_hw_index) & 0xFFFF)
        {
            USBD_SET_EP_SHORT_PACKET(ep_hw_index);
            return false;
        }
#elif defined (TARGET_M480)
        if(HSUSBD->EP[ep_hw_index].EPDATCNT & 0xFFFF)
        {
            HSUSBD_SET_EP_SHORT_PACKET(ep_hw_index);
            return false;
        }
#endif

        if(size < mps)
            g_usb_ShortPacket = 1;

        while(1)
        {
#if defined (TARGET_NUC472)
            if (!(USBD->DMACTL & USBD_DMACTL_DMAEN_Msk))
                break;
            else if (!USBD_IS_ATTACHED())
                break;
#elif defined (TARGET_M480)
            if (!(HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk))
                break;
            else if (!HSUSBD_IS_ATTACHED())
                break;
#endif
        }
        buffer = (uint32_t)data;
        if(buffer % 4)
        {
            len = 4 - (buffer % 4);
            if(size <= len)
                len = size;

            for(i=0;i<len;i++)
                tmp_buffer[i] = data[i];

#if defined (TARGET_NUC472)
            USBD_SET_DMA_LEN(len);
            USBD_SET_DMA_ADDR((uint32_t)tmp_buffer);
            USBD_SET_DMA_READ(ep_logic_index);
            USBD_ENABLE_DMA();
            while(1)
            {
                if (!(USBD->DMACTL & USBD_DMACTL_DMAEN_Msk))
                    break;
                else if (!USBD_IS_ATTACHED())
                    break;
            }
#elif defined (TARGET_M480)
            HSUSBD_SET_DMA_LEN(len);
            HSUSBD_SET_DMA_ADDR((uint32_t)tmp_buffer);
            HSUSBD_SET_DMA_READ(ep_logic_index);
            HSUSBD_ENABLE_DMA();
            while(1)
            {
                if (!(HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk))
                    break;
                else if (!HSUSBD_IS_ATTACHED())
                    break;
            }
#endif
            buffer = buffer + len;
            len = size - len;
        }
        else
            len = size;

        if(len)
        {
#if defined (TARGET_NUC472)
            USBD_SET_DMA_LEN(len);
            USBD_SET_DMA_ADDR((uint32_t)buffer);
            USBD_SET_DMA_READ(ep_logic_index);
            USBD_ENABLE_DMA();
            while(1)
            {
                if (!(USBD->DMACTL & USBD_DMACTL_DMAEN_Msk))
                    break;
                else if (!USBD_IS_ATTACHED())
                    break;
            }
#elif defined (TARGET_M480)
            HSUSBD_SET_DMA_LEN(len);
            HSUSBD_SET_DMA_ADDR((uint32_t)buffer);
            HSUSBD_SET_DMA_READ(ep_logic_index);
            HSUSBD_ENABLE_DMA();
            while(1)
            {
                if (!(HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk))
                    break;
                else if (!HSUSBD_IS_ATTACHED())
                    break;
            }
#endif
        }

#if defined (TARGET_NUC472)
        if(g_usb_ShortPacket)
            USBD_SET_EP_SHORT_PACKET(ep_hw_index);
    USBD_ENABLE_EP_INT(ep_hw_index, USBD_GET_EP_INT_EN(ep_hw_index) | USBD_EPINTEN_TXPKIEN_Msk);
#elif defined (TARGET_M480)
        if(g_usb_ShortPacket)
            HSUSBD_SET_EP_SHORT_PACKET(ep_hw_index);
    HSUSBD->EP[ep_hw_index].EPINTEN |= HSUSBD_EPINTEN_TXPKIEN_Msk;
#endif
    }
    return true;
}

/* Abort the current transfer if it has not yet been sent. */
void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
   uint32_t ep_logic_index = DESC_TO_LOG(endpoint);
   uint32_t ep_hw_index = NU_EPL2EPH(ep_logic_index);
#if defined (TARGET_NUC472)
   USBD_CLR_EP_INT(ep_hw_index, USBD_GET_EP_INT(ep_hw_index));
   USBD_ENABLE_EP_INT(ep_hw_index, 0);
   USBD_SET_EP_BUF_FLUSH(ep_hw_index);
#elif defined (TARGET_M480)
   HSUSBD->EP[ep_hw_index].EPINTEN = 0;
   HSUSBD->EP[ep_hw_index].EPINTSTS = HSUSBD->EP[ep_hw_index].EPINTSTS;
   HSUSBD->EP[ep_hw_index].EPRSPCTL = HSUSBD_EPRSPCTL_FLUSH_Msk;
#endif
}

/* 
  Callback used for performing USB processing.
  USBPhy processing should be triggered by calling USBPhyEvents::start_process and done inside process. All USBPhyEvents callbacks aside from USBPhyEvents::start_process must be called in the context of process
*/
void USBPhyHw::process()
{
#if defined (TARGET_NUC472)
    uint32_t gintsts = USBD->GINTSTS & USBD->GINTEN;
    uint32_t gintsts_epx = gintsts >> 2;     /* EPA, EPB, EPC, ... EPL interrupts */
    uint32_t ep_hw_index = 0;
    uint32_t ep_logic_index = 0,i;

    if (! gintsts)
        return;

    if (gintsts & USBD_GINTSTS_USBIF_Msk)
    {
        uint32_t busintsts = USBD->BUSINTSTS & USBD->BUSINTEN;

        /* SOF */
        if (busintsts & USBD_BUSINTSTS_SOFIF_Msk)
        {
            USBD_CLR_BUS_INT_FLAG(USBD_BUSINTSTS_SOFIF_Msk);

            events->sof(USBD->FRAMECNT >> 3);
        }

        /* Reset */
        if (busintsts & USBD_BUSINTSTS_RSTIF_Msk)
        {
            connect();
            events->reset();
            USBD_CLR_BUS_INT_FLAG(USBD_BUSINTSTS_RSTIF_Msk);
            USBD_CLR_CEP_INT_FLAG(0x1ffc);
        }

        /* Resume */
        if (busintsts & USBD_BUSINTSTS_RESUMEIF_Msk)
        {
            USBD_ENABLE_BUS_INT(USBD_BUSINTEN_RSTIEN_Msk | USBD_BUSINTEN_SUSPENDIEN_Msk);
            USBD_CLR_BUS_INT_FLAG(USBD_BUSINTSTS_RESUMEIF_Msk);
//            events->suspend(false);
        }

        /* Suspend */
        if (busintsts & USBD_BUSINTSTS_SUSPENDIF_Msk)
        {
            USBD_ENABLE_BUS_INT(USBD_BUSINTEN_RSTIEN_Msk | USBD_BUSINTEN_RESUMEIEN_Msk);
            USBD_CLR_BUS_INT_FLAG(USBD_BUSINTSTS_SUSPENDIF_Msk);
        }

        /* High-speed */
        if (busintsts & USBD_BUSINTSTS_HISPDIF_Msk)
        {
            USBD_ENABLE_CEP_INT(USBD_CEPINTEN_SETUPPKIEN_Msk);
            USBD_CLR_BUS_INT_FLAG(USBD_BUSINTSTS_HISPDIF_Msk);
        }

        /* DMA */
        if (busintsts & USBD_BUSINTSTS_DMADONEIF_Msk)
        {
            USBD_CLR_BUS_INT_FLAG(USBD_BUSINTSTS_DMADONEIF_Msk);
        }

        /* PHY clock available */
        if (busintsts & USBD_BUSINTSTS_PHYCLKVLDIF_Msk)
        {
            USBD_CLR_BUS_INT_FLAG(USBD_BUSINTSTS_PHYCLKVLDIF_Msk);
        }

        /* VBUS plug-in */
        if (busintsts & USBD_BUSINTSTS_VBUSDETIF_Msk)
        {
            if (USBD_IS_ATTACHED())
            {
                /* USB plug-in */
                USBD_ENABLE_USB();
            }
            else
            {
                /* USB unplug-out */
                USBD_DISABLE_USB();
            }
            USBD_CLR_BUS_INT_FLAG(USBD_BUSINTSTS_VBUSDETIF_Msk);
        }
    }

    /* CEP interrupts */
    if (gintsts & USBD_GINTSTS_CEPIF_Msk)
    {
        uint32_t cepintsts =  USBD->CEPINTSTS & USBD->CEPINTEN;

        /* SETUP token packet */
        if (cepintsts & USBD_CEPINTSTS_SETUPTKIF_Msk)
        {
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_SETUPTKIF_Msk);

            return;
        }

        /* SETUP transaction */
        if (cepintsts & USBD_CEPINTSTS_SETUPPKIF_Msk)
        {
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_SETUPPKIF_Msk);
            events->ep0_setup();

            return;
        }

        /* OUT token packet */
        if (cepintsts & USBD_CEPINTSTS_OUTTKIF_Msk)
        {
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_OUTTKIF_Msk);
            USBD_ENABLE_CEP_INT(USBD_CEPINTEN_STSDONEIEN_Msk);

            return;
        }

        /* IN token packet */
        if (cepintsts & USBD_CEPINTSTS_INTKIF_Msk)
        {
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_INTKIF_Msk);
            if (!(cepintsts & USBD_CEPINTSTS_STSDONEIF_Msk))
            {
                USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_TXPKIF_Msk);
                USBD_ENABLE_CEP_INT(USBD_CEPINTEN_TXPKIEN_Msk | USBD_CEPINTEN_SETUPPKIEN_Msk);
                USBD_CtrlInput();
            }
            else
            {
                USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_TXPKIF_Msk);
                USBD_ENABLE_CEP_INT(USBD_CEPINTEN_TXPKIEN_Msk| USBD_CEPINTEN_STSDONEIEN_Msk);
            }

            return;
        }

        /* PING packet */
        if (cepintsts & USBD_CEPINTSTS_PINGIF_Msk)
        {
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_PINGIF_Msk);

            return;
        }

        /* IN transaction */
        if (cepintsts & USBD_CEPINTSTS_TXPKIF_Msk)
        {
            events->ep0_in();
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_TXPKIF_Msk);

            return;
        }

        /* OUT transaction */
        if (cepintsts & USBD_CEPINTSTS_RXPKIF_Msk)
        {
            events->ep0_out();
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_RXPKIF_Msk);

            return;
        }

        /* NAK handshake packet */
        if (cepintsts & USBD_CEPINTSTS_NAKIF_Msk)
        {
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_NAKIF_Msk);

            return;
        }

        /* STALL handshake packet */
        if (cepintsts & USBD_CEPINTSTS_STALLIF_Msk)
        {
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_STALLIF_Msk);

            return;
        }

        /* ERR special packet */
        if (cepintsts & USBD_CEPINTSTS_ERRIF_Msk)
        {
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_ERRIF_Msk);

            return;
        }

        /* Status stage transaction */
        if (cepintsts & USBD_CEPINTSTS_STSDONEIF_Msk)
        {
            if (s_usb_addr)
            {
                USBD_SET_ADDR(s_usb_addr);
                s_usb_addr = 0;
            }
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_STSDONEIF_Msk);
            USBD_ENABLE_CEP_INT(USBD_CEPINTEN_SETUPPKIEN_Msk);

            return;
        }

        /* Buffer Full */
        if (cepintsts & USBD_CEPINTSTS_BUFFULLIF_Msk)
        {
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_BUFFULLIF_Msk);

            return;
        }

        /* Buffer Empty */
        if (cepintsts & USBD_CEPINTSTS_BUFEMPTYIF_Msk)
        {
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_BUFEMPTYIF_Msk);

            return;
        }
    }

    while (gintsts_epx)
    {
        if(gintsts_epx & 0x01)
        {
            uint32_t volatile epxintsts = USBD_GET_EP_INT_FLAG(ep_hw_index) & USBD_GET_EP_INT_EN(ep_hw_index);
             
            ep_logic_index = NU_EPH2EPL(ep_hw_index);

            USBD_CLR_EP_INT_FLAG(ep_hw_index, epxintsts);

            /* Buffer Full */
            if (epxintsts & USBD_EPINTSTS_BUFFULLIF_Msk)
            {
            }

            /* Buffer Empty */
            if (epxintsts & USBD_EPINTSTS_BUFEMPTYIF_Msk)
            {
            }

            /* Short Packet Transferred */
            if (epxintsts & USBD_EPINTSTS_SHORTTXIF_Msk)
            {
            }

            /* Data Packet Transmitted */
            if (epxintsts & USBD_EPINTSTS_TXPKIF_Msk)
            {
                USBD_ENABLE_EP_INT(ep_hw_index, USBD_GET_EP_INT_EN(ep_hw_index) & ~USBD_EPINTEN_TXPKIEN_Msk);
                events->in(NU_EPH2EPL(ep_hw_index) | EP_DIR_Msk);
            }

            /* Data Packet Received */
            if (epxintsts & USBD_EPINTSTS_RXPKIF_Msk)
            {
                USBD_ENABLE_EP_INT(ep_hw_index, USBD_GET_EP_INT_EN(ep_hw_index) & ~USBD_EPINTEN_RXPKIEN_Msk);
                events->out(ep_logic_index);
            }

            /* OUT token packet */
            if (epxintsts & USBD_EPINTSTS_OUTTKIF_Msk)
            {

            }

            /* IN token packet */
            if (epxintsts & USBD_EPINTSTS_INTKIF_Msk)
            {
            }

            /* PING packet */
            if (epxintsts & USBD_EPINTSTS_PINGIF_Msk)
            {
            }

            /* NAK handshake packet sent to Host */
            if (epxintsts & USBD_EPINTSTS_NAKIF_Msk)
            {

            }

            /* STALL handshake packet sent to Host */
            if (epxintsts & USBD_EPINTSTS_STALLIF_Msk)
            {

            }

            /* NYET handshake packet sent to Host */
            if (epxintsts & USBD_EPINTSTS_NYETIF_Msk)
            {

            }

            /* ERR packet sent to Host */
            if (epxintsts & USBD_EPINTSTS_ERRIF_Msk)
            {
            }

            /* Bulk Out Short Packet Received */
            if (epxintsts & USBD_EPINTSTS_SHORTRXIF_Msk)
            {
            }
        }
        gintsts_epx = gintsts_epx >> 1;
        ep_hw_index++;
    }
#elif defined (TARGET_M480)
    uint32_t gintsts = HSUSBD->GINTSTS & HSUSBD->GINTEN;
    uint32_t gintsts_epx = gintsts >> 2;     /* EPA, EPB, EPC, ... EPL interrupts */
    uint32_t ep_hw_index = 0;
    uint32_t ep_logic_index = 0,i;

    if (! gintsts)
        return;

    if (gintsts & HSUSBD_GINTSTS_USBIF_Msk)
    {
        uint32_t busintsts = HSUSBD->BUSINTSTS & HSUSBD->BUSINTEN;

        /* SOF */
        if (busintsts & HSUSBD_BUSINTSTS_SOFIF_Msk)
        {
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_SOFIF_Msk);

            events->sof(HSUSBD->FRAMECNT >> 3);
        }

        /* Reset */
        if (busintsts & HSUSBD_BUSINTSTS_RSTIF_Msk)
        {
            connect();
            events->reset();
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_RSTIF_Msk);
            HSUSBD_CLR_CEP_INT_FLAG(0x1ffc);
        }

        /* Resume */
        if (busintsts & HSUSBD_BUSINTSTS_RESUMEIF_Msk)
        {
            HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk | HSUSBD_BUSINTEN_SUSPENDIEN_Msk);
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_RESUMEIF_Msk);
//            events->suspend(false);
        }

        /* Suspend */
        if (busintsts & HSUSBD_BUSINTSTS_SUSPENDIF_Msk)
        {
            HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk | HSUSBD_BUSINTEN_RESUMEIEN_Msk);
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_SUSPENDIF_Msk);
        }

        /* High-speed */
        if (busintsts & HSUSBD_BUSINTSTS_HISPDIF_Msk)
        {
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPTKIEN_Msk|HSUSBD_CEPINTEN_SETUPPKIEN_Msk);
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_HISPDIF_Msk);
        }

        /* DMA */
        if (busintsts & HSUSBD_BUSINTSTS_DMADONEIF_Msk)
        {
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_DMADONEIF_Msk);
        }

        /* PHY clock available */
        if (busintsts & HSUSBD_BUSINTSTS_PHYCLKVLDIF_Msk)
        {
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_PHYCLKVLDIF_Msk);
        }

        /* VBUS plug-in */
        if (busintsts & HSUSBD_BUSINTSTS_VBUSDETIF_Msk)
        {
            if (HSUSBD_IS_ATTACHED())
            {
                /* USB plug-in */
                HSUSBD_ENABLE_USB();
            }
            else
            {
                /* USB unplug-out */
                HSUSBD_DISABLE_USB();
            }
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_VBUSDETIF_Msk);
        }
    }

    /* CEP interrupts */
    if (gintsts & HSUSBD_GINTSTS_CEPIF_Msk)
    {
        uint32_t cepintsts =  HSUSBD->CEPINTSTS & HSUSBD->CEPINTEN;

        /* SETUP token packet */
        if (cepintsts & HSUSBD_CEPINTSTS_SETUPTKIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_SETUPTKIF_Msk);
                 
            /*************************** Disable Buffer ***************************/
            HSUSBD_SetEpBufAddr(CEP, 0, 1);
            return;
        }

        /* SETUP transaction */
        if (cepintsts & HSUSBD_CEPINTSTS_SETUPPKIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_SETUPPKIF_Msk);
            events->ep0_setup();
            return;
        }

        /* OUT token packet */
        if (cepintsts & HSUSBD_CEPINTSTS_OUTTKIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_OUTTKIF_Msk);
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);

            return;
        }

        /* IN token packet */
        if (cepintsts & HSUSBD_CEPINTSTS_INTKIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
            if (!(cepintsts & HSUSBD_CEPINTSTS_STSDONEIF_Msk))
            {
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPTKIEN_Msk|HSUSBD_CEPINTEN_TXPKIEN_Msk | HSUSBD_CEPINTEN_SETUPPKIEN_Msk);
                HSUSBD_CtrlInput();
            }
            else
            {
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_TXPKIEN_Msk | HSUSBD_CEPINTEN_STSDONEIEN_Msk);
            
                /*************************** Disable Buffer ***************************/
                HSUSBD_SetEpBufAddr(CEP, 0, 1);
            }

            return;
        }

        /* PING packet */
        if (cepintsts & HSUSBD_CEPINTSTS_PINGIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_PINGIF_Msk);

            return;
        }

        /* IN transaction */
        if (cepintsts & HSUSBD_CEPINTSTS_TXPKIF_Msk)
        {
            events->ep0_in();
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
            return;
        }

        /* OUT transaction */
        if (cepintsts & HSUSBD_CEPINTSTS_RXPKIF_Msk)
        {
            events->ep0_out();
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_RXPKIF_Msk);
            return;
        }

        /* NAK handshake packet */
        if (cepintsts & HSUSBD_CEPINTSTS_NAKIF_Msk)
        {
            /*************************** Enable Buffer ***************************/
            HSUSBD_SetEpBufAddr(CEP, 0, s_ep0_max_packet_size);
            HSUSBD->CEPINTEN = HSUSBD->CEPINTEN & ~HSUSBD_CEPINTEN_NAKIEN_Msk;

            if(((uint8_t) (HSUSBD->SETUP1_0 & 0xff)) == 0x00 &&
               ((uint8_t) (HSUSBD->SETUP1_0 >> 8) & 0xff) == 0x07 &&
               ((uint16_t) HSUSBD->SETUP3_2) == 0x0100 &&
               ((uint16_t) HSUSBD->SETUP5_4) == 0x0000 &&
               ((uint16_t) HSUSBD->SETUP7_6) == 0x0012)
               HSUSBD_SetStall(0);

            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_NAKIF_Msk);
            return;
        }

        /* STALL handshake packet */
        if (cepintsts & HSUSBD_CEPINTSTS_STALLIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STALLIF_Msk);

            return;
        }

        /* ERR special packet */
        if (cepintsts & HSUSBD_CEPINTSTS_ERRIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_ERRIF_Msk);

            return;
        }

        /* Status stage transaction */
        if (cepintsts & HSUSBD_CEPINTSTS_STSDONEIF_Msk)
        {
            if (s_usb_addr)
            {
                HSUSBD_SET_ADDR(s_usb_addr);
                s_usb_addr = 0;
            }
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPTKIEN_Msk|HSUSBD_CEPINTEN_SETUPPKIEN_Msk);
            return;
        }

        /* Buffer Full */
        if (cepintsts & HSUSBD_CEPINTSTS_BUFFULLIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_BUFFULLIF_Msk);

            return;
        }

        /* Buffer Empty */
        if (cepintsts & HSUSBD_CEPINTSTS_BUFEMPTYIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_BUFEMPTYIF_Msk);

            return;
        }
    }

    while (gintsts_epx)
    {
        if(gintsts_epx & 0x01)
        {
            uint32_t volatile epxintsts = HSUSBD_GET_EP_INT_FLAG(ep_hw_index) & HSUSBD_GET_EP_INT_EN(ep_hw_index);
             
            ep_logic_index = NU_EPH2EPL(ep_hw_index);

            HSUSBD_CLR_EP_INT_FLAG(ep_hw_index, epxintsts);

            /* Buffer Full */
            if (epxintsts & HSUSBD_EPINTSTS_BUFFULLIF_Msk)
            {
            }

            /* Buffer Empty */
            if (epxintsts & HSUSBD_EPINTSTS_BUFEMPTYIF_Msk)
            {
            }

            /* Short Packet Transferred */
            if (epxintsts & HSUSBD_EPINTSTS_SHORTTXIF_Msk)
            {
            }

            /* Data Packet Transmitted */
            if (epxintsts & HSUSBD_EPINTSTS_TXPKIF_Msk)
            {
                HSUSBD->EP[ep_hw_index].EPINTEN &= ~HSUSBD_EPINTEN_TXPKIEN_Msk;
                events->in(NU_EPH2EPL(ep_hw_index) | EP_DIR_Msk);
            }

            /* Data Packet Received */
            if (epxintsts & HSUSBD_EPINTSTS_RXPKIF_Msk)
            {
                HSUSBD->EP[ep_hw_index].EPINTEN &= ~HSUSBD_EPINTEN_RXPKIEN_Msk;
                events->out(ep_logic_index);
            }

            /* OUT token packet */
            if (epxintsts & HSUSBD_EPINTSTS_OUTTKIF_Msk)
            {

            }

            /* IN token packet */
            if (epxintsts & HSUSBD_EPINTSTS_INTKIF_Msk)
            {
            }

            /* PING packet */
            if (epxintsts & HSUSBD_EPINTSTS_PINGIF_Msk)
            {
            }

            /* NAK handshake packet sent to Host */
            if (epxintsts & HSUSBD_EPINTSTS_NAKIF_Msk)
            {

            }

            /* STALL handshake packet sent to Host */
            if (epxintsts & HSUSBD_EPINTSTS_STALLIF_Msk)
            {

            }

            /* NYET handshake packet sent to Host */
            if (epxintsts & HSUSBD_EPINTSTS_NYETIF_Msk)
            {

            }

            /* ERR packet sent to Host */
            if (epxintsts & HSUSBD_EPINTSTS_ERRIF_Msk)
            {
            }

            /* Bulk Out Short Packet Received */
            if (epxintsts & HSUSBD_EPINTSTS_SHORTRXIF_Msk)
            {
            }
        }
        gintsts_epx = gintsts_epx >> 1;
        ep_hw_index++;
    }
#endif
}

extern "C" void USBD_IRQHandler(void)
{
#if defined (TARGET_NUC472)
     NVIC_DisableIRQ(USBD_IRQn);

    instance->events->start_process();

    NVIC_ClearPendingIRQ(USBD_IRQn);
    NVIC_EnableIRQ(USBD_IRQn);
#elif defined (TARGET_M480)
     NVIC_DisableIRQ(USBD20_IRQn);

    instance->events->start_process();

    NVIC_ClearPendingIRQ(USBD20_IRQn);
    NVIC_EnableIRQ(USBD20_IRQn);
#endif
}

#endif

#endif