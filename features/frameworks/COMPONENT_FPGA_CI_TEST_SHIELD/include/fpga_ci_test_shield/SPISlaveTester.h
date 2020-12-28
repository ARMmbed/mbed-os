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

#ifndef SPI_SLAVE_TESTER_H
#define SPI_SLAVE_TESTER_H

#include "SPITester.h"

class SPISlaveTester: public SPITester {
public:

    SPISlaveTester(const PinList *form_factor, const PinList *exclude_pins)
        : SPITester(form_factor, exclude_pins)
    {

    }

    /**
     * Read the number of transfers which have occurred
     *
     * @return The number of SPI transfers that have completed since
     *         spi was reset.
     */
    uint16_t get_transfer_count();

    /**
     * Read a checksum of data sent to the tester
     *
     * @return The sum of all bytes sent to the tester since reset.
     */
    uint32_t get_receive_checksum();

    /**
     * Set the clock mode of the spi_slave module.
     *
     *  @param mode Spi clock mode
     */
    void set_mode(SpiMode mode);

    /**
     * Set bit order during transmission of the spi_slave module.
     *
     *  @param mode Spi clock mode
     */
    void set_bit_order(SpiBitOrder bit_order);

    /**
     * Set symbol size used during transmission of the spi_slave module.
     *
     *  @param mode Spi clock mode
     */
    void set_sym_size(uint32_t sym_size);

    /**
     * Set full-duplex/half-duplex transmission mode of the spi_slave module.
     *
     *  @param duplex duplex mode used for the transmission
     */
    void set_duplex_mode(SpiDuplex duplex);

    /**
     * Set tx/rx symbol count.
     *
     *  @tx_cnt TX symbol count
     *  @rx_cnt RX symbol count
     *
     *  @note Required only in Half-Duplex mode.
     */
    void set_hd_tx_rx_cnt(uint16_t tx_cnt, uint16_t rx_cnt);

    /**
     * Set divisor to generate spi clock from FPGA base clock (100 MHz).
     *
     *  @clk_div clock divisor.
     */
    void set_spi_master_freq(uint16_t clk_div);

    /**
     * Set number of symbols to be transmitted by spi master.
     *
     *  @num_of_sym Number of symbols to be transmitted by spi master.
     */
    void set_num_of_symbols(uint16_t num_of_sym);

    /**
     * Set delay in us between start request and start of transmission.
     *
     *  @start_delay_us Delay in us between start request and start of transmission.
     */
    void set_start_delay_us(uint8_t start_delay_us);

    /**
     * Set delay in ns between transmission of successive symbols.
     *
     *  @sym_delay_ns Delay in ns between transmission of successive symbols.
     */
    void set_sym_delay_ns(uint16_t sym_delay_ns);

    /**
     * Request transmission start from FPGA master.
     *
     *  @note Transmission will be started after the specified delay.
     */
    void start_transfer();
};

#endif
