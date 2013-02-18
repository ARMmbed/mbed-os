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
#define __MODULE__ "USBHost.cpp"
#endif

#include "core/dbg.h"
#include <cstdint>

#include "USBHost.h"
#include "rtos.h"


#define NB_MAX_INTF 2

USBHost * USBHost::instHost = NULL;

USBHost::USBHost()
#if 0 //try not to use this
    : m_usbQueue(), m_usbThread(3, this, &USBHost::usbProcess)
#endif
{
  headControlEndpoint = NULL;
  headBulkEndpoint = NULL;
  headInterruptEndpoint = NULL;
  tailControlEndpoint = NULL;
  tailBulkEndpoint = NULL;
  tailInterruptEndpoint = NULL;

  nb_devices = 0;
  lenReportDescr = 0;

  controlEndpointAllocated = false;

  for (int i = 0; i < MAX_DEVICE_NB; i++) {
    deviceInUse[i] = false;
    devices[i].setAddress(i + 1);
    deviceReset[i] = false;
  }
}


void USBHost::transferCompleted(volatile uint32_t addr)  {
#if 0 //try not to use this
  Interrupt::enter();
  m_usbQueue.post(addr);
  Interrupt::leave();
#else

  if(addr == NULL) //Nothing to process?
  {
    return;
  }

  volatile HCTD* tdList = NULL;

  //First we must reverse the list order and dequeue each TD
  do
  {
    volatile HCTD* td = (volatile HCTD*)addr;

    if(td->control & 0xF0000000 != 0)
    {
      WARN("Condition code %02x", td->control >> 28);
    }

    addr = td->nextTD; //Dequeue from physical list
    td->nextTD = (uint32_t)tdList; //Enqueue into reversed list
    tdList = td;
  } while(addr);

  //Now we can process the list
  USBEndpoint * volatile iter = NULL;

  while(tdList != NULL)
  {
    bool found = false;
    volatile HCTD* td = tdList;
    tdList = (volatile HCTD*)td->nextTD; //Dequeue element now as it could be modified below
    for (int i = 0; i < 3; i++) {
      if (found) {
        break;
      }
      iter = (i == 0) ? headControlEndpoint : ( (i == 1) ? headBulkEndpoint : headInterruptEndpoint);
      while (iter != NULL) {
        if (iter->getProcessedTD() == td) {
          DBG("td=%p FOUND ed: %08X", td, (void *)iter->getHCED());
          if (((HCTD *)td)->control >> 28) {
            DBG("TD Error: %d", td->control >> 28);
            iter->setState(USB_TYPE_TDFAIL);
          } else if ((uint32_t)iter->getHCED() & 0x1) {
            DBG("HALTED");
            iter->setState(USB_TYPE_HALTED);
          } else if (!td->currBufPtr) {
            DBG("!%p", iter);
            iter->setState(USB_TYPE_IDLE);
            found=true;
          } else {
            DBG("!%p", iter);
            iter->setState(USB_TYPE_IDLE);
            iter->setLengthTransferred(td->currBufPtr - iter->getBufStart());
            found=true;
          }
          break;
        }
        iter = iter->nextEndpoint();
      }
    }


    if (found) {
      iter->unqueueTransfer(td);

      if (iter->getType() != CONTROL_ENDPOINT) {
        iter->call();
      }
    }
    else
    {
      WARN("TD not found!!!");
      freeTD((uint8_t *)td); //Device must have been disconnected meanwhile
    }

  }
#endif
}

USBHost * USBHost::getHostInst() {
  if (instHost == NULL) {
    instHost = new USBHost();
    instHost->init();
  }
  return instHost;
}


/*
 * Call in ISR when a device has been connected
 */
void USBHost::deviceConnected(int hub, int port, bool lowSpeed)  {

  for (int i = 0; i < MAX_DEVICE_NB; i++) {
    if (!deviceInUse[i]) {
      deviceInUse[i] = true;
      WARN("will call init on device %p: speed: %d", (void *)&devices[i], lowSpeed);
      devices[i].init(hub, port, lowSpeed);
      deviceReset[i] = false;
      break;
    }
  }

  if (!controlEndpointAllocated) {
    control = newEndpoint(CONTROL_ENDPOINT, OUT, 0x08, 0x00);
    addEndpoint(NULL, 0, (USBEndpoint*)control);
    controlEndpointAllocated = true;
  }
}

