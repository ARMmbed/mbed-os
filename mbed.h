/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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

#include "platform/mbed_version.h"

#if MBED_CONF_RTOS_API_PRESENT
#include "rtos/rtos.h"
#endif

#if MBED_CONF_NSAPI_PRESENT
#include "netsocket/nsapi.h"
#include "netsocket/nsapi_ppp.h"
#endif

#if MBED_CONF_EVENTS_PRESENT
#include "events/mbed_events.h"
#endif

#if MBED_CONF_FILESYSTEM_PRESENT
#include "filesystem/mbed_filesystem.h"
#endif

#include "platform/mbed_toolchain.h"
#include "platform/platform.h"
#include "platform/mbed_application.h"

// Useful C libraries
#include <math.h>
#include <time.h>

// mbed Debug libraries
#include "platform/mbed_error.h"
#include "platform/mbed_interface.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_debug.h"

// mbed Peripheral components
#include "drivers/DigitalIn.h"
#include "drivers/DigitalOut.h"
#include "drivers/DigitalInOut.h"
#include "drivers/BusIn.h"
#include "drivers/BusOut.h"
#include "drivers/BusInOut.h"
#include "drivers/PortIn.h"
#include "drivers/PortInOut.h"
#include "drivers/PortOut.h"
#include "drivers/AnalogIn.h"
#include "drivers/AnalogOut.h"
#include "drivers/PwmOut.h"
#include "drivers/SPI.h"
#include "drivers/SPISlave.h"
#include "drivers/I2C.h"
#include "drivers/I2CSlave.h"
#include "drivers/CAN.h"
#include "drivers/UnbufferedSerial.h"
#include "drivers/BufferedSerial.h"
#include "drivers/FlashIAP.h"
#include "drivers/MbedCRC.h"
#include "drivers/QSPI.h"
#include "drivers/Watchdog.h"

// mbed Internal components
#include "drivers/ResetReason.h"
#include "drivers/HighResClock.h"
#include "drivers/Timer.h"
#include "drivers/Ticker.h"
#include "drivers/Timeout.h"
#include "drivers/LowPowerClock.h"
#include "drivers/LowPowerTimeout.h"
#include "drivers/LowPowerTicker.h"
#include "drivers/LowPowerTimer.h"
#include "drivers/RealTimeClock.h"
#include "platform/LocalFileSystem.h"
#include "drivers/InterruptIn.h"
#include "platform/mbed_wait_api.h"
#include "platform/mbed_thread.h"
#include "hal/sleep_api.h"
#include "platform/mbed_atomic.h"
#include "platform/mbed_power_mgmt.h"
#include "platform/mbed_rtc_time.h"
#include "platform/mbed_poll.h"
#include "platform/ATCmdParser.h"
#include "platform/FileSystemHandle.h"
#include "platform/FileHandle.h"
#include "platform/DirHandle.h"
#include "platform/CriticalSectionLock.h"
#include "platform/DeepSleepLock.h"
#include "platform/ScopedRomWriteLock.h"
#include "platform/ScopedRamExecutionLock.h"
#include "platform/mbed_stats.h"

// mbed Non-hardware components
#include "platform/Callback.h"
#include "platform/ScopedLock.h"

#ifndef MBED_NO_GLOBAL_USING_DIRECTIVE
using namespace mbed;
using namespace std;
#endif

#endif
