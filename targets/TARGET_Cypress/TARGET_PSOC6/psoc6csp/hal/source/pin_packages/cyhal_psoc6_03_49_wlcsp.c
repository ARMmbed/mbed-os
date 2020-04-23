/***************************************************************************//**
* \file cyhal_psoc6_03_49_wlcsp.c
*
* \brief
* PSoC6_03 device GPIO HAL header for 49-WLCSP package
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

#if defined(_GPIO_PSOC6_03_49_WLCSP_H_)
#include "pin_packages/cyhal_psoc6_03_49_wlcsp.h"

/* Hardware Blocks */
static const cyhal_resource_inst_t CYHAL_CAN_0 = { CYHAL_RSC_CAN, 0, 0 };
static const cyhal_resource_inst_t CYHAL_LPCOMP_0_0 = { CYHAL_RSC_LPCOMP, 0, 0 };
static const cyhal_resource_inst_t CYHAL_ADC_0 = { CYHAL_RSC_ADC, 0, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_0 = { CYHAL_RSC_SCB, 0, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_1 = { CYHAL_RSC_SCB, 1, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_2 = { CYHAL_RSC_SCB, 2, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_3 = { CYHAL_RSC_SCB, 3, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_4 = { CYHAL_RSC_SCB, 4, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_5 = { CYHAL_RSC_SCB, 5, 0 };
static const cyhal_resource_inst_t CYHAL_SCB_6 = { CYHAL_RSC_SCB, 6, 0 };
static const cyhal_resource_inst_t CYHAL_SDHC_0 = { CYHAL_RSC_SDHC, 0, 0 };
static const cyhal_resource_inst_t CYHAL_SMIF_0 = { CYHAL_RSC_SMIF, 0, 0 };
static const cyhal_resource_inst_t CYHAL_TCPWM_0_0 = { CYHAL_RSC_TCPWM, 0, 0 };
static const cyhal_resource_inst_t CYHAL_TCPWM_0_1 = { CYHAL_RSC_TCPWM, 0, 1 };
static const cyhal_resource_inst_t CYHAL_TCPWM_0_2 = { CYHAL_RSC_TCPWM, 0, 2 };
static const cyhal_resource_inst_t CYHAL_TCPWM_0_3 = { CYHAL_RSC_TCPWM, 0, 3 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_0 = { CYHAL_RSC_TCPWM, 1, 0 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_3 = { CYHAL_RSC_TCPWM, 1, 3 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_4 = { CYHAL_RSC_TCPWM, 1, 4 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_5 = { CYHAL_RSC_TCPWM, 1, 5 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_6 = { CYHAL_RSC_TCPWM, 1, 6 };
static const cyhal_resource_inst_t CYHAL_TCPWM_1_7 = { CYHAL_RSC_TCPWM, 1, 7 };

/* Pin connections */
/* Connections for: can_ttcan_rx */
const cyhal_resource_pin_mapping_t cyhal_pin_map_can_ttcan_rx[1] = {
    {P5_0, &CYHAL_CAN_0, CYHAL_PIN_IN_FUNCTION(P5_0_CANFD0_TTCAN_RX0)},
};

/* Connections for: can_ttcan_tx */
const cyhal_resource_pin_mapping_t cyhal_pin_map_can_ttcan_tx[1] = {
    {P5_1, &CYHAL_CAN_0, CYHAL_PIN_OUT_FUNCTION(P5_1_CANFD0_TTCAN_TX0)},
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
    {NC, NULL, 0u},
};

/* Connections for: lpcomp_inn_comp1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inn_comp1[1] = {
    {P6_3, &CYHAL_LPCOMP_0_0, CYHAL_PIN_ANALOG_FUNCTION(P6_3_LPCOMP_INN_COMP1)},
};

/* Connections for: lpcomp_inp_comp0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inp_comp0[1] = {
    {NC, NULL, 0u},
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
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_scl[6] = {
    {P2_0, &CYHAL_SCB_1, CYHAL_PIN_OD_FUNCTION(P2_0_SCB1_I2C_SCL)},
    {P5_0, &CYHAL_SCB_5, CYHAL_PIN_OD_FUNCTION(P5_0_SCB5_I2C_SCL)},
    {P6_4, &CYHAL_SCB_6, CYHAL_PIN_OD_FUNCTION(P6_4_SCB6_I2C_SCL)},
    {P7_0, &CYHAL_SCB_4, CYHAL_PIN_OD_FUNCTION(P7_0_SCB4_I2C_SCL)},
    {P9_0, &CYHAL_SCB_2, CYHAL_PIN_OD_FUNCTION(P9_0_SCB2_I2C_SCL)},
    {P10_0, &CYHAL_SCB_1, CYHAL_PIN_OD_FUNCTION(P10_0_SCB1_I2C_SCL)},
};

/* Connections for: scb_i2c_sda */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_sda[6] = {
    {P2_1, &CYHAL_SCB_1, CYHAL_PIN_OD_FUNCTION(P2_1_SCB1_I2C_SDA)},
    {P5_1, &CYHAL_SCB_5, CYHAL_PIN_OD_FUNCTION(P5_1_SCB5_I2C_SDA)},
    {P6_5, &CYHAL_SCB_6, CYHAL_PIN_OD_FUNCTION(P6_5_SCB6_I2C_SDA)},
    {P7_1, &CYHAL_SCB_4, CYHAL_PIN_OD_FUNCTION(P7_1_SCB4_I2C_SDA)},
    {P9_1, &CYHAL_SCB_2, CYHAL_PIN_OD_FUNCTION(P9_1_SCB2_I2C_SDA)},
    {P10_1, &CYHAL_SCB_1, CYHAL_PIN_OD_FUNCTION(P10_1_SCB1_I2C_SDA)},
};

/* Connections for: scb_spi_m_clk */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_clk[7] = {
    {P2_2, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_2_SCB1_SPI_CLK)},
    {P6_2, &CYHAL_SCB_3, CYHAL_PIN_OUT_FUNCTION(P6_2_SCB3_SPI_CLK)},
    {P6_6, &CYHAL_SCB_6, CYHAL_PIN_OUT_FUNCTION(P6_6_SCB6_SPI_CLK)},
    {P7_2, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P7_2_SCB4_SPI_CLK)},
    {P9_2, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P9_2_SCB2_SPI_CLK)},
    {P10_2, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P10_2_SCB1_SPI_CLK)},
    {P11_2, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P11_2_SCB5_SPI_CLK)},
};

/* Connections for: scb_spi_m_miso */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_miso[6] = {
    {P2_1, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_1_SCB1_SPI_MISO)},
    {P5_1, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P5_1_SCB5_SPI_MISO)},
    {P6_5, &CYHAL_SCB_6, CYHAL_PIN_IN_FUNCTION(P6_5_SCB6_SPI_MISO)},
    {P7_1, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P7_1_SCB4_SPI_MISO)},
    {P9_1, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P9_1_SCB2_SPI_MISO)},
    {P10_1, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P10_1_SCB1_SPI_MISO)},
};

/* Connections for: scb_spi_m_mosi */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_mosi[6] = {
    {P2_0, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_0_SCB1_SPI_MOSI)},
    {P5_0, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P5_0_SCB5_SPI_MOSI)},
    {P6_4, &CYHAL_SCB_6, CYHAL_PIN_OUT_FUNCTION(P6_4_SCB6_SPI_MOSI)},
    {P7_0, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P7_0_SCB4_SPI_MOSI)},
    {P9_0, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P9_0_SCB2_SPI_MOSI)},
    {P10_0, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P10_0_SCB1_SPI_MOSI)},
};

