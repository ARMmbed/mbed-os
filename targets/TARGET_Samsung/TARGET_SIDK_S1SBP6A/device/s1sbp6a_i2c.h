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

#ifndef __S1SBP6A_I2C_H
#define __S1SBP6A_I2C_H

#define I2C_CTL_OFFSET                     (0x0000)
#define I2C_FIFO_CTL_OFFSET                (0x0004)
#define I2C_TRAILING_CTL_OFFSET            (0x0008)
#define I2C_INT_EN_OFFSET                  (0x0020)
#define I2C_INT_STAT_OFFSET                (0x0024)
#define I2C_FIFO_STAT_OFFSET               (0x0030)
#define I2C_TX_DATA_OFFSET                 (0x0034)
#define I2C_RX_DATA_OFFSET                 (0x0038)
#define I2C_CONF_OFFSET                    (0x0040)
#define I2C_AUTO_CONF_OFFSET               (0x0044)
#define I2C_TIMEOUT_OFFSET                 (0x0048)
#define I2C_MANUAL_CMD_OFFSET              (0x004C)
#define I2C_TRANS_STATUS_OFFSET            (0x0050)
#define I2C_TIMING_HS1_OFFSET              (0x0054)
#define I2C_TIMING_HS2_OFFSET              (0x0058)
#define I2C_TIMING_HS3_OFFSET              (0x005C)
#define I2C_TIMING_FS1_OFFSET              (0x0060)
#define I2C_TIMING_FS2_OFFSET              (0x0064)
#define I2C_TIMING_FS3_OFFSET              (0x0068)
#define I2C_TIMING_SLA_OFFSET              (0x006C)
#define I2C_TIMING_ADDR_OFFSET             (0x0070)


/* I2C_CTL */
#define I2C_CTL_CS_EN_SHIFT                (0)
#define I2C_CTL_NO_TSCLH_SHIFT             (1)
#define I2C_CTL_NO_1CYC_EDG_SDA_SHIFT      (2)
#define I2C_CTL_MASTER_SHIFT               (3)
#define I2C_CTL_RX_MODE_SHIFT              (4)
#define I2C_CTL_TX_MODE_SHIFT              (5)
#define I2C_CTL_RXCHON_SHIFT               (6)
#define I2C_CTL_TXCHON_SHIFT               (7)
#define I2C_CTL_CH_WIDTH_SHIFT             (10)
#define I2C_CTL_BUS_WIDTH_SHIFT            (12)
#define I2C_CTL_RX_BIT_SWAP_SHIFT          (16)
#define I2C_CTL_TX_BIT_SWAP_SHIFT          (19)
#define I2C_CTL_NO_ARB_FAST_SDA_SHIFT      (28)
#define I2C_CTL_SW_RST_SHIFT               (31)

#define I2C_CTL_CS_EN_MASK                ((0x01) << I2C_CTL_CS_EN_SHIFT)
#define I2C_CTL_NO_TSCLH_MASK             ((0x01) << I2C_CTL_NO_TSCLH_SHIFT)
#define I2C_CTL_NO_1CYC_EDG_SDA_MASK      ((0x01) << I2C_CTL_NO_1CYC_EDG_SDA_SHIFT)
#define I2C_CTL_MASTER_MASK               ((0x01) << I2C_CTL_MASTER_SHIFT)
#define I2C_CTL_RX_MODE_MASK              ((0x01) << I2C_CTL_RX_MODE_SHIFT)
#define I2C_CTL_TX_MODE_MASK              ((0x01) << I2C_CTL_TX_MODE_SHIFT)
#define I2C_CTL_RXCHON_MASK               ((0x01) << I2C_CTL_RXCHON_SHIFT)
#define I2C_CTL_TXCHON_MASK               ((0x01) << I2C_CTL_TXCHON_SHIFT)
#define I2C_CTL_CH_WIDTH_MASK             ((0x03) << I2C_CTL_CH_WIDTH_SHIFT)
#define I2C_CTL_BUS_WIDTH_MASK            ((0x03) << I2C_CTL_BUS_WIDTH_SHIFT)
#define I2C_CTL_RX_BIT_SWAP_MASK          ((0x01) << I2C_CTL_RX_BIT_SWAP_SHIFT)
#define I2C_CTL_TX_BIT_SWAP_MASK          ((0x01) << I2C_CTL_TX_BIT_SWAP_SHIFT)
#define I2C_CTL_NO_ARB_FAST_SDA_MASK      ((0x01) << I2C_CTL_NO_ARB_FAST_SDA_SHIFT)
#define I2C_CTL_SW_RST_MASK               ((0x01) << I2C_CTL_SW_RST_SHIFT)

#define I2C_CTL_CS_EN(c)                  ((c) << I2C_CTL_CS_EN_SHIFT)
#define I2C_CTL_NO_TSCLH(c)               ((c) << I2C_CTL_NO_TSCLH_SHIFT)
#define I2C_CTL_NO_1CYC_EDG_SDA(c)        ((c) << I2C_CTL_NO_1CYC_EDG_SDA_SHIFT)
#define I2C_CTL_MASTER(c)                 ((c) << I2C_CTL_MASTER_SHIFT)
#define I2C_CTL_RX_MODE(c)                ((c) << I2C_CTL_RX_MODE_SHIFT)
#define I2C_CTL_TX_MODE(c)                ((c) << I2C_CTL_TX_MODE_SHIFT)
#define I2C_CTL_RXCHON(c)                 ((c) << I2C_CTL_RXCHON_SHIFT)
#define I2C_CTL_TXCHON(c)                 ((c) << I2C_CTL_TXCHON_SHIFT)
#define I2C_CTL_CH_WIDTH(c)               ((c) << I2C_CTL_CH_WIDTH_SHIFT)
#define I2C_CTL_BUS_WIDTH(c)              ((c) << I2C_CTL_BUS_WIDTH_SHIFT)
#define I2C_CTL_RX_BIT_SWAP(c)            ((c) << I2C_CTL_RX_BIT_SWAP_SHIFT)
#define I2C_CTL_TX_BIT_SWAP(c)            ((c) << I2C_CTL_TX_BIT_SWAP_SHIFT)
#define I2C_CTL_NO_ARB_FAST_SDA(c)        ((c) << I2C_CTL_NO_ARB_FAST_SDA_SHIFT)
#define I2C_CTL_SW_RST(c)                 ((c) << I2C_CTL_SW_RST_SHIFT)


