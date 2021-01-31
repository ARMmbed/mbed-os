/***************************************************************************//**
* \file cyhal_psoc6_01_104_m_csp_ble_usb.h
*
* \brief
* PSoC6_01 device GPIO HAL header for 104-M-CSP-BLE-USB package
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

#ifndef _CYHAL_PSOC6_01_104_M_CSP_BLE_USB_H_
#define _CYHAL_PSOC6_01_104_M_CSP_BLE_USB_H_

#include "cyhal_hw_resources.h"

/**
 * \addtogroup group_hal_impl_pin_package_psoc6_01_104_m_csp_ble_usb PSoC6_01 104-M-CSP-BLE-USB
 * \ingroup group_hal_impl_pin_package
 * \{
 * Pin definitions and connections specific to the PSoC6_01 104-M-CSP-BLE-USB package.
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

/** Definitions for all of the pins that are bonded out on in the 104-M-CSP-BLE-USB package for the PSoC6_01 series. */
typedef enum {
    NC = 0xFF, //!< No Connect/Invalid Pin

    P0_0 = CYHAL_GET_GPIO(CYHAL_PORT_0, 0), //!< Port 0 Pin 0
    P0_1 = CYHAL_GET_GPIO(CYHAL_PORT_0, 1), //!< Port 0 Pin 1
    P0_2 = CYHAL_GET_GPIO(CYHAL_PORT_0, 2), //!< Port 0 Pin 2
    P0_3 = CYHAL_GET_GPIO(CYHAL_PORT_0, 3), //!< Port 0 Pin 3
    P0_4 = CYHAL_GET_GPIO(CYHAL_PORT_0, 4), //!< Port 0 Pin 4
    P0_5 = CYHAL_GET_GPIO(CYHAL_PORT_0, 5), //!< Port 0 Pin 5

    P1_0 = CYHAL_GET_GPIO(CYHAL_PORT_1, 0), //!< Port 1 Pin 0
    P1_1 = CYHAL_GET_GPIO(CYHAL_PORT_1, 1), //!< Port 1 Pin 1
    P1_4 = CYHAL_GET_GPIO(CYHAL_PORT_1, 4), //!< Port 1 Pin 4
    P1_5 = CYHAL_GET_GPIO(CYHAL_PORT_1, 5), //!< Port 1 Pin 5

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

    P13_0 = CYHAL_GET_GPIO(CYHAL_PORT_13, 0), //!< Port 13 Pin 0
    P13_1 = CYHAL_GET_GPIO(CYHAL_PORT_13, 1), //!< Port 13 Pin 1

    USBDP = CYHAL_GET_GPIO(CYHAL_PORT_14, 0), //!< Port 14 Pin 0
    USBDM = CYHAL_GET_GPIO(CYHAL_PORT_14, 1), //!< Port 14 Pin 1
} cyhal_gpio_psoc6_01_104_m_csp_ble_usb_t;

