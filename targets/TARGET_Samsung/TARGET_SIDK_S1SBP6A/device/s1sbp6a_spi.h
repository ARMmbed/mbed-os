/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/


#ifndef __S1SBP6A_SPI_H
#define __S1SBP6A_SPI_H

#define SSP_CR0_OFFSET               (0x0000) /* Control Register 0 */
#define SSP_CR1_OFFSET               (0x0004) /* Control Register 1 */
#define SSP_DR_OFFSET                (0x0008) /* Data Register */
#define SSP_SR_OFFSET                (0x000c) /* Status Register */
#define SSP_CPSR_OFFSET              (0x0010) /* Clock Prescale Register */
#define SSP_IMSC_OFFSET              (0x0014) /* Interrupt Mask Set and Clear Register */
#define SSP_RIS_OFFSET               (0x0018) /* Raw Interrupt Status Register */
#define SSP_MIS_OFFSET               (0x001c) /* Masked Interrupt Status Register */
#define SSP_ICR_OFFSET               (0x0020) /* Interrupt Clear Register */
#define SSP_DMACR_OFFSET             (0x0024) /* DMA Control Register */

/* Control Register 0 */
#define SSP_CR0_DSS_SHIFT            (0)
#define SSP_CR0_FRF_SHIFT            (4)
#define SSP_CR0_SPO_SHIFT            (6)
#define SSP_CR0_SPH_SHIFT            (7)
#define SSP_CR0_SCR_SHIFT            (8)

#define SSP_CR0_DSS_MASK             (0x0f << SSP_CR0_DSS_SHIFT)
#define SSP_CR0_FRF_MASK             (0x03 << SSP_CR0_FRF_SHIFT)
#define SSP_CR0_SPO_MASK             (0x01 << SSP_CR0_SPO_SHIFT)
#define SSP_CR0_SPH_MASK             (0x01 << SSP_CR0_SPH_SHIFT)
#define SSP_CR0_SCR_MASK             (0xff << SSP_CR0_SCR_SHIFT)

#define SSP_CR0_DSS(c)               ((c) << SSP_CR0_DSS_SHIFT)
#define SSP_CR0_FRF(c)               ((c) << SSP_CR0_FRF_SHIFT)
#define SSP_CR0_SPO(c)               ((c) << SSP_CR0_SPO_SHIFT)
#define SSP_CR0_SPH(c)               ((c) << SSP_CR0_SPH_SHIFT)
#define SSP_CR0_SCR(c)               ((c) << SSP_CR0_SCR_SHIFT)

#define SSP_CR0_DSS_4BIT             (3 << SSP_CR0_DSS_SHIFT)
#define SSP_CR0_DSS_5BIT             (4 << SSP_CR0_DSS_SHIFT)
#define SSP_CR0_DSS_6BIT             (5 << SSP_CR0_DSS_SHIFT)
#define SSP_CR0_DSS_7BIT             (6 << SSP_CR0_DSS_SHIFT)
#define SSP_CR0_DSS_8BIT             (7 << SSP_CR0_DSS_SHIFT)
#define SSP_CR0_DSS_9BIT             (8 << SSP_CR0_DSS_SHIFT)
#define SSP_CR0_DSS_10BIT            (9 << SSP_CR0_DSS_SHIFT)
#define SSP_CR0_DSS_11BIT            (10 << SSP_CR0_DSS_SHIFT)
#define SSP_CR0_DSS_12BIT            (11 << SSP_CR0_DSS_SHIFT)
#define SSP_CR0_DSS_13BIT            (12 << SSP_CR0_DSS_SHIFT)
#define SSP_CR0_DSS_14BIT            (13 << SSP_CR0_DSS_SHIFT)
#define SSP_CR0_DSS_15BIT            (14 << SSP_CR0_DSS_SHIFT)
#define SSP_CR0_DSS_16BIT            (15 << SSP_CR0_DSS_SHIFT)

/* Control Register 1 */
#define SSP_CR1_LBM_SHIFT            (0)
#define SSP_CR1_SSE_SHIFT            (1)
#define SSP_CR1_MS_SHIFT             (2)
#define SSP_CR1_SOD_SHIFT            (3)
#define SSP_CR1_RXIFLSEL_SHIFT       (4)
#define SSP_CR1_MFSS_SHIFT           (7)
#define SSP_CR1_RXFIFOCLR_SHIFT      (8)
#define SSP_CR1_TXFIFOCLR_SHIFT      (9)
#define SSP_CR1_TXIFLSEL_SHIFT       (10)