/*
 * Call in ISR when a device has been disconnected
 */
void USBHost::deviceDisconnected(int hub, int port, volatile uint32_t addr)  {

  bool controlListState = disableControlList();
  bool bulkListState = disableBulkList();
  bool interruptListState = disableInterruptList();

  transferCompleted(addr); //Finish processing any pending completed TD

  for (int i = 0; i < MAX_DEVICE_NB; i++) {
    if ((devices[i].getHub() == hub) && (devices[i].getPort() == port)) {
      WARN("device disconnected: %p", (void *)&devices[i]);
      deviceInUse[i] = false;
      deviceReset[i] = false;
      freeDevice((USBDeviceConnected*)&devices[i]);
      break;
    }
  }
  nb_devices--;

  if (controlListState) enableControlList();
  if (bulkListState) enableBulkList();
  if (interruptListState) enableInterruptList();
}

void USBHost::freeDevice(USBDeviceConnected * dev)  {
  USBEndpoint * ep = NULL;
//  HCTD * td = NULL;
  HCED * ed = NULL;

  for (int j = 0; j < dev->getNbInterface(); j++) {
    DBG("FREE INTF %d, %p, nb_endpot: %d", j, (void *)dev->getInterface(j), dev->getInterface(j)->nb_endpoint);
    for (int i = 0; i < dev->getInterface(j)->nb_endpoint; i++) {
      if ((ep = dev->getEndpoint(j, i)) != NULL) {
        DBG("Freeing USBEndpoint");
        ed = (HCED *)ep->getHCED();
        ed->control |= (1 << 13); //sKip bit
        DBG("Dequeueing USBEndpoint");
        unqueueEndpoint(ep);

        DBG("Freeing first transfer descriptor");
        freeTD((volatile uint8_t*)ep->getTDList()[0]);
        DBG("Freeing second transfer descriptor");
        freeTD((volatile uint8_t*)ep->getTDList()[1]);

        DBG("Freeing USBEndpoint descriptor");
        freeED((uint8_t *)ep->getHCED());
      }
      //printBulk();
      //printInt();
    }
  }
  DBG("Disconnecting device");
  dev->disconnect();
  DBG("Device disconnected");
}


void USBHost::unqueueEndpoint(USBEndpoint * ep)  {
  USBEndpoint * prec = NULL;
  USBEndpoint * current = NULL;
  bool found = false;

  DBG("want to unqueue ep: %p", (void *)ep->getHCED());

  for (int i = 0; i < 2; i++) {
    if (found) {
      DBG("USBEndpoint unqueued: %p", (void *)ep->getHCED());
      break;
    }
    current = (i == 0) ? (USBEndpoint*)headBulkEndpoint : (USBEndpoint*)headInterruptEndpoint;
    prec = current;
    while (current != NULL) {
      if (current == ep) {
        if (current->nextEndpoint() != NULL) {
          prec->queueEndpoint(current->nextEndpoint());
          if (current == headBulkEndpoint) {
            updateBulkHeadED((uint32_t)current->nextEndpoint()->getHCED());
            headBulkEndpoint = current->nextEndpoint();
          }
          if (current == headInterruptEndpoint) {
            updateInterruptHeadED((uint32_t)current->nextEndpoint()->getHCED());
            headInterruptEndpoint = current->nextEndpoint();
          }
        } else {
          prec->queueEndpoint(NULL);
          if (current == headBulkEndpoint) {
            updateBulkHeadED(0);
            headBulkEndpoint = current->nextEndpoint();
          }
          if (current == headInterruptEndpoint) {
            updateInterruptHeadED(0);
            headInterruptEndpoint = current->nextEndpoint();
          }
        }
        found = true;
        current->setState(USB_TYPE_FREE);
        break;
      }
      prec = current;
      current = current->nextEndpoint();
    }
  }
  //printBulk();
  //printInt();
}


