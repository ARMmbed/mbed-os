/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
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
 */

#include "gd32f30x.h"

/**
 * Initializes the HW pin for enet
 *
 */
void enet_bsp_init(void)
{
    /* Enable GPIOs clocks */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_AF);

    gpio_para_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_8);
    rcu_pll2_config(RCU_PLL2_MUL10);
    rcu_osci_on(RCU_PLL2_CK);
    rcu_osci_stab_wait(RCU_PLL2_CK);
    rcu_ckout0_config(RCU_CKOUT0SRC_CKPLL2);
    gpio_ethernet_phy_select(GPIO_ENET_PHY_RMII);

    /** ETH GPIO Configuration
      RMII_REF_CLK ----------------------> PA1
      RMII_MDIO -------------------------> PA2
      RMII_MDC --------------------------> PC1
      RMII_MII_CRS_DV -------------------> PA7
      RMII_MII_RXD0 ---------------------> PC4
      RMII_MII_RXD1 ---------------------> PC5
      RMII_MII_TX_EN --------------------> PB11
      RMII_MII_TXD0 ---------------------> PB12
      RMII_MII_TXD1 ---------------------> PB13
     */
    /* PA1: ETH_RMII_REF_CLK */
    gpio_para_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_MAX, GPIO_PIN_1);
    /* PA2: ETH_MDIO */
    gpio_para_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_2);
    /* PA7: ETH_RMII_CRS_DV */
    gpio_para_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_MAX, GPIO_PIN_7);

    /* PB11: ETH_RMII_TX_EN */
    gpio_para_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_11);
    /* PB12: ETH_RMII_TXD0 */
    gpio_para_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_12);
    /* PB13: ETH_RMII_TXD1 */
    gpio_para_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_13);

    /* PC1: ETH_MDC */
    gpio_para_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_1);
    /* PC4: ETH_RMII_RXD0 */
    gpio_para_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_MAX, GPIO_PIN_4);
    /* PC5: ETH_RMII_RXD1 */
    gpio_para_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_MAX, GPIO_PIN_5);

    /* Enable the Ethernet global Interrupt */
    nvic_irq_enable(ENET_IRQn, 0x7, 0);

    /* Enable ETHERNET clock  */
    rcu_periph_clock_enable(RCU_ENET);
    rcu_periph_clock_enable(RCU_ENETTX);
    rcu_periph_clock_enable(RCU_ENETRX);
}
