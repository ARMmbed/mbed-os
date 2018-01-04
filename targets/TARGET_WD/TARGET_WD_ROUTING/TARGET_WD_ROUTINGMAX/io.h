#pragma once
#include "PinNames.h"

// Helpers
#include "AnalogInManager.h"
#include "AnalogOutManager.h"
#include "OneWire.h"
#include "DS18B20.h"

// Sensors
#include "SensorDigitalIn.h"
#include "SensorCurrentLoopIn.h"
#include "SensorPlatinumTemperature.h"

// Actors
#include "ActorDigitalOut.h"
#include "ActorCurrentLoopOut.h"

#define DINCount	10
#define AINCount	6
#define PTCount		2
#define DOUTCount	3
#define AOUTCount	2

extern struct RoutingmaxIO {
	SensorDigitalIn DINs[DINCount];
	SensorCurrentLoopIn AINs[AINCount];
	SensorPlatinumTemperature PTs[PTCount];
	DS18B20 OneWireDS18B20;
	ActorDigitalOut DOUTs[DOUTCount];
	ActorCurrentLoopOut AOUTs[AOUTCount];
} routingmax_io;

extern uint8_t board_hw_version;