/*
 * HealthCheckBase.h
 *
 * Copyright (C) 2017 world-direct.at, MIT License
 *
 * Created: 21.06.2017
 * Author:	Simon Pfeifhofer
 * EMail:	simon.pfeifhofer@world-direct.at
 *
 * Description:
 *		Provides the base class for the HealthCheck-framework
 *
 */ 

#ifndef HEALTHCHECKBASE_H_
#define HEALTHCHECKBASE_H_

class HealthCheckBase {
	
	public:
		
		enum HEALTH_STATE {
			HEALTHIE, 
			UNHEALTHIE
		};
	
		virtual HEALTH_STATE GetState(void * context) = 0;
	
};

#endif