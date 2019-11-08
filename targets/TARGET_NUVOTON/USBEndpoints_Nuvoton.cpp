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

#include "USBPhyHw.h"
#include "USBEndpoints_Nuvoton.h"
#include "mbed_critical.h"

static USBPhyHw *instance;

#undef  MBED_CONF_TARGET_USB_DEVICE_HSUSBD
#define MBED_CONF_TARGET_USB_DEVICE_HSUSBD 1  /* USB 1.1 Only */
 
uint32_t volatile g_ep_index, read_req = 0;

void chip_config(void)
{
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
}

static volatile int epComplete = 0;

/* Conversion macros */
#define DESC_TO_LOG(endpoint)   ((endpoint) & 0xF)
#define NU_EPH2EPL(ep)          ((ep) + 1)
#define NU_EPL2EPH(ep)          (ep - 1)

#define EP_DIR_Msk        0x80
#define EP_DIR_OUT        0x00
#define EP_DIR_IN         0x80

#define HSUSBD_GET_EP_MAX_PAYLOAD(ep)     HSUSBD->EP[ep].EPMPS
#define HSUSBD_GET_EP_DATA_COUNT(ep)      (HSUSBD->EP[ep].EPDATCNT & 0xFFFFF)
#define HSUSBD_SET_EP_SHORT_PACKET(ep)    HSUSBD->EP[ep].EPRSPCTL = ((HSUSBD->EP[ep].EPRSPCTL & 0x10) | 0x40)
#define HSUSBD_GET_EP_INT_EN(ep)          HSUSBD->EP[ep].EPINTEN

#define USBD_GET_EP_MAX_PAYLOAD(ep)       *((__IO uint32_t *) ((uint32_t)&USBD->EPAMPS + (uint32_t)(ep*0x28)))
#define USBD_GET_EP_DATA_COUNT(ep)        *((__IO uint32_t *) ((uint32_t)&USBD->EPADATCNT + (uint32_t)(ep*0x28)))
#define USBD_SET_EP_SHORT_PACKET(ep)      *((__IO uint32_t *) ((uint32_t)&USBD->EPARSPCTL+(uint32_t)(ep*0x28)))=((*((__IO uint32_t *)((uint32_t)&USBD->EPARSPCTL+(uint32_t)(ep*0x28))) & 0x10) | 0x40)
#define USBD_GET_EP_INT_EN(ep)            *((__IO uint32_t *) ((uint32_t)&USBD->EPAINTEN + (uint32_t)(ep*0x28)))

static volatile uint32_t s_ep_compl = 0;
static volatile uint32_t s_ep_buf_ind = 0;
static volatile uint32_t s_ep0_max_packet_size = 64;
static volatile uint8_t s_usb_addr = 0;

static volatile S_HSUSBD_CMD_T s_setup;

static volatile uint8_t *g_usbd_CtrlInPointer = 0;
static uint32_t g_usbd_CtrlMaxPktSize = 64;
static volatile uint32_t g_usb_CtrlInSize = 0;
static uint32_t g_usb_ShortPacket = 0;
static uint32_t gEpReadCnt = 0;
static uint8_t set_addr = 0;

void HSUSBD_CtrlInput(void)
{
    unsigned volatile i;
    uint32_t volatile count;
    
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

    HSUSBD_ENABLE_PHY();
    
    while (1) 
    {
        HSUSBD->EP[0].EPMPS = 0x20;
        if (HSUSBD->EP[0].EPMPS == 0x20)
            break;
    }

    /* Set SE0 (disconnect) */
    HSUSBD_SET_SE0();
    
    NVIC_SetVector(USBD20_IRQn, (uint32_t) &_usbisr);
    NVIC_EnableIRQ(USBD20_IRQn);

    instance = this;  
}

