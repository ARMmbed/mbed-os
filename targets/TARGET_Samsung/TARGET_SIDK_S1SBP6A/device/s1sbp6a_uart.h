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

#ifndef __S1SBP6A_UART_H
#define __S1SBP6A_UART_H

#define UART_ULCON_OFFSET                               0x0000
#define UART_UCON_OFFSET                                0x0004
#define UART_UFCON_OFFSET                               0x0008
#define UART_UMCON_OFFSET                               0x000C
#define UART_UTRSTAT_OFFSET                             0x0010
#define UART_UERSTAT_OFFSET                             0x0014
#define UART_UFSTAT_OFFSET                              0x0018

#define UART_UTXH_OFFSET                                0x0020
#define UART_URXH_OFFSET                                0x0024
#define UART_UBRDIV_OFFSET                              0x0028
#define UART_UFRAC_OFFSET                               0x002C
#define UART_UINTP_OFFSET                               0x0030
#define UART_UINTS_OFFSET                               0x0034
#define UART_UINTM_OFFSET                               0x0038

#define RCV_BUF_DATA_READY                              (0x01<<0)
#define TX_BUF_EMPTY                                    (0x01<<1)
#define TX_EMPTY                                        (0x01<<2)
#define RX_TIMEOUT_CLR                                  (0x01<<3)
#define RX_DMA_FSM_STA                                  (0x0F<<8)
#define TX_DMA_FSM_STA                                  (0x0F<<12)
#define RX_FIFO_CNT                                     (0xFF<<16)

/* UART ULCON */
#define UART_ULCON_WORD_LEN_SHIFT                       (0)
#define UART_ULCON_N_STOP_BIT_SHIFT                     (2)
#define UART_ULCON_PARITY_MODE_SHIFT                    (3)

#define UART_ULCON_WORD_LEN(c)                          ((c)<<UART_ULCON_WORD_LEN_SHIFT)
#define UART_ULCON_N_STOP_BIT(c)                        ((c)<<UART_ULCON_N_STOP_BIT_SHIFT)
#define UART_ULCON_PARITY_MODE(c)                       ((c)<<UART_ULCON_PARITY_MODE_SHIFT)

#define UART_ULCON_WORD_LEN_MASK                        (0x03<<UART_ULCON_WORD_LEN_SHIFT)
#define UART_ULCON_N_STOP_BIT_MASK                      (0x01<<UART_ULCON_N_STOP_BIT_SHIFT)
#define UART_ULCON_PARITY_MODE_MASK                     (0x07<<UART_ULCON_PARITY_MODE_SHIFT)

#define UART_WL_5BIT                                    0x00
#define UART_WL_6BIT                                    0x01
#define UART_WL_7BIT                                    0x02
#define UART_WL_8BIT                                    0x03

#define UART_NO_PARITY                                  0x00
#define UART_ODD_PARITY                                 0x04
#define UART_EVEN_PARITY                                0x05
#define UART_FORCE_1_PARITY                             0x06
#define UART_FORCE_0_PARITY                             0x07

#define UART_1_STOP_BIT                                 0x00
#define UART_2_STOP_BIT                                 0x01

/* UART UCON */
#define UART_UCON_RCV_MODE_SHIFT                        (0)
#define UART_UCON_TX_MODE_SHIFT                         (2)
#define UART_UCON_SND_BRK_SIG_SHIFT                     (4)
#define UART_UCON_LOOP_BACK_SHIFT                       (5)
#define UART_UCON_RX_ERR_INT_EN_SHIFT                   (6)
#define UART_UCON_RX_TIMEOUT_EN_SHIFT                   (7)
#define UART_UCON_RX_INT_TYPE_SHIFT                     (8)
#define UART_UCON_TX_INT_TYPE_SHIFT                     (9)
#define UART_UCON_RX_TIMEOUT_DMA_SPD_EN_SHIFT           (10)
#define UART_UCON_RX_TIMEOUT_EMPTY_FIFO_SHIFT           (11)
#define UART_UCON_RX_TIMOUT_INT_INTER_SHIFT             (12)
#define UART_UCON_RX_DMA_BURST_SIZE_SHIFT               (16)
#define UART_UCON_TX_DMA_BURST_SIZE_SHIFT               (20)

