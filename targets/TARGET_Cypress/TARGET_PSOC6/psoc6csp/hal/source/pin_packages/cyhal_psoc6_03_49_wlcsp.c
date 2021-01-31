/***************************************************************************//**
* \file cyhal_psoc6_03_49_wlcsp.c
*
* \brief
* PSoC6_03 device GPIO HAL header for 49-WLCSP package
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

#if defined(_GPIO_PSOC6_03_49_WLCSP_H_)
#include "pin_packages/cyhal_psoc6_03_49_wlcsp.h"

/* Hardware Blocks */
static const cyhal_resource_inst_t CYHAL_CAN_0 = { CYHAL_RSC_CAN, 0, 0 };
static const cyhal_resource_inst_t CYHAL_LPCOMP_1_0 = { CYHAL_RSC_LPCOMP, 1, 0 };
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
/* Connections for: canfd_ttcan_rx */
const cyhal_resource_pin_mapping_t cyhal_pin_map_canfd_ttcan_rx[1] = {
    {&CYHAL_CAN_0, P5_0, (uint8_t)CY_GPIO_DM_HIGHZ, P5_0_CANFD0_TTCAN_RX0},
};

/* Connections for: canfd_ttcan_tx */
const cyhal_resource_pin_mapping_t cyhal_pin_map_canfd_ttcan_tx[1] = {
    {&CYHAL_CAN_0, P5_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P5_1_CANFD0_TTCAN_TX0},
};

