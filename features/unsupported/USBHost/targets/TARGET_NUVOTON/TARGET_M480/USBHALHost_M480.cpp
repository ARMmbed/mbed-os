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

#include "mbed.h"
#include "USBHALHost.h"
#include "dbg.h"
#include "pinmap.h"

#define HCCA_SIZE   sizeof(HCCA)
#define ED_SIZE     sizeof(HCED)
#define TD_SIZE     sizeof(HCTD)

#define TOTAL_SIZE (HCCA_SIZE + (MAX_ENDPOINT*ED_SIZE) + (MAX_TD*TD_SIZE))
#ifndef USBH_HcRhDescriptorA_POTPGT_Pos
#define USBH_HcRhDescriptorA_POTPGT_Pos  (24)
#endif
#ifndef USBH_HcRhDescriptorA_POTPGT_Msk
#define USBH_HcRhDescriptorA_POTPGT_Msk  (0xfful << USBH_HcRhDescriptorA_POTPGT_Pos)
#endif

static volatile MBED_ALIGN(256) uint8_t usb_buf[TOTAL_SIZE];  // 256 bytes aligned!

USBHALHost * USBHALHost::instHost;

USBHALHost::USBHALHost()
{
    instHost = this;
    memInit();
    memset((void*)usb_hcca, 0, HCCA_SIZE);
    for (int i = 0; i < MAX_ENDPOINT; i++) {
        edBufAlloc[i] = false;
    }
    for (int i = 0; i < MAX_TD; i++) {
        tdBufAlloc[i] = false;
    }
}

