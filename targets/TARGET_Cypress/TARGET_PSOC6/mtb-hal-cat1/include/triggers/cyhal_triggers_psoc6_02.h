/***************************************************************************//**
* \file cyhal_triggers_psoc6_02.h
*
* \brief
* PSoC6_02 family HAL triggers header
*
* \note
* Generator version: 1.6.0.453
*
********************************************************************************
* \copyright
* Copyright 2016-2021 Cypress Semiconductor Corporation
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

#ifndef _CYHAL_TRIGGERS_PSOC6_02_H_
#define _CYHAL_TRIGGERS_PSOC6_02_H_

/**
 * \addtogroup group_hal_impl_triggers_psoc6_02 PSOC6_02
 * \ingroup group_hal_impl_triggers
 * \{
 * Trigger connections for psoc6_02
 */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/** @brief Name of each input trigger. */
typedef enum
{
    CYHAL_TRIGGER_CPUSS_ZERO = 0, //!< cpuss.zero
    CYHAL_TRIGGER_AUDIOSS0_TR_I2S_RX_REQ = 1, //!< audioss[0].tr_i2s_rx_req
    CYHAL_TRIGGER_AUDIOSS1_TR_I2S_RX_REQ = 2, //!< audioss[1].tr_i2s_rx_req
    CYHAL_TRIGGER_AUDIOSS0_TR_I2S_TX_REQ = 3, //!< audioss[0].tr_i2s_tx_req
    CYHAL_TRIGGER_AUDIOSS1_TR_I2S_TX_REQ = 4, //!< audioss[1].tr_i2s_tx_req
    CYHAL_TRIGGER_AUDIOSS0_TR_PDM_RX_REQ = 5, //!< audioss[0].tr_pdm_rx_req
    CYHAL_TRIGGER_CPUSS_CTI_TR_OUT0 = 6, //!< cpuss.cti_tr_out[0]
    CYHAL_TRIGGER_CPUSS_CTI_TR_OUT1 = 7, //!< cpuss.cti_tr_out[1]
    CYHAL_TRIGGER_CPUSS_DMAC_TR_OUT0 = 8, //!< cpuss.dmac_tr_out[0]
    CYHAL_TRIGGER_CPUSS_DMAC_TR_OUT1 = 9, //!< cpuss.dmac_tr_out[1]
    CYHAL_TRIGGER_CPUSS_DMAC_TR_OUT2 = 10, //!< cpuss.dmac_tr_out[2]
    CYHAL_TRIGGER_CPUSS_DMAC_TR_OUT3 = 11, //!< cpuss.dmac_tr_out[3]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT0 = 12, //!< cpuss.dw0_tr_out[0]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT1 = 13, //!< cpuss.dw0_tr_out[1]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT2 = 14, //!< cpuss.dw0_tr_out[2]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT3 = 15, //!< cpuss.dw0_tr_out[3]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT4 = 16, //!< cpuss.dw0_tr_out[4]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT5 = 17, //!< cpuss.dw0_tr_out[5]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT6 = 18, //!< cpuss.dw0_tr_out[6]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT7 = 19, //!< cpuss.dw0_tr_out[7]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT8 = 20, //!< cpuss.dw0_tr_out[8]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT9 = 21, //!< cpuss.dw0_tr_out[9]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT10 = 22, //!< cpuss.dw0_tr_out[10]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT11 = 23, //!< cpuss.dw0_tr_out[11]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT12 = 24, //!< cpuss.dw0_tr_out[12]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT13 = 25, //!< cpuss.dw0_tr_out[13]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT14 = 26, //!< cpuss.dw0_tr_out[14]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT15 = 27, //!< cpuss.dw0_tr_out[15]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT16 = 28, //!< cpuss.dw0_tr_out[16]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT17 = 29, //!< cpuss.dw0_tr_out[17]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT18 = 30, //!< cpuss.dw0_tr_out[18]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT19 = 31, //!< cpuss.dw0_tr_out[19]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT20 = 32, //!< cpuss.dw0_tr_out[20]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT21 = 33, //!< cpuss.dw0_tr_out[21]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT22 = 34, //!< cpuss.dw0_tr_out[22]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT23 = 35, //!< cpuss.dw0_tr_out[23]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT24 = 36, //!< cpuss.dw0_tr_out[24]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT25 = 37, //!< cpuss.dw0_tr_out[25]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT26 = 38, //!< cpuss.dw0_tr_out[26]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT27 = 39, //!< cpuss.dw0_tr_out[27]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT28 = 40, //!< cpuss.dw0_tr_out[28]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT0 = 41, //!< cpuss.dw1_tr_out[0]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT1 = 42, //!< cpuss.dw1_tr_out[1]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT2 = 43, //!< cpuss.dw1_tr_out[2]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT3 = 44, //!< cpuss.dw1_tr_out[3]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT4 = 45, //!< cpuss.dw1_tr_out[4]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT5 = 46, //!< cpuss.dw1_tr_out[5]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT6 = 47, //!< cpuss.dw1_tr_out[6]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT7 = 48, //!< cpuss.dw1_tr_out[7]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT8 = 49, //!< cpuss.dw1_tr_out[8]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT9 = 50, //!< cpuss.dw1_tr_out[9]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT10 = 51, //!< cpuss.dw1_tr_out[10]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT11 = 52, //!< cpuss.dw1_tr_out[11]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT12 = 53, //!< cpuss.dw1_tr_out[12]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT13 = 54, //!< cpuss.dw1_tr_out[13]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT14 = 55, //!< cpuss.dw1_tr_out[14]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT15 = 56, //!< cpuss.dw1_tr_out[15]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT16 = 57, //!< cpuss.dw1_tr_out[16]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT17 = 58, //!< cpuss.dw1_tr_out[17]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT18 = 59, //!< cpuss.dw1_tr_out[18]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT19 = 60, //!< cpuss.dw1_tr_out[19]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT20 = 61, //!< cpuss.dw1_tr_out[20]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT21 = 62, //!< cpuss.dw1_tr_out[21]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT22 = 63, //!< cpuss.dw1_tr_out[22]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT23 = 64, //!< cpuss.dw1_tr_out[23]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT24 = 65, //!< cpuss.dw1_tr_out[24]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT25 = 66, //!< cpuss.dw1_tr_out[25]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT26 = 67, //!< cpuss.dw1_tr_out[26]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT27 = 68, //!< cpuss.dw1_tr_out[27]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT28 = 69, //!< cpuss.dw1_tr_out[28]
    CYHAL_TRIGGER_CPUSS_TR_FAULT0 = 70, //!< cpuss.tr_fault[0]
    CYHAL_TRIGGER_CPUSS_TR_FAULT1 = 71, //!< cpuss.tr_fault[1]
    CYHAL_TRIGGER_CSD_DSI_SAMPLE_OUT = 72, //!< csd.dsi_sample_out
    CYHAL_TRIGGER_CSD_DSI_SENSE_OUT = 73, //!< csd.dsi_sense_out
    CYHAL_TRIGGER_CSD_TR_ADC_DONE = 74, //!< csd.tr_adc_done
    CYHAL_TRIGGER_LPCOMP_DSI_COMP0 = 75, //!< lpcomp.dsi_comp0
    CYHAL_TRIGGER_LPCOMP_DSI_COMP1 = 76, //!< lpcomp.dsi_comp1
    CYHAL_TRIGGER_PASS_TR_SAR_OUT = 77, //!< pass.tr_sar_out
    CYHAL_TRIGGER_PERI_TR_IO_INPUT0 = 78, //!< peri.tr_io_input[0]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT1 = 79, //!< peri.tr_io_input[1]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT2 = 80, //!< peri.tr_io_input[2]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT3 = 81, //!< peri.tr_io_input[3]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT4 = 82, //!< peri.tr_io_input[4]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT5 = 83, //!< peri.tr_io_input[5]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT6 = 84, //!< peri.tr_io_input[6]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT7 = 85, //!< peri.tr_io_input[7]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT8 = 86, //!< peri.tr_io_input[8]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT9 = 87, //!< peri.tr_io_input[9]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT10 = 88, //!< peri.tr_io_input[10]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT11 = 89, //!< peri.tr_io_input[11]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT12 = 90, //!< peri.tr_io_input[12]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT13 = 91, //!< peri.tr_io_input[13]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT14 = 92, //!< peri.tr_io_input[14]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT15 = 93, //!< peri.tr_io_input[15]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT16 = 94, //!< peri.tr_io_input[16]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT17 = 95, //!< peri.tr_io_input[17]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT18 = 96, //!< peri.tr_io_input[18]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT19 = 97, //!< peri.tr_io_input[19]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT20 = 98, //!< peri.tr_io_input[20]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT21 = 99, //!< peri.tr_io_input[21]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT22 = 100, //!< peri.tr_io_input[22]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT23 = 101, //!< peri.tr_io_input[23]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT24 = 102, //!< peri.tr_io_input[24]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT25 = 103, //!< peri.tr_io_input[25]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT26 = 104, //!< peri.tr_io_input[26]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT27 = 105, //!< peri.tr_io_input[27]
    CYHAL_TRIGGER_SCB0_TR_I2C_SCL_FILTERED = 106, //!< scb[0].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB1_TR_I2C_SCL_FILTERED = 107, //!< scb[1].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB2_TR_I2C_SCL_FILTERED = 108, //!< scb[2].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB3_TR_I2C_SCL_FILTERED = 109, //!< scb[3].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB4_TR_I2C_SCL_FILTERED = 110, //!< scb[4].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB5_TR_I2C_SCL_FILTERED = 111, //!< scb[5].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB6_TR_I2C_SCL_FILTERED = 112, //!< scb[6].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB7_TR_I2C_SCL_FILTERED = 113, //!< scb[7].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB8_TR_I2C_SCL_FILTERED = 114, //!< scb[8].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB9_TR_I2C_SCL_FILTERED = 115, //!< scb[9].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB10_TR_I2C_SCL_FILTERED = 116, //!< scb[10].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB11_TR_I2C_SCL_FILTERED = 117, //!< scb[11].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB12_TR_I2C_SCL_FILTERED = 118, //!< scb[12].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB0_TR_RX_REQ = 119, //!< scb[0].tr_rx_req
    CYHAL_TRIGGER_SCB1_TR_RX_REQ = 120, //!< scb[1].tr_rx_req
    CYHAL_TRIGGER_SCB2_TR_RX_REQ = 121, //!< scb[2].tr_rx_req
    CYHAL_TRIGGER_SCB3_TR_RX_REQ = 122, //!< scb[3].tr_rx_req
    CYHAL_TRIGGER_SCB4_TR_RX_REQ = 123, //!< scb[4].tr_rx_req
    CYHAL_TRIGGER_SCB5_TR_RX_REQ = 124, //!< scb[5].tr_rx_req
    CYHAL_TRIGGER_SCB6_TR_RX_REQ = 125, //!< scb[6].tr_rx_req
    CYHAL_TRIGGER_SCB7_TR_RX_REQ = 126, //!< scb[7].tr_rx_req
    CYHAL_TRIGGER_SCB8_TR_RX_REQ = 127, //!< scb[8].tr_rx_req
    CYHAL_TRIGGER_SCB9_TR_RX_REQ = 128, //!< scb[9].tr_rx_req
    CYHAL_TRIGGER_SCB10_TR_RX_REQ = 129, //!< scb[10].tr_rx_req
    CYHAL_TRIGGER_SCB11_TR_RX_REQ = 130, //!< scb[11].tr_rx_req
    CYHAL_TRIGGER_SCB12_TR_RX_REQ = 131, //!< scb[12].tr_rx_req
    CYHAL_TRIGGER_SCB0_TR_TX_REQ = 132, //!< scb[0].tr_tx_req
    CYHAL_TRIGGER_SCB1_TR_TX_REQ = 133, //!< scb[1].tr_tx_req
    CYHAL_TRIGGER_SCB2_TR_TX_REQ = 134, //!< scb[2].tr_tx_req
    CYHAL_TRIGGER_SCB3_TR_TX_REQ = 135, //!< scb[3].tr_tx_req
    CYHAL_TRIGGER_SCB4_TR_TX_REQ = 136, //!< scb[4].tr_tx_req
    CYHAL_TRIGGER_SCB5_TR_TX_REQ = 137, //!< scb[5].tr_tx_req
    CYHAL_TRIGGER_SCB6_TR_TX_REQ = 138, //!< scb[6].tr_tx_req
    CYHAL_TRIGGER_SCB7_TR_TX_REQ = 139, //!< scb[7].tr_tx_req
    CYHAL_TRIGGER_SCB8_TR_TX_REQ = 140, //!< scb[8].tr_tx_req
    CYHAL_TRIGGER_SCB9_TR_TX_REQ = 141, //!< scb[9].tr_tx_req
    CYHAL_TRIGGER_SCB10_TR_TX_REQ = 142, //!< scb[10].tr_tx_req
    CYHAL_TRIGGER_SCB11_TR_TX_REQ = 143, //!< scb[11].tr_tx_req
    CYHAL_TRIGGER_SCB12_TR_TX_REQ = 144, //!< scb[12].tr_tx_req
    CYHAL_TRIGGER_SMIF_TR_RX_REQ = 145, //!< smif.tr_rx_req
    CYHAL_TRIGGER_SMIF_TR_TX_REQ = 146, //!< smif.tr_tx_req
    CYHAL_TRIGGER_TCPWM0_TR_COMPARE_MATCH0 = 147, //!< tcpwm[0].tr_compare_match[0]
    CYHAL_TRIGGER_TCPWM0_TR_COMPARE_MATCH1 = 148, //!< tcpwm[0].tr_compare_match[1]
    CYHAL_TRIGGER_TCPWM0_TR_COMPARE_MATCH2 = 149, //!< tcpwm[0].tr_compare_match[2]
    CYHAL_TRIGGER_TCPWM0_TR_COMPARE_MATCH3 = 150, //!< tcpwm[0].tr_compare_match[3]
    CYHAL_TRIGGER_TCPWM0_TR_COMPARE_MATCH4 = 151, //!< tcpwm[0].tr_compare_match[4]
    CYHAL_TRIGGER_TCPWM0_TR_COMPARE_MATCH5 = 152, //!< tcpwm[0].tr_compare_match[5]
    CYHAL_TRIGGER_TCPWM0_TR_COMPARE_MATCH6 = 153, //!< tcpwm[0].tr_compare_match[6]
    CYHAL_TRIGGER_TCPWM0_TR_COMPARE_MATCH7 = 154, //!< tcpwm[0].tr_compare_match[7]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH0 = 155, //!< tcpwm[1].tr_compare_match[0]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH1 = 156, //!< tcpwm[1].tr_compare_match[1]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH2 = 157, //!< tcpwm[1].tr_compare_match[2]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH3 = 158, //!< tcpwm[1].tr_compare_match[3]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH4 = 159, //!< tcpwm[1].tr_compare_match[4]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH5 = 160, //!< tcpwm[1].tr_compare_match[5]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH6 = 161, //!< tcpwm[1].tr_compare_match[6]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH7 = 162, //!< tcpwm[1].tr_compare_match[7]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH8 = 163, //!< tcpwm[1].tr_compare_match[8]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH9 = 164, //!< tcpwm[1].tr_compare_match[9]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH10 = 165, //!< tcpwm[1].tr_compare_match[10]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH11 = 166, //!< tcpwm[1].tr_compare_match[11]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH12 = 167, //!< tcpwm[1].tr_compare_match[12]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH13 = 168, //!< tcpwm[1].tr_compare_match[13]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH14 = 169, //!< tcpwm[1].tr_compare_match[14]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH15 = 170, //!< tcpwm[1].tr_compare_match[15]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH16 = 171, //!< tcpwm[1].tr_compare_match[16]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH17 = 172, //!< tcpwm[1].tr_compare_match[17]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH18 = 173, //!< tcpwm[1].tr_compare_match[18]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH19 = 174, //!< tcpwm[1].tr_compare_match[19]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH20 = 175, //!< tcpwm[1].tr_compare_match[20]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH21 = 176, //!< tcpwm[1].tr_compare_match[21]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH22 = 177, //!< tcpwm[1].tr_compare_match[22]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH23 = 178, //!< tcpwm[1].tr_compare_match[23]
    CYHAL_TRIGGER_TCPWM0_TR_OVERFLOW0 = 179, //!< tcpwm[0].tr_overflow[0]
    CYHAL_TRIGGER_TCPWM0_TR_OVERFLOW1 = 180, //!< tcpwm[0].tr_overflow[1]
    CYHAL_TRIGGER_TCPWM0_TR_OVERFLOW2 = 181, //!< tcpwm[0].tr_overflow[2]
    CYHAL_TRIGGER_TCPWM0_TR_OVERFLOW3 = 182, //!< tcpwm[0].tr_overflow[3]
    CYHAL_TRIGGER_TCPWM0_TR_OVERFLOW4 = 183, //!< tcpwm[0].tr_overflow[4]
    CYHAL_TRIGGER_TCPWM0_TR_OVERFLOW5 = 184, //!< tcpwm[0].tr_overflow[5]
    CYHAL_TRIGGER_TCPWM0_TR_OVERFLOW6 = 185, //!< tcpwm[0].tr_overflow[6]
    CYHAL_TRIGGER_TCPWM0_TR_OVERFLOW7 = 186, //!< tcpwm[0].tr_overflow[7]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW0 = 187, //!< tcpwm[1].tr_overflow[0]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW1 = 188, //!< tcpwm[1].tr_overflow[1]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW2 = 189, //!< tcpwm[1].tr_overflow[2]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW3 = 190, //!< tcpwm[1].tr_overflow[3]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW4 = 191, //!< tcpwm[1].tr_overflow[4]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW5 = 192, //!< tcpwm[1].tr_overflow[5]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW6 = 193, //!< tcpwm[1].tr_overflow[6]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW7 = 194, //!< tcpwm[1].tr_overflow[7]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW8 = 195, //!< tcpwm[1].tr_overflow[8]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW9 = 196, //!< tcpwm[1].tr_overflow[9]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW10 = 197, //!< tcpwm[1].tr_overflow[10]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW11 = 198, //!< tcpwm[1].tr_overflow[11]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW12 = 199, //!< tcpwm[1].tr_overflow[12]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW13 = 200, //!< tcpwm[1].tr_overflow[13]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW14 = 201, //!< tcpwm[1].tr_overflow[14]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW15 = 202, //!< tcpwm[1].tr_overflow[15]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW16 = 203, //!< tcpwm[1].tr_overflow[16]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW17 = 204, //!< tcpwm[1].tr_overflow[17]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW18 = 205, //!< tcpwm[1].tr_overflow[18]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW19 = 206, //!< tcpwm[1].tr_overflow[19]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW20 = 207, //!< tcpwm[1].tr_overflow[20]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW21 = 208, //!< tcpwm[1].tr_overflow[21]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW22 = 209, //!< tcpwm[1].tr_overflow[22]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW23 = 210, //!< tcpwm[1].tr_overflow[23]
    CYHAL_TRIGGER_TCPWM0_TR_UNDERFLOW0 = 211, //!< tcpwm[0].tr_underflow[0]
    CYHAL_TRIGGER_TCPWM0_TR_UNDERFLOW1 = 212, //!< tcpwm[0].tr_underflow[1]
    CYHAL_TRIGGER_TCPWM0_TR_UNDERFLOW2 = 213, //!< tcpwm[0].tr_underflow[2]
    CYHAL_TRIGGER_TCPWM0_TR_UNDERFLOW3 = 214, //!< tcpwm[0].tr_underflow[3]
    CYHAL_TRIGGER_TCPWM0_TR_UNDERFLOW4 = 215, //!< tcpwm[0].tr_underflow[4]
    CYHAL_TRIGGER_TCPWM0_TR_UNDERFLOW5 = 216, //!< tcpwm[0].tr_underflow[5]
    CYHAL_TRIGGER_TCPWM0_TR_UNDERFLOW6 = 217, //!< tcpwm[0].tr_underflow[6]
    CYHAL_TRIGGER_TCPWM0_TR_UNDERFLOW7 = 218, //!< tcpwm[0].tr_underflow[7]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW0 = 219, //!< tcpwm[1].tr_underflow[0]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW1 = 220, //!< tcpwm[1].tr_underflow[1]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW2 = 221, //!< tcpwm[1].tr_underflow[2]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW3 = 222, //!< tcpwm[1].tr_underflow[3]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW4 = 223, //!< tcpwm[1].tr_underflow[4]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW5 = 224, //!< tcpwm[1].tr_underflow[5]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW6 = 225, //!< tcpwm[1].tr_underflow[6]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW7 = 226, //!< tcpwm[1].tr_underflow[7]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW8 = 227, //!< tcpwm[1].tr_underflow[8]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW9 = 228, //!< tcpwm[1].tr_underflow[9]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW10 = 229, //!< tcpwm[1].tr_underflow[10]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW11 = 230, //!< tcpwm[1].tr_underflow[11]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW12 = 231, //!< tcpwm[1].tr_underflow[12]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW13 = 232, //!< tcpwm[1].tr_underflow[13]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW14 = 233, //!< tcpwm[1].tr_underflow[14]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW15 = 234, //!< tcpwm[1].tr_underflow[15]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW16 = 235, //!< tcpwm[1].tr_underflow[16]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW17 = 236, //!< tcpwm[1].tr_underflow[17]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW18 = 237, //!< tcpwm[1].tr_underflow[18]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW19 = 238, //!< tcpwm[1].tr_underflow[19]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW20 = 239, //!< tcpwm[1].tr_underflow[20]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW21 = 240, //!< tcpwm[1].tr_underflow[21]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW22 = 241, //!< tcpwm[1].tr_underflow[22]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW23 = 242, //!< tcpwm[1].tr_underflow[23]
    CYHAL_TRIGGER_USB_DMA_REQ0 = 243, //!< usb.dma_req[0]
    CYHAL_TRIGGER_USB_DMA_REQ1 = 244, //!< usb.dma_req[1]
    CYHAL_TRIGGER_USB_DMA_REQ2 = 245, //!< usb.dma_req[2]
    CYHAL_TRIGGER_USB_DMA_REQ3 = 246, //!< usb.dma_req[3]
    CYHAL_TRIGGER_USB_DMA_REQ4 = 247, //!< usb.dma_req[4]
    CYHAL_TRIGGER_USB_DMA_REQ5 = 248, //!< usb.dma_req[5]
    CYHAL_TRIGGER_USB_DMA_REQ6 = 249, //!< usb.dma_req[6]
    CYHAL_TRIGGER_USB_DMA_REQ7 = 250, //!< usb.dma_req[7]
} cyhal_trigger_source_psoc6_02_t;

