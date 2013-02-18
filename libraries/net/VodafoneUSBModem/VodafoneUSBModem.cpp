/* VodafoneUSBModem.cpp */
/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
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

#define __DEBUG__ 3
#ifndef __MODULE__
#define __MODULE__ "VodafoneUSBModem.cpp"
#endif

#include "core/fwk.h"

#include "VodafoneUSBModem.h"

VodafoneUSBModem::VodafoneUSBModem(PinName powerGatingPin /*= NC*/, bool powerGatingOnWhenPinHigh /* = true*/) :
   m_dongle(),                         // Construct WANDongle: USB interface with two serial channels to the modem (USBSerialStream objects)
   m_atStream(m_dongle.getSerial(1)),  // AT commands are sent down one serial channel.
   m_pppStream(m_dongle.getSerial(0)), // PPP connections are managed via another serial channel.
   m_at(&m_atStream),                  // Construct ATCommandsInterface with the AT serial channel
   m_sms(&m_at),                       // Construct SMSInterface with the ATCommandsInterface
   m_ussd(&m_at),                      // Construct USSDInterface with the ATCommandsInterface
   m_linkMonitor(&m_at),               // Construct LinkMonitor with the ATCommandsInterface
   m_ppp(&m_pppStream),                // Construct PPPIPInterface with the PPP serial channel
   m_dongleConnected(false),           // Dongle is initially not ready for anything
   m_ipInit(false),                    // PPIPInterface connection is initially down
   m_smsInit(false),                   // SMSInterface starts un-initialised
   m_ussdInit(false),                  // USSDInterface starts un-initialised 
   m_linkMonitorInit(false),           // LinkMonitor subsystem starts un-initialised
   m_atOpen(false),                    // ATCommandsInterface starts in a closed state
   m_powerGatingPin(powerGatingPin),   // set power gating pin
   m_powerGatingOnWhenPinHigh(powerGatingOnWhenPinHigh) // set state semantics for power gating pin
{
  if( m_powerGatingPin != NC )
  {
    power(false); //Dongle will have to be powered on manually
  }
}

class CREGProcessor : public IATCommandsProcessor
{
public:
  CREGProcessor() : status(STATUS_REGISTERING)
  {

  }
  enum REGISTERING_STATUS { STATUS_REGISTERING, STATUS_OK, STATUS_FAILED };
  REGISTERING_STATUS getStatus()
  {
    return status;
  }
private:
  virtual int onNewATResponseLine(ATCommandsInterface* pInst, const char* line)
  {
    int r;
    if( sscanf(line, "+CREG: %*d,%d", &r) == 1 )
    {
      switch(r)
      {
      case 1:
      case 5:
        status = STATUS_OK;
        break;
      case 0:
      case 2:
        status = STATUS_REGISTERING;
        break;
      case 3:
      default:
        status = STATUS_FAILED;
        break;
      }
    }
    return OK;
  }
  virtual int onNewEntryPrompt(ATCommandsInterface* pInst)
  {
    return OK;
  }
  volatile REGISTERING_STATUS status;
};

#if 0
class COPSProcessor : public IATCommandsProcessor
{
public:
  COPSProcessor() : valid(false)
  {
    network[0] = '\0';
    apn[0] = '\0';
    bearer[0] = '\0';
  }
  char* getNetwork()
  {
    return network;
  }
  char* getAPN()
  {
    return apn;
  }
  char* getBearer()
  {
    return bearer;
  }
  bool isValid()
  {
    return valid;
  }
private:
  virtual int onNewATResponseLine(ATCommandsInterface* pInst, const char* line)
  {
    int networkId;
    int bearerId;
    int s = sscanf(line, "+COPS: %*d,%*d,\"%d\",%d", &networkId, &bearerId);
    if( s == 2 )
    {
      switch(networkId)
      {
      case 23415:
        strcpy(network, "Vodafone UK");
        strcpy(apn, "pp.vodafone.co.uk");
        valid = true;
        break;
      case 20810:
        strcpy(network, "SFR FR");
        strcpy(apn, "websfr");
        valid = true;
        break;
      default:
        break;
      }
    }
    else
    {
      return OK;
    }
    switch(bearerId)
    {
    case 0: strcpy(bearer, "GSM"); break;
    case 1: strcpy(bearer, "GSM Compact"); break;
    case 2: strcpy(bearer, "UTRAN"); break;
    case 3: strcpy(bearer, "GSM w/EGPRS"); break;
    case 4: strcpy(bearer, "UTRAN w/HSDPA"); break;
    case 5: strcpy(bearer, "UTRAN w/HSUPA"); break;
    case 6: strcpy(bearer, "UTRAN w/HSDPA and HSUPA"); break;
    case 7: strcpy(bearer, "E-UTRAN"); break;

    default:
      break;
    }
    return OK;
  }
  virtual int onNewEntryPrompt(ATCommandsInterface* pInst)
  {
    return OK;
  }
  char network[24];
  char bearer[24];
  char apn[24];
  volatile bool valid;
};
#endif

