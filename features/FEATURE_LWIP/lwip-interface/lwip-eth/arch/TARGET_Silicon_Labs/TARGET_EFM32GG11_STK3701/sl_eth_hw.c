/***************************************************************************//**
 * @file sl_eth_hw.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2017 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#include "sl_eth_hw.h"
#include "device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "sys_arch.h"

void sl_eth_hw_init(void)
{
    /* Turn on clocks to ETH */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_ETH, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    /* Drive RMII from the MCU-side 50MHz clock */
    GPIO_PinModeSet(gpioPortD, 10, gpioModePushPull, 0);
    CMU->CTRL      |= CMU_CTRL_CLKOUTSEL2_HFXO;
    CMU->ROUTELOC0  = CMU_ROUTELOC0_CLKOUT2LOC_LOC5;
    CMU->ROUTEPEN  |= CMU_ROUTEPEN_CLKOUT2PEN;
    ETH->CTRL = ETH_CTRL_GBLCLKEN | ETH_CTRL_MIISEL_RMII;

    /* Set pins to ETH for RMII config */
#if (TARGET_EFM32GG11_STK3701)
     /* STK3701: KSZ8091 PHY on RMII */
    GPIO_PinModeSet(gpioPortD, 11, gpioModeInput, 0);    /* CRS_DV */
    GPIO_PinModeSet(gpioPortF,  7, gpioModePushPull, 0); /* TXD0   */
    GPIO_PinModeSet(gpioPortF,  6, gpioModePushPull, 0); /* TXD1   */
    GPIO_PinModeSet(gpioPortF,  8, gpioModePushPull, 0); /* TX_EN  */
    GPIO_PinModeSet(gpioPortD,  9, gpioModeInput, 0);    /* RXD0   */
    GPIO_PinModeSet(gpioPortF,  9, gpioModeInput, 0);    /* RXD1   */
    GPIO_PinModeSet(gpioPortD, 12, gpioModeInput, 0);    /* RX_ER  */

    /* Setup route locations and enable pins */
    ETH->ROUTELOC1 = (1 << _ETH_ROUTELOC1_RMIILOC_SHIFT)
                     | (1 << _ETH_ROUTELOC1_MDIOLOC_SHIFT);
#else
#error "No pins defined for this target!"
#endif
    ETH->ROUTEPEN  = ETH_ROUTEPEN_RMIIPEN | ETH_ROUTEPEN_MDIOPEN;
    ETH->ROUTEPEN  = ETH_ROUTEPEN_RMIIPEN | ETH_ROUTEPEN_MDIOPEN;

    /* Setup the MDIO pins */
    GPIO_PinModeSet(gpioPortD, 13, gpioModePushPull, 0); /* MDIO */
    GPIO_PinModeSet(gpioPortD, 14, gpioModePushPull, 0); /* MDC  */

    /* Enable the PHY on the STK */
    GPIO_PinModeSet(gpioPortI, 10, gpioModePushPull, 1);
    GPIO_PinModeSet(gpioPortH,  7, gpioModePushPull, 1);

    osDelay(30);
}

void sl_eth_hw_deinit(void)
{
    /* Turn off PHY */
    GPIO_PinModeSet(gpioPortI, 10, gpioModeDisabled, 0);
    GPIO_PinModeSet(gpioPortH, 7,  gpioModeDisabled, 0);

    /* Turn off MAC */
    ETH->ROUTEPEN = 0;
    ETH->CTRL = _ETH_CTRL_RESETVALUE;

    /* Turn off clock */
    CMU->CTRL      &= ~CMU_CTRL_CLKOUTSEL2_HFXO;
    CMU->ROUTEPEN  &= ~CMU_ROUTEPEN_CLKOUT2PEN;

    CMU_ClockEnable(cmuClock_ETH, false);

    /* Set used pins back to disabled */
#if (TARGET_EFM32GG11_STK3701)
    GPIO_PinModeSet(gpioPortD, 13, gpioModeDisabled, 0); /* MDIO */
    GPIO_PinModeSet(gpioPortD, 14, gpioModeDisabled, 0); /* MDC  */
    GPIO_PinModeSet(gpioPortD, 11, gpioModeDisabled, 0); /* CRS_DV */
    GPIO_PinModeSet(gpioPortF,  7, gpioModeDisabled, 0); /* TXD0   */
    GPIO_PinModeSet(gpioPortF,  6, gpioModeDisabled, 0); /* TXD1   */
    GPIO_PinModeSet(gpioPortF,  8, gpioModeDisabled, 0); /* TX_EN  */
    GPIO_PinModeSet(gpioPortD,  9, gpioModeDisabled, 0); /* RXD0   */
    GPIO_PinModeSet(gpioPortF,  9, gpioModeDisabled, 0); /* RXD1   */
    GPIO_PinModeSet(gpioPortD, 12, gpioModeDisabled, 0); /* RX_ER  */
    GPIO_PinModeSet(gpioPortD, 10, gpioModeDisabled, 0);
#else
#error "No pins defined for this target!"
#endif
}
