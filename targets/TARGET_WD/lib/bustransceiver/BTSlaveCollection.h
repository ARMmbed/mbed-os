/* 
* BTSlaveCollection.h
*
* Created: 04.12.2017 13:01:10
* Author: Patrick.Frischmann
*/


#ifndef __BTSLAVECOLLECTION_H__
#define __BTSLAVECOLLECTION_H__

#include <vector>
#include <iterator>
#include "BTSlaveEntry.h"

#define BT_SLAVE_COLLECTION_DEFAULT_COMM_TIMEOUT_MS		1500

class BTSlaveCollection
{
//variables
public:
protected:
private:
int _commTimeout;
unsigned int _currentSlaveIndex;
std::vector<BTSlaveEntry> _entries;

//functions
public:
	BTSlaveCollection(int communicationTimeout = BT_SLAVE_COLLECTION_DEFAULT_COMM_TIMEOUT_MS) {
		_commTimeout = communicationTimeout;
		_currentSlaveIndex = 0;
	};
	
	~BTSlaveCollection() {
		_entries.clear();	
	};
	
	void add(uint64_t slaveId) {
		BTSlaveEntry se(slaveId);
		remove(slaveId);
		_entries.push_back(se);
	};
	
	void remove(uint64_t slaveId) {
		std::vector<BTSlaveEntry>::iterator it = _entries.begin();
		for( ; it != _entries.end(); ) {
			if (slaveId == (*it).getId()) {
				it = _entries.erase(it);
			} else {
				++it;
			}
		}
	};
	
	uint64_t selectNext(void) {
		if(++_currentSlaveIndex >= getCount()) 
			_currentSlaveIndex = 0;
		
		return getCount() > 0 ? _entries.at(_currentSlaveIndex).getId() : 0;
	};
	
	uint64_t getId(int index) {
		if (index >=0 && index < getCount()) {
			return _entries[index].getId();
		}
		return 0;
	};
	
	void clear(void) {
		_entries.clear();
	};
	
	int getCount(void) { return _entries.size(); };
	
	void getIds(uint64_t * buffer) {
		for (typename std::vector<BTSlaveEntry>::iterator it = _entries.begin(); it != _entries.end(); it++) {
			(*buffer++) = (*it).getId();
		}
	};
	
	void reportSuccessfulCommunication(uint64_t slaveId) {
		for (typename std::vector<BTSlaveEntry>::const_iterator it = _entries.begin(); it != _entries.end(); it++) {
			if (slaveId == (*it).getId()) {
				(*it).resetErrorCount();
			}
		}
	};
	
	void reportErroneousCommunication(uint64_t slaveId) {
		for (typename std::vector<BTSlaveEntry>::const_iterator it = _entries.begin(); it != _entries.end(); it++) {
			if (slaveId == (*it).getId()) {
				(*it).increaseErrorCount();
			}
		}
	};
	
	bool isErrorCountExhausted(uint64_t slaveId) {
		for (typename std::vector<BTSlaveEntry>::const_iterator it = _entries.begin(); it != _entries.end(); it++) {
			if (slaveId == (*it).getId()) {
				return (*it).errorCountExhausted();
			}
		}
		return true;
	};
	
	void reportMsgSent(uint64_t slaveId) {
		for (typename std::vector<BTSlaveEntry>::const_iterator it = _entries.begin(); it != _entries.end(); it++) {
			if (slaveId == (*it).getId()) {
				(*it).updateMsgTimestamp();
			}
		}
	};
	
	uint64_t getCommunicationTimeoutReached(unsigned int timeout) {
		for (typename std::vector<BTSlaveEntry>::const_iterator it = _entries.begin(); it != _entries.end(); it++) {
			if ((*it).communicationTimeoutReached(timeout)) {
				return (*it).getId();
			}
		}
		return 0;
	}
	
	
protected:
private:

}; //BTSlaveCollection

#endif //__BTSLAVECOLLECTION_H__
