/***************************************************************************//**
* \file cyhal_triggers_psoc6_03.h
*
* \brief
* PSoC6_03 family HAL triggers header
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

#ifndef _CYHAL_TRIGGERS_PSOC6_03_H_
#define _CYHAL_TRIGGERS_PSOC6_03_H_

/**
 * \addtogroup group_hal_impl_triggers_psoc6_03 PSOC6_03
 * \ingroup group_hal_impl_triggers
 * \{
 * Trigger connections for psoc6_03
 */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/** @brief Name of each input trigger. */
typedef enum
{
    CYHAL_TRIGGER_CPUSS_ZERO = 0, //!< cpuss.zero
    CYHAL_TRIGGER_CANFD0_TR_DBG_DMA_REQ0 = 1, //!< canfd[0].tr_dbg_dma_req[0]
    CYHAL_TRIGGER_CANFD0_TR_FIFO00 = 2, //!< canfd[0].tr_fifo0[0]
    CYHAL_TRIGGER_CANFD0_TR_FIFO10 = 3, //!< canfd[0].tr_fifo1[0]
    CYHAL_TRIGGER_CANFD0_TR_TMP_RTP_OUT0 = 4, //!< canfd[0].tr_tmp_rtp_out[0]
    CYHAL_TRIGGER_CPUSS_CTI_TR_OUT0 = 5, //!< cpuss.cti_tr_out[0]
    CYHAL_TRIGGER_CPUSS_CTI_TR_OUT1 = 6, //!< cpuss.cti_tr_out[1]
    CYHAL_TRIGGER_CPUSS_DMAC_TR_OUT0 = 7, //!< cpuss.dmac_tr_out[0]
    CYHAL_TRIGGER_CPUSS_DMAC_TR_OUT1 = 8, //!< cpuss.dmac_tr_out[1]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT0 = 9, //!< cpuss.dw0_tr_out[0]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT1 = 10, //!< cpuss.dw0_tr_out[1]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT2 = 11, //!< cpuss.dw0_tr_out[2]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT3 = 12, //!< cpuss.dw0_tr_out[3]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT4 = 13, //!< cpuss.dw0_tr_out[4]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT5 = 14, //!< cpuss.dw0_tr_out[5]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT6 = 15, //!< cpuss.dw0_tr_out[6]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT7 = 16, //!< cpuss.dw0_tr_out[7]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT8 = 17, //!< cpuss.dw0_tr_out[8]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT9 = 18, //!< cpuss.dw0_tr_out[9]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT10 = 19, //!< cpuss.dw0_tr_out[10]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT11 = 20, //!< cpuss.dw0_tr_out[11]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT12 = 21, //!< cpuss.dw0_tr_out[12]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT13 = 22, //!< cpuss.dw0_tr_out[13]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT14 = 23, //!< cpuss.dw0_tr_out[14]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT15 = 24, //!< cpuss.dw0_tr_out[15]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT16 = 25, //!< cpuss.dw0_tr_out[16]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT17 = 26, //!< cpuss.dw0_tr_out[17]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT18 = 27, //!< cpuss.dw0_tr_out[18]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT19 = 28, //!< cpuss.dw0_tr_out[19]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT20 = 29, //!< cpuss.dw0_tr_out[20]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT21 = 30, //!< cpuss.dw0_tr_out[21]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT22 = 31, //!< cpuss.dw0_tr_out[22]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT23 = 32, //!< cpuss.dw0_tr_out[23]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT24 = 33, //!< cpuss.dw0_tr_out[24]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT25 = 34, //!< cpuss.dw0_tr_out[25]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT26 = 35, //!< cpuss.dw0_tr_out[26]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT27 = 36, //!< cpuss.dw0_tr_out[27]
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT28 = 37, //!< cpuss.dw0_tr_out[28]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT0 = 38, //!< cpuss.dw1_tr_out[0]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT1 = 39, //!< cpuss.dw1_tr_out[1]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT2 = 40, //!< cpuss.dw1_tr_out[2]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT3 = 41, //!< cpuss.dw1_tr_out[3]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT4 = 42, //!< cpuss.dw1_tr_out[4]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT5 = 43, //!< cpuss.dw1_tr_out[5]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT6 = 44, //!< cpuss.dw1_tr_out[6]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT7 = 45, //!< cpuss.dw1_tr_out[7]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT8 = 46, //!< cpuss.dw1_tr_out[8]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT9 = 47, //!< cpuss.dw1_tr_out[9]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT10 = 48, //!< cpuss.dw1_tr_out[10]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT11 = 49, //!< cpuss.dw1_tr_out[11]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT12 = 50, //!< cpuss.dw1_tr_out[12]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT13 = 51, //!< cpuss.dw1_tr_out[13]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT14 = 52, //!< cpuss.dw1_tr_out[14]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT15 = 53, //!< cpuss.dw1_tr_out[15]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT16 = 54, //!< cpuss.dw1_tr_out[16]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT17 = 55, //!< cpuss.dw1_tr_out[17]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT18 = 56, //!< cpuss.dw1_tr_out[18]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT19 = 57, //!< cpuss.dw1_tr_out[19]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT20 = 58, //!< cpuss.dw1_tr_out[20]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT21 = 59, //!< cpuss.dw1_tr_out[21]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT22 = 60, //!< cpuss.dw1_tr_out[22]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT23 = 61, //!< cpuss.dw1_tr_out[23]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT24 = 62, //!< cpuss.dw1_tr_out[24]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT25 = 63, //!< cpuss.dw1_tr_out[25]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT26 = 64, //!< cpuss.dw1_tr_out[26]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT27 = 65, //!< cpuss.dw1_tr_out[27]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT28 = 66, //!< cpuss.dw1_tr_out[28]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT29 = 67, //!< cpuss.dw1_tr_out[29]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT30 = 68, //!< cpuss.dw1_tr_out[30]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT31 = 69, //!< cpuss.dw1_tr_out[31]
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
    CYHAL_TRIGGER_SCB0_TR_I2C_SCL_FILTERED = 104, //!< scb[0].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB1_TR_I2C_SCL_FILTERED = 105, //!< scb[1].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB2_TR_I2C_SCL_FILTERED = 106, //!< scb[2].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB3_TR_I2C_SCL_FILTERED = 107, //!< scb[3].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB4_TR_I2C_SCL_FILTERED = 108, //!< scb[4].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB5_TR_I2C_SCL_FILTERED = 109, //!< scb[5].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB6_TR_I2C_SCL_FILTERED = 110, //!< scb[6].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB0_TR_RX_REQ = 111, //!< scb[0].tr_rx_req
    CYHAL_TRIGGER_SCB1_TR_RX_REQ = 112, //!< scb[1].tr_rx_req
    CYHAL_TRIGGER_SCB2_TR_RX_REQ = 113, //!< scb[2].tr_rx_req
    CYHAL_TRIGGER_SCB3_TR_RX_REQ = 114, //!< scb[3].tr_rx_req
    CYHAL_TRIGGER_SCB4_TR_RX_REQ = 115, //!< scb[4].tr_rx_req
    CYHAL_TRIGGER_SCB5_TR_RX_REQ = 116, //!< scb[5].tr_rx_req
    CYHAL_TRIGGER_SCB6_TR_RX_REQ = 117, //!< scb[6].tr_rx_req
    CYHAL_TRIGGER_SCB0_TR_TX_REQ = 118, //!< scb[0].tr_tx_req
    CYHAL_TRIGGER_SCB1_TR_TX_REQ = 119, //!< scb[1].tr_tx_req
    CYHAL_TRIGGER_SCB2_TR_TX_REQ = 120, //!< scb[2].tr_tx_req
    CYHAL_TRIGGER_SCB3_TR_TX_REQ = 121, //!< scb[3].tr_tx_req
    CYHAL_TRIGGER_SCB4_TR_TX_REQ = 122, //!< scb[4].tr_tx_req
    CYHAL_TRIGGER_SCB5_TR_TX_REQ = 123, //!< scb[5].tr_tx_req
    CYHAL_TRIGGER_SCB6_TR_TX_REQ = 124, //!< scb[6].tr_tx_req
    CYHAL_TRIGGER_SMIF_TR_RX_REQ = 125, //!< smif.tr_rx_req
    CYHAL_TRIGGER_SMIF_TR_TX_REQ = 126, //!< smif.tr_tx_req
    CYHAL_TRIGGER_TCPWM0_TR_COMPARE_MATCH0 = 127, //!< tcpwm[0].tr_compare_match[0]
    CYHAL_TRIGGER_TCPWM0_TR_COMPARE_MATCH1 = 128, //!< tcpwm[0].tr_compare_match[1]
    CYHAL_TRIGGER_TCPWM0_TR_COMPARE_MATCH2 = 129, //!< tcpwm[0].tr_compare_match[2]
    CYHAL_TRIGGER_TCPWM0_TR_COMPARE_MATCH3 = 130, //!< tcpwm[0].tr_compare_match[3]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH0 = 131, //!< tcpwm[1].tr_compare_match[0]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH1 = 132, //!< tcpwm[1].tr_compare_match[1]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH2 = 133, //!< tcpwm[1].tr_compare_match[2]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH3 = 134, //!< tcpwm[1].tr_compare_match[3]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH4 = 135, //!< tcpwm[1].tr_compare_match[4]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH5 = 136, //!< tcpwm[1].tr_compare_match[5]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH6 = 137, //!< tcpwm[1].tr_compare_match[6]
    CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH7 = 138, //!< tcpwm[1].tr_compare_match[7]
    CYHAL_TRIGGER_TCPWM0_TR_OVERFLOW0 = 139, //!< tcpwm[0].tr_overflow[0]
    CYHAL_TRIGGER_TCPWM0_TR_OVERFLOW1 = 140, //!< tcpwm[0].tr_overflow[1]
    CYHAL_TRIGGER_TCPWM0_TR_OVERFLOW2 = 141, //!< tcpwm[0].tr_overflow[2]
    CYHAL_TRIGGER_TCPWM0_TR_OVERFLOW3 = 142, //!< tcpwm[0].tr_overflow[3]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW0 = 143, //!< tcpwm[1].tr_overflow[0]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW1 = 144, //!< tcpwm[1].tr_overflow[1]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW2 = 145, //!< tcpwm[1].tr_overflow[2]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW3 = 146, //!< tcpwm[1].tr_overflow[3]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW4 = 147, //!< tcpwm[1].tr_overflow[4]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW5 = 148, //!< tcpwm[1].tr_overflow[5]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW6 = 149, //!< tcpwm[1].tr_overflow[6]
    CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW7 = 150, //!< tcpwm[1].tr_overflow[7]
    CYHAL_TRIGGER_TCPWM0_TR_UNDERFLOW0 = 151, //!< tcpwm[0].tr_underflow[0]
    CYHAL_TRIGGER_TCPWM0_TR_UNDERFLOW1 = 152, //!< tcpwm[0].tr_underflow[1]
    CYHAL_TRIGGER_TCPWM0_TR_UNDERFLOW2 = 153, //!< tcpwm[0].tr_underflow[2]
    CYHAL_TRIGGER_TCPWM0_TR_UNDERFLOW3 = 154, //!< tcpwm[0].tr_underflow[3]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW0 = 155, //!< tcpwm[1].tr_underflow[0]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW1 = 156, //!< tcpwm[1].tr_underflow[1]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW2 = 157, //!< tcpwm[1].tr_underflow[2]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW3 = 158, //!< tcpwm[1].tr_underflow[3]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW4 = 159, //!< tcpwm[1].tr_underflow[4]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW5 = 160, //!< tcpwm[1].tr_underflow[5]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW6 = 161, //!< tcpwm[1].tr_underflow[6]
    CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW7 = 162, //!< tcpwm[1].tr_underflow[7]
    CYHAL_TRIGGER_USB_DMA_REQ0 = 163, //!< usb.dma_req[0]
    CYHAL_TRIGGER_USB_DMA_REQ1 = 164, //!< usb.dma_req[1]
    CYHAL_TRIGGER_USB_DMA_REQ2 = 165, //!< usb.dma_req[2]
    CYHAL_TRIGGER_USB_DMA_REQ3 = 166, //!< usb.dma_req[3]
    CYHAL_TRIGGER_USB_DMA_REQ4 = 167, //!< usb.dma_req[4]
    CYHAL_TRIGGER_USB_DMA_REQ5 = 168, //!< usb.dma_req[5]
    CYHAL_TRIGGER_USB_DMA_REQ6 = 169, //!< usb.dma_req[6]
    CYHAL_TRIGGER_USB_DMA_REQ7 = 170, //!< usb.dma_req[7]
} cyhal_trigger_source_psoc6_03_t;

