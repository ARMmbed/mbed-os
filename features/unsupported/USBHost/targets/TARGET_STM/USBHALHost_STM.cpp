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

#ifdef TARGET_STM
#include "mbed.h"
#include "USBHALHost.h"
#include "dbg.h"
#include "pinmap.h"

#include "USBHALHost_STM.h"

void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)
{
    USBHALHost_Private_t *priv = (USBHALHost_Private_t *)(hhcd->pData);
    USBHALHost *obj = priv->inst;
    void (USBHALHost::*func)(int hub, int port, bool lowSpeed, USBHostHub * hub_parent) = priv->deviceConnected;
    (obj->*func)(0, 1, 0, NULL);
}
void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)
{
    USBHALHost_Private_t *priv = (USBHALHost_Private_t *)(hhcd->pData);
    USBHALHost *obj = priv->inst;
    void (USBHALHost::*func1)(int hub, int port, USBHostHub * hub_parent, volatile uint32_t addr) = priv->deviceDisconnected;
    (obj->*func1)(0, 1, (USBHostHub *)NULL, 0);
}
int HAL_HCD_HC_GetDirection(HCD_HandleTypeDef *hhcd, uint8_t chnum)
{
    /*  useful for transmission */
    return hhcd->hc[chnum].ep_is_in;
}

uint32_t HAL_HCD_HC_GetMaxPacket(HCD_HandleTypeDef *hhcd, uint8_t chnum)
{
    /*  useful for transmission */
    return hhcd->hc[chnum].max_packet;
}

void  HAL_HCD_EnableInt(HCD_HandleTypeDef *hhcd, uint8_t chnum)
{
    USB_OTG_GlobalTypeDef *USBx = hhcd->Instance;
    USBx_HOST->HAINTMSK |= (1 << chnum);
}


void  HAL_HCD_DisableInt(HCD_HandleTypeDef *hhcd, uint8_t chnum)
{
    USB_OTG_GlobalTypeDef *USBx = hhcd->Instance;
    USBx_HOST->HAINTMSK &= ~(1 << chnum);
}
uint32_t HAL_HCD_HC_GetType(HCD_HandleTypeDef *hhcd, uint8_t chnum)
{
    /*  useful for transmission */
    return hhcd->hc[chnum].ep_type;
}

void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum, HCD_URBStateTypeDef urb_state)
{
    USBHALHost_Private_t *priv = (USBHALHost_Private_t *)(hhcd->pData);
    USBHALHost *obj = priv->inst;
    void (USBHALHost::*func)(volatile uint32_t addr) = priv->transferCompleted;

    uint32_t addr = priv->addr[chnum];
    uint32_t max_size = HAL_HCD_HC_GetMaxPacket(hhcd, chnum);
    uint32_t type = HAL_HCD_HC_GetType(hhcd, chnum);
    uint32_t dir = HAL_HCD_HC_GetDirection(hhcd, chnum);
    uint32_t length;
    if ((addr != 0)) {
        HCTD *td = (HCTD *)addr;

        if ((type == EP_TYPE_BULK) || (type == EP_TYPE_CTRL)) {
            switch (urb_state) {
                case URB_DONE:
#if defined(MAX_NYET_RETRY)
                    td->retry = 0;
#endif
                    if (td->size >  max_size) {
                        /*  enqueue  another request */
                        td->currBufPtr += max_size;
                        td->size -= max_size;
                        length = td->size <= max_size ? td->size : max_size;
                        MBED_ASSERT(HAL_HCD_HC_SubmitRequest(hhcd, chnum, dir, type, !td->setup, (uint8_t *) td->currBufPtr, length, 0) == HAL_OK);
                        HAL_HCD_EnableInt(hhcd, chnum);
                        return;
                    }
                    break;
                case  URB_NOTREADY:
                    /*  try again  */
                    /*  abritary limit , to avoid dead lock if other error than
                     *  slow response is  */
#if defined(MAX_NYET_RETRY)
                    if (td->retry < MAX_NYET_RETRY) {
                        /*  increment retry counter */
                        td->retry++;
#endif
                        length = td->size <= max_size ? td->size : max_size;
                        MBED_ASSERT(HAL_HCD_HC_SubmitRequest(hhcd, chnum, dir, type, !td->setup, (uint8_t *) td->currBufPtr, length, 0) == HAL_OK);
                        HAL_HCD_EnableInt(hhcd, chnum);
                        return;
#if defined(MAX_NYET_RETRY)
                    } else {
                        USB_ERR("urb_state != URB_NOTREADY");
                    }
#endif
                    break;
            }
        }
        if ((type == EP_TYPE_INTR)) {
            /*  reply a packet of length NULL, this will be analyse in call back
             *  for mouse or hub */
            td->state = USB_TYPE_IDLE ;
            HAL_HCD_DisableInt(hhcd, chnum);

        } else {
            td->state = (urb_state == URB_DONE) ?  USB_TYPE_IDLE : USB_TYPE_ERROR;
        }
        td->currBufPtr += HAL_HCD_HC_GetXferCount(hhcd, chnum);
        (obj->*func)(addr);
    } else {
        if (urb_state != 0) {
            USB_DBG_EVENT("spurious %d %d", chnum, urb_state);
        }
    }
}

