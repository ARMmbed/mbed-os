/* MPS2 CMSIS Library
 *
 * Copyright (c) 2006-2017 ARM Limited
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
 *
 * This file is derivative from the MPS2 Selftest implementation
 * MPS2 Selftest: https://silver.arm.com/browse/VEI10 ->
 *     \ISCM-1-0\AN491\software\Selftest\v2m_mps2\peripherallink.h
 *
 *******************************************************************************
 * Name:    peripherallink.h
 * Purpose: Include the correct device header file
 *******************************************************************************/

#ifndef __DEVICE_H
#define __DEVICE_H

#if defined CMSDK_CM3DS
  #include "CMSDK_CM3DS.h"                         /* device specific header file */
#else
  #warning "no appropriate header file found!"
#endif

#endif /* __DEVICE_H */
