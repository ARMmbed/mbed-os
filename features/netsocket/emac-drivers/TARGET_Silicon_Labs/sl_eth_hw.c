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
#include "hal/gpio_api.h"

#if defined(ETH_PRESENT)

void sl_eth_hw_init(void)
{
    /* Turn on clocks to ETH */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_ETH, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    /* Drive RMII from the MCU-side 50MHz clock */
    GPIO_PinModeSet(AF_CMU_CLK2_PORT(MBED_CONF_SL_ETH_REFCLK_LOCATION),
                    AF_CMU_CLK2_PIN(MBED_CONF_SL_ETH_REFCLK_LOCATION),
                    gpioModePushPull, 0);
    CMU->CTRL      |= CMU_CTRL_CLKOUTSEL2_HFXO;
    CMU->ROUTELOC0  = (CMU->ROUTELOC0 & ~_CMU_ROUTELOC0_CLKOUT2LOC_MASK) | (MBED_CONF_SL_ETH_REFCLK_LOCATION << _CMU_ROUTELOC0_CLKOUT2LOC_SHIFT);
    CMU->ROUTEPEN  |= CMU_ROUTEPEN_CLKOUT2PEN;

    ETH->CTRL = ETH_CTRL_GBLCLKEN | ETH_CTRL_MIISEL_RMII;

    /* Set pins to ETH for RMII config */
    GPIO_PinModeSet(AF_ETH_RMIICRSDV_PORT(MBED_CONF_SL_ETH_RMII_LOCATION),
                    AF_ETH_RMIICRSDV_PIN(MBED_CONF_SL_ETH_RMII_LOCATION),
                    gpioModeInput, 0);    /* CRS_DV */
    GPIO_PinModeSet(AF_ETH_RMIITXD0_PORT(MBED_CONF_SL_ETH_RMII_LOCATION),
                    AF_ETH_RMIITXD0_PIN(MBED_CONF_SL_ETH_RMII_LOCATION),
                    gpioModePushPull, 0); /* TXD0   */
    GPIO_PinModeSet(AF_ETH_RMIITXD1_PORT(MBED_CONF_SL_ETH_RMII_LOCATION),
                    AF_ETH_RMIITXD1_PIN(MBED_CONF_SL_ETH_RMII_LOCATION),
                    gpioModePushPull, 0); /* TXD1   */
    GPIO_PinModeSet(AF_ETH_RMIITXEN_PORT(MBED_CONF_SL_ETH_RMII_LOCATION),
                    AF_ETH_RMIITXEN_PIN(MBED_CONF_SL_ETH_RMII_LOCATION),
                    gpioModePushPull, 0); /* TX_EN  */
    GPIO_PinModeSet(AF_ETH_RMIIRXD0_PORT(MBED_CONF_SL_ETH_RMII_LOCATION),
                    AF_ETH_RMIIRXD0_PIN(MBED_CONF_SL_ETH_RMII_LOCATION),
                    gpioModeInput, 0);    /* RXD0   */
    GPIO_PinModeSet(AF_ETH_RMIIRXD1_PORT(MBED_CONF_SL_ETH_RMII_LOCATION),
                    AF_ETH_RMIIRXD1_PIN(MBED_CONF_SL_ETH_RMII_LOCATION),
                    gpioModeInput, 0);    /* RXD1   */
    GPIO_PinModeSet(AF_ETH_RMIIRXER_PORT(MBED_CONF_SL_ETH_RMII_LOCATION),
                    AF_ETH_RMIIRXER_PIN(MBED_CONF_SL_ETH_RMII_LOCATION),
                    gpioModeInput, 0);    /* RX_ER  */

    /* Setup route locations and enable pins */
    ETH->ROUTELOC1 = (MBED_CONF_SL_ETH_RMII_LOCATION << _ETH_ROUTELOC1_RMIILOC_SHIFT)
                     | (MBED_CONF_SL_ETH_MDIO_LOCATION << _ETH_ROUTELOC1_MDIOLOC_SHIFT);
    ETH->ROUTEPEN  = ETH_ROUTEPEN_RMIIPEN | ETH_ROUTEPEN_MDIOPEN;
    ETH->ROUTEPEN  = ETH_ROUTEPEN_RMIIPEN | ETH_ROUTEPEN_MDIOPEN;

    /* Setup the MDIO pins */
    GPIO_PinModeSet(AF_ETH_MDIO_PORT(MBED_CONF_SL_ETH_MDIO_LOCATION),
                    AF_ETH_MDIO_PIN(MBED_CONF_SL_ETH_MDIO_LOCATION),
                    gpioModePushPull, 0); /* MDIO */
    GPIO_PinModeSet(AF_ETH_MDC_PORT(MBED_CONF_SL_ETH_MDIO_LOCATION),
                    AF_ETH_MDC_PIN(MBED_CONF_SL_ETH_MDIO_LOCATION),
                    gpioModePushPull, 0); /* MDC  */

    /* Enable the PHY on the STK */
#if defined(MBED_CONF_SL_ETH_PHY_POWER_PIN)
    gpio_t pwr_pin;
    gpio_init_out_ex(&pwr_pin, MBED_CONF_SL_ETH_PHY_POWER_PIN, 1);
#endif

#if defined(MBED_CONF_SL_ETH_PHY_ENABLE_PIN)
    gpio_t en_pin;
    gpio_init_out_ex(&en_pin, MBED_CONF_SL_ETH_PHY_ENABLE_PIN, 1);
#endif
}

