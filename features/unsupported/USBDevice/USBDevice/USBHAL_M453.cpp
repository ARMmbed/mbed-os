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

#if defined(TARGET_M451)

#include "USBHAL.h"
#include "M451Series.h"
#include "pinmap.h"

/**
 * EP: mbed USBD defined endpoint, e.g. EP0OUT/IN, EP1OUT/IN, EP2OUT/IN.
 * EPX: BSP defined endpoint, e.g. CEP, EPA, EPB, EPC.
 */

USBHAL * USBHAL::instance;

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

    instance = this;
    /* Enable USBD module clock */
    CLK_EnableModuleClock(USBD_MODULE);

    CLK_SetModuleClock(USBD_MODULE, 0, CLK_CLKDIV0_USB(3));

    /* Enable USB LDO33 */
    SYS->USBPHY = SYS_USBPHY_LDO33EN_Msk;

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

    if (ep_logic_index == 3 || ep_logic_index == 4)
        ep_type = USBD_CFG_TYPE_ISO;

    USBD_CONFIG_EP(ep_hw_index, ep_dir | ep_type | ep_logic_index);
    /* Buffer range */
    USBD_SET_EP_BUF_ADDR(ep_hw_index, s_ep_buf_ind);

    if (ep_dir == USBD_CFG_EPMODE_OUT)
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
    if (buffer && size)
    {
        if (s_ep_data_bit[0] & 1)
            USBD_SET_DATA1(EP0);
        else
            USBD_SET_DATA0(EP0);
        s_ep_data_bit[0]++;

        USBD_MemCopy((uint8_t *)USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0), buffer, size);
        USBD_SET_PAYLOAD_LEN(EP0, size);
        if (size < MAX_PACKET_SIZE_EP0)
            s_ep_data_bit[0] = 1;

    }
    else
    {
        if (g_usbd_SetupPacket[0] & 0x80)   //Device to Host
        {
            // Status stage
            //          USBD_PrepareCtrlOut(0,0);
        } else
        {
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
    if (endpoint == EP0OUT)
    {
        USBD_MemCopy(g_usbd_SetupPacket, (uint8_t *)USBD_BUF_BASE, 8);
        if (buffer) {
            USBD_MemCopy(buffer, g_usbd_SetupPacket, 8);
        }
        USBD_SET_PAYLOAD_LEN(EP1, MAX_PACKET_SIZE_EP0);
    }
    else
    {
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
    if (ep_logic_index == 0)
        return EP_INVALID;
    else
    {
        uint8_t *buf;
        uint32_t i=0;
        uint32_t ep_hw_index = NU_EP2EPH(endpoint);
        s_ep_compl |= (1 << ep_logic_index);
        buf = (uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(ep_hw_index));
        for (i=0;i<size;i++)
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
    if (u32IntSts & USBD_INTSTS_VBDETIF_Msk)
    {
        // Floating detect
        USBD_CLR_INT_FLAG(USBD_INTSTS_VBDETIF_Msk);

        if (USBD_IS_ATTACHED())
        {
            /* USB Plug In */
            USBD_ENABLE_USB();
        }
        else
        {
            /* USB Un-plug */
            USBD_DISABLE_USB();
        }
    }

//------------------------------------------------------------------
    if (u32IntSts & USBD_INTSTS_BUSIF_Msk)
    {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_BUSIF_Msk);

        if (u32State & USBD_ATTR_USBRST_Msk)
        {
            /* Bus reset */
            USBD_ENABLE_USB();
            USBD_SwReset();
        }
        if (u32State & USBD_ATTR_SUSPEND_Msk)
        {
            /* Enable USB but disable PHY */
            USBD_DISABLE_PHY();
        }
        if (u32State & USBD_ATTR_RESUME_Msk)
        {
            /* Enable USB and enable PHY */
            USBD_ENABLE_USB();
        }
    }

    if (u32IntSts & USBD_INTSTS_USBIF_Msk)
    {
        // USB event
        if (u32IntSts & USBD_INTSTS_SETUP_Msk)
        {
            // Setup packet
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_SETUP_Msk);

            /* Clear the data IN/OUT ready flag of control end-points */
            USBD_STOP_TRANSACTION(EP0);
            USBD_STOP_TRANSACTION(EP1);
            EP0setupCallback();
        }

        // EP events
        if (u32IntSts & USBD_INTSTS_EP0)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP0);
            // control IN
            EP0in();

            // In ACK for Set address
            if ((g_usbd_SetupPacket[0] == REQ_STANDARD) && (g_usbd_SetupPacket[1] == USBD_SET_ADDRESS))
            {
                if ((USBD_GET_ADDR() != s_usb_addr) && (USBD_GET_ADDR() == 0))
                {
                    USBD_SET_ADDR(s_usb_addr);
                }
            }
        }
        if (u32IntSts & USBD_INTSTS_EP1)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP1);

            // control OUT
            EP0out();
        }

        uint32_t gintsts_epx = (u32IntSts >> 18) & 0x3F;
        uint32_t ep_hw_index = 2;
        while (gintsts_epx) {
            if (gintsts_epx & 0x01)
            {
                uint32_t ep_status = (USBD_GET_EP_FLAG() >> (ep_hw_index * 3 + 8)) & 0x7;
                /* Clear event flag */
                USBD_CLR_INT_FLAG(1 << (ep_hw_index + 16));

                if (ep_status == 0x02 || ep_status == 0x06 || (ep_status == 0x07 && NU_EPH2EPL(ep_hw_index) == 3))  //RX
                {
                    if (ep_status == 0x07)
                        SOF(frame_cnt++);
                    if ((instance->*(epCallback[ep_hw_index-2]))())
                    {

                    }
                    USBD_SET_PAYLOAD_LEN(ep_hw_index,s_ep_mxp[NU_EPH2EPL(ep_hw_index)]);
                }
                else if (ep_status == 0x00 || ep_status == 0x07)    //TX
                {
                    s_ep_compl &= ~(1 << (NU_EPH2EPL(ep_hw_index)));
                    if ((instance->*(epCallback[ep_hw_index-2]))())
                    {
                    }
                }
            }

            gintsts_epx = gintsts_epx >> 1;
            ep_hw_index++;
        }
    }
}
#endif

