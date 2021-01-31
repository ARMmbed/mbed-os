/***************************************************************************//**
* \file cyhal_psoc6_03_49_wlcsp.h
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

#ifndef _CYHAL_PSOC6_03_49_WLCSP_H_
#define _CYHAL_PSOC6_03_49_WLCSP_H_

#include "cyhal_hw_resources.h"

/**
 * \addtogroup group_hal_impl_pin_package_psoc6_03_49_wlcsp PSoC6_03 49-WLCSP
 * \ingroup group_hal_impl_pin_package
 * \{
 * Pin definitions and connections specific to the PSoC6_03 49-WLCSP package.
 */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/** Gets a pin definition from the provided port and pin numbers */
#define CYHAL_GET_GPIO(port, pin)   ((((uint8_t)(port)) << 3U) + ((uint8_t)(pin)))

/** Macro that, given a gpio, will extract the pin number */
#define CYHAL_GET_PIN(pin)          ((uint8_t)(((uint8_t)pin) & 0x07U))
/** Macro that, given a gpio, will extract the port number */
#define CYHAL_GET_PORT(pin)         ((uint8_t)((((uint8_t)pin) >> 3U) & 0x1FU))

/** Definitions for all of the pins that are bonded out on in the 49-WLCSP package for the PSoC6_03 series. */
typedef enum {
    NC = 0xFF, //!< No Connect/Invalid Pin

    P0_0 = CYHAL_GET_GPIO(CYHAL_PORT_0, 0), //!< Port 0 Pin 0
    P0_1 = CYHAL_GET_GPIO(CYHAL_PORT_0, 1), //!< Port 0 Pin 1

    P2_0 = CYHAL_GET_GPIO(CYHAL_PORT_2, 0), //!< Port 2 Pin 0
    P2_1 = CYHAL_GET_GPIO(CYHAL_PORT_2, 1), //!< Port 2 Pin 1
    P2_2 = CYHAL_GET_GPIO(CYHAL_PORT_2, 2), //!< Port 2 Pin 2
    P2_3 = CYHAL_GET_GPIO(CYHAL_PORT_2, 3), //!< Port 2 Pin 3
    P2_4 = CYHAL_GET_GPIO(CYHAL_PORT_2, 4), //!< Port 2 Pin 4
    P2_5 = CYHAL_GET_GPIO(CYHAL_PORT_2, 5), //!< Port 2 Pin 5

    P5_0 = CYHAL_GET_GPIO(CYHAL_PORT_5, 0), //!< Port 5 Pin 0
    P5_1 = CYHAL_GET_GPIO(CYHAL_PORT_5, 1), //!< Port 5 Pin 1

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

    P9_0 = CYHAL_GET_GPIO(CYHAL_PORT_9, 0), //!< Port 9 Pin 0
    P9_1 = CYHAL_GET_GPIO(CYHAL_PORT_9, 1), //!< Port 9 Pin 1
    P9_2 = CYHAL_GET_GPIO(CYHAL_PORT_9, 2), //!< Port 9 Pin 2
    P9_3 = CYHAL_GET_GPIO(CYHAL_PORT_9, 3), //!< Port 9 Pin 3

    P10_0 = CYHAL_GET_GPIO(CYHAL_PORT_10, 0), //!< Port 10 Pin 0
    P10_1 = CYHAL_GET_GPIO(CYHAL_PORT_10, 1), //!< Port 10 Pin 1
    P10_2 = CYHAL_GET_GPIO(CYHAL_PORT_10, 2), //!< Port 10 Pin 2
    P10_3 = CYHAL_GET_GPIO(CYHAL_PORT_10, 3), //!< Port 10 Pin 3
    P10_4 = CYHAL_GET_GPIO(CYHAL_PORT_10, 4), //!< Port 10 Pin 4
    P10_5 = CYHAL_GET_GPIO(CYHAL_PORT_10, 5), //!< Port 10 Pin 5

    P11_2 = CYHAL_GET_GPIO(CYHAL_PORT_11, 2), //!< Port 11 Pin 2
    P11_3 = CYHAL_GET_GPIO(CYHAL_PORT_11, 3), //!< Port 11 Pin 3
    P11_4 = CYHAL_GET_GPIO(CYHAL_PORT_11, 4), //!< Port 11 Pin 4
    P11_5 = CYHAL_GET_GPIO(CYHAL_PORT_11, 5), //!< Port 11 Pin 5
    P11_6 = CYHAL_GET_GPIO(CYHAL_PORT_11, 6), //!< Port 11 Pin 6
    P11_7 = CYHAL_GET_GPIO(CYHAL_PORT_11, 7), //!< Port 11 Pin 7
} cyhal_gpio_psoc6_03_49_wlcsp_t;

