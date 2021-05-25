/***********************************************************************************************//**
 * \file cyabs_rtos_internal.h
 *
 * \brief
 * Internal interface used for RTOS abstraction utilities.
 ***************************************************************************************************
 * \copyright
 * Copyright 2018-2021 Cypress Semiconductor Corporation
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
 **************************************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

// Pick a header that will ultimately pull in the proper CMSIS device header file
#if defined(CY_USING_HAL)
#include "cyhal.h"
#elif defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B) || defined(COMPONENT_CAT2)
#include "cy_device_headers.h"
#elif defined(COMPONENT_CAT3)
#include "xmc_device.h"
#else
#error "Unsupported device"
#endif

/** Checks to see if code is currently executing within an interrupt context.
 *
 * @return Boolean indicating whether this was executed from an interrupt context.
 */
static inline bool is_in_isr()
{
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
}


#if defined(__cplusplus)
}
#endif
