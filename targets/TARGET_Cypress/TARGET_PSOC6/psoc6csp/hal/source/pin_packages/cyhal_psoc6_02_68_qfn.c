/***************************************************************************//**
* \file cyhal_psoc6_02_68_qfn.c
*
* \brief
* PSoC6_02 device GPIO HAL header for 68-QFN package
*
* \note
* Generator version: 1.4.7153.30079
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

#if defined(_GPIO_PSOC6_02_68_QFN_H_)
#include "pin_packages/cyhal_psoc6_02_68_qfn.h"

/* Hardware Blocks */
static const cyhal_resource_inst_t CYHAL_I2S_0 = { CYHAL_RSC_I2S, 0, 0 };
static const cyhal_resource_inst_t CYHAL_PDM_0 = { CYHAL_RSC_PDM, 0, 0 };
static const cyhal_resource_inst_t CYHAL_I2S_1 = { CYHAL_RSC_I2S, 1, 0 };
static const cyhal_resource_inst_t CYHAL_LPCOMP_0_0 = { CYHAL_RSC_LPCOMP, 0, 0 };
static const cyhal_resource_inst_t CYHAL_ADC_0 = { CYHAL_RSC_ADC, 0, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_0 = { CYHAL_RSC_SCB, 0, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_1 = { CYHAL_RSC_SCB, 1, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_10 = { CYHAL_RSC_SCB, 10, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_2 = { CYHAL_RSC_SCB, 2, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_3 = { CYHAL_RSC_SCB, 3, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_4 = { CYHAL_RSC_SCB, 4, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_5 = { CYHAL_RSC_SCB, 5, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_6 = { CYHAL_RSC_SCB, 6, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_8 = { CYHAL_RSC_SCB, 8, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_9 = { CYHAL_RSC_SCB, 9, 0 };
static const cyhal_resource_inst_t CYHAL_SDHC_0 = { CYHAL_RSC_SDHC, 0, 0 };
static const cyhal_resource_inst_t CYHAL_SDHC_1 = { CYHAL_RSC_SDHC, 1, 0 };
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
static const cyhal_resource_inst_t CYHAL_TCPWM_1_15 = { CYHAL_RSC_TCPWM, 1, 15 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_16 = { CYHAL_RSC_TCPWM, 1, 16 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_17 = { CYHAL_RSC_TCPWM, 1, 17 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_18 = { CYHAL_RSC_TCPWM, 1, 18 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_19 = { CYHAL_RSC_TCPWM, 1, 19 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_2 = { CYHAL_RSC_TCPWM, 1, 2 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_20 = { CYHAL_RSC_TCPWM, 1, 20 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_21 = { CYHAL_RSC_TCPWM, 1, 21 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_22 = { CYHAL_RSC_TCPWM, 1, 22 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_23 = { CYHAL_RSC_TCPWM, 1, 23 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_3 = { CYHAL_RSC_TCPWM, 1, 3 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_4 = { CYHAL_RSC_TCPWM, 1, 4 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_7 = { CYHAL_RSC_TCPWM, 1, 7 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_9 = { CYHAL_RSC_TCPWM, 1, 9 };
static const cyhal_resource_inst_t CYHAL_USB_0 = { CYHAL_RSC_USB, 0, 0 };

/* Pin connections */
/* Connections for: audioss_clk_i2s_if */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_clk_i2s_if[3] = {
    {P5_0, &CYHAL_I2S_0, CYHAL_PIN_IN_FUNCTION(P5_0_AUDIOSS0_CLK_I2S_IF)},
    {P9_0, &CYHAL_I2S_0, CYHAL_PIN_IN_FUNCTION(P9_0_AUDIOSS0_CLK_I2S_IF)},
    {P11_0, &CYHAL_I2S_1, CYHAL_PIN_IN_FUNCTION(P11_0_AUDIOSS1_CLK_I2S_IF)},
};

/* Connections for: audioss_pdm_clk */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_pdm_clk[1] = {
    {P10_4, &CYHAL_PDM_0, CYHAL_PIN_OUT_FUNCTION(P10_4_AUDIOSS0_PDM_CLK)},
};

/* Connections for: audioss_pdm_data */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_pdm_data[1] = {
    {P10_5, &CYHAL_PDM_0, CYHAL_PIN_IN_FUNCTION(P10_5_AUDIOSS0_PDM_DATA)},
};

/* Connections for: audioss_rx_sck */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_sck[1] = {
    {P11_4, &CYHAL_I2S_1, CYHAL_PIN_OUT_FUNCTION(P11_4_AUDIOSS1_RX_SCK)},
};

/* Connections for: audioss_rx_sdi */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_sdi[2] = {
    {P5_6, &CYHAL_I2S_0, CYHAL_PIN_IN_FUNCTION(P5_6_AUDIOSS0_RX_SDI)},
    {P11_6, &CYHAL_I2S_1, CYHAL_PIN_IN_FUNCTION(P11_6_AUDIOSS1_RX_SDI)},
};

/* Connections for: audioss_rx_ws */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_ws[1] = {
    {P11_5, &CYHAL_I2S_1, CYHAL_PIN_OUT_FUNCTION(P11_5_AUDIOSS1_RX_WS)},
};

/* Connections for: audioss_tx_sck */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_sck[3] = {
    {P5_1, &CYHAL_I2S_0, CYHAL_PIN_OUT_FUNCTION(P5_1_AUDIOSS0_TX_SCK)},
    {P9_1, &CYHAL_I2S_0, CYHAL_PIN_OUT_FUNCTION(P9_1_AUDIOSS0_TX_SCK)},
    {P11_1, &CYHAL_I2S_1, CYHAL_PIN_OUT_FUNCTION(P11_1_AUDIOSS1_TX_SCK)},
};

/* Connections for: audioss_tx_sdo */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_sdo[2] = {
    {P9_3, &CYHAL_I2S_0, CYHAL_PIN_OUT_FUNCTION(P9_3_AUDIOSS0_TX_SDO)},
    {P11_3, &CYHAL_I2S_1, CYHAL_PIN_OUT_FUNCTION(P11_3_AUDIOSS1_TX_SDO)},
};

/* Connections for: audioss_tx_ws */
const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_ws[2] = {
    {P9_2, &CYHAL_I2S_0, CYHAL_PIN_OUT_FUNCTION(P9_2_AUDIOSS0_TX_WS)},
    {P11_2, &CYHAL_I2S_1, CYHAL_PIN_OUT_FUNCTION(P11_2_AUDIOSS1_TX_WS)},
};

/* Connections for: lpcomp_dsi_comp0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_dsi_comp0[1] = {
    {NC, NULL, 0u},
};

/* Connections for: lpcomp_dsi_comp1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_dsi_comp1[1] = {
    {NC, NULL, 0u},
};

/* Connections for: lpcomp_inn_comp0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inn_comp0[1] = {
    {P5_7, &CYHAL_LPCOMP_0_0, CYHAL_PIN_ANALOG_FUNCTION(P5_7_LPCOMP_INN_COMP0)},
};

/* Connections for: lpcomp_inn_comp1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inn_comp1[1] = {
    {P6_3, &CYHAL_LPCOMP_0_0, CYHAL_PIN_ANALOG_FUNCTION(P6_3_LPCOMP_INN_COMP1)},
};

/* Connections for: lpcomp_inp_comp0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inp_comp0[1] = {
    {P5_6, &CYHAL_LPCOMP_0_0, CYHAL_PIN_ANALOG_FUNCTION(P5_6_LPCOMP_INP_COMP0)},
};

/* Connections for: lpcomp_inp_comp1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inp_comp1[1] = {
    {P6_2, &CYHAL_LPCOMP_0_0, CYHAL_PIN_ANALOG_FUNCTION(P6_2_LPCOMP_INP_COMP1)},
};

/* Connections for: pass_sarmux_pads */
const cyhal_resource_pin_mapping_t cyhal_pin_map_pass_sarmux_pads[6] = {
    {P10_0, &CYHAL_ADC_0, CYHAL_PIN_ANALOG_FUNCTION(P10_0_PASS_SARMUX_PADS0)},
    {P10_1, &CYHAL_ADC_0, CYHAL_PIN_ANALOG_FUNCTION(P10_1_PASS_SARMUX_PADS1)},
    {P10_2, &CYHAL_ADC_0, CYHAL_PIN_ANALOG_FUNCTION(P10_2_PASS_SARMUX_PADS2)},
    {P10_3, &CYHAL_ADC_0, CYHAL_PIN_ANALOG_FUNCTION(P10_3_PASS_SARMUX_PADS3)},
    {P10_4, &CYHAL_ADC_0, CYHAL_PIN_ANALOG_FUNCTION(P10_4_PASS_SARMUX_PADS4)},
    {P10_5, &CYHAL_ADC_0, CYHAL_PIN_ANALOG_FUNCTION(P10_5_PASS_SARMUX_PADS5)},
};

/* Connections for: scb_i2c_scl */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_scl[12] = {
    {P0_2, &CYHAL_SCB_0, CYHAL_PIN_OD_FUNCTION(P0_2_SCB0_I2C_SCL)},
    {P2_0, &CYHAL_SCB_1, CYHAL_PIN_OD_FUNCTION(P2_0_SCB1_I2C_SCL)},
    {P2_4, &CYHAL_SCB_9, CYHAL_PIN_OD_FUNCTION(P2_4_SCB9_I2C_SCL)},
    {P3_0, &CYHAL_SCB_2, CYHAL_PIN_OD_FUNCTION(P3_0_SCB2_I2C_SCL)},
    {P5_0, &CYHAL_SCB_5, CYHAL_PIN_OD_FUNCTION(P5_0_SCB5_I2C_SCL)},
    {P6_4, &CYHAL_SCB_6, CYHAL_PIN_OD_FUNCTION(P6_4_SCB6_I2C_SCL)},
    {P6_4, &CYHAL_SCB_8, CYHAL_PIN_OD_FUNCTION(P6_4_SCB8_I2C_SCL)},
    {P7_0, &CYHAL_SCB_4, CYHAL_PIN_OD_FUNCTION(P7_0_SCB4_I2C_SCL)},
    {P8_0, &CYHAL_SCB_4, CYHAL_PIN_OD_FUNCTION(P8_0_SCB4_I2C_SCL)},
    {P9_0, &CYHAL_SCB_2, CYHAL_PIN_OD_FUNCTION(P9_0_SCB2_I2C_SCL)},
    {P10_0, &CYHAL_SCB_1, CYHAL_PIN_OD_FUNCTION(P10_0_SCB1_I2C_SCL)},
    {P11_0, &CYHAL_SCB_5, CYHAL_PIN_OD_FUNCTION(P11_0_SCB5_I2C_SCL)},
};

/* Connections for: scb_i2c_sda */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_sda[12] = {
    {P0_3, &CYHAL_SCB_0, CYHAL_PIN_OD_FUNCTION(P0_3_SCB0_I2C_SDA)},
    {P2_1, &CYHAL_SCB_1, CYHAL_PIN_OD_FUNCTION(P2_1_SCB1_I2C_SDA)},
    {P2_5, &CYHAL_SCB_9, CYHAL_PIN_OD_FUNCTION(P2_5_SCB9_I2C_SDA)},
    {P3_1, &CYHAL_SCB_2, CYHAL_PIN_OD_FUNCTION(P3_1_SCB2_I2C_SDA)},
    {P5_1, &CYHAL_SCB_5, CYHAL_PIN_OD_FUNCTION(P5_1_SCB5_I2C_SDA)},
    {P6_5, &CYHAL_SCB_6, CYHAL_PIN_OD_FUNCTION(P6_5_SCB6_I2C_SDA)},
    {P6_5, &CYHAL_SCB_8, CYHAL_PIN_OD_FUNCTION(P6_5_SCB8_I2C_SDA)},
    {P7_1, &CYHAL_SCB_4, CYHAL_PIN_OD_FUNCTION(P7_1_SCB4_I2C_SDA)},
    {P8_1, &CYHAL_SCB_4, CYHAL_PIN_OD_FUNCTION(P8_1_SCB4_I2C_SDA)},
    {P9_1, &CYHAL_SCB_2, CYHAL_PIN_OD_FUNCTION(P9_1_SCB2_I2C_SDA)},
    {P10_1, &CYHAL_SCB_1, CYHAL_PIN_OD_FUNCTION(P10_1_SCB1_I2C_SDA)},
    {P11_1, &CYHAL_SCB_5, CYHAL_PIN_OD_FUNCTION(P11_1_SCB5_I2C_SDA)},
};

/* Connections for: scb_spi_m_clk */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_clk[10] = {
    {P0_4, &CYHAL_SCB_0, CYHAL_PIN_OUT_FUNCTION(P0_4_SCB0_SPI_CLK)},
    {P2_2, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_2_SCB1_SPI_CLK)},
    {P6_2, &CYHAL_SCB_3, CYHAL_PIN_OUT_FUNCTION(P6_2_SCB3_SPI_CLK)},
    {P6_2, &CYHAL_SCB_8, CYHAL_PIN_OUT_FUNCTION(P6_2_SCB8_SPI_CLK)},
    {P6_6, &CYHAL_SCB_6, CYHAL_PIN_OUT_FUNCTION(P6_6_SCB6_SPI_CLK)},
    {P6_6, &CYHAL_SCB_8, CYHAL_PIN_OUT_FUNCTION(P6_6_SCB8_SPI_CLK)},
    {P7_2, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P7_2_SCB4_SPI_CLK)},
    {P9_2, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P9_2_SCB2_SPI_CLK)},
    {P10_2, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P10_2_SCB1_SPI_CLK)},
    {P11_2, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P11_2_SCB5_SPI_CLK)},
};

/* Connections for: scb_spi_m_miso */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_miso[11] = {
    {P0_3, &CYHAL_SCB_0, CYHAL_PIN_IN_FUNCTION(P0_3_SCB0_SPI_MISO)},
    {P2_1, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_1_SCB1_SPI_MISO)},
    {P3_1, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P3_1_SCB2_SPI_MISO)},
    {P5_1, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P5_1_SCB5_SPI_MISO)},
    {P6_5, &CYHAL_SCB_6, CYHAL_PIN_IN_FUNCTION(P6_5_SCB6_SPI_MISO)},
    {P6_5, &CYHAL_SCB_8, CYHAL_PIN_IN_FUNCTION(P6_5_SCB8_SPI_MISO)},
    {P7_1, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P7_1_SCB4_SPI_MISO)},
    {P8_1, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P8_1_SCB4_SPI_MISO)},
    {P9_1, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P9_1_SCB2_SPI_MISO)},
    {P10_1, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P10_1_SCB1_SPI_MISO)},
    {P11_1, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P11_1_SCB5_SPI_MISO)},
};

/* Connections for: scb_spi_m_mosi */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_mosi[11] = {
    {P0_2, &CYHAL_SCB_0, CYHAL_PIN_OUT_FUNCTION(P0_2_SCB0_SPI_MOSI)},
    {P2_0, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_0_SCB1_SPI_MOSI)},
    {P3_0, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P3_0_SCB2_SPI_MOSI)},
    {P5_0, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P5_0_SCB5_SPI_MOSI)},
    {P6_4, &CYHAL_SCB_6, CYHAL_PIN_OUT_FUNCTION(P6_4_SCB6_SPI_MOSI)},
    {P6_4, &CYHAL_SCB_8, CYHAL_PIN_OUT_FUNCTION(P6_4_SCB8_SPI_MOSI)},
    {P7_0, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P7_0_SCB4_SPI_MOSI)},
    {P8_0, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P8_0_SCB4_SPI_MOSI)},
    {P9_0, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P9_0_SCB2_SPI_MOSI)},
    {P10_0, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P10_0_SCB1_SPI_MOSI)},
    {P11_0, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P11_0_SCB5_SPI_MOSI)},
};

/* Connections for: scb_spi_m_select0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select0[10] = {
    {P0_5, &CYHAL_SCB_0, CYHAL_PIN_OUT_FUNCTION(P0_5_SCB0_SPI_SELECT0)},
    {P2_3, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_3_SCB1_SPI_SELECT0)},
    {P6_3, &CYHAL_SCB_3, CYHAL_PIN_OUT_FUNCTION(P6_3_SCB3_SPI_SELECT0)},
    {P6_3, &CYHAL_SCB_8, CYHAL_PIN_OUT_FUNCTION(P6_3_SCB8_SPI_SELECT0)},
    {P6_7, &CYHAL_SCB_6, CYHAL_PIN_OUT_FUNCTION(P6_7_SCB6_SPI_SELECT0)},
    {P6_7, &CYHAL_SCB_8, CYHAL_PIN_OUT_FUNCTION(P6_7_SCB8_SPI_SELECT0)},
    {P7_3, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P7_3_SCB4_SPI_SELECT0)},
    {P9_3, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P9_3_SCB2_SPI_SELECT0)},
    {P10_3, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P10_3_SCB1_SPI_SELECT0)},
    {P11_3, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P11_3_SCB5_SPI_SELECT0)},
};

/* Connections for: scb_spi_m_select1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select1[5] = {
    {P0_0, &CYHAL_SCB_0, CYHAL_PIN_OUT_FUNCTION(P0_0_SCB0_SPI_SELECT1)},
    {P2_4, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_4_SCB1_SPI_SELECT1)},
    {P7_7, &CYHAL_SCB_3, CYHAL_PIN_OUT_FUNCTION(P7_7_SCB3_SPI_SELECT1)},
    {P10_4, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P10_4_SCB1_SPI_SELECT1)},
    {P11_4, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P11_4_SCB5_SPI_SELECT1)},
};

/* Connections for: scb_spi_m_select2 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select2[4] = {
    {P0_1, &CYHAL_SCB_0, CYHAL_PIN_OUT_FUNCTION(P0_1_SCB0_SPI_SELECT2)},
    {P2_5, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_5_SCB1_SPI_SELECT2)},
    {P10_5, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P10_5_SCB1_SPI_SELECT2)},
    {P11_5, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P11_5_SCB5_SPI_SELECT2)},
};

/* Connections for: scb_spi_m_select3 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select3[5] = {
    {P2_6, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_6_SCB1_SPI_SELECT3)},
    {P5_6, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P5_6_SCB5_SPI_SELECT3)},
    {P5_7, &CYHAL_SCB_3, CYHAL_PIN_OUT_FUNCTION(P5_7_SCB3_SPI_SELECT3)},
    {P11_6, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P11_6_SCB5_SPI_SELECT3)},
    {P12_6, &CYHAL_SCB_6, CYHAL_PIN_OUT_FUNCTION(P12_6_SCB6_SPI_SELECT3)},
};

/* Connections for: scb_spi_s_clk */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_clk[10] = {
    {P0_4, &CYHAL_SCB_0, CYHAL_PIN_IN_FUNCTION(P0_4_SCB0_SPI_CLK)},
    {P2_2, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_2_SCB1_SPI_CLK)},
    {P6_2, &CYHAL_SCB_3, CYHAL_PIN_IN_FUNCTION(P6_2_SCB3_SPI_CLK)},
    {P6_2, &CYHAL_SCB_8, CYHAL_PIN_IN_FUNCTION(P6_2_SCB8_SPI_CLK)},
    {P6_6, &CYHAL_SCB_6, CYHAL_PIN_IN_FUNCTION(P6_6_SCB6_SPI_CLK)},
    {P6_6, &CYHAL_SCB_8, CYHAL_PIN_IN_FUNCTION(P6_6_SCB8_SPI_CLK)},
    {P7_2, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P7_2_SCB4_SPI_CLK)},
    {P9_2, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P9_2_SCB2_SPI_CLK)},
    {P10_2, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P10_2_SCB1_SPI_CLK)},
    {P11_2, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P11_2_SCB5_SPI_CLK)},
};