void USBPhyHw::deinit()
{
    disconnect();

    NVIC_DisableIRQ(USBD20_IRQn);

    HSUSBD_SET_SE0();
    HSUSBD_DISABLE_PHY();

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

    HSUSBD_ResetDMA();
    HSUSBD_SET_ADDR(0);

    /**
      * Control Transfer Packet Size Constraints
      * low-speed: 8
      * full-speed: 8, 16, 32, 64
      * high-speed: 64
      */
    /* Control endpoint */
    HSUSBD_SetEpBufAddr(CEP, 0, s_ep0_max_packet_size);
    s_ep_buf_ind = s_ep0_max_packet_size;

    /* Enable USB/CEP interrupt */
    HSUSBD_ENABLE_USB_INT(HSUSBD_GINTEN_USBIEN_Msk | HSUSBD_GINTEN_CEPIEN_Msk);
    HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk|HSUSBD_CEPINTEN_STSDONEIEN_Msk);

    /* Enable BUS interrupt */
    HSUSBD_ENABLE_BUS_INT(
        HSUSBD_BUSINTEN_RESUMEIEN_Msk |
        HSUSBD_BUSINTEN_RSTIEN_Msk |
        HSUSBD_BUSINTEN_VBUSDETIEN_Msk
    );

    /* Clear SE0 (connect) */
    HSUSBD_CLR_SE0();
}

/* Make the USB phy visible to the USB host.
   Enable either the D+ or D- pullup so the host can detect the presence of this device. */
void USBPhyHw::disconnect()
{
    /* Set SE0 (disconnect) */
    HSUSBD_SET_SE0();
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
    HSUSBD->BUSINTEN |= HSUSBD_BUSINTEN_SOFIEN_Msk;
}

/* Disable the start of frame interrupt. */
void USBPhyHw::sof_disable()
{
    HSUSBD->BUSINTEN &= ~HSUSBD_BUSINTEN_SOFIEN_Msk;
}

/* Set the USBPhy's address. */
void USBPhyHw::set_address(uint8_t address)
{
    s_usb_addr = address;
}

/* Wake upstream devices */
void USBPhyHw::remote_wakeup()
{
    HSUSBD->OPER |= HSUSBD_OPER_RESUMEEN_Msk;
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
    *((uint16_t *) (buffer + 0)) = (uint16_t) HSUSBD->SETUP1_0;
    *((uint16_t *) (buffer + 2)) = (uint16_t) HSUSBD->SETUP3_2;
    *((uint16_t *) (buffer + 4)) = (uint16_t) HSUSBD->SETUP5_4;
    *((uint16_t *) (buffer + 6)) = (uint16_t) HSUSBD->SETUP7_6;

    s_setup.bmRequestType = (uint8_t) (HSUSBD->SETUP1_0 & 0xff);
    s_setup.bRequest = (int8_t) (HSUSBD->SETUP1_0 >> 8) & 0xff;
    s_setup.wValue = (uint16_t) HSUSBD->SETUP3_2;
    s_setup.wIndex = (uint16_t) HSUSBD->SETUP5_4;
    s_setup.wLength = (uint16_t) HSUSBD->SETUP7_6;    
}

/*
  Start receiving a packet of up to wMaxPacketSize on endpoint 0.
*/
void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
    if (s_setup.wLength && ! (s_setup.bmRequestType & 0x80))
    {
        /* Control OUT */
        read_buffers[0] = data;
        read_sizes[0] = size;   
        HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk | HSUSBD_CEPINTEN_RXPKIEN_Msk);
    }
    else
    {
        /* Status stage */
        HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
        HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
        HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk|HSUSBD_CEPINTEN_INTKIEN_Msk);       
    }
}
/*
  Read the contents of a received packet.
*/
uint32_t USBPhyHw::ep0_read_result()
{
    uint32_t i;
    uint8_t *ep0_data = read_buffers[0];

    uint32_t ceprxcnt = HSUSBD->CEPRXCNT;

    for (i = 0; i < ceprxcnt; i ++)
        ep0_data[i] = HSUSBD->CEPDAT_BYTE;

    return ceprxcnt;
}

/* Write a packet on endpoint 0. */
void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
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
}

void stallEndpoint(uint8_t endpoint)
{
    uint32_t  ep_hw_index = NU_EPL2EPH(endpoint);
    if (ep_hw_index >= NUMBER_OF_PHYSICAL_ENDPOINTS)
        return;
    HSUSBD_SetStall(ep_hw_index);
}

/* Protocol stall on endpoint 0.
   Stall all IN and OUT packets on endpoint 0 until a setup packet is received.
   Note The stall is cleared automatically when a setup packet is received 
*/
void USBPhyHw::ep0_stall()
{
    HSUSBD_SetStall(0);
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

    if (ep_dir == 0)
        HSUSBD_ENABLE_EP_INT(ep_hw_index, HSUSBD_EPINTEN_RXPKIEN_Msk);
    else
        HSUSBD_ENABLE_EP_INT(ep_hw_index, HSUSBD_EPINTEN_TXPKIEN_Msk);    

     return true;
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    uint32_t ep_hw_index = NU_EPL2EPH(DESC_TO_LOG(endpoint));
    HSUSBD->EP[ep_hw_index].EPCFG = HSUSBD->EP[ep_hw_index].EPCFG  & ~HSUSBD_EP_CFG_VALID;
}

