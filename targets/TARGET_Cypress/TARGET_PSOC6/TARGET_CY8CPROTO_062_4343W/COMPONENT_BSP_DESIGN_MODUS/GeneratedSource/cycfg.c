/*******************************************************************************
* File Name: cycfg.c
*
* Description:
* Wrapper function to initialize all generated code.
* This file was automatically generated and should not be modified.
* Device Configurator: 2.0.0.1483
* Device Support Library (../../../../output/psoc6/psoc6pdl): 1.4.0.1571
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

#include "cycfg.h"

void init_cycfg_all(void)
{
	init_cycfg_system();
	init_cycfg_clocks();
	init_cycfg_routing();
	init_cycfg_peripherals();
	init_cycfg_pins();
}
