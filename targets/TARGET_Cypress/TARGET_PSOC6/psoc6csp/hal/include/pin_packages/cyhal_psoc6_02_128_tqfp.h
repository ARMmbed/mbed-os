/***************************************************************************//**
* \file cyhal_psoc6_02_128_tqfp.h
*
* \brief
* PSoC6_02 device GPIO HAL header for 128-TQFP package
*
* \note
* Generator version: 1.5.7254.21305
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

#ifndef _CYHAL_PSOC6_02_128_TQFP_H_
#define _CYHAL_PSOC6_02_128_TQFP_H_

#include "cyhal_hw_resources.h"

/**
 * \addtogroup group_hal_psoc6_pin_package_psoc6_02_128_tqfp PSoC6_02 128-TQFP
 * \ingroup group_hal_psoc6_pin_package
 * \{
 */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/** Gets a pin definition from the provided port and pin numbers */
#define CYHAL_GET_GPIO(port, pin) (((port) << 16) + (pin))

/** Definitions for all of the pins that are bonded out on in the 128-TQFP package for the PSoC6_02 series. */
typedef enum {
    NC = (int)0xFFFFFFFF, //!< No Connect/Invalid Pin

    P0_0 = CYHAL_GET_GPIO(CYHAL_PORT_0, 0), //!< Port 0 Pin 0
    P0_1 = CYHAL_GET_GPIO(CYHAL_PORT_0, 1), //!< Port 0 Pin 1
    P0_2 = CYHAL_GET_GPIO(CYHAL_PORT_0, 2), //!< Port 0 Pin 2
    P0_3 = CYHAL_GET_GPIO(CYHAL_PORT_0, 3), //!< Port 0 Pin 3
    P0_4 = CYHAL_GET_GPIO(CYHAL_PORT_0, 4), //!< Port 0 Pin 4
    P0_5 = CYHAL_GET_GPIO(CYHAL_PORT_0, 5), //!< Port 0 Pin 5

    P1_0 = CYHAL_GET_GPIO(CYHAL_PORT_1, 0), //!< Port 1 Pin 0
    P1_1 = CYHAL_GET_GPIO(CYHAL_PORT_1, 1), //!< Port 1 Pin 1
    P1_2 = CYHAL_GET_GPIO(CYHAL_PORT_1, 2), //!< Port 1 Pin 2
    P1_3 = CYHAL_GET_GPIO(CYHAL_PORT_1, 3), //!< Port 1 Pin 3
    P1_4 = CYHAL_GET_GPIO(CYHAL_PORT_1, 4), //!< Port 1 Pin 4
    P1_5 = CYHAL_GET_GPIO(CYHAL_PORT_1, 5), //!< Port 1 Pin 5

    P2_0 = CYHAL_GET_GPIO(CYHAL_PORT_2, 0), //!< Port 2 Pin 0
    P2_1 = CYHAL_GET_GPIO(CYHAL_PORT_2, 1), //!< Port 2 Pin 1
    P2_2 = CYHAL_GET_GPIO(CYHAL_PORT_2, 2), //!< Port 2 Pin 2
    P2_3 = CYHAL_GET_GPIO(CYHAL_PORT_2, 3), //!< Port 2 Pin 3
    P2_4 = CYHAL_GET_GPIO(CYHAL_PORT_2, 4), //!< Port 2 Pin 4
    P2_5 = CYHAL_GET_GPIO(CYHAL_PORT_2, 5), //!< Port 2 Pin 5
    P2_6 = CYHAL_GET_GPIO(CYHAL_PORT_2, 6), //!< Port 2 Pin 6
    P2_7 = CYHAL_GET_GPIO(CYHAL_PORT_2, 7), //!< Port 2 Pin 7

    P3_0 = CYHAL_GET_GPIO(CYHAL_PORT_3, 0), //!< Port 3 Pin 0
    P3_1 = CYHAL_GET_GPIO(CYHAL_PORT_3, 1), //!< Port 3 Pin 1
    P3_2 = CYHAL_GET_GPIO(CYHAL_PORT_3, 2), //!< Port 3 Pin 2
    P3_3 = CYHAL_GET_GPIO(CYHAL_PORT_3, 3), //!< Port 3 Pin 3
    P3_4 = CYHAL_GET_GPIO(CYHAL_PORT_3, 4), //!< Port 3 Pin 4
    P3_5 = CYHAL_GET_GPIO(CYHAL_PORT_3, 5), //!< Port 3 Pin 5

    P4_0 = CYHAL_GET_GPIO(CYHAL_PORT_4, 0), //!< Port 4 Pin 0
    P4_1 = CYHAL_GET_GPIO(CYHAL_PORT_4, 1), //!< Port 4 Pin 1
    P4_2 = CYHAL_GET_GPIO(CYHAL_PORT_4, 2), //!< Port 4 Pin 2
    P4_3 = CYHAL_GET_GPIO(CYHAL_PORT_4, 3), //!< Port 4 Pin 3

    P5_0 = CYHAL_GET_GPIO(CYHAL_PORT_5, 0), //!< Port 5 Pin 0
    P5_1 = CYHAL_GET_GPIO(CYHAL_PORT_5, 1), //!< Port 5 Pin 1
    P5_2 = CYHAL_GET_GPIO(CYHAL_PORT_5, 2), //!< Port 5 Pin 2
    P5_3 = CYHAL_GET_GPIO(CYHAL_PORT_5, 3), //!< Port 5 Pin 3
    P5_4 = CYHAL_GET_GPIO(CYHAL_PORT_5, 4), //!< Port 5 Pin 4
    P5_5 = CYHAL_GET_GPIO(CYHAL_PORT_5, 5), //!< Port 5 Pin 5
    P5_6 = CYHAL_GET_GPIO(CYHAL_PORT_5, 6), //!< Port 5 Pin 6
    P5_7 = CYHAL_GET_GPIO(CYHAL_PORT_5, 7), //!< Port 5 Pin 7

    P6_0 = CYHAL_GET_GPIO(CYHAL_PORT_6, 0), //!< Port 6 Pin 0
    P6_1 = CYHAL_GET_GPIO(CYHAL_PORT_6, 1), //!< Port 6 Pin 1
    P6_2 = CYHAL_GET_GPIO(CYHAL_PORT_6, 2), //!< Port 6 Pin 2
    P6_3 = CYHAL_GET_GPIO(CYHAL_PORT_6, 3), //!< Port 6 Pin 3
    P6_4 = CYHAL_GET_GPIO(CYHAL_PORT_6, 4), //!< Port 6 Pin 4
    P6_5 = CYHAL_GET_GPIO(CYHAL_PORT_6, 5), //!< Port 6 Pin 5
    P6_6 = CYHAL_GET_GPIO(CYHAL_PORT_6, 6), //!< Port 6 Pin 6
    P6_7 = CYHAL_GET_GPIO(CYHAL_PORT_6, 7), //!< Port 6 Pin 7

    P7_0 = CYHAL_GET_GPIO(CYHAL_PORT_7, 0), //!< Port 7 Pin 0
    P7_1 = CYHAL_GET_GPIO(CYHAL_PORT_7, 1), //!< Port 7 Pin 1
    P7_2 = CYHAL_GET_GPIO(CYHAL_PORT_7, 2), //!< Port 7 Pin 2
    P7_3 = CYHAL_GET_GPIO(CYHAL_PORT_7, 3), //!< Port 7 Pin 3
    P7_4 = CYHAL_GET_GPIO(CYHAL_PORT_7, 4), //!< Port 7 Pin 4
    P7_5 = CYHAL_GET_GPIO(CYHAL_PORT_7, 5), //!< Port 7 Pin 5
    P7_6 = CYHAL_GET_GPIO(CYHAL_PORT_7, 6), //!< Port 7 Pin 6
    P7_7 = CYHAL_GET_GPIO(CYHAL_PORT_7, 7), //!< Port 7 Pin 7

    P8_0 = CYHAL_GET_GPIO(CYHAL_PORT_8, 0), //!< Port 8 Pin 0
    P8_1 = CYHAL_GET_GPIO(CYHAL_PORT_8, 1), //!< Port 8 Pin 1
    P8_2 = CYHAL_GET_GPIO(CYHAL_PORT_8, 2), //!< Port 8 Pin 2
    P8_3 = CYHAL_GET_GPIO(CYHAL_PORT_8, 3), //!< Port 8 Pin 3
    P8_4 = CYHAL_GET_GPIO(CYHAL_PORT_8, 4), //!< Port 8 Pin 4
    P8_5 = CYHAL_GET_GPIO(CYHAL_PORT_8, 5), //!< Port 8 Pin 5
    P8_6 = CYHAL_GET_GPIO(CYHAL_PORT_8, 6), //!< Port 8 Pin 6
    P8_7 = CYHAL_GET_GPIO(CYHAL_PORT_8, 7), //!< Port 8 Pin 7

    P9_0 = CYHAL_GET_GPIO(CYHAL_PORT_9, 0), //!< Port 9 Pin 0
    P9_1 = CYHAL_GET_GPIO(CYHAL_PORT_9, 1), //!< Port 9 Pin 1
    P9_2 = CYHAL_GET_GPIO(CYHAL_PORT_9, 2), //!< Port 9 Pin 2
    P9_3 = CYHAL_GET_GPIO(CYHAL_PORT_9, 3), //!< Port 9 Pin 3
    P9_4 = CYHAL_GET_GPIO(CYHAL_PORT_9, 4), //!< Port 9 Pin 4
    P9_5 = CYHAL_GET_GPIO(CYHAL_PORT_9, 5), //!< Port 9 Pin 5
    P9_6 = CYHAL_GET_GPIO(CYHAL_PORT_9, 6), //!< Port 9 Pin 6
    P9_7 = CYHAL_GET_GPIO(CYHAL_PORT_9, 7), //!< Port 9 Pin 7

    P10_0 = CYHAL_GET_GPIO(CYHAL_PORT_10, 0), //!< Port 10 Pin 0
    P10_1 = CYHAL_GET_GPIO(CYHAL_PORT_10, 1), //!< Port 10 Pin 1
    P10_2 = CYHAL_GET_GPIO(CYHAL_PORT_10, 2), //!< Port 10 Pin 2
    P10_3 = CYHAL_GET_GPIO(CYHAL_PORT_10, 3), //!< Port 10 Pin 3
    P10_4 = CYHAL_GET_GPIO(CYHAL_PORT_10, 4), //!< Port 10 Pin 4
    P10_5 = CYHAL_GET_GPIO(CYHAL_PORT_10, 5), //!< Port 10 Pin 5
    P10_6 = CYHAL_GET_GPIO(CYHAL_PORT_10, 6), //!< Port 10 Pin 6
    P10_7 = CYHAL_GET_GPIO(CYHAL_PORT_10, 7), //!< Port 10 Pin 7

    P11_0 = CYHAL_GET_GPIO(CYHAL_PORT_11, 0), //!< Port 11 Pin 0
    P11_1 = CYHAL_GET_GPIO(CYHAL_PORT_11, 1), //!< Port 11 Pin 1
    P11_2 = CYHAL_GET_GPIO(CYHAL_PORT_11, 2), //!< Port 11 Pin 2
    P11_3 = CYHAL_GET_GPIO(CYHAL_PORT_11, 3), //!< Port 11 Pin 3
    P11_4 = CYHAL_GET_GPIO(CYHAL_PORT_11, 4), //!< Port 11 Pin 4
    P11_5 = CYHAL_GET_GPIO(CYHAL_PORT_11, 5), //!< Port 11 Pin 5
    P11_6 = CYHAL_GET_GPIO(CYHAL_PORT_11, 6), //!< Port 11 Pin 6
    P11_7 = CYHAL_GET_GPIO(CYHAL_PORT_11, 7), //!< Port 11 Pin 7

    P12_0 = CYHAL_GET_GPIO(CYHAL_PORT_12, 0), //!< Port 12 Pin 0
    P12_1 = CYHAL_GET_GPIO(CYHAL_PORT_12, 1), //!< Port 12 Pin 1
    P12_2 = CYHAL_GET_GPIO(CYHAL_PORT_12, 2), //!< Port 12 Pin 2
    P12_3 = CYHAL_GET_GPIO(CYHAL_PORT_12, 3), //!< Port 12 Pin 3
    P12_4 = CYHAL_GET_GPIO(CYHAL_PORT_12, 4), //!< Port 12 Pin 4
    P12_5 = CYHAL_GET_GPIO(CYHAL_PORT_12, 5), //!< Port 12 Pin 5
    P12_6 = CYHAL_GET_GPIO(CYHAL_PORT_12, 6), //!< Port 12 Pin 6
    P12_7 = CYHAL_GET_GPIO(CYHAL_PORT_12, 7), //!< Port 12 Pin 7

    P13_0 = CYHAL_GET_GPIO(CYHAL_PORT_13, 0), //!< Port 13 Pin 0
    P13_1 = CYHAL_GET_GPIO(CYHAL_PORT_13, 1), //!< Port 13 Pin 1
    P13_2 = CYHAL_GET_GPIO(CYHAL_PORT_13, 2), //!< Port 13 Pin 2
    P13_3 = CYHAL_GET_GPIO(CYHAL_PORT_13, 3), //!< Port 13 Pin 3
    P13_4 = CYHAL_GET_GPIO(CYHAL_PORT_13, 4), //!< Port 13 Pin 4
    P13_5 = CYHAL_GET_GPIO(CYHAL_PORT_13, 5), //!< Port 13 Pin 5
    P13_6 = CYHAL_GET_GPIO(CYHAL_PORT_13, 6), //!< Port 13 Pin 6
    P13_7 = CYHAL_GET_GPIO(CYHAL_PORT_13, 7), //!< Port 13 Pin 7

    USBDP = CYHAL_GET_GPIO(CYHAL_PORT_14, 0), //!< Port 14 Pin 0
    USBDM = CYHAL_GET_GPIO(CYHAL_PORT_14, 1), //!< Port 14 Pin 1
} cyhal_gpio_psoc6_02_128_tqfp_t;

