/*******************************************************************************
* File Name: cycfg_peripherals.c
*
* Description:
* Peripheral Hardware Block configuration
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

#include "cycfg_peripherals.h"

cy_stc_csd_context_t cy_csd_0_context = 
{
	.lockKey = CY_CSD_NONE_KEY,
};
const cy_stc_scb_ezi2c_config_t CSD_COMM_config = 
{
	.numberOfAddresses = CY_SCB_EZI2C_ONE_ADDRESS,
	.slaveAddress1 = 8U,
	.slaveAddress2 = 0U,
	.subAddressSize = CY_SCB_EZI2C_SUB_ADDR16_BITS,
	.enableWakeFromSleep = false,
};
const cy_stc_scb_uart_config_t KITPROG_UART_config = 
{
	.uartMode = CY_SCB_UART_STANDARD,
	.enableMutliProcessorMode = false,
	.smartCardRetryOnNack = false,
	.irdaInvertRx = false,
	.irdaEnableLowPowerReceiver = false,
	.oversample = 12,
	.enableMsbFirst = false,
	.dataWidth = 9UL,
	.parity = CY_SCB_UART_PARITY_NONE,
	.stopBits = CY_SCB_UART_STOP_BITS_1,
	.enableInputFilter = false,
	.breakWidth = 11UL,
	.dropOnFrameError = false,
	.dropOnParityError = false,
	.receiverAddress = 0x0UL,
	.receiverAddressMask = 0x0UL,
	.acceptAddrInFifo = false,
	.enableCts = false,
	.ctsPolarity = CY_SCB_UART_ACTIVE_LOW,
	.rtsRxFifoLevel = 0UL,
	.rtsPolarity = CY_SCB_UART_ACTIVE_LOW,
	.rxFifoTriggerLevel = 63UL,
	.rxFifoIntEnableMask = 0UL,
	.txFifoTriggerLevel = 63UL,
	.txFifoIntEnableMask = 0UL,
};
const cy_stc_smif_config_t QSPI_config = 
{
	.mode = (uint32_t)CY_SMIF_NORMAL,
	.deselectDelay = QSPI_DESELECT_DELAY,
	.rxClockSel = (uint32_t)CY_SMIF_SEL_INV_INTERNAL_CLK,
	.blockEvent = (uint32_t)CY_SMIF_BUS_ERROR,
};
const cy_stc_mcwdt_config_t MCWDT0_config = 
{
	.c0Match = 32768U,
	.c1Match = 32768U,
	.c0Mode = CY_MCWDT_MODE_NONE,
	.c1Mode = CY_MCWDT_MODE_NONE,
	.c2ToggleBit = 16U,
	.c2Mode = CY_MCWDT_MODE_NONE,
	.c0ClearOnMatch = false,
	.c1ClearOnMatch = false,
	.c0c1Cascade = true,
	.c1c2Cascade = false,
};
const cy_stc_rtc_config_t RTC_config = 
{
	.sec = 0U,
	.min = 0U,
	.hour = 12U,
	.amPm = CY_RTC_AM,
	.hrFormat = CY_RTC_24_HOURS,
	.dayOfWeek = CY_RTC_SUNDAY,
	.date = 1U,
	.month = CY_RTC_JANUARY,
	.year = 0U,
};


void init_cycfg_peripherals(void)
{
	Cy_SysClk_PeriphAssignDivider(PCLK_CSD_CLOCK, CY_SYSCLK_DIV_8_BIT, 0U);

	Cy_SysClk_PeriphAssignDivider(PCLK_SCB3_CLOCK, CY_SYSCLK_DIV_8_BIT, 1U);

	Cy_SysClk_PeriphAssignDivider(PCLK_SCB5_CLOCK, CY_SYSCLK_DIV_8_BIT, 2U);
}
