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

#if defined(TARGET_NUC472)

#include "USBHAL.h"
#include "NUC472_442.h"
#include "pinmap.h"

/**
 * EP: mbed USBD defined endpoint, e.g. EP0OUT/IN, EP1OUT/IN, EP2OUT/IN.
 * EPX: BSP defined endpoint, e.g. CEP, EPA, EPB, EPC.
 */

USBHAL * USBHAL::instance;

static volatile uint32_t s_ep_compl = 0;
static volatile uint32_t s_ep_buf_ind = 0;
static volatile uint8_t s_usb_addr = 0;
static volatile S_USBD_CMD_T s_setup;
static volatile uint16_t s_ctrlin_packetsize;
static uint8_t *g_usbd_CtrlInPointer = 0;
static uint32_t g_usbd_CtrlMaxPktSize = 64;
static uint32_t g_usbd_ShortPkt = 0;
static uint32_t gEpRead = 0;
static uint32_t gEpReadCnt = 0;

void USBD_CtrlInput(void)
{
    int volatile i;
    uint32_t volatile count;

    // Process remained data
    if (g_usbd_CtrlInSize >= g_usbd_CtrlMaxPktSize)
    {
        // Data size > MXPLD
        for (i=0; i<(g_usbd_CtrlMaxPktSize >> 2); i++, g_usbd_CtrlInPointer+=4)
            USBD->CEPDAT = *(uint32_t *)g_usbd_CtrlInPointer;
        USBD_START_CEP_IN(g_usbd_CtrlMaxPktSize);
        g_usbd_CtrlInSize -= g_usbd_CtrlMaxPktSize;
    }
    else
    {
        // Data size <= MXPLD
        for (i=0; i<(g_usbd_CtrlInSize >> 2); i++, g_usbd_CtrlInPointer+=4)
            USBD->CEPDAT = *(uint32_t *)g_usbd_CtrlInPointer;

        count = g_usbd_CtrlInSize % 4;
        for (i=0; i<count; i++)
            USBD->CEPDAT_BYTE = *(uint8_t *)(g_usbd_CtrlInPointer + i);

        USBD_START_CEP_IN(g_usbd_CtrlInSize);
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

    /* Enable USBD module clock */
    CLK_EnableModuleClock(USBD_MODULE);

    /* Enable USB PHY's LDO33. Run as USB device. */
    SYS->USBPHY = SYS_USBPHY_USBROLE_OTG_V33_EN | SYS_USBPHY_USBROLE_STD_USBD;

    /* Enable USB PHY and wait for it ready */
    USBD_ENABLE_PHY();
    while (1)
    {
        USBD->EPAMPS = 0x20;
        if (USBD->EPAMPS == 0x20)
            break;
    }

    /* Force to full-speed */
    USBD->OPER = 0;//USBD_OPER_HISPDEN_Msk;

    /* Set SE0 (disconnect) */
    USBD_SET_SE0();

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
    USBD_ResetDMA();
    USBD_SET_ADDR(0);

    /**
      * Control Transfer Packet Size Constraints
      * low-speed: 8
      * full-speed: 8, 16, 32, 64
      * high-speed: 64
      */
    /* Control endpoint */
    USBD_SetEpBufAddr(CEP, s_ep_buf_ind, MAX_PACKET_SIZE_EP0);
    s_ep_buf_ind = MAX_PACKET_SIZE_EP0;

    /* Enable USB/CEP interrupt */
    USBD_ENABLE_USB_INT(USBD_GINTEN_USBIE_Msk | USBD_GINTEN_CEPIE_Msk);
    USBD_ENABLE_CEP_INT(USBD_CEPINTEN_SETUPPKIEN_Msk|USBD_CEPINTEN_STSDONEIEN_Msk);

    /* Enable BUS interrupt */
    USBD_ENABLE_BUS_INT(
        USBD_BUSINTEN_DMADONEIEN_Msk |
        USBD_BUSINTEN_RESUMEIEN_Msk |
        USBD_BUSINTEN_RSTIEN_Msk |
        USBD_BUSINTEN_VBUSDETIEN_Msk |
        USBD_BUSINTEN_SOFIEN_Msk
    );

    /* Clear SE0 (connect) */
    USBD_CLR_SE0();
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
    s_ep_buf_ind = MAX_PACKET_SIZE_EP0;
}

void USBHAL::setAddress(uint8_t address)
{
    // NOTE: Delay address setting; otherwise, USB controller won't ack.
    s_usb_addr = address;
}

void USBHAL::remoteWakeup(void)
{
    USBD->OPER |= USBD_OPER_RESUMEEN_Msk;
}

bool USBHAL::realiseEndpoint(uint8_t endpoint, uint32_t maxPacket, uint32_t options)
{
    uint32_t ep_type;
    uint32_t ep_hw_index = NU_EP2EPH(endpoint);

    USBD_SetEpBufAddr(ep_hw_index, s_ep_buf_ind, maxPacket);
    s_ep_buf_ind += maxPacket;
    USBD_SET_MAX_PAYLOAD(ep_hw_index, maxPacket);

    switch (NU_EP2EPL(endpoint))
    {
    case 1:    case 2:
        ep_type = USB_EP_CFG_TYPE_INT;
        break;

    case 3:    case 4:
        ep_type = USB_EP_CFG_TYPE_ISO;
        break;

    default:
        ep_type = USB_EP_CFG_TYPE_BULK;
    }
    uint32_t ep_dir = (NU_EP_DIR(endpoint) == NU_EP_DIR_IN) ? USB_EP_CFG_DIR_IN : USB_EP_CFG_DIR_OUT;
    USBD_ConfigEp(ep_hw_index, NU_EP2EPL(endpoint), ep_type, ep_dir);

    /* Enable USB/EPX interrupt */
    // NOTE: Require USBD_GINTEN_EPAIE_Pos, USBD_GINTEN_EPBIE_Pos, ... USBD_GINTEN_EPLIE_Pos to be consecutive.
    USBD_ENABLE_USB_INT(USBD->GINTEN | USBD_GINTEN_USBIE_Msk |
                        USBD_GINTEN_CEPIE_Msk |
                        1 << (ep_hw_index + USBD_GINTEN_EPAIE_Pos));  // Added USB/EPX interrupt

    if (ep_dir == 0)
        USBD_ENABLE_EP_INT(ep_hw_index, USBD_EPINTEN_RXPKIEN_Msk);
    else
        USBD_ENABLE_EP_INT(ep_hw_index, USBD_EPINTEN_TXPKIEN_Msk);
    return true;
}

void USBHAL::EP0setup(uint8_t *buffer)
{
    uint32_t sz;
    endpointReadResult(EP0OUT, buffer, &sz);
}

void USBHAL::EP0read(void)
{
    if (s_setup.wLength && ! (s_setup.bmRequestType & 0x80))
    {
        // Control OUT
        USBD_ENABLE_CEP_INT(USBD_CEPINTEN_SETUPPKIEN_Msk | USBD_CEPINTEN_RXPKIEN_Msk);
    }
    else
    {
        // Status stage
        USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_STSDONEIF_Msk);
        USBD_SET_CEP_STATE(USB_CEPCTL_NAKCLR);
        USBD_ENABLE_CEP_INT(USBD_CEPINTEN_STSDONEIEN_Msk);
    }
}