/* I2C_FIFO_CTL */
#define I2C_FIFO_CTL_RXFIFO_EN_SHIFT        (0)
#define I2C_FIFO_CTL_TXFIFO_EN_SHIFT        (1)
#define I2C_FIFO_CTL_RXFIFO_RST_SHIFT       (2)
#define I2C_FIFO_CTL_TXFIFO_RST_SHIFT       (3)
#define I2C_FIFO_CTL_RXFIFO_TRIG_SHIFT      (4)
#define I2C_FIFO_CTL_TXFIFO_TRIG_SHIFT      (16)

#define I2C_FIFO_CTL_RXFIFO_EN_MASK        (0x01 << I2C_FIFO_CTL_RXFIFO_EN_SHIFT)
#define I2C_FIFO_CTL_TXFIFO_EN_MASK        (0x01 << I2C_FIFO_CTL_TXFIFO_EN_SHIFT)
#define I2C_FIFO_CTL_RXFIFO_RST_MASK       (0x01 << I2C_FIFO_CTL_RXFIFO_RST_SHIFT)
#define I2C_FIFO_CTL_TXFIFO_RST_MASK       (0x01 << I2C_FIFO_CTL_TXFIFO_RST_SHIFT)
#define I2C_FIFO_CTL_RXFIFO_TRIG_MASK      (0x7F << I2C_FIFO_CTL_RXFIFO_TRIG_SHIFT)
#define I2C_FIFO_CTL_TXFIFO_TRIG_MASK      (0x7F << I2C_FIFO_CTL_TXFIFO_TRIG_SHIFT)

#define I2C_FIFO_CTL_RXFIFO_EN(c)          ((c) << I2C_FIFO_CTL_RXFIFO_EN_SHIFT)
#define I2C_FIFO_CTL_TXFIFO_EN(c)          ((c) << I2C_FIFO_CTL_TXFIFO_EN_SHIFT)
#define I2C_FIFO_CTL_RXFIFO_RST(c)         ((c) << I2C_FIFO_CTL_RXFIFO_RST_SHIFT)
#define I2C_FIFO_CTL_TXFIFO_RST(c)         ((c) << I2C_FIFO_CTL_TXFIFO_RST_SHIFT)
#define I2C_FIFO_CTL_RXFIFO_TRIG(c)        ((c) << I2C_FIFO_CTL_RXFIFO_TRIG_SHIFT)
#define I2C_FIFO_CTL_TXFIFO_TRIG(c)        ((c) << I2C_FIFO_CTL_TXFIFO_TRIG_SHIFT)


/* I2C_TRAILING_CTL */

/* I2C_INT_EN */
#define I2C_INT_EN_TX_ALMOST_EMPTY_EN_SHIFT           (0)
#define I2C_INT_EN_RX_ALMOST_FULL_EN_SHIFT            (1)
#define I2C_INT_EN_TX_UNDERRUN_EN_SHIFT               (2)
#define I2C_INT_EN_TX_OVERRUN_EN_SHIFT                (3)
#define I2C_INT_EN_RX_UNDERRUN_EN_SHIFT               (4)
#define I2C_INT_EN_RX_OVERRUN_EN_SHIFT                (5)
#define I2C_INT_EN_TRAILING_EN_SHIFT                  (6)
#define I2C_INT_EN_XFER_DONE_AUTO_EN_SHIFT            (7)
#define I2C_INT_EN_XFER_ABORT_AUTO_EN_SHIFT           (8)
#define I2C_INT_EN_NO_DEV_ACK_AUTO_EN_SHIFT           (9)
#define I2C_INT_EN_NO_DEV_AUTO_EN_SHIFT               (10)
#define I2C_INT_EN_TIMEOUT_AUTO_EN_SHIFT              (11)
#define I2C_INT_EN_XFER_DONE_NOACK_MANUAL_EN_SHIFT    (12)
#define I2C_INT_EN_XFER_DONE_MANUAL_EN_SHIFT          (13)
#define I2C_INT_EN_LOSE_ARB_MANUAL_EN_SHIFT           (14)
#define I2C_INT_EN_SLAVE_ADDR_MATCH_EN_SHIFT          (15)

#define I2C_INT_EN_ALL_MASK                           (0xFFFF)
#define I2C_INT_EN_TX_ALMOST_EMPTY_EN_MASK            (0x01 << I2C_INT_EN_TX_ALMOST_EMPTY_EN_SHIFT)
#define I2C_INT_EN_RX_ALMOST_FULL_EN_MASK             (0x01 << I2C_INT_EN_RX_ALMOST_FULL_EN_SHIFT)
#define I2C_INT_EN_TX_UNDERRUN_EN_MASK                (0x01 << I2C_INT_EN_TX_UNDERRUN_EN_SHIFT)
#define I2C_INT_EN_TX_OVERRUN_EN_MASK                 (0x01 << I2C_INT_EN_TX_OVERRUN_EN_SHIFT)
#define I2C_INT_EN_RX_UNDERRUN_EN_MASK                (0x01 << I2C_INT_EN_RX_UNDERRUN_EN_SHIFT)
#define I2C_INT_EN_RX_OVERRUN_EN_MASK                 (0x01 << I2C_INT_EN_RX_OVERRUN_EN_SHIFT)
#define I2C_INT_EN_TRAILING_EN_MASK                   (0x01 << I2C_INT_EN_TRAILING_EN_SHIFT)
#define I2C_INT_EN_XFER_DONE_AUTO_EN_MASK             (0x01 << I2C_INT_EN_XFER_DONE_AUTO_EN_SHIFT)
#define I2C_INT_EN_XFER_ABORT_AUTO_EN_MASK            (0x01 << I2C_INT_EN_XFER_ABORT_AUTO_EN_SHIFT)
#define I2C_INT_EN_NO_DEV_ACK_AUTO_EN_MASK            (0x01 << I2C_INT_EN_NO_DEV_ACK_AUTO_EN_SHIFT)
#define I2C_INT_EN_NO_DEV_AUTO_EN_MASK                (0x01 << I2C_INT_EN_NO_DEV_AUTO_EN_SHIFT)
#define I2C_INT_EN_TIMEOUT_AUTO_EN_MASK               (0x01 << I2C_INT_EN_TIMEOUT_AUTO_EN_SHIFT)
#define I2C_INT_EN_XFER_DONE_NOACK_MANUAL_EN_MASK     (0x01 << I2C_INT_EN_XFER_DONE_NOACK_MANUAL_EN_SHIFT)
#define I2C_INT_EN_XFER_DONE_MANUAL_EN_MASK           (0x01 << I2C_INT_EN_XFER_DONE_MANUAL_EN_SHIFT)
#define I2C_INT_EN_LOSE_ARB_MANUAL_EN_MASK            (0x01 << I2C_INT_EN_LOSE_ARB_MANUAL_EN_SHIFT)
#define I2C_INT_EN_SLAVE_ADDR_MATCH_EN_MASK           (0x01 << I2C_INT_EN_SLAVE_ADDR_MATCH_EN_SHIFT)

