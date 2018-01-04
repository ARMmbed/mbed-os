/*
 * QuectelM66CommandCoordinator.h
 *
 * Copyright (C) 2017 world-direct.at, MIT License
 *
 * Created: 21.04.2017
 * Author:	Simon Pfeifhofer
 * EMail:	simon.pfeifhofer@world-direct.at
 *
 * Description:
 *	 Contains a coordinator which
 *		(a) encapsulates the startup-sequence for the modem and 
 *		(b) starts the PPP-communication by dialling.
 *
 */ 

#ifndef QUECTELM66COORDINATOR_H
#define QUECTELM66COORDINATOR_H

#include "mbed.h"
#include <stdint.h>
#include <features/netsocket/nsapi_types.h>
#include "ATCommandsInterface.h"
#include "LinkMonitor.h"
#include "BasicInformationsProcessor.h"

/** 
	Coordinates power-sequence, AT-commands and PPP-connection
 */
class QuectelM66CommandCoordinator {
public:
	
	QuectelM66CommandCoordinator(IOStream* serialStream, PinName pwrKey, PinName vdd_ext, const char *apn);

	virtual ~QuectelM66CommandCoordinator();
	
	bool startupPPP();
	
	bool startupAT();
	
	bool shutdown();

    bool reset();
	
	LinkMonitor::REGISTRATION_STATE GetGSMRegistrationState();
	
	LinkMonitor::REGISTRATION_STATE GetGPRSRegistrationState();
	
	LinkMonitor::BEARER GetBearer();
	
	int GetRSSI();
	
	char* GetPhoneNumber();
	
	char * GetLocationAreaCode(void);
	
	char * GetCellId(void);
	
	char* GetICCID();
	
	char* GetIMEI();
	
	bool TestATOK();

private:
	
	QuectelM66CommandCoordinator(ATCommandsInterface* atCommandsInterface, PinName pwrKey, PinName vdd_ext, const char *apn);
	
	const char* _apn;
	
    ATCommandsInterface* _atCommandInterface;
	LinkMonitor* _linkMonitor;
	BasicInformationsProcessor* _basicInformationsProcessor;
	int _rssi;
	LinkMonitor::REGISTRATION_STATE _gsmRegistrationState;
	LinkMonitor::REGISTRATION_STATE _gprsRegistrationState;
	LinkMonitor::BEARER _bearer;
	char _phoneNumber[16];
	char _locationAreaCode[5];
	char _cellId[5];
	
	DigitalOut _pwrKeyPin;
	DigitalIn _vdd_extPin;
	
	bool pppPreparation();
	void executePowerOnSequence();
	bool setupATCommandInterface();
	
};

#endif // QUECTELM66COORDINATOR_H