USBHALHost *USBHALHost::instHost;


void USBHALHost::init()
{

    NVIC_DisableIRQ(USBHAL_IRQn);
    NVIC_SetVector(USBHAL_IRQn, (uint32_t)(_usbisr));
    HAL_HCD_Init((HCD_HandleTypeDef *) usb_hcca);
    NVIC_EnableIRQ(USBHAL_IRQn);
    control_disable = 0;
    HAL_HCD_Start((HCD_HandleTypeDef *) usb_hcca);
    usb_vbus(1);
}

uint32_t USBHALHost::controlHeadED()
{
    return 0xffffffff;
}

uint32_t USBHALHost::bulkHeadED()
{
    return 0xffffffff;
}

uint32_t USBHALHost::interruptHeadED()
{
    return 0xffffffff;
}

void USBHALHost::updateBulkHeadED(uint32_t addr)
{
}


void USBHALHost::updateControlHeadED(uint32_t addr)
{
}

void USBHALHost::updateInterruptHeadED(uint32_t addr)
{
}


void USBHALHost::enableList(ENDPOINT_TYPE type)
{
    /*  react when the 3 lists are requested to be disabled */
    if (type == CONTROL_ENDPOINT) {
        control_disable--;
        if (control_disable == 0) {
            NVIC_EnableIRQ(USBHAL_IRQn);
        } else {
            printf("reent\n");
        }
    }
}


bool USBHALHost::disableList(ENDPOINT_TYPE type)
{
    if (type == CONTROL_ENDPOINT) {
        NVIC_DisableIRQ(USBHAL_IRQn);
        control_disable++;
        if (control_disable > 1) {
            printf("disable reentrance !!!\n");
        }
        return true;
    }
    return false;
}


void USBHALHost::memInit()
{
    usb_hcca = (volatile HCD_HandleTypeDef *)usb_buf;
    usb_edBuf = usb_buf + HCCA_SIZE;
    usb_tdBuf = usb_buf + HCCA_SIZE + (MAX_ENDPOINT * ED_SIZE);
    /*  init channel  */
    memset((void *)usb_buf, 0, TOTAL_SIZE);
    for (int i = 0; i < MAX_ENDPOINT; i++) {
        HCED    *hced = (HCED *)(usb_edBuf + i * ED_SIZE);
        hced->ch_num = i;
        hced->hhcd = (HCCA *) usb_hcca;
    }
}

volatile uint8_t *USBHALHost::getED()
{
    for (int i = 0; i < MAX_ENDPOINT; i++) {
        if (!edBufAlloc[i]) {
            edBufAlloc[i] = true;
            return (volatile uint8_t *)(usb_edBuf + i * ED_SIZE);
        }
    }
    perror("Could not allocate ED\r\n");
    return NULL; //Could not alloc ED
}

volatile uint8_t *USBHALHost::getTD()
{
    int i;
    for (i = 0; i < MAX_TD; i++) {
        if (!tdBufAlloc[i]) {
            tdBufAlloc[i] = true;
            return (volatile uint8_t *)(usb_tdBuf + i * TD_SIZE);
        }
    }
    perror("Could not allocate TD\r\n");
    return NULL; //Could not alloc TD
}


void USBHALHost::freeED(volatile uint8_t *ed)
{
    int i;
    i = (ed - usb_edBuf) / ED_SIZE;
    edBufAlloc[i] = false;
}

void USBHALHost::freeTD(volatile uint8_t *td)
{
    int i;
    i = (td - usb_tdBuf) / TD_SIZE;
    tdBufAlloc[i] = false;
}


void USBHALHost::resetRootHub()
{
    // Initiate port reset
    wait(0.2);
    HAL_HCD_ResetPort((HCD_HandleTypeDef *)usb_hcca);
}


void USBHALHost::_usbisr(void)
{
    if (instHost) {
        instHost->UsbIrqhandler();
    }
}

void USBHALHost::UsbIrqhandler()
{
    HAL_HCD_IRQHandler((HCD_HandleTypeDef *)usb_hcca);
}
#endif