USBDeviceConnected * USBHost::getDevice(uint8_t index)  {
  if ((index >= MAX_DEVICE_NB) || (!deviceInUse[index])) {
    return NULL;
  }
  return (USBDeviceConnected*)&devices[index];
}



// create an USBEndpoint descriptor. the USBEndpoint is not linked
USBEndpoint * USBHost::newEndpoint(ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir, uint32_t size, uint8_t addr)  {
  int i = 0;
  HCED * ed = (HCED *)getED();
  HCTD* td_list[2] = { (HCTD*)getTD(), (HCTD*)getTD() };

  memset((void *)td_list[0], 0x00, sizeof(HCTD));
  memset((void *)td_list[1], 0x00, sizeof(HCTD));

  // search a free USBEndpoint
  for (i = 0; i < MAX_ENDPOINT; i++) {
    if (endpoints[i].getState() == USB_TYPE_FREE) {
      DBG("Trying to create ep");
      endpoints[i].init(ed, type, dir, size, addr, td_list);
      //endpoints[i].queueTransfer(nullTd);
      DBG("USBEndpoint created (%p): type: %d, dir: %d, size: %d, addr: %d", &endpoints[i], type, dir, size, addr);
      return &endpoints[i];
    }
  }
  DBG("could not allocate more endpoints!!!!");
  return NULL;
}


USB_TYPE USBHost::resetDevice(USBDeviceConnected * dev)  {
  int index = findDevice(dev);
  if ((index != -1) && (!deviceReset[index])) {
    resetPort(dev->getHub(), dev->getPort());
    deviceReset[index] = true;
    return USB_TYPE_OK;
  }
  return USB_TYPE_NOTFOUND;
}

// link the USBEndpoint to the linked list and attach an USBEndpoint to a device
bool USBHost::addEndpoint(USBDeviceConnected * dev, uint8_t intf_nb, USBEndpoint * ep)  {

  if (ep == NULL) {
    return false;
  }

  DBG("New ep %p", ep);

   HCED * prevEd;

  // set device address in the USBEndpoint descriptor
  if (dev == NULL) {
    ep->setDeviceAddress(0);
  } else {
    ep->setDeviceAddress(dev->getAddress());
  }

  if (dev != NULL && dev->getSpeed()) {
    DBG("add USBEndpoint: set speed");
    ep->setSpeed(dev->getSpeed());
  }

  // queue the new USBEndpoint on the ED list
  switch (ep->getType()) {

  case CONTROL_ENDPOINT:
    prevEd = ( HCED*) controlHeadED();
    if (!prevEd) {
      updateControlHeadED((uint32_t) ep->getHCED());
      DBG("First control USBEndpoint: %08X", (uint32_t) ep->getHCED());
      headControlEndpoint = ep;
      tailControlEndpoint = ep;
      return true;
    }
    tailControlEndpoint->queueEndpoint(ep);
    tailControlEndpoint = ep;
    return true;

  case BULK_ENDPOINT:
    prevEd = ( HCED*) bulkHeadED();
    if (!prevEd) {
      updateBulkHeadED((uint32_t) ep->getHCED());
      //DBG("First bulk USBEndpoint: %08X\r\n", (uint32_t) ep->getHCED());
      headBulkEndpoint = ep;
      tailBulkEndpoint = ep;
      break;
    }
    tailBulkEndpoint->queueEndpoint(ep);
    tailBulkEndpoint = ep;
    break;

  case INTERRUPT_ENDPOINT:
    prevEd = ( HCED*) interruptHeadED();
    if (!prevEd) {
      updateInterruptHeadED((uint32_t) ep->getHCED());
      //DBG("First interrupt USBEndpoint: %08X\r\n", (uint32_t) ep->getHCED());
      headInterruptEndpoint = ep;
      tailInterruptEndpoint = ep;
      break;
    }
    tailInterruptEndpoint->queueEndpoint(ep);
    tailInterruptEndpoint = ep;
    break;
  default:
    return false;
  }

  dev->addEndpoint(intf_nb, ep);
  //printBulk();
  //printInt();

  return true;
}