#define UART_UCON_RCV_MODE_MASK                         (0x03<<UART_UCON_RCV_MODE_SHIFT)
#define UART_UCON_TX_MODE_MASK                          (0x03<<UART_UCON_TX_MODE_SHIFT)
#define UART_UCON_SND_BRK_SIG_MASK                      (0x01<<UART_UCON_SND_BRK_SIG_SHIFT)
#define UART_UCON_LOOP_BACK_MASK                        (0x01<<UART_UCON_LOOP_BACK_SHIFT)
#define UART_UCON_RX_ERR_INT_EN_MASK                    (0x01<<UART_UCON_RX_ERR_INT_EN_SHIFT)
#define UART_UCON_RX_TIMEOUT_EN_MASK                    (0x01<<UART_UCON_RX_TIMEOUT_EN_SHIFT)
#define UART_UCON_RX_INT_TYPE_MASK                      (0x01<<UART_UCON_RX_INT_TYPE_SHIFT)
#define UART_UCON_TX_INT_TYPE_MASK                      (0x01<<UART_UCON_TX_INT_TYPE_SHIFT)
#define UART_UCON_RX_TIMEOUT_DMA_SPD_EN_MASK            (0x01<<UART_UCON_RX_TIMEOUT_DMA_SPD_EN_SHIFT)
#define UART_UCON_RX_TIMEOUT_EMPTY_FIFO_MASK            (0x01<<UART_UCON_RX_TIMEOUT_EMPTY_FIFO_SHIFT)
#define UART_UCON_RX_TIMOUT_INT_INTER_MASK              (0x0F<<UART_UCON_RX_TIMOUT_INT_INTER_SHIFT)
#define UART_UCON_RX_DMA_BURST_SIZE_MASK                (0x03<<UART_UCON_RX_DMA_BURST_SIZE_SHIFT)
#define UART_UCON_TX_DMA_BURST_SIZE_MASK                (0x07<<UART_UCON_TX_DMA_BURST_SIZE_SHIFT)


#define UART_UCON_RCV_MODE(c)                           ((c)<<UART_UCON_RCV_MODE_SHIFT)
#define UART_UCON_TX_MODE(c)                            ((c)<<UART_UCON_TX_MODE_SHIFT)
#define UART_UCON_SND_BRK_SIG(c)                        ((c)<<UART_UCON_SND_BRK_SIG_SHIFT)
#define UART_UCON_LOOP_BACK(c)                          ((c)<<UART_UCON_LOOP_BACK_SHIFT)
#define UART_UCON_RX_ERR_INT_EN(c)                      ((c)<<UART_UCON_RX_ERR_INT_EN_SHIFT)
#define UART_UCON_RX_TIMEOUT_EN(c)                      ((c)<<UART_UCON_RX_TIMEOUT_EN_SHIFT)
#define UART_UCON_RX_INT_TYPE(c)                        ((c)<<UART_UCON_RX_INT_TYPE_SHIFT)
#define UART_UCON_TX_INT_TYPE(c)                        ((c)<<UART_UCON_TX_INT_TYPE_SHIFT)
#define UART_UCON_RX_TIMEOUT_DMA_SPD_EN(c)              ((c)<<UART_UCON_RX_TIMEOUT_DMA_SPD_EN_SHIFT)
#define UART_UCON_RX_TIMEOUT_EMPTY_FIFO(c)              ((c)<<UART_UCON_RX_TIMEOUT_EMPTY_FIFO_SHIFT)
#define UART_UCON_RX_TIMOUT_INT_INTER(c)                ((c)<<UART_UCON_RX_TIMOUT_INT_INTER_SHIFT)
#define UART_UCON_RX_DMA_BURST_SIZE(c)                  ((c)<<UART_UCON_RX_DMA_BURST_SIZE_SHIFT)
#define UART_UCON_TX_DMA_BURST_SIZE(c)                  ((c)<<UART_UCON_TX_DMA_BURST_SIZE_SHIFT)