#define I2C_INT_EN_TX_ALMOST_EMPTY_EN(c)              ((c) << I2C_INT_EN_TX_ALMOST_EMPTY_EN_SHIFT)
#define I2C_INT_EN_RX_ALMOST_FULL_EN(c)               ((c) << I2C_INT_EN_RX_ALMOST_FULL_EN_SHIFT)
#define I2C_INT_EN_TX_UNDERRUN_EN(c)                  ((c) << I2C_INT_EN_TX_UNDERRUN_EN_SHIFT)
#define I2C_INT_EN_TX_OVERRUN_EN(c)                   ((c) << I2C_INT_EN_TX_OVERRUN_EN_SHIFT)
#define I2C_INT_EN_RX_UNDERRUN_EN(c)                  ((c) << I2C_INT_EN_RX_UNDERRUN_EN_SHIFT)
#define I2C_INT_EN_RX_OVERRUN_EN(c)                   ((c) << I2C_INT_EN_RX_OVERRUN_EN_SHIFT)
#define I2C_INT_EN_TRAILING_EN(c)                     ((c) << I2C_INT_EN_TRAILING_EN_SHIFT)
#define I2C_INT_EN_XFER_DONE_AUTO_EN(c)               ((c) << I2C_INT_EN_XFER_DONE_AUTO_EN_SHIFT)
#define I2C_INT_EN_XFER_ABORT_AUTO_EN(c)              ((c) << I2C_INT_EN_XFER_ABORT_AUTO_EN_SHIFT)
#define I2C_INT_EN_NO_DEV_ACK_AUTO_EN(c)              ((c) << I2C_INT_EN_NO_DEV_ACK_AUTO_EN_SHIFT)
#define I2C_INT_EN_NO_DEV_AUTO_EN(c)                  ((c) << I2C_INT_EN_NO_DEV_AUTO_EN_SHIFT)
#define I2C_INT_EN_TIMEOUT_AUTO_EN(c)                 ((c) << I2C_INT_EN_TIMEOUT_AUTO_EN_SHIFT)
#define I2C_INT_EN_XFER_DONE_NOACK_MANUAL_EN(c)       ((c) << I2C_INT_EN_XFER_DONE_NOACK_MANUAL_EN_SHIFT)
#define I2C_INT_EN_XFER_DONE_MANUAL_EN(c)             ((c) << I2C_INT_EN_XFER_DONE_MANUAL_EN_SHIFT)
#define I2C_INT_EN_LOSE_ARB_MANUAL_EN(c)              ((c) << I2C_INT_EN_LOSE_ARB_MANUAL_EN_SHIFT)
#define I2C_INT_EN_SLAVE_ADDR_MATCH_EN(c)             ((c) << I2C_INT_EN_SLAVE_ADDR_MATCH_EN_SHIFT)

/* I2C_INT_STAT */
#define I2C_INT_STAT_TX_ALMOST_EMPTY_SHIFT            (0)
#define I2C_INT_STAT_RX_ALMOST_FULL_SHIFT             (1)
#define I2C_INT_STAT_TX_OVERRUN_SHIFT                 (3)
#define I2C_INT_STAT_RX_UNDERRUN_SHIFT                (4)
#define I2C_INT_STAT_RX_OVERRUN_SHIFT                 (5)
#define I2C_INT_STAT_TRAILING_SHIFT                   (6)
#define I2C_INT_STAT_XFER_DONE_AUTO_SHIFT             (7)
#define I2C_INT_STAT_XFER_ABORT_AUTO_SHIFT            (8)
#define I2C_INT_STAT_NO_DEV_ACK_AUTO_SHIFT            (9)
#define I2C_INT_STAT_NO_DEV_AUTO_SHIFT                (10)
#define I2C_INT_STAT_TIMEOUT_AUTO_SHIFT               (11)
#define I2C_INT_STAT_XFER_DONE_NOACK_MANUAL_SHIFT     (12)
#define I2C_INT_STAT_XFER_DONE_MANUAL_SHIFT           (13)
#define I2C_INT_STAT_LOSE_ARB_MANUAL_SHIFT            (14)
#define I2C_INT_STAT_SLAVE_ADDR_MATCH_SHIFT           (15)