int USBHost::findDevice(USBDeviceConnected * dev)  {
  for (int i = 0; i < MAX_DEVICE_NB; i++) {
    if (dev == &devices[i]) {
      return i;
    }
  }
  return -1;
}

void USBHost::printBulk() {
  HCED * hced = (HCED *)bulkHeadED();
  HCTD * hctd = NULL;
  printf("---------State of Bulk:--------\r\n");
  while (hced != NULL) {
    printf("hced: %p\r\n", hced);
    hctd = (HCTD *)((uint32_t)(hced->headTD) & ~(0x0f));
    while (((uint32_t)hctd & ~(0x0f)) != ((hced->tailTD) & ~(0x0f))) {
      printf("\thctd: %p\r\n", hctd);
      hctd = (HCTD *)((uint32_t)(hctd->nextTD) & ~(0x0f));
    }
    printf("\thctd: %p\r\n", hctd);
    hced = (HCED *)((uint32_t)(hced->nextED) & ~(0x0f));
  }
  printf("--------------------\r\n");
}

void USBHost::printInt() {
  HCED * hced = (HCED *)interruptHeadED();
  HCTD * hctd = NULL;
  printf("---------State of Int:--------\r\n");
  while (hced != NULL) {
    printf("hced: %p\r\n", hced);
    hctd = (HCTD *)((uint32_t)(hced->headTD) & ~(0x0f));
    while (((uint32_t)hctd & ~(0x0f)) != ((hced->tailTD) & ~(0x0f))) {
      printf("\thctd: %p\r\n", hctd);
      hctd = (HCTD *)((uint32_t)(hctd->nextTD) & ~(0x0f));
    }
    printf("\thctd: %p\r\n", hctd);
    hced = (HCED *)((uint32_t)(hced->nextED) & ~(0x0f));
  }
  printf("--------------------\r\n");
}


// add a transfer on the TD linked list
USB_TYPE USBHost::addTransfer(USBEndpoint * ed, uint8_t * buf, uint32_t len)  {

  // allocate a TD which will be freed in TDcompletion
  volatile HCTD * td = ed->getNextTD();
  if (td == NULL) {
    return USB_TYPE_ERROR;
  }

  DBG("Next td = %p",td);

  uint32_t token = (ed->isSetup() ? TD_SETUP : ( (ed->getDir() == IN) ? TD_IN : TD_OUT ));

  uint32_t td_toggle;

  if (ed->getType() == CONTROL_ENDPOINT) {
    if (ed->isSetup()) {
      td_toggle = TD_TOGGLE_0;
    } else {
      td_toggle = TD_TOGGLE_1;
    }
  } else {
    td_toggle = 0;
  }

  DBG("Buf=%d, len=%d", buf, len);
  td->control      = (TD_ROUNDING | token | TD_DELAY_INT(0) | td_toggle | TD_CC);
  td->currBufPtr   = (uint32_t) buf;
  td->bufEnd       = (uint32_t)(buf + (len - 1));

  DBG("Now do queue transfer on ep %p", ed);

  ed->queueTransfer();

  DBG("Enable list if needed");

  switch (ed->getType()) {
  case CONTROL_ENDPOINT:
    enableControlList();
    break;
  case BULK_ENDPOINT:
    enableBulkList();
    break;
  case INTERRUPT_ENDPOINT:
    //printInt();
    enableInterruptList();
    break;
  }

  DBG("Wait for HC to process TD");


  return USB_TYPE_PROCESSING;
}



USB_TYPE USBHost::getDeviceDescriptor(USBDeviceConnected * dev, uint8_t * buf)  {
  return controlRead(  dev,
      USB_DEVICE_TO_HOST | USB_RECIPIENT_DEVICE,
      GET_DESCRIPTOR,
      (DEVICE_DESCRIPTOR << 8) | (0),
      0,
      buf,
      DEVICE_DESCRIPTOR_LENGTH);
}

