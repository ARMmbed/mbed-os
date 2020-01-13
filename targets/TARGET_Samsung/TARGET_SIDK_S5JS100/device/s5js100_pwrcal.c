/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include "cmsis.h"

unsigned int cal_clk_is_enabled(unsigned int id)
{
    return 0;
}

int cal_clk_setrate(unsigned int id, unsigned long rate)
{
    unsigned long parents;
    unsigned int div;
    unsigned int sel;

    switch (id) {
        case p1_upll:
            if (rate == 0) {
                modifyreg32(SCMU_PLL_CON0_UPLL, (1 << 4), 0 << 4);
                modifyreg32(SCMU_PLL_CON0_UPLL, (0x1 << 31), 0);
            } else if ((getreg32(SCMU_PLL_CON0_UPLL) & (1 << 31)) == 0) {
                modifyreg32(SCMU_PLL_CON0_UPLL, (0x1 << 31), 1);
                while (!(getreg32(SCMU_PLL_CON0_UPLL) & (1 << 29))) {
                };
                modifyreg32(SCMU_PLL_CON0_UPLL, (1 << 4), 1 << 4);
            }
            break;
        case m1_timer1:
            if ((rate >= OSCCLK) || (rate == 0)) {
                sel = 0;
            } else {
                sel = 1;
            }
            modifyreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER1_CLK, 1, sel);
            break;
        case m1_timer2:
            if ((rate >= OSCCLK) || (rate == 0)) {
                sel = 0;
            } else {
                sel = 1;
            }
            modifyreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER2_CLK, 1, sel);
            break;
        case m1_timer3:
            if ((rate >= OSCCLK) || (rate == 0)) {
                sel = 0;
            } else {
                sel = 1;
            }
            modifyreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER3_CLK, 1, sel);
            break;
        case m1_timer4:
            if ((rate >= OSCCLK) || (rate == 0)) {
                sel = 0;
            } else {
                sel = 1;
            }
            modifyreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER4_CLK, 1, sel);
            break;
        case m1_timer5:
            if ((rate >= OSCCLK) || (rate == 0)) {
                sel = 0;
            } else {
                sel = 1;
            }
            modifyreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER5_CLK, 1, sel);
            break;
        case d1_upll_clk_ap:
            parents = cal_clk_getrate(p1_upll);
            div = (parents + rate - 1) / rate;
            if (div > 0xf) {
                div = 0xf;
            }
            modifyreg32(SCMU_CLK_CON_DIV_CKDIVS_UPLL_CLK_AP, 0xF, div - 1);
            break;
        case d1_upll_clk_cp:
            parents = cal_clk_getrate(p1_upll);
            div = (parents + rate - 1) / rate;
            if (div > 0xf) {
                div = 0xf;
            }
            modifyreg32(SCMU_CLK_CON_DIV_CKDIVS_UPLL_CLK_CP, 0xF, div - 1);
            break;
        case d1_upll_clk_mif:
            parents = cal_clk_getrate(p1_upll);
            div = (parents + rate - 1) / rate;
            if (div > 0xf) {
                div = 0xf;
            }
            modifyreg32(SCMU_CLK_CON_DIV_CKDIVS_UPLL_CLK_MIF, 0xF, div - 1);
            break;
        case d1_upll_clk_gnss:
            parents = cal_clk_getrate(p1_upll);
            div = (parents + rate - 1) / rate;
            if (div > 0xf) {
                div = 0xf;
            }
            modifyreg32(SCMU_CLK_CON_DIV_CKDIVS_UPLL_CLK_GNSS, 0xF, div - 1);
            break;
        case d1_acpu_l1:
            parents = cal_clk_getrate(d1_upll_clk_ap);
            div = (parents + rate - 1) / rate;
            if (div > 0x1f) {
                div = 0x1f;
            }
            modifyreg32(ACMU_CLK_CON_DIV_MULTI3_CKDIVA_ACPU_CLK, 0x1F, div - 1);
            break;
        case d1_acpu_l2:
            parents = cal_clk_getrate(d1_upll_clk_ap);
            div = (parents + rate - 1) / rate;
            if (div > 0x1f) {
                div = 0x1f;
            }
            modifyreg32(ACMU_CLK_CON_DIV_MULTI3_CKDIVA_ACPU_CLK, 0x1F << 8, (div - 1) << 8);
            break;
        case d1_acpu_l3:
            parents = cal_clk_getrate(d1_upll_clk_ap);
            div = (parents + rate - 1) / rate;
            if (div > 0x1f) {
                div = 0x1f;
            }
            modifyreg32(ACMU_CLK_CON_DIV_MULTI3_CKDIVA_ACPU_CLK, 0x1F << 16, (div - 1) << 16);
            break;
        case d1_sdio:
            parents = cal_clk_getrate(d1_upll_clk_ap);
            div = (parents + rate - 1) / rate;
            if (div > 0x7f) {
                div = 0x7f;
            }
            modifyreg32(ACMU_CLK_CON_DIV_CKDIVA_SDIO_CLK, 0x7F, div - 1);
            break;
        case d1_spi0:
            parents = cal_clk_getrate(d1_upll_clk_ap);
            div = (parents + rate - 1) / rate;
            if (div > 0x1F) {
                div = 0x1F;
            }

            modifyreg32(ACMU_CLK_CON_DIV_CKDIVA_SPI0_CLK, 0x1F, div - 1);
            break;
        case d1_usi0:
            parents = cal_clk_getrate(d1_upll_clk_ap);
            div = (parents + rate - 1) / rate;
            if (div > 0x1F) {
                div = 0x1F;
            }
            modifyreg32(ACMU_CLK_CON_DIV_CKDIVA_USI0_CLK, 0x1F, div - 1);
            break;
        case d1_usi1:
            parents = cal_clk_getrate(d1_upll_clk_ap);
            div = (parents + rate - 1) / rate;
            if (div > 0x1F) {
                div = 0x1F;
            }
            modifyreg32(ACMU_CLK_CON_DIV_CKDIVA_USI1_CLK, 0x1F, div - 1);
            break;
        case d1_mif_l2:
            parents = cal_clk_getrate(d1_upll_clk_mif);
            div = (parents + rate - 1) / rate;
            if (div > 0x1f) {
                div = 0x1f;
            }
            modifyreg32(MIFCMU_CLK_CON_DIV_MULTI2_CKDIVF_MIF_SRC_CLK, 0x1F << 0, (div - 1) << 0);
            break;
        case d1_mif_l3:
            parents = cal_clk_getrate(d1_upll_clk_mif);
            div = (parents + rate - 1) / rate;
            if (div > 0x1f) {
                div = 0x1f;
            }
            modifyreg32(MIFCMU_CLK_CON_DIV_MULTI2_CKDIVF_MIF_SRC_CLK, 0x1F << 8, (div - 1) << 8);
            break;
        case d1_qspi:
            parents = cal_clk_getrate(d1_upll_clk_mif);
            div = (parents + rate - 1) / rate;
            if (div > 0x1F) {
                div = 0x1F;
            }
            modifyreg32(MIFCMU_CLK_CON_DIV_CKDIVF_QSPI_CLK, 0x1F, div - 1);
            break;
        case d1_smc:
            parents = cal_clk_getrate(d1_upll_clk_mif);
            div = (parents + rate - 1) / rate;
            if (div > 0x1F) {
                div = 0x1F;
            }
            modifyreg32(MIFCMU_CLK_CON_DIV_CKDIVF_SMC_CLK, 0x1F, div - 1);
            break;
        case d1_uart0:
            parents = cal_clk_getrate(d1_upll_clk_mif);
            div = (parents + rate - 1) / rate;
            if (div > 0x1F) {
                div = 0x1F;
            }
            modifyreg32(MIFCMU_CLK_CON_DIV_CKDIVF_UART0_CLK, 0x1F, div - 1);
            break;
        case d1_uart1:
            parents = cal_clk_getrate(d1_upll_clk_mif);
            div = (parents + rate - 1) / rate;
            if (div > 0x1F) {
                div = 0x1F;
            }
            modifyreg32(MIFCMU_CLK_CON_DIV_CKDIVF_UART1_CLK, 0x1F, div - 1);
            break;
        default:
            break;
    }

    return 0;
}