/* Connections for: scb_spi_m_select0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select0[7] = {
    {P2_3, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_3_SCB1_SPI_SELECT0)},
    {P6_3, &CYHAL_SCB_3, CYHAL_PIN_OUT_FUNCTION(P6_3_SCB3_SPI_SELECT0)},
    {P6_7, &CYHAL_SCB_6, CYHAL_PIN_OUT_FUNCTION(P6_7_SCB6_SPI_SELECT0)},
    {P7_3, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P7_3_SCB4_SPI_SELECT0)},
    {P9_3, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P9_3_SCB2_SPI_SELECT0)},
    {P10_3, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P10_3_SCB1_SPI_SELECT0)},
    {P11_3, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P11_3_SCB5_SPI_SELECT0)},
};

/* Connections for: scb_spi_m_select1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select1[5] = {
    {P0_0, &CYHAL_SCB_0, CYHAL_PIN_OUT_FUNCTION(P0_0_SCB0_SPI_SELECT1)},
    {P2_4, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_4_SCB1_SPI_SELECT1)},
    {P7_4, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P7_4_SCB4_SPI_SELECT1)},
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
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select3[1] = {
    {P11_6, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P11_6_SCB5_SPI_SELECT3)},
};

/* Connections for: scb_spi_s_clk */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_clk[7] = {
    {P2_2, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_2_SCB1_SPI_CLK)},
    {P6_2, &CYHAL_SCB_3, CYHAL_PIN_IN_FUNCTION(P6_2_SCB3_SPI_CLK)},
    {P6_6, &CYHAL_SCB_6, CYHAL_PIN_IN_FUNCTION(P6_6_SCB6_SPI_CLK)},
    {P7_2, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P7_2_SCB4_SPI_CLK)},
    {P9_2, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P9_2_SCB2_SPI_CLK)},
    {P10_2, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P10_2_SCB1_SPI_CLK)},
    {P11_2, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P11_2_SCB5_SPI_CLK)},
};

