/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

#if defined(TARGET_M480)

#include "USBHAL.h"
#include "M480.h"
#include "pinmap.h"

/**
 * EP: mbed USBD defined endpoint, e.g. EP0OUT/IN, EP1OUT/IN, EP2OUT/IN.
 * EPX: BSP defined endpoint, e.g. CEP, EPA, EPB, EPC.
 */

USBHAL * USBHAL::instance;

#if (MBED_CONF_TARGET_USB_DEVICE_HSUSBD == 0)

/* Global variables for Control Pipe */
extern uint8_t g_usbd_SetupPacket[];        /*!< Setup packet buffer */

static volatile uint32_t s_ep_compl = 0;
static volatile uint32_t s_ep_buf_ind = 8;
static volatile uint8_t s_usb_addr = 0;
static volatile uint8_t s_ep_data_bit[NUMBER_OF_PHYSICAL_ENDPOINTS] = {1};
static volatile uint8_t s_ep_mxp[NUMBER_OF_PHYSICAL_ENDPOINTS] = {0};

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

USBHAL::USBHAL(void)
{
    SYS_UnlockReg();

    s_ep_buf_ind = 8;

    memset(epCallback, 0x00, sizeof (epCallback));
    epCallback[0] = &USBHAL::EP1_OUT_callback;
    epCallback[1] = &USBHAL::EP2_IN_callback;
    epCallback[2] = &USBHAL::EP3_OUT_callback;
    epCallback[3] = &USBHAL::EP4_IN_callback;
    epCallback[4] = &USBHAL::EP5_OUT_callback;
    epCallback[5] = &USBHAL::EP6_IN_callback;
    epCallback[6] = &USBHAL::EP7_OUT_callback;
    epCallback[7] = &USBHAL::EP8_IN_callback;
    epCallback[8] = &USBHAL::EP9_OUT_callback;
    epCallback[9] = &USBHAL::EP10_IN_callback;
    epCallback[10] = &USBHAL::EP11_OUT_callback;
    epCallback[11] = &USBHAL::EP12_IN_callback;

    instance = this;

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
    
    /* Initial USB engine */
    USBD->ATTR = 0x7D0;

    /* Set SE0 (disconnect) */
    USBD_SET_SE0();

    //NVIC_SetVector(OTG_FS_IRQn, (uint32_t) &_usbisr);
    NVIC_SetVector(USBD_IRQn, (uint32_t) &_usbisr);
    NVIC_EnableIRQ(USBD_IRQn);
}

USBHAL::~USBHAL(void)
{
    NVIC_DisableIRQ(USBD_IRQn);
    USBD_SET_SE0();
    USBD_DISABLE_PHY();
}

void USBHAL::connect(void)
{
    USBD->STBUFSEG = 0;
    frame_cnt = 0;
    /* EP0 ==> control IN endpoint, address 0 */
    USBD_CONFIG_EP(EP0, USBD_CFG_CSTALL | USBD_CFG_EPMODE_IN | 0);
    /* Buffer range for EP0 */
    USBD_SET_EP_BUF_ADDR(EP0, s_ep_buf_ind);

    /* EP1 ==> control OUT endpoint, address 0 */
    USBD_CONFIG_EP(EP1, USBD_CFG_CSTALL | USBD_CFG_EPMODE_OUT | 0);
    /* Buffer range for EP1 */
    USBD_SET_EP_BUF_ADDR(EP1, s_ep_buf_ind);

    s_ep_buf_ind += MAX_PACKET_SIZE_EP0;

    /* Disable software-disconnect function */
    USBD_CLR_SE0();

    /* Clear USB-related interrupts before enable interrupt */
    USBD_CLR_INT_FLAG(USBD_INT_BUS | USBD_INT_USB | USBD_INT_FLDET | USBD_INT_WAKEUP);

    /* Enable USB-related interrupts. */
    USBD_ENABLE_INT(USBD_INT_BUS | USBD_INT_USB | USBD_INT_FLDET | USBD_INT_WAKEUP);
}

void USBHAL::disconnect(void)
{
    /* Set SE0 (disconnect) */
    USBD_SET_SE0();
}

void USBHAL::configureDevice(void)
{
    /**
     * In USBDevice.cpp > USBDevice::requestSetConfiguration, configureDevice() is called after realiseEndpoint() (in USBCallback_setConfiguration()).
     * So we have the following USB buffer management policy:
     *  1. Allocate for CEP on connect().
     *  2. Allocate for EPX in realiseEndpoint().
     *  3. Deallocate all except for CEP in unconfigureDevice().
     */
}

void USBHAL::unconfigureDevice(void)
{
    s_ep_buf_ind = 8;
}

void USBHAL::setAddress(uint8_t address)
{
    // NOTE: Delay address setting; otherwise, USB controller won't ack.
    s_usb_addr = address;
}

void USBHAL::remoteWakeup(void)
{
#if 0
    USBD->OPER |= USBD_OPER_RESUMEEN_Msk;
#endif
}