#define SSP_CR1_LBM_MASK             (0x01 << SSP_CR1_LBM_SHIFT)
#define SSP_CR1_SSE_MASK             (0x01 << SSP_CR1_SSE_SHIFT)
#define SSP_CR1_MS_MASK              (0x01 << SSP_CR1_MS_SHIFT)
#define SSP_CR1_SOD_MASK             (0x01 << SSP_CR1_SOD_SHIFT)
#define SSP_CR1_RXIFLSEL_MASK        (0x07 << SSP_CR1_RXIFLSEL_SHIFT)
#define SSP_CR1_MFSS_MASK            (0x01 << SSP_CR1_MFSS_SHIFT)
#define SSP_CR1_RXFIFOCLR_MASK       (0x01 << SSP_CR1_RXFIFOCLR_SHIFT)
#define SSP_CR1_TXFIFOCLR_MASK       (0x01 << SSP_CR1_TXFIFOCLR_SHIFT)
#define SSP_CR1_TXIFLSEL_MASK        (0x07 << SSP_CR1_TXIFLSEL_SHIFT)

#define SSP_CR1_LBM(c)               ((c) << SSP_CR1_LBM_SHIFT)
#define SSP_CR1_SSE(c)               ((c) << SSP_CR1_SSE_SHIFT)
#define SSP_CR1_MS(c)                ((c) << SSP_CR1_MS_SHIFT)
#define SSP_CR1_SOD(c)               ((c) << SSP_CR1_SOD_SHIFT)
#define SSP_CR1_RXIFLSEL(c)          ((c) << SSP_CR1_RXIFLSEL_SHIFT)
#define SSP_CR1_MFSS(c)              ((c) << SSP_CR1_MFSS_SHIFT)
#define SSP_CR1_RXFIFOCLR(c)         ((c) << SSP_CR1_RXFIFOCLR_SHIFT)
#define SSP_CR1_TXFIFOCLR(c)         ((c) << SSP_CR1_TXFIFOCLR_SHIFT)
#define SSP_CR1_TXIFLSEL(c)          ((c) << SSP_CR1_TXIFLSEL_SHIFT)

/* Status Register */
#define SSP_SR_TFE_SHIFT             (0)
#define SSP_SR_TNF_SHIFT             (1)
#define SSP_SR_RNE_SHIFT             (2)
#define SSP_SR_RFF_SHIFT             (3)
#define SSP_SR_BSY_SHIFT             (4)
#define SSP_SR_RXFIFO_SHIFT          (5)
#define SSP_SR_TXFIFO_SHIFT          (9)

#define SSP_SR_TFE_MASK              ((0x01) << SSP_SR_TFE_SHIFT)
#define SSP_SR_TNF_MASK              ((0x01) << SSP_SR_TNF_SHIFT)
#define SSP_SR_RNE_MASK              ((0x01) << SSP_SR_RNE_SHIFT)
#define SSP_SR_RFF_MASK              ((0x01) << SSP_SR_RFF_SHIFT)
#define SSP_SR_BSY_MASK              ((0x01) << SSP_SR_BSY_SHIFT)
#define SSP_SR_RXFIFO_MASK           ((0x0F) << SSP_SR_RXFIFO_SHIFT)
#define SSP_SR_TXFIFO_MASK           ((0x0F) << SSP_SR_TXFIFO_SHIFT)

#define SSP_SR_TFE(c)                ((c) << SSP_SR_TFE_SHIFT)
#define SSP_SR_TNF(c)                ((c) << SSP_SR_TNF_SHIFT)
#define SSP_SR_RNE(c)                ((c) << SSP_SR_RNE_SHIFT)
#define SSP_SR_RFF(c)                ((c) << SSP_SR_RFF_SHIFT)
#define SSP_SR_BSY(c)                ((c) << SSP_SR_BSY_SHIFT)
#define SSP_SR_RXFIFO(c)             ((c) << SSP_SR_RXFIFO_SHIFT)
#define SSP_SR_TXFIFO(c)             ((c) << SSP_SR_TXFIFO_SHIFT)

