/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
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

#ifndef S2LPREG_H_
#define S2LPREG_H_
#ifdef __cplusplus
extern "C" {
#endif

#define RF_MTU              2047
#define PARTNUM             0x03
#define VERSION             0xC1
#define FIFO_SIZE           128
#define SPI_HEADER_LENGTH   2
#define RSSI_SETTLING_TIME  250

#define S2LP_GPIO0   0
#define S2LP_GPIO1   1
#define S2LP_GPIO2   2
#define S2LP_GPIO3   3

// GPIO modes
#define DIG_IN          1
#define DIG_OUT_LOW     2
#define DIG_OUT_HIGH    3

// Interrupt events
#define RX_DATA_READY           0
#define RX_DATA_DISCARDED       1
#define TX_DATA_SENT            2
#define MAX_RE_TX               3
#define CRC_ERROR               4
#define TX_FIFO_UNF_OVF         5
#define RX_FIFO_UNF_OVF         6
#define TX_FIFO_ALMOST_FULL     7
#define TX_FIFO_ALMOST_EMPTY    8
#define RX_FIFO_ALMOST_FULL     9
#define RX_FIFO_ALMOST_EMPTY    10
#define MAX_CCA_BACKOFFS        11
#define VALID_PREAMBLE          12
#define SYNC_WORD               13
#define RSSI_ABOVE_THR          14
#define WAKE_UP_TIMEOUT         15
#define READY                   16
#define STANDBY_SWITCHING       17
#define LOW_BATTERY_LVL         18
#define POWER_ON_RESET          19
#define RX_TIMER_TIMEOUT        28
#define SNIFF_TIMER_TIMEOUT     29

// GPIO signals
#define NIRQ                                0
#define POR                                 1
#define WUT_EXPIRE                          2
#define LOW_BATTERY                         3
#define TX_DATA_OUTPUT                      4
#define TX_STATE                            5
#define TXRX_FIFO_ALMOST_EMPTY              6
#define TXRX_FIFO_ALMOST_FULL               7
#define RX_DATA_OUTPUT                      8
#define RX_CLOCK_OUTPUT                     9
#define RX_STATE                            10
#define STATE_OTHER_THAN_SLEEP_OR_STANDBY   11
#define STANDBY_STATE                       12
#define ANTENNA_SWITCH                      13
#define VALID_PREAMBLE_DETECTED             14
#define SYNC_WORD_DETECTED                  15
#define RSSI_ABOVE_THRESHOLD                16
#define TXRX_MODE_INDICATOR                 18
#define VDD                                 19
#define GND                                 20
#define SMPS_ENABLE                         21
#define SLEEP_STATE                         22
#define READY_STATE                         23
#define LOCK_STATE                          24
#define WAIT_LOCK_DETECTOR                  25
#define TX_DATA_OOK                         26
#define WAIT_READY                          27
#define WAIT_TIMER_EXPIRATION               28
#define END_OF_CALIBRATION                  29
#define ENABLE_SYNTH_BLOCK                  30

// RF registers
#define GPIO0_CONF          0x00
#define GPIO1_CONF          0x01
#define GPIO2_CONF          0x02
#define GPIO3_CONF          0x03
#define SYNT3               0x05
#define SYNT2               0x06
#define SYNT1               0x07
#define SYNT0               0x08
#define IF_OFFSET_ANA       0x09
#define IF_OFFSET_DIG       0x0A
#define CHSPACE             0x0C
#define CHNUM               0x0D
#define MOD4                0x0E
#define MOD3                0x0F
#define MOD2                0x10
#define MOD1                0x11
#define MOD0                0x12
#define CHFLT               0x13
#define AFC2                0x14
#define AFC1                0x15
#define AFC0                0x16
#define RSSI_FLT            0x17
#define RSSI_TH             0x18
#define AGCCTRL4            0x1A
#define AGCCTRL3            0x1B
#define AGCCTRL2            0x1C
#define AGCCTRL1            0x1D
#define AGCCTRL0            0x1E
#define ANT_SELECT_CONF     0x1F
#define CLOCKREC2           0x20
#define CLOCKREC1           0x21
#define PCKTCTRL6           0x2B
#define PCKTCTRL5           0x2C
#define PCKTCTRL4           0x2D
#define PCKTCTRL3           0x2E
#define PCKTCTRL2           0x2F
#define PCKTCTRL1           0x30
#define PCKTLEN1            0x31
#define PCKTLEN0            0x32
#define SYNC3               0x33
#define SYNC2               0x34
#define SYNC1               0x35
#define SYNC0               0x36
#define QI                  0x37
#define PCKT_PSTMBL         0x38
#define PROTOCOL2           0x39
#define PROTOCOL1           0x3A
#define PROTOCOL0           0x3B
#define FIFO_CONFIG3        0x3C
#define FIFO_CONFIG2        0x3D
#define FIFO_CONFIG1        0x3E
#define FIFO_CONFIG0        0x3F
#define PCKT_FLT_OPTIONS    0x40
#define PCKT_FLT_GOALS4     0x41
#define PCKT_FLT_GOALS3     0x42
#define PCKT_FLT_GOALS2     0x43
#define PCKT_FLT_GOALS1     0x44
#define PCKT_FLT_GOALS0     0x45
#define TIMERS5             0x46
#define TIMERS4             0x47
#define TIMERS3             0x48
#define TIMERS2             0x49
#define TIMERS1             0x4A
#define TIMERS0             0x4B
#define CSMA_CONF3          0x4C
#define CSMA_CONF2          0x4D
#define CSMA_CONF1          0x4E
#define CSMA_CONF0          0x4F
#define IRQ_MASK3           0x50
#define IRQ_MASK2           0x51
#define IRQ_MASK1           0x52
#define IRQ_MASK0           0x53
#define FAST_RX_TIMER       0x54
#define PA_POWER8           0x5A
#define PA_POWER7           0x5B
#define PA_POWER6           0x5C
#define PA_POWER5           0x5D
#define PA_POWER4           0x5E
#define PA_POWER3           0x5F
#define PA_POWER2           0x60
#define PA_POWER1           0x61
#define PA_POWER0           0x62
#define PA_CONFIG1          0x63
#define PA_CONFIG0          0x64
#define SYNTH_CONFIG2       0x65
#define VCO_CONFIG          0x68
#define VCO_CALIBR_IN2      0x69
#define VCO_CALIBR_IN1      0x6A
#define VCO_CALIBR_IN0      0x6B
#define XO_RCO_CONF1        0x6C
#define XO_RCO_CONF0        0x6D
#define RCO_CALIBR_CONF3    0x6E
#define RCO_CALIBR_CONF2    0x6F
#define PM_CONF4            0x75
#define PM_CONF3            0x76
#define PM_CONF2            0x77
#define PM_CONF1            0x78
#define PM_CONF0            0x79
#define MC_STATE1           0x8D
#define MC_STATE0           0x8E
#define TX_FIFO_STATUS      0x8F
#define RX_FIFO_STATUS      0x90
#define RCO_CALIBR_OUT4     0x94
#define RCO_CALIBR_OUT3     0x95
#define VCO_CALIBR_OUT1     0x99
#define VCO_CALIBR_OUT0     0x9A
#define TX_PCKT_INFO        0x9C
#define RX_PCKT_INFO        0x9D
#define AFC_CORR            0x9E
#define LINK_QUALIF2        0x9F
#define LINK_QUALIF1        0xA0
#define RSSI_LEVEL          0xA2
#define RX_PCKT_LEN1        0xA4
#define RX_PCKT_LEN0        0xA5
#define CRC_FIELD3          0xA6
#define CRC_FIELD2          0xA7
#define CRC_FIELD1          0xA8
#define CRC_FIELD0          0xA9
#define RX_ADDRE_FIELD1     0xAA
#define RX_ADDRE_FIELD0     0xAB
#define RSSI_LEVEL_RUN      0xEF
#define DEVICE_INFO1        0xF0
#define DEVICE_INFO0        0xF1
#define IRQ_STATUS3         0xFA
#define IRQ_STATUS2         0xFB
#define IRQ_STATUS1         0xFC
#define IRQ_STATUS0         0xFD
#define TX_FIFO             0xFF
#define RX_FIFO             0xFF

#define SFD0 0x90
#define SFD1 0x4e

#define DEFAULT_DEVIATION       125000
#define RX_FILTER_BANDWIDTH     540000
#define RSSI_THRESHOLD          -85

// PCKTCTRL6
#define PCKT_SYNCLEN_FIELD      0xFC
#define PCKT_SYNCLEN            (16 << 2)

// PCKTCTRL5
#define PCKT_PREAMBLE_LEN       32

// PCKTCTRL3
#define PCKT_FORMAT_FIELD       0xC0
#define PCKT_FORMAT_802_15_4    (1 << 6)
#define PCKT_RXMODE_FIELD       0x30
#define PCKT_RXMODE_NORMAL      (0 << 4)
#define PCKT_BYTE_SWAP_FIELD    0x04
#define PCKT_BYTE_SWAP_LSB      (1 << 2)

// PCKTCTRL2
#define PCKT_FIXVARLEN_FIELD    0x01
#define PCKT_VARIABLE_LEN       (1 << 0)

// PCKTCTRL1
#define PCKT_CRCMODE_FIELD      0xE0
#define PCKT_CRCMODE_0X1021     (3 << 5)
#define PCKT_TXSOURCE_FIELD     0x0C
#define PCKT_TXSOURCE_NORMAL    (0 << 2)
#define PCKT_WHITENING_FIELD    0x10
#define PCKT_WHITENING_ENABLED  (1 << 4)

// MOD4
#define DATARATE_M_MSB          0x47
// MOD3
#define DATARATE_M_LSB          0xAE

// MOD2
#define MOD_TYPE_FIELD          0xF0
#define MOD_2FSK                (0 << 4)
#define MOD_2GFSK               (10 << 4)
#define DATARATE_E_FIELD        0x0F
#define DATARATE_E              (10 << 0)

// MOD1
#define FDEV_E_FIELD            0x0F

// QI
#define PQI_TH_FIELD            0x1E
#define PQI_TH                  (8 << 1)
#define SQI_EN_FIELD            0x01
#define SQI_EN                  (1 << 0)

// SYNT3
#define SYNT_FIELD              0x0F

// CHFLT
#define CHFLT_M_FIELD           0xF0
#define CHFLT_E_FIELD           0x0F

// LINK_QUALIF1
#define CARRIER_SENSE           (1 << 7)

#define SPI_WR_REG  0x00
#define SPI_RD_REG  0x01
#define SPI_CMD     0x80

typedef enum {
    S2LP_STATE_STANDBY      = 0x02,
    S2LP_STATE_SLEEPA       = 0x01,
    S2LP_STATE_SLEEPB       = 0x03,
    S2LP_STATE_READY        = 0x00,
    S2LP_STATE_LOCK         = 0x0C,
    S2LP_STATE_RX           = 0x30,
    S2LP_STATE_TX           = 0x5C,
    S2LP_STATE_SYNTH_SETUP  = 0x50
} s2lp_states_e;

#if defined __cplusplus && __cplusplus >= 201103
typedef enum : uint8_t {
#else
typedef enum {
#endif
    S2LP_CMD_TX = 0x60,
    S2LP_CMD_RX,
    S2LP_CMD_READY,
    S2LP_CMD_STANDBY,
    S2LP_CMD_SLEEP,
    S2LP_CMD_LOCKRX,
    S2LP_CMD_LOCKTX,
    S2LP_CMD_SABORT,
    S2LP_CMD_LDC_RELOAD,
    S2LP_CMD_SRES = 0x70,
    S2LP_CMD_FLUSHRXFIFO,
    S2LP_CMD_FLUSHTXFIFO,
    S2LP_CMD_SEQUPDATE
} s2lp_commands_e;

typedef enum {
    RF_IDLE,
    RF_CSMA_STARTED,
    RF_TX_STARTED,
    RF_RX_STARTED,
    RF_TX_ACK
} rf_states_e;

#ifdef __cplusplus
}
#endif

#endif /* S2LPREG_H_ */