USB_TYPE USBHost::getConfigurationDescriptor(USBDeviceConnected * dev, uint8_t * buf, uint16_t * len_conf_descr)  {
  USB_TYPE res;
  uint16_t total_conf_descr_length = 0;

  // fourth step: get the beginning of the configuration descriptor to have the total length of the conf descr
  res = controlRead(  dev,
      USB_DEVICE_TO_HOST | USB_RECIPIENT_DEVICE,
      GET_DESCRIPTOR,
      (CONFIGURATION_DESCRIPTOR << 8) | (0),
      0,
      buf,
      CONFIGURATION_DESCRIPTOR_LENGTH);

  if (res != USB_TYPE_OK) {
    ERR("GET CONF 1 DESCR FAILED");
    return res;
  }
  total_conf_descr_length = buf[2] | (buf[3] << 8);
  if (len_conf_descr != NULL) {
    *len_conf_descr = total_conf_descr_length;
  }
  DBG("TOTAL_LENGTH: %d \t NUM_INTERF: %d", total_conf_descr_length, buf[4]);

  return controlRead(  dev,
      USB_DEVICE_TO_HOST | USB_RECIPIENT_DEVICE,
      GET_DESCRIPTOR,
      (CONFIGURATION_DESCRIPTOR << 8) | (0),
      0,
      buf,
      total_conf_descr_length);

}

USB_TYPE USBHost::setConfiguration(USBDeviceConnected * dev, uint8_t conf)  {
  return controlWrite( dev,
      USB_HOST_TO_DEVICE | USB_RECIPIENT_DEVICE,
      SET_CONFIGURATION,
      conf,
      0,
      NULL,
      0);

}


// enumerate a device with the control USBEndpoint
USB_TYPE USBHost::enumerate(USBDeviceConnected * dev, IUSBEnumerator* pEnumerator)  {
  uint8_t data[384];
  uint16_t total_conf_descr_length = 0;
  USB_TYPE res;

  DBG("data = %p", data);

  if (dev->isEnumerated()) {
    return USB_TYPE_OK;
  }

  // first step: get the size of USBEndpoint 0
  DBG("Get size of EP 0");
  res = controlRead(  dev,
      USB_DEVICE_TO_HOST | USB_RECIPIENT_DEVICE,
      GET_DESCRIPTOR,
      (DEVICE_DESCRIPTOR << 8) | (0),
      0,
      data,
      8);

 if (res != USB_TYPE_OK) {
    ERR("Control read failed!!");
    return res;
  }
  dev->setSizeControlEndpoint(data[7]);
  DBG("size control USBEndpoint: %d", dev->getSizeControlEndpoint());
  
DBG("Now set addr");
  // second step: set an address to the device
  res = controlWrite( dev,
      USB_HOST_TO_DEVICE | USB_RECIPIENT_DEVICE,
      SET_ADDRESS,
      dev->getAddress(),
      0,
      NULL,
      0);

  if (res != USB_TYPE_OK) {
    DBG("SET ADDR FAILED");
    freeDevice(dev);
    return res;
  }
  dev->activeAddress();


  // third step: get the whole device descriptor to see vid, pid
  res = getDeviceDescriptor(dev, data);

  if (res != USB_TYPE_OK) {
    DBG("GET DEV DESCR FAILED");
    return res;
  }
  dev->setClass(data[4]);
  dev->setSubClass(data[5]);
  dev->setProtocol(data[6]);
  dev->setVid(data[8] | (data[9] << 8));
  dev->setPid(data[10] | (data[11] << 8));
  DBG("CLASS: %02X \t VID: %04X \t PID: %04X", data[4], data[8] | (data[9] << 8), data[10] | (data[11] << 8));
  
  pEnumerator->setVidPid( data[8] | (data[9] << 8), data[10] | (data[11] << 8) );

  res = getConfigurationDescriptor(dev, data, &total_conf_descr_length);
  if (res != USB_TYPE_OK) {
    return res;
  }

  // Parse the configuration descriptor
  parseConfDescr(dev, data, total_conf_descr_length, pEnumerator);


  // sixth step: set configuration (only 1 supported)
  res = setConfiguration(dev, 1);

  if (res != USB_TYPE_OK) {
    DBG("SET CONF FAILED");
    freeDevice(dev);
    return res;
  }

  // Now the device is enumerated!
  dev->setEnumerated();
  DBG("device enumerated!!!!");

  // Some devices may require this delay
  Thread::wait(100);

  return USB_TYPE_OK;
}

