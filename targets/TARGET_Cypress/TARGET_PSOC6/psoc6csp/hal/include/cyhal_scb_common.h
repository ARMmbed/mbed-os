/***************************************************************************//**
* \file cyhal_scb_common.h
*
* \brief
* Provides a struct definitions for configuration resources in the SCB (UART, I2C, SPI).
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

#include "cy_device.h"
#include "cy_pdl.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** The start address of the SCB blocks */
extern CySCB_Type* CY_SCB_BASE_ADDRESSES[CY_IP_MXSCB_INSTANCES];
/** The interrupt number of the SCB blocks. */
extern IRQn_Type CY_SCB_IRQ_N[CY_IP_MXSCB_INSTANCES];

#if defined(__cplusplus)
}
#endif