/* Connections for: scb_spi_s_miso */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_miso[11] = {
    {P0_3, &CYHAL_SCB_0, CYHAL_PIN_OUT_FUNCTION(P0_3_SCB0_SPI_MISO)},
    {P2_1, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_1_SCB1_SPI_MISO)},
    {P3_1, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P3_1_SCB2_SPI_MISO)},
    {P5_1, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P5_1_SCB5_SPI_MISO)},
    {P6_5, &CYHAL_SCB_6, CYHAL_PIN_OUT_FUNCTION(P6_5_SCB6_SPI_MISO)},
    {P6_5, &CYHAL_SCB_8, CYHAL_PIN_OUT_FUNCTION(P6_5_SCB8_SPI_MISO)},
    {P7_1, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P7_1_SCB4_SPI_MISO)},
    {P8_1, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P8_1_SCB4_SPI_MISO)},
    {P9_1, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P9_1_SCB2_SPI_MISO)},
    {P10_1, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P10_1_SCB1_SPI_MISO)},
    {P11_1, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P11_1_SCB5_SPI_MISO)},
};

/* Connections for: scb_spi_s_mosi */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_mosi[11] = {
    {P0_2, &CYHAL_SCB_0, CYHAL_PIN_IN_FUNCTION(P0_2_SCB0_SPI_MOSI)},
    {P2_0, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_0_SCB1_SPI_MOSI)},
    {P3_0, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P3_0_SCB2_SPI_MOSI)},
    {P5_0, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P5_0_SCB5_SPI_MOSI)},
    {P6_4, &CYHAL_SCB_6, CYHAL_PIN_IN_FUNCTION(P6_4_SCB6_SPI_MOSI)},
    {P6_4, &CYHAL_SCB_8, CYHAL_PIN_IN_FUNCTION(P6_4_SCB8_SPI_MOSI)},
    {P7_0, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P7_0_SCB4_SPI_MOSI)},
    {P8_0, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P8_0_SCB4_SPI_MOSI)},
    {P9_0, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P9_0_SCB2_SPI_MOSI)},
    {P10_0, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P10_0_SCB1_SPI_MOSI)},
    {P11_0, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P11_0_SCB5_SPI_MOSI)},
};