bool USBHAL::realiseEndpoint(uint8_t endpoint, uint32_t maxPacket, uint32_t options)
{
    uint32_t ep_type = 0;
    uint32_t ep_hw_index = NU_EP2EPH(endpoint);
    uint32_t ep_logic_index = NU_EP2EPL(endpoint);
    uint32_t ep_dir = (NU_EP_DIR(endpoint) == NU_EP_DIR_IN) ? USBD_CFG_EPMODE_IN : USBD_CFG_EPMODE_OUT;

    if(ep_logic_index == 3 || ep_logic_index == 4)
        ep_type = USBD_CFG_TYPE_ISO;

    USBD_CONFIG_EP(ep_hw_index, ep_dir | ep_type | ep_logic_index);
    /* Buffer range */
    USBD_SET_EP_BUF_ADDR(ep_hw_index, s_ep_buf_ind);

    if(ep_dir == USBD_CFG_EPMODE_OUT)
        USBD_SET_PAYLOAD_LEN(ep_hw_index, maxPacket);

    s_ep_mxp[ep_logic_index] = maxPacket;

    s_ep_buf_ind += maxPacket;

    return true;
}

void USBHAL::EP0setup(uint8_t *buffer)
{
    uint32_t sz;
    endpointReadResult(EP0OUT, buffer, &sz);
}

void USBHAL::EP0read(void)
{


}

void USBHAL::EP0readStage(void)
{
    // N/A

    USBD_PrepareCtrlOut(0,0);
}

uint32_t USBHAL::EP0getReadResult(uint8_t *buffer)
{
    uint32_t i;
    uint8_t *buf = (uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP1));
    uint32_t ceprxcnt = USBD_GET_PAYLOAD_LEN(EP1);
    for (i = 0; i < ceprxcnt; i ++)
        buffer[i] = buf[i];
    USBD_SET_PAYLOAD_LEN(EP1, MAX_PACKET_SIZE_EP0);
    return ceprxcnt;
}

void USBHAL::EP0write(uint8_t *buffer, uint32_t size)
{
    if (buffer && size) {
        if(s_ep_data_bit[0] & 1)
            USBD_SET_DATA1(EP0);
        else
            USBD_SET_DATA0(EP0);
        s_ep_data_bit[0]++;

        USBD_MemCopy((uint8_t *)USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0), buffer, size);
        USBD_SET_PAYLOAD_LEN(EP0, size);
        if(size < MAX_PACKET_SIZE_EP0)
            s_ep_data_bit[0] = 1;

    } else {
        if(g_usbd_SetupPacket[0] & 0x80) {	//Device to Host
            // Status stage
            //			USBD_PrepareCtrlOut(0,0);
        } else {
            USBD_SET_DATA1(EP0);
            USBD_SET_PAYLOAD_LEN(EP0, 0);
        }
    }
}

void USBHAL::EP0getWriteResult(void)
{
    // N/A
}

void USBHAL::EP0stall(void)
{
    stallEndpoint(EP0OUT);
}

EP_STATUS USBHAL::endpointRead(uint8_t endpoint, uint32_t maximumSize)
{
    return EP_PENDING;
}

EP_STATUS USBHAL::endpointReadResult(uint8_t endpoint, uint8_t * buffer, uint32_t *bytesRead) //spcheng
{
    if(endpoint == EP0OUT) {
        USBD_MemCopy(g_usbd_SetupPacket, (uint8_t *)USBD_BUF_BASE, 8);
        if (buffer) {
            USBD_MemCopy(buffer, g_usbd_SetupPacket, 8);
        }
        USBD_SET_PAYLOAD_LEN(EP1, MAX_PACKET_SIZE_EP0);
    } else {
        uint32_t i;
        uint8_t *buf = (uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(NU_EP2EPH(endpoint)));
        uint32_t eprxcnt = USBD_GET_PAYLOAD_LEN(NU_EP2EPH(endpoint));
        for (i = 0; i < eprxcnt; i ++)
            buffer[i] = buf[i];

        *bytesRead = eprxcnt;

        USBD_SET_PAYLOAD_LEN(NU_EP2EPH(endpoint),s_ep_mxp[NU_EPH2EPL(NU_EP2EPL(endpoint))]);
    }
    return EP_COMPLETED;
}


uint32_t USBHAL::endpointReadcore(uint8_t endpoint, uint8_t *buffer)
{
    return 0;
}

EP_STATUS USBHAL::endpointWrite(uint8_t endpoint, uint8_t *data, uint32_t size)
{
    uint32_t ep_logic_index = NU_EP2EPL(endpoint);
    if(ep_logic_index == 0)
        return EP_INVALID;
    else {
        uint8_t *buf;
        uint32_t i=0;
        uint32_t ep_hw_index = NU_EP2EPH(endpoint);
        s_ep_compl |= (1 << ep_logic_index);
        buf = (uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(ep_hw_index));
        for(i=0; i<size; i++)
            buf[i] = data[i];

        /* Set transfer length and trigger IN transfer */
        USBD_SET_PAYLOAD_LEN(ep_hw_index, size);

    }
    return EP_PENDING;
}

