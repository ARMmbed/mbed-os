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

#include "USBHostConf.h"

#ifdef USBHOST_3GMODULE

#define __DEBUG__ 0
#ifndef __MODULE__
#define __MODULE__ "WANDongleInitializer.cpp"
#endif

#include "core/dbg.h"

#include <cstdint>
using std::uint16_t;

#include "WANDongleInitializer.h"

WANDongleInitializer::WANDongleInitializer(USBHost* pHost) : m_pHost(pHost)
{

}

WANDongleInitializer** WANDongleInitializer::getInitializers(USBHost* pHost)
{
  static VodafoneK3770Initializer vodafoneK3770(pHost);
  static VodafoneK3772ZInitializer vodafoneK3772Z(pHost);
  // mamm
  static UbxInitializer ubx(pHost);
  const static WANDongleInitializer* list[] = { &vodafoneK3770, &vodafoneK3772Z, &ubx, NULL };
//  const static WANDongleInitializer* list[] = { &vodafoneK3770, &vodafoneK3772Z, NULL };
  // mamm
  return (WANDongleInitializer**)list;
}

//Huawei K3770 (Vodafone)
// Switching from mass storage device string is: "55 53 42 43 12 34 56 78 00 00 00 00 00 00 00 11 06 20 00 00 01 00 00 00 00 00 00 00 00 00 00"
static uint8_t vodafone_k3770_switch_packet[] = {
    0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78, 0, 0, 0, 0, 0, 0, 0, 0x11, 0x06, 0x20, 0, 0, 0x01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

VodafoneK3770Initializer::VodafoneK3770Initializer(USBHost* pHost) : WANDongleInitializer(pHost)
{
  
}

uint16_t VodafoneK3770Initializer::getMSDVid()      { return 0x12D1; }
uint16_t VodafoneK3770Initializer::getMSDPid()      { return 0x14D1; }

uint16_t VodafoneK3770Initializer::getSerialVid()   { return 0x12D1; }
uint16_t VodafoneK3770Initializer::getSerialPid()   { return 0x14C9; }

bool VodafoneK3770Initializer::switchMode(USBDeviceConnected* pDev)
{
  for (int i = 0; i < pDev->getNbIntf(); i++)
  {
    if (pDev->getInterface(i)->intf_class == MSD_CLASS)
    {
      USBEndpoint* pEp = pDev->getEndpoint(i, BULK_ENDPOINT, OUT);
      if ( pEp != NULL ) 
      {
        DBG("MSD descriptor found on device %p, intf %d, will now try to switch into serial mode", (void *)pDev, i);
        m_pHost->bulkWrite(pDev, pEp, vodafone_k3770_switch_packet, 31);
        return true;
      }
    }  
  }
  return false;
}

USBEndpoint* VodafoneK3770Initializer::getEp(USBDeviceConnected* pDev, int serialPortNumber, bool tx)
{
  return pDev->getEndpoint(serialPortNumber, BULK_ENDPOINT, tx?OUT:IN, 0);
}

int VodafoneK3770Initializer::getSerialPortCount()
{
  return 2;
}

/*virtual*/ void VodafoneK3770Initializer::setVidPid(uint16_t vid, uint16_t pid)
{
    if( (vid == getSerialVid() ) && ( pid == getSerialPid() ) )
    {
      m_hasSwitched = true;
      m_currentSerialIntf = 0;
      m_endpointsToFetch = 4;
    }
    else
    {
      m_hasSwitched = false;
      m_endpointsToFetch = 1;
    }
}

/*virtual*/ bool VodafoneK3770Initializer::parseInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol) //Must return true if the interface should be parsed
{
  if( m_hasSwitched )
  {
    if( intf_class == 0xFF )
    {
      if( (m_currentSerialIntf == 0) || (m_currentSerialIntf == 4) )
      {
        m_currentSerialIntf++;
        return true;
      }
      m_currentSerialIntf++;
    }
  }
  else
  {
    if( (intf_nb == 0) && (intf_class == MSD_CLASS) )
    {
      return true;
    }
  }
  return false;
}