#define I2C_INT_STAT_TX_ALMOST_EMPTY_MASK           (0x01 << I2C_INT_STAT_TX_ALMOST_EMPTY_SHIFT)
#define I2C_INT_STAT_RX_ALMOST_FULL_MASK            (0x01 << I2C_INT_STAT_RX_ALMOST_FULL_SHIFT)
#define I2C_INT_STAT_TX_OVERRUN_MASK                (0x01 << I2C_INT_STAT_TX_OVERRUN_SHIFT)
#define I2C_INT_STAT_RX_UNDERRUN_MASK               (0x01 << I2C_INT_STAT_RX_UNDERRUN_SHIFT)
#define I2C_INT_STAT_RX_OVERRUN_MASK                (0x01 << I2C_INT_STAT_RX_OVERRUN_SHIFT)
#define I2C_INT_STAT_TRAILING_MASK                  (0x01 << I2C_INT_STAT_TRAILING_SHIFT)
#define I2C_INT_STAT_XFER_DONE_AUTO_MASK            (0x01 << I2C_INT_STAT_XFER_DONE_AUTO_SHIFT)
#define I2C_INT_STAT_XFER_ABORT_AUTO_MASK           (0x01 << I2C_INT_STAT_XFER_ABORT_AUTO_SHIFT)
#define I2C_INT_STAT_NO_DEV_ACK_AUTO_MASK           (0x01 << I2C_INT_STAT_NO_DEV_ACK_AUTO_SHIFT)
#define I2C_INT_STAT_NO_DEV_AUTO_MASK               (0x01 << I2C_INT_STAT_NO_DEV_AUTO_SHIFT)
#define I2C_INT_STAT_TIMEOUT_AUTO_MASK              (0x01 << I2C_INT_STAT_TIMEOUT_AUTO_SHIFT)
#define I2C_INT_STAT_XFER_DONE_NOACK_MANUAL_MASK    (0x01 << I2C_INT_STAT_XFER_DONE_NOACK_MANUAL_SHIFT)
#define I2C_INT_STAT_XFER_DONE_MANUAL_MASK          (0x01 << I2C_INT_STAT_XFER_DONE_MANUAL_SHIFT)
#define I2C_INT_STAT_LOSE_ARB_MANUAL_MASK           (0x01 << I2C_INT_STAT_LOSE_ARB_MANUAL_SHIFT)
#define I2C_INT_STAT_SLAVE_ADDR_MATCH_MASK          (0x01 << I2C_INT_STAT_SLAVE_ADDR_MATCH_SHIFT)

#define I2C_INT_STAT_TX_ALMOST_EMPTY(c)             ((c) << I2C_INT_STAT_TX_ALMOST_EMPTY_SHIFT)
#define I2C_INT_STAT_RX_ALMOST_FULL(c)              ((c) << I2C_INT_STAT_RX_ALMOST_FULL_SHIFT)
#define I2C_INT_STAT_TX_OVERRUN(c)                  ((c) << I2C_INT_STAT_TX_OVERRUN_SHIFT)
#define I2C_INT_STAT_RX_UNDERRUN(c)                 ((c) << I2C_INT_STAT_RX_UNDERRUN_SHIFT)
#define I2C_INT_STAT_RX_OVERRUN(c)                  ((c) << I2C_INT_STAT_RX_OVERRUN_SHIFT)
#define I2C_INT_STAT_TRAILING(c)                    ((c) << I2C_INT_STAT_TRAILING_SHIFT)
#define I2C_INT_STAT_XFER_DONE_AUTO(c)              ((c) << I2C_INT_STAT_XFER_DONE_AUTO_SHIFT)
#define I2C_INT_STAT_XFER_ABORT_AUTO(c)             ((c) << I2C_INT_STAT_XFER_ABORT_AUTO_SHIFT)
#define I2C_INT_STAT_NO_DEV_ACK_AUTO(c)             ((c) << I2C_INT_STAT_NO_DEV_ACK_AUTO_SHIFT)
#define I2C_INT_STAT_NO_DEV_AUTO(c)                 ((c) << I2C_INT_STAT_NO_DEV_AUTO_SHIFT)
#define I2C_INT_STAT_TIMEOUT_AUTO(c)                ((c) << I2C_INT_STAT_TIMEOUT_AUTO_SHIFT)
#define I2C_INT_STAT_XFER_DONE_NOACK_MANUAL(c)      ((c) << I2C_INT_STAT_XFER_DONE_NOACK_MANUAL_SHIFT)
#define I2C_INT_STAT_XFER_DONE_MANUAL(c)            ((c) << I2C_INT_STAT_XFER_DONE_MANUAL_SHIFT)
#define I2C_INT_STAT_LOSE_ARB_MANUAL(c)             ((c) << I2C_INT_STAT_LOSE_ARB_MANUAL_SHIFT)
#define I2C_INT_STAT_SLAVE_ADDR_MATCH_(c)           ((c) << I2C_INT_STAT_SLAVE_ADDR_MATCH_SHIFT)

/* I2C_FIFO_STAT */
#define I2C_FIFO_STAT_TX_FIFO_LEVEL_SHIFT           (0)
#define I2C_FIFO_STAT_TX_FIFO_FULL_SHIFT            (7)
#define I2C_FIFO_STAT_TX_FIFO_EMPTY_SHIFT           (8)
#define I2C_FIFO_STAT_RX_FIFO_LEVEL_SHIFT           (16)
#define I2C_FIFO_STAT_RX_FIFO_FULL_SHIFT            (23)
#define I2C_FIFO_STAT_RX_FIFO_EMPTY_SHIFT           (24)

#define I2C_FIFO_STAT_TX_FIFO_LEVEL_MASK            (0x7F << I2C_FIFO_STAT_TX_FIFO_LEVEL_SHIFT)
#define I2C_FIFO_STAT_TX_FIFO_FULL_MASK             (0x01 << I2C_FIFO_STAT_TX_FIFO_FULL_SHIFT)
#define I2C_FIFO_STAT_TX_FIFO_EMPTY_MASK            (0x01 << I2C_FIFO_STAT_TX_FIFO_EMPTY_SHIFT)
#define I2C_FIFO_STAT_RX_FIFO_LEVEL_MASK            (0x7F << I2C_FIFO_STAT_RX_FIFO_LEVEL_SHIFT)
#define I2C_FIFO_STAT_RX_FIFO_FULL_MASK             (0x01 << I2C_FIFO_STAT_RX_FIFO_FULL_SHIFT)
#define I2C_FIFO_STAT_RX_FIFO_EMPTY_MASK            (0x01 << I2C_FIFO_STAT_RX_FIFO_EMPTY_SHIFT)

