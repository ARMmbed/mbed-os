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

#ifndef __S5JS100_VCLK_H__
#define __S5JS100_VCLK_H__
#ifdef __cplusplus
extern "C" {
#endif
enum {
    gate_dummy = 0x0A000000,
    vclk_group_grpgate_end,
    num_of_grpgate = vclk_group_grpgate_end - 0x0A000000,

    sclk_dummy = 0x0A010000,
    vclk_group_m1d1g1_end,
    num_of_m1d1g1 = vclk_group_m1d1g1_end - 0x0A010000,

    p1_upll = 0x0A020000,
    vclk_group_p1_end,
    num_of_p1 = vclk_group_p1_end - 0x0A020000,

    m1_timer0 = 0x0A030000,
    m1_timer1,
    m1_timer2,
    m1_timer3,
    m1_timer4,
    m1_timer5,
    vclk_group_m1_end,
    num_of_m1 = vclk_group_m1_end - 0x0A030000,

    d1_upll_clk_ap = 0x0A040000,
    d1_upll_clk_cp,
    d1_upll_clk_mif,
    d1_upll_clk_gnss,
    d1_acpu_l1,
    d1_acpu_l2,
    d1_acpu_l3,
    d1_sdio,
    d1_spi0,
    d1_usi0,
    d1_usi1,
    d1_mif_l2,
    d1_mif_l3,
    d1_qspi,
    d1_smc,
    d1_uart0,
    d1_uart1,
    vclk_group_d1_end,
    num_of_d1 = vclk_group_d1_end - 0x0A040000,

    pxmxdx_top = 0x0A050000,
    vclk_group_pxmxdx_end,
    num_of_pxmxdx = vclk_group_pxmxdx_end - 0x0A050000,

    umux_dummy = 0x0A060000,
    vclk_group_umux_end,
    num_of_umux = vclk_group_umux_end - 0x0A060000,

    dvfs_dummy = 0x0A070000,
    vclk_group_dfs_end,
    num_of_dfs = vclk_group_dfs_end - 0x0A070000,
};

extern int cal_clk_setrate(unsigned int id, unsigned long rate);
extern unsigned long cal_clk_getrate(unsigned int id);

extern int cal_clk_enable(unsigned int id);
extern int cal_clk_disable(unsigned int id);
#ifdef __cplusplus
}
#endif
#endif
