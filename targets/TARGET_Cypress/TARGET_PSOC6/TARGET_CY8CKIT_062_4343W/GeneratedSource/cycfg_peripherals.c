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

#define PWM_INPUT_DISABLED 0x7U
#define USBUART_INTR_LVL_SEL (CY_USBFS_DEV_DRV_SET_SOF_LVL(0x2U) | \
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
const cy_stc_scb_uart_config_t BT_UART_config = 
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
	.rxFifoTriggerLevel = 63UL,
	.rxFifoIntEnableMask = 0UL,
	.txFifoTriggerLevel = 63UL,
	.txFifoIntEnableMask = 0UL,
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
cy_en_sd_host_card_capacity_t SDIO_cardCapacity = CY_SD_HOST_SDSC;
cy_en_sd_host_card_type_t SDIO_cardType = CY_SD_HOST_EMMC;
uint32_t SDIO_rca = 0u;
const cy_stc_sd_host_init_config_t SDIO_config = 
{
	.emmc = true,
	.dmaType = CY_SD_HOST_DMA_SDMA,
	.enableLedControl = false,
};
cy_stc_sd_host_sd_card_config_t SDIO_card_cfg = 
{
	.lowVoltageSignaling = false,
	.busWidth = CY_SD_HOST_BUS_WIDTH_4_BIT,
	.cardType = &SDIO_cardType,
	.rca = &SDIO_rca,
	.cardCapacity = &SDIO_cardCapacity,
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
const cy_stc_tcpwm_pwm_config_t PWM_config = 
{
	.pwmMode = CY_TCPWM_PWM_MODE_PWM,
	.clockPrescaler = CY_TCPWM_PWM_PRESCALER_DIVBY_1,
	.pwmAlignment = CY_TCPWM_PWM_LEFT_ALIGN,
	.deadTimeClocks = 0,
	.runMode = CY_TCPWM_PWM_CONTINUOUS,
	.period0 = 32000,
	.period1 = 32768,
	.enablePeriodSwap = false,
	.compare0 = 16384,
	.compare1 = 16384,
	.enableCompareSwap = false,
	.interruptSources = CY_TCPWM_INT_NONE,
	.invertPWMOut = CY_TCPWM_PWM_INVERT_DISABLE,
	.invertPWMOutN = CY_TCPWM_PWM_INVERT_DISABLE,
	.killMode = CY_TCPWM_PWM_STOP_ON_KILL,
	.swapInputMode = PWM_INPUT_DISABLED & 0x3U,
	.swapInput = CY_TCPWM_INPUT_0,
	.reloadInputMode = PWM_INPUT_DISABLED & 0x3U,
	.reloadInput = CY_TCPWM_INPUT_0,
	.startInputMode = PWM_INPUT_DISABLED & 0x3U,
	.startInput = CY_TCPWM_INPUT_0,
	.killInputMode = PWM_INPUT_DISABLED & 0x3U,
	.killInput = CY_TCPWM_INPUT_0,
	.countInputMode = PWM_INPUT_DISABLED & 0x3U,
	.countInput = CY_TCPWM_INPUT_1,
};
const cy_stc_usbfs_dev_drv_config_t USBUART_config = 
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
	.intrLevelSel = USBUART_INTR_LVL_SEL,
};


void init_cycfg_peripherals(void)
{
	Cy_SysClk_PeriphAssignDivider(PCLK_CSD_CLOCK, CY_SYSCLK_DIV_8_BIT, 4U);

	Cy_SysClk_PeriphAssignDivider(PCLK_SCB2_CLOCK, CY_SYSCLK_DIV_8_BIT, 2U);

	Cy_SysClk_PeriphAssignDivider(PCLK_SCB3_CLOCK, CY_SYSCLK_DIV_8_BIT, 1U);

	Cy_SysClk_PeriphAssignDivider(PCLK_SCB5_CLOCK, CY_SYSCLK_DIV_8_BIT, 2U);

	Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM1_CLOCKS1, CY_SYSCLK_DIV_8_BIT, 3U);

	Cy_SysClk_PeriphAssignDivider(PCLK_USB_CLOCK_DEV_BRS, CY_SYSCLK_DIV_16_BIT, 0U);
}