// this method fills the USBDeviceConnected object: class,.... . It also add endpoints found in the descriptor.
void USBHost::parseConfDescr(USBDeviceConnected * dev, uint8_t * conf_descr, uint32_t len, IUSBEnumerator* pEnumerator)  {
  uint32_t index = 0;
  uint32_t len_desc = 0;
  uint8_t id = 0;
  int nb_endpoints_used = 0;
  USBEndpoint * ep = NULL;
  uint8_t intf_nb = 0;
  bool parsing_intf = false;

  while (index < len) {
    len_desc = conf_descr[index];
    id = conf_descr[index+1];
    switch (id) {
    case CONFIGURATION_DESCRIPTOR:
      break;
    case INTERFACE_DESCRIPTOR:
      if(pEnumerator->parseInterface(intf_nb, conf_descr[index + 5], conf_descr[index + 6], conf_descr[index + 7]))
      {
        if (intf_nb++ <= NB_MAX_INTF) {
          dev->addInterface(intf_nb - 1, conf_descr[index + 5], conf_descr[index + 6], conf_descr[index + 7]);
          nb_endpoints_used = 0;
          DBG("ADD INTF %d on device %p: class: %d, subclass: %d, proto: %d", intf_nb - 1, (void *)dev, conf_descr[index + 5],conf_descr[index + 6],conf_descr[index + 7]);
        } else {
          DBG("Drop intf...");
        }
        parsing_intf = true;
      }
      else
      {
        parsing_intf = false;
      }
      break;
    case ENDPOINT_DESCRIPTOR:
      DBG("Ep DESC");
      if (parsing_intf && (intf_nb <= NB_MAX_INTF) ) {
        if (nb_endpoints_used < MAX_ENDPOINT_PER_INTERFACE) {
          if( pEnumerator->useEndpoint(intf_nb - 1, (ENDPOINT_TYPE)(conf_descr[index + 3] & 0x03), (ENDPOINT_DIRECTION)((conf_descr[index + 2] >> 7) + 1)) )
          {
            // if the USBEndpoint is isochronous -> skip it (TODO: fix this)
            if ((conf_descr[index + 3] & 0x03) != ISOCHRONOUS_ENDPOINT) {
              ep = newEndpoint((ENDPOINT_TYPE)(conf_descr[index+3] & 0x03),
                  (ENDPOINT_DIRECTION)((conf_descr[index + 2] >> 7) + 1),
                  conf_descr[index + 4] | (conf_descr[index + 5] << 8),
                  conf_descr[index + 2] & 0x0f);
              DBG("ADD USBEndpoint %p, on interf %d on device %p", (void *)ep, intf_nb - 1, (void *)dev);
              if (ep != NULL && dev != NULL) {
                addEndpoint(dev, intf_nb - 1, ep);
              } else {
                DBG("EP NULL");
              }
              nb_endpoints_used++;
            } else {
              DBG("ISO USBEndpoint NOT SUPPORTED");
            }
          }
        }
      }
      //DBG("USBEndpoint DESCR");
      break;
    case HID_DESCRIPTOR:
      lenReportDescr = conf_descr[index + 7] | (conf_descr[index + 8] << 8);
      break;
    default:
      break;
    }
    index += len_desc;
  }
}


