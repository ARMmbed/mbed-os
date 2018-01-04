/* 
* ResettableTimeout.cpp
*
* Created: 31.07.2017 15:14:44
* Author: Patrick.Frischmann
*/


#include "ResettableTimeout.h"

// default constructor
ResettableTimeout::ResettableTimeout(Callback<void()> func, timestamp_t us) {
	this->_function = func;
	this->_delay = us;
} //ResettableTimeout

 
// default destructor
ResettableTimeout::~ResettableTimeout() {} //~ResettableTimeout

void ResettableTimeout::stop(void) {

	core_util_critical_section_enter();
	this->remove();
	core_util_critical_section_exit();

}

void ResettableTimeout::reset(void) {
	
	this->setup(this->_delay);

}

void ResettableTimeout::reset(timestamp_t us) {
	
	this->_delay = us;
	this->reset();
	
}