#define I2C_FIFO_STAT_TX_FIFO_LEVEL(c)              ((c) << I2C_FIFO_STAT_TX_FIFO_LEVEL_SHIFT)
#define I2C_FIFO_STAT_TX_FIFO_FULL(c)               ((c) << I2C_FIFO_STAT_TX_FIFO_FULL_SHIFT)
#define I2C_FIFO_STAT_TX_FIFO_EMPTY(c)              ((c) << I2C_FIFO_STAT_TX_FIFO_EMPTY_SHIFT)
#define I2C_FIFO_STAT_RX_FIFO_LEVEL(c)              ((c) << I2C_FIFO_STAT_RX_FIFO_LEVEL_SHIFT)
#define I2C_FIFO_STAT_RX_FIFO_FULL(c)               ((c) << I2C_FIFO_STAT_RX_FIFO_FULL_SHIFT)
#define I2C_FIFO_STAT_RX_FIFO_EMPTY(c)              ((c) << I2C_FIFO_STAT_RX_FIFO_EMPTY_SHIFT)

#define I2C_CONF_TSTOPTOIDLE_SHIFT           (0)
#define I2C_CONF_STRCH_EN_SHIFT              (8)
#define I2C_CONF_STRCH_MANUAL_SHIFT          (9)
#define I2C_CONF_FLT_CYCLE_SDA_SHIFT         (13)
#define I2C_CONF_FLT_CYCLE_SCL_SHIFT         (16)
#define I2C_CONF_FILTER_EN_SDA_SHIFT         (27)
#define I2C_CONF_FILTER_EN_SCL_SHIFT         (28)
#define I2C_CONF_HS_MODE_SHIFT               (29)
#define I2C_CONF_ADDR_MODE_SHIFT             (30)
#define I2C_CONF_AUTO_MODE_SHIFT             (31)

#define I2C_CONF_TSTOPTOIDLE_MASK            (0xFF << I2C_CONF_TSTOPTOIDLE_SHIFT)
#define I2C_CONF_STRCH_EN_MASK               (0x01 << I2C_CONF_STRCH_EN_SHIFT)
#define I2C_CONF_STRCH_MANUAL_MASK           (0x01 << I2C_CONF_STRCH_MANUAL_SHIFT)
#define I2C_CONF_FLT_CYCLE_SDA_MASK          (0x07 << I2C_CONF_FLT_CYCLE_SDA_SHIFT)
#define I2C_CONF_FLT_CYCLE_SCL_MASK          (0x07 << I2C_CONF_FLT_CYCLE_SCL_SHIFT)
#define I2C_CONF_FILTER_EN_SDA_MASK          (0x01 << I2C_CONF_FILTER_EN_SDA_SHIFT)
#define I2C_CONF_FILTER_EN_SCL_MASK          (0x01 << I2C_CONF_FILTER_EN_SCL_SHIFT)
#define I2C_CONF_HS_MODE_MASK                (0x01 << I2C_CONF_HS_MODE_SHIFT)
#define I2C_CONF_ADDR_MODE_MASK              (0x01 << I2C_CONF_ADDR_MODE_SHIFT)
#define I2C_CONF_AUTO_MODE_MASK              (0x01 << I2C_CONF_AUTO_MODE_SHIFT)

#define I2C_CONF_TSTOPTOIDLE(c)              ((c) << I2C_CONF_TSTOPTOIDLE_SHIFT)
#define I2C_CONF_STRCH_EN(c)                 ((c) << I2C_CONF_STRCH_EN_SHIFT)
#define I2C_CONF_STRCH_MANUAL(c)             ((c) << I2C_CONF_STRCH_MANUAL_SHIFT)
#define I2C_CONF_FLT_CYCLE_SDA(c)            ((c) << I2C_CONF_FLT_CYCLE_SDA_SHIFT)
#define I2C_CONF_FLT_CYCLE_SCL(c)            ((c) << I2C_CONF_FLT_CYCLE_SCL_SHIFT)
#define I2C_CONF_FILTER_EN_SDA(c)            ((c) << I2C_CONF_FILTER_EN_SDA_SHIFT)
#define I2C_CONF_FILTER_EN_SCL(c)            ((c) << I2C_CONF_FILTER_EN_SCL_SHIFT)
#define I2C_CONF_HS_MODE(c)                  ((c) << I2C_CONF_HS_MODE_SHIFT)
#define I2C_CONF_ADDR_MODE(c)                ((c) << I2C_CONF_ADDR_MODE_SHIFT)
#define I2C_CONF_AUTO_MODE(c)                ((c) << I2C_CONF_AUTO_MODE_SHIFT)

/* I2C_AUTO_CONF */
#define I2C_AUTO_CONF_TRANS_LEN_SHIFT         (0)
#define I2C_AUTO_CONF_READ_WRITE_SHIFT        (16)
#define I2C_AUTO_CONF_STOP_AFTER_TRANS_SHIFT  (17)
#define I2C_AUTO_CONF_MASTER_RUN_SHIFT        (31)

#define I2C_AUTO_CONF_TRANS_LEN_MASK          (0xFF << I2C_AUTO_CONF_TRANS_LEN_SHIFT)
#define I2C_AUTO_CONF_READ_WRITE_MASK         (0x01 << I2C_AUTO_CONF_READ_WRITE_SHIFT)
#define I2C_AUTO_CONF_STOP_AFTER_TRANS_MASK   (0x01 << I2C_AUTO_CONF_STOP_AFTER_TRANS_SHIFT)
#define I2C_AUTO_CONF_MASTER_RUN_MASK         (0x01 << I2C_AUTO_CONF_MASTER_RUN_SHIFT)

#define I2C_AUTO_CONF_TRANS_LEN(c)            ((c) << I2C_AUTO_CONF_TRANS_LEN_SHIFT)
#define I2C_AUTO_CONF_READ_WRITE(c)           ((c) << I2C_AUTO_CONF_READ_WRITE_SHIFT)
#define I2C_AUTO_CONF_STOP_AFTER_TRANS(c)     ((c) << I2C_AUTO_CONF_STOP_AFTER_TRANS_SHIFT)
#define I2C_AUTO_CONF_MASTER_RUN(c)           ((c) << I2C_AUTO_CONF_MASTER_RUN_SHIFT)

/* I2C_TIMEOUT */
#define I2C_TIMEOUT_TOUT_COUNT_SHIFT          (0)
#define I2C_TIMEOUT_TIMEOUT_EN_SHIFT          (31)