/* Connections for: lpcomp_dsi_comp */
const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_dsi_comp[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: lpcomp_inn_comp */
const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inn_comp[1] = {
    {&CYHAL_LPCOMP_1_0, P6_3, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
};

/* Connections for: lpcomp_inp_comp */
const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inp_comp[1] = {
    {&CYHAL_LPCOMP_1_0, P6_2, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
};

/* Connections for: pass_sarmux_pads */
const cyhal_resource_pin_mapping_t cyhal_pin_map_pass_sarmux_pads[6] = {
    {&CYHAL_ADC_0, P10_0, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
    {&CYHAL_ADC_0, P10_1, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
    {&CYHAL_ADC_0, P10_2, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
    {&CYHAL_ADC_0, P10_3, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
    {&CYHAL_ADC_0, P10_4, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
    {&CYHAL_ADC_0, P10_5, (uint8_t)CY_GPIO_DM_ANALOG, HSIOM_SEL_GPIO},
};

/* Connections for: scb_i2c_scl */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_scl[6] = {
    {&CYHAL_SCB_1, P2_0, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P2_0_SCB1_I2C_SCL},
    {&CYHAL_SCB_5, P5_0, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P5_0_SCB5_I2C_SCL},
    {&CYHAL_SCB_6, P6_4, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P6_4_SCB6_I2C_SCL},
    {&CYHAL_SCB_4, P7_0, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P7_0_SCB4_I2C_SCL},
    {&CYHAL_SCB_2, P9_0, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P9_0_SCB2_I2C_SCL},
    {&CYHAL_SCB_1, P10_0, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P10_0_SCB1_I2C_SCL},
};

/* Connections for: scb_i2c_sda */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_sda[6] = {
    {&CYHAL_SCB_1, P2_1, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P2_1_SCB1_I2C_SDA},
    {&CYHAL_SCB_5, P5_1, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P5_1_SCB5_I2C_SDA},
    {&CYHAL_SCB_6, P6_5, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P6_5_SCB6_I2C_SDA},
    {&CYHAL_SCB_4, P7_1, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P7_1_SCB4_I2C_SDA},
    {&CYHAL_SCB_2, P9_1, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P9_1_SCB2_I2C_SDA},
    {&CYHAL_SCB_1, P10_1, (uint8_t)CY_GPIO_DM_OD_DRIVESLOW, P10_1_SCB1_I2C_SDA},
};

/* Connections for: scb_spi_m_clk */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_clk[7] = {
    {&CYHAL_SCB_1, P2_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_2_SCB1_SPI_CLK},
    {&CYHAL_SCB_3, P6_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_2_SCB3_SPI_CLK},
    {&CYHAL_SCB_6, P6_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_6_SCB6_SPI_CLK},
    {&CYHAL_SCB_4, P7_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_2_SCB4_SPI_CLK},
    {&CYHAL_SCB_2, P9_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_2_SCB2_SPI_CLK},
    {&CYHAL_SCB_1, P10_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_2_SCB1_SPI_CLK},
    {&CYHAL_SCB_5, P11_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_2_SCB5_SPI_CLK},
};

/* Connections for: scb_spi_m_miso */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_miso[6] = {
    {&CYHAL_SCB_1, P2_1, (uint8_t)CY_GPIO_DM_HIGHZ, P2_1_SCB1_SPI_MISO},
    {&CYHAL_SCB_5, P5_1, (uint8_t)CY_GPIO_DM_HIGHZ, P5_1_SCB5_SPI_MISO},
    {&CYHAL_SCB_6, P6_5, (uint8_t)CY_GPIO_DM_HIGHZ, P6_5_SCB6_SPI_MISO},
    {&CYHAL_SCB_4, P7_1, (uint8_t)CY_GPIO_DM_HIGHZ, P7_1_SCB4_SPI_MISO},
    {&CYHAL_SCB_2, P9_1, (uint8_t)CY_GPIO_DM_HIGHZ, P9_1_SCB2_SPI_MISO},
    {&CYHAL_SCB_1, P10_1, (uint8_t)CY_GPIO_DM_HIGHZ, P10_1_SCB1_SPI_MISO},
};

/* Connections for: scb_spi_m_mosi */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_mosi[6] = {
    {&CYHAL_SCB_1, P2_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_0_SCB1_SPI_MOSI},
    {&CYHAL_SCB_5, P5_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P5_0_SCB5_SPI_MOSI},
    {&CYHAL_SCB_6, P6_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_4_SCB6_SPI_MOSI},
    {&CYHAL_SCB_4, P7_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_0_SCB4_SPI_MOSI},
    {&CYHAL_SCB_2, P9_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_0_SCB2_SPI_MOSI},
    {&CYHAL_SCB_1, P10_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_0_SCB1_SPI_MOSI},
};

/* Connections for: scb_spi_m_select0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select0[7] = {
    {&CYHAL_SCB_1, P2_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_3_SCB1_SPI_SELECT0},
    {&CYHAL_SCB_3, P6_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_3_SCB3_SPI_SELECT0},
    {&CYHAL_SCB_6, P6_7, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_7_SCB6_SPI_SELECT0},
    {&CYHAL_SCB_4, P7_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_3_SCB4_SPI_SELECT0},
    {&CYHAL_SCB_2, P9_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_3_SCB2_SPI_SELECT0},
    {&CYHAL_SCB_1, P10_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_3_SCB1_SPI_SELECT0},
    {&CYHAL_SCB_5, P11_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_3_SCB5_SPI_SELECT0},
};

/* Connections for: scb_spi_m_select1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select1[5] = {
    {&CYHAL_SCB_0, P0_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_0_SCB0_SPI_SELECT1},
    {&CYHAL_SCB_1, P2_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_4_SCB1_SPI_SELECT1},
    {&CYHAL_SCB_4, P7_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_4_SCB4_SPI_SELECT1},
    {&CYHAL_SCB_1, P10_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_4_SCB1_SPI_SELECT1},
    {&CYHAL_SCB_5, P11_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_4_SCB5_SPI_SELECT1},
};

/* Connections for: scb_spi_m_select2 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select2[4] = {
    {&CYHAL_SCB_0, P0_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_1_SCB0_SPI_SELECT2},
    {&CYHAL_SCB_1, P2_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_5_SCB1_SPI_SELECT2},
    {&CYHAL_SCB_1, P10_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_5_SCB1_SPI_SELECT2},
    {&CYHAL_SCB_5, P11_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_5_SCB5_SPI_SELECT2},
};

/* Connections for: scb_spi_m_select3 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select3[1] = {
    {&CYHAL_SCB_5, P11_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_6_SCB5_SPI_SELECT3},
};

/* Connections for: scb_spi_s_clk */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_clk[7] = {
    {&CYHAL_SCB_1, P2_2, (uint8_t)CY_GPIO_DM_HIGHZ, P2_2_SCB1_SPI_CLK},
    {&CYHAL_SCB_3, P6_2, (uint8_t)CY_GPIO_DM_HIGHZ, P6_2_SCB3_SPI_CLK},
    {&CYHAL_SCB_6, P6_6, (uint8_t)CY_GPIO_DM_HIGHZ, P6_6_SCB6_SPI_CLK},
    {&CYHAL_SCB_4, P7_2, (uint8_t)CY_GPIO_DM_HIGHZ, P7_2_SCB4_SPI_CLK},
    {&CYHAL_SCB_2, P9_2, (uint8_t)CY_GPIO_DM_HIGHZ, P9_2_SCB2_SPI_CLK},
    {&CYHAL_SCB_1, P10_2, (uint8_t)CY_GPIO_DM_HIGHZ, P10_2_SCB1_SPI_CLK},
    {&CYHAL_SCB_5, P11_2, (uint8_t)CY_GPIO_DM_HIGHZ, P11_2_SCB5_SPI_CLK},
};

/* Connections for: scb_spi_s_miso */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_miso[6] = {
    {&CYHAL_SCB_1, P2_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_1_SCB1_SPI_MISO},
    {&CYHAL_SCB_5, P5_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P5_1_SCB5_SPI_MISO},
    {&CYHAL_SCB_6, P6_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_5_SCB6_SPI_MISO},
    {&CYHAL_SCB_4, P7_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_1_SCB4_SPI_MISO},
    {&CYHAL_SCB_2, P9_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_1_SCB2_SPI_MISO},
    {&CYHAL_SCB_1, P10_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_1_SCB1_SPI_MISO},
};

/* Connections for: scb_spi_s_mosi */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_mosi[6] = {
    {&CYHAL_SCB_1, P2_0, (uint8_t)CY_GPIO_DM_HIGHZ, P2_0_SCB1_SPI_MOSI},
    {&CYHAL_SCB_5, P5_0, (uint8_t)CY_GPIO_DM_HIGHZ, P5_0_SCB5_SPI_MOSI},
    {&CYHAL_SCB_6, P6_4, (uint8_t)CY_GPIO_DM_HIGHZ, P6_4_SCB6_SPI_MOSI},
    {&CYHAL_SCB_4, P7_0, (uint8_t)CY_GPIO_DM_HIGHZ, P7_0_SCB4_SPI_MOSI},
    {&CYHAL_SCB_2, P9_0, (uint8_t)CY_GPIO_DM_HIGHZ, P9_0_SCB2_SPI_MOSI},
    {&CYHAL_SCB_1, P10_0, (uint8_t)CY_GPIO_DM_HIGHZ, P10_0_SCB1_SPI_MOSI},
};

/* Connections for: scb_spi_s_select0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select0[7] = {
    {&CYHAL_SCB_1, P2_3, (uint8_t)CY_GPIO_DM_HIGHZ, P2_3_SCB1_SPI_SELECT0},
    {&CYHAL_SCB_3, P6_3, (uint8_t)CY_GPIO_DM_HIGHZ, P6_3_SCB3_SPI_SELECT0},
    {&CYHAL_SCB_6, P6_7, (uint8_t)CY_GPIO_DM_HIGHZ, P6_7_SCB6_SPI_SELECT0},
    {&CYHAL_SCB_4, P7_3, (uint8_t)CY_GPIO_DM_HIGHZ, P7_3_SCB4_SPI_SELECT0},
    {&CYHAL_SCB_2, P9_3, (uint8_t)CY_GPIO_DM_HIGHZ, P9_3_SCB2_SPI_SELECT0},
    {&CYHAL_SCB_1, P10_3, (uint8_t)CY_GPIO_DM_HIGHZ, P10_3_SCB1_SPI_SELECT0},
    {&CYHAL_SCB_5, P11_3, (uint8_t)CY_GPIO_DM_HIGHZ, P11_3_SCB5_SPI_SELECT0},
};

/* Connections for: scb_spi_s_select1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select1[5] = {
    {&CYHAL_SCB_0, P0_0, (uint8_t)CY_GPIO_DM_HIGHZ, P0_0_SCB0_SPI_SELECT1},
    {&CYHAL_SCB_1, P2_4, (uint8_t)CY_GPIO_DM_HIGHZ, P2_4_SCB1_SPI_SELECT1},
    {&CYHAL_SCB_4, P7_4, (uint8_t)CY_GPIO_DM_HIGHZ, P7_4_SCB4_SPI_SELECT1},
    {&CYHAL_SCB_1, P10_4, (uint8_t)CY_GPIO_DM_HIGHZ, P10_4_SCB1_SPI_SELECT1},
    {&CYHAL_SCB_5, P11_4, (uint8_t)CY_GPIO_DM_HIGHZ, P11_4_SCB5_SPI_SELECT1},
};

/* Connections for: scb_spi_s_select2 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select2[4] = {
    {&CYHAL_SCB_0, P0_1, (uint8_t)CY_GPIO_DM_HIGHZ, P0_1_SCB0_SPI_SELECT2},
    {&CYHAL_SCB_1, P2_5, (uint8_t)CY_GPIO_DM_HIGHZ, P2_5_SCB1_SPI_SELECT2},
    {&CYHAL_SCB_1, P10_5, (uint8_t)CY_GPIO_DM_HIGHZ, P10_5_SCB1_SPI_SELECT2},
    {&CYHAL_SCB_5, P11_5, (uint8_t)CY_GPIO_DM_HIGHZ, P11_5_SCB5_SPI_SELECT2},
};

/* Connections for: scb_spi_s_select3 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select3[1] = {
    {&CYHAL_SCB_5, P11_6, (uint8_t)CY_GPIO_DM_HIGHZ, P11_6_SCB5_SPI_SELECT3},
};

/* Connections for: scb_uart_cts */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_cts[6] = {
    {&CYHAL_SCB_1, P2_3, (uint8_t)CY_GPIO_DM_HIGHZ, P2_3_SCB1_UART_CTS},
    {&CYHAL_SCB_3, P6_3, (uint8_t)CY_GPIO_DM_HIGHZ, P6_3_SCB3_UART_CTS},
    {&CYHAL_SCB_4, P7_3, (uint8_t)CY_GPIO_DM_HIGHZ, P7_3_SCB4_UART_CTS},
    {&CYHAL_SCB_2, P9_3, (uint8_t)CY_GPIO_DM_HIGHZ, P9_3_SCB2_UART_CTS},
    {&CYHAL_SCB_1, P10_3, (uint8_t)CY_GPIO_DM_HIGHZ, P10_3_SCB1_UART_CTS},
    {&CYHAL_SCB_5, P11_3, (uint8_t)CY_GPIO_DM_HIGHZ, P11_3_SCB5_UART_CTS},
};

/* Connections for: scb_uart_rts */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rts[6] = {
    {&CYHAL_SCB_1, P2_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_2_SCB1_UART_RTS},
    {&CYHAL_SCB_3, P6_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_2_SCB3_UART_RTS},
    {&CYHAL_SCB_4, P7_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_2_SCB4_UART_RTS},
    {&CYHAL_SCB_2, P9_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_2_SCB2_UART_RTS},
    {&CYHAL_SCB_1, P10_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_2_SCB1_UART_RTS},
    {&CYHAL_SCB_5, P11_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_2_SCB5_UART_RTS},
};

/* Connections for: scb_uart_rx */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rx[5] = {
    {&CYHAL_SCB_1, P2_0, (uint8_t)CY_GPIO_DM_HIGHZ, P2_0_SCB1_UART_RX},
    {&CYHAL_SCB_5, P5_0, (uint8_t)CY_GPIO_DM_HIGHZ, P5_0_SCB5_UART_RX},
    {&CYHAL_SCB_4, P7_0, (uint8_t)CY_GPIO_DM_HIGHZ, P7_0_SCB4_UART_RX},
    {&CYHAL_SCB_2, P9_0, (uint8_t)CY_GPIO_DM_HIGHZ, P9_0_SCB2_UART_RX},
    {&CYHAL_SCB_1, P10_0, (uint8_t)CY_GPIO_DM_HIGHZ, P10_0_SCB1_UART_RX},
};

/* Connections for: scb_uart_tx */
const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_tx[5] = {
    {&CYHAL_SCB_1, P2_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_1_SCB1_UART_TX},
    {&CYHAL_SCB_5, P5_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P5_1_SCB5_UART_TX},
    {&CYHAL_SCB_4, P7_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_1_SCB4_UART_TX},
    {&CYHAL_SCB_2, P9_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_1_SCB2_UART_TX},
    {&CYHAL_SCB_1, P10_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_1_SCB1_UART_TX},
};

/* Connections for: sdhc_card_cmd */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_cmd[1] = {
    {&CYHAL_SDHC_0, P2_4, (uint8_t)CY_GPIO_DM_STRONG, P2_4_SDHC0_CARD_CMD},
};

/* Connections for: sdhc_card_dat_3to0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_dat_3to0[4] = {
    {&CYHAL_SDHC_0, P2_0, (uint8_t)CY_GPIO_DM_STRONG, P2_0_SDHC0_CARD_DAT_3TO00},
    {&CYHAL_SDHC_0, P2_1, (uint8_t)CY_GPIO_DM_STRONG, P2_1_SDHC0_CARD_DAT_3TO01},
    {&CYHAL_SDHC_0, P2_2, (uint8_t)CY_GPIO_DM_STRONG, P2_2_SDHC0_CARD_DAT_3TO02},
    {&CYHAL_SDHC_0, P2_3, (uint8_t)CY_GPIO_DM_STRONG, P2_3_SDHC0_CARD_DAT_3TO03},
};

/* Connections for: sdhc_card_detect_n */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_detect_n[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: sdhc_card_if_pwr_en */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_if_pwr_en[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: sdhc_card_mech_write_prot */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_mech_write_prot[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: sdhc_clk_card */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_clk_card[1] = {
    {&CYHAL_SDHC_0, P2_5, (uint8_t)CY_GPIO_DM_STRONG, P2_5_SDHC0_CLK_CARD},
};

/* Connections for: sdhc_io_volt_sel */
const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_io_volt_sel[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
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

/* Connections for: smif_spi_select0 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select0[1] = {
    {&CYHAL_SMIF_0, P11_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_2_SMIF_SPI_SELECT0},
};

/* Connections for: smif_spi_select1 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select1[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: smif_spi_select2 */
const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select2[1] = {
    {NULL, NC, (uint8_t)CY_GPIO_DM_HIGHZ, HSIOM_SEL_GPIO},
};

/* Connections for: tcpwm_line */
const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line[38] = {
    {&CYHAL_TCPWM_0_0, P0_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_0_TCPWM0_LINE0},
    {&CYHAL_TCPWM_1_0, P0_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_0_TCPWM1_LINE0},
    {&CYHAL_TCPWM_0_3, P2_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_0_TCPWM0_LINE3},
    {&CYHAL_TCPWM_1_3, P2_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_0_TCPWM1_LINE3},
    {&CYHAL_TCPWM_0_0, P2_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_2_TCPWM0_LINE0},
    {&CYHAL_TCPWM_1_4, P2_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_2_TCPWM1_LINE4},
    {&CYHAL_TCPWM_0_1, P2_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_4_TCPWM0_LINE1},
    {&CYHAL_TCPWM_1_5, P2_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_4_TCPWM1_LINE5},
    {&CYHAL_TCPWM_0_0, P5_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P5_0_TCPWM0_LINE0},
    {&CYHAL_TCPWM_1_0, P5_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P5_0_TCPWM1_LINE0},
    {&CYHAL_TCPWM_0_3, P6_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_2_TCPWM0_LINE3},
    {&CYHAL_TCPWM_1_3, P6_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_2_TCPWM1_LINE3},
    {&CYHAL_TCPWM_0_0, P6_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_4_TCPWM0_LINE0},
    {&CYHAL_TCPWM_1_4, P6_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_4_TCPWM1_LINE4},
    {&CYHAL_TCPWM_0_1, P6_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_6_TCPWM0_LINE1},
    {&CYHAL_TCPWM_1_5, P6_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_6_TCPWM1_LINE5},
    {&CYHAL_TCPWM_0_2, P7_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_0_TCPWM0_LINE2},
    {&CYHAL_TCPWM_1_6, P7_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_0_TCPWM1_LINE6},
    {&CYHAL_TCPWM_0_3, P7_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_2_TCPWM0_LINE3},
    {&CYHAL_TCPWM_1_7, P7_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_2_TCPWM1_LINE7},
    {&CYHAL_TCPWM_0_0, P7_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_4_TCPWM0_LINE0},
    {&CYHAL_TCPWM_1_0, P7_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_4_TCPWM1_LINE0},
    {&CYHAL_TCPWM_0_0, P9_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_0_TCPWM0_LINE0},
    {&CYHAL_TCPWM_1_4, P9_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_0_TCPWM1_LINE4},
    {&CYHAL_TCPWM_0_1, P9_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_2_TCPWM0_LINE1},
    {&CYHAL_TCPWM_1_5, P9_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_2_TCPWM1_LINE5},
    {&CYHAL_TCPWM_0_2, P10_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_0_TCPWM0_LINE2},
    {&CYHAL_TCPWM_1_6, P10_0, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_0_TCPWM1_LINE6},
    {&CYHAL_TCPWM_0_3, P10_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_2_TCPWM0_LINE3},
    {&CYHAL_TCPWM_1_7, P10_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_2_TCPWM1_LINE7},
    {&CYHAL_TCPWM_0_0, P10_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_4_TCPWM0_LINE0},
    {&CYHAL_TCPWM_1_0, P10_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_4_TCPWM1_LINE0},
    {&CYHAL_TCPWM_0_3, P11_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_2_TCPWM0_LINE3},
    {&CYHAL_TCPWM_1_3, P11_2, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_2_TCPWM1_LINE3},
    {&CYHAL_TCPWM_0_0, P11_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_4_TCPWM0_LINE0},
    {&CYHAL_TCPWM_1_4, P11_4, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_4_TCPWM1_LINE4},
    {&CYHAL_TCPWM_0_1, P11_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_6_TCPWM0_LINE1},
    {&CYHAL_TCPWM_1_5, P11_6, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_6_TCPWM1_LINE5},
};

/* Connections for: tcpwm_line_compl */
const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line_compl[36] = {
    {&CYHAL_TCPWM_0_0, P0_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_1_TCPWM0_LINE_COMPL0},
    {&CYHAL_TCPWM_1_0, P0_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P0_1_TCPWM1_LINE_COMPL0},
    {&CYHAL_TCPWM_0_3, P2_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_1_TCPWM0_LINE_COMPL3},
    {&CYHAL_TCPWM_1_3, P2_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_1_TCPWM1_LINE_COMPL3},
    {&CYHAL_TCPWM_0_0, P2_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_3_TCPWM0_LINE_COMPL0},
    {&CYHAL_TCPWM_1_4, P2_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_3_TCPWM1_LINE_COMPL4},
    {&CYHAL_TCPWM_0_1, P2_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_5_TCPWM0_LINE_COMPL1},
    {&CYHAL_TCPWM_1_5, P2_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P2_5_TCPWM1_LINE_COMPL5},
    {&CYHAL_TCPWM_0_0, P5_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P5_1_TCPWM0_LINE_COMPL0},
    {&CYHAL_TCPWM_1_0, P5_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P5_1_TCPWM1_LINE_COMPL0},
    {&CYHAL_TCPWM_0_3, P6_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_3_TCPWM0_LINE_COMPL3},
    {&CYHAL_TCPWM_1_3, P6_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_3_TCPWM1_LINE_COMPL3},
    {&CYHAL_TCPWM_0_0, P6_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_5_TCPWM0_LINE_COMPL0},
    {&CYHAL_TCPWM_1_4, P6_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_5_TCPWM1_LINE_COMPL4},
    {&CYHAL_TCPWM_0_1, P6_7, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_7_TCPWM0_LINE_COMPL1},
    {&CYHAL_TCPWM_1_5, P6_7, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P6_7_TCPWM1_LINE_COMPL5},
    {&CYHAL_TCPWM_0_2, P7_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_1_TCPWM0_LINE_COMPL2},
    {&CYHAL_TCPWM_1_6, P7_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_1_TCPWM1_LINE_COMPL6},
    {&CYHAL_TCPWM_0_3, P7_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_3_TCPWM0_LINE_COMPL3},
    {&CYHAL_TCPWM_1_7, P7_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P7_3_TCPWM1_LINE_COMPL7},
    {&CYHAL_TCPWM_0_0, P9_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_1_TCPWM0_LINE_COMPL0},
    {&CYHAL_TCPWM_1_4, P9_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_1_TCPWM1_LINE_COMPL4},
    {&CYHAL_TCPWM_0_1, P9_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_3_TCPWM0_LINE_COMPL1},
    {&CYHAL_TCPWM_1_5, P9_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P9_3_TCPWM1_LINE_COMPL5},
    {&CYHAL_TCPWM_0_2, P10_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_1_TCPWM0_LINE_COMPL2},
    {&CYHAL_TCPWM_1_6, P10_1, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_1_TCPWM1_LINE_COMPL6},
    {&CYHAL_TCPWM_0_3, P10_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_3_TCPWM0_LINE_COMPL3},
    {&CYHAL_TCPWM_1_7, P10_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_3_TCPWM1_LINE_COMPL7},
    {&CYHAL_TCPWM_0_0, P10_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_5_TCPWM0_LINE_COMPL0},
    {&CYHAL_TCPWM_1_0, P10_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P10_5_TCPWM1_LINE_COMPL0},
    {&CYHAL_TCPWM_0_3, P11_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_3_TCPWM0_LINE_COMPL3},
    {&CYHAL_TCPWM_1_3, P11_3, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_3_TCPWM1_LINE_COMPL3},
    {&CYHAL_TCPWM_0_0, P11_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_5_TCPWM0_LINE_COMPL0},
    {&CYHAL_TCPWM_1_4, P11_5, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_5_TCPWM1_LINE_COMPL4},
    {&CYHAL_TCPWM_0_1, P11_7, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_7_TCPWM0_LINE_COMPL1},
    {&CYHAL_TCPWM_1_5, P11_7, (uint8_t)CY_GPIO_DM_STRONG_IN_OFF, P11_7_TCPWM1_LINE_COMPL5},
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
