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
#ifndef USBHALHOST_STM_H
#define USBHALHOST_STM_H

#if defined(TARGET_DISCO_F746NG)
#if (MBED_CONF_TARGET_USB_SPEED == 1) // Defined in json configuration file
#define TARGET_DISCO_F746NG_HS
#else
#define TARGET_DISCO_F746NG_FS
#endif
#endif

#if defined(TARGET_DISCO_F746NG_HS) || defined(TARGET_DISCO_F769NI)
#define USBHAL_IRQn  OTG_HS_IRQn
#else
#define USBHAL_IRQn  OTG_FS_IRQn
#endif

#define HCCA_SIZE sizeof(HCD_HandleTypeDef)
#define ED_SIZE  sizeof(HCED)
#define TD_SIZE  sizeof(HCTD)

#define TOTAL_SIZE (HCCA_SIZE + (MAX_ENDPOINT * ED_SIZE) + (MAX_TD * TD_SIZE))

/* STM device FS have 11 channels (definition is for 60 channels) */
static volatile  uint8_t usb_buf[TOTAL_SIZE];

typedef struct {
    /* store the request ongoing on each endpoit  */
    /*  1st field of structure avoid  giving knowledge of all structure to
     *  endpoint */
    volatile uint32_t addr[MAX_ENDPOINT];
    USBHALHost *inst;
    void (USBHALHost::*deviceConnected)(int hub, int port, bool lowSpeed, USBHostHub *hub_parent);
    void (USBHALHost::*deviceDisconnected)(int hub, int port, USBHostHub *hub_parent, volatile uint32_t addr);
    void (USBHALHost::*transferCompleted)(volatile uint32_t addr);
} USBHALHost_Private_t;

static gpio_t gpio_powerpin;

// NUCLEO_64 boards
#if defined(TARGET_NUCLEO_F401RE) || \
    defined(TARGET_NUCLEO_F411RE) || \
    defined(TARGET_NUCLEO_F446RE) || \
    defined(TARGET_NUCLEO_L476RG) || \
    defined(TARGET_NUCLEO_L486RG)
#define USB_POWER_ON  0
#define USB_POWER_OFF 1
#define USB_POWERPIN_CONFIG {__HAL_RCC_GPIOC_CLK_ENABLE();gpio_init_out_ex(&gpio_powerpin, PC_0, USB_POWER_OFF);}

// NUCLEO_144 boards
#elif defined(TARGET_NUCLEO_F207ZG) || \
    defined(TARGET_NUCLEO_F412ZG) || \
    defined(TARGET_NUCLEO_F413ZH) || \
    defined(TARGET_NUCLEO_F429ZI) || \
    defined(TARGET_NUCLEO_F439ZI) || \
    defined(TARGET_NUCLEO_F446ZE) || \
    defined(TARGET_NUCLEO_F767ZI) || \
    defined(TARGET_NUCLEO_F746ZG) || \
    defined(TARGET_NUCLEO_F756ZG) || \
    defined(TARGET_NUCLEO_F767ZI)
#define USB_POWER_ON  1
#define USB_POWER_OFF 0
#define USB_POWERPIN_CONFIG {__HAL_RCC_GPIOG_CLK_ENABLE();gpio_init_out_ex(&gpio_powerpin, PG_6, USB_POWER_OFF);}

// DISCOVERY boards
#elif defined(TARGET_DISCO_F413ZH)
#define USB_POWER_ON  0
#define USB_POWER_OFF 1
#define USB_POWERPIN_CONFIG {__HAL_RCC_GPIOG_CLK_ENABLE();gpio_init_out_ex(&gpio_powerpin, PG_8, USB_POWER_OFF);}

#elif defined(TARGET_DISCO_F469NI)
#define USB_POWER_ON  1
#define USB_POWER_OFF 0
#define USB_POWERPIN_CONFIG {__HAL_RCC_GPIOB_CLK_ENABLE();gpio_init_out_ex(&gpio_powerpin, PB_2, USB_POWER_OFF);}

#elif defined(TARGET_DISCO_F746NG_FS)
#define USB_POWER_ON  0
#define USB_POWER_OFF 1
#define USB_POWERPIN_CONFIG {__HAL_RCC_GPIOD_CLK_ENABLE();gpio_init_out_ex(&gpio_powerpin, PD_5, USB_POWER_OFF);}

#elif defined(TARGET_DISCO_F746NG_HS)
#define USB_POWER_ON  0
#define USB_POWER_OFF 1
#define USB_POWERPIN_CONFIG {}

#elif defined(TARGET_DISCO_F769NI)
#define USB_POWER_ON  0
#define USB_POWER_OFF 1
#define USB_POWERPIN_CONFIG {}

#elif defined(TARGET_DISCO_L475VG_IOT01A)
#define USB_POWER_ON  0
#define USB_POWER_OFF 1
#define USB_POWERPIN_CONFIG {__HAL_RCC_GPIOD_CLK_ENABLE();gpio_init_out_ex(&gpio_powerpin, PD_12, USB_POWER_OFF);}

#elif defined(TARGET_DISCO_L476VG)
#define USB_POWER_ON  0
#define USB_POWER_OFF 1
#define USB_POWERPIN_CONFIG {__HAL_RCC_GPIOC_CLK_ENABLE();gpio_init_out_ex(&gpio_powerpin, PC_9, USB_POWER_OFF);}