/* Connections for: scb_spi_s_miso */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_miso[6] = {
    {P2_1, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_1_SCB1_SPI_MISO)},
    {P5_1, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P5_1_SCB5_SPI_MISO)},
    {P6_5, &CYHAL_SCB_6, CYHAL_PIN_OUT_FUNCTION(P6_5_SCB6_SPI_MISO)},
    {P7_1, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P7_1_SCB4_SPI_MISO)},
    {P9_1, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P9_1_SCB2_SPI_MISO)},
    {P10_1, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P10_1_SCB1_SPI_MISO)},
};

/* Connections for: scb_spi_s_mosi */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_mosi[6] = {
    {P2_0, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_0_SCB1_SPI_MOSI)},
    {P5_0, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P5_0_SCB5_SPI_MOSI)},
    {P6_4, &CYHAL_SCB_6, CYHAL_PIN_IN_FUNCTION(P6_4_SCB6_SPI_MOSI)},
    {P7_0, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P7_0_SCB4_SPI_MOSI)},
    {P9_0, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P9_0_SCB2_SPI_MOSI)},
    {P10_0, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P10_0_SCB1_SPI_MOSI)},
};

/* Connections for: scb_spi_s_select0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select0[7] = {
    {P2_3, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_3_SCB1_SPI_SELECT0)},
    {P6_3, &CYHAL_SCB_3, CYHAL_PIN_IN_FUNCTION(P6_3_SCB3_SPI_SELECT0)},
    {P6_7, &CYHAL_SCB_6, CYHAL_PIN_IN_FUNCTION(P6_7_SCB6_SPI_SELECT0)},
    {P7_3, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P7_3_SCB4_SPI_SELECT0)},
    {P9_3, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P9_3_SCB2_SPI_SELECT0)},
    {P10_3, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P10_3_SCB1_SPI_SELECT0)},
    {P11_3, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P11_3_SCB5_SPI_SELECT0)},
};

/* Connections for: scb_spi_s_select1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select1[5] = {
    {P0_0, &CYHAL_SCB_0, CYHAL_PIN_IN_FUNCTION(P0_0_SCB0_SPI_SELECT1)},
    {P2_4, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_4_SCB1_SPI_SELECT1)},
    {P7_4, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P7_4_SCB4_SPI_SELECT1)},
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
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select3[1] = {
    {P11_6, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P11_6_SCB5_SPI_SELECT3)},
};

/* Connections for: scb_uart_cts */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_cts[6] = {
    {P2_3, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_3_SCB1_UART_CTS)},
    {P6_3, &CYHAL_SCB_3, CYHAL_PIN_IN_FUNCTION(P6_3_SCB3_UART_CTS)},
    {P7_3, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P7_3_SCB4_UART_CTS)},
    {P9_3, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P9_3_SCB2_UART_CTS)},
    {P10_3, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P10_3_SCB1_UART_CTS)},
    {P11_3, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P11_3_SCB5_UART_CTS)},
};

/* Connections for: scb_uart_rts */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rts[6] = {
    {P2_2, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_2_SCB1_UART_RTS)},
    {P6_2, &CYHAL_SCB_3, CYHAL_PIN_OUT_FUNCTION(P6_2_SCB3_UART_RTS)},
    {P7_2, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P7_2_SCB4_UART_RTS)},
    {P9_2, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P9_2_SCB2_UART_RTS)},
    {P10_2, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P10_2_SCB1_UART_RTS)},
    {P11_2, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P11_2_SCB5_UART_RTS)},
};

