/*
 * WatchDog.h
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

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include "mbed.h"
#include "HealthCheckBase.h"
#include <vector>

class WatchDog {
	
	private:
	
		IWDG_HandleTypeDef _handle;
		Ticker _healthCheckTicker;
		vector<HealthCheckBase *> _healthChecks;
		char * _context;
	
		void Check();
	
	public:
	
		WatchDog();
	
		/* 
			Configures the Watchdog
		*/
		void Start(uint32_t prescaler = IWDG_PRESCALER_256, uint32_t reload = 0x0FFFU, float healthCheckIntervalSeconds = 5, char * context = NULL);
	
		/* 
			Resets the counter to avoid a system-reset
		*/
		void Kick();
	
		/*
			Registers a new HealthCheck
		*/
		void RegisterHealthCheck(HealthCheckBase * healthCheck);
	
};

#endif


