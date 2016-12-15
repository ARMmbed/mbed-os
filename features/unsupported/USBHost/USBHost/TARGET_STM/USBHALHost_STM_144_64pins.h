/* mbed USBHost Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef USBHALHOST_STM32_144_64
#define USBHALHOST_STM32_144_64

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

/*  CONFIGURATION for USB_VBUS  
 *  on 64 bits board PC_0 is used  (0  VBUS on,  1 VBUS off)
 *  on 144 pins board PG_6 is used ( 1 VBUS on, 0 VBUS on)
 */
static gpio_t gpio_vbus;

#if defined(USBHALHOST_64pins)
#define  VBUS_OFF 1
#define  VBUS_ON 0
#define USB_VBUS_CONFIG \
    do {__HAL_RCC_GPIOC_CLK_ENABLE();\
		gpio_init_out_ex(&gpio_vbus, PC_0, VBUS_OFF);\
	}while(0);
#else
#define  VBUS_OFF 0
#define  VBUS_ON 1
#define USB_VBUS_CONFIG \
    do {__HAL_RCC_GPIOG_CLK_ENABLE();\
		gpio_init_out_ex(&gpio_vbus, PG_6, VBUS_OFF);\
	}while(0);
#endif

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
    hhcd->Init.speed = HCD_SPEED_FULL; 
    hhcd->Init.phy_itface = HCD_PHY_EMBEDDED;
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
    /* Configure USB FS GPIOs */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*USB DM and DP */
    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS));
    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS));
    /*USB ID */
    pin_function(PA_10, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_PULLUP, GPIO_AF10_OTG_FS));

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    /* Configure POWER_SWITCH IO pin */
    USB_VBUS_CONFIG;
    /* Enable USB FS Clocks */
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

    /* Set USBFS Interrupt priority */
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 6, 0);
}
#endif