void USBHALHost::init()
{
    // Unlock protected registers
    SYS_UnlockReg();

    /* Enable IP clock */
    CLK->AHBCLK |= CLK_AHBCLK_USBHCKEN_Msk | (1 << 4) | CLK_AHBCLK_HSUSBDCKEN_Msk;


    /* USB Host desired input clock is 48 MHz. Set as PLL divided by 4 (192/4 = 48) */
    CLK->CLKDIV0 = (CLK->CLKDIV0 & ~CLK_CLKDIV0_USBDIV_Msk) | (3 << CLK_CLKDIV0_USBDIV_Pos);

    /* Enable USBD and OTG clock */
    CLK->APBCLK0 |= CLK_APBCLK0_USBDCKEN_Msk | CLK_APBCLK0_OTGCKEN_Msk;

    /* Configure USB to USB Host role */
    SYS->USBPHY = SYS_USBPHY_HSUSBEN_Msk | SYS_USBPHY_HSUSBROLE_STD_USBH | SYS_USBPHY_USBEN_Msk | SYS_USBPHY_SBO_Msk | SYS_USBPHY_USBROLE_STD_USBH;

    wait_us(20);

    SYS->USBPHY |= SYS_USBPHY_HSUSBACT_Msk;

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/


    /* USB_VBUS_EN (USB 1.1 VBUS power enable pin) multi-function pin - PB.15     */
    pin_function(PB_15, SYS_GPB_MFPH_PB15MFP_USB_VBUS_EN);

    /* USB_VBUS_ST (USB 1.1 over-current detect pin) multi-function pin - PC.14   */
    pin_function(PC_14, SYS_GPC_MFPH_PC14MFP_USB_VBUS_ST);

    /* HSUSB_VBUS_EN (USB 2.0 VBUS power enable pin) multi-function pin - PB.10   */
    pin_function(PB_10, SYS_GPB_MFPH_PB10MFP_HSUSB_VBUS_EN);

    /* HSUSB_VBUS_ST (USB 2.0 over-current detect pin) multi-function pin - PB.11 */
    pin_function(PB_11, SYS_GPB_MFPH_PB11MFP_HSUSB_VBUS_ST);

    /* Configure pins for USB 1.1 port: VBUS/D+/D-/ID */
    pin_function(PA_12, SYS_GPA_MFPH_PA12MFP_USB_VBUS);
    pin_function(PA_13, SYS_GPA_MFPH_PA13MFP_USB_D_N);
    pin_function(PA_14, SYS_GPA_MFPH_PA14MFP_USB_D_P);
    pin_function(PA_15, (int) SYS_GPA_MFPH_PA15MFP_USB_OTG_ID);

    SYS_LockReg();
    HSUSBH->USBPCR0 = 0x160;                /* enable PHY 0          */
    HSUSBH->USBPCR1 = 0x520;                /* enable PHY 1          */

    // Overcurrent flag is low active
    USBH->HcMiscControl |= USBH_HcMiscControl_OCAL_Msk;

    // Disable HC interrupts
    USBH->HcInterruptDisable = OR_INTR_ENABLE_MIE;

    // Needed by some controllers
    USBH->HcControl = 0;

    // Software reset
    USBH->HcCommandStatus = OR_CMD_STATUS_HCR;
    while (USBH->HcCommandStatus & OR_CMD_STATUS_HCR);

    // Put HC in reset state
    USBH->HcControl = (USBH->HcControl & ~OR_CONTROL_HCFS) | OR_CONTROL_HC_RSET;
    // HCD must wait 10ms for HC reset complete
    wait_ms(100);

    USBH->HcControlHeadED = 0;                      // Initialize Control ED list head to 0
    USBH->HcBulkHeadED = 0;                         // Initialize Bulk ED list head to 0
    USBH->HcHCCA = (uint32_t) usb_hcca;

    USBH->HcFmInterval = DEFAULT_FMINTERVAL;        // Frame interval = 12000 - 1
    // MPS = 10,104
    USBH->HcPeriodicStart = FI * 90 / 100;          // 90% of frame interval
    USBH->HcLSThreshold = 0x628;                    // Low speed threshold

    // Put HC in operational state
    USBH->HcControl  = (USBH->HcControl & (~OR_CONTROL_HCFS)) | OR_CONTROL_HC_OPER;

    // FIXME
    USBH->HcRhDescriptorA = USBH->HcRhDescriptorA & ~(USBH_HcRhDescriptorA_NOCP_Msk | USBH_HcRhDescriptorA_OCPM_Msk | USBH_HcRhDescriptorA_PSM_Msk);
    // Issue SetGlobalPower command
    USBH->HcRhStatus = USBH_HcRhStatus_LPSC_Msk;
    // Power On To Power Good Time, in 2 ms units
    wait_ms(((USBH->HcRhDescriptorA & USBH_HcRhDescriptorA_POTPGT_Msk) >> USBH_HcRhDescriptorA_POTPGT_Pos) * 2);

    // Clear Interrrupt Status
    USBH->HcInterruptStatus |= USBH->HcInterruptStatus;
    // Enable interrupts we care about
    USBH->HcInterruptEnable  = OR_INTR_ENABLE_MIE | OR_INTR_ENABLE_WDH | OR_INTR_ENABLE_RHSC;

    NVIC_SetVector(USBH_IRQn, (uint32_t)(_usbisr));
    NVIC_EnableIRQ(USBH_IRQn);


    // Check for any connected devices
    if (USBH->HcRhPortStatus[0] & OR_RH_PORT_CCS) {
        // Device connected
        wait_ms(150);
        deviceConnected(0, 1, USBH->HcRhPortStatus[0] & OR_RH_PORT_LSDA);
    }

    // Check for any connected devices
    if (USBH->HcRhPortStatus[1] & OR_RH_PORT_CCS) {
        // Device connected
        wait_ms(150);
        deviceConnected(0, 2, USBH->HcRhPortStatus[1] & OR_RH_PORT_LSDA);
    }
}

uint32_t USBHALHost::controlHeadED()
{
    return USBH->HcControlHeadED;
}

uint32_t USBHALHost::bulkHeadED()
{
    return USBH->HcBulkHeadED;
}

uint32_t USBHALHost::interruptHeadED()
{
    // FIXME: Only support one INT ED?
    return usb_hcca->IntTable[0];
}

void USBHALHost::updateBulkHeadED(uint32_t addr)
{
    USBH->HcBulkHeadED = addr;
}


void USBHALHost::updateControlHeadED(uint32_t addr)
{
    USBH->HcControlHeadED = addr;
}

