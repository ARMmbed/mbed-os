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

#include "mbed.h"
#include "USBHALHost.h"
#include "dbg.h"
#include "pinmap.h"

#define HCCA_SIZE   sizeof(HCCA)
#define ED_SIZE     sizeof(HCED)
#define TD_SIZE     sizeof(HCTD)

#define TOTAL_SIZE (HCCA_SIZE + (MAX_ENDPOINT*ED_SIZE) + (MAX_TD*TD_SIZE))

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
    
    // NOTE: Configure as OTG device first; otherwise, program will trap in wait loop CLK_STATUS_PLL2STB_Msk below.
    SYS->USBPHY = SYS_USBPHY_LDO33EN_Msk | SYS_USBPHY_USBROLE_ON_THE_GO;
    
    // NOTE: Enable OTG here; otherwise, program will trap in wait loop CLK_STATUS_PLL2STB_Msk below.
    CLK_EnableModuleClock(OTG_MODULE);
    OTG->PHYCTL = (OTG->PHYCTL | OTG_PHYCTL_OTGPHYEN_Msk) & ~OTG_PHYCTL_IDDETEN_Msk;
    //OTG->CTL |= OTG_CTL_OTGEN_Msk | OTG_CTL_BUSREQ_Msk;
    
    // PB.0: USB0 external VBUS regulator status
    // USB_OC
    // PB.1: USB0 external VBUS regulator enable
    // NCT3520U low active (USB_PWR_EN)
    pin_function(PB_0, SYS_GPB_MFPL_PB0MFP_USB0_OTG5V_ST);
    pin_function(PB_1, SYS_GPB_MFPL_PB1MFP_USB0_OTG5V_EN);

    // PB.2: USB1 differential signal D-
    // PB.3: USB1 differential signal D+
    //pin_function(PB_2, SYS_GPB_MFPL_PB2MFP_USB1_D_N);
    //pin_function(PB_3, SYS_GPB_MFPL_PB3MFP_USB1_D_P);

    // Set PB.4 output high to enable USB power
    //gpio_t gpio;
    //gpio_init_out_ex(&gpio, PB_4, 1);
    
    // NOTE:
    // 1. Set USBH clock source to PLL2; otherwise, program will trap in wait loop CLK_STATUS_PLL2STB_Msk below.
    // 2. Don't set CLK_PLL2CTL_PLL2CKEN_Msk. USBH will work abnormally with it enabled.
    CLK->CLKSEL0 &= ~CLK_CLKSEL0_USBHSEL_Msk;
    // Enable PLL2, 480 MHz / 2 / (1+4) => 48 MHz output
    CLK->PLL2CTL = /*CLK_PLL2CTL_PLL2CKEN_Msk | */ (4 << CLK_PLL2CTL_PLL2DIV_Pos);
    // Wait PLL2 stable ...
    while (!(CLK->STATUS & CLK_STATUS_PLL2STB_Msk));
    
    // Select USB Host clock source from PLL2, clock divied by 1
    CLK_SetModuleClock(USBH_MODULE, CLK_CLKSEL0_USBHSEL_PLL2, CLK_CLKDIV0_USB(1));
    
    // Enable USB Host clock
    CLK_EnableModuleClock(USBH_MODULE);
    
    // Lock protected registers
    SYS_LockReg();
    
    // Overcurrent flag is high active
    USBH->HcMiscControl &= ~USBH_HcMiscControl_OCAL_Msk;
    
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

    // FIXME: Ports are power switched. All ports are powered at the same time. Doesn't match BSP sample.
    USBH->HcRhDescriptorA = USBH->HcRhDescriptorA & ~USBH_HcRhDescriptorA_NPS_Msk & ~USBH_HcRhDescriptorA_PSM_Msk;
    // Issue SetGlobalPower command
    USBH->HcRhStatus = USBH_HcRhStatus_LPSC_Msk;
    // Power On To Power Good Time, in 2 ms units
    wait_ms(((USBH->HcRhDescriptorA & USBH_HcRhDescriptorA_POTPGT_Msk) >> USBH_HcRhDescriptorA_POTPGT_Pos) * 2);
    
    // Clear Interrrupt Status
    USBH->HcInterruptStatus |= USBH->HcInterruptStatus;
    // Enable interrupts we care about
    USBH->HcInterruptEnable  = OR_INTR_ENABLE_MIE | OR_INTR_ENABLE_WDH | OR_INTR_ENABLE_RHSC;


    // Unlock protected registers
    SYS_UnlockReg();
    
    // NOTE: Configure as USB host after USBH init above; otherwise system will crash.
    SYS->USBPHY = (SYS->USBPHY & ~SYS_USBPHY_USBROLE_Msk) | SYS_USBPHY_USBROLE_STD_USBH;
    
    // Lock protected registers
    SYS_LockReg();
    
    NVIC_SetVector(USBH_IRQn, (uint32_t)(_usbisr));
    NVIC_EnableIRQ(USBH_IRQn);
    
    // Check for any connected devices
    if (USBH->HcRhPortStatus[0] & OR_RH_PORT_CCS) {
        // Device connected
        wait_ms(150);
        deviceConnected(0, 1, USBH->HcRhPortStatus[0] & OR_RH_PORT_LSDA);
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
            if(USBH->HcControl & OR_CONTROL_BLE){
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
        // Port1: Reset completed
        if (ints_port1 & OR_RH_PORT_PRSC) {
            USBH->HcRhPortStatus[0] = OR_RH_PORT_PRSC;
        }
        // Port1: PortEnableStatusChange
        if (ints_port1 & OR_RH_PORT_PESC) {
            USBH->HcRhPortStatus[0] = OR_RH_PORT_PESC;
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