#define I2C_TIMEOUT_TOUT_COUNT_MASK           (0xFF << I2C_TIMEOUT_TOUT_COUNT_SHIFT)
#define I2C_TIMEOUT_TIMEOUT_EN_MASK           (0x01 << I2C_TIMEOUT_TIMEOUT_EN_SHIFT)

#define I2C_TIMEOUT_TOUT_COUNT(c)             ((c) << I2C_TIMEOUT_TOUT_COUNT_SHIFT)
#define I2C_TIMEOUT_TIMEOUT_EN(c)             ((c) << I2C_TIMEOUT_TIMEOUT_EN_SHIFT)

/* I2C_MANUAL_CMD */
#define I2C_MANUAL_CMD_SEND_START_SHIFT       (0)
#define I2C_MANUAL_CMD_SEND_RESTART_SHIFT     (1)
#define I2C_MANUAL_CMD_SEND_STOP_SHIFT        (2)
#define I2C_MANUAL_CMD_SEND_DATA_SHIFT        (3)
#define I2C_MANUAL_CMD_READ_DATA_SHIFT        (4)
#define I2C_MANUAL_CMD_RX_ACK_SHIFT           (6)
#define I2C_MANUAL_CMD_RX_DATA_SHIFT          (16)
#define I2C_MANUAL_CMD_TX_DATA_SHIFT          (24)

#define I2C_MANUAL_CMD_SEND_START_MASK        (0x01 << I2C_MANUAL_CMD_SEND_START_SHIFT)
#define I2C_MANUAL_CMD_SEND_RESTART_MASK      (0x01 << I2C_MANUAL_CMD_SEND_RESTART_SHIFT)
#define I2C_MANUAL_CMD_SEND_STOP_MASK         (0x01 << I2C_MANUAL_CMD_SEND_STOP_SHIFT)
#define I2C_MANUAL_CMD_SEND_DATA_MASK         (0x01 << I2C_MANUAL_CMD_SEND_DATA_SHIFT)
#define I2C_MANUAL_CMD_READ_DATA_MASK         (0x01 << I2C_MANUAL_CMD_READ_DATA_SHIFT)
#define I2C_MANUAL_CMD_RX_ACK_MASK            (0x01 << I2C_MANUAL_CMD_RX_ACK_SHIFT)
#define I2C_MANUAL_CMD_RX_DATA_MASK           (0xFF << I2C_MANUAL_CMD_RX_DATA_SHIFT)
#define I2C_MANUAL_CMD_TX_DATA_MASK           (0xFF << I2C_MANUAL_CMD_TX_DATA_SHIFT)

#define I2C_MANUAL_CMD_SEND_START(c)          ((c) << I2C_MANUAL_CMD_SEND_START_SHIFT)
#define I2C_MANUAL_CMD_SEND_RESTART(c)        ((c) << I2C_MANUAL_CMD_SEND_RESTART_SHIFT)
#define I2C_MANUAL_CMD_SEND_STOP(c)           ((c) << I2C_MANUAL_CMD_SEND_STOP_SHIFT)
#define I2C_MANUAL_CMD_SEND_DATA(c)           ((c) << I2C_MANUAL_CMD_SEND_DATA_SHIFT)
#define I2C_MANUAL_CMD_READ_DATA(c)           ((c) << I2C_MANUAL_CMD_READ_DATA_SHIFT)
#define I2C_MANUAL_CMD_RX_ACK(c)              ((c) << I2C_MANUAL_CMD_RX_ACK_SHIFT)
#define I2C_MANUAL_CMD_RX_DATA(c)             ((c) << I2C_MANUAL_CMD_RX_DATA_SHIFT)
#define I2C_MANUAL_CMD_TX_DATA(c)             ((c) << I2C_MANUAL_CMD_TX_DATA_SHIFT)

/* I2C_TRANS_STATUS  */
#define I2C_TRANS_STATUS_MASTER_ST_SHIFT            (0)
#define I2C_TRANS_STATUS_SLAVE_SCL_STRETCH_SHIFT    (15)
#define I2C_TRANS_STATUS_SLAVE_BUSY_SHIFT           (16)
#define I2C_TRANS_STATUS_MASTER_BUSY_SHIFT          (17)
#define I2C_TRANS_STATUS_STOP_COND_SHIFT            (18)
#define I2C_TRANS_STATUS_SLAVE_RW_SHIFT             (19)

#define I2C_TRANS_STATUS_MASTER_ST_MASK             (0x0F << I2C_TRANS_STATUS_MASTER_ST_SHIFT)
#define I2C_TRANS_STATUS_SLAVE_SCL_STRETCH_MASK     (0x00 << I2C_TRANS_STATUS_SLAVE_SCL_STRETCH_SHIFT)
#define I2C_TRANS_STATUS_SLAVE_BUSY_MASK            (0x00 << I2C_TRANS_STATUS_SLAVE_BUSY_SHIFT)
#define I2C_TRANS_STATUS_MASTER_BUSY_MASK           (0x00 << I2C_TRANS_STATUS_MASTER_BUSY_SHIFT)
#define I2C_TRANS_STATUS_STOP_COND_MASK             (0x00 << I2C_TRANS_STATUS_STOP_COND_SHIFT)
#define I2C_TRANS_STATUS_SLAVE_RW_MASK              (0x00 << I2C_TRANS_STATUS_SLAVE_RW_SHIFT)

#define I2C_TRANS_STATUS_MASTER_ST(c)               ((c) << I2C_TRANS_STATUS_MASTER_ST_SHIFT)
#define I2C_TRANS_STATUS_SLAVE_SCL_STRETCH(c)       ((c) << I2C_TRANS_STATUS_SLAVE_SCL_STRETCH_SHIFT)
#define I2C_TRANS_STATUS_SLAVE_BUSY(c)              ((c) << I2C_TRANS_STATUS_SLAVE_BUSY_SHIFT)
#define I2C_TRANS_STATUS_MASTER_BUSY(c)             ((c) << I2C_TRANS_STATUS_MASTER_BUSY_SHIFT)
#define I2C_TRANS_STATUS_STOP_COND(c)               ((c) << I2C_TRANS_STATUS_STOP_COND_SHIFT)
#define I2C_TRANS_STATUS_SLAVE_RW(c)                ((c) << I2C_TRANS_STATUS_SLAVE_RW_SHIFT)

