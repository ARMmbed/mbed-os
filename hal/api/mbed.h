/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_H
#define MBED_H

#define MBED_LIBRARY_VERSION 123

#if MBED_CONF_RTOS_PRESENT
#include "rtos/rtos.h"
#endif

#if MBED_CONF_NSAPI_PRESENT
#include "network-socket/nsapi.h"
#endif

#include "toolchain.h"
#include "platform.h"

// Useful C libraries
#include <math.h>
#include <time.h>

// mbed Debug libraries
#include "mbed_error.h"
#include "mbed_interface.h"
#include "mbed_assert.h"

// mbed Peripheral components
#include "DigitalIn.h"
#include "DigitalOut.h"
#include "DigitalInOut.h"
#include "BusIn.h"
#include "BusOut.h"
#include "BusInOut.h"
#include "PortIn.h"
#include "PortInOut.h"
#include "PortOut.h"
#include "AnalogIn.h"
#include "AnalogOut.h"
#include "PwmOut.h"
#include "Serial.h"
#include "SPI.h"
#include "SPISlave.h"
#include "I2C.h"
#include "I2CSlave.h"
#include "Ethernet.h"
#include "CAN.h"
#include "RawSerial.h"

// mbed Internal components
#include "Timer.h"
#include "Ticker.h"
#include "Timeout.h"
#include "LowPowerTimeout.h"
#include "LowPowerTicker.h"
#include "LowPowerTimer.h"
#include "LocalFileSystem.h"
#include "InterruptIn.h"
#include "wait_api.h"
#include "sleep_api.h"
#include "rtc_time.h"

// mbed Non-hardware components
#include "Callback.h"
#include "FunctionPointer.h"

// expansion of the std namespace
using namespace std;

// explicit expansion of mbed classes
using mbed::DigitalIn;
using mbed::DigitalOut;
using mbed::DigitalInOut;
using mbed::BusIn;
using mbed::BusOut;
using mbed::BusInOut;
#if DEVICE_PORTIN
using mbed::PortIn;
#endif
#if DEVICE_PORTINOUT
using mbed::PortInOut;
#endif
#if DEVICE_PORTOUT
using mbed::PortOut;
#endif
#if DEVICE_ANALOGIN
using mbed::AnalogIn;
#endif
#if DEVICE_ANALOGOUT
using mbed::AnalogOut;
#endif
#if DEVICE_PWMOUT
using mbed::PwmOut;
#endif
#if DEVICE_SERIAL
using mbed::SerialBase;
using mbed::Serial;
using mbed::RawSerial;
#endif
#if DEVICE_SPI
using mbed::SPI;
#endif
#if DEVICE_SPISLAVE
using mbed::SPISlave;
#endif
#if DEVICE_I2C
using mbed::I2C;
#endif
#if DEVICE_I2CSLAVE
using mbed::I2CSlave;
#endif
#if DEVICE_ETHERNET
using mbed::Ethernet;
#endif
#if DEVICE_CAN
using mbed::CAN;
#endif
using mbed::Timer;
using mbed::Ticker;
using mbed::Timeout;
#if DEVICE_LOWPOWERTIMER
using mbed::LowPowerTimeout;
using mbed::LowPowerTicker;
using mbed::LowPowerTimer;
#endif
#if DEVICE_LOCALFILESYSTEM
using mbed::LocalFileSystem;
#endif
#if DEVICE_INTERRUPTIN
using mbed::InterruptIn;
#endif
using mbed::Stream;
using mbed::Callback;
using mbed::FunctionPointer;


#endif
