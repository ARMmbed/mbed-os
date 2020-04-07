/***************************************************************************//**
* \file cyhal_triggers_psoc6_03.h
*
* \brief
* PSoC6_03 family HAL triggers header
*
* \note
* Generator version: 1.5.7254.19579
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

#ifndef _CYHAL_TRIGGERS_PSOC6_03_H_
#define _CYHAL_TRIGGERS_PSOC6_03_H_

/**
 * \addtogroup group_hal_psoc6_triggers_psoc6_03 PSOC6_03
 * \ingroup group_hal_psoc6_triggers
 * \{
 * Trigger connections for psoc6_03
 */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/** @brief Name of each output trigger. */
typedef enum
{
    TRIGGER_CANFD0_TR_DBG_DMA_ACK0 = 0, //!< CAN DW0 triggers (from DW back to CAN) - canfd[0].tr_dbg_dma_ack[0]
    TRIGGER_CANFD0_TR_EVT_SWT_IN0 = 1, //!< CAN TT Sync - canfd[0].tr_evt_swt_in[0]
    TRIGGER_CPUSS_CTI_TR_IN0 = 2, //!< CPUSS Debug trigger multiplexer - cpuss.cti_tr_in[0]
    TRIGGER_CPUSS_CTI_TR_IN1 = 3, //!< CPUSS Debug trigger multiplexer - cpuss.cti_tr_in[1]
    TRIGGER_CPUSS_DMAC_TR_IN0 = 4, //!< MDMA trigger multiplexer - cpuss.dmac_tr_in[0]
    TRIGGER_CPUSS_DMAC_TR_IN1 = 5, //!< MDMA trigger multiplexer - cpuss.dmac_tr_in[1]
    TRIGGER_CPUSS_DW0_TR_IN0 = 6, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[0]
    TRIGGER_CPUSS_DW0_TR_IN1 = 7, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[1]
    TRIGGER_CPUSS_DW0_TR_IN2 = 8, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[2]
    TRIGGER_CPUSS_DW0_TR_IN3 = 9, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[3]
    TRIGGER_CPUSS_DW0_TR_IN4 = 10, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[4]
    TRIGGER_CPUSS_DW0_TR_IN5 = 11, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[5]
    TRIGGER_CPUSS_DW0_TR_IN6 = 12, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[6]
    TRIGGER_CPUSS_DW0_TR_IN7 = 13, //!< PDMA0 trigger multiplexer - cpuss.dw0_tr_in[7]
    TRIGGER_CPUSS_DW0_TR_IN8 = 14, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[8]
    TRIGGER_CPUSS_DW0_TR_IN9 = 15, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[9]
    TRIGGER_CPUSS_DW0_TR_IN10 = 16, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[10]
    TRIGGER_CPUSS_DW0_TR_IN11 = 17, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[11]
    TRIGGER_CPUSS_DW0_TR_IN12 = 18, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[12]
    TRIGGER_CPUSS_DW0_TR_IN13 = 19, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[13]
    TRIGGER_CPUSS_DW0_TR_IN14 = 20, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[14]
    TRIGGER_CPUSS_DW0_TR_IN15 = 21, //!< USB PDMA0 Triggers - cpuss.dw0_tr_in[15]
    TRIGGER_CPUSS_DW0_TR_IN16 = 22, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[16]
    TRIGGER_CPUSS_DW0_TR_IN17 = 23, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[17]
    TRIGGER_CPUSS_DW0_TR_IN18 = 24, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[18]
    TRIGGER_CPUSS_DW0_TR_IN19 = 25, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[19]
    TRIGGER_CPUSS_DW0_TR_IN20 = 26, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[20]
    TRIGGER_CPUSS_DW0_TR_IN21 = 27, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[21]
    TRIGGER_CPUSS_DW0_TR_IN22 = 28, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[22]
    TRIGGER_CPUSS_DW0_TR_IN23 = 29, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[23]
    TRIGGER_CPUSS_DW0_TR_IN24 = 30, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[24]
    TRIGGER_CPUSS_DW0_TR_IN25 = 31, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[25]
    TRIGGER_CPUSS_DW0_TR_IN26 = 32, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[26]
    TRIGGER_CPUSS_DW0_TR_IN27 = 33, //!< SCB PDMA0 Triggers - cpuss.dw0_tr_in[27]
    TRIGGER_CPUSS_DW0_TR_IN28 = 34, //!< SAR to PDMA1 direct connect - cpuss.dw0_tr_in[28]
    TRIGGER_CPUSS_DW1_TR_IN0 = 35, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[0]
    TRIGGER_CPUSS_DW1_TR_IN1 = 36, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[1]
    TRIGGER_CPUSS_DW1_TR_IN2 = 37, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[2]
    TRIGGER_CPUSS_DW1_TR_IN3 = 38, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[3]
    TRIGGER_CPUSS_DW1_TR_IN4 = 39, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[4]
    TRIGGER_CPUSS_DW1_TR_IN5 = 40, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[5]
    TRIGGER_CPUSS_DW1_TR_IN6 = 41, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[6]
    TRIGGER_CPUSS_DW1_TR_IN7 = 42, //!< PDMA1 trigger multiplexer - cpuss.dw1_tr_in[7]
    TRIGGER_CPUSS_DW1_TR_IN8 = 43, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[8]
    TRIGGER_CPUSS_DW1_TR_IN9 = 44, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[9]
    TRIGGER_CPUSS_DW1_TR_IN10 = 45, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[10]
    TRIGGER_CPUSS_DW1_TR_IN11 = 46, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[11]
    TRIGGER_CPUSS_DW1_TR_IN12 = 47, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[12]
    TRIGGER_CPUSS_DW1_TR_IN13 = 48, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[13]
    TRIGGER_CPUSS_DW1_TR_IN14 = 49, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[14]
    TRIGGER_CPUSS_DW1_TR_IN15 = 50, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[15]
    TRIGGER_CPUSS_DW1_TR_IN16 = 51, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[16]
    TRIGGER_CPUSS_DW1_TR_IN17 = 52, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[17]
    TRIGGER_CPUSS_DW1_TR_IN18 = 53, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[18]
    TRIGGER_CPUSS_DW1_TR_IN19 = 54, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[19]
    TRIGGER_CPUSS_DW1_TR_IN20 = 55, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[20]
    TRIGGER_CPUSS_DW1_TR_IN21 = 56, //!< SCB PDMA1 Triggers - cpuss.dw1_tr_in[21]
    TRIGGER_CPUSS_DW1_TR_IN22 = 57, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[22]
    TRIGGER_CPUSS_DW1_TR_IN23 = 58, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[23]
    TRIGGER_CPUSS_DW1_TR_IN24 = 59, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[24]
    TRIGGER_CPUSS_DW1_TR_IN25 = 60, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[25]
    TRIGGER_CPUSS_DW1_TR_IN26 = 61, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[26]
    TRIGGER_CPUSS_DW1_TR_IN27 = 62, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[27]
    TRIGGER_CPUSS_DW1_TR_IN28 = 63, //!< SMIF to PDMA1 direct connect - cpuss.dw1_tr_in[28]
    TRIGGER_CPUSS_DW1_TR_IN29 = 64, //!< CAN PDMA1 triggers - cpuss.dw1_tr_in[29]
    TRIGGER_CPUSS_DW1_TR_IN30 = 65, //!< CAN PDMA1 triggers - cpuss.dw1_tr_in[30]
    TRIGGER_CPUSS_DW1_TR_IN31 = 66, //!< CAN PDMA1 triggers - cpuss.dw1_tr_in[31]
    TRIGGER_CSD_DSI_START = 67, //!< Capsense trigger multiplexer - csd.dsi_start
    TRIGGER_PASS_TR_SAR_IN = 68, //!< ADC trigger multiplexer - pass.tr_sar_in
    TRIGGER_PERI_TR_DBG_FREEZE = 69, //!< PERI Freeze trigger multiplexer - peri.tr_dbg_freeze
    TRIGGER_PERI_TR_IO_OUTPUT0 = 70, //!< HSIOM trigger multiplexer - peri.tr_io_output[0]
    TRIGGER_PERI_TR_IO_OUTPUT1 = 71, //!< HSIOM trigger multiplexer - peri.tr_io_output[1]
    TRIGGER_TCPWM0_TR_IN0 = 72, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[0]
    TRIGGER_TCPWM0_TR_IN1 = 73, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[1]
    TRIGGER_TCPWM0_TR_IN2 = 74, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[2]
    TRIGGER_TCPWM0_TR_IN3 = 75, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[3]
    TRIGGER_TCPWM0_TR_IN4 = 76, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[4]
    TRIGGER_TCPWM0_TR_IN5 = 77, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[5]
    TRIGGER_TCPWM0_TR_IN6 = 78, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[6]
    TRIGGER_TCPWM0_TR_IN7 = 79, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[7]
    TRIGGER_TCPWM0_TR_IN8 = 80, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[8]
    TRIGGER_TCPWM0_TR_IN9 = 81, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[9]
    TRIGGER_TCPWM0_TR_IN10 = 82, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[10]
    TRIGGER_TCPWM0_TR_IN11 = 83, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[11]
    TRIGGER_TCPWM0_TR_IN12 = 84, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[12]
    TRIGGER_TCPWM0_TR_IN13 = 85, //!< TCPWM0 trigger multiplexer - tcpwm[0].tr_in[13]
    TRIGGER_TCPWM1_TR_IN0 = 86, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[0]
    TRIGGER_TCPWM1_TR_IN1 = 87, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[1]
    TRIGGER_TCPWM1_TR_IN2 = 88, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[2]
    TRIGGER_TCPWM1_TR_IN3 = 89, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[3]
    TRIGGER_TCPWM1_TR_IN4 = 90, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[4]
    TRIGGER_TCPWM1_TR_IN5 = 91, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[5]
    TRIGGER_TCPWM1_TR_IN6 = 92, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[6]
    TRIGGER_TCPWM1_TR_IN7 = 93, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[7]
    TRIGGER_TCPWM1_TR_IN8 = 94, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[8]
    TRIGGER_TCPWM1_TR_IN9 = 95, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[9]
    TRIGGER_TCPWM1_TR_IN10 = 96, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[10]
    TRIGGER_TCPWM1_TR_IN11 = 97, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[11]
    TRIGGER_TCPWM1_TR_IN12 = 98, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[12]
    TRIGGER_TCPWM1_TR_IN13 = 99, //!< TCPWM1 trigger multiplexer - tcpwm[1].tr_in[13]
    TRIGGER_USB_DMA_BURSTEND0 = 100, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[0]
    TRIGGER_USB_DMA_BURSTEND1 = 101, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[1]
    TRIGGER_USB_DMA_BURSTEND2 = 102, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[2]
    TRIGGER_USB_DMA_BURSTEND3 = 103, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[3]
    TRIGGER_USB_DMA_BURSTEND4 = 104, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[4]
    TRIGGER_USB_DMA_BURSTEND5 = 105, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[5]
    TRIGGER_USB_DMA_BURSTEND6 = 106, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[6]
    TRIGGER_USB_DMA_BURSTEND7 = 107, //!< USB PDMA0 Acknowledge Triggers - usb.dma_burstend[7]
} cyhal_trigger_dest_psoc6_03_t;

/** Typedef from device family specific trigger dest to generic trigger dest */
typedef cyhal_trigger_dest_psoc6_03_t cyhal_dest_t;

/** \cond INTERNAL */
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
/** \} group_hal_psoc6_triggers_psoc6_03 */
#endif /* _CYHAL_TRIGGERS_PSOC6_03_H_ */


/* [] END OF FILE */
