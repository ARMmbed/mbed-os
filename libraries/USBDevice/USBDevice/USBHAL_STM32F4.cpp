/* Copyright (c) 2010-2011 mbed.org, MIT License
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

#if defined(TARGET_STM32F407)

#include "USBHAL.h"
#include "USBRegisters_STM32F4.h"

#define OTG ((USB_OTG_FS_BASE_ADDR *) USB_OTG_CORE_REGS)

// Get endpoint direction
#define IN_EP(endpoint)     ((endpoint) & 1U ? true : false)
#define OUT_EP(endpoint)    ((endpoint) & 1U ? false : true)

// Convert physical endpoint number to register bit
#define EP(endpoint) (1UL<<endpoint)

// USB Control register
#define LOG_ENDPOINT(endpoint) ((endpoint>>1)<<2)

USBHAL * USBHAL::instance;

static volatile int epComplete;
static uint32_t endpointStallState;

static uint8_t selectEndpointClearInterrupt(uint8_t endpoint) {
    // Implemented using using EP_INT_CLR.
    LPC_USB->USBEpIntClr = EP(endpoint);
    while (!(LPC_USB->USBDevIntSt & CDFULL));
    return (uint8_t)LPC_USB->USBCmdData;
}


static void enableEndpointEvent(uint8_t endpoint) {
    // Enable an endpoint interrupt
    uint8_t index = endpoint >> 1;
    if (IN_EP(endpoint)) {
        OTG->DREGS->DAINTMSK |= (1 << index);
    }
    else {
        OTG->DREGS->DAINTMSK |= (1 << index << 16);
    }
}

static void disableEndpointEvent(uint8_t endpoint) __attribute__ ((unused));
static void disableEndpointEvent(uint8_t endpoint) {
    // Disable an endpoint interrupt
    uint8_t index = endpoint >> 1;
    if (IN_EP(endpoint)) {
        OTG->DREGS->DAINTMSK &= ~(1 << index);
    }
    else {
        OTG->DREGS->DAINTMSK &= ~(1 << index << 16);
    }

}

static volatile uint32_t __attribute__((used)) dummyRead;
uint32_t USBHAL::endpointReadcore(uint8_t endpoint, uint8_t *buffer) {
    // Read from an OUT endpoint
    uint32_t size;
    uint32_t i;
    uint32_t data = 0;
    uint8_t offset;

    LPC_USB->USBCtrl = LOG_ENDPOINT(endpoint) | RD_EN;
    while (!(LPC_USB->USBRxPLen & PKT_RDY));

    size = LPC_USB->USBRxPLen & PKT_LNGTH_MASK;

    offset = 0;

    if (size > 0) {
        for (i=0; i<size; i++) {
            if (offset==0) {
                // Fetch up to four bytes of data as a word
                data = LPC_USB->USBRxData;
            }

            // extract a byte
            *buffer = (data>>offset) & 0xff;
            buffer++;

            // move on to the next byte
            offset = (offset + 8) % 32;
        }
    } else {
        dummyRead = LPC_USB->USBRxData;
    }

    LPC_USB->USBCtrl = 0;

    if ((endpoint >> 1) % 3 || (endpoint >> 1) == 0) {
        SIEselectEndpoint(endpoint);
        SIEclearBuffer();
    }
    
    return size;
}

static void endpointWritecore(uint8_t endpoint, uint8_t *buffer, uint32_t size) {
    // Write to an IN endpoint
    uint32_t temp, data;
    uint8_t offset;

    LPC_USB->USBCtrl = LOG_ENDPOINT(endpoint) | WR_EN;

    LPC_USB->USBTxPLen = size;
    offset = 0;
    data = 0;

    if (size>0) {
        do {
            // Fetch next data byte into a word-sized temporary variable
            temp = *buffer++;

            // Add to current data word
            temp = temp << offset;
            data = data | temp;

            // move on to the next byte
            offset = (offset + 8) % 32;
            size--;

            if ((offset==0) || (size==0)) {
                // Write the word to the endpoint
                LPC_USB->USBTxData = data;
                data = 0;
            }
        } while (size>0);
    } else {
        LPC_USB->USBTxData = 0;
    }

    // Clear WR_EN to cover zero length packet case
    LPC_USB->USBCtrl=0;

    SIEselectEndpoint(endpoint);
    SIEvalidateBuffer();
}

USBHAL::USBHAL(void) {
    // Disable IRQ
    NVIC_DisableIRQ(OTG_FS_IRQn);
    
    // fill in callback array
    epCallback[0] = &USBHAL::EP1_OUT_callback;
    epCallback[1] = &USBHAL::EP1_IN_callback;
    epCallback[2] = &USBHAL::EP2_OUT_callback;
    epCallback[3] = &USBHAL::EP2_IN_callback;
    epCallback[4] = &USBHAL::EP3_OUT_callback;
    epCallback[5] = &USBHAL::EP3_IN_callback;
    epCallback[6] = &USBHAL::EP4_OUT_callback;
    epCallback[7] = &USBHAL::EP4_IN_callback;

    // Enable power to USB device controller
    RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;

    // Enable power to GPIO port A
    RCC->ABH1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Configure pins PA12 and PA11 to be USB D+ and USB D-
    GPIOA->MODER |= 0xA << 22;
    GPIOA->AFHR |= 0xAA << 12;
    
    // Disconnect USB device
    wait(0.3);

    // Unmask global interrupts and set TX empty level
    OTG->GREGS->GAHBCFG |= (1 << 0) | (1 << 7);
    //                      GINTMSK    TXFELVL

    // Clear the RXFLVL bit in interrupt status
    OTG->GREGS->GINTSTS |= (1 << 4);
    //                      RXFFLSH

    // USB Configuration
    // Disable HNP
    OTG->GREGS->GUSBCFG &= ~(1 << 9);
    //                       HNPCAP
    // Force device mode, and set turnaround time
    OTG->GREGS->GUSBCFG |= (1 << 30) | (0xf << 10);
    //                      FDMOD       TRDT
    
    // Set device speed and non-zero-length status handshake
    OTG->DREGS->DCFG |= (1 << 3) | 0x3;

    // Attach IRQ
    instance = this;
    NVIC_SetVector(OTG_FS_IRQn, (uint32_t)&_usbisr);

    // Enable interrupts for reset and enumeration complete
    OTG->GREGS->GINTMSK |= (1<<12) | (1<<13);

    // Don't use Vbus sensing
    OTG->GREGS->GCCFG |= (1 << 21);
}

USBHAL::~USBHAL(void) {
    // Ensure device disconnected
    OTG->GREGS->GCCFG &= ~(1 << 16);
    // Disable USB interrupts
    NVIC_DisableIRQ(USB_IRQn);
}

void USBHAL::connect(void) {
    NVIC_EnableIRQ(USB_IRQn);
    // Connect USB Device
    OTG->GREGS->GCCFG |= (1 << 16);
}

void USBHAL::disconnect(void) {
    NVIC_DisableIRQ(USB_IRQn);
    // Disconnect USB device
    OTG->GREGS->GCCFG &= ~(1 << 16);
}

void USBHAL::configureDevice(void) {
}

void USBHAL::unconfigureDevice(void) {
}

void USBHAL::setAddress(uint8_t address) {
}

void USBHAL::EP0setup(uint8_t *buffer) {
    endpointReadcore(EP0OUT, buffer);
}

void USBHAL::EP0read(void) {
    // Not required
}

void USBHAL::EP0readStage(void) {
    // Not required
}

uint32_t USBHAL::EP0getReadResult(uint8_t *buffer) {
    return endpointReadcore(EP0OUT, buffer);
}

void USBHAL::EP0write(uint8_t *buffer, uint32_t size) {
    endpointWritecore(EP0IN, buffer, size);
}

void USBHAL::EP0getWriteResult(void) {
    // Not required
}

void USBHAL::EP0stall(void) {
    // This will stall both control endpoints
    stallEndpoint(EP0OUT);
}

EP_STATUS USBHAL::endpointRead(uint8_t endpoint, uint32_t maximumSize) {
    return EP_PENDING;
}

EP_STATUS USBHAL::endpointReadResult(uint8_t endpoint, uint8_t * buffer, uint32_t *bytesRead) {

    //for isochronous endpoint, we don't wait an interrupt
    if ((endpoint >> 1) % 3 || (endpoint >> 1) == 0) {
        if (!(epComplete & EP(endpoint)))
            return EP_PENDING;
    }
    
    *bytesRead = endpointReadcore(endpoint, buffer);
    epComplete &= ~EP(endpoint);
    return EP_COMPLETED;
}

EP_STATUS USBHAL::endpointWrite(uint8_t endpoint, uint8_t *data, uint32_t size) {
    if (getEndpointStallState(endpoint)) {
        return EP_STALLED;
    }

    epComplete &= ~EP(endpoint);

    endpointWritecore(endpoint, data, size);
    return EP_PENDING;
}

EP_STATUS USBHAL::endpointWriteResult(uint8_t endpoint) {
    if (epComplete & EP(endpoint)) {
        epComplete &= ~EP(endpoint);
        return EP_COMPLETED;
    }

    return EP_PENDING;
}

bool USBHAL::realiseEndpoint(uint8_t endpoint, uint32_t maxPacket, uint32_t flags) {
    uint8_t index = endpoint >> 1;
    if (OUT_EP(endpoint)) {
        // Enable the endpoint
        OTG->OUTEP_REGS[index]->DOEPCTL |= (1 << 15);
        // Set the maximum packet size
        if (index == 0) {
            switch (maxPacket) {
                case 32:
                    OTG->OUTEP_REGS[index]->DOEPCTL |= 0x1; break;
                case 16: 
                    OTG->OUTEP_REGS[index]->DOIEPCTL |= 0x2; break;
                case 8:
                    OTG->OUTEP_REGS[index]->DOEPCTL |= 0x3; break;
                default:
                    OTG->OUTEP_REGS[index]->DOEPCTL |= 0x0;
            }

        }
        else {
            OTG->OUTEP_REGS[index]->DOEPCTL |= maxPacket;
        }
        // Set the transfer size and packet count
        OTG->OUTEP_REGS[index]->DOEPTSIZ = (1 << 19) | maxPacket;
        
        // Set the endpoint type
        switch (endpoint) {
            case EPINT_OUT:
                OTG->OUTEP_REGS[index]->DOEPCTL |= (0x3 << 18);
                break;
            case EPBULK_OUT:
                OTG->OUTEP_REGS[index]->DOEPCTL |= (0x2 << 18);
                break;
            case EPISO_OUT:
                OTG->OUTEP_REGS[index]->DOEPCTL |= (0x1 << 18);
                break;
        }
    }
    if (IN_EP(endpoint)) {
        // Enable the endpoint
        OTG->INEP_REGS[index]->DIEPCTL |= (1 << 15);
        // Set the maximum packet size
        if (index == 0) {
            switch (maxPacket) {
                case 32:
                    OTG->INEP_REGS[index]->DIEPCTL |= 0x1; break;
                case 16:
                    OTG->INEP_REGS[index]->DIEPCTL |= 0x2; break;
                case 8:
                    OTG->INEP_REGS[index]->DIEPCTL |= 0x3; break;
                default:
                    OTG->INEP_REGS[index]->DIEPCTL |= 0x0;
            }
        }
        else {
            OTG->INEP_REGS[index]->DIEPCTL |= maxPacket;
        }
        // Set the transfer size and packet count
        OTG->INEP_REGS[index]->DIEPTSIZ = (1 << 19) | maxPacket;

        // Set the endpoint type
        switch (endpoint) {
            case EPINT_OUT:
                OTG->INEP_REGS[index]->DIEPCTL |= (0x3 << 18);
                break;
            case EPBULK_OUT: 
                OTG->INEP_REGS[index]->DIEPCTL |= (0x2 << 18);
                break;
            case EPISO_OUT: 
                OTG->INEP_REGS[index]->DIEPCTL |= (0x1 << 18);
                break;
        }
    }

    enableEndpointEvent(endpoint);
    return true;
}

void USBHAL::stallEndpoint(uint8_t endpoint) {
    // Stall an endpoint
    if ( (endpoint==EP0IN) || (endpoint==EP0OUT) ) {
        // Conditionally stall both control endpoints
        SIEsetEndpointStatus(EP0OUT, SIE_SES_CND_ST);
    } else {
        SIEsetEndpointStatus(endpoint, SIE_SES_ST);

        // Update stall state
        endpointStallState |= (1 << endpoint);
    }
}

void USBHAL::unstallEndpoint(uint8_t endpoint) {
    // Unstall an endpoint. The endpoint will also be reinitialised
    SIEsetEndpointStatus(endpoint, 0);

    // Update stall state
    endpointStallState &= ~(1 << endpoint);
}

bool USBHAL::getEndpointStallState(uint8_t endpoint) {
    // Returns true if endpoint stalled
    return endpointStallState & (1 << endpoint);
}

void USBHAL::remoteWakeup(void) {
    // Remote wakeup
    uint8_t status;

    // Enable USB clocks
    LPC_USB->USBClkCtrl |= DEV_CLK_EN | AHB_CLK_EN;
    while (LPC_USB->USBClkSt != (DEV_CLK_ON | AHB_CLK_ON));

    status = SIEgetDeviceStatus();
    SIEsetDeviceStatus(status & ~SIE_DS_SUS);
}

void USBHAL::_usbisr(void) {
    instance->usbisr();
}


void USBHAL::usbisr(void) {
    uint32_t status = OTG->GREGS->GINTSTS;

    if(status & (1<<12)) {
        // USB Reset
        // Send NACK on all out endpoints
        OTG->OUTEP_REGS[0]->DOEPCTL |= (1 << 27);
        OTG->OUTEP_REGS[1]->DOEPCTL |= (1 << 27);
        OTG->OUTEP_REGS[2]->DOEPCTL |= (1 << 27);
        OTG->OUTEP_REGS[3]->DOEPCTL |= (1 << 27);

        realiseEndpoint(EP0_IN, MAX_PACKET_SIZE_EP0, 0);
        realiseEndpoint(EP0_OUT, MAX_PACKET_SIZE_EP0, 0);

        // Unmask setup done, transfer complete
        OTG.DOEPMSK = (1<<3) | (1<<0);
        // Unmask timeout mask, transfer complete
        OTG.DIEPMSK = (1<<3) | (1<<0);

        // Setup data FIFOs
        OTG->GREGS->GRXFSIZ = MAX_PACKET_SIZE_EP0 + 2 + 10;
        OTG->GREGS->DIEPTXF0_HNPTXFSIZ = (MAX_PACKET_SIZE_EP0 << 16) |
                                         (MAX_PACKET_SIZE_EP0 + 2 + 10);
        
        // Expect three setup packets
        OTG->OUTEPREGS[0]->DOEPTSIZ |= (3 << 29)
    }
    else if (status & (1 << 13)) {
        // Enumeration completed
        // Reset the status register
        OTG->GREGS->GINTSTS = (1 << 13);
    }

/* TODO: Suspend status
    if (LPC_USB->USBDevIntSt & DEV_STAT) {
        // Device Status interrupt
        // Must clear the interrupt status flag before reading the device status from the SIE
        LPC_USB->USBDevIntClr = DEV_STAT;

        // Read device status from SIE
        devStat = SIEgetDeviceStatus();
        //printf("devStat: %d\r\n", devStat);

        if (devStat & SIE_DS_SUS_CH) {
            // Suspend status changed
            if((devStat & SIE_DS_SUS) != 0) {
                suspendStateChanged(0);
            }
        }

        if (devStat & SIE_DS_RST) {
            // Bus reset
            if((devStat & SIE_DS_SUS) == 0) {
                suspendStateChanged(1);
            }
            busReset();
        }
    }

*/
    else if (status & (1 << 4)) {
        // RX FIFO not empty

        // Read and pop the status
        uint32_t status = OTG->GREGS->GRXSTSP;
        uint32_t endpoint = status & 0xF; // Index of the out endpoint
        uint32_t count = (status & 0x7ff0) >> 4;
        uint32_t type = (status >> 17) & 0xF;

        // Disable receive interrupt
        OTG->GREGS->GINTMSK &= ~(1 << 4);

        if (endpoint == 0) {
            if (type == 0x6) {
                // Setup packet
                EP0setupCallback();
            }
            else {
                EP0out();
            }
        }
        

        if (type == 0x2 && count) {
        }

        
        // Re-enable recieve interrupt
        OTG->GREGS->GINTMSK &= ~(1 << 4);

    if (LPC_USB->USBDevIntSt & EP_SLOW) {
        // (Slow) Endpoint Interrupt

        // Process each endpoint interrupt
        if (LPC_USB->USBEpIntSt & EP(EP0OUT)) {
            if (selectEndpointClearInterrupt(EP0OUT) & SIE_SE_STP) {
                // this is a setup packet
                EP0setupCallback();
            } else {
                EP0out();
            }
            LPC_USB->USBDevIntClr = EP_SLOW;
        }

        if (LPC_USB->USBEpIntSt & EP(EP0IN)) {
            selectEndpointClearInterrupt(EP0IN);
            LPC_USB->USBDevIntClr = EP_SLOW;
            EP0in();
        }
        
        for (uint8_t num = 2; num < 16*2; num++) {
            if (LPC_USB->USBEpIntSt & EP(num)) {
                selectEndpointClearInterrupt(num);
                epComplete |= EP(num);
                LPC_USB->USBDevIntClr = EP_SLOW;
                if ((instance->*(epCallback[num - 2]))()) {
                    epComplete &= ~EP(num);
                }
            }
        }
    }
}

#endif
