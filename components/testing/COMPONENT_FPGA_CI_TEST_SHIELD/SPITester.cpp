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

#include "SPITester.h"
#include "fpga_config.h"

uint16_t SPITester::get_transfer_count(uint32_t addr_transfers, uint32_t size_transfers)
{
    uint16_t transfers = 0;
    MBED_ASSERT(sizeof(transfers) == size_transfers);
    read(addr_transfers, (uint8_t *)&transfers, sizeof(transfers));
    return transfers;
}

uint32_t SPITester::get_receive_checksum(uint32_t addr_checksum, uint32_t size_checksum)
{
    uint32_t checksum = 0;
    MBED_ASSERT(sizeof(checksum) == size_checksum);
    read(addr_checksum, (uint8_t *)&checksum, sizeof(checksum));
    return checksum;
}

void SPITester::set_mode(SPITester::SpiMode mode, uint32_t addr_spi_ctrl, uint32_t size_spi_ctrl, uint32_t offset_clk_mode, uint32_t size_clk_mode)
{
    uint32_t spi_ctrl = 0;
    read(addr_spi_ctrl, (uint8_t *)&spi_ctrl, size_spi_ctrl);
    spi_ctrl &= ~(((1  <<  size_clk_mode) - 1) << offset_clk_mode);
    spi_ctrl |= (mode  <<  offset_clk_mode);
    write(addr_spi_ctrl, (uint8_t *)&spi_ctrl, size_spi_ctrl);
}

void SPITester::set_bit_order(SPITester::SpiBitOrder bit_order, uint32_t addr_spi_ctrl, uint32_t size_spi_ctrl, uint32_t offset_bit_order, uint32_t size_bit_order)
{
    uint32_t spi_ctrl = 0;
    read(addr_spi_ctrl, (uint8_t *)&spi_ctrl, size_spi_ctrl);
    spi_ctrl &= ~(((1  <<  size_bit_order) - 1) << offset_bit_order);
    spi_ctrl |= (bit_order  <<  offset_bit_order);
    write(addr_spi_ctrl, (uint8_t *)&spi_ctrl, size_spi_ctrl);
}

void SPITester::set_sym_size(uint32_t sym_size, uint32_t addr_spi_ctrl, uint32_t size_spi_ctrl, uint32_t offset_sym_size, uint32_t size_sym_size)
{
    uint32_t spi_ctrl = 0;
    read(addr_spi_ctrl, (uint8_t *)&spi_ctrl, size_spi_ctrl);
    spi_ctrl &= ~(((1  <<  size_sym_size) - 1) << offset_sym_size) ;
    spi_ctrl |= (sym_size  <<  offset_sym_size);
    write(addr_spi_ctrl, (uint8_t *)&spi_ctrl, size_spi_ctrl);
}

void SPITester::set_duplex_mode(SPITester::SpiDuplex duplex, uint32_t addr_spi_ctrl, uint32_t size_spi_ctrl, uint32_t offset_duplex, uint32_t size_duplex)
{
    uint32_t spi_ctrl = 0;
    read(addr_spi_ctrl, (uint8_t *)&spi_ctrl, size_spi_ctrl);
    spi_ctrl &= ~(((1  <<  size_duplex) - 1) << offset_duplex);
    spi_ctrl |= (duplex  <<  offset_duplex);
    write(addr_spi_ctrl, (uint8_t *)&spi_ctrl, size_spi_ctrl);
}

void SPITester::set_hd_tx_rx_cnt(uint16_t tx_cnt, uint16_t rx_cnt, uint32_t addr_hd_rx_cnt, uint32_t size_hd_rx_cnt, uint32_t addr_hd_tx_cnt, uint32_t size_hd_tx_cnt)
{
    write(addr_hd_rx_cnt, (uint8_t *)&rx_cnt, size_hd_rx_cnt);
    write(addr_hd_tx_cnt, (uint8_t *)&tx_cnt, size_hd_tx_cnt);
}