/* Connections for: scb_spi_s_select0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select0[10] = {
    {P0_5, &CYHAL_SCB_0, CYHAL_PIN_IN_FUNCTION(P0_5_SCB0_SPI_SELECT0)},
    {P2_3, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_3_SCB1_SPI_SELECT0)},
    {P6_3, &CYHAL_SCB_3, CYHAL_PIN_IN_FUNCTION(P6_3_SCB3_SPI_SELECT0)},
    {P6_3, &CYHAL_SCB_8, CYHAL_PIN_IN_FUNCTION(P6_3_SCB8_SPI_SELECT0)},
    {P6_7, &CYHAL_SCB_6, CYHAL_PIN_IN_FUNCTION(P6_7_SCB6_SPI_SELECT0)},
    {P6_7, &CYHAL_SCB_8, CYHAL_PIN_IN_FUNCTION(P6_7_SCB8_SPI_SELECT0)},
    {P7_3, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P7_3_SCB4_SPI_SELECT0)},
    {P9_3, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P9_3_SCB2_SPI_SELECT0)},
    {P10_3, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P10_3_SCB1_SPI_SELECT0)},
    {P11_3, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P11_3_SCB5_SPI_SELECT0)},
};

/* Connections for: scb_spi_s_select1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select1[5] = {
    {P0_0, &CYHAL_SCB_0, CYHAL_PIN_IN_FUNCTION(P0_0_SCB0_SPI_SELECT1)},
    {P2_4, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_4_SCB1_SPI_SELECT1)},
    {P7_7, &CYHAL_SCB_3, CYHAL_PIN_IN_FUNCTION(P7_7_SCB3_SPI_SELECT1)},
    {P10_4, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P10_4_SCB1_SPI_SELECT1)},
    {P11_4, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P11_4_SCB5_SPI_SELECT1)},
};

/* Connections for: scb_spi_s_select2 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select2[4] = {
    {P0_1, &CYHAL_SCB_0, CYHAL_PIN_IN_FUNCTION(P0_1_SCB0_SPI_SELECT2)},
    {P2_5, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_5_SCB1_SPI_SELECT2)},
    {P10_5, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P10_5_SCB1_SPI_SELECT2)},
    {P11_5, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P11_5_SCB5_SPI_SELECT2)},
};

/* Connections for: scb_spi_s_select3 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select3[5] = {
    {P2_6, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_6_SCB1_SPI_SELECT3)},
    {P5_6, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P5_6_SCB5_SPI_SELECT3)},
    {P5_7, &CYHAL_SCB_3, CYHAL_PIN_IN_FUNCTION(P5_7_SCB3_SPI_SELECT3)},
    {P11_6, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P11_6_SCB5_SPI_SELECT3)},
    {P12_6, &CYHAL_SCB_6, CYHAL_PIN_IN_FUNCTION(P12_6_SCB6_SPI_SELECT3)},
};

/* Connections for: scb_uart_cts */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_cts[10] = {
    {P0_5, &CYHAL_SCB_0, CYHAL_PIN_IN_FUNCTION(P0_5_SCB0_UART_CTS)},
    {P2_3, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_3_SCB1_UART_CTS)},
    {P2_7, &CYHAL_SCB_9, CYHAL_PIN_IN_FUNCTION(P2_7_SCB9_UART_CTS)},
    {P5_7, &CYHAL_SCB_10, CYHAL_PIN_IN_FUNCTION(P5_7_SCB10_UART_CTS)},
    {P6_3, &CYHAL_SCB_3, CYHAL_PIN_IN_FUNCTION(P6_3_SCB3_UART_CTS)},
    {P6_7, &CYHAL_SCB_6, CYHAL_PIN_IN_FUNCTION(P6_7_SCB6_UART_CTS)},
    {P7_3, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P7_3_SCB4_UART_CTS)},
    {P9_3, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P9_3_SCB2_UART_CTS)},
    {P10_3, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P10_3_SCB1_UART_CTS)},
    {P11_3, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P11_3_SCB5_UART_CTS)},
};