USB_TYPE USBHost::bulkRead(USBDeviceConnected * dev, USBEndpoint * ep, uint8_t * buf, uint32_t len, bool blocking)  {
  USB_TYPE res;

  if (dev == NULL || ep == NULL) {
    return USB_TYPE_ERROR;
  }

  if ((ep->getDir() != IN) || (ep->getType() != BULK_ENDPOINT)) {
    DBG("wrong dir or bad USBEndpoint type");
    return USB_TYPE_ERROR;
  }
  if (dev->getAddress() != ep->getDeviceAddress()) {
    DBG("USBEndpoint addr and device addr don't match");
    return USB_TYPE_ERROR;
  }
  addTransfer(ep, buf, len);
  if (blocking) {
    unlock();
    while ((res = control->getState()) == USB_TYPE_PROCESSING);
    lock();
    if (res != USB_TYPE_IDLE) {
      return res;
    }
    return USB_TYPE_OK;
  }
  return USB_TYPE_PROCESSING;
}

USB_TYPE USBHost::bulkWrite(USBDeviceConnected * dev, USBEndpoint * ep, uint8_t * buf, uint32_t len, bool blocking)  {
  USB_TYPE res;

  if (dev == NULL || ep == NULL) {
    return USB_TYPE_ERROR;
  }

  if ((ep->getDir() != OUT) || (ep->getType() != BULK_ENDPOINT)) {
    DBG("wrong dir or bad USBEndpoint type");
    return USB_TYPE_ERROR;
  }
  if (dev->getAddress() != ep->getDeviceAddress()) {
    DBG("USBEndpoint addr and device addr don't match");
    return USB_TYPE_ERROR;
  }
  addTransfer(ep, buf, len);
  if (blocking) {
    unlock();
    while ((res = control->getState()) == USB_TYPE_PROCESSING)
    {
      DBG("!!!!!!!!!!!!!wait bulkwrite");
      Thread::wait(100);
    }
    lock();
    DBG("!!!!!!!!!!!!! bulkwrite finished");
    if (res != USB_TYPE_IDLE) {
      return res;
    }
    return USB_TYPE_OK;
  }
  return USB_TYPE_PROCESSING;
}

USB_TYPE USBHost::interruptWrite(USBDeviceConnected * dev, USBEndpoint * ep, uint8_t * buf, uint32_t len, bool blocking)  {
  USB_TYPE res;

  if (dev == NULL || ep == NULL) {
    return USB_TYPE_ERROR;
  }

  if (ep->getState() != USB_TYPE_IDLE) {
    return ep->getState();
  }

  if ((ep->getDir() != OUT) || (ep->getType() != INTERRUPT_ENDPOINT)) {
    ERR("wrong dir or bad USBEndpoint type: %d, %d", ep->getDir(), ep->getType());
    return USB_TYPE_ERROR;
  }
  if (dev->getAddress() != ep->getDeviceAddress()) {
    ERR("USBEndpoint addr and device addr don't match");
    return USB_TYPE_ERROR;
  }
  addTransfer(ep, buf, len);
  if (blocking) {
    while ((res = ep->getState()) == USB_TYPE_PROCESSING);
    if (res != USB_TYPE_IDLE) {
      return res;
    }
    return USB_TYPE_OK;
  }
  return USB_TYPE_PROCESSING;
}

USB_TYPE USBHost::interruptRead(USBDeviceConnected * dev, USBEndpoint * ep, uint8_t * buf, uint32_t len, bool blocking)  {
  USB_TYPE res;

  if (dev == NULL || ep == NULL) {
    return USB_TYPE_ERROR;
  }

  if (ep->getState() != USB_TYPE_IDLE) {
    return ep->getState();
  }

  if ((ep->getDir() != IN) || (ep->getType() != INTERRUPT_ENDPOINT)) {
    ERR("wrong dir or bad USBEndpoint type");
    return USB_TYPE_ERROR;
  }

  if (dev->getAddress() != ep->getDeviceAddress()) {
    ERR("USBEndpoint addr and device addr don't match");
    return USB_TYPE_ERROR;
  }
  addTransfer(ep, buf, len);
  if (blocking) {
    while ((res = ep->getState()) == USB_TYPE_PROCESSING);
    if (res != USB_TYPE_IDLE) {
      return res;
    }
    return USB_TYPE_OK;
  }
  return USB_TYPE_PROCESSING;
}


