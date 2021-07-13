/***************************************************************************//**
* \file cyhal_pdmpcm_impl.h
*
* Description:
* Provides a high level interface for interacting with the Cypress PDM/PCM converted.
*
********************************************************************************
* \copyright
* Copyright 2019-2021 Cypress Semiconductor Corporation
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

#ifdef CY_IP_MXAUDIOSS_INSTANCES

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/** The PGA gain in units of 0.5 dB (-12 dB) */
#define CYHAL_PDM_PCM_MIN_GAIN -24
/** The PGA gain in units of 0.5 dB (10.5 dB) */
#define CYHAL_PDM_PCM_MAX_GAIN 21
/** The rate of which PCM data is generated. This value is halved for mono channel. */
#define CYHAL_PDM_PCM_MAX_SAMPLE_RATE 48000
#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* defined(CY_IP_MXAUDIOSS_INSTANCES) */