/** Create generic name for the series/package specific type. */
typedef cyhal_gpio_psoc6_01_104_m_csp_ble_usb_t cyhal_gpio_t;

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
/** Indicates that a pin map exists for audioss_clk_i2s_if*/
#define CYHAL_PIN_MAP_AUDIOSS_CLK_I2S_IF
/** List of valid pin to peripheral connections for the audioss_clk_i2s_if signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_clk_i2s_if[1];
/** Indicates that a pin map exists for audioss_pdm_clk*/
#define CYHAL_PIN_MAP_AUDIOSS_PDM_CLK
/** List of valid pin to peripheral connections for the audioss_pdm_clk signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_pdm_clk[2];
/** Indicates that a pin map exists for audioss_pdm_data*/
#define CYHAL_PIN_MAP_AUDIOSS_PDM_DATA
/** List of valid pin to peripheral connections for the audioss_pdm_data signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_pdm_data[1];
/** Indicates that a pin map exists for audioss_rx_sck*/
#define CYHAL_PIN_MAP_AUDIOSS_RX_SCK
/** List of valid pin to peripheral connections for the audioss_rx_sck signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_sck[1];
/** Indicates that a pin map exists for audioss_rx_sdi*/
#define CYHAL_PIN_MAP_AUDIOSS_RX_SDI
/** List of valid pin to peripheral connections for the audioss_rx_sdi signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_sdi[1];
/** Indicates that a pin map exists for audioss_rx_ws*/
#define CYHAL_PIN_MAP_AUDIOSS_RX_WS
/** List of valid pin to peripheral connections for the audioss_rx_ws signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_ws[1];
/** Indicates that a pin map exists for audioss_tx_sck*/
#define CYHAL_PIN_MAP_AUDIOSS_TX_SCK
/** List of valid pin to peripheral connections for the audioss_tx_sck signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_sck[1];
/** Indicates that a pin map exists for audioss_tx_sdo*/
#define CYHAL_PIN_MAP_AUDIOSS_TX_SDO
/** List of valid pin to peripheral connections for the audioss_tx_sdo signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_sdo[1];
/** Indicates that a pin map exists for audioss_tx_ws*/
#define CYHAL_PIN_MAP_AUDIOSS_TX_WS
/** List of valid pin to peripheral connections for the audioss_tx_ws signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_ws[1];
/** Indicates that a pin map exists for bless_ext_lna_rx_ctl_out*/
#define CYHAL_PIN_MAP_BLESS_EXT_LNA_RX_CTL_OUT
/** List of valid pin to peripheral connections for the bless_ext_lna_rx_ctl_out signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_ext_lna_rx_ctl_out[1];
/** Indicates that a pin map exists for bless_ext_pa_lna_chip_en_out*/
#define CYHAL_PIN_MAP_BLESS_EXT_PA_LNA_CHIP_EN_OUT
/** List of valid pin to peripheral connections for the bless_ext_pa_lna_chip_en_out signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_ext_pa_lna_chip_en_out[1];
/** Indicates that a pin map exists for bless_ext_pa_tx_ctl_out*/
#define CYHAL_PIN_MAP_BLESS_EXT_PA_TX_CTL_OUT
/** List of valid pin to peripheral connections for the bless_ext_pa_tx_ctl_out signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_ext_pa_tx_ctl_out[1];
/** Indicates that a pin map exists for bless_mxd_act_bpktctl*/
#define CYHAL_PIN_MAP_BLESS_MXD_ACT_BPKTCTL
/** List of valid pin to peripheral connections for the bless_mxd_act_bpktctl signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_act_bpktctl[1];
/** Indicates that a pin map exists for bless_mxd_act_dbus_rx_en*/
#define CYHAL_PIN_MAP_BLESS_MXD_ACT_DBUS_RX_EN
/** List of valid pin to peripheral connections for the bless_mxd_act_dbus_rx_en signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_act_dbus_rx_en[1];
/** Indicates that a pin map exists for bless_mxd_act_dbus_tx_en*/
#define CYHAL_PIN_MAP_BLESS_MXD_ACT_DBUS_TX_EN
/** List of valid pin to peripheral connections for the bless_mxd_act_dbus_tx_en signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_act_dbus_tx_en[1];
/** Indicates that a pin map exists for bless_mxd_act_txd_rxd*/
#define CYHAL_PIN_MAP_BLESS_MXD_ACT_TXD_RXD
/** List of valid pin to peripheral connections for the bless_mxd_act_txd_rxd signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_act_txd_rxd[1];
/** Indicates that a pin map exists for bless_mxd_dpslp_act_ldo_en*/
#define CYHAL_PIN_MAP_BLESS_MXD_DPSLP_ACT_LDO_EN
/** List of valid pin to peripheral connections for the bless_mxd_dpslp_act_ldo_en signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_act_ldo_en[1];
/** Indicates that a pin map exists for bless_mxd_dpslp_buck_en*/
#define CYHAL_PIN_MAP_BLESS_MXD_DPSLP_BUCK_EN
/** List of valid pin to peripheral connections for the bless_mxd_dpslp_buck_en signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_buck_en[1];
/** Indicates that a pin map exists for bless_mxd_dpslp_clk_en*/
#define CYHAL_PIN_MAP_BLESS_MXD_DPSLP_CLK_EN
/** List of valid pin to peripheral connections for the bless_mxd_dpslp_clk_en signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_clk_en[1];
/** Indicates that a pin map exists for bless_mxd_dpslp_dig_ldo_en*/
#define CYHAL_PIN_MAP_BLESS_MXD_DPSLP_DIG_LDO_EN
/** List of valid pin to peripheral connections for the bless_mxd_dpslp_dig_ldo_en signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_dig_ldo_en[1];
/** Indicates that a pin map exists for bless_mxd_dpslp_isolate_n*/
#define CYHAL_PIN_MAP_BLESS_MXD_DPSLP_ISOLATE_N
/** List of valid pin to peripheral connections for the bless_mxd_dpslp_isolate_n signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_isolate_n[1];
/** Indicates that a pin map exists for bless_mxd_dpslp_mxd_clk_out*/
#define CYHAL_PIN_MAP_BLESS_MXD_DPSLP_MXD_CLK_OUT
/** List of valid pin to peripheral connections for the bless_mxd_dpslp_mxd_clk_out signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_mxd_clk_out[1];
/** Indicates that a pin map exists for bless_mxd_dpslp_rcb_clk*/
#define CYHAL_PIN_MAP_BLESS_MXD_DPSLP_RCB_CLK
/** List of valid pin to peripheral connections for the bless_mxd_dpslp_rcb_clk signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_rcb_clk[1];
/** Indicates that a pin map exists for bless_mxd_dpslp_rcb_data*/
#define CYHAL_PIN_MAP_BLESS_MXD_DPSLP_RCB_DATA
/** List of valid pin to peripheral connections for the bless_mxd_dpslp_rcb_data signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_rcb_data[1];
/** Indicates that a pin map exists for bless_mxd_dpslp_rcb_le*/
#define CYHAL_PIN_MAP_BLESS_MXD_DPSLP_RCB_LE
/** List of valid pin to peripheral connections for the bless_mxd_dpslp_rcb_le signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_rcb_le[1];
/** Indicates that a pin map exists for bless_mxd_dpslp_reset_n*/
#define CYHAL_PIN_MAP_BLESS_MXD_DPSLP_RESET_N
/** List of valid pin to peripheral connections for the bless_mxd_dpslp_reset_n signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_reset_n[1];
/** Indicates that a pin map exists for bless_mxd_dpslp_ret_ldo_ol_hv*/
#define CYHAL_PIN_MAP_BLESS_MXD_DPSLP_RET_LDO_OL_HV
/** List of valid pin to peripheral connections for the bless_mxd_dpslp_ret_ldo_ol_hv signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_ret_ldo_ol_hv[1];
/** Indicates that a pin map exists for bless_mxd_dpslp_ret_switch_hv*/
#define CYHAL_PIN_MAP_BLESS_MXD_DPSLP_RET_SWITCH_HV
/** List of valid pin to peripheral connections for the bless_mxd_dpslp_ret_switch_hv signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_ret_switch_hv[1];
/** Indicates that a pin map exists for bless_mxd_dpslp_xtal_en*/
#define CYHAL_PIN_MAP_BLESS_MXD_DPSLP_XTAL_EN
/** List of valid pin to peripheral connections for the bless_mxd_dpslp_xtal_en signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_xtal_en[1];
/** Indicates that a pin map exists for dac_ctdac_voutsw*/
#define CYHAL_PIN_MAP_DAC_CTDAC_VOUTSW
/** List of valid pin to peripheral connections for the dac_ctdac_voutsw signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_dac_ctdac_voutsw[1];
/** Indicates that a pin map exists for lpcomp_dsi_comp*/
#define CYHAL_PIN_MAP_LPCOMP_DSI_COMP
/** List of valid pin to peripheral connections for the lpcomp_dsi_comp signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_dsi_comp[2];
/** Indicates that a pin map exists for lpcomp_inn_comp*/
#define CYHAL_PIN_MAP_LPCOMP_INN_COMP
/** List of valid pin to peripheral connections for the lpcomp_inn_comp signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inn_comp[2];
/** Indicates that a pin map exists for lpcomp_inp_comp*/
#define CYHAL_PIN_MAP_LPCOMP_INP_COMP
/** List of valid pin to peripheral connections for the lpcomp_inp_comp signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_inp_comp[2];
/** Indicates that a pin map exists for opamp_dsi_ctb_cmp*/
#define CYHAL_PIN_MAP_OPAMP_DSI_CTB_CMP
/** List of valid pin to peripheral connections for the opamp_dsi_ctb_cmp signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_opamp_dsi_ctb_cmp[2];
/** Indicates that a pin map exists for opamp_out_10x*/
#define CYHAL_PIN_MAP_OPAMP_OUT_10X
/** List of valid pin to peripheral connections for the opamp_out_10x signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_opamp_out_10x[2];
/** Indicates that a pin map exists for opamp_vin_m*/
#define CYHAL_PIN_MAP_OPAMP_VIN_M
/** List of valid pin to peripheral connections for the opamp_vin_m signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_opamp_vin_m[1];
/** Indicates that a pin map exists for opamp_vin_p0*/
#define CYHAL_PIN_MAP_OPAMP_VIN_P0
/** List of valid pin to peripheral connections for the opamp_vin_p0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_opamp_vin_p0[1];
/** Indicates that a pin map exists for opamp_vin_p1*/
#define CYHAL_PIN_MAP_OPAMP_VIN_P1
/** List of valid pin to peripheral connections for the opamp_vin_p1 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_opamp_vin_p1[1];
/** Indicates that a pin map exists for pass_sarmux_pads*/
#define CYHAL_PIN_MAP_PASS_SARMUX_PADS
/** List of valid pin to peripheral connections for the pass_sarmux_pads signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_pass_sarmux_pads[8];
/** Indicates that a pin map exists for scb_i2c_scl*/
#define CYHAL_PIN_MAP_SCB_I2C_SCL
/** List of valid pin to peripheral connections for the scb_i2c_scl signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_scl[14];
/** Indicates that a pin map exists for scb_i2c_sda*/
#define CYHAL_PIN_MAP_SCB_I2C_SDA
/** List of valid pin to peripheral connections for the scb_i2c_sda signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_sda[14];
/** Indicates that a pin map exists for scb_spi_m_clk*/
#define CYHAL_PIN_MAP_SCB_SPI_M_CLK
/** List of valid pin to peripheral connections for the scb_spi_m_clk signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_clk[12];
/** Indicates that a pin map exists for scb_spi_m_miso*/
#define CYHAL_PIN_MAP_SCB_SPI_M_MISO
/** List of valid pin to peripheral connections for the scb_spi_m_miso signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_miso[14];
/** Indicates that a pin map exists for scb_spi_m_mosi*/
#define CYHAL_PIN_MAP_SCB_SPI_M_MOSI
/** List of valid pin to peripheral connections for the scb_spi_m_mosi signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_mosi[14];
/** Indicates that a pin map exists for scb_spi_m_select0*/
#define CYHAL_PIN_MAP_SCB_SPI_M_SELECT0
/** List of valid pin to peripheral connections for the scb_spi_m_select0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select0[12];
/** Indicates that a pin map exists for scb_spi_m_select1*/
#define CYHAL_PIN_MAP_SCB_SPI_M_SELECT1
/** List of valid pin to peripheral connections for the scb_spi_m_select1 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select1[9];
/** Indicates that a pin map exists for scb_spi_m_select2*/
#define CYHAL_PIN_MAP_SCB_SPI_M_SELECT2
/** List of valid pin to peripheral connections for the scb_spi_m_select2 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select2[8];
/** Indicates that a pin map exists for scb_spi_m_select3*/
#define CYHAL_PIN_MAP_SCB_SPI_M_SELECT3
/** List of valid pin to peripheral connections for the scb_spi_m_select3 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_m_select3[6];
/** Indicates that a pin map exists for scb_spi_s_clk*/
#define CYHAL_PIN_MAP_SCB_SPI_S_CLK
/** List of valid pin to peripheral connections for the scb_spi_s_clk signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_clk[12];
/** Indicates that a pin map exists for scb_spi_s_miso*/
#define CYHAL_PIN_MAP_SCB_SPI_S_MISO
/** List of valid pin to peripheral connections for the scb_spi_s_miso signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_miso[14];
/** Indicates that a pin map exists for scb_spi_s_mosi*/
#define CYHAL_PIN_MAP_SCB_SPI_S_MOSI
/** List of valid pin to peripheral connections for the scb_spi_s_mosi signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_mosi[14];
/** Indicates that a pin map exists for scb_spi_s_select0*/
#define CYHAL_PIN_MAP_SCB_SPI_S_SELECT0
/** List of valid pin to peripheral connections for the scb_spi_s_select0 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select0[12];
/** Indicates that a pin map exists for scb_spi_s_select1*/
#define CYHAL_PIN_MAP_SCB_SPI_S_SELECT1
/** List of valid pin to peripheral connections for the scb_spi_s_select1 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select1[9];
/** Indicates that a pin map exists for scb_spi_s_select2*/
#define CYHAL_PIN_MAP_SCB_SPI_S_SELECT2
/** List of valid pin to peripheral connections for the scb_spi_s_select2 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select2[8];
/** Indicates that a pin map exists for scb_spi_s_select3*/
#define CYHAL_PIN_MAP_SCB_SPI_S_SELECT3
/** List of valid pin to peripheral connections for the scb_spi_s_select3 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_s_select3[6];
/** Indicates that a pin map exists for scb_uart_cts*/
#define CYHAL_PIN_MAP_SCB_UART_CTS
/** List of valid pin to peripheral connections for the scb_uart_cts signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_cts[10];
/** Indicates that a pin map exists for scb_uart_rts*/
#define CYHAL_PIN_MAP_SCB_UART_RTS
/** List of valid pin to peripheral connections for the scb_uart_rts signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rts[10];
/** Indicates that a pin map exists for scb_uart_rx*/
#define CYHAL_PIN_MAP_SCB_UART_RX
/** List of valid pin to peripheral connections for the scb_uart_rx signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rx[12];
/** Indicates that a pin map exists for scb_uart_tx*/
#define CYHAL_PIN_MAP_SCB_UART_TX
/** List of valid pin to peripheral connections for the scb_uart_tx signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_tx[12];
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
/** Indicates that a pin map exists for smif_spi_data4*/
#define CYHAL_PIN_MAP_SMIF_SPI_DATA4
/** List of valid pin to peripheral connections for the smif_spi_data4 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data4[1];
/** Indicates that a pin map exists for smif_spi_data5*/
#define CYHAL_PIN_MAP_SMIF_SPI_DATA5
/** List of valid pin to peripheral connections for the smif_spi_data5 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data5[1];
/** Indicates that a pin map exists for smif_spi_data6*/
#define CYHAL_PIN_MAP_SMIF_SPI_DATA6
/** List of valid pin to peripheral connections for the smif_spi_data6 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data6[1];
/** Indicates that a pin map exists for smif_spi_data7*/
#define CYHAL_PIN_MAP_SMIF_SPI_DATA7
/** List of valid pin to peripheral connections for the smif_spi_data7 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data7[1];
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
/** Indicates that a pin map exists for smif_spi_select3*/
#define CYHAL_PIN_MAP_SMIF_SPI_SELECT3
/** List of valid pin to peripheral connections for the smif_spi_select3 signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select3[1];
/** Indicates that a pin map exists for tcpwm_line*/
#define CYHAL_PIN_MAP_TCPWM_LINE
/** List of valid pin to peripheral connections for the tcpwm_line signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line[68];
/** Indicates that a pin map exists for tcpwm_line_compl*/
#define CYHAL_PIN_MAP_TCPWM_LINE_COMPL
/** List of valid pin to peripheral connections for the tcpwm_line_compl signal. */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line_compl[66];
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

#endif /* _CYHAL_PSOC6_01_104_M_CSP_BLE_USB_H_ */


/* [] END OF FILE */