/* Connections for: scb_uart_rx */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rx[5] = {
    {P2_0, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P2_0_SCB1_UART_RX)},
    {P5_0, &CYHAL_SCB_5, CYHAL_PIN_IN_FUNCTION(P5_0_SCB5_UART_RX)},
    {P7_0, &CYHAL_SCB_4, CYHAL_PIN_IN_FUNCTION(P7_0_SCB4_UART_RX)},
    {P9_0, &CYHAL_SCB_2, CYHAL_PIN_IN_FUNCTION(P9_0_SCB2_UART_RX)},
    {P10_0, &CYHAL_SCB_1, CYHAL_PIN_IN_FUNCTION(P10_0_SCB1_UART_RX)},
};

/* Connections for: scb_uart_tx */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_tx[5] = {
    {P2_1, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P2_1_SCB1_UART_TX)},
    {P5_1, &CYHAL_SCB_5, CYHAL_PIN_OUT_FUNCTION(P5_1_SCB5_UART_TX)},
    {P7_1, &CYHAL_SCB_4, CYHAL_PIN_OUT_FUNCTION(P7_1_SCB4_UART_TX)},
    {P9_1, &CYHAL_SCB_2, CYHAL_PIN_OUT_FUNCTION(P9_1_SCB2_UART_TX)},
    {P10_1, &CYHAL_SCB_1, CYHAL_PIN_OUT_FUNCTION(P10_1_SCB1_UART_TX)},
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

/* Connections for: sdhc_card_detect_n */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_detect_n[1] = {
    {NC, NULL, 0u},
};

/* Connections for: sdhc_card_if_pwr_en */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_if_pwr_en[1] = {
    {NC, NULL, 0u},
};

/* Connections for: sdhc_card_mech_write_prot */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_mech_write_prot[1] = {
    {NC, NULL, 0u},
};

/* Connections for: sdhc_clk_card */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_clk_card[1] = {
    {P2_5, &CYHAL_SDHC_0, CYHAL_PIN_OUT_BUF_FUNCTION(P2_5_SDHC0_CLK_CARD)},
};