/** Create generic name for the series/package specific type. */
typedef cyhal_gpio_psoc6_03_49_wlcsp_t cyhal_gpio_t;

/* Connection type definition */
/** Represents an association between a pin and a resource */
typedef struct
{
    const cyhal_resource_inst_t  *inst;      //!< The associated resource instance
    cyhal_gpio_t                 pin;        //!< The GPIO pin
    uint8_t                      drive_mode; //!< The DriveMode configuration value
    en_hsiom_sel_t               hsiom;      //!< The HSIOM configuration value
} cyhal_resource_pin_mapping_t;

/* Pin connections */
/** Indicates that a pin map exists for canfd_ttcan_rx*/
#define CYHAL_PIN_MAP_CANFD_TTCAN_RX
/** List of valid pin to peripheral connections for the canfd_ttcan_rx signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_canfd_ttcan_rx[1];
/** Indicates that a pin map exists for canfd_ttcan_tx*/
#define CYHAL_PIN_MAP_CANFD_TTCAN_TX
/** List of valid pin to peripheral connections for the canfd_ttcan_tx signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_canfd_ttcan_tx[1];
/** Indicates that a pin map exists for lpcomp_dsi_comp*/
#define CYHAL_PIN_MAP_LPCOMP_DSI_COMP
/** List of valid pin to peripheral connections for the lpcomp_dsi_comp signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_dsi_comp[1];
/** Indicates that a pin map exists for lpcomp_inn_comp*/
#define CYHAL_PIN_MAP_LPCOMP_INN_COMP
/** List of valid pin to peripheral connections for the lpcomp_inn_comp signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inn_comp[1];
/** Indicates that a pin map exists for lpcomp_inp_comp*/
#define CYHAL_PIN_MAP_LPCOMP_INP_COMP
/** List of valid pin to peripheral connections for the lpcomp_inp_comp signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inp_comp[1];
/** Indicates that a pin map exists for pass_sarmux_pads*/
#define CYHAL_PIN_MAP_PASS_SARMUX_PADS
/** List of valid pin to peripheral connections for the pass_sarmux_pads signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_pass_sarmux_pads[6];
/** Indicates that a pin map exists for scb_i2c_scl*/
#define CYHAL_PIN_MAP_SCB_I2C_SCL
/** List of valid pin to peripheral connections for the scb_i2c_scl signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_scl[6];
/** Indicates that a pin map exists for scb_i2c_sda*/
#define CYHAL_PIN_MAP_SCB_I2C_SDA
/** List of valid pin to peripheral connections for the scb_i2c_sda signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_sda[6];
/** Indicates that a pin map exists for scb_spi_m_clk*/
#define CYHAL_PIN_MAP_SCB_SPI_M_CLK
/** List of valid pin to peripheral connections for the scb_spi_m_clk signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_clk[7];
/** Indicates that a pin map exists for scb_spi_m_miso*/
#define CYHAL_PIN_MAP_SCB_SPI_M_MISO
/** List of valid pin to peripheral connections for the scb_spi_m_miso signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_miso[6];
/** Indicates that a pin map exists for scb_spi_m_mosi*/
#define CYHAL_PIN_MAP_SCB_SPI_M_MOSI
/** List of valid pin to peripheral connections for the scb_spi_m_mosi signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_mosi[6];
/** Indicates that a pin map exists for scb_spi_m_select0*/
#define CYHAL_PIN_MAP_SCB_SPI_M_SELECT0
/** List of valid pin to peripheral connections for the scb_spi_m_select0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select0[7];
/** Indicates that a pin map exists for scb_spi_m_select1*/
#define CYHAL_PIN_MAP_SCB_SPI_M_SELECT1
/** List of valid pin to peripheral connections for the scb_spi_m_select1 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select1[5];
/** Indicates that a pin map exists for scb_spi_m_select2*/
#define CYHAL_PIN_MAP_SCB_SPI_M_SELECT2
/** List of valid pin to peripheral connections for the scb_spi_m_select2 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select2[4];
/** Indicates that a pin map exists for scb_spi_m_select3*/
#define CYHAL_PIN_MAP_SCB_SPI_M_SELECT3
/** List of valid pin to peripheral connections for the scb_spi_m_select3 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select3[1];
/** Indicates that a pin map exists for scb_spi_s_clk*/
#define CYHAL_PIN_MAP_SCB_SPI_S_CLK
/** List of valid pin to peripheral connections for the scb_spi_s_clk signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_clk[7];
/** Indicates that a pin map exists for scb_spi_s_miso*/
#define CYHAL_PIN_MAP_SCB_SPI_S_MISO
/** List of valid pin to peripheral connections for the scb_spi_s_miso signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_miso[6];
/** Indicates that a pin map exists for scb_spi_s_mosi*/
#define CYHAL_PIN_MAP_SCB_SPI_S_MOSI
/** List of valid pin to peripheral connections for the scb_spi_s_mosi signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_mosi[6];
/** Indicates that a pin map exists for scb_spi_s_select0*/
#define CYHAL_PIN_MAP_SCB_SPI_S_SELECT0
/** List of valid pin to peripheral connections for the scb_spi_s_select0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select0[7];
/** Indicates that a pin map exists for scb_spi_s_select1*/
#define CYHAL_PIN_MAP_SCB_SPI_S_SELECT1
/** List of valid pin to peripheral connections for the scb_spi_s_select1 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select1[5];
/** Indicates that a pin map exists for scb_spi_s_select2*/
#define CYHAL_PIN_MAP_SCB_SPI_S_SELECT2
/** List of valid pin to peripheral connections for the scb_spi_s_select2 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select2[4];
/** Indicates that a pin map exists for scb_spi_s_select3*/
#define CYHAL_PIN_MAP_SCB_SPI_S_SELECT3
/** List of valid pin to peripheral connections for the scb_spi_s_select3 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select3[1];
/** Indicates that a pin map exists for scb_uart_cts*/
#define CYHAL_PIN_MAP_SCB_UART_CTS
/** List of valid pin to peripheral connections for the scb_uart_cts signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_cts[6];
/** Indicates that a pin map exists for scb_uart_rts*/
#define CYHAL_PIN_MAP_SCB_UART_RTS
/** List of valid pin to peripheral connections for the scb_uart_rts signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rts[6];
/** Indicates that a pin map exists for scb_uart_rx*/
#define CYHAL_PIN_MAP_SCB_UART_RX
/** List of valid pin to peripheral connections for the scb_uart_rx signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rx[5];
/** Indicates that a pin map exists for scb_uart_tx*/
#define CYHAL_PIN_MAP_SCB_UART_TX
/** List of valid pin to peripheral connections for the scb_uart_tx signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_tx[5];
/** Indicates that a pin map exists for sdhc_card_cmd*/
#define CYHAL_PIN_MAP_SDHC_CARD_CMD
/** List of valid pin to peripheral connections for the sdhc_card_cmd signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_cmd[1];
/** Indicates that a pin map exists for sdhc_card_dat_3to0*/
#define CYHAL_PIN_MAP_SDHC_CARD_DAT_3TO0
/** List of valid pin to peripheral connections for the sdhc_card_dat_3to0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_dat_3to0[4];
/** Indicates that a pin map exists for sdhc_card_detect_n*/
#define CYHAL_PIN_MAP_SDHC_CARD_DETECT_N
/** List of valid pin to peripheral connections for the sdhc_card_detect_n signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_detect_n[1];
/** Indicates that a pin map exists for sdhc_card_if_pwr_en*/
#define CYHAL_PIN_MAP_SDHC_CARD_IF_PWR_EN
/** List of valid pin to peripheral connections for the sdhc_card_if_pwr_en signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_if_pwr_en[1];
/** Indicates that a pin map exists for sdhc_card_mech_write_prot*/
#define CYHAL_PIN_MAP_SDHC_CARD_MECH_WRITE_PROT
/** List of valid pin to peripheral connections for the sdhc_card_mech_write_prot signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_card_mech_write_prot[1];
/** Indicates that a pin map exists for sdhc_clk_card*/
#define CYHAL_PIN_MAP_SDHC_CLK_CARD
/** List of valid pin to peripheral connections for the sdhc_clk_card signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_clk_card[1];
/** Indicates that a pin map exists for sdhc_io_volt_sel*/
#define CYHAL_PIN_MAP_SDHC_IO_VOLT_SEL
/** List of valid pin to peripheral connections for the sdhc_io_volt_sel signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_sdhc_io_volt_sel[1];
/** Indicates that a pin map exists for smif_spi_clk*/
#define CYHAL_PIN_MAP_SMIF_SPI_CLK
/** List of valid pin to peripheral connections for the smif_spi_clk signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_clk[1];
/** Indicates that a pin map exists for smif_spi_data0*/
#define CYHAL_PIN_MAP_SMIF_SPI_DATA0
/** List of valid pin to peripheral connections for the smif_spi_data0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data0[1];
/** Indicates that a pin map exists for smif_spi_data1*/
#define CYHAL_PIN_MAP_SMIF_SPI_DATA1
/** List of valid pin to peripheral connections for the smif_spi_data1 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data1[1];
/** Indicates that a pin map exists for smif_spi_data2*/
#define CYHAL_PIN_MAP_SMIF_SPI_DATA2
/** List of valid pin to peripheral connections for the smif_spi_data2 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data2[1];
/** Indicates that a pin map exists for smif_spi_data3*/
#define CYHAL_PIN_MAP_SMIF_SPI_DATA3
/** List of valid pin to peripheral connections for the smif_spi_data3 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data3[1];
/** Indicates that a pin map exists for smif_spi_select0*/
#define CYHAL_PIN_MAP_SMIF_SPI_SELECT0
/** List of valid pin to peripheral connections for the smif_spi_select0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select0[1];
/** Indicates that a pin map exists for smif_spi_select1*/
#define CYHAL_PIN_MAP_SMIF_SPI_SELECT1
/** List of valid pin to peripheral connections for the smif_spi_select1 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select1[1];
/** Indicates that a pin map exists for smif_spi_select2*/
#define CYHAL_PIN_MAP_SMIF_SPI_SELECT2
/** List of valid pin to peripheral connections for the smif_spi_select2 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select2[1];
/** Indicates that a pin map exists for tcpwm_line*/
#define CYHAL_PIN_MAP_TCPWM_LINE
/** List of valid pin to peripheral connections for the tcpwm_line signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line[38];
/** Indicates that a pin map exists for tcpwm_line_compl*/
#define CYHAL_PIN_MAP_TCPWM_LINE_COMPL
/** List of valid pin to peripheral connections for the tcpwm_line_compl signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line_compl[36];
/** Indicates that a pin map exists for usb_usb_dm_pad*/
#define CYHAL_PIN_MAP_USB_USB_DM_PAD
/** List of valid pin to peripheral connections for the usb_usb_dm_pad signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_usb_usb_dm_pad[1];
/** Indicates that a pin map exists for usb_usb_dp_pad*/
#define CYHAL_PIN_MAP_USB_USB_DP_PAD
/** List of valid pin to peripheral connections for the usb_usb_dp_pad signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_usb_usb_dp_pad[1];

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/** \} group_hal_impl_pin_package */

#endif /* _CYHAL_PSOC6_03_49_WLCSP_H_ */


/* [] END OF FILE */
