/***************************************************************************//**
* \file cyhal_implementation.h
*
* \brief
* Provides references for the PSoC 6 specific implementation of the HAL drivers.
* This includes references to implementation specific header files and any
* supporting data types. This file should not be used directly. It should only
* be referenced by HAL drivers to pull in the implementation specific code.
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

/**
* \addtogroup group_hal_psoc6 PSoC 6 HAL Implementation
* \{
* PSoC 6 specific implementation of the HAL drivers
* \} group_hal_psoc6
*/

#pragma once

#include "cy_pdl.h"
#include "cyhal.h"
#include "cyhal_hw_types.h"
#include "cyhal_crc_impl.h"
#include "cyhal_gpio_impl.h"
#include "cyhal_scb_common.h"
#include "cyhal_utils.h"
#include "cyhal_system_impl.h"
#include "cyhal_trng_impl.h"
