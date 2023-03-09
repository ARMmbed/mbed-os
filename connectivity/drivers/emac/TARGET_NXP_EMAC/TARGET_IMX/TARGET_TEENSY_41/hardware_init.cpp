/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_clock.h"
#include "mbed_wait_api.h"

#include "DigitalOut.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {true, false, 1};
    CLOCK_InitEnetPll(&config);
}

extern "C" void kinetis_init_eth_hardware(void)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03u */

    // Power down pin (leave high for now, keeping the phy active)
    static mbed::DigitalOut powerDown(GPIO_B0_15, 1);

    // Reset pin
    static mbed::DigitalOut reset(GPIO_B0_14, 1);


    // Use temporary digital outputs to set the strapping options.
    // Note: Cannot use GPIO pullups/pulldowns as they are not strong enough to overcome the phy's
    // 10k internal pulldowns
    {
        mbed::DigitalOut strapPhyAdd0(GPIO_B1_04, 1);
        mbed::DigitalOut strapPhyAdd1(GPIO_B1_06, 0);
        mbed::DigitalOut strapRMIIMode(GPIO_B1_05, 1);

        // Send reset pulse
        reset.write(0);
        wait_us(25); // DP83825 datasheet specifies >=25us reset pulse
        reset.write(1);
        wait_us(2000); // DP83825 datasheet specifies at least 2ms between reset and SMI access
    }


    // REF CLK (high speed output)
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_10_ENET_REF_CLK, 1);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_10_ENET_REF_CLK,
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2));

    // MDIO (bidirectional, open drain with 22k pullup)
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_15_ENET_MDIO, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_15_ENET_MDIO,
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5) |
                        IOMUXC_SW_PAD_CTL_PAD_ODE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(3));

    // MDC (low speed output)
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_ENET_MDC, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_10_ENET_REF_CLK,
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5));

    // RXER (input)
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_ENET_RX_ER, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_ENET_RX_ER, 0);

    // CRS_DV (input with 100k pulldown)
    // Note: Constant is called "ENET_RX_EN" but this signal acts as CRS_DV in RMII mode
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_06_ENET_RX_EN, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_06_ENET_RX_EN,
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(0));

    // TXEN (high speed output)
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_ENET_TX_EN, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_09_ENET_TX_EN,
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3));

    // TXD0 (high speed output)
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_07_ENET_TX_DATA00, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_07_ENET_TX_DATA00,
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3));

    // TXD1 (high speed output)
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_ENET_TX_DATA01, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_ENET_TX_DATA01,
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3));

    // RXD0 (input )
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_04_ENET_RX_DATA00, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_04_ENET_RX_DATA00, 0);

    // RXD1 (input)
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_05_ENET_RX_DATA01, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_05_ENET_RX_DATA01, 0);

    BOARD_InitModuleClock();

    // Set up 50MHz clock output to the phy on GPIO_B1_10
    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/


