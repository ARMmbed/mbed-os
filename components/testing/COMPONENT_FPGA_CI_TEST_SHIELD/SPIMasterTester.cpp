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

#include "SPIMasterTester.h"
#include "fpga_config.h"

uint16_t SPIMasterTester::get_transfer_count()
{
    return SPITester::get_transfer_count(TESTER_SPI_MASTER_TRANSFERS, TESTER_SPI_MASTER_TRANSFERS_SIZE);
}

uint32_t SPIMasterTester::get_receive_checksum()
{
    return SPITester::get_receive_checksum(TESTER_SPI_MASTER_TO_SLAVE_CHECKSUM, TESTER_SPI_MASTER_TO_SLAVE_CHECKSUM_SIZE);
}

void SPIMasterTester::set_mode(SPITester::SpiMode mode)
{
    SPITester::set_mode(mode, TESTER_SPI_MASTER_CTRL, TESTER_SPI_MASTER_CTRL_SIZE, TESTER_SPI_MASTER_CLK_MODE_OFFSET, TESTER_SPI_MASTER_CLK_MODE_SIZE);
}

void SPIMasterTester::set_bit_order(SPITester::SpiBitOrder bit_order)
{
    SPITester::set_bit_order(bit_order, TESTER_SPI_MASTER_CTRL, TESTER_SPI_MASTER_CTRL_SIZE, TESTER_SPI_MASTER_BIT_ORDER_OFFSET, TESTER_SPI_MASTER_BIT_ORDER_SIZE);
}

void SPIMasterTester::set_sym_size(uint32_t sym_size)
{
    SPITester::set_sym_size(sym_size, TESTER_SPI_MASTER_CTRL, TESTER_SPI_MASTER_CTRL_SIZE, TESTER_SPI_MASTER_SYM_SIZE_OFFSET, TESTER_SPI_MASTER_SYM_SIZE_SIZE);
}

void SPIMasterTester::set_duplex_mode(SPITester::SpiDuplex duplex)
{
    SPITester::set_duplex_mode(duplex, TESTER_SPI_MASTER_CTRL, TESTER_SPI_MASTER_CTRL_SIZE, TESTER_SPI_MASTER_DUPLEX_OFFSET, TESTER_SPI_MASTER_DUPLEX_SIZE);
}

void SPIMasterTester::set_hd_tx_rx_cnt(uint16_t tx_cnt, uint16_t rx_cnt)
{
    SPITester::set_hd_tx_rx_cnt(tx_cnt, rx_cnt, TESTER_SPI_MASTER_HD_RX_CNT, TESTER_SPI_MASTER_HD_RX_CNT_SIZE, TESTER_SPI_MASTER_HD_TX_CNT, TESTER_SPI_MASTER_HD_TX_CNT_SIZE);
}

uint32_t SPIMasterTester::get_cs_to_first_clk_edge_ns()
{
    uint32_t delay_ns;
    read(TESTER_SPI_MASTER_CS_TO_FIRST_SCLK_CNT, (uint8_t *)&delay_ns, TESTER_SPI_MASTER_CS_TO_FIRST_SCLK_CNT_SIZE);

    return (delay_ns * 10);
}

uint32_t SPIMasterTester::get_last_clk_edge_to_cs_ns()
{
    uint32_t delay_ns;
    read(TESTER_SPI_MASTER_LAST_SCLK_TO_CS_CNT, (uint8_t *)&delay_ns, TESTER_SPI_MASTER_LAST_SCLK_TO_CS_CNT_SIZE);

    return (delay_ns * 10);
}

uint8_t SPIMasterTester::get_start_stop_stats()
{
    uint8_t start_stop_stats;
    read(TESTER_SPI_MASTER_START_STOP_STATS, (uint8_t *)&start_stop_stats, TESTER_SPI_MASTER_START_STOP_STATS_SIZE);

    return start_stop_stats;
}