unsigned long cal_clk_getrate(unsigned int id)
{
    unsigned long parents;
    unsigned long rate = 0;
    unsigned long div;

    switch (id) {
        case p1_upll:
            if (((getreg32(SCMU_PLL_CON0_UPLL) >> 4) & 1) == 0) {
                rate = OSCCLK;
            } else {
                unsigned long p, m, s;
                p = (getreg32(SCMU_PLL_CON0_UPLL) >> 8) & 0x3f;
                m = (getreg32(SCMU_PLL_CON0_UPLL) >> 16) & 0x3ff;
                s = (getreg32(SCMU_PLL_CON0_UPLL) >> 0) & 0x7;
                rate = (OSCCLK / p * m) >> s;
            }
            break;
        case m1_timer0:
            if ((getreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER0_CLK) & 1) == 0) {
                rate = OSCCLK;
            } else {
                rate = SLPCLK_CP;
            }
            break;
        case m1_timer1:
            if ((getreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER1_CLK) & 1) == 0) {
                rate = OSCCLK;
            } else {
                rate = SLPCLK_CP;
            }
            break;
        case m1_timer2:
            if ((getreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER2_CLK) & 1) == 0) {
                rate = OSCCLK;
            } else {
                rate = SLPCLK_CP;
            }
            break;
        case m1_timer3:
            if ((getreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER3_CLK) & 1) == 0) {
                rate = OSCCLK;
            } else {
                rate = SLPCLK_CP;
            }
            break;
        case m1_timer4:
            if ((getreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER4_CLK) & 1) == 0) {
                rate = OSCCLK;
            } else {
                rate = SLPCLK_CP;
            }
            break;
        case m1_timer5:
            if ((getreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER5_CLK) & 1) == 0) {
                rate = OSCCLK;
            } else {
                rate = SLPCLK_CP;
            }
            break;
        case d1_upll_clk_ap:
            parents = cal_clk_getrate(p1_upll);
            div = (getreg32(SCMU_CLK_CON_DIV_CKDIVS_UPLL_CLK_AP) & 0xF) + 1;
            rate = parents / div;
            break;
        case d1_upll_clk_cp:
            parents = cal_clk_getrate(p1_upll);
            div = (getreg32(SCMU_CLK_CON_DIV_CKDIVS_UPLL_CLK_CP) & 0xF) + 1;
            rate = parents / div;
            break;
        case d1_upll_clk_mif:
            parents = cal_clk_getrate(p1_upll);
            div = (getreg32(SCMU_CLK_CON_DIV_CKDIVS_UPLL_CLK_MIF) & 0xF) + 1;
            rate = parents / div;
            break;
        case d1_upll_clk_gnss:
            parents = cal_clk_getrate(p1_upll);
            div = (getreg32(SCMU_CLK_CON_DIV_CKDIVS_UPLL_CLK_GNSS) & 0xF) + 1;
            rate = parents / div;
            break;
        case d1_acpu_l1:
            parents = cal_clk_getrate(d1_upll_clk_ap);
            div = (getreg32(ACMU_CLK_CON_DIV_MULTI3_CKDIVA_ACPU_CLK) & 0x1F) + 1;
            rate = parents / div;
            break;
        case d1_acpu_l2:
            parents = cal_clk_getrate(d1_upll_clk_ap);
            div = ((getreg32(ACMU_CLK_CON_DIV_MULTI3_CKDIVA_ACPU_CLK) >> 8) & 0x1F) + 1;
            rate = parents / div;
            break;
        case d1_acpu_l3:
            parents = cal_clk_getrate(d1_upll_clk_ap);
            div = ((getreg32(ACMU_CLK_CON_DIV_MULTI3_CKDIVA_ACPU_CLK) >> 16) & 0x1F) + 1;
            rate = parents / div;
            break;
        case d1_sdio:
            parents = cal_clk_getrate(d1_upll_clk_ap);
            div = (getreg32(ACMU_CLK_CON_DIV_CKDIVA_SDIO_CLK) & 0x7F) + 1;
            rate = parents / div;
            break;
        case d1_spi0:
            parents = cal_clk_getrate(d1_upll_clk_ap);
            div = (getreg32(ACMU_CLK_CON_DIV_CKDIVA_SPI0_CLK) & 0x1F) + 1;
            rate = parents / div;
            break;
        case d1_usi0:
            parents = cal_clk_getrate(d1_upll_clk_ap);
            div = (getreg32(ACMU_CLK_CON_DIV_CKDIVA_USI0_CLK) & 0x1F) + 1;
            rate = parents / div;
            break;
        case d1_usi1:
            parents = cal_clk_getrate(d1_upll_clk_ap);
            div = (getreg32(ACMU_CLK_CON_DIV_CKDIVA_USI1_CLK) & 0x1F) + 1;
            rate = parents / div;
            break;
        case d1_mif_l2:
            parents = cal_clk_getrate(d1_upll_clk_mif);
            div = ((getreg32(MIFCMU_CLK_CON_DIV_MULTI2_CKDIVF_MIF_SRC_CLK) >> 0) & 0x1F) + 1;
            rate = parents / div;
            break;
        case d1_mif_l3:
            parents = cal_clk_getrate(d1_upll_clk_mif);
            div = ((getreg32(MIFCMU_CLK_CON_DIV_MULTI2_CKDIVF_MIF_SRC_CLK) >> 8) & 0x1F) + 1;
            rate = parents / div;
            break;
        case d1_qspi:
            parents = cal_clk_getrate(d1_upll_clk_mif);
            div = (getreg32(MIFCMU_CLK_CON_DIV_CKDIVF_QSPI_CLK) & 0x1F) + 1;
            rate = parents / div;
            break;
        case d1_smc:
            parents = cal_clk_getrate(d1_upll_clk_mif);
            div = (getreg32(MIFCMU_CLK_CON_DIV_CKDIVF_SMC_CLK) & 0x1F) + 1;
            rate = parents / div;
            break;
        case d1_uart0:
            parents = cal_clk_getrate(d1_upll_clk_mif);
            div = (getreg32(MIFCMU_CLK_CON_DIV_CKDIVF_UART0_CLK) & 0x1F) + 1;
            rate = parents / div;
            break;
        case d1_uart1:
            parents = cal_clk_getrate(d1_upll_clk_mif);
            div = (getreg32(MIFCMU_CLK_CON_DIV_CKDIVF_UART1_CLK) & 0x1F) + 1;
            rate = parents / div;
            break;
        default:
            break;
    }

    return rate;
}