/* Connections for: scb_uart_rts */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rts[10] = {
    {P0_4, &CYHAL_SCB_0, CYHAL_PIN_OUT_FUNCTION(P0_4_SCB0_UART_RTS)},
    {P2_2, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_2_SCB1_UART_RTS)},
    {P2_6, &CYHAL_SCB_9, CYHAL_PIN_OUT_FUNCTION(P2_6_SCB9_UART_RTS)},
    {P5_6, &CYHAL_SCB_10, CYHAL_PIN_OUT_FUNCTION(P5_6_SCB10_UART_RTS)},
    {P6_2, &CYHAL_SCB_3, CYHAL_PIN_OUT_FUNCTION(P6_2_SCB3_UART_RTS)},
    {P6_6, &CYHAL_SCB_6, CYHAL_PIN_OUT_FUNCTION(P6_6_SCB6_UART_RTS)},
    {P7_2, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P7_2_SCB4_UART_RTS)},
    {P9_2, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P9_2_SCB2_UART_RTS)},
    {P10_2, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P10_2_SCB1_UART_RTS)},
    {P11_2, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P11_2_SCB5_UART_RTS)},
};

/* Connections for: scb_uart_rx */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rx[11] = {
    {P0_2, &CYHAL_SCB_0, CYHAL_PIN_IN_FUNCTION(P0_2_SCB0_UART_RX)},
    {P2_0, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_0_SCB1_UART_RX)},
    {P2_4, &CYHAL_SCB_9, CYHAL_PIN_IN_FUNCTION(P2_4_SCB9_UART_RX)},
    {P3_0, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P3_0_SCB2_UART_RX)},
    {P5_0, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P5_0_SCB5_UART_RX)},
    {P6_4, &CYHAL_SCB_6, CYHAL_PIN_IN_FUNCTION(P6_4_SCB6_UART_RX)},
    {P7_0, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P7_0_SCB4_UART_RX)},
    {P8_0, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P8_0_SCB4_UART_RX)},
    {P9_0, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P9_0_SCB2_UART_RX)},
    {P10_0, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P10_0_SCB1_UART_RX)},
    {P11_0, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P11_0_SCB5_UART_RX)},
};

