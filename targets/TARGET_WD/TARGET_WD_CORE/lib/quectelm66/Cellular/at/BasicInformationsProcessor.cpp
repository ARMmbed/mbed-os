#include "BasicInformationsProcessor.h"

BasicInformationsProcessor::BasicInformationsProcessor(ATCommandsInterface* atCommandsInterface){
	this->_atCommandsInterface = atCommandsInterface;
}

char* BasicInformationsProcessor::GetIMEI() {
	
	ATCommandsInterface::ATResult result;
	if (this->_atCommandsInterface->execute("AT+GSN", this, &result, 500) != 0) {
		return NULL;
	}
	if (result.result != ATCommandsInterface::ATResult::AT_OK) {
		return NULL;
	}
	
	return this->_imei;
	
}

char* BasicInformationsProcessor::GetICCID(){
	
	ATCommandsInterface::ATResult result;
	if (this->_atCommandsInterface->execute("AT+QCCID", this, &result, 500) != 0) {
		return NULL;
	}
	if (result.result != ATCommandsInterface::ATResult::AT_OK) {
		return NULL;
	}
	
	return this->_iccid;
	
}

int BasicInformationsProcessor::onNewATResponseLine(ATCommandsInterface* pInst, const char* line){
	
	char parsedLine[30] = {};
	if (sscanf(line, "%[0123456789]", parsedLine) > 0) {
		
		if (strlen(parsedLine) > 0 && strlen(parsedLine) < 18) {
			sprintf(this->_imei, "%s", parsedLine);	
		}
		
		if (strlen(parsedLine) > 18 && strlen(parsedLine) < 23) {
			sprintf(this->_iccid, "%s", parsedLine);	
		}
		
	}
	
	return OK ;
}

int BasicInformationsProcessor::onNewEntryPrompt(ATCommandsInterface* pInst){
	return OK;
}