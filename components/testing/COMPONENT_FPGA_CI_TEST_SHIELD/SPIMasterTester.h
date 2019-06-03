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

#ifndef SPI_MASTER_TESTER_H
#define SPI_MASTER_TESTER_H

#include "SPITester.h"

class SPIMasterTester: public SPITester {
public:

    SPIMasterTester(const PinList *form_factor, const PinList *exclude_pins)
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
     * Read a checksum of data send to the tester
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
     * Set bit order durring transmission of the spi_slave module.
     *
     *  @param mode Spi clock mode
     */
    void set_bit_order(SpiBitOrder bit_order);

    /**
     * Set symbol size used durring transmission of the spi_slave module.
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
     * Get nano seconds between chip select assertion and the first spi clock edge.
     *
     *  @return nano seconds between chip select assertion and the first spi clock edge.
     *
     *  @note Number of nano seconds is calculated based of number of counted clk changes and
     *        clk frequency (100 MHz => 1 clk tick corresponds to 10 ns).
     *        Accuracy of the returned value is +/- 10 ns.
     */
    uint32_t get_cs_to_first_clk_edge_ns();

    /**
     * Get nano seconds between last spi clock edge and chip select de-assertion.
     *
     *  @return nano seconds between last spi clock edge and chip select de-assertion.
     *
     *  @note Number of nano seconds is calculated based of number of counted clk changes and
     *        clk frequency (100 MHz => 1 clk tick corresponds to 10 ns).
     *        Accuracy of the returned value is +/- 10 ns.
     */
    uint32_t get_last_clk_edge_to_cs_ns();

    /**
     * Get SCLK and SIN states on transmission start and stop.
     *
     *  @return latched SCLK and SIN states on transmission start and stop.
     *
     *  @note Stats are encoded as fallows (bit 0 - LSB):
     *        - bit 0: start SCLK
     *        - bit 1: start SIN
     *        - bit 2: stop SCLK,
     *        - bit 3: stop SIN,
     *        - other: unused
     */
    uint8_t get_start_stop_stats();

};

#endif