EP_STATUS USBHAL::endpointWriteResult(uint8_t endpoint)
{
    if (!(s_ep_compl & (1 << NU_EP2EPL(endpoint))))
        return EP_COMPLETED;
    return EP_PENDING;
}

void USBHAL::stallEndpoint(uint8_t endpoint)
{
    uint32_t ep_hw_index = NU_EP2EPH(endpoint);
    if (ep_hw_index >= NUMBER_OF_PHYSICAL_ENDPOINTS)
        return;

    USBD_SetStall(NU_EPH2EPL(ep_hw_index));

}

void USBHAL::unstallEndpoint(uint8_t endpoint)
{
    uint32_t ep_hw_index = NU_EP2EPH(endpoint);
    if (ep_hw_index >= NUMBER_OF_PHYSICAL_ENDPOINTS)
        return;
    USBD_ClearStall(NU_EPH2EPL(ep_hw_index));
}

bool USBHAL::getEndpointStallState(uint8_t endpoint)
{
    uint32_t ep_hw_index = NU_EP2EPH(endpoint);
    if (ep_hw_index >= NUMBER_OF_PHYSICAL_ENDPOINTS)
        return false;

    return USBD_GetStall(NU_EPH2EPL(ep_hw_index)) ? 1 : 0;
}

void USBHAL::_usbisr(void)
{
    MBED_ASSERT(instance);
    instance->usbisr();
}

void USBHAL::usbisr(void)
{
    uint32_t u32IntSts = USBD_GET_INT_FLAG();
    uint32_t u32State = USBD_GET_BUS_STATE();

//------------------------------------------------------------------
    if(u32IntSts & USBD_INTSTS_VBDETIF_Msk) {
        // Floating detect
        USBD_CLR_INT_FLAG(USBD_INTSTS_VBDETIF_Msk);

        if(USBD_IS_ATTACHED()) {
            /* USB Plug In */
            USBD_ENABLE_USB();
        } else {
            /* USB Un-plug */
            USBD_DISABLE_USB();
        }
    }

//------------------------------------------------------------------
    if(u32IntSts & USBD_INTSTS_BUSIF_Msk) {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_BUSIF_Msk);

        if(u32State & USBD_ATTR_USBRST_Msk) {
            /* Bus reset */
            USBD_ENABLE_USB();
            USBD_SwReset();
        }
        if(u32State & USBD_ATTR_SUSPEND_Msk) {
            /* Enable USB but disable PHY */
            USBD_DISABLE_PHY();
        }
        if(u32State & USBD_ATTR_RESUME_Msk) {
            /* Enable USB and enable PHY */
            USBD_ENABLE_USB();
        }
    }

    if(u32IntSts & USBD_INTSTS_USBIF_Msk) {
        // USB event
        if(u32IntSts & USBD_INTSTS_SETUP_Msk) {
            // Setup packet
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_SETUP_Msk);

            /* Clear the data IN/OUT ready flag of control end-points */
            USBD_STOP_TRANSACTION(EP0);
            USBD_STOP_TRANSACTION(EP1);
            EP0setupCallback();
        }

        // EP events
        if(u32IntSts & USBD_INTSTS_EP0) {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP0);
            // control IN
            EP0in();

            // In ACK for Set address
            if((g_usbd_SetupPacket[0] == REQ_STANDARD) && (g_usbd_SetupPacket[1] == USBD_SET_ADDRESS)) {
                if((USBD_GET_ADDR() != s_usb_addr) && (USBD_GET_ADDR() == 0)) {
                    USBD_SET_ADDR(s_usb_addr);
                }
            }
        }
        if(u32IntSts & USBD_INTSTS_EP1) {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP1);

            // control OUT
            EP0out();
        }

        uint32_t gintsts_epx = (u32IntSts >> 18) & 0x3F;
        uint32_t ep_hw_index = 2;
        while (gintsts_epx) {
            if(gintsts_epx & 0x01) {
                uint32_t ep_status = (USBD_GET_EP_FLAG() >> (ep_hw_index * 3 + 8)) & 0x7;
                /* Clear event flag */
                USBD_CLR_INT_FLAG(1 << (ep_hw_index + 16));

                if(ep_status == 0x02 || ep_status == 0x06 || (ep_status == 0x07 && NU_EPH2EPL(ep_hw_index) == 3)) {	//RX
                    if(ep_status == 0x07)
                        SOF(frame_cnt++);
                    if ((instance->*(epCallback[ep_hw_index-2]))()) {

                    }
                    USBD_SET_PAYLOAD_LEN(ep_hw_index,s_ep_mxp[NU_EPH2EPL(ep_hw_index)]);
                } else if(ep_status == 0x00 || ep_status == 0x07) {	//TX
                    s_ep_compl &= ~(1 << (NU_EPH2EPL(ep_hw_index)));
                    if ((instance->*(epCallback[ep_hw_index-2]))()) {
                    }
                }
            }

            gintsts_epx = gintsts_epx >> 1;
            ep_hw_index++;
        }
    }
}
#else