#define isTXFIFOEmpty(c)             (((c)& SSP_SR_TFE_MASK) >> SSP_SR_TFE_SHIFT)
#define isTXFIFOFull(c)              (!((c)& SSP_SR_TNF_MASK))
#define isRXFIFOEmpty(c)             (((c)& SSP_SR_RNE_MASK) >> SSP_SR_RNE_SHIFT)
#define isRXFIFOFull(c)              (!((c)& SSP_SR_RFF_MASK))
#define isBusy(c)                    (((c)& SSP_SR_BSY_MASK) >> SSP_SR_BSY_SHIFT)

/* Interrupt mask  Regsiter */
#define SSP_IMSC_RORIM_SHIFT         (0)
#define SSP_IMSC_RTIM_SHIFT          (1)
#define SSP_IMSC_RXIM_SHIFT          (2)
#define SSP_IMSC_TXIM_SHIFT          (3)

#define SSP_IMSC_RORIM_MASK          ((0x01) << SSP_IMSC_RORIM_SHIFT)
#define SSP_IMSC_RTIM_MASK           ((0x01) << SSP_IMSC_RTIM_SHIFT)
#define SSP_IMSC_RXIM_MASK           ((0x01) << SSP_IMSC_RXIM_SHIFT)
#define SSP_IMSC_TXIM_MASK           ((0x01) << SSP_IMSC_TXIM_SHIFT)

#define SSP_IMSC_RORIM(c)            ((c) << SSP_IMSC_RORIM_SHIFT
#define SSP_IMSC_RTIM(c)             ((c) << SSP_IMSC_RTIM_SHIFT)
#define SSP_IMSC_RXIM(c)             ((c) << SSP_IMSC_RXIM_SHIFT)
#define SSP_IMSC_TXIM(c)             ((c) << SSP_IMSC_TXIM_SHIFT)

/* Interrupt status Regsiter */
#define SSP_RIS_ROROS_SHIFT          (0)
#define SSP_RIS_RTRIS_SHIFT          (1)
#define SSP_RIS_RXRIS_SHIFT          (2)
#define SSP_RIS_TXRIS_SHIFT          (3)

#define SSP_RIS_ROROS_MASK           ((0x01) << SSP_RIS_ROROS_SHIFT)
#define SSP_RIS_RTRIS_MASK           ((0x01) << SSP_RIS_RTRIS_SHIFT)
#define SSP_RIS_RXRIS_MASK           ((0x01) << SSP_RIS_RXRIS_SHIFT)
#define SSP_RIS_TXRIS_MASK           ((0x01) << SSP_RIS_TXRIS_SHIFT)

#define SSP_RIS_ROROS(c)             ((c) << SSP_RIS_ROROS_SHIFT)
#define SSP_RIS_RTRIS(c)             ((c) << SSP_RIS_RTRIS_SHIFT)
#define SSP_RIS_RXRIS(c)             ((c) << SSP_RIS_RXRIS_SHIFT)
#define SSP_RIS_TXRIS(c)             ((c) << SSP_RIS_TXRIS_SHIFT)

/* Interrupt clear Regsiter */
#define SSP_ICR_RORIC_SHIFT          (0)
#define SSP_ICR_RTIC_SHIFT           (1)

#define SSP_RORIC_MASK               ((0x01) << SSP_ICR_RORIC_SHIFT)
#define SSP_RTIC_MASK                ((0x01) << SSP_ICR_RTIC_SHIFT)

#define SSP_RORIC(c)                 ((c) << SSP_ICR_RORIC_SHIFT)
#define SSP_RTIC(c)                  ((c) << SSP_ICR_RTIC_SHIFT)


/* DMA Control Regsiter */
#define SSP_DMACTR_RXDMAE_SHIFT      (0)
#define SSP_DMACTR_TXDMAE_SHIFT      (1)

#define SSP_DMACTR_RXDMAE_MASK       ((0x01) << SSP_DMACTR_RXDMAE_SHIFT)
#define SSP_DMACTR_TXDMAE_MASK       ((0x01) << SSP_DMACTR_TXDMAE_SHIFT)

#define SSP_DMACTR_RXDMAE(c)         ((c) << SSP_DMACTR_RXDMAE_SHIFT)
#define SSP_DMACTR_TXDMAE(c)         ((c) << SSP_DMACTR_TXDMAE_SHIFT)


#endif /*__S1SBP6A_SPI_H */
