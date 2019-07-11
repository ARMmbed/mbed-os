/*
 * Copyright (c) 2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define TESTER_CLOCK_FREQUENCY_HZ                     100000000
#define TESTER_CLOCK_PERIOD_NS                        10
#define TESTER_CONTROL                                0x00000000
#define TESTER_CONTROL_RESET                          0x00000000
#define TESTER_CONTROL_RESET_PERIPHERALS              (1 << 0)
#define TESTER_CONTROL_RESET_ALL                      (1 << 1)
#define TESTER_CONTROL_REPROGRAM                      (1 << 2)
#define TESTER_CONTROL_VERSION                        0x00000010
#define TESTER_CONTROL_VERSION_SIZE                   4
#define TESTER_REMAP                                  0x00001000
#define TESTER_SYS_IO                                 0x00002000
#define TESTER_SYS_IO_MODE                            (0x000 + 0x00002C00)
#define TESTER_SYS_IO_MODE_DISABLED                   0
#define TESTER_SYS_IO_MODE_SPI_SERIAL_FLASH           1
#define TESTER_SYS_IO_MODE_I2C_IO_EXPANDER0           2
#define TESTER_SYS_IO_MODE_I2C_IO_EXPANDER1           3
#define TESTER_SYS_IO_MODE_I2C_IO_EXPANDER2           4
#define TESTER_SYS_IO_PWM_ENABLE                      (0x001 + 0x00002C00)
#define TESTER_SYS_IO_PWM_PERIOD                      (0x002 + 0x00002C00)
#define TESTER_SYS_IO_PWM_CYCLES_HIGH                 (0x006 + 0x00002C00)
#define TESTER_SYS_IO_AN_MUX_ANALOGIN_MEASUREMENT     (0x00A + 0x00002C00)
#define TESTER_SYS_IO_NUM_POWER_SAMPLES               (0x00C + 0x00002C00)
#define TESTER_SYS_IO_NUM_POWER_CYCLES                (0x010 + 0x00002C00)
#define TESTER_SYS_IO_ADC_SNAPSHOT                    (0x018 + 0x00002C00)
#define TESTER_SYS_IO_SAMPLE_ADC                      (0x019 + 0x00002C00)
#define TESTER_SYS_IO_ANIN0_MEASUREMENT               (0x030 + 0x00002C00)
#define TESTER_SYS_IO_ANIN0_MEASUREMENTS_SUM          (0x032 + 0x00002C00)
#define TESTER_SYS_IO_ANIN1_MEASUREMENT               (0x03A + 0x00002C00)
#define TESTER_SYS_IO_ANIN1_MEASUREMENTS_SUM          (0x03C + 0x00002C00)
#define TESTER_SYS_IO_ANIN2_MEASUREMENT               (0x044 + 0x00002C00)
#define TESTER_SYS_IO_ANIN2_MEASUREMENTS_SUM          (0x046 + 0x00002C00)
#define TESTER_SYS_IO_ANIN3_MEASUREMENT               (0x04E + 0x00002C00)
#define TESTER_SYS_IO_ANIN3_MEASUREMENTS_SUM          (0x050 + 0x00002C00)
#define TESTER_PERIPHERAL                             0x00100000
#define TESTER_PERIPHERAL_SELECT                      0x00100000
#define TESTER_GPIO                                   0x00101000
#define TESTER_SPI_MASTER                             0x00102000
#define TESTER_SPI_MASTER_STARTS                      0x00102008
#define TESTER_SPI_MASTER_STOPS                       0x00102009
#define TESTER_SPI_MASTER_TRANSFERS                   0x0010200A
#define TESTER_SPI_MASTER_TRANSFERS_SIZE              2
#define TESTER_SPI_MASTER_START_STOP_STATS            0x0010200C
#define TESTER_SPI_MASTER_START_STOP_STATS_SIZE       1
#define TESTER_SPI_MASTER_TO_SLAVE_CHECKSUM           0x00102012
#define TESTER_SPI_MASTER_TO_SLAVE_CHECKSUM_SIZE      4
#define TESTER_SPI_MASTER_CTRL                        0x00102016
#define TESTER_SPI_MASTER_CTRL_SIZE                   2
#define TESTER_SPI_MASTER_HD_TX_CNT                   0x00102018
#define TESTER_SPI_MASTER_HD_TX_CNT_SIZE              2
#define TESTER_SPI_MASTER_HD_RX_CNT                   0x0010201A
#define TESTER_SPI_MASTER_HD_RX_CNT_SIZE              2
#define TESTER_SPI_MASTER_CS_TO_FIRST_SCLK_CNT        0x0010201C
#define TESTER_SPI_MASTER_CS_TO_FIRST_SCLK_CNT_SIZE   4
#define TESTER_SPI_MASTER_LAST_SCLK_TO_CS_CNT         0x00102020
#define TESTER_SPI_MASTER_LAST_SCLK_TO_CS_CNT_SIZE    4
#define TESTER_SPI_MASTER_CLK_MODE_OFFSET             0
#define TESTER_SPI_MASTER_CLK_MODE_SIZE               2
#define TESTER_SPI_MASTER_BIT_ORDER_OFFSET            2
#define TESTER_SPI_MASTER_BIT_ORDER_SIZE              1
#define TESTER_SPI_MASTER_DUPLEX_OFFSET               3
#define TESTER_SPI_MASTER_DUPLEX_SIZE                 1
#define TESTER_SPI_MASTER_SYM_SIZE_OFFSET             4
#define TESTER_SPI_MASTER_SYM_SIZE_SIZE               6
#define TESTER_SPI_SLAVE                              0x00106000
#define TESTER_SPI_SLAVE_STARTS                       0x00106008
#define TESTER_SPI_SLAVE_STOPS                        0x00106009
#define TESTER_SPI_SLAVE_TRANSFERS                    0x0010600A
#define TESTER_SPI_SLAVE_TRANSFERS_SIZE               2
#define TESTER_SPI_SLAVE_TO_MASTER_CHECKSUM           0x00106015
#define TESTER_SPI_SLAVE_TO_MASTER_CHECKSUM_SIZE      4
#define TESTER_SPI_SLAVE_CTRL                         0x00106019
#define TESTER_SPI_SLAVE_CTRL_SIZE                    2
#define TESTER_SPI_SLAVE_HD_TX_CNT                    0x0010601B
#define TESTER_SPI_SLAVE_HD_TX_CNT_SIZE               2
#define TESTER_SPI_SLAVE_HD_RX_CNT                    0x0010601D
#define TESTER_SPI_SLAVE_HD_RX_CNT_SIZE               2
#define TESTER_SPI_SLAVE_CLK_DIV                      0x0010601F
#define TESTER_SPI_SLAVE_CLK_DIV_SIZE                 2
#define TESTER_SPI_SLAVE_NUM_OF_SYMBOLS               0x00106021
#define TESTER_SPI_SLAVE_NUM_OF_SYMBOLS_SIZE          2
#define TESTER_SPI_SLAVE_START_DELAY_US               0x00106023
#define TESTER_SPI_SLAVE_START_DELAY_US_SIZE          1
#define TESTER_SPI_SLAVE_SYM_DELAY_TICKS              0x00106024
#define TESTER_SPI_SLAVE_SYM_DELAY_TICKS_SIZE         2
#define TESTER_SPI_SLAVE_CLK_MODE_OFFSET              0
#define TESTER_SPI_SLAVE_CLK_MODE_SIZE                2
#define TESTER_SPI_SLAVE_BIT_ORDER_OFFSET             2
#define TESTER_SPI_SLAVE_BIT_ORDER_SIZE               1
#define TESTER_SPI_SLAVE_DUPLEX_OFFSET                3
#define TESTER_SPI_SLAVE_DUPLEX_SIZE                  1
#define TESTER_SPI_SLAVE_SYM_SIZE_OFFSET              4
#define TESTER_SPI_SLAVE_SYM_SIZE_SIZE                6
#define TESTER_SPI_SLAVE_START_REQUEST_OFFSET         10
#define TESTER_SPI_SLAVE_START_REQUEST_SIZE           1
#define TESTER_IO_METRICS                             0x00103000
#define TESTER_IO_METRICS_CTRL                        0x00103000
#define TESTER_IO_METRICS_CTRL_ACTIVE_BIT             (1 << 0)
#define TESTER_IO_METRICS_CTRL_RESET_BIT              (1 << 1)
#define TESTER_IO_METRICS_BASE(i)                     (0x00103040 + 0x40 * (i))
#define TESTER_IO_METRICS_MIN_PULSE_LOW(i)            (TESTER_IO_METRICS_BASE(i) + 0x00)
#define TESTER_IO_METRICS_MIN_PULSE_LOW_SIZE          4
#define TESTER_IO_METRICS_MIN_PULSE_HIGH(i)           (TESTER_IO_METRICS_BASE(i) + 0x04)
#define TESTER_IO_METRICS_MIN_PULSE_HIGH_SIZE         4
#define TESTER_IO_METRICS_MAX_PULSE_LOW(i)            (TESTER_IO_METRICS_BASE(i) + 0x08)
#define TESTER_IO_METRICS_MAX_PULSE_LOW_SIZE          4
#define TESTER_IO_METRICS_MAX_PULSE_HIGH(i)           (TESTER_IO_METRICS_BASE(i) + 0x0C)
#define TESTER_IO_METRICS_MAX_PULSE_HIGH_SIZE         4
#define TESTER_IO_METRICS_RISING_EDGES(i)             (TESTER_IO_METRICS_BASE(i) + 0x10)
#define TESTER_IO_METRICS_RISING_EDGES_SIZE           4
#define TESTER_IO_METRICS_FALLING_EDGES(i)            (TESTER_IO_METRICS_BASE(i) + 0x14)
#define TESTER_IO_METRICS_FALLING_EDGES_SIZE          4
#define TESTER_UART_CONTROL                           (0x000 + 0x00104000)
#define TESTER_UART_CONTROL_SIZE                      4
#define TESTER_UART_BAUD_DIVISOR                      (0x004 + 0x00104000)
#define TESTER_UART_BAUD_DIVISOR_SIZE                 2
#define TESTER_UART_BIT_COUNT                         (0x010 + 0x00104000)
#define TESTER_UART_BIT_COUNT_SIZE                    1
#define TESTER_UART_STOP_COUNT                        (0x011 + 0x00104000)
#define TESTER_UART_STOP_COUNT_SIZE                   1
#define TESTER_UART_PARITY                            (0x012 + 0x00104000)
#define TESTER_UART_PARITY_SIZE                       1
#define TESTER_UART_PARITY_ENABLE                     (1 << 0)
#define TESTER_UART_PARITY_ODD_N_EVEN                 (1 << 1)
#define TESTER_UART_RX_CONTROL                        (0x100 + 0x00104000)
#define TESTER_UART_RX_CONTROL_SIZE                   4
#define TESTER_UART_RX_CONTROL_ENABLE                 (1 << 0)
#define TESTER_UART_RX_CONTROL_RESET                  (1 << 1)
#define TESTER_UART_RX_CHECKSUM                       (0x104 + 0x00104000)
#define TESTER_UART_RX_CHECKSUM_SIZE                  4
#define TESTER_UART_RX_COUNT                          (0x108 + 0x00104000)
#define TESTER_UART_RX_COUNT_SIZE                     4
#define TESTER_UART_RX_PARITY_ERRORS                  (0x10C + 0x00104000)
#define TESTER_UART_RX_PARITY_ERRORS_SIZE             4
#define TESTER_UART_RX_STOP_ERRORS                    (0x110 + 0x00104000)
#define TESTER_UART_RX_STOP_ERRORS_SIZE               4
#define TESTER_UART_RX_FRAMING_ERRORS                 (0x114 + 0x00104000)
#define TESTER_UART_RX_FRAMING_ERRORS_SIZE            4
#define TESTER_UART_RX_PREV_4                         (0x118 + 0x00104000)
#define TESTER_UART_RX_PREV_4_SIZE                    2
#define TESTER_UART_RX_PREV_3                         (0x11A + 0x00104000)
#define TESTER_UART_RX_PREV_3_SIZE                    2
#define TESTER_UART_RX_PREV_2                         (0x11C + 0x00104000)
#define TESTER_UART_RX_PREV_2_SIZE                    2
#define TESTER_UART_RX_PREV_1                         (0x11E + 0x00104000)
#define TESTER_UART_RX_PREV_1_SIZE                    2
#define TESTER_UART_TX_CONTROL                        (0x200 + 0x00104000)
#define TESTER_UART_TX_CONTROL_SIZE                   4
#define TESTER_UART_TX_CONTROL_ENABLE                 (1 << 0)
#define TESTER_UART_TX_CONTROL_RESET                  (1 << 1)
#define TESTER_UART_TX_CONTROL_ENABLE_CTS             (1 << 2)
#define TESTER_UART_TX_COUNT                          (0x204 + 0x00104000)
#define TESTER_UART_TX_COUNT_SIZE                     4
#define TESTER_UART_TX_NEXT                           (0x208 + 0x00104000)
#define TESTER_UART_TX_NEXT_SIZE                      2
#define TESTER_UART_CTS_DEACTIVATE_DELAY              (0x210 + 0x00104000)
#define TESTER_UART_CTS_DEACTIVATE_DELAY_SIZE         4
#define TESTER_UART_TX_DELAY                          (0x214 + 0x00104000)
#define TESTER_UART_TX_DELAY_SIZE                     4
#define TESTER_I2C_STARTS                             (0x000 + 0x00105000)
#define TESTER_I2C_STOPS                              (0x001 + 0x00105000)
#define TESTER_I2C_ACKS                               (0x002 + 0x00105000)
#define TESTER_I2C_NACKS                              (0x004 + 0x00105000)
#define TESTER_I2C_TRANSFERS                          (0x006 + 0x00105000)
#define TESTER_I2C_TRANSFERS_SIZE                     2
#define TESTER_I2C_TO_SLAVE_CHECKSUM                  (0x008 + 0x00105000)
#define TESTER_I2C_TO_SLAVE_CHECKSUM_SIZE             4
#define TESTER_I2C_STATE_NUM                          (0x00C + 0x00105000)
#define TESTER_I2C_NUMBER_DEV_ADDR_MATCHES            (0x00D + 0x00105000)
#define TESTER_I2C_DEVICE_ADDRESS                     (0x00E + 0x00105000)
#define TESTER_I2C_SET_SDA                            (0x010 + 0x00105000)
#define TESTER_I2C_PREV_TO_SLAVE_4                    (0x011 + 0x00105000)
#define TESTER_I2C_PREV_TO_SLAVE_3                    (0x012 + 0x00105000)
#define TESTER_I2C_PREV_TO_SLAVE_2                    (0x013 + 0x00105000)
#define TESTER_I2C_PREV_TO_SLAVE_1                    (0x014 + 0x00105000)
#define TESTER_I2C_NEXT_FROM_SLAVE                    (0x015 + 0x00105000)
#define TESTER_I2C_NUM_WRITES                         (0x016 + 0x00105000)
#define TESTER_I2C_NUM_READS                          (0x018 + 0x00105000)
#define TESTER_I2C_FROM_SLAVE_CHECKSUM                (0x01A + 0x00105000)
#define TESTER_I2C_FROM_SLAVE_CHECKSUM_SIZE           4
#define TESTER_I2C_NUMBER_DEV_ADDR_MISMATCHES         (0x01E + 0x00105000)
#define TESTER_I2C_NUMBER_DEV_ADDR_MISMATCHES_SIZE    1