/* UART UFCON */
#define UART_UFCON_FIFO_EN_SHIFT                        (0)
#define UART_UFCON_RX_FIFO_RESET_SHIFT                  (1)
#define UART_UFCON_TX_FIFO_RESET_SHIFT                  (2)
#define UART_UFCON_RX_FIFO_TRIG_LVL_SHIFT               (4)
#define UART_UFCON_TX_FIFO_TRIG_LVL_SHIFT               (8)

#define  UART_UFCON_FIFO_EN_MASK                        (0x01<<UART_UFCON_FIFO_EN_SHIFT)
#define  UART_UFCON_RX_FIFO_RESET_MASK                  (0x01<<UART_UFCON_RX_FIFO_RESET_SHIFT)
#define  UART_UFCON_TX_FIFO_RESET_MASK                  (0x01<<UART_UFCON_TX_FIFO_RESET_SHIFT)
#define  UART_UFCON_RX_FIFO_TRIG_LVL_MASK               (0x07<<UART_UFCON_RX_FIFO_TRIG_LVL_SHIFT)
#define  UART_UFCON_TX_FIFO_TRIG_LVL_MASK               (0x07<<UART_UFCON_TX_FIFO_TRIG_LVL_SHIFT)

#define  UART_UFCON_FIFO_EN(c)                          ((c)<<UART_UFCON_FIFO_EN_SHIFT)
#define  UART_UFCON_RX_FIFO_RESET(c)                    ((c)<<UART_UFCON_RX_FIFO_RESET_SHIFT)
#define  UART_UFCON_TX_FIFO_RESET(c)                    ((c)<<UART_UFCON_TX_FIFO_RESET_SHIFT)
#define  UART_UFCON_RX_FIFO_TRIG_LVL(c)                 ((c)<<UART_UFCON_RX_FIFO_TRIG_LVL_SHIFT)
#define  UART_UFCON_TX_FIFO_TRIG_LVL(c)                 ((c)<<UART_UFCON_TX_FIFO_TRIG_LVL_SHIFT)


/* UART UMCON */
#define UART_UMCON_RTS_SHIFT                            (0)
#define UART_UMCON_AFC_SHIFT                            (4)
#define UART_UMCON_RTS_TRIGGER_LEVEL_SHIFT              (5)

#define UART_UMCON_RTS_MASK                             (0x01<<UART_UMCON_RTS_SHIFT)
#define UART_UMCON_AFC_MASK                             (0x01<<UART_UMCON_AFC_SHIFT)
#define UART_UMCON_RTS_TRIGGER_LEVEL_MASK               (0x07<<UART_UMCON_RTS_TRIGGER_LEVEL_SHIFT)

#define UART_UMCON_RTS(c)                               ((c)<<UART_UMCON_RTS_MASK)
#define UART_UMCON_AFC(c)                               ((c)<<UART_UMCON_AFC_MASK)
#define UART_UMCON_RTS_TRIGGER_LEVEL(c)                 ((c)<<UART_UMCON_RTS_TRIGGER_LEVEL_MASK)


/* UART UTRSTAT */
#define UART_UTRSTAT_RCV_BUF_DATA_READY_SHIFT               (0)
#define UART_UTRSTAT_TX_BUF_EMPTY_SHIFT                     (1)
#define UART_UTRSTAT_TX_EMPTY_SHIFT                         (2)
#define UART_UTRSTAT_RX_TIMEOUT_CLR_SHIFT                   (3)
#define UART_UTRSTAT_RX_DMA_FSM_STA_SHIFT                   (8)
#define UART_UTRSTAT_TX_DMA_FSM_STA_SHIFT                   (12)
#define UART_UTRSTAT_RX_FIFO_CNT_SHIFT                      (16)