/* I2C_TIMING_HS1 */
#define I2C_TIMING_HS1_TSDA_SU_HS_SHIFT             (0)
#define I2C_TIMING_HS1_TSTOP_SU_HS_SHIFT            (8)
#define I2C_TIMING_HS1_TSTART_HD_HS_SHIFT           (16)
#define I2C_TIMING_HS1_TSTART_SU_HS_SHIFT           (24)

#define I2C_TIMING_HS1_TSDA_SU_HS_MASK              (0xFF << I2C_TIMING_HS1_TSDA_SU_HS_SHIFT)
#define I2C_TIMING_HS1_TSTOP_SU_HS_MASK             (0xFF << I2C_TIMING_HS1_TSTOP_SU_HS_SHIFT)
#define I2C_TIMING_HS1_TSTART_HD_HS_MASK            (0xFF << I2C_TIMING_HS1_TSTART_HD_HS_SHIFT)
#define I2C_TIMING_HS1_TSTART_SU_HS_MASK            (0xFF << I2C_TIMING_HS1_TSTART_SU_HS_SHIFT)

#define I2C_TIMING_HS1_TSDA_SU_HS(c)                ((c) << I2C_TIMING_HS1_TSDA_SU_HS_SHIFT)
#define I2C_TIMING_HS1_TSTOP_SU_HS(c)               ((c) << I2C_TIMING_HS1_TSTOP_SU_HS_SHIFT)
#define I2C_TIMING_HS1_TSTART_HD_HS(c)              ((c) << I2C_TIMING_HS1_TSTART_HD_HS_SHIFT)
#define I2C_TIMING_HS1_TSTART_SU_HS(c)              ((c) << I2C_TIMING_HS1_TSTART_SU_HS_SHIFT)


/* I2C_TIMING_HS2 */
#define I2C_TIMING_HS2_TSCL_H_HS_SHIFT              (0)
#define I2C_TIMING_HS2_TSCL_L_HS_SHIFT              (8)
#define I2C_TIMING_HS2_TDATA_SU_HS_SHIFT            (24)

#define I2C_TIMING_HS2_TSCL_H_HS_MASK               (0xFF << I2C_TIMING_HS2_TSCL_H_HS_SHIFT)
#define I2C_TIMING_HS2_TSCL_L_HS_MASK               (0xFF << I2C_TIMING_HS2_TSCL_L_HS_SHIFT)
#define I2C_TIMING_HS2_TDATA_SU_HS_MASK             (0xFF << I2C_TIMING_HS2_TDATA_SU_HS_SHIFT)

#define I2C_TIMING_HS2_TSCL_H_HS(c)                 ((c) << I2C_TIMING_HS2_TSCL_H_HS_SHIFT)
#define I2C_TIMING_HS2_TSCL_L_HS(c)                 ((c) << I2C_TIMING_HS2_TSCL_L_HS_SHIFT)
#define I2C_TIMING_HS2_TDATA_SU_HS(c)               ((c) << I2C_TIMING_HS2_TDATA_SU_HS_SHIFT)



/* I2C_TIMING_HS3 */
#define I2C_TIMING_HS3_TSR_RELEASE_SHIFT            (0)
#define I2C_TIMING_HS3_CLK_DIV_SHIFT                (16)

#define I2C_TIMING_HS3_TSR_RELEASE_MASK             (0xFF << I2C_TIMING_HS3_TSR_RELEASE_SHIFT)
#define I2C_TIMING_HS3_CLK_DIV_MASK                 (0xFF << I2C_TIMING_HS3_CLK_DIV_SHIFT)

#define I2C_TIMING_HS3_TSR_RELEASE(c)               ((c) << I2C_TIMING_HS3_TSR_RELEASE_SHIFT)
#define I2C_TIMING_HS3_CLK_DIV(c)                   ((c) << I2C_TIMING_HS3_CLK_DIV_SHIFT)
/* I2C_TIMING_FS1 */
#define I2C_TIMING_FS1_TSDA_SU_FS_SHIFT             (0)
#define I2C_TIMING_FS1_TSTOP_SU_FS_SHIFT            (8)
#define I2C_TIMING_FS1_TSTART_HD_FS_SHIFT           (16)
#define I2C_TIMING_FS1_TSTART_SU_FS_SHIFT           (24)

#define I2C_TIMING_FS1_TSDA_SU_FS_MASK              (0xFF << I2C_TIMING_FS1_TSDA_SU_FS_SHIFT)
#define I2C_TIMING_FS1_TSTOP_SU_FS_MASK             (0xFF << I2C_TIMING_FS1_TSTOP_SU_FS_SHIFT)
#define I2C_TIMING_FS1_TSTART_HD_FS_MASK            (0xFF << I2C_TIMING_FS1_TSTART_HD_FS_SHIFT)
#define I2C_TIMING_FS1_TSTART_SU_FS_MASK            (0xFF << I2C_TIMING_FS1_TSTART_SU_FS_SHIFT)

#define I2C_TIMING_FS1_TSDA_SU_FS(c)                ((c) << I2C_TIMING_FS1_TSDA_SU_FS_SHIFT)
#define I2C_TIMING_FS1_TSTOP_SU_FS(c)               ((c) << I2C_TIMING_FS1_TSTOP_SU_FS_SHIFT)
#define I2C_TIMING_FS1_TSTART_HD_FS(c)              ((c) << I2C_TIMING_FS1_TSTART_HD_FS_SHIFT)
#define I2C_TIMING_FS1_TSTART_SU_FS(c)              ((c) << I2C_TIMING_FS1_TSTART_SU_FS_SHIFT)

/* I2C_TIMING_FS2 */
#define I2C_TIMING_FS2_TSCL_H_FS_SHIFT              (0)
#define I2C_TIMING_FS2_TSCL_L_FS_SHIFT              (8)
#define I2C_TIMING_FS2_TDATA_SU_FS_SHIFT            (24)