void USBHAL::EP0readStage(void)
{
    // N/A
}

uint32_t USBHAL::EP0getReadResult(uint8_t *buffer)
{
    uint32_t i;
    uint32_t ceprxcnt = USBD->CEPRXCNT;
    for (i = 0; i < ceprxcnt; i ++)
        *buffer ++ = USBD->CEPDAT_BYTE;
    return ceprxcnt;
}

void USBHAL::EP0write(uint8_t *buffer, uint32_t size)
{
    if (buffer && size)
    {
        g_usbd_CtrlInPointer = buffer;
        g_usbd_CtrlInSize = size;
        USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_INTKIF_Msk);
        USBD_ENABLE_CEP_INT(USBD_CEPINTEN_INTKIEN_Msk);
    }
    else
    {
        /* Status stage */
        s_ctrlin_packetsize = 0;
        USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_STSDONEIF_Msk);
        USBD_SET_CEP_STATE(USB_CEPCTL_NAKCLR);
        USBD_ENABLE_CEP_INT(USBD_CEPINTEN_STSDONEIEN_Msk);
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
    if (endpoint == EP0OUT)
    {
        if (buffer) {
            *((uint16_t *) (buffer + 0)) = (uint16_t) USBD->SETUP1_0;
            *((uint16_t *) (buffer + 2)) = (uint16_t) USBD->SETUP3_2;
            *((uint16_t *) (buffer + 4)) = (uint16_t) USBD->SETUP5_4;
            *((uint16_t *) (buffer + 6)) = (uint16_t) USBD->SETUP7_6;
        }

        s_setup.bmRequestType = (uint8_t) (USBD->SETUP1_0 & 0xff);
        s_setup.bRequest = (int8_t) (USBD->SETUP1_0 >> 8) & 0xff;
        s_setup.wValue = (uint16_t) USBD->SETUP3_2;
        s_setup.wIndex = (uint16_t) USBD->SETUP5_4;
        s_setup.wLength = (uint16_t) USBD->SETUP7_6;
    }
    else
    {
        if (!(s_ep_compl & (1 << NU_EP2EPL(endpoint))))
        {
            while (1)
            {
                if (!(USBD->DMACTL & USBD_DMACTL_DMAEN_Msk))
                    break;
                else
                    if (!USBD_IS_ATTACHED())
                        break;
            }
            gEpReadCnt = USBD_GET_EP_DATA_COUNT(NU_EP2EPH(endpoint));
            if (gEpReadCnt == 0)
            {
                *bytesRead = 0;
                return EP_COMPLETED;
            }
            s_ep_compl |= (1 << NU_EP2EPL(endpoint));
            USBD_SET_DMA_LEN(gEpReadCnt);
            USBD_SET_DMA_ADDR((uint32_t)buffer);
            USBD_SET_DMA_WRITE(NU_EP2EPL(endpoint));
            USBD_ENABLE_DMA();
            return EP_PENDING;;

        }
        else
        {
            if ((USBD->DMACTL & USBD_DMACTL_DMAEN_Msk))
                return EP_PENDING;;

            USBD_CLR_BUS_INT_FLAG(USBD_BUSINTSTS_DMADONEIF_Msk);
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
    if (ep_logic_index == 0)
        return EP_INVALID;
    else
    {
        uint32_t ep_hw_index = NU_EP2EPH(endpoint);
        uint32_t mps = USBD_GET_EP_MAX_PAYLOAD(ep_hw_index);
        if (size > mps) {
            return EP_INVALID;
        }
        if (size < mps)
            g_usbd_ShortPkt = 1;
        if (!(s_ep_compl & (1 << NU_EP2EPL(endpoint))))
        {
            s_ep_compl |= (1 << ep_logic_index);

            while (1)
            {
                if (!(USBD->DMACTL & USBD_DMACTL_DMAEN_Msk))
                    break;
                else
                    if (!USBD_IS_ATTACHED())
                        break;
            }
            USBD_SET_DMA_LEN(size);
            USBD_SET_DMA_ADDR((uint32_t)data);
            USBD_SET_DMA_READ(ep_logic_index);
            USBD_ENABLE_DMA();
        }
    }
    return EP_PENDING;
}

EP_STATUS USBHAL::endpointWriteResult(uint8_t endpoint)
{
    if (!(s_ep_compl & (1 << NU_EP2EPL(endpoint))))
        return EP_COMPLETED;
    else
    {
        if ((USBD_GET_EP_DATA_COUNT(NU_EP2EPH(endpoint))) == 0 && !(USBD->DMACTL & USBD_DMACTL_DMAEN_Msk))
        {
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
    USBD_SetStall(ep_hw_index);
}

void USBHAL::unstallEndpoint(uint8_t endpoint)
{
    uint32_t ep_hw_index = NU_EP2EPH(endpoint);
    if (ep_hw_index >= NUMBER_OF_PHYSICAL_ENDPOINTS)
        return;
    USBD_ClearStall(ep_hw_index);
}

bool USBHAL::getEndpointStallState(uint8_t endpoint)
{
    uint32_t ep_hw_index = NU_EP2EPH(endpoint);
    if (ep_hw_index >= NUMBER_OF_PHYSICAL_ENDPOINTS)
        return false;
    return USBD_GetStall(ep_hw_index) ? 1 : 0;
}

void USBHAL::_usbisr(void)
{
    MBED_ASSERT(instance);
    instance->usbisr();
}

void USBHAL::usbisr(void)
{
    uint32_t gintsts = USBD->GINTSTS & USBD->GINTEN;
    if (! gintsts)
        return;

    if (gintsts & USBD_GINTSTS_USBIF_Msk)
    {
        uint32_t busintsts = USBD->BUSINTSTS & USBD->BUSINTEN;

        /* SOF */
        if (busintsts & USBD_BUSINTSTS_SOFIF_Msk)
        {
            USBD_CLR_BUS_INT_FLAG(USBD_BUSINTSTS_SOFIF_Msk);
            // TODO
            SOF(USBD->FRAMECNT >> 3);
        }

        /* Reset */
        if (busintsts & USBD_BUSINTSTS_RSTIF_Msk)
        {
            connect();
            USBD_CLR_BUS_INT_FLAG(USBD_BUSINTSTS_RSTIF_Msk);
            USBD_CLR_CEP_INT_FLAG(0x1ffc);
        }

        /* Resume */
        if (busintsts & USBD_BUSINTSTS_RESUMEIF_Msk)
        {
            USBD_ENABLE_BUS_INT(USBD_BUSINTEN_RSTIEN_Msk|USBD_BUSINTEN_SUSPENDIEN_Msk | USBD_BUSINTEN_SOFIEN_Msk | USBD_BUSINTEN_SOFIEN_Msk);
            USBD_CLR_BUS_INT_FLAG(USBD_BUSINTSTS_RESUMEIF_Msk);
        }

        /* Suspend */
        if (busintsts & USBD_BUSINTSTS_SUSPENDIF_Msk)
        {
            USBD_ENABLE_BUS_INT(USBD_BUSINTEN_RSTIEN_Msk | USBD_BUSINTEN_RESUMEIEN_Msk |USBD_BUSINTEN_SOFIEN_Msk);
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
            if (USBD->DMACTL & 0x10) /* IN - Read */
            {
                if (g_usbd_ShortPkt)
                {
                    uint32_t ep_hw_index = NU_EPL2EPH((USBD->DMACTL & 0xF));
                    USBD_SET_EP_SHORT_PACKET(ep_hw_index);
                    g_usbd_ShortPkt = 0;
                }
            }
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
                // USB plug-in
                USBD_ENABLE_USB();
            }
            else
            {
                // USB unplug-out
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
            EP0setupCallback();
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
                USBD_ENABLE_CEP_INT(USBD_CEPINTEN_TXPKIEN_Msk);
                USBD_CtrlInput();
            }
            else
            {
                USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_TXPKIF_Msk);
                USBD_ENABLE_CEP_INT(USBD_CEPINTEN_TXPKIEN_Msk|USBD_CEPINTEN_STSDONEIEN_Msk);
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
            EP0in();
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_TXPKIF_Msk);
            return;
        }

        /* OUT transaction */
        if (cepintsts & USBD_CEPINTSTS_RXPKIF_Msk)
        {
            EP0out();
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
    /* EPA, EPB, EPC, ... EPL interrupts */
    uint32_t gintsts_epx = gintsts >> 2;
    uint32_t ep_hw_index = 0;
    while (gintsts_epx) {
        if (gintsts_epx & 0x01)
        {
            uint32_t epxintsts = USBD_GET_EP_INT_FLAG(ep_hw_index) & USBD_GET_EP_INT_EN(ep_hw_index);

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
                s_ep_compl &= ~(1 << (NU_EPH2EPL(ep_hw_index)));
                if ((instance->*(epCallback[ep_hw_index]))())
                {
                }
            }

            /* Data Packet Received */
            if (epxintsts & USBD_EPINTSTS_RXPKIF_Msk)
            {
                if ((instance->*(epCallback[ep_hw_index]))())
                {

                }
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
}
#endif

