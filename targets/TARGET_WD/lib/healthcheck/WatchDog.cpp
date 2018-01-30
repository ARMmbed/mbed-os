/*
 * WatchDog.cpp
 *
 * Copyright (C) 2017 world-direct.at, MIT License
 *
 * Created: 21.06.2017
 * Author:	Simon Pfeifhofer
 * EMail:	simon.pfeifhofer@world-direct.at
 *
 * Description:
 *		Provides the watch-dog-functionality (IWDG)
 *
 */ 

#include "WatchDog.h"
#include "wd_logging.h"

WatchDog::WatchDog(){
	
}

void WatchDog::Start(uint32_t prescaler /* = IWDG_PRESCALER_256 */, uint32_t reload /* = 0x0FFFU */, float healthCheckIntervalSeconds /*= 5*/, char * context /*= NULL*/) {
	
	_context = context;
	
	/*
		Watchdog calculation:
			
			General:
				Timeout = (Clock ^ (-1)) * Prescaler * Downcounter
			
			Example (based on default-settings):
				Clock:			32		kHz
				Prescaler:		256		(0xF)
				Downcounter:	4096	(0xFFF)
				Timeout = (32000 (1/s))^(-1) * 256 * 4096 = 32,768 seconds
			
	*/
	
	_handle.Instance = IWDG;
	_handle.Init.Prescaler = prescaler; // e.g.: IWDG_PRESCALER_256
	_handle.Init.Reload = reload;		// Range: 0x0000, 0x0FFFU
	
	if(HAL_IWDG_Init(&_handle) != HAL_OK){
		wd_log_error("WatchDog cannot be initialized");
		mbed_die();
	}
	
	events::EventQueue * eq = mbed_event_queue();
	eq->call_every(healthCheckIntervalSeconds * 1000, callback(this, &WatchDog::Check));

}


void WatchDog::Kick() {
	
	HAL_IWDG_Refresh(&_handle);
	
}

void WatchDog::RegisterHealthCheck(HealthCheckBase * healthCheck) {
	
	_healthChecks.push_back(healthCheck);
	
}

void WatchDog::Check(){
	
	for (vector<HealthCheckBase*>::iterator it = _healthChecks.begin(); it != _healthChecks.end(); ++it) {
		
		if((*it)->GetState(this->_context) != HealthCheckBase::HEALTHIE){
			
			// HealthCheck failed, system is in an unhealthy state --> danger of reset
			wd_log_warn("WatchDog --> Failed by %s", (*it)->GetFriendlyName());
			return;
			
		}
		
	}
	
	// All checks succeeded
	// Kick the watchdog to avoid reset
	wd_log_info("WatchDog --> Kick");
	Kick();
	
}