#else
#error "USB power pin is not configured !"
#endif


void usb_vbus(uint8_t state)
{
    if (state == 0) {
        gpio_write(&gpio_powerpin, USB_POWER_OFF);
    } else {
        gpio_write(&gpio_powerpin, USB_POWER_ON);
    }
    wait(0.2);
}


USBHALHost::USBHALHost()
{
    instHost = this;
    HCD_HandleTypeDef *hhcd = {0};
    USBHALHost_Private_t *HALPriv = new (USBHALHost_Private_t);

    memset(HALPriv, 0, sizeof(USBHALHost_Private_t));
    memInit();
    memset((void *)usb_hcca, 0, HCCA_SIZE);

    hhcd = (HCD_HandleTypeDef *)usb_hcca;
    hhcd->pData = (void *)HALPriv;

#if defined(TARGET_DISCO_F746NG_HS) || defined(TARGET_DISCO_F769NI)
    hhcd->Instance = USB_OTG_HS;
    hhcd->Init.speed =  HCD_SPEED_HIGH;
    hhcd->Init.phy_itface = HCD_PHY_ULPI;
#else
    hhcd->Instance = USB_OTG_FS;
    hhcd->Init.speed =  HCD_SPEED_FULL;
    hhcd->Init.phy_itface = HCD_PHY_EMBEDDED;
#endif

    hhcd->Init.Host_channels = 11;
    hhcd->Init.dma_enable = 0; // for now failed with dma
    hhcd->Init.low_power_enable = 0;
    hhcd->Init.Sof_enable = 0;
    hhcd->Init.vbus_sensing_enable = 0;
    hhcd->Init.use_external_vbus = 1;
    hhcd->Init.lpm_enable = 0;

    HALPriv->inst = this;
    HALPriv->deviceConnected = &USBHALHost::deviceConnected;
    HALPriv->deviceDisconnected = &USBHALHost::deviceDisconnected;
    HALPriv->transferCompleted = &USBHALHost::transferCompleted;

    for (int i = 0; i < MAX_ENDPOINT; i++) {
        edBufAlloc[i] = false;
        HALPriv->addr[i] = (uint32_t) - 1;
    }

    for (int i = 0; i < MAX_TD; i++) {
        tdBufAlloc[i] = false;
    }

    __HAL_RCC_PWR_CLK_ENABLE();

#ifdef TARGET_STM32L4
    HAL_PWREx_EnableVddUSB();
#endif

    // Configure USB pins
#if defined(TARGET_NUCLEO_F401RE) || \
    defined(TARGET_NUCLEO_F411RE) || \
    defined(TARGET_NUCLEO_F446RE) || \
    defined(TARGET_NUCLEO_L476RG) || \
    defined(TARGET_NUCLEO_L486RG) || \
    defined(TARGET_NUCLEO_F207ZG) || \
    defined(TARGET_NUCLEO_F412ZG) || \
    defined(TARGET_NUCLEO_F413ZH) || \
    defined(TARGET_NUCLEO_F429ZI) || \
    defined(TARGET_NUCLEO_F439ZI) || \
    defined(TARGET_NUCLEO_F446ZE) || \
    defined(TARGET_NUCLEO_F767ZI) || \
    defined(TARGET_NUCLEO_F746ZG) || \
    defined(TARGET_NUCLEO_F756ZG) || \
    defined(TARGET_NUCLEO_F767ZI) || \
    defined(TARGET_DISCO_F413ZH) || \
    defined(TARGET_DISCO_F469NI) || \
    defined(TARGET_DISCO_L475VG_IOT01A)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // DM
    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // DP
    pin_function(PA_10, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_PULLUP, GPIO_AF10_OTG_FS)); // ID
    pin_function(PA_9, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // VBUS

#elif defined(TARGET_DISCO_F746NG_FS)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // DM
    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // DP
    pin_function(PA_10, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_PULLUP, GPIO_AF10_OTG_FS)); // ID
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    pin_function(PJ_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // VBUS

#elif defined(TARGET_DISCO_F746NG_HS)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
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
    pin_function(PC_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // DIR

#elif defined(TARGET_DISCO_F769NI)
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
    pin_function(PI_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_HS)); // DIR

#elif defined(TARGET_DISCO_L476VG)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // DM
    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // DP
    pin_function(PA_10, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_PULLUP, GPIO_AF10_OTG_FS)); // ID
    pin_function(PC_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // VBUS

#elif defined(TARGET_STEVAL_3DP001V1)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // DM
    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS)); // DP

#else
#error "USB pins are not configured !"
#endif

    // Configure USB POWER pin
    USB_POWERPIN_CONFIG;

    // Enable clocks
    __HAL_RCC_SYSCFG_CLK_ENABLE();

#if defined(TARGET_DISCO_F746NG_HS) || defined(TARGET_DISCO_F769NI)
    __HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE();
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
#else
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
#endif

    // Set USB interrupt
    HAL_NVIC_SetPriority(USBHAL_IRQn, 5, 0);
    NVIC_SetVector(USBHAL_IRQn, (uint32_t)&_usbisr);
}

#endif // USBHALHOST_STM_H