#define UART_UTRSTAT_RCV_BUF_DATA_READY_MASK                (0x01<<UART_UTRSTAT_RCV_BUF_DATA_READY_SHIFT)
#define UART_UTRSTAT_TX_BUF_EMPTY_MASK                      (0x01<<UART_UTRSTAT_TX_BUF_EMPTY_SHIFT)
#define UART_UTRSTAT_TX_EMPTY_MASK                          (0x01<<UART_UTRSTAT_TX_EMPTY_SHIFT)
#define UART_UTRSTAT_RX_TIMEOUT_CLR_MASK                    (0x01<<UART_UTRSTAT_RX_TIMEOUT_CLR_SHIFT)
#define UART_UTRSTAT_RX_DMA_FSM_STA_MASK                    (0x0F<<UART_UTRSTAT_RX_DMA_FSM_STA_SHIFT)
#define UART_UTRSTAT_TX_DMA_FSM_STA_MASK                    (0x0F<<UART_UTRSTAT_TX_DMA_FSM_STA_SHIFT)
#define UART_UTRSTAT_RX_FIFO_CNT_MASK                       (0xFF<<UART_UTRSTAT_RX_FIFO_CNT_SHIFT)

#define UART_UTRSTAT_RCV_BUF_DATA_READY(c)                  ((c)<<UART_UTRSTAT_RCV_BUF_DATA_READY_SHIFT)
#define UART_UTRSTAT_TX_BUF_EMPTY(c)                        ((c)<<UART_UTRSTAT_TX_BUF_EMPTY_SHIFT)
#define UART_UTRSTAT_TX_EMPTY(c)                            ((c)<<UART_UTRSTAT_TX_EMPTY_SHIFT)
#define UART_UTRSTAT_RX_TIMEOUT_CLR(c)                      ((c)<<UART_UTRSTAT_RX_TIMEOUT_CLR_SHIFT)
#define UART_UTRSTAT_RX_DMA_FSM_STA(c)                      ((c)<<UART_UTRSTAT_RX_DMA_FSM_STA_SHIFT)
#define UART_UTRSTAT_TX_DMA_FSM_STA(c)                      ((c)<<UART_UTRSTAT_TX_DMA_FSM_STA_SHIFT)
#define UART_UTRSTAT_RX_FIFO_CNT(c)                         ((c)<<UART_UTRSTAT_RX_FIFO_CNT_SHIFT)


/* UART UERSTAT */
#define UART_UERSTAT_OVERRUN_ERR_SHIFT                      (0)
#define UART_UERSTAT_PRITY_ERR_SHIFT                        (1)
#define UART_UERSTAT_FRAME_ERR_SHIFT                        (2)
#define UART_UERSTAT_BREAK_DETECT_SHIFT                     (3)

#define UART_UERSTAT_OVERRUN_ERR_MASK                       (0x01<<UART_UERSTAT_OVERRUN_ERR_SHIFT)
#define UART_UERSTAT_PRITY_ERR_MASK                         (0x01<<UART_UERSTAT_PRITY_ERR_SHIFT)
#define UART_UERSTAT_FRAME_ERR_MASK                         (0x01<<UART_UERSTAT_FRAME_ERR_SHIFT)
#define UART_UERSTAT_BREAK_DETECT_MASK                      (0x01<<UART_UERSTAT_BREAK_DETECT_SHIFT)

#define UART_UERSTAT_OVERRUN_ERR(c)                         ((c)<<UART_UERSTAT_OVERRUN_ERR_SHIFT)
#define UART_UERSTAT_PRITY_ERR(c)                           ((c)<<UART_UERSTAT_PRITY_ERR_SHIFT)
#define UART_UERSTAT_FRAME_ERR(c)                           ((c)<<UART_UERSTAT_FRAME_ERR_SHIFT)
#define UART_UERSTAT_BREAK_DETECT(c)                        ((c)<<UART_UERSTAT_BREAK_DETECT_SHIFT)

/* UART UFSTAT */
#define UART_UFSTAT_RX_FIFO_CNT_SHIFT                       (0)
#define UART_UFSTAT_RX_FIFO_FULL_SHIFT                      (8)
#define UART_UFSTAT_RX_FIFO_ERR_SHIFT                       (9)
#define UART_UFSTAT_TX_FIFO_CNT_SHIFT                       (16)
#define UART_UFSTAT_TX_FIFO_FULL_SHIFT                      (24)

