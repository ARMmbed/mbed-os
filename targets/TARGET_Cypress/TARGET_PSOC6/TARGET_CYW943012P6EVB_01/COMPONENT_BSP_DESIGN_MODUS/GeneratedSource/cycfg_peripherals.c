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

#define CYBSP_USBUART_INTR_LVL_SEL (CY_USBFS_DEV_DRV_SET_SOF_LVL(0x2U) | \
                 CY_USBFS_DEV_DRV_SET_BUS_RESET_LVL(0x2U) | \
                 CY_USBFS_DEV_DRV_SET_EP0_LVL(0x2U) | \
                 CY_USBFS_DEV_DRV_SET_LPM_LVL(0x0U) | \
                 CY_USBFS_DEV_DRV_SET_ARB_EP_LVL(0x0U) | \
                 CY_USBFS_DEV_DRV_SET_EP1_LVL(0x2U) | \
                 CY_USBFS_DEV_DRV_SET_EP2_LVL(0x2U) | \
                 CY_USBFS_DEV_DRV_SET_EP3_LVL(0x1U) | \
                 CY_USBFS_DEV_DRV_SET_EP4_LVL(0x1U) | \
                 CY_USBFS_DEV_DRV_SET_EP5_LVL(0x1U) | \
                 CY_USBFS_DEV_DRV_SET_EP6_LVL(0x1U) | \
                 CY_USBFS_DEV_DRV_SET_EP7_LVL(0x1U) | \
                 CY_USBFS_DEV_DRV_SET_EP8_LVL(0x1U))