static volatile uint32_t s_ep_compl = 0;
static volatile uint32_t s_ep_buf_ind = 0;
static volatile uint8_t s_usb_addr = 0;
static volatile S_HSUSBD_CMD_T s_setup;
static volatile uint16_t s_ctrlin_packetsize;
static uint8_t *g_usbd_CtrlInPointer = 0;
static uint32_t g_usbd_CtrlMaxPktSize = 64;
static volatile uint32_t g_usbd_CtrlInSize;
static uint32_t g_usbd_ShortPacket = 0;
//static uint32_t gEpRead = 0;
static uint32_t gEpReadCnt = 0;

void HSUSBD_CtrlInput(void)
{
    unsigned volatile i;
    uint32_t volatile count;

    // Process remained data
    if(g_usbd_CtrlInSize >= g_usbd_CtrlMaxPktSize) {
        // Data size > MXPLD
        for (i=0; i<(g_usbd_CtrlMaxPktSize >> 2); i++, g_usbd_CtrlInPointer+=4)
            HSUSBD->CEPDAT = *(uint32_t *)g_usbd_CtrlInPointer;
        HSUSBD_START_CEP_IN(g_usbd_CtrlMaxPktSize);
        g_usbd_CtrlInSize -= g_usbd_CtrlMaxPktSize;
    } else {
        // Data size <= MXPLD
        for (i=0; i<(g_usbd_CtrlInSize >> 2); i++, g_usbd_CtrlInPointer+=4)
            HSUSBD->CEPDAT = *(uint32_t *)g_usbd_CtrlInPointer;

        count = g_usbd_CtrlInSize % 4;
        for (i=0; i<count; i++)
            HSUSBD->CEPDAT_BYTE = *(uint8_t *)(g_usbd_CtrlInPointer + i);

        HSUSBD_START_CEP_IN(g_usbd_CtrlInSize);
        g_usbd_CtrlInPointer = 0;
        g_usbd_CtrlInSize = 0;
    }
}

USBHAL::USBHAL(void)
{
    SYS_UnlockReg();

    s_ep_buf_ind = 0;

    memset(epCallback, 0x00, sizeof (epCallback));
    epCallback[0] = &USBHAL::EP1_OUT_callback;
    epCallback[1] = &USBHAL::EP2_IN_callback;
    epCallback[2] = &USBHAL::EP3_OUT_callback;
    epCallback[3] = &USBHAL::EP4_IN_callback;
    epCallback[4] = &USBHAL::EP5_OUT_callback;
    epCallback[5] = &USBHAL::EP6_IN_callback;
    epCallback[6] = &USBHAL::EP7_OUT_callback;
    epCallback[7] = &USBHAL::EP8_IN_callback;
    epCallback[8] = &USBHAL::EP9_OUT_callback;
    epCallback[9] = &USBHAL::EP10_IN_callback;
    epCallback[10] = &USBHAL::EP11_OUT_callback;
    epCallback[11] = &USBHAL::EP12_IN_callback;

    instance = this;

    /* Configure HSUSB to Device mode */
    SYS->USBPHY = (SYS->USBPHY & ~SYS_USBPHY_HSUSBROLE_Msk) | SYS_USBPHY_HSUSBROLE_STD_USBD;
    /* Enable HSUSB PHY */
    SYS->USBPHY = (SYS->USBPHY & ~(SYS_USBPHY_HSUSBEN_Msk | SYS_USBPHY_HSUSBACT_Msk)) | SYS_USBPHY_HSUSBEN_Msk;
    /* Delay >10 us and then switch HSUSB PHY from reset state to active state */
    wait_us(10);
    SYS->USBPHY |= SYS_USBPHY_HSUSBACT_Msk;

    /* Enable USBD module clock */
    CLK_EnableModuleClock(HSUSBD_MODULE);

    /* Enable USB PHY and wait for it ready */
    HSUSBD_ENABLE_PHY();
    while (1) {
        HSUSBD->EP[0].EPMPS = 0x20;
        if (HSUSBD->EP[0].EPMPS == 0x20)
            break;
    }

    /* Force to full-speed */
    HSUSBD->OPER = 0;//USBD_OPER_HISPDEN_Msk;

    /* Set SE0 (disconnect) */
    HSUSBD_SET_SE0();

    NVIC_SetVector(USBD20_IRQn, (uint32_t) &_usbisr);
    NVIC_EnableIRQ(USBD20_IRQn);
}

USBHAL::~USBHAL(void)
{
    NVIC_DisableIRQ(USBD20_IRQn);
    HSUSBD_SET_SE0();
    HSUSBD_DISABLE_PHY();
}

