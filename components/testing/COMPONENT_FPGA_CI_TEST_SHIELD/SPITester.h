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

#ifndef SPI_TESTER_H
#define SPI_TESTER_H

#include "MbedTester.h"

class SPITester: public MbedTester {
public:

    enum SpiMode {
        Mode0 = 0,
        Mode1 = 1,
        Mode2 = 2,
        Mode3 = 3
    };

    enum SpiBitOrder {
        MSBFirst = 0,
        LSBFirst = 1
    };

    enum SpiDuplex {
        FullDuplex = 0,
        HalfDuplex = 1
    };

    SPITester(const PinList *form_factor, const PinList *exclude_pins)
        : MbedTester(form_factor, exclude_pins)
    {

    }

protected:
    /*
     * Read the number of transfers which have occurred
     *
     * return The number of SPI transfers that have completed since
     *         spi was reset.
     */
    uint16_t get_transfer_count(uint32_t addr_transfers, uint32_t size_transfers);

    /*
     * Read a checksum of data send to the tester
     *
     * param addr_checksum Address of the FPGA checksum reg.
     * param size_checksum Size of the FPGA checksum reg.
     *
     * return The sum of all bytes sent to the tester since reset.
     */
    uint32_t get_receive_checksum(uint32_t addr_checksum, uint32_t size_checksum);

    /*
     * Set the clock mode of the spi_slave module.
     *
     * param mode Spi clock mode
     * param addr_spi_ctrl Address of the FPGA spi control reg.
     * param size_spi_ctrl Size of the FPGA FPGA spi control reg.
     * param offset_clk_mode Clock mode offset.
     * param size_clk_mode Clock mode size.
     */
    void set_mode(SpiMode mode, uint32_t addr_spi_ctrl, uint32_t size_spi_ctrl, uint32_t offset_clk_mode, uint32_t size_clk_mode);

    /*
     * Set bit order durring transmission of the spi_slave module.
     *
     * param bit_order Spi clock mode
     * param addr_spi_ctrl Address of the FPGA spi control reg.
     * param size_spi_ctrl Size of the FPGA FPGA spi control reg.
     * param offset_bit_order Bit order offset.
     * param size_bit_order Bit order size.
     */
    void set_bit_order(SpiBitOrder bit_order, uint32_t addr_spi_ctrl, uint32_t size_spi_ctrl, uint32_t offset_bit_order, uint32_t size_bit_order);

    /*
     * Set symbol size used durring transmission of the spi_slave module.
     *
     * param sym_size Spi symbol size
     * param addr_spi_ctrl Address of the FPGA spi control reg.
     * param size_spi_ctrl Size of the FPGA FPGA spi control reg.
     * param offset_sym_size Symbol size offset.
     * param size_sym_size Symbol size size.
     */
    void set_sym_size(uint32_t sym_size, uint32_t addr_spi_ctrl, uint32_t size_spi_ctrl, uint32_t offset_sym_size, uint32_t size_sym_size);

    /*
     * Set full-duplex/half-duplex transmission mode of the spi_slave module.
     *
     * param duplex Duplex mode used for the transmission
     * param addr_spi_ctrl Address of the FPGA spi control reg.
     * param size_spi_ctrl Size of the FPGA FPGA spi control reg.
     * param offset_duplex Duplex mode offset.
     * param size_duplex Duplex mode size.
     */
    void set_duplex_mode(SpiDuplex duplex, uint32_t addr_spi_ctrl, uint32_t size_spi_ctrl, uint32_t offset_duplex, uint32_t size_duplex);

    /*
     * Set tx/rx symbol count.
     *
     * tx_cnt TX symbol count
     * rx_cnt RX symbol count
     * param addr_hd_rx_cnt Address of the FPGA half duplex RX count reg.
     * param size_hd_rx_cnt Size of the FPGA half duplex RX count reg.
     * param addr_hd_tx_cnt Address of the FPGA half duplex TX count reg.
     * param size_hd_tx_cnt Size of the FPGA half duplex TX count reg.
     *
     *  note Required only in Half-Duplex mode.
     */
    void set_hd_tx_rx_cnt(uint16_t tx_cnt, uint16_t rx_cnt, uint32_t addr_hd_rx_cnt, uint32_t size_hd_rx_cnt, uint32_t addr_hd_tx_cnt, uint32_t size_hd_tx_cnt);
};

#endif