#define UART_UFSTAT_RX_FIFO_CNT_MASK                        (0xFF<<UART_UFSTAT_RX_FIFO_CNT_SHIFT)
#define UART_UFSTAT_RX_FIFO_FULL_MASK                       (0x01<<UART_UFSTAT_RX_FIFO_FULL_SHIFT)
#define UART_UFSTAT_RX_FIFO_ERR_MASK                        (0x01<<UART_UFSTAT_RX_FIFO_ERR_SHIFT)
#define UART_UFSTAT_TX_FIFO_CNT_MASK                        (0xFF<<UART_UFSTAT_TX_FIFO_CNT_SHIFT)
#define UART_UFSTAT_TX_FIFO_FULL_MASK                       (0x01<<UART_UFSTAT_TX_FIFO_FULL_SHIFT)

#define UART_UFSTAT_RX_FIFO_CNT(c)                          ((c)<<UART_UFSTAT_RX_FIFO_CNT_SHIFT)
#define UART_UFSTAT_RX_FIFO_FULL(c)                         ((c)<<UART_UFSTAT_RX_FIFO_FULL_SHIFT)
#define UART_UFSTAT_RX_FIFO_ERR(c)                          ((c)<<UART_UFSTAT_RX_FIFO_ERR_SHIFT)
#define UART_UFSTAT_TX_FIFO_CNT(c)                          ((c)<<UART_UFSTAT_TX_FIFO_CNT_SHIFT)
#define UART_UFSTAT_TX_FIFO_FULL(c)                         ((c)<<UART_UFSTAT_TX_FIFO_FULL_SHIFT)

/* UART_UINTP */
#define UART_UINTP_RXD_SHIFT                                (0)
#define UART_UINTP_ERR_SHIFT                                (1)
#define UART_UINTP_TXD_SHIFT                                (2)

#define UART_UINTP_RXD_MASK                                 (0x01<<UART_UINTP_RXD_SHIFT)
#define UART_UINTP_ERR_MASK                                 (0x01<<UART_UINTP_ERR_SHIFT)
#define UART_UINTP_TXD_MASK                                 (0x01<<UART_UINTP_TXD_SHIFT)

#define UART_UINTP_RXD(c)                               ((c)<<UART_UINTP_RXD_SHIFT)
#define UART_UINTP_ERR(c)                               ((c)<<UART_UINTP_ERR_SHIFT)
#define UART_UINTP_TXD(c)                               ((c)<<UART_UINTP_TXD_SHIFT)

/* UART_UINTS */
#define UART_UINTS_RXD_SHIFT                                (0)
#define UART_UINTS_ERR_SHIFT                                (1)
#define UART_UINTS_TXD_SHIFT                                (2)

#define UART_UINTS_RXD_MASK                                 (0x01<<UART_UINTS_RXD_SHIFT)
#define UART_UINTS_ERR_MASK                                 (0x01<<UART_UINTS_ERR_SHIFT)
#define UART_UINTS_TXD_MASK                                 (0x01<<UART_UINTS_TXD_SHIFT)

#define UART_UINTS_RXD(c)                               ((c)<<UART_UINTS_RXD_SHIFT)
#define UART_UINTS_ERR(c)                               ((c)<<UART_UINTS_ERR_SHIFT)
#define UART_UINTS_TXD(c)                               ((c)<<UART_UINTS_TXD_SHIFT)

/* UART_UINTM */
#define UART_UINTM_RXD_SHIFT                                (0)
#define UART_UINTM_ERR_SHIFT                                (1)
#define UART_UINTM_TXD_SHIFT                                (2)

#define UART_UINTM_RXD_MASK                                 (0x01 << UART_UINTM_RXD_SHIFT)
#define UART_UINTM_ERR_MASK                                 (0x01 << UART_UINTM_ERR_SHIFT)
#define UART_UINTM_TXD_MASK                                 (0x01 << UART_UINTM_TXD_SHIFT)

#define UART_UINTM_RXD(c)                               ((c) << UART_UINTM_RXD_SHIFT)
#define UART_UINTM_ERR(c)                               ((c) << UART_UINTM_ERR_SHIFT)
#define UART_UINTM_TXD(c)                               ((c) << UART_UINTM_TXD_SHIFT)

#endif /*__S1SBP6A_UART_H */