/*virtual*/ bool VodafoneK3770Initializer::useEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir) //Must return true if the endpoint will be used
{
  if( m_hasSwitched )
  {
    if( (type == BULK_ENDPOINT) && m_endpointsToFetch )
    {
      m_endpointsToFetch--;
      return true;
    }
  }
  else
  {
    if( (type == BULK_ENDPOINT) && (dir == OUT) && m_endpointsToFetch )
    {
      m_endpointsToFetch--;
      return true;
    }
  }
  return false;
}

/*virtual*/ WAN_DONGLE_TYPE VodafoneK3770Initializer::getType()
{
  return WAN_DONGLE_TYPE_VODAFONEK3770;
}

// NVIDIA (ICERA) /ZTE K3772-Z (Vodafone)
// Switching from mass storage device string is: "55 53 42 43 12 34 56 78 00 00 00 00 00 00 06 1b 00 00 00 02 00 00 00 00 00 00 00 00 00 00 00"
static uint8_t vodafone_k3772_z_switch_packet[] = {
    0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78, 0, 0, 0, 0, 0, 0, 0x06, 0x1b, 0, 0, 0, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

VodafoneK3772ZInitializer::VodafoneK3772ZInitializer(USBHost* pHost) : WANDongleInitializer(pHost)
{
  
}

uint16_t VodafoneK3772ZInitializer::getMSDVid()      { return 0x19D2; }
uint16_t VodafoneK3772ZInitializer::getMSDPid()      { return 0x1179; }

uint16_t VodafoneK3772ZInitializer::getSerialVid()   { return 0x19D2; }
uint16_t VodafoneK3772ZInitializer::getSerialPid()   { return 0x1181; }

bool VodafoneK3772ZInitializer::switchMode(USBDeviceConnected* pDev)
{
  for (int i = 0; i < pDev->getNbIntf(); i++)
  {
    if (pDev->getInterface(i)->intf_class == MSD_CLASS)
    {
      USBEndpoint* pEp = pDev->getEndpoint(i, BULK_ENDPOINT, OUT);
      if ( pEp != NULL ) 
      {
        DBG("MSD descriptor found on device %p, intf %d, will now try to switch into serial mode", (void *)pDev, i);
        m_pHost->bulkWrite(pDev, pEp, vodafone_k3772_z_switch_packet, 31);
        return true;
      }
    }  
  }
  return false;
}

USBEndpoint* VodafoneK3772ZInitializer::getEp(USBDeviceConnected* pDev, int serialPortNumber, bool tx)
{
  return pDev->getEndpoint((serialPortNumber==1)?0:1, BULK_ENDPOINT, tx?OUT:IN, 0);
}

int VodafoneK3772ZInitializer::getSerialPortCount()
{
  return 2;
}

/*virtual*/ void VodafoneK3772ZInitializer::setVidPid(uint16_t vid, uint16_t pid)
{
    if( (vid == getSerialVid() ) && ( pid == getSerialPid() ) )
    {
      m_hasSwitched = true;
      m_currentSerialIntf = 0;
      m_endpointsToFetch = 4;
    }
    else
    {
      m_hasSwitched = false;
      m_endpointsToFetch = 1;
    }
}

/*virtual*/ bool VodafoneK3772ZInitializer::parseInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol) //Must return true if the interface should be parsed
{
  if( m_hasSwitched )
  {
    DBG("Interface #%d; Class:%02x; SubClass:%02x; Protocol:%02x", intf_nb, intf_class, intf_subclass, intf_protocol);
    if( intf_class == 0x0A )
    {
      if( (m_currentSerialIntf == 0) || (m_currentSerialIntf == 1) )
      {
        m_currentSerialIntf++;
        return true;
      }
      m_currentSerialIntf++;
    }
  }
  else
  {
    if( (intf_nb == 0) && (intf_class == MSD_CLASS) )
    {
      return true;
    }
  }
  return false;
}

