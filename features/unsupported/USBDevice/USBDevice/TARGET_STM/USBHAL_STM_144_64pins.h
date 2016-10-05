/* Copyright (c) 2016 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef USBHAL_STM32_144_64
#define USBHAL_STM32_144_64

#define USBHAL_IRQn  OTG_FS_IRQn
/*  must be multiple of 4 bytes */
#define NB_ENDPOINT 4
#define MAXTRANSFER_SIZE  0x200
#define FIFO_USB_RAM_SIZE (MAXTRANSFER_SIZE+MAX_PACKET_SIZE_EP0+MAX_PACKET_SIZE_EP1+MAX_PACKET_SIZE_EP2+MAX_PACKET_SIZE_EP3)
#if (FIFO_USB_RAM_SIZE > 0x500)
#error "FIFO dimensioning incorrect"
#endif

typedef struct
{
    USBHAL *inst;
    void (USBHAL::*bus_reset)(void);
    void (USBHAL::*sof)(int frame);
    void (USBHAL::*connect_change)(unsigned int  connected);
    void (USBHAL::*suspend_change)(unsigned int suspended);
    void (USBHAL::*ep0_setup)(void);
    void (USBHAL::*ep0_in)(void);
    void (USBHAL::*ep0_out)(void);
    void (USBHAL::*ep0_read)(void);
    bool (USBHAL::*ep_realise)(uint8_t endpoint, uint32_t maxPacket, uint32_t flags);
    bool (USBHAL::*epCallback[2*NB_ENDPOINT-2])(void);
    /*  memorize dummy buffer used for reception */
    uint32_t pBufRx[MAXTRANSFER_SIZE>>2];
    uint32_t pBufRx0[MAX_PACKET_SIZE_EP0>>2];
    uint8_t epComplete[2*NB_ENDPOINT];
}USBHAL_Private_t;

uint32_t HAL_PCDEx_GetTxFiFo(PCD_HandleTypeDef *hpcd, uint8_t fifo)
{
    uint32_t len;
    if (fifo == 0) len = hpcd->Instance->DIEPTXF0_HNPTXFSIZ>>16;
    else
        len =  hpcd->Instance->DIEPTXF[fifo - 1] >> 16;
    return len*4;
}
void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd) 
{
    USBHAL_Private_t *priv=((USBHAL_Private_t *)(hpcd->pData));
    USBHAL *obj= priv->inst;
    USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;
    uint32_t sofnum = (USBx_DEVICE->DSTS & USB_OTG_DSTS_FNSOF) >> 8;
    void (USBHAL::*func)(int frame) = priv->sof;
    /* fix me  call with same frame number */
    (obj->*func)(sofnum);
}


USBHAL * USBHAL::instance;

USBHAL::USBHAL(void) {
    /*  init parameter  */
    USBHAL_Private_t *HALPriv = new(USBHAL_Private_t);
    hpcd.Instance = USB_OTG_FS;
    memset(&hpcd.Init, 0, sizeof(hpcd.Init));
    hpcd.Init.dev_endpoints = NB_ENDPOINT;
    hpcd.Init.ep0_mps =   MAX_PACKET_SIZE_EP0;
    hpcd.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd.Init.Sof_enable = 1;

    hpcd.Init.speed = PCD_SPEED_FULL;
    //hpcd.Init.vbus_sensing_enable = 0;
    //hpcd.Init.lpm_enable = 0;
    /*  pass instance for usage inside call back */
    HALPriv->inst = this;
    HALPriv->bus_reset = &USBHAL::busReset;
    HALPriv->suspend_change = &USBHAL::suspendStateChanged;
    HALPriv->connect_change = &USBHAL::connectStateChanged;
    HALPriv->sof = &USBHAL::SOF;
    HALPriv->ep0_setup = &USBHAL::EP0setupCallback;
    HALPriv->ep_realise = &USBHAL::realiseEndpoint;
    HALPriv->ep0_in = &USBHAL::EP0in;
    HALPriv->ep0_out = &USBHAL::EP0out;
    HALPriv->ep0_read = &USBHAL::EP0read;
    hpcd.pData = (void*)HALPriv;
    HALPriv->epCallback[0] = &USBHAL::EP1_OUT_callback;
    HALPriv->epCallback[1] = &USBHAL::EP1_IN_callback;
    HALPriv->epCallback[2] = &USBHAL::EP2_OUT_callback;
    HALPriv->epCallback[3] = &USBHAL::EP2_IN_callback;
    HALPriv->epCallback[4] = &USBHAL::EP3_OUT_callback;
    HALPriv->epCallback[5] = &USBHAL::EP3_IN_callback;
    instance = this;
    // Enable power and clocking
    /*  board 144 pin all similar  */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    pin_function(PA_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS));
    pin_function(PA_9, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, GPIO_AF10_OTG_FS));
    pin_function(PA_10, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_PULLUP, GPIO_AF10_OTG_FS));
    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS));
    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS));

    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    hpcd.State = HAL_PCD_STATE_RESET;
    HAL_PCD_Init(&hpcd);
    /* 1.25kbytes */
    /* min value 16 (= 16 x 4 bytes) */
    /*  max value 256 (= 1K bytes ) */
    /*  maximum sum is 0x140 */
    HAL_PCDEx_SetRxFiFo(&hpcd, (MAXTRANSFER_SIZE/4));
    /*  bulk/int 64 bytes in FS  */
    HAL_PCDEx_SetTxFiFo(&hpcd, 0, (MAX_PACKET_SIZE_EP0/4)+1);
    /*  bulk/int bytes in FS */
    HAL_PCDEx_SetTxFiFo(&hpcd, 1, (MAX_PACKET_SIZE_EP1/4));
    HAL_PCDEx_SetTxFiFo(&hpcd, 2, (MAX_PACKET_SIZE_EP2/4));
    /* ISOchronous */
    HAL_PCDEx_SetTxFiFo(&hpcd, 3, (MAX_PACKET_SIZE_EP3/4));
    NVIC_SetVector(USBHAL_IRQn, (uint32_t)&_usbisr);
    NVIC_SetPriority(USBHAL_IRQn, 1);
    HAL_PCD_Start(&hpcd);
}
#endif