#define I2C_TIMING_FS2_TSCL_H_FS_MASK               (0xFF << I2C_TIMING_FS2_TSCL_H_FS_SHIFT)
#define I2C_TIMING_FS2_TSCL_L_FS_MASK               (0xFF << I2C_TIMING_FS2_TSCL_L_FS_SHIFT)
#define I2C_TIMING_FS2_TDATA_SU_FS_MASK             (0xFF << I2C_TIMING_FS2_TDATA_SU_FS_SHIFT)

#define I2C_TIMING_FS2_TSCL_H_FS(c)                 ((c) << I2C_TIMING_FS2_TSCL_H_FS_SHIFT)
#define I2C_TIMING_FS2_TSCL_L_FS(c)                 ((c) << I2C_TIMING_FS2_TSCL_L_FS_SHIFT)
#define I2C_TIMING_FS2_TDATA_SU_FS(c)               ((c) << I2C_TIMING_FS2_TDATA_SU_FS_SHIFT)

/* I2C_TIMING_FS3 */
#define I2C_TIMING_FS3_TSR_RELEASE_SHIFT            (0)
#define I2C_TIMING_FS3_CLK_DIV_SHIFT                (8)

#define I2C_TIMING_FS3_TSR_RELEASE_MASK             (0xFF << I2C_TIMING_FS3_TSR_RELEASE_SHIFT)
#define I2C_TIMING_FS3_CLK_DIV_MASK                 (0xFF << I2C_TIMING_FS3_CLK_DIV_SHIFT)

#define I2C_TIMING_FS3_TSR_RELEASE(c)               ((c) << I2C_TIMING_FS3_TSR_RELEASE_SHIFT)
#define I2C_TIMING_FS3_CLK_DIV(c)                   ((c) << I2C_TIMING_FS3_CLK_DIV_SHIFT)

/* I2C_TIMING_SLA */


/* I2C_ADDR */
#define I2C_ADDR_SLAVE_ADDR_SLA_SHIFT               (0)
#define I2C_ADDR_SLAVE_ADDR_MAS_SHIFT               (10)
#define I2C_ADDR_MASTERID_SHIFT                     (24)

#define I2C_ADDR_SLAVE_ADDR_SLA_MASK                (0x3FFul << I2C_ADDR_SLAVE_ADDR_SLA_SHIFT)
#define I2C_ADDR_SLAVE_ADDR_MAS_MASK                (0x3FFul << I2C_ADDR_SLAVE_ADDR_MAS_SHIFT)
#define I2C_ADDR_MASTERID_MASK                      (0x3FFul << I2C_ADDR_MASTERID_SHIFT)

#define I2C_ADDR_SLAVE_ADDR_SLA(c)                  ((c) << I2C_ADDR_SLAVE_ADDR_SLA_SHIFT)
#define I2C_ADDR_SLAVE_ADDR_MAS(c)                  ((c) << I2C_ADDR_SLAVE_ADDR_MAS_SHIFT)
#define I2C_ADDR_MASTERID(c)                        ((c) << I2C_ADDR_MASTERID_SHIFT)

#define I2C_FREQ_400KHZ                             (400000)
#define DEFAULT_I2CXFER_CLOCK                       I2C_FREQ_400KHZ
#define DEFAULT_I2CSLAVE_ADDR                       (0x5Fu)
#define DEFAULT_I2C_TIMEOUT                         (10000)
#define DEFAULT_I2C_TX_TRIGLVL                      (8u)
#define DEFAULT_I2C_RX_TRIGLVL                      (8u)
#define I2C_DEFAULT_I2CSLAVE_ADDR                   (0x77)
#define I2C_DEFAULT_MASTER_ADDRESS                  (0x09)

/* Msg flag */
#define I2C_M_WRITE          (0x0000u)
#define I2C_M_READ           (0x0001u)
#define I2C_M_TEN            (0x0002u)
#define I2C_M_NORESTART      (0x0080u)
#define I2C_M_IGNORE_NAK     (0x1000u)
#define I2C_M_NOSTART        (0x4000u)
#define I2C_M_NOSTOP         (0x8000u)

/* Mode */
#define I2C_MANUAL           (0x0000u)
#define I2C_AUTO             (0x0010u)
#define I2C_INTERRUPT        (0x0020u)
#define I2C_DMA              (0x0040u)
#define I2C_MODE_Msk         (0x0060u)

struct i2c_msg_s {
    uint16_t addr;                /**< Slave address */
    uint32_t timeout;
    uint16_t flags;                /**< I2C flags; See I2C_M_* definitions */
    uint16_t length;            /**< The length of buffer */
    uint8_t *buffer;            /**< The Buffer for transferring message */
};

typedef struct _bp6a_i2c_priv_t_ {
    uint32_t index;
    int xfer_speed;
    bool master;
    uint32_t mode;
    uint32_t slave_addr;
    uint32_t addrlen;
    uint32_t timeout;
    /*  master data  */
    uint8_t msgc;                ///< Message count
    struct i2c_msg_s *msgv;      ///< Message list
    uint16_t mflags;             ///< Current message flags

    /* slave data */
    uint8_t *mptr;               ///< Current message buffer
    int mcnt;                    ///< Current message length
    int cur_msg;
    struct i2c_msg_s *msg;
    volatile uint8_t st_slave_tx_master_rx;
    volatile uint8_t st_slave_rx_master_tx;
    volatile uint16_t int_stat;
} bp6a_i2c_priv_t;

void bp6a_i2c_master_init(uint32_t index, uint32_t freq, int addr_len);
void bp6a_i2c_slave_init(uint32_t index);
int bp6a_i2c_setaddress(uint32_t index, int addr, int nbits);
int bp6a_i2c_start(uint32_t index);
int bp6a_i2c_stop(uint32_t index);
void bp6a_i2c_reset(uint32_t index);
int bp6a_i2c_read_byte(uint32_t index, bool last);
int bp6a_i2c_write_byte(uint32_t index, int data);
int bp6a_i2c_read(uint32_t index, uint8_t *buffer, int buflen, int start, int stop);
int bp6a_i2c_write(uint32_t index, const uint8_t *buffer, int buflen, int start, int stop);
int bp6a_i2c_get_slave_status(uint32_t index);
void bp6a_i2c_set_slave_address(uint32_t index, int addr, bool is_slave);

#endif /*__S1SBP6A_I2C_H */
