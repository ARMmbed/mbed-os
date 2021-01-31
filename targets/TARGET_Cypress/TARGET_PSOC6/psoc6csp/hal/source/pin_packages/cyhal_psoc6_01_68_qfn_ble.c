/***************************************************************************//**
* \file cyhal_psoc6_01_68_qfn_ble.c
*
* \brief
* PSoC6_01 device GPIO HAL header for 68-QFN-BLE package
*
* \note
* Generator version: 1.6.0.229
*
********************************************************************************
* \copyright
* Copyright 2016-2020 Cypress Semiconductor Corporation
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
*******************************************************************************/

#include "cy_device_headers.h"
#include "cyhal_hw_types.h"

#if defined(_GPIO_PSOC6_01_68_QFN_BLE_H_)
#include "pin_packages/cyhal_psoc6_01_68_qfn_ble.h"

/* Hardware Blocks */
static const cyhal_resource_inst_t CYHAL_BLESS_0 = { CYHAL_RSC_BLESS, 0, 0 };
static const cyhal_resource_inst_t CYHAL_LPCOMP_0_0 = { CYHAL_RSC_LPCOMP, 0, 0 };
static const cyhal_resource_inst_t CYHAL_LPCOMP_1_0 = { CYHAL_RSC_LPCOMP, 1, 0 };
static const cyhal_resource_inst_t CYHAL_OPAMP_0 = { CYHAL_RSC_OPAMP, 0, 0 };
static const cyhal_resource_inst_t CYHAL_OPAMP_1 = { CYHAL_RSC_OPAMP, 1, 0 };
static const cyhal_resource_inst_t CYHAL_ADC_0 = { CYHAL_RSC_ADC, 0, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_0 = { CYHAL_RSC_SCB, 0, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_1 = { CYHAL_RSC_SCB, 1, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_2 = { CYHAL_RSC_SCB, 2, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_3 = { CYHAL_RSC_SCB, 3, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_4 = { CYHAL_RSC_SCB, 4, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_5 = { CYHAL_RSC_SCB, 5, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_6 = { CYHAL_RSC_SCB, 6, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_8 = { CYHAL_RSC_SCB, 8, 0 };
static const cyhal_resource_inst_t CYHAL_SMIF_0 = { CYHAL_RSC_SMIF, 0, 0 };
static const cyhal_resource_inst_t CYHAL_TCPWM_0_0 = { CYHAL_RSC_TCPWM, 0, 0 };
static const cyhal_resource_inst_t CYHAL_TCPWM_0_1 = { CYHAL_RSC_TCPWM, 0, 1 };
static const cyhal_resource_inst_t CYHAL_TCPWM_0_2 = { CYHAL_RSC_TCPWM, 0, 2 };
static const cyhal_resource_inst_t CYHAL_TCPWM_0_3 = { CYHAL_RSC_TCPWM, 0, 3 };
static const cyhal_resource_inst_t CYHAL_TCPWM_0_4 = { CYHAL_RSC_TCPWM, 0, 4 };
static const cyhal_resource_inst_t CYHAL_TCPWM_0_5 = { CYHAL_RSC_TCPWM, 0, 5 };
static const cyhal_resource_inst_t CYHAL_TCPWM_0_6 = { CYHAL_RSC_TCPWM, 0, 6 };
static const cyhal_resource_inst_t CYHAL_TCPWM_0_7 = { CYHAL_RSC_TCPWM, 0, 7 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_0 = { CYHAL_RSC_TCPWM, 1, 0 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_1 = { CYHAL_RSC_TCPWM, 1, 1 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_10 = { CYHAL_RSC_TCPWM, 1, 10 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_11 = { CYHAL_RSC_TCPWM, 1, 11 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_12 = { CYHAL_RSC_TCPWM, 1, 12 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_13 = { CYHAL_RSC_TCPWM, 1, 13 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_14 = { CYHAL_RSC_TCPWM, 1, 14 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_15 = { CYHAL_RSC_TCPWM, 1, 15 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_16 = { CYHAL_RSC_TCPWM, 1, 16 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_17 = { CYHAL_RSC_TCPWM, 1, 17 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_2 = { CYHAL_RSC_TCPWM, 1, 2 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_20 = { CYHAL_RSC_TCPWM, 1, 20 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_21 = { CYHAL_RSC_TCPWM, 1, 21 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_22 = { CYHAL_RSC_TCPWM, 1, 22 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_3 = { CYHAL_RSC_TCPWM, 1, 3 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_7 = { CYHAL_RSC_TCPWM, 1, 7 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_8 = { CYHAL_RSC_TCPWM, 1, 8 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_9 = { CYHAL_RSC_TCPWM, 1, 9 };

/* Pin connections */
/* Connections for: audioss_clk_i2s_if */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_clk_i2s_if[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: audioss_pdm_clk */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_pdm_clk[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: audioss_pdm_data */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_pdm_data[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: audioss_rx_sck */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_sck[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: audioss_rx_sdi */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_sdi[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: audioss_rx_ws */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_ws[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: audioss_tx_sck */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_sck[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: audioss_tx_sdo */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_sdo[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: audioss_tx_ws */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_ws[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_ext_lna_rx_ctl_out */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_ext_lna_rx_ctl_out[1] = {
    {&CYHAL_BLESS_0, P7_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_4_BLESS_EXT_LNA_RX_CTL_OUT},
};

/* Connections for: bless_ext_pa_lna_chip_en_out */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_ext_pa_lna_chip_en_out[1] = {
    {&CYHAL_BLESS_0, P7_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_6_BLESS_EXT_PA_LNA_CHIP_EN_OUT},
};

/* Connections for: bless_ext_pa_tx_ctl_out */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_ext_pa_tx_ctl_out[1] = {
    {&CYHAL_BLESS_0, P7_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_5_BLESS_EXT_PA_TX_CTL_OUT},
};

/* Connections for: bless_mxd_act_bpktctl */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_act_bpktctl[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_act_dbus_rx_en */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_act_dbus_rx_en[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_act_dbus_tx_en */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_act_dbus_tx_en[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_act_txd_rxd */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_act_txd_rxd[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_dpslp_act_ldo_en */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_act_ldo_en[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_dpslp_buck_en */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_buck_en[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_dpslp_clk_en */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_clk_en[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_dpslp_dig_ldo_en */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_dig_ldo_en[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_dpslp_isolate_n */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_isolate_n[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_dpslp_mxd_clk_out */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_mxd_clk_out[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_dpslp_rcb_clk */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_rcb_clk[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_dpslp_rcb_data */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_rcb_data[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_dpslp_rcb_le */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_rcb_le[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_dpslp_reset_n */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_reset_n[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_dpslp_ret_ldo_ol_hv */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_ret_ldo_ol_hv[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_dpslp_ret_switch_hv */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_ret_switch_hv[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: bless_mxd_dpslp_xtal_en */
const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_xtal_en[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: dac_ctdac_voutsw */
const cyhal_resource_pin_mapping_t cyhal_pin_map_dac_ctdac_voutsw[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: lpcomp_dsi_comp */
const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_dsi_comp[1] = {
    {&CYHAL_LPCOMP_0_0, P8_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P8_2_LPCOMP_DSI_COMP0},
};

/* Connections for: lpcomp_inn_comp */
const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inn_comp[1] = {
    {&CYHAL_LPCOMP_1_0, P6_3, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
};

/* Connections for: lpcomp_inp_comp */
const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inp_comp[1] = {
    {&CYHAL_LPCOMP_1_0, P6_2, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
};

/* Connections for: opamp_dsi_ctb_cmp */
const cyhal_resource_pin_mapping_t cyhal_pin_map_opamp_dsi_ctb_cmp[2] = {
    {&CYHAL_OPAMP_0, P9_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_2_PASS_DSI_CTB_CMP0},
    {&CYHAL_OPAMP_1, P9_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_3_PASS_DSI_CTB_CMP1},
};

/* Connections for: opamp_out_10x */
const cyhal_resource_pin_mapping_t cyhal_pin_map_opamp_out_10x[2] = {
    {&CYHAL_OPAMP_0, P9_2, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
    {&CYHAL_OPAMP_1, P9_3, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
};

/* Connections for: opamp_vin_m */
const cyhal_resource_pin_mapping_t cyhal_pin_map_opamp_vin_m[1] = {
    {&CYHAL_OPAMP_0, P9_1, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
};

/* Connections for: opamp_vin_p0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_opamp_vin_p0[1] = {
    {&CYHAL_OPAMP_0, P9_0, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
};

/* Connections for: opamp_vin_p1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_opamp_vin_p1[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: pass_sarmux_pads */
const cyhal_resource_pin_mapping_t cyhal_pin_map_pass_sarmux_pads[2] = {
    {&CYHAL_ADC_0, P10_0, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
    {&CYHAL_ADC_0, P10_1, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
};

/* Connections for: scb_i2c_scl */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_scl[10] = {
    {&CYHAL_SCB_0, P0_2, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P0_2_SCB0_I2C_SCL},
    {&CYHAL_SCB_3, P6_0, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P6_0_SCB3_I2C_SCL},
    {&CYHAL_SCB_8, P6_0, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P6_0_SCB8_I2C_SCL},
    {&CYHAL_SCB_6, P6_4, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P6_4_SCB6_I2C_SCL},
    {&CYHAL_SCB_8, P6_4, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P6_4_SCB8_I2C_SCL},
    {&CYHAL_SCB_4, P7_0, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P7_0_SCB4_I2C_SCL},
    {&CYHAL_SCB_4, P8_0, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P8_0_SCB4_I2C_SCL},
    {&CYHAL_SCB_2, P9_0, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P9_0_SCB2_I2C_SCL},
    {&CYHAL_SCB_1, P10_0, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P10_0_SCB1_I2C_SCL},
    {&CYHAL_SCB_5, P11_0, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P11_0_SCB5_I2C_SCL},
};

/* Connections for: scb_i2c_sda */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_sda[10] = {
    {&CYHAL_SCB_0, P0_3, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P0_3_SCB0_I2C_SDA},
    {&CYHAL_SCB_3, P6_1, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P6_1_SCB3_I2C_SDA},
    {&CYHAL_SCB_8, P6_1, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P6_1_SCB8_I2C_SDA},
    {&CYHAL_SCB_6, P6_5, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P6_5_SCB6_I2C_SDA},
    {&CYHAL_SCB_8, P6_5, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P6_5_SCB8_I2C_SDA},
    {&CYHAL_SCB_4, P7_1, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P7_1_SCB4_I2C_SDA},
    {&CYHAL_SCB_4, P8_1, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P8_1_SCB4_I2C_SDA},
    {&CYHAL_SCB_2, P9_1, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P9_1_SCB2_I2C_SDA},
    {&CYHAL_SCB_1, P10_1, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P10_1_SCB1_I2C_SDA},
    {&CYHAL_SCB_5, P11_1, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P11_1_SCB5_I2C_SDA},
};

/* Connections for: scb_spi_m_clk */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_clk[9] = {
    {&CYHAL_SCB_0, P0_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_4_SCB0_SPI_CLK},
    {&CYHAL_SCB_3, P6_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_2_SCB3_SPI_CLK},
    {&CYHAL_SCB_8, P6_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_2_SCB8_SPI_CLK},
    {&CYHAL_SCB_6, P6_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_6_SCB6_SPI_CLK},
    {&CYHAL_SCB_8, P6_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_6_SCB8_SPI_CLK},
    {&CYHAL_SCB_4, P7_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_2_SCB4_SPI_CLK},
    {&CYHAL_SCB_4, P8_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P8_2_SCB4_SPI_CLK},
    {&CYHAL_SCB_2, P9_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_2_SCB2_SPI_CLK},
    {&CYHAL_SCB_5, P11_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_2_SCB5_SPI_CLK},
};

/* Connections for: scb_spi_m_miso */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_miso[10] = {
    {&CYHAL_SCB_0, P0_3, (uint8_t)CY_GPIO_DM_HIGHZ, P0_3_SCB0_SPI_MISO},
    {&CYHAL_SCB_3, P6_1, (uint8_t)CY_GPIO_DM_HIGHZ, P6_1_SCB3_SPI_MISO},
    {&CYHAL_SCB_8, P6_1, (uint8_t)CY_GPIO_DM_HIGHZ, P6_1_SCB8_SPI_MISO},
    {&CYHAL_SCB_6, P6_5, (uint8_t)CY_GPIO_DM_HIGHZ, P6_5_SCB6_SPI_MISO},
    {&CYHAL_SCB_8, P6_5, (uint8_t)CY_GPIO_DM_HIGHZ, P6_5_SCB8_SPI_MISO},
    {&CYHAL_SCB_4, P7_1, (uint8_t)CY_GPIO_DM_HIGHZ, P7_1_SCB4_SPI_MISO},
    {&CYHAL_SCB_4, P8_1, (uint8_t)CY_GPIO_DM_HIGHZ, P8_1_SCB4_SPI_MISO},
    {&CYHAL_SCB_2, P9_1, (uint8_t)CY_GPIO_DM_HIGHZ, P9_1_SCB2_SPI_MISO},
    {&CYHAL_SCB_1, P10_1, (uint8_t)CY_GPIO_DM_HIGHZ, P10_1_SCB1_SPI_MISO},
    {&CYHAL_SCB_5, P11_1, (uint8_t)CY_GPIO_DM_HIGHZ, P11_1_SCB5_SPI_MISO},
};

/* Connections for: scb_spi_m_mosi */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_mosi[10] = {
    {&CYHAL_SCB_0, P0_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_2_SCB0_SPI_MOSI},
    {&CYHAL_SCB_3, P6_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_0_SCB3_SPI_MOSI},
    {&CYHAL_SCB_8, P6_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_0_SCB8_SPI_MOSI},
    {&CYHAL_SCB_6, P6_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_4_SCB6_SPI_MOSI},
    {&CYHAL_SCB_8, P6_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_4_SCB8_SPI_MOSI},
    {&CYHAL_SCB_4, P7_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_0_SCB4_SPI_MOSI},
    {&CYHAL_SCB_4, P8_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P8_0_SCB4_SPI_MOSI},
    {&CYHAL_SCB_2, P9_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_0_SCB2_SPI_MOSI},
    {&CYHAL_SCB_1, P10_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_0_SCB1_SPI_MOSI},
    {&CYHAL_SCB_5, P11_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_0_SCB5_SPI_MOSI},
};

/* Connections for: scb_spi_m_select0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select0[8] = {
    {&CYHAL_SCB_0, P0_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_5_SCB0_SPI_SELECT0},
    {&CYHAL_SCB_3, P6_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_3_SCB3_SPI_SELECT0},
    {&CYHAL_SCB_8, P6_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_3_SCB8_SPI_SELECT0},
    {&CYHAL_SCB_6, P6_7, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_7_SCB6_SPI_SELECT0},
    {&CYHAL_SCB_8, P6_7, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_7_SCB8_SPI_SELECT0},
    {&CYHAL_SCB_4, P7_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_3_SCB4_SPI_SELECT0},
    {&CYHAL_SCB_2, P9_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_3_SCB2_SPI_SELECT0},
    {&CYHAL_SCB_5, P11_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_3_SCB5_SPI_SELECT0},
};

/* Connections for: scb_spi_m_select1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select1[4] = {
    {&CYHAL_SCB_0, P0_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_0_SCB0_SPI_SELECT1},
    {&CYHAL_SCB_4, P7_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_4_SCB4_SPI_SELECT1},
    {&CYHAL_SCB_3, P7_7, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_7_SCB3_SPI_SELECT1},
    {&CYHAL_SCB_5, P11_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_4_SCB5_SPI_SELECT1},
};

/* Connections for: scb_spi_m_select2 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select2[3] = {
    {&CYHAL_SCB_0, P0_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_1_SCB0_SPI_SELECT2},
    {&CYHAL_SCB_4, P7_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_5_SCB4_SPI_SELECT2},
    {&CYHAL_SCB_5, P11_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_5_SCB5_SPI_SELECT2},
};

/* Connections for: scb_spi_m_select3 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select3[3] = {
    {&CYHAL_SCB_4, P7_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_6_SCB4_SPI_SELECT3},
    {&CYHAL_SCB_5, P11_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_6_SCB5_SPI_SELECT3},
    {&CYHAL_SCB_6, P12_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P12_6_SCB6_SPI_SELECT3},
};

/* Connections for: scb_spi_s_clk */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_clk[9] = {
    {&CYHAL_SCB_0, P0_4, (uint8_t)CY_GPIO_DM_HIGHZ, P0_4_SCB0_SPI_CLK},
    {&CYHAL_SCB_3, P6_2, (uint8_t)CY_GPIO_DM_HIGHZ, P6_2_SCB3_SPI_CLK},
    {&CYHAL_SCB_8, P6_2, (uint8_t)CY_GPIO_DM_HIGHZ, P6_2_SCB8_SPI_CLK},
    {&CYHAL_SCB_6, P6_6, (uint8_t)CY_GPIO_DM_HIGHZ, P6_6_SCB6_SPI_CLK},
    {&CYHAL_SCB_8, P6_6, (uint8_t)CY_GPIO_DM_HIGHZ, P6_6_SCB8_SPI_CLK},
    {&CYHAL_SCB_4, P7_2, (uint8_t)CY_GPIO_DM_HIGHZ, P7_2_SCB4_SPI_CLK},
    {&CYHAL_SCB_4, P8_2, (uint8_t)CY_GPIO_DM_HIGHZ, P8_2_SCB4_SPI_CLK},
    {&CYHAL_SCB_2, P9_2, (uint8_t)CY_GPIO_DM_HIGHZ, P9_2_SCB2_SPI_CLK},
    {&CYHAL_SCB_5, P11_2, (uint8_t)CY_GPIO_DM_HIGHZ, P11_2_SCB5_SPI_CLK},
};

/* Connections for: scb_spi_s_miso */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_miso[10] = {
    {&CYHAL_SCB_0, P0_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_3_SCB0_SPI_MISO},
    {&CYHAL_SCB_3, P6_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_1_SCB3_SPI_MISO},
    {&CYHAL_SCB_8, P6_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_1_SCB8_SPI_MISO},
    {&CYHAL_SCB_6, P6_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_5_SCB6_SPI_MISO},
    {&CYHAL_SCB_8, P6_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_5_SCB8_SPI_MISO},
    {&CYHAL_SCB_4, P7_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_1_SCB4_SPI_MISO},
    {&CYHAL_SCB_4, P8_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P8_1_SCB4_SPI_MISO},
    {&CYHAL_SCB_2, P9_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_1_SCB2_SPI_MISO},
    {&CYHAL_SCB_1, P10_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_1_SCB1_SPI_MISO},
    {&CYHAL_SCB_5, P11_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_1_SCB5_SPI_MISO},
};

/* Connections for: scb_spi_s_mosi */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_mosi[10] = {
    {&CYHAL_SCB_0, P0_2, (uint8_t)CY_GPIO_DM_HIGHZ, P0_2_SCB0_SPI_MOSI},
    {&CYHAL_SCB_3, P6_0, (uint8_t)CY_GPIO_DM_HIGHZ, P6_0_SCB3_SPI_MOSI},
    {&CYHAL_SCB_8, P6_0, (uint8_t)CY_GPIO_DM_HIGHZ, P6_0_SCB8_SPI_MOSI},
    {&CYHAL_SCB_6, P6_4, (uint8_t)CY_GPIO_DM_HIGHZ, P6_4_SCB6_SPI_MOSI},
    {&CYHAL_SCB_8, P6_4, (uint8_t)CY_GPIO_DM_HIGHZ, P6_4_SCB8_SPI_MOSI},
    {&CYHAL_SCB_4, P7_0, (uint8_t)CY_GPIO_DM_HIGHZ, P7_0_SCB4_SPI_MOSI},
    {&CYHAL_SCB_4, P8_0, (uint8_t)CY_GPIO_DM_HIGHZ, P8_0_SCB4_SPI_MOSI},
    {&CYHAL_SCB_2, P9_0, (uint8_t)CY_GPIO_DM_HIGHZ, P9_0_SCB2_SPI_MOSI},
    {&CYHAL_SCB_1, P10_0, (uint8_t)CY_GPIO_DM_HIGHZ, P10_0_SCB1_SPI_MOSI},
    {&CYHAL_SCB_5, P11_0, (uint8_t)CY_GPIO_DM_HIGHZ, P11_0_SCB5_SPI_MOSI},
};

/* Connections for: scb_spi_s_select0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select0[8] = {
    {&CYHAL_SCB_0, P0_5, (uint8_t)CY_GPIO_DM_HIGHZ, P0_5_SCB0_SPI_SELECT0},
    {&CYHAL_SCB_3, P6_3, (uint8_t)CY_GPIO_DM_HIGHZ, P6_3_SCB3_SPI_SELECT0},
    {&CYHAL_SCB_8, P6_3, (uint8_t)CY_GPIO_DM_HIGHZ, P6_3_SCB8_SPI_SELECT0},
    {&CYHAL_SCB_6, P6_7, (uint8_t)CY_GPIO_DM_HIGHZ, P6_7_SCB6_SPI_SELECT0},
    {&CYHAL_SCB_8, P6_7, (uint8_t)CY_GPIO_DM_HIGHZ, P6_7_SCB8_SPI_SELECT0},
    {&CYHAL_SCB_4, P7_3, (uint8_t)CY_GPIO_DM_HIGHZ, P7_3_SCB4_SPI_SELECT0},
    {&CYHAL_SCB_2, P9_3, (uint8_t)CY_GPIO_DM_HIGHZ, P9_3_SCB2_SPI_SELECT0},
    {&CYHAL_SCB_5, P11_3, (uint8_t)CY_GPIO_DM_HIGHZ, P11_3_SCB5_SPI_SELECT0},
};

/* Connections for: scb_spi_s_select1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select1[4] = {
    {&CYHAL_SCB_0, P0_0, (uint8_t)CY_GPIO_DM_HIGHZ, P0_0_SCB0_SPI_SELECT1},
    {&CYHAL_SCB_4, P7_4, (uint8_t)CY_GPIO_DM_HIGHZ, P7_4_SCB4_SPI_SELECT1},
    {&CYHAL_SCB_3, P7_7, (uint8_t)CY_GPIO_DM_HIGHZ, P7_7_SCB3_SPI_SELECT1},
    {&CYHAL_SCB_5, P11_4, (uint8_t)CY_GPIO_DM_HIGHZ, P11_4_SCB5_SPI_SELECT1},
};

/* Connections for: scb_spi_s_select2 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select2[3] = {
    {&CYHAL_SCB_0, P0_1, (uint8_t)CY_GPIO_DM_HIGHZ, P0_1_SCB0_SPI_SELECT2},
    {&CYHAL_SCB_4, P7_5, (uint8_t)CY_GPIO_DM_HIGHZ, P7_5_SCB4_SPI_SELECT2},
    {&CYHAL_SCB_5, P11_5, (uint8_t)CY_GPIO_DM_HIGHZ, P11_5_SCB5_SPI_SELECT2},
};

/* Connections for: scb_spi_s_select3 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select3[3] = {
    {&CYHAL_SCB_4, P7_6, (uint8_t)CY_GPIO_DM_HIGHZ, P7_6_SCB4_SPI_SELECT3},
    {&CYHAL_SCB_5, P11_6, (uint8_t)CY_GPIO_DM_HIGHZ, P11_6_SCB5_SPI_SELECT3},
    {&CYHAL_SCB_6, P12_6, (uint8_t)CY_GPIO_DM_HIGHZ, P12_6_SCB6_SPI_SELECT3},
};

/* Connections for: scb_uart_cts */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_cts[6] = {
    {&CYHAL_SCB_0, P0_5, (uint8_t)CY_GPIO_DM_HIGHZ, P0_5_SCB0_UART_CTS},
    {&CYHAL_SCB_3, P6_3, (uint8_t)CY_GPIO_DM_HIGHZ, P6_3_SCB3_UART_CTS},
    {&CYHAL_SCB_6, P6_7, (uint8_t)CY_GPIO_DM_HIGHZ, P6_7_SCB6_UART_CTS},
    {&CYHAL_SCB_4, P7_3, (uint8_t)CY_GPIO_DM_HIGHZ, P7_3_SCB4_UART_CTS},
    {&CYHAL_SCB_2, P9_3, (uint8_t)CY_GPIO_DM_HIGHZ, P9_3_SCB2_UART_CTS},
    {&CYHAL_SCB_5, P11_3, (uint8_t)CY_GPIO_DM_HIGHZ, P11_3_SCB5_UART_CTS},
};

/* Connections for: scb_uart_rts */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rts[7] = {
    {&CYHAL_SCB_0, P0_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_4_SCB0_UART_RTS},
    {&CYHAL_SCB_3, P6_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_2_SCB3_UART_RTS},
    {&CYHAL_SCB_6, P6_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_6_SCB6_UART_RTS},
    {&CYHAL_SCB_4, P7_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_2_SCB4_UART_RTS},
    {&CYHAL_SCB_4, P8_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P8_2_SCB4_UART_RTS},
    {&CYHAL_SCB_2, P9_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_2_SCB2_UART_RTS},
    {&CYHAL_SCB_5, P11_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_2_SCB5_UART_RTS},
};

/* Connections for: scb_uart_rx */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rx[8] = {
    {&CYHAL_SCB_0, P0_2, (uint8_t)CY_GPIO_DM_HIGHZ, P0_2_SCB0_UART_RX},
    {&CYHAL_SCB_3, P6_0, (uint8_t)CY_GPIO_DM_HIGHZ, P6_0_SCB3_UART_RX},
    {&CYHAL_SCB_6, P6_4, (uint8_t)CY_GPIO_DM_HIGHZ, P6_4_SCB6_UART_RX},
    {&CYHAL_SCB_4, P7_0, (uint8_t)CY_GPIO_DM_HIGHZ, P7_0_SCB4_UART_RX},
    {&CYHAL_SCB_4, P8_0, (uint8_t)CY_GPIO_DM_HIGHZ, P8_0_SCB4_UART_RX},
    {&CYHAL_SCB_2, P9_0, (uint8_t)CY_GPIO_DM_HIGHZ, P9_0_SCB2_UART_RX},
    {&CYHAL_SCB_1, P10_0, (uint8_t)CY_GPIO_DM_HIGHZ, P10_0_SCB1_UART_RX},
    {&CYHAL_SCB_5, P11_0, (uint8_t)CY_GPIO_DM_HIGHZ, P11_0_SCB5_UART_RX},
};

/* Connections for: scb_uart_tx */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_tx[8] = {
    {&CYHAL_SCB_0, P0_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_3_SCB0_UART_TX},
    {&CYHAL_SCB_3, P6_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_1_SCB3_UART_TX},
    {&CYHAL_SCB_6, P6_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_5_SCB6_UART_TX},
    {&CYHAL_SCB_4, P7_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_1_SCB4_UART_TX},
    {&CYHAL_SCB_4, P8_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P8_1_SCB4_UART_TX},
    {&CYHAL_SCB_2, P9_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_1_SCB2_UART_TX},
    {&CYHAL_SCB_1, P10_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_1_SCB1_UART_TX},
    {&CYHAL_SCB_5, P11_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_1_SCB5_UART_TX},
};

/* Connections for: smif_spi_clk */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_clk[1] = {
    {&CYHAL_SMIF_0, P11_7, (uint8_t)CY_GPIO_DM_STRONG, P11_7_SMIF_SPI_CLK},
};

/* Connections for: smif_spi_data0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data0[1] = {
    {&CYHAL_SMIF_0, P11_6, (uint8_t)CY_GPIO_DM_STRONG, P11_6_SMIF_SPI_DATA0},
};

/* Connections for: smif_spi_data1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data1[1] = {
    {&CYHAL_SMIF_0, P11_5, (uint8_t)CY_GPIO_DM_STRONG, P11_5_SMIF_SPI_DATA1},
};

/* Connections for: smif_spi_data2 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data2[1] = {
    {&CYHAL_SMIF_0, P11_4, (uint8_t)CY_GPIO_DM_STRONG, P11_4_SMIF_SPI_DATA2},
};

/* Connections for: smif_spi_data3 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data3[1] = {
    {&CYHAL_SMIF_0, P11_3, (uint8_t)CY_GPIO_DM_STRONG, P11_3_SMIF_SPI_DATA3},
};

/* Connections for: smif_spi_data4 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data4[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: smif_spi_data5 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data5[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: smif_spi_data6 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data6[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: smif_spi_data7 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data7[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: smif_spi_select0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select0[1] = {
    {&CYHAL_SMIF_0, P11_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_2_SMIF_SPI_SELECT0},
};

/* Connections for: smif_spi_select1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select1[1] = {
    {&CYHAL_SMIF_0, P11_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_1_SMIF_SPI_SELECT1},
};

/* Connections for: smif_spi_select2 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select2[1] = {
    {&CYHAL_SMIF_0, P11_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_0_SMIF_SPI_SELECT2},
};

/* Connections for: smif_spi_select3 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select3[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: tcpwm_line */
const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line[40] = {
    {&CYHAL_TCPWM_0_0, P0_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_0_TCPWM0_LINE0},
    {&CYHAL_TCPWM_1_0, P0_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_0_TCPWM1_LINE0},
    {&CYHAL_TCPWM_0_1, P0_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_2_TCPWM0_LINE1},
    {&CYHAL_TCPWM_1_1, P0_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_2_TCPWM1_LINE1},
    {&CYHAL_TCPWM_0_2, P0_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_4_TCPWM0_LINE2},
    {&CYHAL_TCPWM_1_2, P0_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_4_TCPWM1_LINE2},
    {&CYHAL_TCPWM_0_0, P6_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_0_TCPWM0_LINE0},
    {&CYHAL_TCPWM_1_8, P6_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_0_TCPWM1_LINE8},
    {&CYHAL_TCPWM_0_1, P6_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_2_TCPWM0_LINE1},
    {&CYHAL_TCPWM_1_9, P6_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_2_TCPWM1_LINE9},
    {&CYHAL_TCPWM_0_2, P6_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_4_TCPWM0_LINE2},
    {&CYHAL_TCPWM_1_10, P6_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_4_TCPWM1_LINE10},
    {&CYHAL_TCPWM_0_3, P6_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_6_TCPWM0_LINE3},
    {&CYHAL_TCPWM_1_11, P6_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_6_TCPWM1_LINE11},
    {&CYHAL_TCPWM_0_4, P7_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_0_TCPWM0_LINE4},
    {&CYHAL_TCPWM_1_12, P7_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_0_TCPWM1_LINE12},
    {&CYHAL_TCPWM_0_5, P7_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_2_TCPWM0_LINE5},
    {&CYHAL_TCPWM_1_13, P7_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_2_TCPWM1_LINE13},
    {&CYHAL_TCPWM_0_6, P7_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_4_TCPWM0_LINE6},
    {&CYHAL_TCPWM_1_14, P7_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_4_TCPWM1_LINE14},
    {&CYHAL_TCPWM_0_7, P7_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_6_TCPWM0_LINE7},
    {&CYHAL_TCPWM_1_15, P7_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_6_TCPWM1_LINE15},
    {&CYHAL_TCPWM_0_0, P8_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P8_0_TCPWM0_LINE0},
    {&CYHAL_TCPWM_1_16, P8_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P8_0_TCPWM1_LINE16},
    {&CYHAL_TCPWM_0_1, P8_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P8_2_TCPWM0_LINE1},
    {&CYHAL_TCPWM_1_17, P8_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P8_2_TCPWM1_LINE17},
    {&CYHAL_TCPWM_0_4, P9_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_0_TCPWM0_LINE4},
    {&CYHAL_TCPWM_1_20, P9_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_0_TCPWM1_LINE20},
    {&CYHAL_TCPWM_0_5, P9_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_2_TCPWM0_LINE5},
    {&CYHAL_TCPWM_1_21, P9_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_2_TCPWM1_LINE21},
    {&CYHAL_TCPWM_0_6, P10_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_0_TCPWM0_LINE6},
    {&CYHAL_TCPWM_1_22, P10_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_0_TCPWM1_LINE22},
    {&CYHAL_TCPWM_0_1, P11_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_0_TCPWM0_LINE1},
    {&CYHAL_TCPWM_1_1, P11_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_0_TCPWM1_LINE1},
    {&CYHAL_TCPWM_0_2, P11_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_2_TCPWM0_LINE2},
    {&CYHAL_TCPWM_1_2, P11_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_2_TCPWM1_LINE2},
    {&CYHAL_TCPWM_0_3, P11_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_4_TCPWM0_LINE3},
    {&CYHAL_TCPWM_1_3, P11_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_4_TCPWM1_LINE3},
    {&CYHAL_TCPWM_0_7, P12_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P12_6_TCPWM0_LINE7},
    {&CYHAL_TCPWM_1_7, P12_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P12_6_TCPWM1_LINE7},
};

/* Connections for: tcpwm_line_compl */
const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line_compl[38] = {
    {&CYHAL_TCPWM_0_0, P0_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_1_TCPWM0_LINE_COMPL0},
    {&CYHAL_TCPWM_1_0, P0_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_1_TCPWM1_LINE_COMPL0},
    {&CYHAL_TCPWM_0_1, P0_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_3_TCPWM0_LINE_COMPL1},
    {&CYHAL_TCPWM_1_1, P0_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_3_TCPWM1_LINE_COMPL1},
    {&CYHAL_TCPWM_0_2, P0_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_5_TCPWM0_LINE_COMPL2},
    {&CYHAL_TCPWM_1_2, P0_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_5_TCPWM1_LINE_COMPL2},
    {&CYHAL_TCPWM_0_0, P6_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_1_TCPWM0_LINE_COMPL0},
    {&CYHAL_TCPWM_1_8, P6_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_1_TCPWM1_LINE_COMPL8},
    {&CYHAL_TCPWM_0_1, P6_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_3_TCPWM0_LINE_COMPL1},
    {&CYHAL_TCPWM_1_9, P6_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_3_TCPWM1_LINE_COMPL9},
    {&CYHAL_TCPWM_0_2, P6_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_5_TCPWM0_LINE_COMPL2},
    {&CYHAL_TCPWM_1_10, P6_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_5_TCPWM1_LINE_COMPL10},
    {&CYHAL_TCPWM_0_3, P6_7, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_7_TCPWM0_LINE_COMPL3},
    {&CYHAL_TCPWM_1_11, P6_7, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_7_TCPWM1_LINE_COMPL11},
    {&CYHAL_TCPWM_0_4, P7_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_1_TCPWM0_LINE_COMPL4},
    {&CYHAL_TCPWM_1_12, P7_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_1_TCPWM1_LINE_COMPL12},
    {&CYHAL_TCPWM_0_5, P7_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_3_TCPWM0_LINE_COMPL5},
    {&CYHAL_TCPWM_1_13, P7_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_3_TCPWM1_LINE_COMPL13},
    {&CYHAL_TCPWM_0_6, P7_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_5_TCPWM0_LINE_COMPL6},
    {&CYHAL_TCPWM_1_14, P7_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_5_TCPWM1_LINE_COMPL14},
    {&CYHAL_TCPWM_0_7, P7_7, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_7_TCPWM0_LINE_COMPL7},
    {&CYHAL_TCPWM_1_15, P7_7, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_7_TCPWM1_LINE_COMPL15},
    {&CYHAL_TCPWM_0_0, P8_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P8_1_TCPWM0_LINE_COMPL0},
    {&CYHAL_TCPWM_1_16, P8_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P8_1_TCPWM1_LINE_COMPL16},
    {&CYHAL_TCPWM_0_4, P9_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_1_TCPWM0_LINE_COMPL4},
    {&CYHAL_TCPWM_1_20, P9_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_1_TCPWM1_LINE_COMPL20},
    {&CYHAL_TCPWM_0_5, P9_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_3_TCPWM0_LINE_COMPL5},
    {&CYHAL_TCPWM_1_21, P9_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_3_TCPWM1_LINE_COMPL21},
    {&CYHAL_TCPWM_0_6, P10_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_1_TCPWM0_LINE_COMPL6},
    {&CYHAL_TCPWM_1_22, P10_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_1_TCPWM1_LINE_COMPL22},
    {&CYHAL_TCPWM_0_1, P11_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_1_TCPWM0_LINE_COMPL1},
    {&CYHAL_TCPWM_1_1, P11_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_1_TCPWM1_LINE_COMPL1},
    {&CYHAL_TCPWM_0_2, P11_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_3_TCPWM0_LINE_COMPL2},
    {&CYHAL_TCPWM_1_2, P11_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_3_TCPWM1_LINE_COMPL2},
    {&CYHAL_TCPWM_0_3, P11_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_5_TCPWM0_LINE_COMPL3},
    {&CYHAL_TCPWM_1_3, P11_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_5_TCPWM1_LINE_COMPL3},
    {&CYHAL_TCPWM_0_7, P12_7, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P12_7_TCPWM0_LINE_COMPL7},
    {&CYHAL_TCPWM_1_7, P12_7, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P12_7_TCPWM1_LINE_COMPL7},
};

/* Connections for: usb_usb_dm_pad */
const cyhal_resource_pin_mapping_t cyhal_pin_map_usb_usb_dm_pad[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: usb_usb_dp_pad */
const cyhal_resource_pin_mapping_t cyhal_pin_map_usb_usb_dp_pad[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

#endif
