/*
 * BasicInformationsProcessor.h
 *
 * Copyright (C) 2017 world-direct.at, MIT License
 *
 * Created: 07.07.2017
 * Author:	Simon Pfeifhofer
 * EMail:	simon.pfeifhofer@world-direct.at
 *
 * Description:
 *	 Contains an AT-command-processor which evaluates basic-AT-commands 
 *
 */ 

#ifndef BASICINFORMATIONSPROCESSOR_H_
#define BASICINFORMATIONSPROCESSOR_H_

#include "ATCommandsInterface.h"

class BasicInformationsProcessor : public IATCommandsProcessor {
	
	public:
		BasicInformationsProcessor(ATCommandsInterface* atCommandsInterface);
		char* GetIMEI();
		char* GetICCID();
	
	protected:
		int onNewATResponseLine(ATCommandsInterface* pInst, const char* line);
		int onNewEntryPrompt(ATCommandsInterface* pInst);
	
	private:
		ATCommandsInterface* _atCommandsInterface;
		char _imei[16] = { };
		char _iccid[24] = { };
	
};

#endif // BASICINFORMATIONSPROCESSOR_H_