/* 
* BTSlaveEntry.h
*
* Created: 05.12.2017 13:12:26
* Author: Patrick.Frischmann
*/


#ifndef __BTSLAVEENTRY_H__
#define __BTSLAVEENTRY_H__

#include <time.h>

#define BT_SLAVE_COLLECTION_MAX_ERROR_COUNT				10

class BTSlaveEntry
{
//variables
public:
protected:
private:
	uint64_t _id;
	time_t _lastMsgTimestamp;
	uint8_t _errorCount;

//functions
public:
	BTSlaveEntry(uint64_t slaveId) {
		_id = slaveId;
		_errorCount = 0;
		_lastMsgTimestamp = time(NULL);
	};
	~BTSlaveEntry(){};
	
	uint64_t getId(void) const { return _id; };
	void increaseErrorCount(void) { _errorCount++; };
	void resetErrorCount(void) { _errorCount = 0; };
	bool errorCountExhausted(void) { return _errorCount > BT_SLAVE_COLLECTION_MAX_ERROR_COUNT; };
	
	void updateMsgTimestamp(void) { _lastMsgTimestamp = time(NULL); };
	bool communicationTimeoutReached(unsigned int timeout) { return time(NULL) > (_lastMsgTimestamp + timeout); };
		
protected:
private:

}; //BTSlaveEntry

#endif //__BTSLAVEENTRY_H__