/* Connections for: sdhc_io_volt_sel */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_io_volt_sel[1] = {
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

/* Connections for: smif_spi_select0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select0[1] = {
    {P11_2, &CYHAL_SMIF_0, CYHAL_PIN_OUT_FUNCTION(P11_2_SMIF_SPI_SELECT0)},
};

/* Connections for: smif_spi_select1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select1[1] = {
    {NC, NULL, 0u},
};

/* Connections for: smif_spi_select2 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select2[1] = {
    {NC, NULL, 0u},
};

/* Connections for: tcpwm_line */
const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line[38] = {
    {P0_0, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P0_0_TCPWM0_LINE0)},
    {P0_0, &CYHAL_TCPWM_1_0, CYHAL_PIN_OUT_FUNCTION(P0_0_TCPWM1_LINE0)},
    {P2_0, &CYHAL_TCPWM_0_3, CYHAL_PIN_OUT_FUNCTION(P2_0_TCPWM0_LINE3)},
    {P2_0, &CYHAL_TCPWM_1_3, CYHAL_PIN_OUT_FUNCTION(P2_0_TCPWM1_LINE3)},
    {P2_2, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P2_2_TCPWM0_LINE0)},
    {P2_2, &CYHAL_TCPWM_1_4, CYHAL_PIN_OUT_FUNCTION(P2_2_TCPWM1_LINE4)},
    {P2_4, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P2_4_TCPWM0_LINE1)},
    {P2_4, &CYHAL_TCPWM_1_5, CYHAL_PIN_OUT_FUNCTION(P2_4_TCPWM1_LINE5)},
    {P5_0, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P5_0_TCPWM0_LINE0)},
    {P5_0, &CYHAL_TCPWM_1_0, CYHAL_PIN_OUT_FUNCTION(P5_0_TCPWM1_LINE0)},
    {P6_2, &CYHAL_TCPWM_0_3, CYHAL_PIN_OUT_FUNCTION(P6_2_TCPWM0_LINE3)},
    {P6_2, &CYHAL_TCPWM_1_3, CYHAL_PIN_OUT_FUNCTION(P6_2_TCPWM1_LINE3)},
    {P6_4, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P6_4_TCPWM0_LINE0)},
    {P6_4, &CYHAL_TCPWM_1_4, CYHAL_PIN_OUT_FUNCTION(P6_4_TCPWM1_LINE4)},
    {P6_6, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P6_6_TCPWM0_LINE1)},
    {P6_6, &CYHAL_TCPWM_1_5, CYHAL_PIN_OUT_FUNCTION(P6_6_TCPWM1_LINE5)},
    {P7_0, &CYHAL_TCPWM_0_2, CYHAL_PIN_OUT_FUNCTION(P7_0_TCPWM0_LINE2)},
    {P7_0, &CYHAL_TCPWM_1_6, CYHAL_PIN_OUT_FUNCTION(P7_0_TCPWM1_LINE6)},
    {P7_2, &CYHAL_TCPWM_0_3, CYHAL_PIN_OUT_FUNCTION(P7_2_TCPWM0_LINE3)},
    {P7_2, &CYHAL_TCPWM_1_7, CYHAL_PIN_OUT_FUNCTION(P7_2_TCPWM1_LINE7)},
    {P7_4, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P7_4_TCPWM0_LINE0)},
    {P7_4, &CYHAL_TCPWM_1_0, CYHAL_PIN_OUT_FUNCTION(P7_4_TCPWM1_LINE0)},
    {P9_0, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P9_0_TCPWM0_LINE0)},
    {P9_0, &CYHAL_TCPWM_1_4, CYHAL_PIN_OUT_FUNCTION(P9_0_TCPWM1_LINE4)},
    {P9_2, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P9_2_TCPWM0_LINE1)},
    {P9_2, &CYHAL_TCPWM_1_5, CYHAL_PIN_OUT_FUNCTION(P9_2_TCPWM1_LINE5)},
    {P10_0, &CYHAL_TCPWM_0_2, CYHAL_PIN_OUT_FUNCTION(P10_0_TCPWM0_LINE2)},
    {P10_0, &CYHAL_TCPWM_1_6, CYHAL_PIN_OUT_FUNCTION(P10_0_TCPWM1_LINE6)},
    {P10_2, &CYHAL_TCPWM_0_3, CYHAL_PIN_OUT_FUNCTION(P10_2_TCPWM0_LINE3)},
    {P10_2, &CYHAL_TCPWM_1_7, CYHAL_PIN_OUT_FUNCTION(P10_2_TCPWM1_LINE7)},
    {P10_4, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P10_4_TCPWM0_LINE0)},
    {P10_4, &CYHAL_TCPWM_1_0, CYHAL_PIN_OUT_FUNCTION(P10_4_TCPWM1_LINE0)},
    {P11_2, &CYHAL_TCPWM_0_3, CYHAL_PIN_OUT_FUNCTION(P11_2_TCPWM0_LINE3)},
    {P11_2, &CYHAL_TCPWM_1_3, CYHAL_PIN_OUT_FUNCTION(P11_2_TCPWM1_LINE3)},
    {P11_4, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P11_4_TCPWM0_LINE0)},
    {P11_4, &CYHAL_TCPWM_1_4, CYHAL_PIN_OUT_FUNCTION(P11_4_TCPWM1_LINE4)},
    {P11_6, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P11_6_TCPWM0_LINE1)},
    {P11_6, &CYHAL_TCPWM_1_5, CYHAL_PIN_OUT_FUNCTION(P11_6_TCPWM1_LINE5)},
};