/* Connections for: scb_uart_tx */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_tx[11] = {
    {P0_3, &CYHAL_SCB_0, CYHAL_PIN_OUT_FUNCTION(P0_3_SCB0_UART_TX)},
    {P2_1, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_1_SCB1_UART_TX)},
    {P2_5, &CYHAL_SCB_9, CYHAL_PIN_OUT_FUNCTION(P2_5_SCB9_UART_TX)},
    {P3_1, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P3_1_SCB2_UART_TX)},
    {P5_1, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P5_1_SCB5_UART_TX)},
    {P6_5, &CYHAL_SCB_6, CYHAL_PIN_OUT_FUNCTION(P6_5_SCB6_UART_TX)},
    {P7_1, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P7_1_SCB4_UART_TX)},
    {P8_1, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P8_1_SCB4_UART_TX)},
    {P9_1, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P9_1_SCB2_UART_TX)},
    {P10_1, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P10_1_SCB1_UART_TX)},
    {P11_1, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P11_1_SCB5_UART_TX)},
};

/* Connections for: sdhc_card_cmd */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_cmd[1] = {
    {P2_4, &CYHAL_SDHC_0, CYHAL_PIN_OUT_BUF_FUNCTION(P2_4_SDHC0_CARD_CMD)},
};

/* Connections for: sdhc_card_dat_3to0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_dat_3to0[4] = {
    {P2_0, &CYHAL_SDHC_0, CYHAL_PIN_OUT_BUF_FUNCTION(P2_0_SDHC0_CARD_DAT_3TO00)},
    {P2_1, &CYHAL_SDHC_0, CYHAL_PIN_OUT_BUF_FUNCTION(P2_1_SDHC0_CARD_DAT_3TO01)},
    {P2_2, &CYHAL_SDHC_0, CYHAL_PIN_OUT_BUF_FUNCTION(P2_2_SDHC0_CARD_DAT_3TO02)},
    {P2_3, &CYHAL_SDHC_0, CYHAL_PIN_OUT_BUF_FUNCTION(P2_3_SDHC0_CARD_DAT_3TO03)},
};

/* Connections for: sdhc_card_dat_7to4 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_dat_7to4[1] = {
    {NC, NULL, 0u},
};

/* Connections for: sdhc_card_detect_n */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_detect_n[1] = {
    {P2_6, &CYHAL_SDHC_0, CYHAL_PIN_IN_FUNCTION(P2_6_SDHC0_CARD_DETECT_N)},
};

/* Connections for: sdhc_card_emmc_reset_n */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_emmc_reset_n[1] = {
    {NC, NULL, 0u},
};

/* Connections for: sdhc_card_if_pwr_en */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_if_pwr_en[2] = {
    {P3_1, &CYHAL_SDHC_0, CYHAL_PIN_OUT_FUNCTION(P3_1_SDHC0_CARD_IF_PWR_EN)},
    {P12_6, &CYHAL_SDHC_1, CYHAL_PIN_OUT_FUNCTION(P12_6_SDHC1_CARD_IF_PWR_EN)},
};

/* Connections for: sdhc_card_mech_write_prot */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_mech_write_prot[1] = {
    {P2_7, &CYHAL_SDHC_0, CYHAL_PIN_IN_FUNCTION(P2_7_SDHC0_CARD_MECH_WRITE_PROT)},
};

/* Connections for: sdhc_clk_card */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_clk_card[1] = {
    {P2_5, &CYHAL_SDHC_0, CYHAL_PIN_OUT_BUF_FUNCTION(P2_5_SDHC0_CLK_CARD)},
};

