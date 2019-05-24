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

#include "SPISlaveTester.h"
#include "fpga_config.h"

uint16_t SPISlaveTester::get_transfer_count()
{
    return SPITester::get_transfer_count(TESTER_SPI_SLAVE_TRANSFERS, TESTER_SPI_SLAVE_TRANSFERS_SIZE);
}

uint32_t SPISlaveTester::get_receive_checksum()
{
    return SPITester::get_receive_checksum(TESTER_SPI_SLAVE_TO_MASTER_CHECKSUM, TESTER_SPI_SLAVE_TO_MASTER_CHECKSUM_SIZE);
}

void SPISlaveTester::set_mode(SPITester::SpiMode mode)
{
    SPITester::set_mode(mode, TESTER_SPI_SLAVE_CTRL, TESTER_SPI_SLAVE_CTRL_SIZE, TESTER_SPI_SLAVE_CLK_MODE_OFFSET, TESTER_SPI_SLAVE_CLK_MODE_SIZE);
}

void SPISlaveTester::set_bit_order(SPITester::SpiBitOrder bit_order)
{
    SPITester::set_bit_order(bit_order, TESTER_SPI_SLAVE_CTRL, TESTER_SPI_SLAVE_CTRL_SIZE, TESTER_SPI_SLAVE_BIT_ORDER_OFFSET, TESTER_SPI_SLAVE_BIT_ORDER_SIZE);
}

void SPISlaveTester::set_sym_size(uint32_t sym_size)
{
    SPITester::set_sym_size(sym_size, TESTER_SPI_SLAVE_CTRL, TESTER_SPI_SLAVE_CTRL_SIZE, TESTER_SPI_SLAVE_SYM_SIZE_OFFSET, TESTER_SPI_SLAVE_SYM_SIZE_SIZE);
}

void SPISlaveTester::set_duplex_mode(SPITester::SpiDuplex duplex)
{
    SPITester::set_duplex_mode(duplex, TESTER_SPI_SLAVE_CTRL, TESTER_SPI_SLAVE_CTRL_SIZE, TESTER_SPI_SLAVE_DUPLEX_OFFSET, TESTER_SPI_SLAVE_DUPLEX_SIZE);
}

void SPISlaveTester::set_hd_tx_rx_cnt(uint16_t tx_cnt, uint16_t rx_cnt)
{
    SPITester::set_hd_tx_rx_cnt(tx_cnt, rx_cnt, TESTER_SPI_SLAVE_HD_RX_CNT, TESTER_SPI_SLAVE_HD_RX_CNT_SIZE, TESTER_SPI_SLAVE_HD_TX_CNT, TESTER_SPI_SLAVE_HD_TX_CNT_SIZE);
}

void SPISlaveTester::set_spi_master_freq(uint16_t clk_div)
{
    write(TESTER_SPI_SLAVE_CLK_DIV, (uint8_t *)&clk_div, TESTER_SPI_SLAVE_CLK_DIV_SIZE);
}

void SPISlaveTester::set_num_of_symbols(uint16_t num_of_sym)
{
    write(TESTER_SPI_SLAVE_NUM_OF_SYMBOLS, (uint8_t *)&num_of_sym, TESTER_SPI_SLAVE_NUM_OF_SYMBOLS_SIZE);
}

void SPISlaveTester::set_start_delay_us(uint8_t start_delay_us)
{
    write(TESTER_SPI_SLAVE_START_DELAY_US, (uint8_t *)&start_delay_us, TESTER_SPI_SLAVE_START_DELAY_US_SIZE);
}

void SPISlaveTester::set_sym_delay_ns(uint16_t sym_delay_ns)
{
    const uint16_t sym_delay_ticks = (sym_delay_ns + 9) / 10; // round up
    write(TESTER_SPI_SLAVE_SYM_DELAY_TICKS, (uint8_t *)&sym_delay_ticks, TESTER_SPI_SLAVE_SYM_DELAY_TICKS_SIZE);
}

void SPISlaveTester::start_transfer()
{
    uint32_t spi_ctrl = 0;
    read(TESTER_SPI_SLAVE_CTRL, (uint8_t *)&spi_ctrl, TESTER_SPI_SLAVE_CTRL_SIZE);
    spi_ctrl &= ~(((1  <<  TESTER_SPI_SLAVE_START_REQUEST_SIZE) - 1) << TESTER_SPI_SLAVE_START_REQUEST_OFFSET);
    spi_ctrl |= (1  <<  TESTER_SPI_SLAVE_START_REQUEST_OFFSET);
    write(TESTER_SPI_SLAVE_CTRL, (uint8_t *)&spi_ctrl, TESTER_SPI_SLAVE_CTRL_SIZE);
}