int cal_clk_enable(unsigned int id)
{
    switch (id) {
        case p1_upll:
            modifyreg32(SCMU_PLL_CON0_UPLL, (0x1 << 31), 1);
            while (!(getreg32(SCMU_PLL_CON0_UPLL) & (1 << 29))) {
            };
            modifyreg32(SCMU_PLL_CON0_UPLL, (1 << 4), 1 << 4);
            break;
        default:
            break;
    }
    return 0;
}

int cal_clk_disable(unsigned int id)
{
    switch (id) {
        case p1_upll:
            modifyreg32(SCMU_PLL_CON0_UPLL, (1 << 4), 0 << 4);
            modifyreg32(SCMU_PLL_CON0_UPLL, (0x1 << 31), 0);
            break;
        default:
            break;
    }
    return 0;
}

int cal_init(void)
{
    /* disable 0 value setting to reduce binary size */

    /* enable UPLL */
    modifyreg32(SCMU_PLL_CON0_UPLL, (0x3ff << 16), (0x18c << 16));
    modifyreg32(SCMU_PLL_CON0_UPLL, (0x1 << 31), (1 << 31));

    modifyreg32(SCMU_CLK_CON_DIV_CKDIVS_UPLL_CLK_AP, 0xf, 1);
    /* need to fix to 0x1 */
    modifyreg32(SCMU_CLK_CON_DIV_CKDIVS_UPLL_CLK_MIF, 0xf, 0x1);

    //  modifyreg32(ACMU_CLK_CON_DIV_MULTI3_CKDIVA_ACPU_CLK, (0x1f << 0), 0); /* L1 CLK */
    //  modifyreg32(ACMU_CLK_CON_DIV_MULTI3_CKDIVA_ACPU_CLK, (0x1f << 8), 0); /* L2 CLK */
    modifyreg32(ACMU_CLK_CON_DIV_MULTI3_CKDIVA_ACPU_CLK, (0x1f << 16), 1 << 16);    /* L3 CLK */

    //  modifyreg32(ACMU_CLK_CON_DIV_CKDIVA_SDIO_CLK, 0x7f, 0);
    modifyreg32(ACMU_CLK_CON_DIV_CKDIVA_SPI0_CLK, 0x1f, 1);
    //  modifyreg32(ACMU_CLK_CON_DIV_CKDIVA_USI0_CLK, 0x1f, 0);
    //  modifyreg32(ACMU_CLK_CON_DIV_CKDIVA_USI1_CLK, 0x1f, 0);

    //  modifyreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER0_CLK, 0x1, 0);
    //  modifyreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER1_CLK, 0x1, 0);
    //  modifyreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER2_CLK, 0x1, 0);
    //  modifyreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER3_CLK, 0x1, 0);
    //  modifyreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER4_CLK, 0x1, 0);
    //  modifyreg32(ACMU_CLK_CON_MUX_CKMUXA_TIMER5_CLK, 0x1, 0);

    //  modifyreg32(MIFCMU_CLK_CON_DIV_MULTI2_CKDIVF_MIF_SRC_CLK, (0x1f << 0), 0); /* L2 CLK */
    modifyreg32(MIFCMU_CLK_CON_DIV_MULTI2_CKDIVF_MIF_SRC_CLK, (0x1f << 8), 1 << 8); /* L3 CLK */

    modifyreg32(MIFCMU_CLK_CON_DIV_CKDIVF_QSPI_CLK, 0x1f, 4);   //QSPI_CLK to be 40Mhz as first
    modifyreg32(MIFCMU_CLK_CON_DIV_CKDIVF_SMC_CLK, 0x1f, 1);
    modifyreg32(MIFCMU_CLK_CON_DIV_CKDIVF_UART0_CLK, 0x1f, 1);
    modifyreg32(MIFCMU_CLK_CON_DIV_CKDIVF_UART1_CLK, 0x1f, 1);

    /* wait for UPLL lock */
    while (!(getreg32(SCMU_PLL_CON0_UPLL) & (1 << 29))) {
    };
    modifyreg32(SCMU_PLL_CON0_UPLL, (0x1 << 4), (1 << 4));
#if 0
    /* Q-CHANNEL enable */
    //modifyreg32(ACMU_AP_ACMU_CONTROLLER_OPTION, (0x3 << 28), (0x3 << 28));
    modifyreg32(SCMU_SYS_SCMU_CONTROLLER_OPTION, (0x3 << 28), (0x3 << 28));
    modifyreg32(MIFCMU_MIF_MIFCMU_CONTROLLER_OPTION, (0x3 << 28), (0x3 << 28));
    /* BUS_FREQ_EN, MIN_FREQ_EN enable */
    modifyreg32(ACMU_ACMU_DFSC_CTL, (0x3 << 1), (0x3 << 1));
#endif

    /* PDMAC Q-CH workaround */
    putreg32(0x4046, ACMU_ACMU_BUS_ACT_MSK);

    /* Gate clocks of unused peripherals */
    // SDIO HOST
    putreg32(0x100000, ACMU_CLK_CON_GAT_GOUT_AP_UID_SDIO_IPCLKPORT_CLK);
    putreg32(0x100000, ACMU_CLK_CON_GAT_GOUT_AP_UID_SDIO_IPCLKPORT_L2CLK);
    putreg32(0x100000, ACMU_CLK_CON_GAT_GOUT_AP_UID_SDIO_IPCLKPORT_SDIO_HCLK);

    // PPMUs
    putreg32(0x100000, ACMU_CLK_CON_GAT_GOUT_AP_UID_ABUS_IPCLKPORT_PPMU_L2CLK);
    putreg32(0x100000, ACMU_CLK_CON_GAT_GOUT_AP_UID_ABUS_IPCLKPORT_PPMU_L3CLK);
    putreg32(0x100000, MIFCMU_CLK_CON_GAT_GOUT_MIF_UID_MIFBUS_IPCLKPORT_PPMU_ACPU2MIF_L2CLK);
    putreg32(0x100000, MIFCMU_CLK_CON_GAT_GOUT_MIF_UID_MIFBUS_IPCLKPORT_PPMU_ACPU2MIF_L3CLK);
    putreg32(0x100000, MIFCMU_CLK_CON_GAT_GOUT_MIF_UID_MIFBUS_IPCLKPORT_PPMU_MCPU2MIF_L2CLK);
    putreg32(0x100000, MIFCMU_CLK_CON_GAT_GOUT_MIF_UID_MIFBUS_IPCLKPORT_PPMU_MCPU2MIF_L3CLK);

    // SMC
    putreg32(0x100000, MIFCMU_CLK_CON_GAT_GOUT_MIF_UID_SMC_IPCLKPORT_CLK);
    putreg32(0x100000, MIFCMU_CLK_CON_GAT_GOUT_MIF_UID_SMC_IPCLKPORT_L2CLK);
    return 0;
}