/*
  Perform a functional stall on the given endpoint.
  Set the HALT feature for this endpoint so that all further communication is aborted. 
*/
void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    HSUSBD_SetEpStall(DESC_TO_LOG(endpoint));
}

/*
   Unstall the endpoint.
   Clear the HALT feature on this endpoint so communication can resume. 
 */
void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    HSUSBD_ClearEpStall(DESC_TO_LOG(endpoint));
}

/* Start a read on the given endpoint. */
bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
/* Store the buffer address & length */
    uint8_t log = DESC_TO_LOG(endpoint);
    if(read_sizes[log])
       return false;
    read_req++;
    read_buffers[log] = data;
    read_sizes[log] = size;
    return true;
}

/* 
  Finish a read on the given endpoint 
  Returns The number of bytes received
*/
uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    uint8_t log = DESC_TO_LOG(endpoint);

    uint32_t bytes_read = 0;
    core_util_critical_section_enter();
    endpoint_read_result_core(endpoint, read_buffers[log], read_sizes[log], &bytes_read);

    read_buffers[log] = 0;
    read_sizes[log] = 0;
    core_util_critical_section_exit();
    return bytes_read;
}

uint32_t volatile buffer, len, i;

bool USBPhyHw::endpoint_read_result_core(usb_ep_t endpoint, uint8_t *data, uint32_t size, uint32_t *bytes_read)
{
    uint8_t log = DESC_TO_LOG(endpoint);
    uint32_t tmp;
    uint8_t __attribute__((aligned(4))) tmp_buffer[4];

//core_util_critical_section_enter();
    gEpReadCnt = HSUSBD_GET_EP_DATA_COUNT(NU_EPL2EPH(DESC_TO_LOG(endpoint)));

    if(gEpReadCnt == 0) 
    {
        *bytes_read = 0;
        return true;
    }
#if 0
    for(i=0;i<gEpReadCnt;i++)
    {
        if(data)
            data[i]  = HSUSBD->EP[NU_EPL2EPH(DESC_TO_LOG(endpoint))].EPDAT_BYTE ;
        else
            tmp  = HSUSBD->EP[NU_EPL2EPH(DESC_TO_LOG(endpoint))].EPDAT_BYTE ;
    }
#else
    buffer = (uint32_t)data;

    if(buffer % 4)
    {
        len = 4 - (buffer % 4);
        if(gEpReadCnt <= len)
            len = gEpReadCnt;

        while(1)
        {
            if (!(HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk))
                break;
            else if (!HSUSBD_IS_ATTACHED())
                break;
        }
        HSUSBD_SET_DMA_LEN(len);
        HSUSBD_SET_DMA_ADDR((uint32_t)tmp_buffer);
        HSUSBD_SET_DMA_WRITE(DESC_TO_LOG(endpoint));
        HSUSBD_ENABLE_DMA();

        while(1)
        {
            if (!(HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk))
                break;
            else if (!HSUSBD_IS_ATTACHED())
                break;
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
        HSUSBD_SET_DMA_LEN(len);
        HSUSBD_SET_DMA_ADDR(buffer);
        HSUSBD_SET_DMA_WRITE(DESC_TO_LOG(endpoint));
        HSUSBD_ENABLE_DMA();
        while(1)
        {
            if (!(HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk))
                break;
            else if (!HSUSBD_IS_ATTACHED())
                break;
        }
    }   
#endif
    s_ep_compl &= ~(1 << NU_EPL2EPH(DESC_TO_LOG(endpoint)));
    *bytes_read = gEpReadCnt;   

//core_util_critical_section_exit();
    return true;
}

/*
  Start a write on the given endpoint.
  true if the data was prepared for transmit, false otherwise
*/
bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
   uint32_t ep_logic_index = DESC_TO_LOG(endpoint);
   uint8_t __attribute__((aligned(4))) tmp_buffer[4];

    if(ep_logic_index == 0)
        return false;
    else 
    {
        uint32_t ep_hw_index = NU_EPL2EPH(DESC_TO_LOG(endpoint));
        uint32_t mps = HSUSBD_GET_EP_MAX_PAYLOAD(ep_hw_index);
        uint32_t buffer, len, i;
        if (size > mps)
        {
            return false;
        }

        if(HSUSBD->EP[ep_hw_index].EPDATCNT & 0xFFFF)
        {
               HSUSBD_SET_EP_SHORT_PACKET(ep_hw_index);
            return false;
        }
        if(size < mps)
            g_usb_ShortPacket = 1;
#if 0
        for(i=0;i<size;i++)
            HSUSBD->EP[ep_hw_index].EPDAT_BYTE = data[i];
#else
        while(1)
        {
            if (!(HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk))
                break;
            else if (!HSUSBD_IS_ATTACHED())
                break;
        }
        buffer = (uint32_t)data;
        if(buffer % 4)
        {
            len = 4 - (buffer % 4);
            if(size <= len)
                len = size;
            for(i=0;i<len;i++)
                HSUSBD->EP[ep_hw_index].EPDAT_BYTE = data[i];
            buffer = buffer + len;
            len = size - len;
        }
        else
            len = size;

        if(len)
        {
            s_ep_compl |= (1 << ep_logic_index);
            HSUSBD_SET_DMA_LEN(len);
            HSUSBD_SET_DMA_ADDR(buffer);
            HSUSBD_SET_DMA_READ(ep_logic_index);
            HSUSBD_ENABLE_DMA();  
            while(1)
            {
                if (!(HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk))
                    break;
                else if (!HSUSBD_IS_ATTACHED())
                    break;
            }
        }
#endif
        if(g_usb_ShortPacket)
            HSUSBD_SET_EP_SHORT_PACKET(ep_hw_index);

//core_util_critical_section_exit();
    }
    return true;
}