/** Create generic name for the series/package specific type. */
typedef cyhal_gpio_psoc6_02_128_tqfp_t cyhal_gpio_t;

/* Connection type definition */
/** Represents an association between a pin and a resource */
typedef struct
{
    cyhal_gpio_t                  pin;  //!< The GPIO pin
    const cyhal_resource_inst_t  *inst; //!< The associated resource instance
    cyhal_gpio_mapping_cfg_t      cfg;  //!< The DriveMode and HSIOM configuration value
} cyhal_resource_pin_mapping_t;

/* Pin connections */
/** List of valid pin to peripheral connections for the audioss_clk_i2s_if signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_clk_i2s_if[4];
/** List of valid pin to peripheral connections for the audioss_pdm_clk signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_pdm_clk[2];
/** List of valid pin to peripheral connections for the audioss_pdm_data signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_pdm_data[2];
/** List of valid pin to peripheral connections for the audioss_rx_sck signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_sck[4];
/** List of valid pin to peripheral connections for the audioss_rx_sdi signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_sdi[4];
/** List of valid pin to peripheral connections for the audioss_rx_ws signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_ws[4];
/** List of valid pin to peripheral connections for the audioss_tx_sck signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_sck[4];
/** List of valid pin to peripheral connections for the audioss_tx_sdo signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_sdo[4];
/** List of valid pin to peripheral connections for the audioss_tx_ws signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_ws[4];
/** List of valid pin to peripheral connections for the lpcomp_dsi_comp0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_dsi_comp0[1];
/** List of valid pin to peripheral connections for the lpcomp_dsi_comp1 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_dsi_comp1[1];
/** List of valid pin to peripheral connections for the lpcomp_inn_comp0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inn_comp0[1];
/** List of valid pin to peripheral connections for the lpcomp_inn_comp1 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inn_comp1[1];
/** List of valid pin to peripheral connections for the lpcomp_inp_comp0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inp_comp0[1];
/** List of valid pin to peripheral connections for the lpcomp_inp_comp1 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inp_comp1[1];
/** List of valid pin to peripheral connections for the pass_sarmux_pads signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_pass_sarmux_pads[8];
/** List of valid pin to peripheral connections for the scb_i2c_scl signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_scl[21];
/** List of valid pin to peripheral connections for the scb_i2c_sda signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_sda[21];
/** List of valid pin to peripheral connections for the scb_spi_m_clk signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_clk[17];
/** List of valid pin to peripheral connections for the scb_spi_m_miso signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_miso[17];
/** List of valid pin to peripheral connections for the scb_spi_m_mosi signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_mosi[17];
/** List of valid pin to peripheral connections for the scb_spi_m_select0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select0[17];
/** List of valid pin to peripheral connections for the scb_spi_m_select1 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select1[13];
/** List of valid pin to peripheral connections for the scb_spi_m_select2 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select2[13];
/** List of valid pin to peripheral connections for the scb_spi_m_select3 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select3[10];
/** List of valid pin to peripheral connections for the scb_spi_s_clk signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_clk[17];
/** List of valid pin to peripheral connections for the scb_spi_s_miso signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_miso[17];
/** List of valid pin to peripheral connections for the scb_spi_s_mosi signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_mosi[17];
/** List of valid pin to peripheral connections for the scb_spi_s_select0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select0[17];
/** List of valid pin to peripheral connections for the scb_spi_s_select1 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select1[13];
/** List of valid pin to peripheral connections for the scb_spi_s_select2 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select2[13];
/** List of valid pin to peripheral connections for the scb_spi_s_select3 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select3[10];
/** List of valid pin to peripheral connections for the scb_uart_cts signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_cts[19];
/** List of valid pin to peripheral connections for the scb_uart_rts signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rts[19];
/** List of valid pin to peripheral connections for the scb_uart_rx signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rx[19];
/** List of valid pin to peripheral connections for the scb_uart_tx signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_tx[19];
/** List of valid pin to peripheral connections for the sdhc_card_cmd signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_cmd[2];
/** List of valid pin to peripheral connections for the sdhc_card_dat_3to0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_dat_3to0[8];
/** List of valid pin to peripheral connections for the sdhc_card_dat_7to4 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_dat_7to4[4];
/** List of valid pin to peripheral connections for the sdhc_card_detect_n signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_detect_n[2];
/** List of valid pin to peripheral connections for the sdhc_card_emmc_reset_n signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_emmc_reset_n[1];
/** List of valid pin to peripheral connections for the sdhc_card_if_pwr_en signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_if_pwr_en[2];
/** List of valid pin to peripheral connections for the sdhc_card_mech_write_prot signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_mech_write_prot[2];
/** List of valid pin to peripheral connections for the sdhc_clk_card signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_clk_card[2];
/** List of valid pin to peripheral connections for the sdhc_io_volt_sel signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_io_volt_sel[2];
/** List of valid pin to peripheral connections for the sdhc_led_ctrl signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_led_ctrl[1];
/** List of valid pin to peripheral connections for the smif_spi_clk signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_clk[1];
/** List of valid pin to peripheral connections for the smif_spi_data0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data0[1];
/** List of valid pin to peripheral connections for the smif_spi_data1 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data1[1];
/** List of valid pin to peripheral connections for the smif_spi_data2 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data2[1];
/** List of valid pin to peripheral connections for the smif_spi_data3 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data3[1];
/** List of valid pin to peripheral connections for the smif_spi_data4 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data4[1];
/** List of valid pin to peripheral connections for the smif_spi_data5 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data5[1];
/** List of valid pin to peripheral connections for the smif_spi_data6 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data6[1];
/** List of valid pin to peripheral connections for the smif_spi_data7 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data7[1];
/** List of valid pin to peripheral connections for the smif_spi_select0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select0[1];
/** List of valid pin to peripheral connections for the smif_spi_select1 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select1[1];
/** List of valid pin to peripheral connections for the smif_spi_select2 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select2[1];
/** List of valid pin to peripheral connections for the smif_spi_select3 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select3[1];
/** List of valid pin to peripheral connections for the tcpwm_line signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line[100];
/** List of valid pin to peripheral connections for the tcpwm_line_compl signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line_compl[100];
/** List of valid pin to peripheral connections for the usb_usb_dm_pad signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_usb_usb_dm_pad[1];
/** List of valid pin to peripheral connections for the usb_usb_dp_pad signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_usb_usb_dp_pad[1];

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/** \} group_hal_psoc6 */

#endif /* _CYHAL_PSOC6_02_128_TQFP_H_ */


/* [] END OF FILE */
