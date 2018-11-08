/* Copyright (c) 2017 mbed.org, MIT License
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
#ifndef USBHAL_IP_DEVICE_H
#define USBHAL_IP_DEVICE_H

#if defined(TARGET_NUCLEO_F303ZE)
#define USBHAL_IRQn  USB_LP_CAN_RX0_IRQn

#elif defined(TARGET_NUCLEO_F103RB)
#define USBHAL_IRQn  USB_LP_CAN1_RX0_IRQn

#elif defined(TARGET_DISCO_L072CZ_LRWAN1) || \
      defined(TARGET_DISCO_L053C8)
#define USBHAL_IRQn  USB_IRQn

#else
#error "USB IRQ is not configured !"
#endif

#define NB_ENDPOINT  8 // Must be a multiple of 4 bytes

#define MAXTRANSFER_SIZE  0x200

#define FIFO_USB_RAM_SIZE (MAXTRANSFER_SIZE + MAX_PACKET_SIZE_EP0 + MAX_PACKET_SIZE_EP1 + MAX_PACKET_SIZE_EP2 + MAX_PACKET_SIZE_EP3)

#if (FIFO_USB_RAM_SIZE > 0x500)
#error "FIFO dimensioning incorrect"
#endif

typedef struct {
    USBHAL *inst;

    void (USBHAL::*bus_reset)(void);
    void (USBHAL::*sof)(int frame);
    void (USBHAL::*connect_change)(unsigned int connected);
    void (USBHAL::*suspend_change)(unsigned int suspended);
    void (USBHAL::*ep0_setup)(void);
    void (USBHAL::*ep0_in)(void);
    void (USBHAL::*ep0_out)(void);
    void (USBHAL::*ep0_read)(void);
    bool (USBHAL::*ep_realise)(uint8_t endpoint, uint32_t maxPacket, uint32_t flags);
    bool (USBHAL::*epCallback[(2 * NB_ENDPOINT) - 2])(void);

    uint8_t epComplete[2 * NB_ENDPOINT];

    /* Memorize dummy buffer used for reception */
    uint32_t pBufRx[MAXTRANSFER_SIZE >> 2];
    uint32_t pBufRx0[MAX_PACKET_SIZE_EP0 >> 2];

    gpio_t usb_switch;
} USBHAL_Private_t;

uint32_t HAL_PCDEx_GetTxFiFo(PCD_HandleTypeDef *hpcd, uint8_t fifo)
{
    return 1024;
}

void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
    USBHAL_Private_t *priv = ((USBHAL_Private_t *)(hpcd->pData));
    USBHAL *obj = priv->inst;
    uint32_t sofnum = (hpcd->Instance->FNR) & USB_FNR_FN;
    void (USBHAL::*func)(int frame) = priv->sof;
    (obj->*func)(sofnum);
}

void HAL_PCDEx_SetConnectionState(PCD_HandleTypeDef *hpcd, uint8_t state)
{
    USBHAL_Private_t *priv = ((USBHAL_Private_t *)(hpcd->pData));
#if defined(TARGET_NUCLEO_F103RB)
    gpio_write(&(priv->usb_switch), !state);
#else
    gpio_write(&(priv->usb_switch), state);
#endif
}

USBHAL *USBHAL::instance;

USBHAL::USBHAL(void)
{
    USBHAL_Private_t *HALPriv = new (USBHAL_Private_t);

    hpcd.Instance = USB;

    memset(&hpcd.Init, 0, sizeof(hpcd.Init));
    hpcd.Init.dev_endpoints = NB_ENDPOINT;
    hpcd.Init.ep0_mps = MAX_PACKET_SIZE_EP0;
    hpcd.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd.Init.Sof_enable = 1;
    hpcd.Init.speed = PCD_SPEED_FULL;

    // Pass instance for usage inside call back
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
    HALPriv->epCallback[0] = &USBHAL::EP1_OUT_callback;
    HALPriv->epCallback[1] = &USBHAL::EP1_IN_callback;
    HALPriv->epCallback[2] = &USBHAL::EP2_OUT_callback;
    HALPriv->epCallback[3] = &USBHAL::EP2_IN_callback;
    HALPriv->epCallback[4] = &USBHAL::EP3_OUT_callback;
    HALPriv->epCallback[5] = &USBHAL::EP3_IN_callback;
    instance = this;

    // Configure USB pins and other clocks
#if defined(TARGET_NUCLEO_F303ZE)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF14_USB)); // DM
    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF14_USB)); // DP
    __HAL_RCC_GPIOG_CLK_ENABLE();
    gpio_init_out(&HALPriv->usb_switch, PG_6);

#elif defined(TARGET_NUCLEO_F103RB)
    // Make sure to connect a 1.5K resistor between USB-DP PA12 pin and +3.3V
    __HAL_RCC_GPIOA_CLK_ENABLE();
    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_MODE_AF_INPUT)); // DM
    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_MODE_AF_INPUT)); // DP

#elif defined(TARGET_DISCO_L072CZ_LRWAN1) || \
      defined(TARGET_DISCO_L053C8)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_USB)); // DM
    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_USB)); // DP

#else
#error "USB pins are not configured !"
#endif

    __HAL_RCC_USB_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    // Configure PCD and FIFOs
    hpcd.pData = (void *)HALPriv;
    hpcd.State = HAL_PCD_STATE_RESET;
    HAL_PCD_Init(&hpcd);

    // EP0
    HAL_PCDEx_PMAConfig(&hpcd, 0x00, PCD_SNG_BUF, 0x30);
    HAL_PCDEx_PMAConfig(&hpcd, 0x80, PCD_SNG_BUF, 0x70);
    // EP1
    HAL_PCDEx_PMAConfig(&hpcd, 0x01, PCD_SNG_BUF, 0x90);
    HAL_PCDEx_PMAConfig(&hpcd, 0x81, PCD_SNG_BUF, 0xb0);
    // EP2
    HAL_PCDEx_PMAConfig(&hpcd, 0x02, PCD_SNG_BUF, 0x100);
    HAL_PCDEx_PMAConfig(&hpcd, 0x82, PCD_SNG_BUF, 0x120);
    // EP3
    HAL_PCDEx_PMAConfig(&hpcd, 0x03, PCD_DBL_BUF, 0x018000b0);
    HAL_PCDEx_PMAConfig(&hpcd, 0x83, PCD_SNG_BUF, 0xb0);

    // Configure interrupt vector
    NVIC_SetVector(USBHAL_IRQn, (uint32_t)&_usbisr);
    NVIC_SetPriority(USBHAL_IRQn, 1);

    HAL_PCD_Start(&hpcd);
}

#endif // USBHAL_IP_DEVICE_H
