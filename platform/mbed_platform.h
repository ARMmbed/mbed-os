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
#ifndef MBED_PLATFORM_H
#define MBED_PLATFORM_H


// Toolchain definitions
#include "toolchain.h"

// Core hardware definitions
#include "device.h"
#include "PinNames.h"
#include "PeripheralNames.h"

// Useful C libraries
#include <math.h>
#include <time.h>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>

// mbed Debug libraries
#include "mbed_error.h"
#include "mbed_interface.h"
#include "mbed_assert.h"

// mbed Non-hardware components
#include "Callback.h"
#include "FunctionPointer.h"
#include "PlatformMutex.h"
#include "SingletonPtr.h"

using namespace mbed;
using namespace std;


#endif
