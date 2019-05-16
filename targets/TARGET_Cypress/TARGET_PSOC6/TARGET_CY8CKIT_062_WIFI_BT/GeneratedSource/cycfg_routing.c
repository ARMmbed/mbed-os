/*******************************************************************************
* File Name: cycfg_routing.c
*
* Description:
* Establishes all necessary connections between hardware elements.
* This file was automatically generated and should not be modified.
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

#include "cy_trigmux.h"

#include "stdbool.h"

#include "cy_device_headers.h"

void init_cycfg_routing(void)
{
	Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP14_OUTPUT3, TRIG0_OUT_CPUSS_DW0_TR_IN0, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP14_OUTPUT4, TRIG0_OUT_CPUSS_DW0_TR_IN1, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB0, TRIG14_OUT_TR_GROUP1_INPUT44, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB1, TRIG14_OUT_TR_GROUP0_INPUT47, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB3, TRIG14_OUT_TR_GROUP0_INPUT46, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB7, TRIG14_OUT_TR_GROUP1_INPUT43, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG1_IN_TR_GROUP14_OUTPUT0, TRIG1_OUT_CPUSS_DW1_TR_IN3, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG1_IN_TR_GROUP14_OUTPUT1, TRIG1_OUT_CPUSS_DW1_TR_IN1, false, TRIGGER_TYPE_LEVEL);
	HSIOM->AMUX_SPLIT_CTL[2] = HSIOM_AMUX_SPLIT_CTL_SWITCH_AA_SL_Msk |
		HSIOM_AMUX_SPLIT_CTL_SWITCH_AA_SR_Msk |
		HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk |
		HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;
	HSIOM->AMUX_SPLIT_CTL[4] = HSIOM_AMUX_SPLIT_CTL_SWITCH_AA_SL_Msk |
		HSIOM_AMUX_SPLIT_CTL_SWITCH_AA_SR_Msk |
		HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk |
		HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;
}