int VodafoneUSBModem::connect(const char* apn, const char* user, const char* password)
{
  if( !m_ipInit )
  {
    m_ipInit = true;
    m_ppp.init();
  }
  m_ppp.setup(user, password);

  int ret = init();
  if(ret)
  {
    return ret;
  }

  #if USE_ONE_PORT
  m_smsInit = false; //SMS status reset
  m_ussdInit = false; //USSD status reset
  m_linkMonitorInit = false; //Link monitor status reset
  #endif

  ATCommandsInterface::ATResult result;

  #if 0
  //Get network info & select corresponding APN
  COPSProcessor copsProcessor;
  DBG("Get network info & select APN from DB");
  ret = m_at.execute("AT+COPS=,2;+COPS?", &copsProcessor, &result); //Configure to get operator's info in numeric code & get operator's id
  DBG("Result of command: Err code=%d", ret);
  DBG("ATResult: AT return=%d (code %d)", result.result, result.code);

  if(!copsProcessor.isValid())
  {
    WARN("Connected to an unknown network, try to connect with default parameters");
    DBG("Connected with %s", copsProcessor.getBearer());
  }
  else
  {
    DBG("Connected to %s with %s", copsProcessor.getNetwork(), copsProcessor.getBearer());
    char cmd[48];
    int tries = 3;
    sprintf(cmd, "AT+CGDCONT=1,\"IP\",\"%s\"", copsProcessor.getAPN());
    do //Try 3 times because for some reasons it can fail with the K3772-Z dongle
    {
      ret = m_at.executeSimple(cmd, &result);
      DBG("Result of command: Err code=%d", ret);
    } while(ret && --tries);
    DBG("ATResult: AT return=%d (code %d)", result.result, result.code);
    DBG("APN set to %s", copsProcessor.getAPN());
  }
  #else
  if(apn != NULL)
  {
    char cmd[48];
    int tries = 30;
    sprintf(cmd, "AT+CGDCONT=1,\"IP\",\"%s\"", apn);
    do //Try 30 times because for some reasons it can fail *a lot* with the K3772-Z dongle
    {
      ret = m_at.executeSimple(cmd, &result);
      DBG("Result of command: Err code=%d", ret);
      if(ret)
      {
        Thread::wait(500);
      }
    } while(ret && --tries);
    DBG("ATResult: AT return=%d (code %d)", result.result, result.code);
    DBG("APN set to %s", apn);
  }
  #endif


  //Connect
  DBG("Connecting");
  #if 0
  ret = m_at.executeSimple("ATDT *99#", &result);
  DBG("Result of command: Err code=%d", ret);
  DBG("ATResult: AT return=%d (code %d)", result.result, result.code);
  #endif
  #if USE_ONE_PORT
  m_at.close(); // Closing AT parser
  m_atOpen = false; //Will need to be reinitialized afterwards
  #endif

  #if 0
  DBG("AT Parser closed");
  if( (ret!=NET_MOREINFO) || (result.result != ATCommandsInterface::ATResult::AT_CONNECT))
  {
    ERR("Could not connect");
    return ret; //Could not connect
  }
  #endif
  DBG("Connecting PPP");

  ret = m_ppp.connect();
  DBG("Result of connect: Err code=%d", ret);
  return ret;
}