/* Connections for: sdhc_io_volt_sel */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_io_volt_sel[2] = {
    {P3_0, &CYHAL_SDHC_0, CYHAL_PIN_OUT_FUNCTION(P3_0_SDHC0_IO_VOLT_SEL)},
    {P12_7, &CYHAL_SDHC_1, CYHAL_PIN_OUT_FUNCTION(P12_7_SDHC1_IO_VOLT_SEL)},
};

/* Connections for: sdhc_led_ctrl */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_led_ctrl[1] = {
    {NC, NULL, 0u},
};

/* Connections for: smif_spi_clk */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_clk[1] = {
    {P11_7, &CYHAL_SMIF_0, CYHAL_PIN_OUT_BUF_FUNCTION(P11_7_SMIF_SPI_CLK)},
};

/* Connections for: smif_spi_data0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data0[1] = {
    {P11_6, &CYHAL_SMIF_0, CYHAL_PIN_OUT_BUF_FUNCTION(P11_6_SMIF_SPI_DATA0)},
};

/* Connections for: smif_spi_data1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data1[1] = {
    {P11_5, &CYHAL_SMIF_0, CYHAL_PIN_OUT_BUF_FUNCTION(P11_5_SMIF_SPI_DATA1)},
};

/* Connections for: smif_spi_data2 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data2[1] = {
    {P11_4, &CYHAL_SMIF_0, CYHAL_PIN_OUT_BUF_FUNCTION(P11_4_SMIF_SPI_DATA2)},
};

/* Connections for: smif_spi_data3 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data3[1] = {
    {P11_3, &CYHAL_SMIF_0, CYHAL_PIN_OUT_BUF_FUNCTION(P11_3_SMIF_SPI_DATA3)},
};

/* Connections for: smif_spi_data4 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data4[1] = {
    {NC, NULL, 0u},
};

/* Connections for: smif_spi_data5 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data5[1] = {
    {NC, NULL, 0u},
};

/* Connections for: smif_spi_data6 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data6[1] = {
    {NC, NULL, 0u},
};

/* Connections for: smif_spi_data7 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data7[1] = {
    {NC, NULL, 0u},
};

/* Connections for: smif_spi_select0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select0[1] = {
    {P11_2, &CYHAL_SMIF_0, CYHAL_PIN_OUT_FUNCTION(P11_2_SMIF_SPI_SELECT0)},
};

/* Connections for: smif_spi_select1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select1[1] = {
    {P11_1, &CYHAL_SMIF_0, CYHAL_PIN_OUT_FUNCTION(P11_1_SMIF_SPI_SELECT1)},
};

/* Connections for: smif_spi_select2 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select2[1] = {
    {P11_0, &CYHAL_SMIF_0, CYHAL_PIN_OUT_FUNCTION(P11_0_SMIF_SPI_SELECT2)},
};

/* Connections for: smif_spi_select3 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select3[1] = {
    {NC, NULL, 0u},
};

/* Connections for: tcpwm_line */
const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line[50] = {
    {P0_0, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P0_0_TCPWM0_LINE0)},
    {P0_0, &CYHAL_TCPWM_1_0, CYHAL_PIN_OUT_FUNCTION(P0_0_TCPWM1_LINE0)},
    {P0_2, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P0_2_TCPWM0_LINE1)},
    {P0_2, &CYHAL_TCPWM_1_1, CYHAL_PIN_OUT_FUNCTION(P0_2_TCPWM1_LINE1)},
    {P0_4, &CYHAL_TCPWM_0_2, CYHAL_PIN_OUT_FUNCTION(P0_4_TCPWM0_LINE2)},
    {P0_4, &CYHAL_TCPWM_1_2, CYHAL_PIN_OUT_FUNCTION(P0_4_TCPWM1_LINE2)},
    {P2_0, &CYHAL_TCPWM_0_6, CYHAL_PIN_OUT_FUNCTION(P2_0_TCPWM0_LINE6)},
    {P2_0, &CYHAL_TCPWM_1_15, CYHAL_PIN_OUT_FUNCTION(P2_0_TCPWM1_LINE15)},
    {P2_2, &CYHAL_TCPWM_0_7, CYHAL_PIN_OUT_FUNCTION(P2_2_TCPWM0_LINE7)},
    {P2_2, &CYHAL_TCPWM_1_16, CYHAL_PIN_OUT_FUNCTION(P2_2_TCPWM1_LINE16)},
    {P2_4, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P2_4_TCPWM0_LINE0)},
    {P2_4, &CYHAL_TCPWM_1_17, CYHAL_PIN_OUT_FUNCTION(P2_4_TCPWM1_LINE17)},
    {P2_6, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P2_6_TCPWM0_LINE1)},
    {P2_6, &CYHAL_TCPWM_1_18, CYHAL_PIN_OUT_FUNCTION(P2_6_TCPWM1_LINE18)},
    {P3_0, &CYHAL_TCPWM_0_2, CYHAL_PIN_OUT_FUNCTION(P3_0_TCPWM0_LINE2)},
    {P3_0, &CYHAL_TCPWM_1_19, CYHAL_PIN_OUT_FUNCTION(P3_0_TCPWM1_LINE19)},
    {P5_0, &CYHAL_TCPWM_0_4, CYHAL_PIN_OUT_FUNCTION(P5_0_TCPWM0_LINE4)},
    {P5_0, &CYHAL_TCPWM_1_4, CYHAL_PIN_OUT_FUNCTION(P5_0_TCPWM1_LINE4)},
    {P5_6, &CYHAL_TCPWM_0_7, CYHAL_PIN_OUT_FUNCTION(P5_6_TCPWM0_LINE7)},
    {P5_6, &CYHAL_TCPWM_1_7, CYHAL_PIN_OUT_FUNCTION(P5_6_TCPWM1_LINE7)},
    {P6_2, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P6_2_TCPWM0_LINE1)},
    {P6_2, &CYHAL_TCPWM_1_9, CYHAL_PIN_OUT_FUNCTION(P6_2_TCPWM1_LINE9)},
    {P6_4, &CYHAL_TCPWM_0_2, CYHAL_PIN_OUT_FUNCTION(P6_4_TCPWM0_LINE2)},
    {P6_4, &CYHAL_TCPWM_1_10, CYHAL_PIN_OUT_FUNCTION(P6_4_TCPWM1_LINE10)},
    {P6_6, &CYHAL_TCPWM_0_3, CYHAL_PIN_OUT_FUNCTION(P6_6_TCPWM0_LINE3)},
    {P6_6, &CYHAL_TCPWM_1_11, CYHAL_PIN_OUT_FUNCTION(P6_6_TCPWM1_LINE11)},
    {P7_0, &CYHAL_TCPWM_0_4, CYHAL_PIN_OUT_FUNCTION(P7_0_TCPWM0_LINE4)},
    {P7_0, &CYHAL_TCPWM_1_12, CYHAL_PIN_OUT_FUNCTION(P7_0_TCPWM1_LINE12)},
    {P7_2, &CYHAL_TCPWM_0_5, CYHAL_PIN_OUT_FUNCTION(P7_2_TCPWM0_LINE5)},
    {P7_2, &CYHAL_TCPWM_1_13, CYHAL_PIN_OUT_FUNCTION(P7_2_TCPWM1_LINE13)},
    {P8_0, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P8_0_TCPWM0_LINE0)},
    {P8_0, &CYHAL_TCPWM_1_16, CYHAL_PIN_OUT_FUNCTION(P8_0_TCPWM1_LINE16)},
    {P9_0, &CYHAL_TCPWM_0_4, CYHAL_PIN_OUT_FUNCTION(P9_0_TCPWM0_LINE4)},
    {P9_0, &CYHAL_TCPWM_1_20, CYHAL_PIN_OUT_FUNCTION(P9_0_TCPWM1_LINE20)},
    {P9_2, &CYHAL_TCPWM_0_5, CYHAL_PIN_OUT_FUNCTION(P9_2_TCPWM0_LINE5)},
    {P9_2, &CYHAL_TCPWM_1_21, CYHAL_PIN_OUT_FUNCTION(P9_2_TCPWM1_LINE21)},
    {P10_0, &CYHAL_TCPWM_0_6, CYHAL_PIN_OUT_FUNCTION(P10_0_TCPWM0_LINE6)},
    {P10_0, &CYHAL_TCPWM_1_22, CYHAL_PIN_OUT_FUNCTION(P10_0_TCPWM1_LINE22)},
    {P10_2, &CYHAL_TCPWM_0_7, CYHAL_PIN_OUT_FUNCTION(P10_2_TCPWM0_LINE7)},
    {P10_2, &CYHAL_TCPWM_1_23, CYHAL_PIN_OUT_FUNCTION(P10_2_TCPWM1_LINE23)},
    {P10_4, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P10_4_TCPWM0_LINE0)},
    {P10_4, &CYHAL_TCPWM_1_0, CYHAL_PIN_OUT_FUNCTION(P10_4_TCPWM1_LINE0)},
    {P11_0, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P11_0_TCPWM0_LINE1)},
    {P11_0, &CYHAL_TCPWM_1_1, CYHAL_PIN_OUT_FUNCTION(P11_0_TCPWM1_LINE1)},
    {P11_2, &CYHAL_TCPWM_0_2, CYHAL_PIN_OUT_FUNCTION(P11_2_TCPWM0_LINE2)},
    {P11_2, &CYHAL_TCPWM_1_2, CYHAL_PIN_OUT_FUNCTION(P11_2_TCPWM1_LINE2)},
    {P11_4, &CYHAL_TCPWM_0_3, CYHAL_PIN_OUT_FUNCTION(P11_4_TCPWM0_LINE3)},
    {P11_4, &CYHAL_TCPWM_1_3, CYHAL_PIN_OUT_FUNCTION(P11_4_TCPWM1_LINE3)},
    {P12_6, &CYHAL_TCPWM_0_7, CYHAL_PIN_OUT_FUNCTION(P12_6_TCPWM0_LINE7)},
    {P12_6, &CYHAL_TCPWM_1_7, CYHAL_PIN_OUT_FUNCTION(P12_6_TCPWM1_LINE7)},
};