void USBHAL::connect(void)
{
    HSUSBD_ResetDMA();
    HSUSBD_SET_ADDR(0);

    /**
      * Control Transfer Packet Size Constraints
      * low-speed: 8
      * full-speed: 8, 16, 32, 64
      * high-speed: 64
      */
    /* Control endpoint */
    HSUSBD_SetEpBufAddr(CEP, s_ep_buf_ind, MAX_PACKET_SIZE_EP0);
    s_ep_buf_ind = MAX_PACKET_SIZE_EP0;

    /* Enable USB/CEP interrupt */
    HSUSBD_ENABLE_USB_INT(HSUSBD_GINTEN_USBIEN_Msk | HSUSBD_GINTEN_CEPIEN_Msk);
    HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk|HSUSBD_CEPINTEN_STSDONEIEN_Msk);

    /* Enable BUS interrupt */
    HSUSBD_ENABLE_BUS_INT(
        HSUSBD_BUSINTEN_DMADONEIEN_Msk |
        HSUSBD_BUSINTEN_RESUMEIEN_Msk |
        HSUSBD_BUSINTEN_RSTIEN_Msk |
        HSUSBD_BUSINTEN_VBUSDETIEN_Msk |
        HSUSBD_BUSINTEN_SOFIEN_Msk
    );

    /* Clear SE0 (connect) */
    HSUSBD_CLR_SE0();
}

void USBHAL::disconnect(void)
{
    /* Set SE0 (disconnect) */
    HSUSBD_SET_SE0();
}

void USBHAL::configureDevice(void)
{
    /**
     * In USBDevice.cpp > USBDevice::requestSetConfiguration, configureDevice() is called after realiseEndpoint() (in USBCallback_setConfiguration()).
     * So we have the following USB buffer management policy:
     *  1. Allocate for CEP on connect().
     *  2. Allocate for EPX in realiseEndpoint().
     *  3. Deallocate all except for CEP in unconfigureDevice().
     */
}

void USBHAL::unconfigureDevice(void)
{
    s_ep_buf_ind = MAX_PACKET_SIZE_EP0;
}

void USBHAL::setAddress(uint8_t address)
{
    // NOTE: Delay address setting; otherwise, USB controller won't ack.
    s_usb_addr = address;
}

void USBHAL::remoteWakeup(void)
{
    HSUSBD->OPER |= HSUSBD_OPER_RESUMEEN_Msk;
}

bool USBHAL::realiseEndpoint(uint8_t endpoint, uint32_t maxPacket, uint32_t options)
{
    uint32_t ep_type;
    uint32_t ep_hw_index = NU_EP2EPH(endpoint);

    HSUSBD_SetEpBufAddr(ep_hw_index, s_ep_buf_ind, maxPacket);
    s_ep_buf_ind += maxPacket;
    HSUSBD_SET_MAX_PAYLOAD(ep_hw_index, maxPacket);

    switch (NU_EP2EPL(endpoint)) {
    case 1:
    case 2:
        ep_type = HSUSBD_EP_CFG_TYPE_INT;
        break;

    case 3:
    case 4:
        ep_type = HSUSBD_EP_CFG_TYPE_ISO;
        break;

    default:
        ep_type = HSUSBD_EP_CFG_TYPE_BULK;
    }
    uint32_t ep_dir = (NU_EP_DIR(endpoint) == NU_EP_DIR_IN) ? HSUSBD_EP_CFG_DIR_IN : HSUSBD_EP_CFG_DIR_OUT;
    HSUSBD_ConfigEp(ep_hw_index, NU_EP2EPL(endpoint), ep_type, ep_dir);

    /* Enable USB/EPX interrupt */
    // NOTE: Require USBD_GINTEN_EPAIE_Pos, USBD_GINTEN_EPBIE_Pos, ... USBD_GINTEN_EPLIE_Pos to be consecutive.
    HSUSBD_ENABLE_USB_INT(HSUSBD->GINTEN | HSUSBD_GINTEN_USBIEN_Msk |
                          HSUSBD_GINTEN_CEPIEN_Msk |
                          1 << (ep_hw_index + HSUSBD_GINTEN_EPAIEN_Pos));  // Added USB/EPX interrupt

    if (ep_dir == 0)
        HSUSBD_ENABLE_EP_INT(ep_hw_index, HSUSBD_EPINTEN_RXPKIEN_Msk);
    else
        HSUSBD_ENABLE_EP_INT(ep_hw_index, HSUSBD_EPINTEN_TXPKIEN_Msk);
    return true;
}

void USBHAL::EP0setup(uint8_t *buffer)
{
    uint32_t sz;
    endpointReadResult(EP0OUT, buffer, &sz);
}

void USBHAL::EP0read(void)
{
    if (s_setup.wLength && ! (s_setup.bmRequestType & 0x80)) {
        // Control OUT
        HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk | HSUSBD_CEPINTEN_RXPKIEN_Msk);
    } else {
        // Status stage
        HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
        HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
        HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
    }
}