/* Connections for: tcpwm_line_compl */
const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line_compl[36] = {
    {P0_1, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P0_1_TCPWM0_LINE_COMPL0)},
    {P0_1, &CYHAL_TCPWM_1_0, CYHAL_PIN_OUT_FUNCTION(P0_1_TCPWM1_LINE_COMPL0)},
    {P2_1, &CYHAL_TCPWM_0_3, CYHAL_PIN_OUT_FUNCTION(P2_1_TCPWM0_LINE_COMPL3)},
    {P2_1, &CYHAL_TCPWM_1_3, CYHAL_PIN_OUT_FUNCTION(P2_1_TCPWM1_LINE_COMPL3)},
    {P2_3, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P2_3_TCPWM0_LINE_COMPL0)},
    {P2_3, &CYHAL_TCPWM_1_4, CYHAL_PIN_OUT_FUNCTION(P2_3_TCPWM1_LINE_COMPL4)},
    {P2_5, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P2_5_TCPWM0_LINE_COMPL1)},
    {P2_5, &CYHAL_TCPWM_1_5, CYHAL_PIN_OUT_FUNCTION(P2_5_TCPWM1_LINE_COMPL5)},
    {P5_1, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P5_1_TCPWM0_LINE_COMPL0)},
    {P5_1, &CYHAL_TCPWM_1_0, CYHAL_PIN_OUT_FUNCTION(P5_1_TCPWM1_LINE_COMPL0)},
    {P6_3, &CYHAL_TCPWM_0_3, CYHAL_PIN_OUT_FUNCTION(P6_3_TCPWM0_LINE_COMPL3)},
    {P6_3, &CYHAL_TCPWM_1_3, CYHAL_PIN_OUT_FUNCTION(P6_3_TCPWM1_LINE_COMPL3)},
    {P6_5, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P6_5_TCPWM0_LINE_COMPL0)},
    {P6_5, &CYHAL_TCPWM_1_4, CYHAL_PIN_OUT_FUNCTION(P6_5_TCPWM1_LINE_COMPL4)},
    {P6_7, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P6_7_TCPWM0_LINE_COMPL1)},
    {P6_7, &CYHAL_TCPWM_1_5, CYHAL_PIN_OUT_FUNCTION(P6_7_TCPWM1_LINE_COMPL5)},
    {P7_1, &CYHAL_TCPWM_0_2, CYHAL_PIN_OUT_FUNCTION(P7_1_TCPWM0_LINE_COMPL2)},
    {P7_1, &CYHAL_TCPWM_1_6, CYHAL_PIN_OUT_FUNCTION(P7_1_TCPWM1_LINE_COMPL6)},
    {P7_3, &CYHAL_TCPWM_0_3, CYHAL_PIN_OUT_FUNCTION(P7_3_TCPWM0_LINE_COMPL3)},
    {P7_3, &CYHAL_TCPWM_1_7, CYHAL_PIN_OUT_FUNCTION(P7_3_TCPWM1_LINE_COMPL7)},
    {P9_1, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P9_1_TCPWM0_LINE_COMPL0)},
    {P9_1, &CYHAL_TCPWM_1_4, CYHAL_PIN_OUT_FUNCTION(P9_1_TCPWM1_LINE_COMPL4)},
    {P9_3, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P9_3_TCPWM0_LINE_COMPL1)},
    {P9_3, &CYHAL_TCPWM_1_5, CYHAL_PIN_OUT_FUNCTION(P9_3_TCPWM1_LINE_COMPL5)},
    {P10_1, &CYHAL_TCPWM_0_2, CYHAL_PIN_OUT_FUNCTION(P10_1_TCPWM0_LINE_COMPL2)},
    {P10_1, &CYHAL_TCPWM_1_6, CYHAL_PIN_OUT_FUNCTION(P10_1_TCPWM1_LINE_COMPL6)},
    {P10_3, &CYHAL_TCPWM_0_3, CYHAL_PIN_OUT_FUNCTION(P10_3_TCPWM0_LINE_COMPL3)},
    {P10_3, &CYHAL_TCPWM_1_7, CYHAL_PIN_OUT_FUNCTION(P10_3_TCPWM1_LINE_COMPL7)},
    {P10_5, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P10_5_TCPWM0_LINE_COMPL0)},
    {P10_5, &CYHAL_TCPWM_1_0, CYHAL_PIN_OUT_FUNCTION(P10_5_TCPWM1_LINE_COMPL0)},
    {P11_3, &CYHAL_TCPWM_0_3, CYHAL_PIN_OUT_FUNCTION(P11_3_TCPWM0_LINE_COMPL3)},
    {P11_3, &CYHAL_TCPWM_1_3, CYHAL_PIN_OUT_FUNCTION(P11_3_TCPWM1_LINE_COMPL3)},
    {P11_5, &CYHAL_TCPWM_0_0, CYHAL_PIN_OUT_FUNCTION(P11_5_TCPWM0_LINE_COMPL0)},
    {P11_5, &CYHAL_TCPWM_1_4, CYHAL_PIN_OUT_FUNCTION(P11_5_TCPWM1_LINE_COMPL4)},
    {P11_7, &CYHAL_TCPWM_0_1, CYHAL_PIN_OUT_FUNCTION(P11_7_TCPWM0_LINE_COMPL1)},
    {P11_7, &CYHAL_TCPWM_1_5, CYHAL_PIN_OUT_FUNCTION(P11_7_TCPWM1_LINE_COMPL5)},
};

/* Connections for: usb_usb_dm_pad */
const cyhal_resource_pin_mapping_t cyhal_pin_map_usb_usb_dm_pad[1] = {
    {NC, NULL, 0u},
};

/* Connections for: usb_usb_dp_pad */
const cyhal_resource_pin_mapping_t cyhal_pin_map_usb_usb_dp_pad[1] = {
    {NC, NULL, 0u},
};

#endif
