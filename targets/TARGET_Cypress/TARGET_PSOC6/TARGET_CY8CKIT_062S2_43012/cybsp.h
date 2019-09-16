/***************************************************************************//**
* \file CY8CKIT-062S2-43012/cybsp.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CKIT-062S2-43012 pioneer kit.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
*******************************************************************************/

#pragma once

#include "cybsp_types.h"
#include "cybsp_core.h"
#if defined(CYBSP_WIFI_CAPABLE)
#include "cybsp_wifi_sdio.h"
#endif
#ifndef __MBED__
#include "cybsp_retarget.h"
#include "cybsp_serial_flash.h"
#include "cybsp_rgb_led.h"
#endif /* __MBED__ */

#if defined(__cplusplus)
extern "C" {
#endif


#if defined(__cplusplus)
}
#endif
