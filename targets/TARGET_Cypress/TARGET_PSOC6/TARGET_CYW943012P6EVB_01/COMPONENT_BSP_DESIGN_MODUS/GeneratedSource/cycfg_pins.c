/*******************************************************************************
* File Name: cycfg_pins.c
*
* Description:
* Pin configuration
* This file was automatically generated and should not be modified.
* cfg-backend-cli: 1.2.0.1478
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

#include "cycfg_pins.h"

const cy_stc_gpio_pin_config_t CYBSP_WCO_IN_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_WCO_IN_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_WCO_IN_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_WCO_IN_PORT_NUM,
		.channel_num = CYBSP_WCO_IN_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_WCO_OUT_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_WCO_OUT_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_WCO_OUT_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_WCO_OUT_PORT_NUM,
		.channel_num = CYBSP_WCO_OUT_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_ECO_IN_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_ECO_IN_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_ECO_IN_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_ECO_IN_PORT_NUM,
		.channel_num = CYBSP_ECO_IN_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_ECO_OUT_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_ECO_OUT_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_ECO_OUT_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_ECO_OUT_PORT_NUM,
		.channel_num = CYBSP_ECO_OUT_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_CSD_TX_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_CSD_TX_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_CSD_TX_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_CSD_TX_PORT_NUM,
		.channel_num = CYBSP_CSD_TX_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_SWO_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = CYBSP_SWO_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_SWO_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_SWO_PORT_NUM,
		.channel_num = CYBSP_SWO_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_SWDIO_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_PULLUP,
	.hsiom = CYBSP_SWDIO_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_SWDIO_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_SWDIO_PORT_NUM,
		.channel_num = CYBSP_SWDIO_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_SWCLK_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_PULLDOWN,
	.hsiom = CYBSP_SWCLK_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_SWCLK_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_SWCLK_PORT_NUM,
		.channel_num = CYBSP_SWCLK_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_TRACECLK_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = CYBSP_TRACECLK_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_TRACECLK_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_TRACECLK_PORT_NUM,
		.channel_num = CYBSP_TRACECLK_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_CINTA_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_CINTA_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_CINTA_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_CINTA_PORT_NUM,
		.channel_num = CYBSP_CINTA_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_CINTB_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_CINTB_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_CINTB_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_CINTB_PORT_NUM,
		.channel_num = CYBSP_CINTB_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_CMOD_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_CMOD_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_CMOD_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_CMOD_PORT_NUM,
		.channel_num = CYBSP_CMOD_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_CSD_BTN0_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_CSD_BTN0_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_CSD_BTN0_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_CSD_BTN0_PORT_NUM,
		.channel_num = CYBSP_CSD_BTN0_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_CSD_BTN1_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_CSD_BTN1_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_CSD_BTN1_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_CSD_BTN1_PORT_NUM,
		.channel_num = CYBSP_CSD_BTN1_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD0_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_CSD_SLD0_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_CSD_SLD0_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_CSD_SLD0_PORT_NUM,
		.channel_num = CYBSP_CSD_SLD0_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD1_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_CSD_SLD1_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_CSD_SLD1_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_CSD_SLD1_PORT_NUM,
		.channel_num = CYBSP_CSD_SLD1_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD2_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_CSD_SLD2_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_CSD_SLD2_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_CSD_SLD2_PORT_NUM,
		.channel_num = CYBSP_CSD_SLD2_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD3_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_CSD_SLD3_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_CSD_SLD3_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_CSD_SLD3_PORT_NUM,
		.channel_num = CYBSP_CSD_SLD3_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD4_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_CSD_SLD4_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_CSD_SLD4_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_CSD_SLD4_PORT_NUM,
		.channel_num = CYBSP_CSD_SLD4_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_TRACEDATA3_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = CYBSP_TRACEDATA3_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_TRACEDATA3_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_TRACEDATA3_PORT_NUM,
		.channel_num = CYBSP_TRACEDATA3_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_TRACEDATA2_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = CYBSP_TRACEDATA2_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_TRACEDATA2_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_TRACEDATA2_PORT_NUM,
		.channel_num = CYBSP_TRACEDATA2_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_TRACEDATA1_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = CYBSP_TRACEDATA1_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_TRACEDATA1_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_TRACEDATA1_PORT_NUM,
		.channel_num = CYBSP_TRACEDATA1_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_TRACEDATA0_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = CYBSP_TRACEDATA0_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_TRACEDATA0_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_TRACEDATA0_PORT_NUM,
		.channel_num = CYBSP_TRACEDATA0_PIN,
	};
#endif //defined (CY_USING_HAL)


void init_cycfg_pins(void)
{
	Cy_GPIO_Pin_Init(CYBSP_WCO_IN_PORT, CYBSP_WCO_IN_PIN, &CYBSP_WCO_IN_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_WCO_IN_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_WCO_OUT_PORT, CYBSP_WCO_OUT_PIN, &CYBSP_WCO_OUT_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_WCO_OUT_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_ECO_IN_PORT, CYBSP_ECO_IN_PIN, &CYBSP_ECO_IN_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_ECO_IN_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_ECO_OUT_PORT, CYBSP_ECO_OUT_PIN, &CYBSP_ECO_OUT_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_ECO_OUT_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_CSD_TX_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_SWO_PORT, CYBSP_SWO_PIN, &CYBSP_SWO_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_SWO_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_SWDIO_PORT, CYBSP_SWDIO_PIN, &CYBSP_SWDIO_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_SWDIO_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_SWCLK_PORT, CYBSP_SWCLK_PIN, &CYBSP_SWCLK_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_SWCLK_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_TRACECLK_PORT, CYBSP_TRACECLK_PIN, &CYBSP_TRACECLK_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_TRACECLK_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_CINTA_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_CINTB_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_CMOD_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_CSD_BTN0_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_CSD_BTN1_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_CSD_SLD0_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_CSD_SLD1_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_CSD_SLD2_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_CSD_SLD3_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_CSD_SLD4_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_TRACEDATA3_PORT, CYBSP_TRACEDATA3_PIN, &CYBSP_TRACEDATA3_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_TRACEDATA3_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_TRACEDATA2_PORT, CYBSP_TRACEDATA2_PIN, &CYBSP_TRACEDATA2_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_TRACEDATA2_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_TRACEDATA1_PORT, CYBSP_TRACEDATA1_PIN, &CYBSP_TRACEDATA1_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_TRACEDATA1_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_TRACEDATA0_PORT, CYBSP_TRACEDATA0_PIN, &CYBSP_TRACEDATA0_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_TRACEDATA0_obj);
#endif //defined (CY_USING_HAL)
}