void USBHAL::EP0readStage(void)
{
    // N/A
}

uint32_t USBHAL::EP0getReadResult(uint8_t *buffer)
{
    uint32_t i;
    uint32_t ceprxcnt = HSUSBD->CEPRXCNT;
    for (i = 0; i < ceprxcnt; i ++)
        *buffer ++ = HSUSBD->CEPDAT_BYTE;
    return ceprxcnt;
}

void USBHAL::EP0write(uint8_t *buffer, uint32_t size)
{
    if (buffer && size) {
        g_usbd_CtrlInPointer = buffer;
        g_usbd_CtrlInSize = size;
        HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
        HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_INTKIEN_Msk);
    } else {
        /* Status stage */
        s_ctrlin_packetsize = 0;
        HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
        HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
        HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
    }
}

void USBHAL::EP0getWriteResult(void)
{
    // N/A
}

void USBHAL::EP0stall(void)
{
    stallEndpoint(EP0OUT);
}

EP_STATUS USBHAL::endpointRead(uint8_t endpoint, uint32_t maximumSize)
{
    return EP_PENDING;
}

EP_STATUS USBHAL::endpointReadResult(uint8_t endpoint, uint8_t * buffer, uint32_t *bytesRead) //spcheng
{
    if(endpoint == EP0OUT) {
        if (buffer) {
            *((uint16_t *) (buffer + 0)) = (uint16_t) HSUSBD->SETUP1_0;
            *((uint16_t *) (buffer + 2)) = (uint16_t) HSUSBD->SETUP3_2;
            *((uint16_t *) (buffer + 4)) = (uint16_t) HSUSBD->SETUP5_4;
            *((uint16_t *) (buffer + 6)) = (uint16_t) HSUSBD->SETUP7_6;
        }

        s_setup.bmRequestType = (uint8_t) (HSUSBD->SETUP1_0 & 0xff);
        s_setup.bRequest = (int8_t) (HSUSBD->SETUP1_0 >> 8) & 0xff;
        s_setup.wValue = (uint16_t) HSUSBD->SETUP3_2;
        s_setup.wIndex = (uint16_t) HSUSBD->SETUP5_4;
        s_setup.wLength = (uint16_t) HSUSBD->SETUP7_6;
    } else {
        if (!(s_ep_compl & (1 << NU_EP2EPL(endpoint)))) {
            while(1) {
                if (!(HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk))
                    break;
                else if (!HSUSBD_IS_ATTACHED())
                    break;
            }
            gEpReadCnt = HSUSBD_GET_EP_DATA_COUNT(NU_EP2EPH(endpoint));
            if(gEpReadCnt == 0) {
                *bytesRead = 0;
                return EP_COMPLETED;
            }
            s_ep_compl |= (1 << NU_EP2EPL(endpoint));
            HSUSBD_SET_DMA_LEN(gEpReadCnt);
            HSUSBD_SET_DMA_ADDR((uint32_t)buffer);
            HSUSBD_SET_DMA_WRITE(NU_EP2EPL(endpoint));
            HSUSBD_ENABLE_DMA();
            return EP_PENDING;;

        } else {
            if ((HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk))
                return EP_PENDING;;

            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_DMADONEIF_Msk);
            s_ep_compl &= ~(1 << NU_EP2EPL(endpoint));
            *bytesRead = gEpReadCnt;
        }
    }
    return EP_COMPLETED;
}


uint32_t USBHAL::endpointReadcore(uint8_t endpoint, uint8_t *buffer)
{
    return 0;
}

EP_STATUS USBHAL::endpointWrite(uint8_t endpoint, uint8_t *data, uint32_t size)
{
    uint32_t ep_logic_index = NU_EP2EPL(endpoint);
    if(ep_logic_index == 0)
        return EP_INVALID;
    else {
        uint32_t ep_hw_index = NU_EP2EPH(endpoint);
        uint32_t mps = HSUSBD_GET_EP_MAX_PAYLOAD(ep_hw_index);
        if (size > mps) {
            return EP_INVALID;
        }
        if(size < mps)
            g_usbd_ShortPacket = 1;
        if (!(s_ep_compl & (1 << NU_EP2EPL(endpoint)))) {
            s_ep_compl |= (1 << ep_logic_index);

            while(1) {
                if (!(HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk))
                    break;
                else if (!HSUSBD_IS_ATTACHED())
                    break;
            }
            HSUSBD_SET_DMA_LEN(size);
            HSUSBD_SET_DMA_ADDR((uint32_t)data);
            HSUSBD_SET_DMA_READ(ep_logic_index);
            HSUSBD_ENABLE_DMA();
        }
    }
    return EP_PENDING;
}

