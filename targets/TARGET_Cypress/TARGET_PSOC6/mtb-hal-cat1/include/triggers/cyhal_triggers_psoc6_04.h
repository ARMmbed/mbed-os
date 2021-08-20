/***************************************************************************//**
* \file cyhal_triggers_psoc6_04.h
*
* \brief
* PSoC6_04 family HAL triggers header
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

#ifndef _CYHAL_TRIGGERS_PSOC6_04_H_
#define _CYHAL_TRIGGERS_PSOC6_04_H_

/**
 * \addtogroup group_hal_impl_triggers_psoc6_04 PSOC6_04
 * \ingroup group_hal_impl_triggers
 * \{
 * Trigger connections for psoc6_04
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
    CYHAL_TRIGGER_CPUSS_DW0_TR_OUT29 = 38, //!< cpuss.dw0_tr_out[29]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT0 = 39, //!< cpuss.dw1_tr_out[0]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT1 = 40, //!< cpuss.dw1_tr_out[1]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT2 = 41, //!< cpuss.dw1_tr_out[2]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT3 = 42, //!< cpuss.dw1_tr_out[3]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT4 = 43, //!< cpuss.dw1_tr_out[4]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT5 = 44, //!< cpuss.dw1_tr_out[5]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT6 = 45, //!< cpuss.dw1_tr_out[6]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT7 = 46, //!< cpuss.dw1_tr_out[7]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT8 = 47, //!< cpuss.dw1_tr_out[8]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT9 = 48, //!< cpuss.dw1_tr_out[9]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT10 = 49, //!< cpuss.dw1_tr_out[10]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT11 = 50, //!< cpuss.dw1_tr_out[11]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT12 = 51, //!< cpuss.dw1_tr_out[12]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT13 = 52, //!< cpuss.dw1_tr_out[13]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT14 = 53, //!< cpuss.dw1_tr_out[14]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT15 = 54, //!< cpuss.dw1_tr_out[15]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT16 = 55, //!< cpuss.dw1_tr_out[16]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT17 = 56, //!< cpuss.dw1_tr_out[17]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT18 = 57, //!< cpuss.dw1_tr_out[18]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT19 = 58, //!< cpuss.dw1_tr_out[19]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT20 = 59, //!< cpuss.dw1_tr_out[20]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT21 = 60, //!< cpuss.dw1_tr_out[21]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT22 = 61, //!< cpuss.dw1_tr_out[22]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT23 = 62, //!< cpuss.dw1_tr_out[23]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT24 = 63, //!< cpuss.dw1_tr_out[24]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT25 = 64, //!< cpuss.dw1_tr_out[25]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT26 = 65, //!< cpuss.dw1_tr_out[26]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT27 = 66, //!< cpuss.dw1_tr_out[27]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT28 = 67, //!< cpuss.dw1_tr_out[28]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT29 = 68, //!< cpuss.dw1_tr_out[29]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT30 = 69, //!< cpuss.dw1_tr_out[30]
    CYHAL_TRIGGER_CPUSS_DW1_TR_OUT31 = 70, //!< cpuss.dw1_tr_out[31]
    CYHAL_TRIGGER_CPUSS_TR_FAULT0 = 71, //!< cpuss.tr_fault[0]
    CYHAL_TRIGGER_CPUSS_TR_FAULT1 = 72, //!< cpuss.tr_fault[1]
    CYHAL_TRIGGER_CSD_DSI_SAMPLE_OUT = 73, //!< csd.dsi_sample_out
    CYHAL_TRIGGER_CSD_DSI_SENSE_OUT = 74, //!< csd.dsi_sense_out
    CYHAL_TRIGGER_CSD_TR_ADC_DONE = 75, //!< csd.tr_adc_done
    CYHAL_TRIGGER_LPCOMP_DSI_COMP0 = 76, //!< lpcomp.dsi_comp0
    CYHAL_TRIGGER_LPCOMP_DSI_COMP1 = 77, //!< lpcomp.dsi_comp1
    CYHAL_TRIGGER_PASS_DSI_CTB_CMP0 = 78, //!< pass.dsi_ctb_cmp0
    CYHAL_TRIGGER_PASS_DSI_CTB_CMP1 = 79, //!< pass.dsi_ctb_cmp1
    CYHAL_TRIGGER_PASS_TR_CTDAC_EMPTY = 80, //!< pass.tr_ctdac_empty
    CYHAL_TRIGGER_PASS_TR_SAR_OUT0 = 81, //!< pass.tr_sar_out[0]
    CYHAL_TRIGGER_PASS_TR_SAR_OUT1 = 82, //!< pass.tr_sar_out[1]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT0 = 83, //!< peri.tr_io_input[0]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT1 = 84, //!< peri.tr_io_input[1]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT2 = 85, //!< peri.tr_io_input[2]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT3 = 86, //!< peri.tr_io_input[3]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT4 = 87, //!< peri.tr_io_input[4]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT5 = 88, //!< peri.tr_io_input[5]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT6 = 89, //!< peri.tr_io_input[6]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT7 = 90, //!< peri.tr_io_input[7]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT8 = 91, //!< peri.tr_io_input[8]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT9 = 92, //!< peri.tr_io_input[9]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT10 = 93, //!< peri.tr_io_input[10]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT11 = 94, //!< peri.tr_io_input[11]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT12 = 95, //!< peri.tr_io_input[12]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT13 = 96, //!< peri.tr_io_input[13]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT14 = 97, //!< peri.tr_io_input[14]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT15 = 98, //!< peri.tr_io_input[15]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT16 = 99, //!< peri.tr_io_input[16]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT17 = 100, //!< peri.tr_io_input[17]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT18 = 101, //!< peri.tr_io_input[18]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT19 = 102, //!< peri.tr_io_input[19]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT20 = 103, //!< peri.tr_io_input[20]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT21 = 104, //!< peri.tr_io_input[21]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT22 = 105, //!< peri.tr_io_input[22]
    CYHAL_TRIGGER_PERI_TR_IO_INPUT23 = 106, //!< peri.tr_io_input[23]
    CYHAL_TRIGGER_SCB0_TR_I2C_SCL_FILTERED = 107, //!< scb[0].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB1_TR_I2C_SCL_FILTERED = 108, //!< scb[1].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB2_TR_I2C_SCL_FILTERED = 109, //!< scb[2].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB4_TR_I2C_SCL_FILTERED = 110, //!< scb[4].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB5_TR_I2C_SCL_FILTERED = 111, //!< scb[5].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB6_TR_I2C_SCL_FILTERED = 112, //!< scb[6].tr_i2c_scl_filtered
    CYHAL_TRIGGER_SCB0_TR_RX_REQ = 113, //!< scb[0].tr_rx_req
    CYHAL_TRIGGER_SCB1_TR_RX_REQ = 114, //!< scb[1].tr_rx_req
    CYHAL_TRIGGER_SCB2_TR_RX_REQ = 115, //!< scb[2].tr_rx_req
    CYHAL_TRIGGER_SCB4_TR_RX_REQ = 116, //!< scb[4].tr_rx_req
    CYHAL_TRIGGER_SCB5_TR_RX_REQ = 117, //!< scb[5].tr_rx_req
    CYHAL_TRIGGER_SCB6_TR_RX_REQ = 118, //!< scb[6].tr_rx_req
    CYHAL_TRIGGER_SCB0_TR_TX_REQ = 119, //!< scb[0].tr_tx_req
    CYHAL_TRIGGER_SCB1_TR_TX_REQ = 120, //!< scb[1].tr_tx_req
    CYHAL_TRIGGER_SCB2_TR_TX_REQ = 121, //!< scb[2].tr_tx_req
    CYHAL_TRIGGER_SCB4_TR_TX_REQ = 122, //!< scb[4].tr_tx_req
    CYHAL_TRIGGER_SCB5_TR_TX_REQ = 123, //!< scb[5].tr_tx_req
    CYHAL_TRIGGER_SCB6_TR_TX_REQ = 124, //!< scb[6].tr_tx_req
    CYHAL_TRIGGER_SMIF_TR_RX_REQ = 125, //!< smif.tr_rx_req
    CYHAL_TRIGGER_SMIF_TR_TX_REQ = 126, //!< smif.tr_tx_req
    CYHAL_TRIGGER_TCPWM0_TR_OUT00 = 127, //!< tcpwm[0].tr_out0[0]
    CYHAL_TRIGGER_TCPWM0_TR_OUT01 = 128, //!< tcpwm[0].tr_out0[1]
    CYHAL_TRIGGER_TCPWM0_TR_OUT02 = 129, //!< tcpwm[0].tr_out0[2]
    CYHAL_TRIGGER_TCPWM0_TR_OUT03 = 130, //!< tcpwm[0].tr_out0[3]
    CYHAL_TRIGGER_TCPWM0_TR_OUT0256 = 131, //!< tcpwm[0].tr_out0[256]
    CYHAL_TRIGGER_TCPWM0_TR_OUT0257 = 132, //!< tcpwm[0].tr_out0[257]
    CYHAL_TRIGGER_TCPWM0_TR_OUT0258 = 133, //!< tcpwm[0].tr_out0[258]
    CYHAL_TRIGGER_TCPWM0_TR_OUT0259 = 134, //!< tcpwm[0].tr_out0[259]
    CYHAL_TRIGGER_TCPWM0_TR_OUT0260 = 135, //!< tcpwm[0].tr_out0[260]
    CYHAL_TRIGGER_TCPWM0_TR_OUT0261 = 136, //!< tcpwm[0].tr_out0[261]
    CYHAL_TRIGGER_TCPWM0_TR_OUT0262 = 137, //!< tcpwm[0].tr_out0[262]
    CYHAL_TRIGGER_TCPWM0_TR_OUT0263 = 138, //!< tcpwm[0].tr_out0[263]
    CYHAL_TRIGGER_TCPWM0_TR_OUT10 = 139, //!< tcpwm[0].tr_out1[0]
    CYHAL_TRIGGER_TCPWM0_TR_OUT11 = 140, //!< tcpwm[0].tr_out1[1]
    CYHAL_TRIGGER_TCPWM0_TR_OUT12 = 141, //!< tcpwm[0].tr_out1[2]
    CYHAL_TRIGGER_TCPWM0_TR_OUT13 = 142, //!< tcpwm[0].tr_out1[3]
    CYHAL_TRIGGER_TCPWM0_TR_OUT1256 = 143, //!< tcpwm[0].tr_out1[256]
    CYHAL_TRIGGER_TCPWM0_TR_OUT1257 = 144, //!< tcpwm[0].tr_out1[257]
    CYHAL_TRIGGER_TCPWM0_TR_OUT1258 = 145, //!< tcpwm[0].tr_out1[258]
    CYHAL_TRIGGER_TCPWM0_TR_OUT1259 = 146, //!< tcpwm[0].tr_out1[259]
    CYHAL_TRIGGER_TCPWM0_TR_OUT1260 = 147, //!< tcpwm[0].tr_out1[260]
    CYHAL_TRIGGER_TCPWM0_TR_OUT1261 = 148, //!< tcpwm[0].tr_out1[261]
    CYHAL_TRIGGER_TCPWM0_TR_OUT1262 = 149, //!< tcpwm[0].tr_out1[262]
    CYHAL_TRIGGER_TCPWM0_TR_OUT1263 = 150, //!< tcpwm[0].tr_out1[263]
    CYHAL_TRIGGER_USB_DMA_REQ0 = 151, //!< usb.dma_req[0]
    CYHAL_TRIGGER_USB_DMA_REQ1 = 152, //!< usb.dma_req[1]
    CYHAL_TRIGGER_USB_DMA_REQ2 = 153, //!< usb.dma_req[2]
    CYHAL_TRIGGER_USB_DMA_REQ3 = 154, //!< usb.dma_req[3]
    CYHAL_TRIGGER_USB_DMA_REQ4 = 155, //!< usb.dma_req[4]
    CYHAL_TRIGGER_USB_DMA_REQ5 = 156, //!< usb.dma_req[5]
    CYHAL_TRIGGER_USB_DMA_REQ6 = 157, //!< usb.dma_req[6]
    CYHAL_TRIGGER_USB_DMA_REQ7 = 158, //!< usb.dma_req[7]
} cyhal_trigger_source_psoc6_04_t;

/** Typedef from device family specific trigger source to generic trigger source */
typedef cyhal_trigger_source_psoc6_04_t cyhal_source_t;

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
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN28 = 34, //!< SAR0 to PDMA1 direct connect - cpuss.dw0_tr_in[28]
    CYHAL_TRIGGER_CPUSS_DW0_TR_IN29 = 35, //!< SAR1 to PDMA1 direct connect - cpuss.dw0_tr_in[29]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN0 = 36, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[0]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN1 = 37, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[1]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN2 = 38, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[2]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN3 = 39, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[3]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN4 = 40, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[4]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN5 = 41, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[5]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN6 = 42, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[6]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN7 = 43, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[7]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN8 = 44, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[8]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN9 = 45, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[9]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN10 = 46, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[10]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN11 = 47, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[11]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN12 = 48, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[12]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN13 = 49, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[13]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN14 = 50, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[14]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN15 = 51, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[15]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN16 = 52, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[16]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN17 = 53, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[17]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN18 = 54, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[18]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN19 = 55, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[19]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN20 = 56, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[20]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN21 = 57, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[21]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN22 = 58, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[22]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN23 = 59, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[23]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN24 = 60, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[24]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN25 = 61, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[25]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN26 = 62, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[26]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN27 = 63, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[27]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN28 = 64, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[28]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN29 = 65, //!< CAN PDMA1 triggers - cpuss.dw1_tr_in[29]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN30 = 66, //!< CAN PDMA1 triggers - cpuss.dw1_tr_in[30]
    CYHAL_TRIGGER_CPUSS_DW1_TR_IN31 = 67, //!< CAN PDMA1 triggers - cpuss.dw1_tr_in[31]
    CYHAL_TRIGGER_CSD_DSI_START = 68, //!< Capsense trigger multiplexer - csd.dsi_start
    CYHAL_TRIGGER_PASS_DSI_CTDAC_STROBE = 69, //!< CTDAC trigger multiplexer - pass.dsi_ctdac_strobe
    CYHAL_TRIGGER_PASS_TR_SAR_IN0 = 70, //!< ADC trigger multiplexer - pass.tr_sar_in[0]
    CYHAL_TRIGGER_PASS_TR_SAR_IN1 = 71, //!< ADC trigger multiplexer - pass.tr_sar_in[1]
    CYHAL_TRIGGER_PERI_TR_DBG_FREEZE = 72, //!< PERI Freeze trigger multiplexer - peri.tr_dbg_freeze
    CYHAL_TRIGGER_PERI_TR_IO_OUTPUT0 = 73, //!< HSIOM trigger multiplexer - peri.tr_io_output[0]
    CYHAL_TRIGGER_PERI_TR_IO_OUTPUT1 = 74, //!< HSIOM trigger multiplexer - peri.tr_io_output[1]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN0 = 75, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_all_cnt_in[0]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN1 = 76, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_all_cnt_in[1]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN2 = 77, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_all_cnt_in[2]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN3 = 78, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_all_cnt_in[3]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN4 = 79, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_all_cnt_in[4]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN5 = 80, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_all_cnt_in[5]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN6 = 81, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_all_cnt_in[6]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN7 = 82, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_all_cnt_in[7]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN8 = 83, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_all_cnt_in[8]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN9 = 84, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_all_cnt_in[9]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN10 = 85, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_all_cnt_in[10]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN11 = 86, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_all_cnt_in[11]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN12 = 87, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_all_cnt_in[12]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN13 = 88, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_all_cnt_in[13]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN14 = 89, //!< TCPWM0 trigger multiplexer - 2nd - tcpwm[0].tr_all_cnt_in[14]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN15 = 90, //!< TCPWM0 trigger multiplexer - 2nd - tcpwm[0].tr_all_cnt_in[15]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN16 = 91, //!< TCPWM0 trigger multiplexer - 2nd - tcpwm[0].tr_all_cnt_in[16]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN17 = 92, //!< TCPWM0 trigger multiplexer - 2nd - tcpwm[0].tr_all_cnt_in[17]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN18 = 93, //!< TCPWM0 trigger multiplexer - 2nd - tcpwm[0].tr_all_cnt_in[18]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN19 = 94, //!< TCPWM0 trigger multiplexer - 2nd - tcpwm[0].tr_all_cnt_in[19]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN20 = 95, //!< TCPWM0 trigger multiplexer - 2nd - tcpwm[0].tr_all_cnt_in[20]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN21 = 96, //!< TCPWM0 trigger multiplexer - 2nd - tcpwm[0].tr_all_cnt_in[21]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN22 = 97, //!< TCPWM0 trigger multiplexer - 2nd - tcpwm[0].tr_all_cnt_in[22]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN23 = 98, //!< TCPWM0 trigger multiplexer - 2nd - tcpwm[0].tr_all_cnt_in[23]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN24 = 99, //!< TCPWM0 trigger multiplexer - 2nd - tcpwm[0].tr_all_cnt_in[24]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN25 = 100, //!< TCPWM0 trigger multiplexer - 2nd - tcpwm[0].tr_all_cnt_in[25]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN26 = 101, //!< TCPWM0 trigger multiplexer - 2nd - tcpwm[0].tr_all_cnt_in[26]
    CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN27 = 102, //!< TCPWM0 trigger multiplexer - 2nd - tcpwm[0].tr_all_cnt_in[27]
    CYHAL_TRIGGER_TCPWM0_TR_DEBUG_FREEZE = 103, //!< PERI Freeze trigger multiplexer - tcpwm[0].tr_debug_freeze
    CYHAL_TRIGGER_USB_DMA_BURSTEND0 = 104, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[0]
    CYHAL_TRIGGER_USB_DMA_BURSTEND1 = 105, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[1]
    CYHAL_TRIGGER_USB_DMA_BURSTEND2 = 106, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[2]
    CYHAL_TRIGGER_USB_DMA_BURSTEND3 = 107, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[3]
    CYHAL_TRIGGER_USB_DMA_BURSTEND4 = 108, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[4]
    CYHAL_TRIGGER_USB_DMA_BURSTEND5 = 109, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[5]
    CYHAL_TRIGGER_USB_DMA_BURSTEND6 = 110, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[6]
    CYHAL_TRIGGER_USB_DMA_BURSTEND7 = 111, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[7]
} cyhal_trigger_dest_psoc6_04_t;

/** Typedef from device family specific trigger dest to generic trigger dest */
typedef cyhal_trigger_dest_psoc6_04_t cyhal_dest_t;

/** \cond INTERNAL */
/** Table of number of inputs to each mux. */
extern const uint16_t cyhal_sources_per_mux[21];

/** Table indicating whether mux is 1to1. */
extern const bool cyhal_is_mux_1to1[21];

/** Table pointing to each mux source table. The index of each source in the table is its mux input index. */
extern const cyhal_source_t* cyhal_mux_to_sources [21];

/** Maps each cyhal_destination_t to a mux index.
 * If bit 8 of the mux index is set, this denotes that the trigger is a
 * one to one trigger.
 */
extern const uint8_t cyhal_dest_to_mux[112];

/* Maps each cyhal_destination_t to a specific output in its mux */
extern const uint8_t cyhal_mux_dest_index[112];
/** \endcond */

#if defined(__cplusplus)
}
#endif /* __cplusplus */
/** \} group_hal_impl_triggers_psoc6_04 */
#endif /* _CYHAL_TRIGGERS_PSOC6_04_H_ */


/* [] END OF FILE */
