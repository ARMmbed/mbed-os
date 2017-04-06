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
#ifndef USBHALHOST_DISCOL476VG
#define USBHALHOST_DISCOL476VG

#define USBHAL_IRQn  OTG_FS_IRQn

#define HCCA_SIZE sizeof(HCD_HandleTypeDef)
#define ED_SIZE  sizeof(HCED)
#define TD_SIZE  sizeof(HCTD)

#define TOTAL_SIZE (HCCA_SIZE + (MAX_ENDPOINT*ED_SIZE) + (MAX_TD*TD_SIZE))
/* STM device FS have 11 channels  (definition is for 60 channels) */
static volatile  uint8_t usb_buf[TOTAL_SIZE];
typedef struct
{
    /* store the request ongoing on each endpoit  */
    /*  1st field of structure avoid  giving knowledge of all structure to
     *  endpoint */
    volatile uint32_t addr[MAX_ENDPOINT];
    USBHALHost *inst;
    void (USBHALHost::*deviceConnected)(int hub, int port, bool lowSpeed, USBHostHub * hub_parent);
    void (USBHALHost::*deviceDisconnected)(int hub, int port, USBHostHub * hub_parent, volatile uint32_t addr);
    void (USBHALHost::*transferCompleted)(volatile uint32_t addr);
}USBHALHost_Private_t;

static gpio_t gpio_vbus;

#define  VBUS_OFF 1
#define  VBUS_ON 0
#define USB_VBUS_CONFIG \
    do {\
		gpio_init_out_ex(&gpio_vbus, PC_9, VBUS_OFF);\
	}while(0);


void  usb_vbus( uint8_t state)
{
    if(state == 0)
    {
        gpio_write(&gpio_vbus, VBUS_OFF);
    }
    else
    {
        gpio_write(&gpio_vbus, VBUS_ON);
    }
    wait(0.2);
}


USBHALHost::USBHALHost() {
    instHost = this;
    HCD_HandleTypeDef *hhcd;
    USBHALHost_Private_t *HALPriv = new(USBHALHost_Private_t);
    memset(HALPriv, 0, sizeof(USBHALHost_Private_t));
    memInit();
    memset((void*)usb_hcca, 0, HCCA_SIZE);
    hhcd = (HCD_HandleTypeDef *)usb_hcca;
    hhcd->Instance = USB_OTG_FS;
    hhcd->pData = (void*)HALPriv;
    hhcd->Init.Host_channels = 11;
    /*   for now failed with dma */
    hhcd->Init.dma_enable = 0;
    hhcd->Init.speed =  HCD_SPEED_FULL;
    hhcd->Init.phy_itface = HCD_PHY_EMBEDDED;
    hhcd->Init.use_external_vbus = 1;
    HALPriv->inst = this;
    HALPriv->deviceConnected = &USBHALHost::deviceConnected;
    HALPriv->deviceDisconnected = &USBHALHost::deviceDisconnected;
    HALPriv->transferCompleted = &USBHALHost::transferCompleted;
    for (int i = 0; i < MAX_ENDPOINT; i++) {
        edBufAlloc[i] = false;
        HALPriv->addr[i]=(uint32_t)-1;
    }
    for (int i = 0; i < MAX_TD; i++) {
        tdBufAlloc[i] = false;
    }
    __HAL_RCC_PWR_CLK_ENABLE();
#ifdef TARGET_STM32L4
    HAL_PWREx_EnableVddUSB();
#endif

    /* Configure USB HS GPIOs */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*USB DM and DP */
    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS));
    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS));

    /* Configure  VBUS Pin */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    pin_function(PC_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS));
    /* Configure POWER_SWITCH IO pin */
    USB_VBUS_CONFIG;
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* Enable USB FS Clocks */
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    /* Set USBFS Interrupt priority */
    HAL_NVIC_SetPriority(USBHAL_IRQn, 5, 0);
    NVIC_SetVector(USBHAL_IRQn, (uint32_t)&_usbisr);
}
#endif