void USBHALHost::updateInterruptHeadED(uint32_t addr)
{
    // FIXME: Only support one INT ED?
    usb_hcca->IntTable[0] = addr;
}


void USBHALHost::enableList(ENDPOINT_TYPE type)
{
    switch(type) {
    case CONTROL_ENDPOINT:
        USBH->HcCommandStatus = OR_CMD_STATUS_CLF;
        USBH->HcControl |= OR_CONTROL_CLE;
        break;
    case ISOCHRONOUS_ENDPOINT:
        // FIXME
        break;
    case BULK_ENDPOINT:
        USBH->HcCommandStatus = OR_CMD_STATUS_BLF;
        USBH->HcControl |= OR_CONTROL_BLE;
        break;
    case INTERRUPT_ENDPOINT:
        USBH->HcControl |= OR_CONTROL_PLE;
        break;
    }
}


bool USBHALHost::disableList(ENDPOINT_TYPE type)
{
    switch(type) {
    case CONTROL_ENDPOINT:
        if(USBH->HcControl & OR_CONTROL_CLE) {
            USBH->HcControl &= ~OR_CONTROL_CLE;
            return true;
        }
        return false;
    case ISOCHRONOUS_ENDPOINT:
        // FIXME
        return false;
    case BULK_ENDPOINT:
        if(USBH->HcControl & OR_CONTROL_BLE) {
            USBH->HcControl &= ~OR_CONTROL_BLE;
            return true;
        }
        return false;
    case INTERRUPT_ENDPOINT:
        if(USBH->HcControl & OR_CONTROL_PLE) {
            USBH->HcControl &= ~OR_CONTROL_PLE;
            return true;
        }
        return false;
    }
    return false;
}


void USBHALHost::memInit()
{
    usb_hcca = (volatile HCCA *)usb_buf;
    usb_edBuf = usb_buf + HCCA_SIZE;
    usb_tdBuf = usb_buf + HCCA_SIZE + (MAX_ENDPOINT*ED_SIZE);
}

volatile uint8_t * USBHALHost::getED()
{
    for (int i = 0; i < MAX_ENDPOINT; i++) {
        if ( !edBufAlloc[i] ) {
            edBufAlloc[i] = true;
            return (volatile uint8_t *)(usb_edBuf + i*ED_SIZE);
        }
    }
    perror("Could not allocate ED\r\n");
    return NULL; //Could not alloc ED
}

volatile uint8_t * USBHALHost::getTD()
{
    int i;
    for (i = 0; i < MAX_TD; i++) {
        if ( !tdBufAlloc[i] ) {
            tdBufAlloc[i] = true;
            return (volatile uint8_t *)(usb_tdBuf + i*TD_SIZE);
        }
    }
    perror("Could not allocate TD\r\n");
    return NULL; //Could not alloc TD
}


void USBHALHost::freeED(volatile uint8_t * ed)
{
    int i;
    i = (ed - usb_edBuf) / ED_SIZE;
    edBufAlloc[i] = false;
}

void USBHALHost::freeTD(volatile uint8_t * td)
{
    int i;
    i = (td - usb_tdBuf) / TD_SIZE;
    tdBufAlloc[i] = false;
}


void USBHALHost::resetRootHub()
{
    // Reset port1
    USBH->HcRhPortStatus[0] = OR_RH_PORT_PRS;
    while (USBH->HcRhPortStatus[0] & OR_RH_PORT_PRS);
    USBH->HcRhPortStatus[0] = OR_RH_PORT_PRSC;

    USBH->HcRhPortStatus[1] = OR_RH_PORT_PRS;
    while (USBH->HcRhPortStatus[1] & OR_RH_PORT_PRS);
    USBH->HcRhPortStatus[1] = OR_RH_PORT_PRSC;
}


void USBHALHost::_usbisr(void)
{
    if (instHost) {
        instHost->UsbIrqhandler();
    }
}

