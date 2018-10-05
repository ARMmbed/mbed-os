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
#ifndef USBHAL_IP_OTGFSHS_H
#define USBHAL_IP_OTGFSHS_H

#if defined(TARGET_DISCO_F746NG)
#if (MBED_CONF_TARGET_USB_SPEED == 1) // Defined in json configuration file
#define TARGET_DISCO_F746NG_OTG_HS
#else
#define TARGET_DISCO_F746NG_OTG_FS
#endif
#endif

#if defined(TARGET_DISCO_F429ZI) || \
    defined(TARGET_DISCO_F769NI) || \
    defined(TARGET_DISCO_F746NG_OTG_HS)
#define USBHAL_IRQn  OTG_HS_IRQn
#else
#define USBHAL_IRQn  OTG_FS_IRQn
#endif

#define NB_ENDPOINT  4 // Must be a multiple of 4 bytes

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
} USBHAL_Private_t;

uint32_t HAL_PCDEx_GetTxFiFo(PCD_HandleTypeDef *hpcd, uint8_t fifo)
{
    uint32_t len;
    if (fifo == 0) {
        len = hpcd->Instance->DIEPTXF0_HNPTXFSIZ >> 16;
    } else {
        len =  hpcd->Instance->DIEPTXF[fifo - 1] >> 16;
    }
    return len * 4;
}

void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
    USBHAL_Private_t *priv = ((USBHAL_Private_t *)(hpcd->pData));
    USBHAL *obj = priv->inst;
    USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;
    uint32_t sofnum = (USBx_DEVICE->DSTS & USB_OTG_DSTS_FNSOF) >> 8;
    void (USBHAL::*func)(int frame) = priv->sof;
    (obj->*func)(sofnum);
}

USBHAL *USBHAL::instance;

USBHAL::USBHAL(void)
{
    USBHAL_Private_t *HALPriv = new (USBHAL_Private_t);

    memset(&hpcd.Init, 0, sizeof(hpcd.Init));

#if defined(TARGET_DISCO_F769NI) || \
    defined(TARGET_DISCO_F746NG_OTG_HS)
    hpcd.Instance = USB_OTG_HS;
    hpcd.Init.phy_itface = PCD_PHY_ULPI;
    hpcd.Init.Sof_enable = 0;
    hpcd.Init.speed = PCD_SPEED_HIGH;
#elif defined(TARGET_DISCO_F429ZI)
    hpcd.Instance = USB_OTG_HS;
    hpcd.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd.Init.Sof_enable = 0;
    hpcd.Init.speed = PCD_SPEED_HIGH;
#else
    hpcd.Instance = USB_OTG_FS;
    hpcd.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd.Init.Sof_enable = 1;
    hpcd.Init.speed = PCD_SPEED_FULL;
#endif
    hpcd.Init.dev_endpoints = NB_ENDPOINT;
    hpcd.Init.ep0_mps = MAX_PACKET_SIZE_EP0;

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

#if defined(TARGET_NUCLEO_F207ZG) || \
    defined(TARGET_NUCLEO_F401RE) || \
    defined(TARGET_NUCLEO_F411RE) || \
    defined(TARGET_NUCLEO_F412ZG) || \
    defined(TARGET_NUCLEO_F413ZH) || \
    defined(TARGET_NUCLEO_F429ZI) || \
    defined(TARGET_NUCLEO_F446RE) || \
    defined(TARGET_NUCLEO_F446ZE) || \
    defined(TARGET_NUCLEO_F767ZI) || \
    defined(TARGET_NUCLEO_F746ZG) || \
    defined(TARGET_DISCO_F407VG) || \
    defined(TARGET_DISCO_F413ZH) || \
    defined(TARGET_DISCO_F469NI) || \
    defined(TARGET_DISCO_F746NG_OTG_FS)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // DM
    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // DP
    pin_function(PA_9, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, GPIO_AF10_OTG_FS));  // VBUS
    pin_function(PA_10, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_PULLUP, GPIO_AF10_OTG_FS)); // ID
    pin_function(PA_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS));  // SOF
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

#elif defined(TARGET_DISCO_F429ZI)
    __HAL_RCC_GPIOB_CLK_ENABLE();
    pin_function(PB_14, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF12_OTG_HS_FS)); // DM
    pin_function(PB_15, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF12_OTG_HS_FS)); // DP
    pin_function(PB_13, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0)); // VBUS
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();

#elif defined(TARGET_DISCO_L475VG_IOT01A) || \
      defined(TARGET_DISCO_L476VG)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // DM
    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // DP
    __HAL_RCC_GPIOC_CLK_ENABLE();
    pin_function(PC_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // VBUS
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_EnableVddUSB();
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

#elif defined(TARGET_DISCO_F769NI) || \
      defined(TARGET_DISCO_F746NG_OTG_HS)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    pin_function(PA_5, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // CLK
    pin_function(PA_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // D0
    pin_function(PB_0, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // D1
    pin_function(PB_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // D2
    pin_function(PB_5, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // D3
    pin_function(PB_10, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // D4
    pin_function(PB_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // D5
    pin_function(PB_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // D6
    pin_function(PB_13, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // D7
    pin_function(PC_0, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // STP
    pin_function(PH_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // NXT
#if defined(TARGET_DISCO_F769NI)
    pin_function(PI_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // DIR
#else // TARGET_DISCO_F746NG
    pin_function(PC_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // DIR
#endif
    __HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE();
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();

#elif defined(TARGET_STEVAL_3DP001V1)
    __HAL_RCC_GPIOB_CLK_ENABLE();
    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // DM
    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // DP
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

#else
#error "USB pins are not configured !"
#endif

    __HAL_RCC_SYSCFG_CLK_ENABLE();

    // Configure PCD and FIFOs
    hpcd.pData = (void *)HALPriv;
    hpcd.State = HAL_PCD_STATE_RESET;
    HAL_PCD_Init(&hpcd);

    /* 1.25 kbytes */
    /* min value 16 (= 16 x 4 bytes) */
    /* max value 256 (= 1K bytes ) */
    /* maximum sum is 0x140 */
    HAL_PCDEx_SetRxFiFo(&hpcd, (MAXTRANSFER_SIZE / 4));
    // EP0 = Bulk/Int 64 bytes in FS
    HAL_PCDEx_SetTxFiFo(&hpcd, 0, (MAX_PACKET_SIZE_EP0 / 4) + 1);
    // EP1 = Bulk/Int bytes in FS
    HAL_PCDEx_SetTxFiFo(&hpcd, 1, (MAX_PACKET_SIZE_EP1 / 4) + 1);
    // EP2
    HAL_PCDEx_SetTxFiFo(&hpcd, 2, (MAX_PACKET_SIZE_EP2 / 4));
    // EP3 = ISOchronous
    HAL_PCDEx_SetTxFiFo(&hpcd, 3, (MAX_PACKET_SIZE_EP3 / 4));

    // Configure interrupt vector
    NVIC_SetVector(USBHAL_IRQn, (uint32_t)&_usbisr);
    NVIC_SetPriority(USBHAL_IRQn, 1);

    HAL_PCD_Start(&hpcd);
}

#endif // USBHAL_IP_OTGFSHS_H