/*virtual*/ bool VodafoneK3772ZInitializer::useEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir) //Must return true if the endpoint will be used
{
  if( m_hasSwitched )
  {
    DBG("USBEndpoint on Inteface #%d; Type:%d; Direction:%d", intf_nb, type, dir);
    if( (type == BULK_ENDPOINT) && m_endpointsToFetch )
    {
      m_endpointsToFetch--;
      return true;
    }
  }
  else
  {
    if( (type == BULK_ENDPOINT) && (dir == OUT) && m_endpointsToFetch )
    {
      m_endpointsToFetch--;
      return true;
    }
  }
  return false;
}


/*virtual*/ WAN_DONGLE_TYPE VodafoneK3772ZInitializer::getType()
{
  return WAN_DONGLE_TYPE_VODAFONEK3772Z;
}

//-----------------------------------------------------------------------
// mamm, u-blox Modem
//-----------------------------------------------------------------------

UbxInitializer::UbxInitializer(USBHost* pHost) : WANDongleInitializer(pHost)
{
  
}

uint16_t UbxInitializer::getMSDVid()      { return 0x1546; }
uint16_t UbxInitializer::getMSDPid()      { return 0x0000; }

uint16_t UbxInitializer::getSerialVid()   { return 0x1546; }
uint16_t UbxInitializer::getSerialPid()   { return 0x1102; }

bool UbxInitializer::switchMode(USBDeviceConnected* pDev)
{
  for (int i = 0; i < pDev->getNbIntf(); i++)
  {
    if (pDev->getInterface(i)->intf_class == MSD_CLASS)
    {
      USBEndpoint* pEp = pDev->getEndpoint(i, BULK_ENDPOINT, OUT);
      if ( pEp != NULL ) 
      {
        ERR("MSD descriptor found on device %p, intf %d", (void *)pDev, i);
      }
    }  
  }
  return false;
}

#define UBX_SERIALCOUNT 7 

int UbxInitializer::getSerialPortCount()
{
  return UBX_SERIALCOUNT;
}

/*virtual*/ void UbxInitializer::setVidPid(uint16_t vid, uint16_t pid)
{
    if( (vid == getSerialVid() ) && ( pid == getSerialPid() ) )
    {
      m_hasSwitched = true;
      m_currentSerialIntf = 0;
      m_endpointsToFetch = UBX_SERIALCOUNT*2;
    }
    else
    {
      m_hasSwitched = false;
      m_endpointsToFetch = 1;
    }
}

/*virtual*/ bool UbxInitializer::parseInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol) //Must return true if the interface should be parsed
{
  if( m_hasSwitched )
  {
    DBG("Interface #%d; Class:%02x; SubClass:%02x; Protocol:%02x", intf_nb, intf_class, intf_subclass, intf_protocol);
    if( intf_class == 0x0A )
    {
      if( (m_currentSerialIntf == 0) || (m_currentSerialIntf == 1) )
      {
        m_serialIntfMap[m_currentSerialIntf++] = intf_nb;
        return true;
      }
      m_currentSerialIntf++;
    }
  }
  else
  {
    if( (intf_nb == 0) && (intf_class == MSD_CLASS) )
    {
      return true;
    }
  }
  return false;
}

/*virtual*/ bool UbxInitializer::useEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir) //Must return true if the endpoint will be used
{
  if( m_hasSwitched )
  {
    DBG("USBEndpoint on Interface #%d; Type:%d; Direction:%d", intf_nb, type, dir);
    if( (type == BULK_ENDPOINT) && m_endpointsToFetch )
    {
      m_endpointsToFetch--;
      return true;
    }
  }
  else
  {
    if( (type == BULK_ENDPOINT) && (dir == OUT) && m_endpointsToFetch )
    {
      m_endpointsToFetch--;
      return true;
    }
  }
  return false;
}

/*virtual*/ WAN_DONGLE_TYPE UbxInitializer::getType()
{
  return WAN_DONGLE_TYPE_UBX;
}

#endif /* USBHOST_3GMODULE */
