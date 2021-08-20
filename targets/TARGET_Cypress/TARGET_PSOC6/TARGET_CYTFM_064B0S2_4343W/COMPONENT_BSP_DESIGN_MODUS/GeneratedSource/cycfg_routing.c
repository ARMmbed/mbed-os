/*******************************************************************************
* File Name: cycfg_routing.c
*
* Description:
* Establishes all necessary connections between hardware elements.
* This file was automatically generated and should not be modified.
* cfg-backend-cli: 1.2.0.1483
* Device Support Library (libs/psoc6pdl): 1.6.0.4266
*
********************************************************************************
* Copyright 2017-2019 Cypress Semiconductor Corporation
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
********************************************************************************/

#include "cycfg_routing.h"

#include "cy_device_headers.h"

void init_cycfg_routing(void)
{
	HSIOM->AMUX_SPLIT_CTL[2] = HSIOM_V2_AMUX_SPLIT_CTL_SWITCH_AA_SL_Msk |
		HSIOM_V2_AMUX_SPLIT_CTL_SWITCH_AA_SR_Msk |
		HSIOM_V2_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk |
		HSIOM_V2_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;
	HSIOM->AMUX_SPLIT_CTL[4] = HSIOM_V2_AMUX_SPLIT_CTL_SWITCH_AA_SL_Msk |
		HSIOM_V2_AMUX_SPLIT_CTL_SWITCH_AA_SR_Msk |
		HSIOM_V2_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk |
		HSIOM_V2_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;
}
