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

#ifndef WANDONGLEINITIALIZER_H
#define WANDONGLEINITIALIZER_H

#include "USBHostConf.h"

#ifdef USBHOST_3GMODULE

#include <cstdint>
using std::uint16_t;
using std::uint32_t;

#include "USBHost.h"
#include "IUSBEnumerator.h"

enum WAN_DONGLE_TYPE
{
  WAN_DONGLE_TYPE_UNKNOWN = -1,
  WAN_DONGLE_TYPE_VODAFONEK3770 = 0,
  WAN_DONGLE_TYPE_VODAFONEK3772Z = 1,
  WAN_DONGLE_TYPE_UBX = 2,
};

#define WANDONGLE_MAX_SERIAL_PORTS 2

class WANDongleInitializer : public IUSBEnumerator
{
protected:
    WANDongleInitializer(USBHost* pHost);
    USBHost* m_pHost;
    uint8_t m_serialIntfMap[WANDONGLE_MAX_SERIAL_PORTS];
    
public:
    virtual uint16_t getMSDVid() = 0;
    virtual uint16_t getMSDPid() = 0;
    
    virtual uint16_t getSerialVid() = 0;
    virtual uint16_t getSerialPid() = 0;
    
    virtual bool switchMode(USBDeviceConnected* pDev) = 0;
    
    virtual USBEndpoint* getEp(USBDeviceConnected* pDev, int serialPortNumber, bool tx) {
        return pDev->getEndpoint(m_serialIntfMap[serialPortNumber], BULK_ENDPOINT, tx ? OUT : IN, 0);
    }
    
    virtual int getSerialPortCount() = 0;
    
    virtual void setVidPid(uint16_t vid, uint16_t pid) = 0;
    
    virtual bool parseInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol) = 0; //Must return true if the interface should be parsed
    
    virtual bool useEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir) = 0; //Must return true if the endpoint will be used
    
    virtual WAN_DONGLE_TYPE getType() = 0;

    virtual uint8_t getSerialIntf(int index) { return m_serialIntfMap[index]; }
    
    static WANDongleInitializer** getInitializers(USBHost* pHost);
};

class VodafoneK3770Initializer : public WANDongleInitializer
{
public:
    VodafoneK3770Initializer(USBHost* pHost);
    
    virtual uint16_t getMSDVid();
    virtual uint16_t getMSDPid();
    
    virtual uint16_t getSerialVid();
    virtual uint16_t getSerialPid();
    
    virtual bool switchMode(USBDeviceConnected* pDev);
    
    virtual USBEndpoint* getEp(USBDeviceConnected* pDev, int serialPortNumber, bool tx);
    
    virtual int getSerialPortCount();
    
    virtual void setVidPid(uint16_t vid, uint16_t pid);
    
    virtual bool parseInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol); //Must return true if the interface should be parsed
    
    virtual bool useEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir); //Must return true if the endpoint will be used
    
    virtual WAN_DONGLE_TYPE getType();
    
private:

  bool m_hasSwitched;
  int m_currentSerialIntf;
  int m_endpointsToFetch;
};

class VodafoneK3772ZInitializer : public WANDongleInitializer
{
public:
    VodafoneK3772ZInitializer(USBHost* pHost);
    
    virtual uint16_t getMSDVid();
    virtual uint16_t getMSDPid();
    
    virtual uint16_t getSerialVid();
    virtual uint16_t getSerialPid();
    
    virtual bool switchMode(USBDeviceConnected* pDev);
    
    virtual USBEndpoint* getEp(USBDeviceConnected* pDev, int serialPortNumber, bool tx);
    
    virtual int getSerialPortCount();
    
    virtual void setVidPid(uint16_t vid, uint16_t pid);
    
    virtual bool parseInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol); //Must return true if the interface should be parsed
    
    virtual bool useEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir); //Must return true if the endpoint will be used
    
    virtual WAN_DONGLE_TYPE getType();
    
private:

  bool m_hasSwitched;
  int m_currentSerialIntf;
  int m_endpointsToFetch;
};

//-----------------------------------------------------------------------
// mamm, u-blox Modem
//-----------------------------------------------------------------------

class UbxInitializer : public WANDongleInitializer
{
public:
    UbxInitializer(USBHost* pHost);
    
    virtual uint16_t getMSDVid();
    virtual uint16_t getMSDPid();
    
    virtual uint16_t getSerialVid();
    virtual uint16_t getSerialPid();
    
    virtual bool switchMode(USBDeviceConnected* pDev);
    
    virtual int getSerialPortCount();
    
    virtual void setVidPid(uint16_t vid, uint16_t pid);
    
    virtual bool parseInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol); //Must return true if the interface should be parsed
    
    virtual bool useEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir); //Must return true if the endpoint will be used
    
    virtual WAN_DONGLE_TYPE getType();
    
private:

  bool m_hasSwitched;
  int m_currentSerialIntf;
  int m_endpointsToFetch;
};

#endif /* USBHOST_3GMODULE */

#endif
