/* Copyright (c) 2010-2012 mbed.org, MIT License
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

#define __DEBUG__ 0 //WARN: ENABLING DEBUGGING HERE WILL PRINTF IN IRQS!! UNEXPECTED BEHAVIOUR MAY RESULT...
#ifndef __MODULE__
#define __MODULE__ "USBHALHost.cpp"
#endif

#include "core/dbg.h"
#include <cstdint>

#include "mbed.h"
#include "USBHALHost.h"

// bits of the USB/OTG clock control register
#define HOST_CLK_EN     (1<<0)
#define DEV_CLK_EN      (1<<1)
#define PORTSEL_CLK_EN  (1<<3)
#define AHB_CLK_EN      (1<<4)

// bits of the USB/OTG clock status register
#define HOST_CLK_ON     (1<<0)
#define DEV_CLK_ON      (1<<1)
#define PORTSEL_CLK_ON  (1<<3)
#define AHB_CLK_ON      (1<<4)

// we need host clock, OTG/portsel clock and AHB clock
#define CLOCK_MASK (HOST_CLK_EN | PORTSEL_CLK_EN | AHB_CLK_EN)



#define HCCA_SIZE 0x100
#define ED_SIZE 0x10
#define TD_SIZE 0x10

#define TOTAL_SIZE (HCCA_SIZE + (MAX_ENDPOINT*ED_SIZE) + (MAX_TD*TD_SIZE))

static volatile uint8_t usb_buf[TOTAL_SIZE] __attribute((section("AHBSRAM1"),aligned(256)));  //256 bytes aligned!

USBHALHost * USBHALHost::instHost;

USBHALHost::USBHALHost() : thread(USBHALHost::staticCb, (void*)this, osPriorityNormal, 4*128) {
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


void USBHALHost::init() {
  thread.signal_set(USBHALHOST_SIG_INIT);
}


uint32_t USBHALHost::controlHeadED() {
    return LPC_USB->HcControlHeadED;
}

uint32_t USBHALHost::bulkHeadED() {
    return LPC_USB->HcBulkHeadED;
}

uint32_t USBHALHost::interruptHeadED() {
    return usb_hcca->IntTable[0];
}

void USBHALHost::updateBulkHeadED(uint32_t addr) {
    LPC_USB->HcBulkHeadED = addr;
}


void USBHALHost::updateControlHeadED(uint32_t addr) {
    LPC_USB->HcControlHeadED = addr;
}

void USBHALHost::updateInterruptHeadED(uint32_t addr) {
    usb_hcca->IntTable[0] = addr;
}


void USBHALHost::enableControlList() {
    LPC_USB->HcCommandStatus = OR_CMD_STATUS_CLF;
    LPC_USB->HcControl       |= OR_CONTROL_CLE; //Enable control list
}

void USBHALHost::enableBulkList() {
    LPC_USB->HcCommandStatus = OR_CMD_STATUS_BLF;
    LPC_USB->HcControl       |= OR_CONTROL_BLE; //Enable bulk list
}

void USBHALHost::enableInterruptList() {
    LPC_USB->HcControl |= OR_CONTROL_PLE;
}

bool USBHALHost::disableControlList() {
    if(LPC_USB->HcControl & OR_CONTROL_CLE)
    {
      LPC_USB->HcControl       &= ~OR_CONTROL_CLE; //Disable control list
      return true;
    }
    else
    {
      return false;
    }
}

bool USBHALHost::disableBulkList() {
    if(LPC_USB->HcControl & OR_CONTROL_BLE)
    {
      LPC_USB->HcControl       &= ~OR_CONTROL_BLE; //Disable bulk list
      return true;
    }
    else
    {
      return false;
    }
}

bool USBHALHost::disableInterruptList() {
    if(LPC_USB->HcControl & OR_CONTROL_PLE)
    {
      LPC_USB->HcControl       &= ~OR_CONTROL_PLE; //Disable interrupt list
      return true;
    }
    else
    {
      return false;
    }
}

//Lock processing
void USBHALHost::lock()
{
  mtx.lock();
}

void USBHALHost::unlock()
{
  mtx.unlock();
}

void USBHALHost::memInit() {
    usb_hcca = (volatile HCCA *)usb_buf;
    usb_edBuf = usb_buf + HCCA_SIZE;
    usb_tdBuf = usb_buf + HCCA_SIZE + (MAX_ENDPOINT*ED_SIZE);
}

volatile uint8_t * USBHALHost::getED() {
    for (int i = 0; i < MAX_ENDPOINT; i++) {
        if ( !edBufAlloc[i] ) {
            edBufAlloc[i] = true;
            return (volatile uint8_t *)(usb_edBuf + i*ED_SIZE);
        }
    }
    perror("Could not allocate ED\r\n");
    return NULL; //Could not alloc ED
}

volatile uint8_t * USBHALHost::getTD() {
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


void USBHALHost::freeED(volatile uint8_t * ed) {
    int i;
    i = (ed - usb_edBuf) / ED_SIZE;
    edBufAlloc[i] = false;
}

void USBHALHost::freeTD(volatile uint8_t * td) {
    int i;
    i = (td - usb_tdBuf) / TD_SIZE;
    tdBufAlloc[i] = false;
}


void USBHALHost::resetPort(int hub, int port) {
    DBG("Resetting hub %d, port %d\n", hub, port);
    if (hub == 0) { //Root hub
        // USB 2.0 spec says at least 50ms delay before port reset
        Thread::wait(200);
        LPC_USB->HcRhPortStatus1 = OR_RH_PORT_PRS; // Initiate port reset
        while (LPC_USB->HcRhPortStatus1 & OR_RH_PORT_PRS);
        LPC_USB->HcRhPortStatus1 = OR_RH_PORT_PRSC; // ...and clear port reset signal
        // Wait for 100 MS after port reset
        Thread::wait(200);
    } else {
        //TODO: Hubs
    }
}


void USBHALHost::_usbisr(void) {
    if (instHost) {
        instHost->UsbIrqhandler();
    }
}

void USBHALHost::UsbIrqhandler() {
  if( LPC_USB->HcInterruptStatus & LPC_USB->HcInterruptEnable ) //Is there something to actually process?
  {
    NVIC_DisableIRQ(USB_IRQn);
    NVIC_ClearPendingIRQ(USB_IRQn);
    thread.signal_set(USBHALHOST_SIG_IRQ); //Signal processing thread
  }

}

void USBHALHost::process()
{
  DBG("USB Process started");

  lock();
  Thread::signal_wait(USBHALHOST_SIG_INIT);

  NVIC_DisableIRQ(USB_IRQn);                           // Disable the USB interrupt source

  LPC_SC->PCONP       &= ~(1UL<<31); //Cut power
  Thread::wait(200);

  // turn on power for USB
  LPC_SC->PCONP       |= (1UL<<31);

  // Enable USB host clock, port selection and AHB clock
  LPC_USB->USBClkCtrl |= CLOCK_MASK;

  // Wait for clocks to become available
  while ((LPC_USB->USBClkSt & CLOCK_MASK) != CLOCK_MASK);

  // it seems the bits[0:1] mean the following
  // 0: U1=device, U2=host
  // 1: U1=host, U2=host
  // 2: reserved
  // 3: U1=host, U2=device
  // NB: this register is only available if OTG clock (aka "port select") is enabled!!
  // since we don't care about port 2, set just bit 0 to 1 (U1=host)
  LPC_USB->OTGStCtrl |= 1;

  // now that we've configured the ports, we can turn off the portsel clock
  LPC_USB->USBClkCtrl &= ~PORTSEL_CLK_EN;

  // configure USB D+/D- pins
  // P0[29] = USB_D+, 01
  // P0[30] = USB_D-, 01
  LPC_PINCON->PINSEL1 &= ~((3<<26) | (3<<28));
  LPC_PINCON->PINSEL1 |=  ((1<<26)|(1<<28));     // 0x14000000

  LPC_USB->HcControl       = 0;                       // HARDWARE RESET
  LPC_USB->HcControlHeadED = 0;                       // Initialize Control list head to Zero
  LPC_USB->HcBulkHeadED    = 0;                       // Initialize Bulk list head to Zero

  //wait_ms(100);                                   // Wait 50 ms before apply reset
  Thread::wait(100);

  // SOFTWARE RESET
  LPC_USB->HcCommandStatus = OR_CMD_STATUS_HCR;
  LPC_USB->HcFmInterval    = DEFAULT_FMINTERVAL;      // Write Fm Interval and Largest Data Packet Counter
  LPC_USB->HcPeriodicStart = FI * 90 / 100;

  // Put HC in operational state
  LPC_USB->HcControl  = (LPC_USB->HcControl & (~OR_CONTROL_HCFS)) | OR_CONTROL_HC_OPER;
  LPC_USB->HcRhStatus = OR_RH_STATUS_LPSC;            // Set Global Power

  LPC_USB->HcHCCA = (uint32_t)(usb_hcca);
  LPC_USB->HcInterruptStatus |= LPC_USB->HcInterruptStatus;                   // Clear Interrrupt Status

  LPC_USB->HcInterruptEnable  = OR_INTR_ENABLE_MIE | OR_INTR_ENABLE_WDH | OR_INTR_ENABLE_RHSC;

  //DG: Do not set prio
  //NVIC_SetPriority(USB_IRQn, 0);       // highest priority
  // Enable the USB Interrupt
  NVIC_SetVector(USB_IRQn, (uint32_t)(_usbisr));
  LPC_USB->HcRhPortStatus1 = OR_RH_PORT_CSC;
  LPC_USB->HcRhPortStatus1 = OR_RH_PORT_PRSC;

  NVIC_EnableIRQ(USB_IRQn);


  // Check for any connected devices
  if (LPC_USB->HcRhPortStatus1 & OR_RH_PORT_CCS) { //Root device connected
      //Device connected
      Thread::wait(500);
      DBG("Device connected (%08x)\n\r", LPC_USB->HcRhPortStatus1);
      deviceConnected(0, 1, LPC_USB->HcRhPortStatus1 & OR_RH_PORT_LSDA); //Hub 0 (root hub), Port 1 (count starts at 1), Low or High speed
  }

  unlock();


  for(;;)
  {
    Thread::signal_wait(USBHALHOST_SIG_IRQ); //Wait for IRQ to process

    lock();
    DBG("Locked");

    WARN("isr %08x [EN %08x]", LPC_USB->HcInterruptStatus, LPC_USB->HcInterruptEnable);

    //Now process IRQ
    uint32_t int_status = LPC_USB->HcInterruptStatus & LPC_USB->HcInterruptEnable;

    if (int_status & OR_INTR_STATUS_RHSC)
    { // Root hub status change interrupt
      WARN("Port status %08x", LPC_USB->HcRhPortStatus1);
      if (LPC_USB->HcRhPortStatus1 & OR_RH_PORT_CSC)
      {
        if (LPC_USB->HcRhStatus & OR_RH_STATUS_DRWE)
        {

          // When DRWE is on, Connect Status Change
          // means a remote wakeup event.

        }
        else
        {

          // When DRWE is off, Connect Status Change
          // is NOT a remote wakeup event

          if (LPC_USB->HcRhPortStatus1 & OR_RH_PORT_CCS)
          { //Root device connected
            //Device connected
            WARN("Device connected!!");
           // Thread::wait(500);
            deviceConnected(0, 1, LPC_USB->HcRhPortStatus1 & OR_RH_PORT_LSDA); //Hub 0 (root hub), Port 1 (count starts at 1), Low or High speed
          }
          else
          { //Root device disconnected
            //Device disconnected
            WARN("Device disconnected!!");
            Thread::wait(500);
            if (!(int_status & OR_INTR_STATUS_WDH))
            {
              usb_hcca->DoneHead = 0;
            }
            deviceDisconnected(0, 1, usb_hcca->DoneHead & 0xFFFFFFFE);
            if (int_status & OR_INTR_STATUS_WDH)
            {
              usb_hcca->DoneHead = 0;
              LPC_USB->HcInterruptStatus = OR_INTR_STATUS_WDH;
            }

          }
          //TODO: HUBS
        }
        LPC_USB->HcRhPortStatus1 = OR_RH_PORT_CSC;
      }
      if (LPC_USB->HcRhPortStatus1 & OR_RH_PORT_PRSC)
      {
        LPC_USB->HcRhPortStatus1 = OR_RH_PORT_PRSC;
        //int_status &= ~OR_RH_PORT_PRSC;
      }
      LPC_USB->HcInterruptStatus = OR_INTR_STATUS_RHSC;
    }

    if (int_status & OR_INTR_STATUS_WDH)
    { // Writeback Done Head interrupt
      transferCompleted(usb_hcca->DoneHead & 0xFFFFFFFE);
      LPC_USB->HcInterruptStatus = OR_INTR_STATUS_WDH;
    }

    //IRQ Processed

    DBG("Unlocked");

    NVIC_EnableIRQ(USB_IRQn);

    unlock();

  }
}

/*static*/ void USBHALHost::staticCb(void const* p)
{
  ((USBHALHost*)p)->process();
}
