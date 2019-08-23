/***************************************************************************//**
* \file cyhal_psoc6_02_68_qfn.h
*
* \brief
* PSoC6_02 device GPIO HAL header for 68-QFN package
*
* \note
* Generator version: 1.4.7153.30079
*
********************************************************************************
* \copyright
* Copyright 2016-2019 Cypress Semiconductor Corporation
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

#ifndef _CYHAL_PSOC6_02_68_QFN_H_
#define _CYHAL_PSOC6_02_68_QFN_H_

#include "cyhal_hw_resources.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#define CYHAL_GET_GPIO(port, pin) (((port) << 16) + (pin))

/* Pin names */
typedef enum {
    NC = (int)0xFFFFFFFF,

    P0_0 = CYHAL_GET_GPIO(CYHAL_PORT_0, 0),
    P0_1 = CYHAL_GET_GPIO(CYHAL_PORT_0, 1),
    P0_2 = CYHAL_GET_GPIO(CYHAL_PORT_0, 2),
    P0_3 = CYHAL_GET_GPIO(CYHAL_PORT_0, 3),
    P0_4 = CYHAL_GET_GPIO(CYHAL_PORT_0, 4),
    P0_5 = CYHAL_GET_GPIO(CYHAL_PORT_0, 5),

    P2_0 = CYHAL_GET_GPIO(CYHAL_PORT_2, 0),
    P2_1 = CYHAL_GET_GPIO(CYHAL_PORT_2, 1),
    P2_2 = CYHAL_GET_GPIO(CYHAL_PORT_2, 2),
    P2_3 = CYHAL_GET_GPIO(CYHAL_PORT_2, 3),
    P2_4 = CYHAL_GET_GPIO(CYHAL_PORT_2, 4),
    P2_5 = CYHAL_GET_GPIO(CYHAL_PORT_2, 5),
    P2_6 = CYHAL_GET_GPIO(CYHAL_PORT_2, 6),
    P2_7 = CYHAL_GET_GPIO(CYHAL_PORT_2, 7),

    P3_0 = CYHAL_GET_GPIO(CYHAL_PORT_3, 0),
    P3_1 = CYHAL_GET_GPIO(CYHAL_PORT_3, 1),

    P5_0 = CYHAL_GET_GPIO(CYHAL_PORT_5, 0),
    P5_1 = CYHAL_GET_GPIO(CYHAL_PORT_5, 1),
    P5_6 = CYHAL_GET_GPIO(CYHAL_PORT_5, 6),
    P5_7 = CYHAL_GET_GPIO(CYHAL_PORT_5, 7),

    P6_2 = CYHAL_GET_GPIO(CYHAL_PORT_6, 2),
    P6_3 = CYHAL_GET_GPIO(CYHAL_PORT_6, 3),
    P6_4 = CYHAL_GET_GPIO(CYHAL_PORT_6, 4),
    P6_5 = CYHAL_GET_GPIO(CYHAL_PORT_6, 5),
    P6_6 = CYHAL_GET_GPIO(CYHAL_PORT_6, 6),
    P6_7 = CYHAL_GET_GPIO(CYHAL_PORT_6, 7),

    P7_0 = CYHAL_GET_GPIO(CYHAL_PORT_7, 0),
    P7_1 = CYHAL_GET_GPIO(CYHAL_PORT_7, 1),
    P7_2 = CYHAL_GET_GPIO(CYHAL_PORT_7, 2),
    P7_3 = CYHAL_GET_GPIO(CYHAL_PORT_7, 3),
    P7_7 = CYHAL_GET_GPIO(CYHAL_PORT_7, 7),

    P8_0 = CYHAL_GET_GPIO(CYHAL_PORT_8, 0),
    P8_1 = CYHAL_GET_GPIO(CYHAL_PORT_8, 1),

    P9_0 = CYHAL_GET_GPIO(CYHAL_PORT_9, 0),
    P9_1 = CYHAL_GET_GPIO(CYHAL_PORT_9, 1),
    P9_2 = CYHAL_GET_GPIO(CYHAL_PORT_9, 2),
    P9_3 = CYHAL_GET_GPIO(CYHAL_PORT_9, 3),

    P10_0 = CYHAL_GET_GPIO(CYHAL_PORT_10, 0),
    P10_1 = CYHAL_GET_GPIO(CYHAL_PORT_10, 1),
    P10_2 = CYHAL_GET_GPIO(CYHAL_PORT_10, 2),
    P10_3 = CYHAL_GET_GPIO(CYHAL_PORT_10, 3),
    P10_4 = CYHAL_GET_GPIO(CYHAL_PORT_10, 4),
    P10_5 = CYHAL_GET_GPIO(CYHAL_PORT_10, 5),

    P11_0 = CYHAL_GET_GPIO(CYHAL_PORT_11, 0),
    P11_1 = CYHAL_GET_GPIO(CYHAL_PORT_11, 1),
    P11_2 = CYHAL_GET_GPIO(CYHAL_PORT_11, 2),
    P11_3 = CYHAL_GET_GPIO(CYHAL_PORT_11, 3),
    P11_4 = CYHAL_GET_GPIO(CYHAL_PORT_11, 4),
    P11_5 = CYHAL_GET_GPIO(CYHAL_PORT_11, 5),
    P11_6 = CYHAL_GET_GPIO(CYHAL_PORT_11, 6),
    P11_7 = CYHAL_GET_GPIO(CYHAL_PORT_11, 7),

    P12_6 = CYHAL_GET_GPIO(CYHAL_PORT_12, 6),
    P12_7 = CYHAL_GET_GPIO(CYHAL_PORT_12, 7),

    USBDP = CYHAL_GET_GPIO(CYHAL_PORT_14, 0),
    USBDM = CYHAL_GET_GPIO(CYHAL_PORT_14, 1),
} cyhal_gpio_t;

/* Connection type definition */
/** Represents an association between a pin and a resource */
typedef struct
{
    cyhal_gpio_t                  pin;  //!< The GPIO pin
    const cyhal_resource_inst_t  *inst; //!< The associated resource instance
    cyhal_gpio_mapping_cfg_t      cfg;  //!< The DriveMode and HSIOM configuration value
} cyhal_resource_pin_mapping_t;

/* Pin connections */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_clk_i2s_if[3];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_pdm_clk[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_pdm_data[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_sck[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_sdi[2];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_ws[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_sck[3];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_sdo[2];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_ws[2];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_dsi_comp0[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_dsi_comp1[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inn_comp0[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inn_comp1[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inp_comp0[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inp_comp1[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_pass_sarmux_pads[6];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_scl[12];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_sda[12];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_clk[10];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_miso[11];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_mosi[11];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select0[10];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select1[5];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select2[4];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select3[5];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_clk[10];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_miso[11];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_mosi[11];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select0[10];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select1[5];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select2[4];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select3[5];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_cts[10];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rts[10];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rx[11];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_tx[11];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_cmd[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_dat_3to0[4];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_dat_7to4[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_detect_n[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_emmc_reset_n[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_if_pwr_en[2];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_mech_write_prot[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_clk_card[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_io_volt_sel[2];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_led_ctrl[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_clk[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data0[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data1[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data2[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data3[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data4[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data5[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data6[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data7[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select0[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select1[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select2[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select3[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line[50];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line_compl[52];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_usb_usb_dm_pad[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_usb_usb_dp_pad[1];

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _CYHAL_PSOC6_02_68_QFN_H_ */


/* [] END OF FILE */