/* Connections for: tcpwm_line_compl */
const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line_compl[52] = {
    {P0_1, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P0_1_TCPWM0_LINE_COMPL0)},
    {P0_1, &CYHAL_TCPWM_1_0, CYHAL_PIN_OUT_FUNCTION(P0_1_TCPWM1_LINE_COMPL0)},
    {P0_3, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P0_3_TCPWM0_LINE_COMPL1)},
    {P0_3, &CYHAL_TCPWM_1_1, CYHAL_PIN_OUT_FUNCTION(P0_3_TCPWM1_LINE_COMPL1)},
    {P0_5, &CYHAL_TCPWM_0_2, CYHAL_PIN_OUT_FUNCTION(P0_5_TCPWM0_LINE_COMPL2)},
    {P0_5, &CYHAL_TCPWM_1_2, CYHAL_PIN_OUT_FUNCTION(P0_5_TCPWM1_LINE_COMPL2)},
    {P2_1, &CYHAL_TCPWM_0_6, CYHAL_PIN_OUT_FUNCTION(P2_1_TCPWM0_LINE_COMPL6)},
    {P2_1, &CYHAL_TCPWM_1_15, CYHAL_PIN_OUT_FUNCTION(P2_1_TCPWM1_LINE_COMPL15)},
    {P2_3, &CYHAL_TCPWM_0_7, CYHAL_PIN_OUT_FUNCTION(P2_3_TCPWM0_LINE_COMPL7)},
    {P2_3, &CYHAL_TCPWM_1_16, CYHAL_PIN_OUT_FUNCTION(P2_3_TCPWM1_LINE_COMPL16)},
    {P2_5, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P2_5_TCPWM0_LINE_COMPL0)},
    {P2_5, &CYHAL_TCPWM_1_17, CYHAL_PIN_OUT_FUNCTION(P2_5_TCPWM1_LINE_COMPL17)},
    {P2_7, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P2_7_TCPWM0_LINE_COMPL1)},
    {P2_7, &CYHAL_TCPWM_1_18, CYHAL_PIN_OUT_FUNCTION(P2_7_TCPWM1_LINE_COMPL18)},
    {P3_1, &CYHAL_TCPWM_0_2, CYHAL_PIN_OUT_FUNCTION(P3_1_TCPWM0_LINE_COMPL2)},
    {P3_1, &CYHAL_TCPWM_1_19, CYHAL_PIN_OUT_FUNCTION(P3_1_TCPWM1_LINE_COMPL19)},
    {P5_1, &CYHAL_TCPWM_0_4, CYHAL_PIN_OUT_FUNCTION(P5_1_TCPWM0_LINE_COMPL4)},
    {P5_1, &CYHAL_TCPWM_1_4, CYHAL_PIN_OUT_FUNCTION(P5_1_TCPWM1_LINE_COMPL4)},
    {P5_7, &CYHAL_TCPWM_0_7, CYHAL_PIN_OUT_FUNCTION(P5_7_TCPWM0_LINE_COMPL7)},
    {P5_7, &CYHAL_TCPWM_1_7, CYHAL_PIN_OUT_FUNCTION(P5_7_TCPWM1_LINE_COMPL7)},
    {P6_3, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P6_3_TCPWM0_LINE_COMPL1)},
    {P6_3, &CYHAL_TCPWM_1_9, CYHAL_PIN_OUT_FUNCTION(P6_3_TCPWM1_LINE_COMPL9)},
    {P6_5, &CYHAL_TCPWM_0_2, CYHAL_PIN_OUT_FUNCTION(P6_5_TCPWM0_LINE_COMPL2)},
    {P6_5, &CYHAL_TCPWM_1_10, CYHAL_PIN_OUT_FUNCTION(P6_5_TCPWM1_LINE_COMPL10)},
    {P6_7, &CYHAL_TCPWM_0_3, CYHAL_PIN_OUT_FUNCTION(P6_7_TCPWM0_LINE_COMPL3)},
    {P6_7, &CYHAL_TCPWM_1_11, CYHAL_PIN_OUT_FUNCTION(P6_7_TCPWM1_LINE_COMPL11)},
    {P7_1, &CYHAL_TCPWM_0_4, CYHAL_PIN_OUT_FUNCTION(P7_1_TCPWM0_LINE_COMPL4)},
    {P7_1, &CYHAL_TCPWM_1_12, CYHAL_PIN_OUT_FUNCTION(P7_1_TCPWM1_LINE_COMPL12)},
    {P7_3, &CYHAL_TCPWM_0_5, CYHAL_PIN_OUT_FUNCTION(P7_3_TCPWM0_LINE_COMPL5)},
    {P7_3, &CYHAL_TCPWM_1_13, CYHAL_PIN_OUT_FUNCTION(P7_3_TCPWM1_LINE_COMPL13)},
    {P7_7, &CYHAL_TCPWM_0_7, CYHAL_PIN_OUT_FUNCTION(P7_7_TCPWM0_LINE_COMPL7)},
    {P7_7, &CYHAL_TCPWM_1_15, CYHAL_PIN_OUT_FUNCTION(P7_7_TCPWM1_LINE_COMPL15)},
    {P8_1, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P8_1_TCPWM0_LINE_COMPL0)},
    {P8_1, &CYHAL_TCPWM_1_16, CYHAL_PIN_OUT_FUNCTION(P8_1_TCPWM1_LINE_COMPL16)},
    {P9_1, &CYHAL_TCPWM_0_4, CYHAL_PIN_OUT_FUNCTION(P9_1_TCPWM0_LINE_COMPL4)},
    {P9_1, &CYHAL_TCPWM_1_20, CYHAL_PIN_OUT_FUNCTION(P9_1_TCPWM1_LINE_COMPL20)},
    {P9_3, &CYHAL_TCPWM_0_5, CYHAL_PIN_OUT_FUNCTION(P9_3_TCPWM0_LINE_COMPL5)},
    {P9_3, &CYHAL_TCPWM_1_21, CYHAL_PIN_OUT_FUNCTION(P9_3_TCPWM1_LINE_COMPL21)},
    {P10_1, &CYHAL_TCPWM_0_6, CYHAL_PIN_OUT_FUNCTION(P10_1_TCPWM0_LINE_COMPL6)},
    {P10_1, &CYHAL_TCPWM_1_22, CYHAL_PIN_OUT_FUNCTION(P10_1_TCPWM1_LINE_COMPL22)},
    {P10_3, &CYHAL_TCPWM_0_7, CYHAL_PIN_OUT_FUNCTION(P10_3_TCPWM0_LINE_COMPL7)},
    {P10_3, &CYHAL_TCPWM_1_23, CYHAL_PIN_OUT_FUNCTION(P10_3_TCPWM1_LINE_COMPL23)},
    {P10_5, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P10_5_TCPWM0_LINE_COMPL0)},
    {P10_5, &CYHAL_TCPWM_1_0, CYHAL_PIN_OUT_FUNCTION(P10_5_TCPWM1_LINE_COMPL0)},
    {P11_1, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P11_1_TCPWM0_LINE_COMPL1)},
    {P11_1, &CYHAL_TCPWM_1_1, CYHAL_PIN_OUT_FUNCTION(P11_1_TCPWM1_LINE_COMPL1)},
    {P11_3, &CYHAL_TCPWM_0_2, CYHAL_PIN_OUT_FUNCTION(P11_3_TCPWM0_LINE_COMPL2)},
    {P11_3, &CYHAL_TCPWM_1_2, CYHAL_PIN_OUT_FUNCTION(P11_3_TCPWM1_LINE_COMPL2)},
    {P11_5, &CYHAL_TCPWM_0_3, CYHAL_PIN_OUT_FUNCTION(P11_5_TCPWM0_LINE_COMPL3)},
    {P11_5, &CYHAL_TCPWM_1_3, CYHAL_PIN_OUT_FUNCTION(P11_5_TCPWM1_LINE_COMPL3)},
    {P12_7, &CYHAL_TCPWM_0_7, CYHAL_PIN_OUT_FUNCTION(P12_7_TCPWM0_LINE_COMPL7)},
    {P12_7, &CYHAL_TCPWM_1_7, CYHAL_PIN_OUT_FUNCTION(P12_7_TCPWM1_LINE_COMPL7)},
};

/* Connections for: usb_usb_dm_pad */
const cyhal_resource_pin_mapping_t cyhal_pin_map_usb_usb_dm_pad[1] = {
    {USBDM, &CYHAL_USB_0, CYHAL_PIN_AUX_FUNCTION(USBDM_USB_USB_DM_PAD)},
};

/* Connections for: usb_usb_dp_pad */
const cyhal_resource_pin_mapping_t cyhal_pin_map_usb_usb_dp_pad[1] = {
    {USBDP, &CYHAL_USB_0, CYHAL_PIN_AUX_FUNCTION(USBDP_USB_USB_DP_PAD)},
};

#endif
