/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AT86RFREG_H_
#define AT86RFREG_H_
#ifdef __cplusplus
extern "C" {
#endif

/*AT86RF212 PHY Modes*/
#define BPSK_20                     0x00
#define BPSK_40                     0x04
#define BPSK_40_ALT                 0x14
#define OQPSK_SIN_RC_100            0x08
#define OQPSK_SIN_RC_200            0x09
#define OQPSK_RC_100                0x18
#define OQPSK_RC_200                0x19
#define OQPSK_SIN_250               0x0c
#define OQPSK_SIN_500               0x0d
#define OQPSK_SIN_500_ALT           0x0f
#define OQPSK_RC_250                0x1c
#define OQPSK_RC_500                0x1d
#define OQPSK_RC_500_ALT            0x1f
#define OQPSK_SIN_RC_400_SCR_ON     0x2A
#define OQPSK_SIN_RC_400_SCR_OFF    0x0A
#define OQPSK_RC_400_SCR_ON         0x3A
#define OQPSK_RC_400_SCR_OFF        0x1A
#define OQPSK_SIN_1000_SCR_ON       0x2E
#define OQPSK_SIN_1000_SCR_OFF      0x0E
#define OQPSK_RC_1000_SCR_ON        0x3E
#define OQPSK_RC_1000_SCR_OFF       0x1E

/*Supported transceivers*/
#define PART_AT86RF231              0x03
#define PART_AT86RF212              0x07
#define PART_AT86RF233              0x0B
#define VERSION_AT86RF212           0x01
#define VERSION_AT86RF212B          0x03

/*RF Configuration Registers*/
#define TRX_STATUS                  0x01
#define TRX_STATE                   0x02
#define TRX_CTRL_0                  0x03
#define TRX_CTRL_1                  0x04
#define PHY_TX_PWR                  0x05
#define PHY_RSSI                    0x06
#define PHY_ED_LEVEL                0x07
#define PHY_CC_CCA                  0x08
#define RX_CTRL                     0x0A
#define SFD_VALUE                   0x0B
#define TRX_CTRL_2                  0x0C
#define ANT_DIV                     0x0D
#define IRQ_MASK                    0x0E
#define IRQ_STATUS                  0x0F
#define VREG_CTRL                   0x10
#define BATMON                      0x11
#define XOSC_CTRL                   0x12
#define CC_CTRL_0                   0x13
#define CC_CTRL_1                   0x14
#define RX_SYN                      0x15
#define TRX_RPC                     0x16
#define RF_CTRL_0                   0x16
#define XAH_CTRL_1                  0x17
#define FTN_CTRL                    0x18
#define PLL_CF                      0x1A
#define PLL_DCU                     0x1B
#define PART_NUM                    0x1C
#define VERSION_NUM                 0x1D
#define MAN_ID_0                    0x1E
#define MAN_ID_1                    0x1F
#define SHORT_ADDR_0                0x20
#define SHORT_ADDR_1                0x21
#define PAN_ID_0                    0x22
#define PAN_ID_1                    0x23
#define IEEE_ADDR_0                 0x24
#define IEEE_ADDR_1                 0x25
#define IEEE_ADDR_2                 0x26
#define IEEE_ADDR_3                 0x27
#define IEEE_ADDR_4                 0x28
#define IEEE_ADDR_5                 0x29
#define IEEE_ADDR_6                 0x2A
#define IEEE_ADDR_7                 0x2B
#define XAH_CTRL_0                  0x2C
#define CSMA_SEED_0                 0x2D
#define CSMA_SEED_1                 0x2E
#define CSMA_BE                     0x2F

/* CSMA_SEED_1*/
#define AACK_FVN_MODE1              7
#define AACK_FVN_MODE0              6
#define AACK_SET_PD                 5
#define AACK_DIS_ACK                4
#define AACK_I_AM_COORD             3
#define CSMA_SEED_12                2
#define CSMA_SEED_11                1
#define CSMA_SEED_10                0

/*TRX_STATUS bits*/
#define CCA_STATUS                  0x40
#define CCA_DONE                    0x80

/*PHY_CC_CCA bits*/
#define CCA_REQUEST                 0x80
#define CCA_MODE_3A                 0x00
#define CCA_MODE_1                  0x20
#define CCA_MODE_2                  0x40
#define CCA_MODE_3B                 0x60
#define CCA_MODE_MASK               0x60
#define CCA_CHANNEL_MASK            0x1F

/*IRQ_MASK bits*/
#define RX_START                    0x04
#define TRX_END                     0x08
#define CCA_ED_DONE                 0x10
#define AMI                         0x20
#define TRX_UR                      0x40

/*ANT_DIV bits*/
#define ANT_DIV_EN                  0x08
#define ANT_EXT_SW_EN               0x04
#define ANT_CTRL_DEFAULT            0x03

/*TRX_CTRL_1 bits*/
#define PA_EXT_EN                   0x80
#define TX_AUTO_CRC_ON              0x20
#define SPI_CMD_MODE_TRX_STATUS     0x04
#define SPI_CMD_MODE_PHY_RSSI       0x08
#define SPI_CMD_MODE_IRQ_STATUS     0x0C

/*TRX_CTRL_2 bits*/
#define RX_SAFE_MODE                0x80

/*FTN_CTRL bits*/
#define FTN_START                   0x80

/*PHY_RSSI bits*/
#define CRC_VALID                   0x80

/*RX_SYN bits*/
#define RX_PDT_DIS                  0x80

/*TRX_RPC bits */
#define RX_RPC_CTRL                 0xC0
#define RX_RPC_EN                   0x20
#define PDT_RPC_EN                  0x10
#define PLL_RPC_EN                  0x08
#define XAH_TX_RPC_EN               0x04
#define IPAN_RPC_EN                 0x02
#define TRX_RPC_RSVD_1              0x01

/*XAH_CTRL_1 bits*/
#define AACK_PROM_MODE              0x02


#ifdef __cplusplus
}
#endif

#endif /* AT86RFREG_H_ */