/* Abort the current transfer if it has not yet been sent. */
void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
}

/* 
  Callback used for performing USB processing.
  USBPhy processing should be triggered by calling USBPhyEvents::start_process and done inside process. All USBPhyEvents callbacks aside from USBPhyEvents::start_process must be called in the context of process
*/

 uint32_t volatile sp_debug = 0; 

void USBPhyHw::process()
{
    uint32_t gintsts = HSUSBD->GINTSTS & HSUSBD->GINTEN;
    uint32_t gintsts_epx = gintsts >> 2;     /* EPA, EPB, EPC, ... EPL interrupts */
    uint32_t ep_hw_index = 0;

    if (! gintsts)
    {
        return;
    }
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
            HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk|HSUSBD_BUSINTEN_SUSPENDIEN_Msk | HSUSBD_BUSINTEN_SOFIEN_Msk | HSUSBD_BUSINTEN_SOFIEN_Msk);
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_RESUMEIF_Msk);
//            events->suspend(false);
        }

        /* Suspend */
        if (busintsts & HSUSBD_BUSINTSTS_SUSPENDIF_Msk)
        {
            HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk | HSUSBD_BUSINTEN_RESUMEIEN_Msk |HSUSBD_BUSINTEN_SOFIEN_Msk);
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_SUSPENDIF_Msk);
        }

        /* High-speed */
        if (busintsts & HSUSBD_BUSINTSTS_HISPDIF_Msk)
        {
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk);
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
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_TXPKIEN_Msk | HSUSBD_CEPINTEN_SETUPPKIEN_Msk);
                HSUSBD_CtrlInput();
            }
            else
            {
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_TXPKIEN_Msk|HSUSBD_CEPINTEN_STSDONEIEN_Msk);
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
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk);

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
//                s_ep_compl &= ~(1 << (NU_EPH2EPL(ep_hw_index)));
                events->in(NU_EPH2EPL(ep_hw_index) | EP_DIR_Msk);
            }

            /* Data Packet Received */
            if (epxintsts & HSUSBD_EPINTSTS_RXPKIF_Msk)
            {
                 g_ep_index = NU_EPH2EPL(ep_hw_index);
                 s_ep_compl |= (1 << ep_hw_index);

                 if(read_req == 0)//read_buffers[g_ep_index] == 0 && read_sizes[g_ep_index] == 0)
                     endpoint_read_result(g_ep_index);
                 else
								 {
                     events->out(g_ep_index);
									 read_req--;
								 }
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
}

void USBPhyHw::_usbisr(void)
{
    NVIC_DisableIRQ(USBD20_IRQn);

    instance->events->start_process();

    NVIC_ClearPendingIRQ(USBD20_IRQn);
    NVIC_EnableIRQ(USBD20_IRQn);
}