void USBHALHost::UsbIrqhandler()
{
    uint32_t ints = USBH->HcInterruptStatus;

    // Root hub status change interrupt
    if (ints & OR_INTR_STATUS_RHSC) {
        uint32_t ints_roothub = USBH->HcRhStatus;
        uint32_t ints_port1 = USBH->HcRhPortStatus[0];
        uint32_t ints_port2 = USBH->HcRhPortStatus[1];

        // Port1: ConnectStatusChange
        if (ints_port1 & OR_RH_PORT_CSC) {
            if (ints_roothub & OR_RH_STATUS_DRWE) {
                // When DRWE is on, Connect Status Change means a remote wakeup event.
            } else {
                if (ints_port1 & OR_RH_PORT_CCS) {
                    // Root device connected

                    // wait 150ms to avoid bounce
                    wait_ms(150);

                    //Hub 0 (root hub), Port 1 (count starts at 1), Low or High speed
                    deviceConnected(0, 1, ints_port1 & OR_RH_PORT_LSDA);
                } else {
                    // Root device disconnected

                    if (!(ints & OR_INTR_STATUS_WDH)) {
                        usb_hcca->DoneHead = 0;
                    }

                    // wait 200ms to avoid bounce
                    wait_ms(200);

                    deviceDisconnected(0, 1, NULL, usb_hcca->DoneHead & 0xFFFFFFFE);

                    if (ints & OR_INTR_STATUS_WDH) {
                        usb_hcca->DoneHead = 0;
                        USBH->HcInterruptStatus = OR_INTR_STATUS_WDH;
                    }
                }
            }
            USBH->HcRhPortStatus[0] = OR_RH_PORT_CSC;
        }

        // Port1: ConnectStatusChange
        if (ints_port2 & OR_RH_PORT_CSC) {
            if (ints_roothub & OR_RH_STATUS_DRWE) {
                // When DRWE is on, Connect Status Change means a remote wakeup event.
            } else {
                if (ints_port2 & OR_RH_PORT_CCS) {
                    // Root device connected

                    // wait 150ms to avoid bounce
                    wait_ms(150);

                    //Hub 0 (root hub), Port 2 (count starts at 2), Low or High speed
                    deviceConnected(0, 2, ints_port2 & OR_RH_PORT_LSDA);
                } else {
                    // Root device disconnected

                    if (!(ints & OR_INTR_STATUS_WDH)) {
                        usb_hcca->DoneHead = 0;
                    }

                    // wait 200ms to avoid bounce
                    wait_ms(200);

                    deviceDisconnected(0, 2, NULL, usb_hcca->DoneHead & 0xFFFFFFFE);

                    if (ints & OR_INTR_STATUS_WDH) {
                        usb_hcca->DoneHead = 0;
                        USBH->HcInterruptStatus = OR_INTR_STATUS_WDH;
                    }
                }
            }
            USBH->HcRhPortStatus[1] = OR_RH_PORT_CSC;
        }


        // Port1: Reset completed
        if (ints_port1 & OR_RH_PORT_PRSC) {
            USBH->HcRhPortStatus[0] = OR_RH_PORT_PRSC;
        }
        // Port1: PortEnableStatusChange
        if (ints_port1 & OR_RH_PORT_PESC) {
            USBH->HcRhPortStatus[0] = OR_RH_PORT_PESC;
        }

        // Port2: PortOverCurrentIndicatorChange
        if (ints_port2 & OR_RH_PORT_OCIC) {
            USBH->HcRhPortStatus[1] = OR_RH_PORT_OCIC;
        }

        // Port2: Reset completed
        if (ints_port2 & OR_RH_PORT_PRSC) {
            USBH->HcRhPortStatus[1] = OR_RH_PORT_PRSC;
        }
        // Port2: PortEnableStatusChange
        if (ints_port2 & OR_RH_PORT_PESC) {
            USBH->HcRhPortStatus[1] = OR_RH_PORT_PESC;
        }
        USBH->HcInterruptStatus = OR_INTR_STATUS_RHSC;
    }

    // Writeback Done Head interrupt
    if (ints & OR_INTR_STATUS_WDH) {
        transferCompleted(usb_hcca->DoneHead & 0xFFFFFFFE);
        USBH->HcInterruptStatus = OR_INTR_STATUS_WDH;
    }


}
#endif