void sl_eth_hw_deinit(void)
{
    /* Turn off PHY */
#if defined(MBED_CONF_SL_ETH_PHY_POWER_PIN)
    gpio_t pwr_pin;
    gpio_init(&pwr_pin, MBED_CONF_SL_ETH_PHY_ENABLE_PIN);
    gpio_write(&pwr_pin, 0);
    gpio_mode(&pwr_pin, Disabled);
#endif

#if defined(MBED_CONF_SL_ETH_PHY_ENABLE_PIN)
    gpio_t en_pin;
    gpio_init(&en_pin, MBED_CONF_SL_ETH_PHY_POWER_PIN);
    gpio_write(&en_pin, 0);
    gpio_mode(&en_pin, Disabled);
#endif

    /* Turn off MAC */
    ETH->ROUTEPEN = 0;
    ETH->CTRL = _ETH_CTRL_RESETVALUE;

    /* Turn off clock */
    CMU->CTRL      &= ~CMU_CTRL_CLKOUTSEL2_HFXO;
    CMU->ROUTEPEN  &= ~CMU_ROUTEPEN_CLKOUT2PEN;

    CMU_ClockEnable(cmuClock_ETH, false);

    /* Set used pins back to disabled */
    GPIO_PinModeSet(AF_ETH_MDIO_PORT(MBED_CONF_SL_ETH_MDIO_LOCATION),
                    AF_ETH_MDIO_PIN(MBED_CONF_SL_ETH_MDIO_LOCATION),
                    gpioModeDisabled, 0); /* MDIO */
    GPIO_PinModeSet(AF_ETH_MDC_PORT(MBED_CONF_SL_ETH_MDIO_LOCATION),
                    AF_ETH_MDC_PIN(MBED_CONF_SL_ETH_MDIO_LOCATION),
                    gpioModeDisabled, 0); /* MDC  */
    GPIO_PinModeSet(AF_ETH_RMIICRSDV_PORT(MBED_CONF_SL_ETH_RMII_LOCATION),
                    AF_ETH_RMIICRSDV_PIN(MBED_CONF_SL_ETH_RMII_LOCATION),
                    gpioModeDisabled, 0);    /* CRS_DV */
    GPIO_PinModeSet(AF_ETH_RMIITXD0_PORT(MBED_CONF_SL_ETH_RMII_LOCATION),
                    AF_ETH_RMIITXD0_PIN(MBED_CONF_SL_ETH_RMII_LOCATION),
                    gpioModeDisabled, 0); /* TXD0   */
    GPIO_PinModeSet(AF_ETH_RMIITXD1_PORT(MBED_CONF_SL_ETH_RMII_LOCATION),
                    AF_ETH_RMIITXD1_PIN(MBED_CONF_SL_ETH_RMII_LOCATION),
                    gpioModeDisabled, 0); /* TXD1   */
    GPIO_PinModeSet(AF_ETH_RMIITXEN_PORT(MBED_CONF_SL_ETH_RMII_LOCATION),
                    AF_ETH_RMIITXEN_PIN(MBED_CONF_SL_ETH_RMII_LOCATION),
                    gpioModeDisabled, 0); /* TX_EN  */
    GPIO_PinModeSet(AF_ETH_RMIIRXD0_PORT(MBED_CONF_SL_ETH_RMII_LOCATION),
                    AF_ETH_RMIIRXD0_PIN(MBED_CONF_SL_ETH_RMII_LOCATION),
                    gpioModeDisabled, 0);    /* RXD0   */
    GPIO_PinModeSet(AF_ETH_RMIIRXD1_PORT(MBED_CONF_SL_ETH_RMII_LOCATION),
                    AF_ETH_RMIIRXD1_PIN(MBED_CONF_SL_ETH_RMII_LOCATION),
                    gpioModeDisabled, 0);    /* RXD1   */
    GPIO_PinModeSet(AF_ETH_RMIIRXER_PORT(MBED_CONF_SL_ETH_RMII_LOCATION),
                    AF_ETH_RMIIRXER_PIN(MBED_CONF_SL_ETH_RMII_LOCATION),
                    gpioModeDisabled, 0);    /* RX_ER  */
    GPIO_PinModeSet(AF_CMU_CLK2_PORT(MBED_CONF_SL_ETH_REFCLK_LOCATION),
                    AF_CMU_CLK2_PIN(MBED_CONF_SL_ETH_REFCLK_LOCATION),
                    gpioModeDisabled, 0); /* REF_CLK */
}

#endif //ETH_PRESENT