int VodafoneUSBModem::disconnect()
{
  DBG("Disconnecting from PPP");
  int ret = m_ppp.disconnect();
  if(ret)
  {
    ERR("Disconnect returned %d, still trying to disconnect", ret);
  }

  //Ugly but leave dongle time to recover
  Thread::wait(500);

  #if USE_ONE_PORT
  ATCommandsInterface::ATResult result;
  DBG("Starting AT thread");
  ret = m_at.open();
  if(ret)
  {
    return ret;
  }
  #endif

  DBG("Trying to hangup");

  #if 0 //Does not appear to work
  int tries = 10;
  do
  {
    ret = m_at.executeSimple("+++", &result, 1000);
    DBG("Result of command: Err code=%d\n", ret);
    DBG("ATResult: AT return=%d (code %d)\n", result.result, result.code);
  } while(tries-- && ret);
  if(!ret)
  {
    ret = m_at.executeSimple("ATH", &result);
    DBG("Result of command: Err code=%d\n", ret);
    DBG("ATResult: AT return=%d (code %d)\n", result.result, result.code);
  }
  #endif

  #if USE_ONE_PORT
  //Reinit AT parser
  ret = m_at.init();
  DBG("Result of command: Err code=%d\n", ret);
  if(ret)
  {
    m_at.close(); // Closing AT parser
    DBG("AT Parser closed, could not complete disconnection");
    return NET_TIMEOUT;
  }

  #if 0
  m_at.close(); // Closing AT parser
  DBG("AT Parser closed");
  #endif
  #endif
  return OK;
}

int VodafoneUSBModem::sendSM(const char* number, const char* message)
{
  int ret = init();
  if(ret)
  {
    return ret;
  }

  if(!m_smsInit)
  {
    ret = m_sms.init();
    if(ret)
    {
      return ret;
    }
    m_smsInit = true;
  }

  ret = m_sms.send(number, message);
  if(ret)
  {
    return ret;
  }

  return OK;
}

int VodafoneUSBModem::getSM(char* number, char* message, size_t maxLength)
{
  int ret = init();
  if(ret)
  {
    return ret;
  }

  if(!m_smsInit)
  {
    ret = m_sms.init();
    if(ret)
    {
      return ret;
    }
    m_smsInit = true;
  }

  ret = m_sms.get(number, message, maxLength);
  if(ret)
  {
    return ret;
  }

  return OK;
}

int VodafoneUSBModem::getSMCount(size_t* pCount)
{
  int ret = init();
  if(ret)
  {
    return ret;
  }

  if(!m_smsInit)
  {
    ret = m_sms.init();
    if(ret)
    {
      return ret;
    }
    m_smsInit = true;
  }

  ret = m_sms.getCount(pCount);
  if(ret)
  {
    return ret;
  }

  return OK;
}

int VodafoneUSBModem::sendUSSD(const char* command, char* result, size_t maxLength)
{
  int ret = init();
  if(ret)
  {
    return ret;
  }

  if(!m_ussdInit)
  {
    ret = m_ussd.init();
    if(ret)
    {
      return ret;
    }
    m_ussdInit = true;
  }

  ret = m_ussd.send(command, result, maxLength);
  if(ret)
  {
    return ret;
  }

  return OK;
}

int VodafoneUSBModem::getLinkState(int* pRssi, LinkMonitor::REGISTRATION_STATE* pRegistrationState, LinkMonitor::BEARER* pBearer)
{
  int ret = init();
  if(ret)
  {
    return ret;
  }
  
  if(!m_linkMonitorInit)
  {
    ret = m_linkMonitor.init();
    if(ret)
    {
      return ret;
    }
    m_linkMonitorInit = true;
  }

  ret = m_linkMonitor.getState(pRssi, pRegistrationState, pBearer);
  if(ret)
  {
    return ret;
  }

  return OK;
}


ATCommandsInterface* VodafoneUSBModem::getATCommandsInterface()
{
  return &m_at;
}

int VodafoneUSBModem::power(bool enable)
{
  if( m_powerGatingPin == NC )
  {
    return NET_INVALID; //A pin name has not been provided in the constructor
  }

  if(!enable) //Will force components to re-init
  {
    cleanup();
  }
  
  DigitalOut powerGatingOut(m_powerGatingPin);
  powerGatingOut = m_powerGatingOnWhenPinHigh?enable:!enable;

  return OK;
}