cy_stc_csd_context_t cy_csd_0_context = 
{
	.lockKey = CY_CSD_NONE_KEY,
};
const cy_stc_scb_spi_config_t CYBSP_SPI_config = 
{
	.spiMode = CY_SCB_SPI_MASTER,
	.subMode = CY_SCB_SPI_MOTOROLA,
	.sclkMode = CY_SCB_SPI_CPHA0_CPOL0,
	.oversample = 16,
	.rxDataWidth = 8UL,
	.txDataWidth = 8UL,
	.enableMsbFirst = true,
	.enableInputFilter = false,
	.enableFreeRunSclk = false,
	.enableMisoLateSample = true,
	.enableTransferSeperation = false,
	.ssPolarity = ((CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT0) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT1) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT2) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT3)),
	.enableWakeFromSleep = false,
	.rxFifoTriggerLevel = 63UL,
	.rxFifoIntEnableMask = 0UL,
	.txFifoTriggerLevel = 63UL,
	.txFifoIntEnableMask = 0UL,
	.masterSlaveIntEnableMask = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_SPI_obj = 
	{
		.type = CYHAL_RSC_SCB,
		.block_num = 1U,
		.channel_num = 0U,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_scb_uart_config_t CYBSP_BT_UART_config = 
{
	.uartMode = CY_SCB_UART_STANDARD,
	.enableMutliProcessorMode = false,
	.smartCardRetryOnNack = false,
	.irdaInvertRx = false,
	.irdaEnableLowPowerReceiver = false,
	.oversample = 8,
	.enableMsbFirst = false,
	.dataWidth = 8UL,
	.parity = CY_SCB_UART_PARITY_NONE,
	.stopBits = CY_SCB_UART_STOP_BITS_1,
	.enableInputFilter = false,
	.breakWidth = 11UL,
	.dropOnFrameError = false,
	.dropOnParityError = false,
	.receiverAddress = 0x0UL,
	.receiverAddressMask = 0x0UL,
	.acceptAddrInFifo = false,
	.enableCts = true,
	.ctsPolarity = CY_SCB_UART_ACTIVE_LOW,
	.rtsRxFifoLevel = 63,
	.rtsPolarity = CY_SCB_UART_ACTIVE_LOW,
	.rxFifoTriggerLevel = 1UL,
	.rxFifoIntEnableMask = 0UL,
	.txFifoTriggerLevel = 63UL,
	.txFifoIntEnableMask = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_BT_UART_obj = 
	{
		.type = CYHAL_RSC_SCB,
		.block_num = 2U,
		.channel_num = 0U,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_scb_ezi2c_config_t CYBSP_CSD_COMM_config = 
{
	.numberOfAddresses = CY_SCB_EZI2C_ONE_ADDRESS,
	.slaveAddress1 = 8U,
	.slaveAddress2 = 0U,
	.subAddressSize = CY_SCB_EZI2C_SUB_ADDR16_BITS,
	.enableWakeFromSleep = false,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_CSD_COMM_obj = 
	{
		.type = CYHAL_RSC_SCB,
		.block_num = 3U,
		.channel_num = 0U,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_scb_uart_config_t CYBSP_WL_UART_config = 
{
	.uartMode = CY_SCB_UART_STANDARD,
	.enableMutliProcessorMode = false,
	.smartCardRetryOnNack = false,
	.irdaInvertRx = false,
	.irdaEnableLowPowerReceiver = false,
	.oversample = 8,
	.enableMsbFirst = false,
	.dataWidth = 8UL,
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
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_WL_UART_obj = 
	{
		.type = CYHAL_RSC_SCB,
		.block_num = 5U,
		.channel_num = 0U,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_smif_config_t CYBSP_QSPI_config = 
{
	.mode = (uint32_t)CY_SMIF_NORMAL,
	.deselectDelay = CYBSP_QSPI_DESELECT_DELAY,
	.rxClockSel = (uint32_t)CY_SMIF_SEL_INV_INTERNAL_CLK,
	.blockEvent = (uint32_t)CY_SMIF_BUS_ERROR,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_QSPI_obj = 
	{
		.type = CYHAL_RSC_SMIF,
		.block_num = 0U,
		.channel_num = 0U,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_mcwdt_config_t CYBSP_MCWDT0_config = 
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
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_MCWDT0_obj = 
	{
		.type = CYHAL_RSC_LPTIMER,
		.block_num = 0U,
		.channel_num = 0U,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_rtc_config_t CYBSP_RTC_config = 
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
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_RTC_obj = 
	{
		.type = CYHAL_RSC_RTC,
		.block_num = 0U,
		.channel_num = 0U,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_usbfs_dev_drv_config_t CYBSP_USBUART_config = 
{
	.mode = CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU,
	.epAccess = CY_USBFS_DEV_DRV_USE_8_BITS_DR,
	.epBuffer = NULL,
	.epBufferSize = 0U,
	.dmaConfig[0] = NULL,
	.dmaConfig[1] = NULL,
	.dmaConfig[2] = NULL,
	.dmaConfig[3] = NULL,
	.dmaConfig[4] = NULL,
	.dmaConfig[5] = NULL,
	.dmaConfig[6] = NULL,
	.dmaConfig[7] = NULL,
	.enableLpm = false,
	.intrLevelSel = CYBSP_USBUART_INTR_LVL_SEL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_USBUART_obj = 
	{
		.type = CYHAL_RSC_USB,
		.block_num = 0U,
		.channel_num = 0U,
	};
#endif //defined (CY_USING_HAL)


void init_cycfg_peripherals(void)
{
	Cy_SysClk_PeriphAssignDivider(PCLK_CSD_CLOCK, CY_SYSCLK_DIV_8_BIT, 4U);

	Cy_SysClk_PeriphAssignDivider(PCLK_SCB1_CLOCK, CY_SYSCLK_DIV_8_BIT, 5U);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_SPI_obj);
#endif //defined (CY_USING_HAL)

	Cy_SysClk_PeriphAssignDivider(PCLK_SCB2_CLOCK, CY_SYSCLK_DIV_8_BIT, 3U);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_BT_UART_obj);
#endif //defined (CY_USING_HAL)

	Cy_SysClk_PeriphAssignDivider(PCLK_SCB3_CLOCK, CY_SYSCLK_DIV_8_BIT, 1U);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_CSD_COMM_obj);
#endif //defined (CY_USING_HAL)

	Cy_SysClk_PeriphAssignDivider(PCLK_SCB5_CLOCK, CY_SYSCLK_DIV_8_BIT, 6U);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_WL_UART_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_QSPI_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_MCWDT0_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_RTC_obj);
#endif //defined (CY_USING_HAL)

	Cy_SysClk_PeriphAssignDivider(PCLK_USB_CLOCK_DEV_BRS, CY_SYSCLK_DIV_16_BIT, 0U);
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_USBUART_obj);
#endif //defined (CY_USING_HAL)
}