EP_STATUS USBHAL::endpointWriteResult(uint8_t endpoint)
{
    if (!(s_ep_compl & (1 << NU_EP2EPL(endpoint))))
        return EP_COMPLETED;
    else {
        if((HSUSBD_GET_EP_DATA_COUNT(NU_EP2EPH(endpoint))) == 0 && !(HSUSBD->DMACTL & HSUSBD_DMACTL_DMAEN_Msk)) {
            s_ep_compl &= ~(s_ep_compl & (1 << NU_EP2EPL(endpoint)));
            return EP_COMPLETED;
        }
    }
    return EP_PENDING;
}

void USBHAL::stallEndpoint(uint8_t endpoint)
{
    uint32_t ep_hw_index = NU_EP2EPH(endpoint);
    if (ep_hw_index >= NUMBER_OF_PHYSICAL_ENDPOINTS)
        return;
    HSUSBD_SetStall(ep_hw_index);
}

void USBHAL::unstallEndpoint(uint8_t endpoint)
{
    uint32_t ep_hw_index = NU_EP2EPH(endpoint);
    if (ep_hw_index >= NUMBER_OF_PHYSICAL_ENDPOINTS)
        return;
    HSUSBD_ClearStall(ep_hw_index);
}

bool USBHAL::getEndpointStallState(uint8_t endpoint)
{
    uint32_t ep_hw_index = NU_EP2EPH(endpoint);
    if (ep_hw_index >= NUMBER_OF_PHYSICAL_ENDPOINTS)
        return false;
    return HSUSBD_GetStall(ep_hw_index) ? 1 : 0;
}

void USBHAL::_usbisr(void)
{
    MBED_ASSERT(instance);
    instance->usbisr();
}

