/*******************************************************************************
* File Name: cycfg_pins.c
*
* Description:
* Pin configuration
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
const cy_stc_gpio_pin_config_t CYBSP_QSPI_SS0_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = CYBSP_QSPI_SS0_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_QSPI_SS0_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_QSPI_SS0_PORT_NUM,
		.channel_num = CYBSP_QSPI_SS0_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_QSPI_DATA3_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG,
	.hsiom = CYBSP_QSPI_DATA3_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_QSPI_DATA3_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_QSPI_DATA3_PORT_NUM,
		.channel_num = CYBSP_QSPI_DATA3_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_QSPI_DATA2_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG,
	.hsiom = CYBSP_QSPI_DATA2_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_QSPI_DATA2_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_QSPI_DATA2_PORT_NUM,
		.channel_num = CYBSP_QSPI_DATA2_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_QSPI_DATA1_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG,
	.hsiom = CYBSP_QSPI_DATA1_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_QSPI_DATA1_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_QSPI_DATA1_PORT_NUM,
		.channel_num = CYBSP_QSPI_DATA1_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_QSPI_DATA0_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG,
	.hsiom = CYBSP_QSPI_DATA0_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_QSPI_DATA0_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_QSPI_DATA0_PORT_NUM,
		.channel_num = CYBSP_QSPI_DATA0_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_QSPI_SPI_CLOCK_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = CYBSP_QSPI_SPI_CLOCK_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_QSPI_SPI_CLOCK_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_QSPI_SPI_CLOCK_PORT_NUM,
		.channel_num = CYBSP_QSPI_SPI_CLOCK_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_USB_DP_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_USB_DP_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_USB_DP_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_USB_DP_PORT_NUM,
		.channel_num = CYBSP_USB_DP_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_USB_DM_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_USB_DM_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_USB_DM_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_USB_DM_PORT_NUM,
		.channel_num = CYBSP_USB_DM_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_CSD_RX_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_CSD_RX_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_CSD_RX_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_CSD_RX_PORT_NUM,
		.channel_num = CYBSP_CSD_RX_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_BT_UART_RX_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_HIGHZ,
	.hsiom = CYBSP_BT_UART_RX_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_BT_UART_RX_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_BT_UART_RX_PORT_NUM,
		.channel_num = CYBSP_BT_UART_RX_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_BT_UART_TX_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = CYBSP_BT_UART_TX_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_BT_UART_TX_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_BT_UART_TX_PORT_NUM,
		.channel_num = CYBSP_BT_UART_TX_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_BT_UART_RTS_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = CYBSP_BT_UART_RTS_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_BT_UART_RTS_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_BT_UART_RTS_PORT_NUM,
		.channel_num = CYBSP_BT_UART_RTS_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_BT_UART_CTS_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_HIGHZ,
	.hsiom = CYBSP_BT_UART_CTS_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_BT_UART_CTS_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_BT_UART_CTS_PORT_NUM,
		.channel_num = CYBSP_BT_UART_CTS_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_BT_POWER_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_OD_DRIVESHIGH_IN_OFF,
	.hsiom = CYBSP_BT_POWER_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_BT_POWER_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_BT_POWER_PORT_NUM,
		.channel_num = CYBSP_BT_POWER_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_BT_DEVICE_WAKE_config = 
{
	.outVal = 0,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = CYBSP_BT_DEVICE_WAKE_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_BT_DEVICE_WAKE_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_BT_DEVICE_WAKE_PORT_NUM,
		.channel_num = CYBSP_BT_DEVICE_WAKE_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_BT_HOST_WAKE_config = 
{
	.outVal = 0,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_BT_HOST_WAKE_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_BT_HOST_WAKE_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_BT_HOST_WAKE_PORT_NUM,
		.channel_num = CYBSP_BT_HOST_WAKE_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_WIFI_HOST_WAKE_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_WIFI_HOST_WAKE_HSIOM,
	.intEdge = CY_GPIO_INTR_RISING,
	.intMask = 1UL,
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
	const cyhal_resource_inst_t CYBSP_WIFI_HOST_WAKE_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_WIFI_HOST_WAKE_PORT_NUM,
		.channel_num = CYBSP_WIFI_HOST_WAKE_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_EZI2C_SCL_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_OD_DRIVESLOW,
	.hsiom = CYBSP_EZI2C_SCL_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_EZI2C_SCL_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_EZI2C_SCL_PORT_NUM,
		.channel_num = CYBSP_EZI2C_SCL_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_EZI2C_SDA_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_OD_DRIVESLOW,
	.hsiom = CYBSP_EZI2C_SDA_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_EZI2C_SDA_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_EZI2C_SDA_PORT_NUM,
		.channel_num = CYBSP_EZI2C_SDA_PIN,
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
const cy_stc_gpio_pin_config_t CYBSP_SWDCK_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_PULLDOWN,
	.hsiom = CYBSP_SWDCK_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_SWDCK_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_SWDCK_PORT_NUM,
		.channel_num = CYBSP_SWDCK_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_CINA_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_CINA_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_CINA_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_CINA_PORT_NUM,
		.channel_num = CYBSP_CINA_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_CINB_config = 
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_ANALOG,
	.hsiom = CYBSP_CINB_HSIOM,
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
	const cyhal_resource_inst_t CYBSP_CINB_obj = 
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_CINB_PORT_NUM,
		.channel_num = CYBSP_CINB_PIN,
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

	Cy_GPIO_Pin_Init(CYBSP_QSPI_SS0_PORT, CYBSP_QSPI_SS0_PIN, &CYBSP_QSPI_SS0_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_QSPI_SS0_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_QSPI_DATA3_PORT, CYBSP_QSPI_DATA3_PIN, &CYBSP_QSPI_DATA3_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_QSPI_DATA3_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_QSPI_DATA2_PORT, CYBSP_QSPI_DATA2_PIN, &CYBSP_QSPI_DATA2_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_QSPI_DATA2_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_QSPI_DATA1_PORT, CYBSP_QSPI_DATA1_PIN, &CYBSP_QSPI_DATA1_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_QSPI_DATA1_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_QSPI_DATA0_PORT, CYBSP_QSPI_DATA0_PIN, &CYBSP_QSPI_DATA0_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_QSPI_DATA0_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_QSPI_SPI_CLOCK_PORT, CYBSP_QSPI_SPI_CLOCK_PIN, &CYBSP_QSPI_SPI_CLOCK_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_QSPI_SPI_CLOCK_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_USB_DP_PORT, CYBSP_USB_DP_PIN, &CYBSP_USB_DP_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_USB_DP_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_USB_DM_PORT, CYBSP_USB_DM_PIN, &CYBSP_USB_DM_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_USB_DM_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_CSD_RX_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_BT_UART_RX_PORT, CYBSP_BT_UART_RX_PIN, &CYBSP_BT_UART_RX_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_BT_UART_RX_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_BT_UART_TX_PORT, CYBSP_BT_UART_TX_PIN, &CYBSP_BT_UART_TX_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_BT_UART_TX_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_BT_UART_RTS_PORT, CYBSP_BT_UART_RTS_PIN, &CYBSP_BT_UART_RTS_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_BT_UART_RTS_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_BT_UART_CTS_PORT, CYBSP_BT_UART_CTS_PIN, &CYBSP_BT_UART_CTS_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_BT_UART_CTS_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_BT_POWER_PORT, CYBSP_BT_POWER_PIN, &CYBSP_BT_POWER_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_BT_POWER_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_BT_DEVICE_WAKE_PORT, CYBSP_BT_DEVICE_WAKE_PIN, &CYBSP_BT_DEVICE_WAKE_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_BT_DEVICE_WAKE_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_BT_HOST_WAKE_PORT, CYBSP_BT_HOST_WAKE_PIN, &CYBSP_BT_HOST_WAKE_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_BT_HOST_WAKE_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_WIFI_HOST_WAKE_PORT, CYBSP_WIFI_HOST_WAKE_PIN, &CYBSP_WIFI_HOST_WAKE_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_WIFI_HOST_WAKE_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_EZI2C_SCL_PORT, CYBSP_EZI2C_SCL_PIN, &CYBSP_EZI2C_SCL_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_EZI2C_SCL_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_EZI2C_SDA_PORT, CYBSP_EZI2C_SDA_PIN, &CYBSP_EZI2C_SDA_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_EZI2C_SDA_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_SWO_PORT, CYBSP_SWO_PIN, &CYBSP_SWO_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_SWO_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_SWDIO_PORT, CYBSP_SWDIO_PIN, &CYBSP_SWDIO_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_SWDIO_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_SWDCK_PORT, CYBSP_SWDCK_PIN, &CYBSP_SWDCK_config);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_SWDCK_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_CINA_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_CINB_obj);
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
}