/** Typedef from device family specific trigger source to generic trigger source */
typedef cyhal_trigger_source_psoc6_02_t cyhal_source_t;

/** @brief Name of each output trigger. */
typedef enum
{
    CYHAL_TRIGGER_CPUSS_CTI_TR_IN0 = 0, //!< CPUSS Debug and Profiler trigger multiplexer - cpuss.cti_tr_in[0]
    CYHAL_TRIGGER_CPUSS_CTI_TR_IN1 = 1, //!< CPUSS Debug and Profiler trigger multiplexer - cpuss.cti_tr_in[1]
    CYHAL_TRIGGER_CPUSS_DMAC_TR_IN0 = 2, //!< MDMA trigger multiplexer - cpuss.dmac_tr_in[0]
    CYHAL_TRIGGER_CPUSS_DMAC_TR_IN1 = 3, //!< MDMA trigger multiplexer - cpuss.dmac_tr_in[1]
    CYHAL_TRIGGER_CPUSS_DMAC_TR_IN2 = 4, //!< MDMA trigger multiplexer - cpuss.dmac_tr_in[2]
    CYHAL_TRIGGER_CPUSS_DMAC_TR_IN3 = 5, //!< MDMA trigger multiplexer - cpuss.dmac_tr_in[3]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN0 = 6, //!< P-DMA0 trigger multiplexer - cpuss.dw0_tr_in[0]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN1 = 7, //!< P-DMA0 trigger multiplexer - cpuss.dw0_tr_in[1]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN2 = 8, //!< P-DMA0 trigger multiplexer - cpuss.dw0_tr_in[2]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN3 = 9, //!< P-DMA0 trigger multiplexer - cpuss.dw0_tr_in[3]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN4 = 10, //!< P-DMA0 trigger multiplexer - cpuss.dw0_tr_in[4]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN5 = 11, //!< P-DMA0 trigger multiplexer - cpuss.dw0_tr_in[5]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN6 = 12, //!< P-DMA0 trigger multiplexer - cpuss.dw0_tr_in[6]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN7 = 13, //!< P-DMA0 trigger multiplexer - cpuss.dw0_tr_in[7]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN8 = 14, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[8]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN9 = 15, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[9]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN10 = 16, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[10]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN11 = 17, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[11]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN12 = 18, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[12]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN13 = 19, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[13]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN14 = 20, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[14]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN15 = 21, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[15]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN16 = 22, //!< SCB DW0 Triggers - cpuss.dw0_tr_in[16]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN17 = 23, //!< SCB DW0 Triggers - cpuss.dw0_tr_in[17]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN18 = 24, //!< SCB DW0 Triggers - cpuss.dw0_tr_in[18]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN19 = 25, //!< SCB DW0 Triggers - cpuss.dw0_tr_in[19]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN20 = 26, //!< SCB DW0 Triggers - cpuss.dw0_tr_in[20]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN21 = 27, //!< SCB DW0 Triggers - cpuss.dw0_tr_in[21]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN22 = 28, //!< SCB DW0 Triggers - cpuss.dw0_tr_in[22]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN23 = 29, //!< SCB DW0 Triggers - cpuss.dw0_tr_in[23]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN24 = 30, //!< SCB DW0 Triggers - cpuss.dw0_tr_in[24]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN25 = 31, //!< SCB DW0 Triggers - cpuss.dw0_tr_in[25]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN26 = 32, //!< SCB DW0 Triggers - cpuss.dw0_tr_in[26]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN27 = 33, //!< SCB DW0 Triggers - cpuss.dw0_tr_in[27]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN28 = 34, //!< SAR to PDMA0 direct connect - cpuss.dw0_tr_in[28]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN0 = 35, //!< P-DMA1 trigger multiplexer - cpuss.dw1_tr_in[0]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN1 = 36, //!< P-DMA1 trigger multiplexer - cpuss.dw1_tr_in[1]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN2 = 37, //!< P-DMA1 trigger multiplexer - cpuss.dw1_tr_in[2]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN3 = 38, //!< P-DMA1 trigger multiplexer - cpuss.dw1_tr_in[3]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN4 = 39, //!< P-DMA1 trigger multiplexer - cpuss.dw1_tr_in[4]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN5 = 40, //!< P-DMA1 trigger multiplexer - cpuss.dw1_tr_in[5]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN6 = 41, //!< P-DMA1 trigger multiplexer - cpuss.dw1_tr_in[6]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN7 = 42, //!< P-DMA1 trigger multiplexer - cpuss.dw1_tr_in[7]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN8 = 43, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[8]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN9 = 44, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[9]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN10 = 45, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[10]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN11 = 46, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[11]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN12 = 47, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[12]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN13 = 48, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[13]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN14 = 49, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[14]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN15 = 50, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[15]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN16 = 51, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[16]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN17 = 52, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[17]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN18 = 53, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[18]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN19 = 54, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[19]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN20 = 55, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[20]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN21 = 56, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[21]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN22 = 57, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[22]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN23 = 58, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[23]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN24 = 59, //!< AUDIOSS PDMA1 triggers (I2S & PDM) - cpuss.dw1_tr_in[24]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN25 = 60, //!< AUDIOSS PDMA1 triggers (I2S & PDM) - cpuss.dw1_tr_in[25]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN26 = 61, //!< AUDIOSS PDMA1 triggers (I2S & PDM) - cpuss.dw1_tr_in[26]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN27 = 62, //!< AUDIOSS PDMA1 triggers (I2S & PDM) - cpuss.dw1_tr_in[27]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN28 = 63, //!< AUDIOSS PDMA1 triggers (I2S & PDM) - cpuss.dw1_tr_in[28]
    CYHAL_TRIGGER_CSD_DSI_START = 64, //!< Capsense trigger multiplexer - csd.dsi_start
    CYHAL_TRIGGER_PASS_TR_SAR_IN = 65, //!< ADC trigger multiplexer - pass.tr_sar_in
    CYHAL_TRIGGER_PERI_TR_DBG_FREEZE = 66, //!< PERI Freeze trigger multiplexer - peri.tr_dbg_freeze
    CYHAL_TRIGGER_PERI_TR_IO_OUTPUT0 = 67, //!< HSIOM trigger multiplexer - peri.tr_io_output[0]
    CYHAL_TRIGGER_PERI_TR_IO_OUTPUT1 = 68, //!< HSIOM trigger multiplexer - peri.tr_io_output[1]
    CYHAL_TRIGGER_PROFILE_TR_START = 69, //!< CPUSS Debug and Profiler trigger multiplexer - profile.tr_start
    CYHAL_TRIGGER_PROFILE_TR_STOP = 70, //!< CPUSS Debug and Profiler trigger multiplexer - profile.tr_stop
    CYHAL_TRIGGER_TCPWM0_TR_IN0 = 71, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[0]
    CYHAL_TRIGGER_TCPWM0_TR_IN1 = 72, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[1]
    CYHAL_TRIGGER_TCPWM0_TR_IN2 = 73, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[2]
    CYHAL_TRIGGER_TCPWM0_TR_IN3 = 74, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[3]
    CYHAL_TRIGGER_TCPWM0_TR_IN4 = 75, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[4]
    CYHAL_TRIGGER_TCPWM0_TR_IN5 = 76, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[5]
    CYHAL_TRIGGER_TCPWM0_TR_IN6 = 77, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[6]
    CYHAL_TRIGGER_TCPWM0_TR_IN7 = 78, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[7]
    CYHAL_TRIGGER_TCPWM0_TR_IN8 = 79, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[8]
    CYHAL_TRIGGER_TCPWM0_TR_IN9 = 80, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[9]
    CYHAL_TRIGGER_TCPWM0_TR_IN10 = 81, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[10]
    CYHAL_TRIGGER_TCPWM0_TR_IN11 = 82, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[11]
    CYHAL_TRIGGER_TCPWM0_TR_IN12 = 83, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[12]
    CYHAL_TRIGGER_TCPWM0_TR_IN13 = 84, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[13]
    CYHAL_TRIGGER_TCPWM1_TR_IN0 = 85, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[0]
    CYHAL_TRIGGER_TCPWM1_TR_IN1 = 86, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[1]
    CYHAL_TRIGGER_TCPWM1_TR_IN2 = 87, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[2]
    CYHAL_TRIGGER_TCPWM1_TR_IN3 = 88, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[3]
    CYHAL_TRIGGER_TCPWM1_TR_IN4 = 89, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[4]
    CYHAL_TRIGGER_TCPWM1_TR_IN5 = 90, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[5]
    CYHAL_TRIGGER_TCPWM1_TR_IN6 = 91, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[6]
    CYHAL_TRIGGER_TCPWM1_TR_IN7 = 92, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[7]
    CYHAL_TRIGGER_TCPWM1_TR_IN8 = 93, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[8]
    CYHAL_TRIGGER_TCPWM1_TR_IN9 = 94, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[9]
    CYHAL_TRIGGER_TCPWM1_TR_IN10 = 95, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[10]
    CYHAL_TRIGGER_TCPWM1_TR_IN11 = 96, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[11]
    CYHAL_TRIGGER_TCPWM1_TR_IN12 = 97, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[12]
    CYHAL_TRIGGER_TCPWM1_TR_IN13 = 98, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[13]
    CYHAL_TRIGGER_USB_DMA_BURSTEND0 = 99, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[0]
    CYHAL_TRIGGER_USB_DMA_BURSTEND1 = 100, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[1]
    CYHAL_TRIGGER_USB_DMA_BURSTEND2 = 101, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[2]
    CYHAL_TRIGGER_USB_DMA_BURSTEND3 = 102, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[3]
    CYHAL_TRIGGER_USB_DMA_BURSTEND4 = 103, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[4]
    CYHAL_TRIGGER_USB_DMA_BURSTEND5 = 104, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[5]
    CYHAL_TRIGGER_USB_DMA_BURSTEND6 = 105, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[6]
    CYHAL_TRIGGER_USB_DMA_BURSTEND7 = 106, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[7]
} cyhal_trigger_dest_psoc6_02_t;

/** Typedef from device family specific trigger dest to generic trigger dest */
typedef cyhal_trigger_dest_psoc6_02_t cyhal_dest_t;

/** \cond INTERNAL */
/** Table of number of inputs to each mux. */
extern const uint16_t cyhal_sources_per_mux[17];

/** Table indicating whether mux is 1to1. */
extern const bool cyhal_is_mux_1to1[17];

/** Table pointing to each mux source table. The index of each source in the table is its mux input index. */
extern const cyhal_source_t* cyhal_mux_to_sources [17];

/** Maps each cyhal_destination_t to a mux index.
 * If bit 8 of the mux index is set, this denotes that the trigger is a
 * one to one trigger.
 */
extern const uint8_t cyhal_dest_to_mux[107];

/* Maps each cyhal_destination_t to a specific output in its mux */
extern const uint8_t cyhal_mux_dest_index[107];
/** \endcond */

#if defined(__cplusplus)
}
#endif /* __cplusplus */
/** \} group_hal_impl_triggers_psoc6_02 */
#endif /* _CYHAL_TRIGGERS_PSOC6_02_H_ */


/* [] END OF FILE */