void USBHAL::usbisr(void)
{
    uint32_t gintsts = HSUSBD->GINTSTS & HSUSBD->GINTEN;
    if (! gintsts)
        return;

    if (gintsts & HSUSBD_GINTSTS_USBIF_Msk) {
        uint32_t busintsts = HSUSBD->BUSINTSTS & HSUSBD->BUSINTEN;

        /* SOF */
        if (busintsts & HSUSBD_BUSINTSTS_SOFIF_Msk) {
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_SOFIF_Msk);
            // TODO
            SOF(HSUSBD->FRAMECNT >> 3);
        }

        /* Reset */
        if (busintsts & HSUSBD_BUSINTSTS_RSTIF_Msk) {
            connect();
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_RSTIF_Msk);
            HSUSBD_CLR_CEP_INT_FLAG(0x1ffc);
        }

        /* Resume */
        if (busintsts & HSUSBD_BUSINTSTS_RESUMEIF_Msk) {
            HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk|HSUSBD_BUSINTEN_SUSPENDIEN_Msk | HSUSBD_BUSINTEN_SOFIEN_Msk | HSUSBD_BUSINTEN_SOFIEN_Msk);
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_RESUMEIF_Msk);
        }

        /* Suspend */
        if (busintsts & HSUSBD_BUSINTSTS_SUSPENDIF_Msk) {
            HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk | HSUSBD_BUSINTEN_RESUMEIEN_Msk |HSUSBD_BUSINTEN_SOFIEN_Msk);
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_SUSPENDIF_Msk);
        }

        /* High-speed */
        if (busintsts & HSUSBD_BUSINTSTS_HISPDIF_Msk) {
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk);
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_HISPDIF_Msk);
        }

        /* DMA */
        if (busintsts & HSUSBD_BUSINTSTS_DMADONEIF_Msk) {
            if(HSUSBD->DMACTL & 0x10) {	/* IN - Read */
                if(g_usbd_ShortPacket) {
                    uint32_t ep_hw_index = NU_EPL2EPH((HSUSBD->DMACTL & 0xF));
                    HSUSBD_SET_EP_SHORT_PACKET(ep_hw_index);
                    g_usbd_ShortPacket = 0;
                }
            }
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_DMADONEIF_Msk);
        }

        /* PHY clock available */
        if (busintsts & HSUSBD_BUSINTSTS_PHYCLKVLDIF_Msk) {
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_PHYCLKVLDIF_Msk);
        }

        /* VBUS plug-in */
        if (busintsts & HSUSBD_BUSINTSTS_VBUSDETIF_Msk) {
            if (HSUSBD_IS_ATTACHED()) {
                // USB plug-in
                HSUSBD_ENABLE_USB();
            } else {
                // USB unplug-out
                HSUSBD_DISABLE_USB();
            }
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_VBUSDETIF_Msk);
        }
    }

    /* CEP interrupts */
    if (gintsts & HSUSBD_GINTSTS_CEPIF_Msk) {
        uint32_t cepintsts =  HSUSBD->CEPINTSTS & HSUSBD->CEPINTEN;

        /* SETUP token packet */
        if (cepintsts & HSUSBD_CEPINTSTS_SETUPTKIF_Msk) {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_SETUPTKIF_Msk);
            return;
        }

        /* SETUP transaction */
        if (cepintsts & HSUSBD_CEPINTSTS_SETUPPKIF_Msk) {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_SETUPPKIF_Msk);
            EP0setupCallback();
            return;
        }

        /* OUT token packet */
        if (cepintsts & HSUSBD_CEPINTSTS_OUTTKIF_Msk) {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_OUTTKIF_Msk);
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
            return;
        }

        /* IN token packet */
        if (cepintsts & HSUSBD_CEPINTSTS_INTKIF_Msk) {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
            if (!(cepintsts & HSUSBD_CEPINTSTS_STSDONEIF_Msk)) {
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_TXPKIEN_Msk);
                HSUSBD_CtrlInput();
            } else {
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_TXPKIEN_Msk|HSUSBD_CEPINTEN_STSDONEIEN_Msk);
            }
            return;
        }

        /* PING packet */
        if (cepintsts & HSUSBD_CEPINTSTS_PINGIF_Msk) {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_PINGIF_Msk);
            return;
        }

        /* IN transaction */
        if (cepintsts & HSUSBD_CEPINTSTS_TXPKIF_Msk) {
            EP0in();
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
            return;
        }

        /* OUT transaction */
        if (cepintsts & HSUSBD_CEPINTSTS_RXPKIF_Msk) {
            EP0out();
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_RXPKIF_Msk);
            return;
        }

        /* NAK handshake packet */
        if (cepintsts & HSUSBD_CEPINTSTS_NAKIF_Msk) {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_NAKIF_Msk);
            return;
        }

        /* STALL handshake packet */
        if (cepintsts & HSUSBD_CEPINTSTS_STALLIF_Msk) {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STALLIF_Msk);
            return;
        }

        /* ERR special packet */
        if (cepintsts & HSUSBD_CEPINTSTS_ERRIF_Msk) {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_ERRIF_Msk);
            return;
        }

        /* Status stage transaction */
        if (cepintsts & HSUSBD_CEPINTSTS_STSDONEIF_Msk) {
            if (s_usb_addr) {
                HSUSBD_SET_ADDR(s_usb_addr);
                s_usb_addr = 0;
            }
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk);
            return;
        }

        /* Buffer Full */
        if (cepintsts & HSUSBD_CEPINTSTS_BUFFULLIF_Msk) {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_BUFFULLIF_Msk);
            return;
        }

        /* Buffer Empty */
        if (cepintsts & HSUSBD_CEPINTSTS_BUFEMPTYIF_Msk) {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_BUFEMPTYIF_Msk);
            return;
        }
    }
    /* EPA, EPB, EPC, ... EPL interrupts */
    uint32_t gintsts_epx = gintsts >> 2;
    uint32_t ep_hw_index = 0;
    while (gintsts_epx) {
        if(gintsts_epx & 0x01) {
            uint32_t epxintsts = HSUSBD_GET_EP_INT_FLAG(ep_hw_index) & HSUSBD_GET_EP_INT_EN(ep_hw_index);

            HSUSBD_CLR_EP_INT_FLAG(ep_hw_index, epxintsts);

            /* Buffer Full */
            if (epxintsts & HSUSBD_EPINTSTS_BUFFULLIF_Msk) {
            }

            /* Buffer Empty */
            if (epxintsts & HSUSBD_EPINTSTS_BUFEMPTYIF_Msk) {
            }

            /* Short Packet Transferred */
            if (epxintsts & HSUSBD_EPINTSTS_SHORTTXIF_Msk) {
            }

            /* Data Packet Transmitted */
            if (epxintsts & HSUSBD_EPINTSTS_TXPKIF_Msk) {
                s_ep_compl &= ~(1 << (NU_EPH2EPL(ep_hw_index)));
                if ((instance->*(epCallback[ep_hw_index]))()) {
                }
            }

            /* Data Packet Received */
            if (epxintsts & HSUSBD_EPINTSTS_RXPKIF_Msk) {
                if ((instance->*(epCallback[ep_hw_index]))()) {

                }
            }

            /* OUT token packet */
            if (epxintsts & HSUSBD_EPINTSTS_OUTTKIF_Msk) {
            }

            /* IN token packet */
            if (epxintsts & HSUSBD_EPINTSTS_INTKIF_Msk) {
            }

            /* PING packet */
            if (epxintsts & HSUSBD_EPINTSTS_PINGIF_Msk) {
            }

            /* NAK handshake packet sent to Host */
            if (epxintsts & HSUSBD_EPINTSTS_NAKIF_Msk) {
            }

            /* STALL handshake packet sent to Host */
            if (epxintsts & HSUSBD_EPINTSTS_STALLIF_Msk) {
            }

            /* NYET handshake packet sent to Host */
            if (epxintsts & HSUSBD_EPINTSTS_NYETIF_Msk) {
            }

            /* ERR packet sent to Host */
            if (epxintsts & HSUSBD_EPINTSTS_ERRIF_Msk) {
            }

            /* Bulk Out Short Packet Received */
            if (epxintsts & HSUSBD_EPINTSTS_SHORTRXIF_Msk) {
            }
        }
        gintsts_epx = gintsts_epx >> 1;
        ep_hw_index++;
    }
}
#endif

#endif