/** Typedef from device family specific trigger source to generic trigger source */
typedef cyhal_trigger_source_psoc6_03_t cyhal_source_t;

/** @brief Name of each output trigger. */
typedef enum
{
    CYHAL_TRIGGER_CANFD0_TR_DBG_DMA_ACK0 = 0, //!< CAN DW0 triggers (from DW back to CAN) - canfd[0].tr_dbg_dma_ack[0]
    CYHAL_TRIGGER_CANFD0_TR_EVT_SWT_IN0 = 1, //!< CAN TT Sync - canfd[0].tr_evt_swt_in[0]
    CYHAL_TRIGGER_CPUSS_CTI_TR_IN0 = 2, //!< CPUSS Debug trigger multiplexer - cpuss.cti_tr_in[0]
    CYHAL_TRIGGER_CPUSS_CTI_TR_IN1 = 3, //!< CPUSS Debug trigger multiplexer - cpuss.cti_tr_in[1]
    CYHAL_TRIGGER_CPUSS_DMAC_TR_IN0 = 4, //!< MDMA trigger multiplexer - cpuss.dmac_tr_in[0]
    CYHAL_TRIGGER_CPUSS_DMAC_TR_IN1 = 5, //!< MDMA trigger multiplexer - cpuss.dmac_tr_in[1]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN0 = 6, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[0]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN1 = 7, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[1]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN2 = 8, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[2]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN3 = 9, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[3]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN4 = 10, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[4]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN5 = 11, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[5]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN6 = 12, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[6]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN7 = 13, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[7]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN8 = 14, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[8]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN9 = 15, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[9]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN10 = 16, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[10]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN11 = 17, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[11]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN12 = 18, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[12]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN13 = 19, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[13]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN14 = 20, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[14]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN15 = 21, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[15]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN16 = 22, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[16]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN17 = 23, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[17]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN18 = 24, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[18]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN19 = 25, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[19]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN20 = 26, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[20]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN21 = 27, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[21]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN22 = 28, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[22]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN23 = 29, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[23]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN24 = 30, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[24]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN25 = 31, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[25]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN26 = 32, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[26]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN27 = 33, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[27]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN28 = 34, //!< SAR to PDMA1 direct connect - cpuss.dw0_tr_in[28]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN0 = 35, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[0]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN1 = 36, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[1]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN2 = 37, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[2]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN3 = 38, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[3]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN4 = 39, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[4]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN5 = 40, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[5]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN6 = 41, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[6]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN7 = 42, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[7]
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
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN24 = 59, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[24]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN25 = 60, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[25]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN26 = 61, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[26]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN27 = 62, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[27]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN28 = 63, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[28]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN29 = 64, //!< CAN PDMA1 triggers - cpuss.dw1_tr_in[29]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN30 = 65, //!< CAN PDMA1 triggers - cpuss.dw1_tr_in[30]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN31 = 66, //!< CAN PDMA1 triggers - cpuss.dw1_tr_in[31]
    CYHAL_TRIGGER_CSD_DSI_START = 67, //!< Capsense trigger multiplexer - csd.dsi_start
    CYHAL_TRIGGER_PASS_TR_SAR_IN = 68, //!< ADC trigger multiplexer - pass.tr_sar_in
    CYHAL_TRIGGER_PERI_TR_DBG_FREEZE = 69, //!< PERI Freeze trigger multiplexer - peri.tr_dbg_freeze
    CYHAL_TRIGGER_PERI_TR_IO_OUTPUT0 = 70, //!< HSIOM trigger multiplexer - peri.tr_io_output[0]
    CYHAL_TRIGGER_PERI_TR_IO_OUTPUT1 = 71, //!< HSIOM trigger multiplexer - peri.tr_io_output[1]
    CYHAL_TRIGGER_TCPWM0_TR_IN0 = 72, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[0]
    CYHAL_TRIGGER_TCPWM0_TR_IN1 = 73, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[1]
    CYHAL_TRIGGER_TCPWM0_TR_IN2 = 74, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[2]
    CYHAL_TRIGGER_TCPWM0_TR_IN3 = 75, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[3]
    CYHAL_TRIGGER_TCPWM0_TR_IN4 = 76, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[4]
    CYHAL_TRIGGER_TCPWM0_TR_IN5 = 77, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[5]
    CYHAL_TRIGGER_TCPWM0_TR_IN6 = 78, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[6]
    CYHAL_TRIGGER_TCPWM0_TR_IN7 = 79, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[7]
    CYHAL_TRIGGER_TCPWM0_TR_IN8 = 80, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[8]
    CYHAL_TRIGGER_TCPWM0_TR_IN9 = 81, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[9]
    CYHAL_TRIGGER_TCPWM0_TR_IN10 = 82, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[10]
    CYHAL_TRIGGER_TCPWM0_TR_IN11 = 83, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[11]
    CYHAL_TRIGGER_TCPWM0_TR_IN12 = 84, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[12]
    CYHAL_TRIGGER_TCPWM0_TR_IN13 = 85, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[13]
    CYHAL_TRIGGER_TCPWM1_TR_IN0 = 86, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[0]
    CYHAL_TRIGGER_TCPWM1_TR_IN1 = 87, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[1]
    CYHAL_TRIGGER_TCPWM1_TR_IN2 = 88, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[2]
    CYHAL_TRIGGER_TCPWM1_TR_IN3 = 89, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[3]
    CYHAL_TRIGGER_TCPWM1_TR_IN4 = 90, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[4]
    CYHAL_TRIGGER_TCPWM1_TR_IN5 = 91, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[5]
    CYHAL_TRIGGER_TCPWM1_TR_IN6 = 92, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[6]
    CYHAL_TRIGGER_TCPWM1_TR_IN7 = 93, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[7]
    CYHAL_TRIGGER_TCPWM1_TR_IN8 = 94, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[8]
    CYHAL_TRIGGER_TCPWM1_TR_IN9 = 95, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[9]
    CYHAL_TRIGGER_TCPWM1_TR_IN10 = 96, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[10]
    CYHAL_TRIGGER_TCPWM1_TR_IN11 = 97, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[11]
    CYHAL_TRIGGER_TCPWM1_TR_IN12 = 98, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[12]
    CYHAL_TRIGGER_TCPWM1_TR_IN13 = 99, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[13]
    CYHAL_TRIGGER_USB_DMA_BURSTEND0 = 100, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[0]
    CYHAL_TRIGGER_USB_DMA_BURSTEND1 = 101, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[1]
    CYHAL_TRIGGER_USB_DMA_BURSTEND2 = 102, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[2]
    CYHAL_TRIGGER_USB_DMA_BURSTEND3 = 103, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[3]
    CYHAL_TRIGGER_USB_DMA_BURSTEND4 = 104, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[4]
    CYHAL_TRIGGER_USB_DMA_BURSTEND5 = 105, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[5]
    CYHAL_TRIGGER_USB_DMA_BURSTEND6 = 106, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[6]
    CYHAL_TRIGGER_USB_DMA_BURSTEND7 = 107, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[7]
} cyhal_trigger_dest_psoc6_03_t;

/** Typedef from device family specific trigger dest to generic trigger dest */
typedef cyhal_trigger_dest_psoc6_03_t cyhal_dest_t;

/** \cond INTERNAL */
/** Table of number of inputs to each mux. */
extern const uint16_t cyhal_sources_per_mux[19];

/** Table indicating whether mux is 1to1. */
extern const bool cyhal_is_mux_1to1[19];

/** Table pointing to each mux source table. The index of each source in the table is its mux input index. */
extern const cyhal_source_t* cyhal_mux_to_sources [19];

/** Maps each cyhal_destination_t to a mux index.
 * If bit 8 of the mux index is set, this denotes that the trigger is a
 * one to one trigger.
 */
extern const uint8_t cyhal_dest_to_mux[108];

/* Maps each cyhal_destination_t to a specific output in its mux */
extern const uint8_t cyhal_mux_dest_index[108];
/** \endcond */

#if defined(__cplusplus)
}
#endif /* __cplusplus */
/** \} group_hal_impl_triggers_psoc6_03 */
#endif /* _CYHAL_TRIGGERS_PSOC6_03_H_ */


/* [] END OF FILE */