USB_TYPE USBHost::controlRead(USBDeviceConnected * dev, uint8_t requestType, uint8_t request, uint32_t value, uint32_t index, uint8_t * buf, uint32_t len)  {
  int length_transfer = len;
  //DBG("want to transfer: %d bytes\r\n", length_transfer);
  USB_TYPE res;
  control->setSpeed(dev->getSpeed());
  control->setSize(dev->getSizeControlEndpoint());
  if (dev->isActiveAddress()) {
    control->setDeviceAddress(dev->getAddress());
  } else {
    control->setDeviceAddress(0);
  }
  fillControlBuf(requestType, request, value, index, len);
 /* DBG("will call transfer: ");
    for (int i = 0; i < 8; i++) {
        DBG("%02X ", setupPacket[i]);
    }*/
  control->setNextToken(TD_SETUP);
  addTransfer(control, (uint8_t*)setupPacket, 8);
  DBG("Now wait for TD to be processed");
  unlock();
  DBG("Unlocked");
  while ((res = control->getState()) == USB_TYPE_PROCESSING);
  lock();
  DBG("TD processed with result %d", res);
  if (res != USB_TYPE_IDLE) {
    return res;
  }

  if (length_transfer) {
    DBG("In data to be transfered...");
    control->setNextToken(TD_IN);
    addTransfer(control, (uint8_t *)buf, length_transfer);
    unlock();
    while ((res = control->getState()) == USB_TYPE_PROCESSING);
    lock();
    if (res != USB_TYPE_IDLE) {
      return res;
    }
  }

  DBG("Transfer NULL packet (OUT)");
  control->setNextToken(TD_OUT);
  addTransfer(control, NULL, 0);
  unlock();
  while ((res = control->getState()) == USB_TYPE_PROCESSING);
  lock();
  if (res != USB_TYPE_IDLE) {
    return res;
  }
  return USB_TYPE_OK;
}


USB_TYPE USBHost::controlWrite(USBDeviceConnected * dev, uint8_t requestType, uint8_t request, uint32_t value, uint32_t index, uint8_t * buf, uint32_t len)  {
  control->setSpeed(dev->getSpeed());

  int length_transfer = len;
  USB_TYPE res;

  control->setSize(dev->getSizeControlEndpoint());
  if (dev->isActiveAddress()) {
    control->setDeviceAddress(dev->getAddress());
  } else {
    control->setDeviceAddress(0);
  }
  fillControlBuf(requestType, request, value, index, len);
  /*DBG("will call transfer: ");
    for (int i = 0; i < 8; i++) {
        printf("%01X ", setupPacket[i]);
    }
    printf("\r\n");*/
  control->setNextToken(TD_SETUP);
  addTransfer(control, (uint8_t*)setupPacket, 8);
  unlock();
  while ((res = control->getState()) == USB_TYPE_PROCESSING);
  lock();
  if (res != USB_TYPE_IDLE) {
    return res;
  }

  if (length_transfer) {
    control->setNextToken(TD_OUT);
    addTransfer(control, (uint8_t *)buf, length_transfer);
    unlock();
    while ((res = control->getState()) == USB_TYPE_PROCESSING);
    lock();
    if (res != USB_TYPE_IDLE) {
      return res;
    }
  }

  control->setNextToken(TD_IN);
  addTransfer(control, NULL, 0);
  unlock();
  while ((res = control->getState()) == USB_TYPE_PROCESSING);
  lock();
  if (res != USB_TYPE_IDLE) {
    return res;
  }
  return USB_TYPE_OK;
}


void USBHost::fillControlBuf(uint8_t requestType, uint8_t request, uint32_t value, uint32_t index, int len)  {
#ifdef __BIG_ENDIAN
#error "Must implement BE to LE conv here"
#endif
  setupPacket[0] = requestType;
  setupPacket[1] = request;
  //We are in LE so it's fine
  *((uint32_t*)&setupPacket[2]) = value;
  *((uint32_t*)&setupPacket[4]) = index;
  *((uint32_t*)&setupPacket[6]) = (uint32_t) len;
}