bool VodafoneUSBModem::power()
{
  if( m_powerGatingPin == NC )
  {
    return true; //Assume power is always on 
  }
  
  DigitalOut powerGatingOut(m_powerGatingPin);
  return m_powerGatingOnWhenPinHigh?powerGatingOut:!powerGatingOut;
}

int VodafoneUSBModem::init()
{
  if( !m_dongleConnected )
  {
    if(!power())
    {
      //Obviously cannot initialize the dongle if it is disconnected...
      ERR("Power is off");
      return NET_INVALID;
    }
    m_dongleConnected = true;
    while( !m_dongle.connected() )
    {
      m_dongle.tryConnect();
      Thread::wait(10);
    }
  }

  if(m_atOpen)
  {
    return OK;
  }
  
  DBG("Starting AT thread if needed");
  int ret = m_at.open();
  if(ret)
  {
    return ret;
  }
  
  DBG("Sending initialisation commands");
  ret = m_at.init();
  if(ret)
  {
    return ret;
  }

  if(m_dongle.getDongleType() == WAN_DONGLE_TYPE_VODAFONEK3770)
  {
    INFO("Using a Vodafone K3770 Dongle");
    #if USE_ONE_PORT
    DBG("Configuring unsolicited result codes support properly");
    //Configuring port to enable 3GPP-compliant unsollicited response codes but disable Huawei-specific unsollicited response codes
    ret = m_at.executeSimple("AT^CURC=0;^PORTSEL=1", NULL); //Huawei-specific, not 3GPP-compliant
    if(ret != OK)
    {
      return NET_PROTOCOL;
    }
    #else
    //Configuring port to disable Huawei-specific unsollicited response codes
    ret = m_at.executeSimple("AT^CURC=0", NULL); //Huawei-specific, not 3GPP-compliant
    if(ret != OK)
    {
      return NET_PROTOCOL;
    }  
    #endif
  }
  else if(m_dongle.getDongleType() == WAN_DONGLE_TYPE_VODAFONEK3772Z)
  {
    INFO("Using a Vodafone K3772-Z Dongle");
    //FIXME this returns %USBMODEM: [0] MODEM DRIVER<CR><LF><CR><LF><CR><LF>OK<CR><LF> which is not a compliant response
    /*
    //Configuring modem to directly boot into modem mode
    ret = m_at.executeSimple("AT%USBMODEM=0", NULL); //Icera-specific, not 3GPP-compliant
    if(ret != OK)
    {
      return NET_PROTOCOL;
    }
    */
  }
  else
  {
    WARN("Using an Unknown Dongle");
  }

  ATCommandsInterface::ATResult result;

  //Wait for network registration
  CREGProcessor cregProcessor;
  do
  {
    DBG("Waiting for network registration");
    ret = m_at.execute("AT+CREG?", &cregProcessor, &result);
    DBG("Result of command: Err code=%d\n", ret);
    DBG("ATResult: AT return=%d (code %d)\n", result.result, result.code);
    if(cregProcessor.getStatus() == CREGProcessor::STATUS_REGISTERING)
    {
      Thread::wait(3000);
    }
  } while(cregProcessor.getStatus() == CREGProcessor::STATUS_REGISTERING);
  if(cregProcessor.getStatus() == CREGProcessor::STATUS_FAILED)
  {
    ERR("Registration denied");
    return NET_AUTH;
  }

  m_atOpen = true;

  return OK;
}

int VodafoneUSBModem::cleanup()
{
  if(m_ppp.isConnected())
  {
    WARN("Data connection is still open"); //Try to encourage good behaviour from the user
    m_ppp.disconnect(); 
  }
  
  m_smsInit = false;
  m_ussdInit = false;
  m_linkMonitorInit = false;
  //We don't reset m_ipInit as PPPIPInterface::init() only needs to be called once
  
  if(m_atOpen)
  {
    m_at.close();
    m_atOpen = false;
  }
  
  m_dongle.disconnect();
  m_dongleConnected = false;
  
  return OK;
}

