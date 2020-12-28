/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/


#ifndef MBED_MBED_RTX_H
#define MBED_MBED_RTX_H
#include "memory_zones.h"

#if defined(TARGET_S1SBP6A)

#ifndef INITIAL_SP
#define INITIAL_SP   (MBED_RAM_START + MBED_RAM_SIZE)
#endif

#endif /* defined(TARGET_...) */

#endif  /* MBED_MBED_RTX_H */
