/* LinkMonitor.cpp */
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

#define __DEBUG__ 0
#ifndef __MODULE__
#define __MODULE__ "LinkMonitor.cpp"
#endif

#include "core/fwk.h"

#include "LinkMonitor.h"
#include "wd_logging.h"

#include <cstdio>
using std::sscanf;

LinkMonitor::LinkMonitor(ATCommandsInterface* pIf)
	: m_pIf(pIf)
	, m_rssi(0)
	, m_gsmRegistrationState(REGISTRATION_STATE_UNKNOWN)
	, m_gprsRegistrationState(REGISTRATION_STATE_UNKNOWN)
	, m_bearer(BEARER_UNKNOWN)
	, m_locationAreaCode()
	, m_cellId()
{
}

int LinkMonitor::Init()
{
	return OK;
}

/*virtual*/ int LinkMonitor::onNewATResponseLine(ATCommandsInterface* pInst, const char* line)
{
	wd_log_debug("LinkMonitor --> Line is % s", line);
	char n[32] = "";
	char s[32] = "";
	char lac[4] = "";
	char ci[4] = "";
	int v;
	if (sscanf(line, "+CREG: %*d,%d", &v) >= 1) //Reg state is valid
	{
		wd_log_info("LinkMonitor --> +CREG %d", v);
		switch (v)
		{
		case 0:
			m_gsmRegistrationState = REGISTRATION_STATE_UNKNOWN;
			break;
		case 1:
			m_gsmRegistrationState = REGISTRATION_STATE_HOME_NETWORK;
			break;
		case 2:
			m_gsmRegistrationState = REGISTRATION_STATE_REGISTERING;
			break;
		case 3:
			m_gsmRegistrationState = REGISTRATION_STATE_DENIED;  
			break;
		case 4:
			m_gsmRegistrationState = REGISTRATION_STATE_NO_SIGNAL;  
			break;        
		case 5:
			m_gsmRegistrationState = REGISTRATION_STATE_ROAMING;  
			break;
		default:
			m_gsmRegistrationState = REGISTRATION_STATE_UNKNOWN;  
			break;
		}
	}
	else if (sscanf(line, "+CGREG: %*d,%d,\"%4c\",\"%4c\"", &v, lac, ci) >= 1) //Reg state is valid	%*c%[4]c%*c
	{
		wd_log_info("LinkMonitor --> +CGREG %d", v);
		switch (v)
		{
		case 0:
			m_gprsRegistrationState = REGISTRATION_STATE_UNKNOWN;
			break;
		case 1:
			m_gprsRegistrationState = REGISTRATION_STATE_HOME_NETWORK;
			break;
		case 2:
			m_gprsRegistrationState = REGISTRATION_STATE_REGISTERING;
			break;
		case 3:
			m_gprsRegistrationState = REGISTRATION_STATE_DENIED;  
			break;
		case 4:
			m_gprsRegistrationState = REGISTRATION_STATE_NO_SIGNAL;  
			break;        
		case 5:
			m_gprsRegistrationState = REGISTRATION_STATE_ROAMING;  
			break;
		default:
			m_gprsRegistrationState = REGISTRATION_STATE_UNKNOWN;  
			break;
		}
		
		if (m_gprsRegistrationState == REGISTRATION_STATE_HOME_NETWORK || m_gprsRegistrationState == REGISTRATION_STATE_ROAMING) {
			memset(m_locationAreaCode, '\0', sizeof(m_locationAreaCode));
			memcpy(m_locationAreaCode, lac, 4);
			memset(m_cellId, '\0', sizeof(m_locationAreaCode));
			memcpy(m_cellId, ci, 4);
		}
	}
	else if (sscanf(line, "+COPS: %*d,%*d,\"%*[^\"]\",%d", &v) >= 1)
	{
		wd_log_info("LinkMonitor --> +COPS %d", v);
		switch (v)
		{
		case 0:
			m_bearer = BEARER_GSM;
			break;
		case 2:
			m_bearer = BEARER_UMTS;
			break;
		case 3:
			m_bearer = BEARER_EDGE;
			break;
		case 4: //HSDPA
		case 5: //HSUPA
		case 6: //HSDPA + HSUPA
			m_bearer = BEARER_HSPA;
			break;
		case 7:
			m_bearer = BEARER_LTE;
			break;  
		case 1: //GSM Compact
		default:
			m_bearer = BEARER_UNKNOWN;
			break;
		}
	}
	else if (sscanf(line, "+CSQ: %d,%*d", &v) >= 1)
	{
		wd_log_info("LinkMonitor --> + CSQ % d", v);
		if (v == 99) //Unknown
		{
			m_rssi = 0; //Unknown
		}
		else
		{
			m_rssi = -113 + 2*v;
		}
	}
	else if ((sscanf(line, "+CNUM: \"%[^\"]\",\"%[^\"]\",%d", n, s, &v) == 3) || 
	          (sscanf(line, "+CNUM: \"\",\"%[^\"]\",%d", s, &v) == 2))
	{
		if (*s && ((v == 145/*number includes + */) || (v == 129/*otherwise*/))) {
			strcpy(m_phoneNumber, s);
		}
	}
	return OK;
}

/*virtual*/ int LinkMonitor::onNewEntryPrompt(ATCommandsInterface* pInst)
{
  return OK;
}

int LinkMonitor::GetState(int* pRssi, REGISTRATION_STATE* pGsmRegistrationState, REGISTRATION_STATE* pGprsRegistrationState, BEARER* pBearer, char * locationAreaCode, char * cellId, int timeout)
{
	m_rssi = 0;
	m_gsmRegistrationState = REGISTRATION_STATE_UNKNOWN;
	m_gprsRegistrationState = REGISTRATION_STATE_UNKNOWN;
	m_bearer = BEARER_UNKNOWN;
	int ret = m_pIf->execute("AT+CREG?;+CGREG?;+COPS?;+CSQ", this, NULL, timeout); //Configure to get registration info & get it; get signal quality
	if (ret != OK)
	{
		return NET_PROTOCOL;
	}
	*pRssi = m_rssi;
	*pGsmRegistrationState = m_gsmRegistrationState;
	*pGprsRegistrationState = m_gprsRegistrationState;
	*pBearer = m_bearer;
	strcpy(locationAreaCode, m_locationAreaCode);
	strcpy(cellId, m_cellId);
	
	return OK;
}

int LinkMonitor::GetPhoneNumber(char* phoneNumber, int timeout) {
	
	*m_phoneNumber = '\0';
	int ret = m_pIf->execute("AT+CNUM", this, NULL, timeout);
	if (ret != OK)
	{
		return NET_PROTOCOL;
	}
	strcpy(phoneNumber, m_phoneNumber);
	return OK;
	
}