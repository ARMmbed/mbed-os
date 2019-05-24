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

#ifndef UART_TESTER_H
#define UART_TESTER_H

#include "MbedTester.h"


class UARTTester: public MbedTester {
public:

    UARTTester(const PinList *form_factor, const PinList *exclude_pins)
        : MbedTester(form_factor, exclude_pins)
    {

    }

    /**
     * Set the baudrate for uart TX and RX
     *
     * @param baudrate Target baudrate in HZ
     */
    void set_baud(uint32_t baudrate);

    /**
     * Set the number of data bits
     *
     * Supported values for data bits is 1 to 16
     *
     * @param data_bits The number of data bits in this transfer
     */
    void set_bits(uint8_t data_bits);

    /**
     * Set the number of stop bits
     *
     * Supported values for stop bits is 1 to 16
     *
     * @param stop_bits The number of stop bits to end the transfer with
     */
    void set_stops(uint8_t stop_bits);

    /**
     * Enable or disable parity checking
     *
     * @param enable true to enable parity checking, false to disable it
     * @param odd_n_even true of odd parity, false for even
     */
    void set_parity(bool enable, bool odd_n_even);

    /**
     * Enable UART reception
     */
    void rx_start();

    /**
     * Disable UART reception
     */
    void rx_stop();

    /**
     * Get the sum of all bytes received
     *
     * @return the sum of all bytes received
     */
    uint32_t rx_get_checksum();

    /**
     * Get the number of bytes received
     *
     * @return the number of bytes received
     */
    uint32_t rx_get_count();

    /**
     * Get the previous data(s) sent
     *
     * @param prev index of data to get 1 for the previous
     * @return data
     */
    uint16_t rx_get_data(int prev = 1);

    /**
     * Get the number of parity errors that have occurred
     *
     * @return number of parity errors that have occurred
     */
    uint32_t rx_get_parity_errors();

    /**
     * Get the number of stop errors that have occurred
     *
     * @return number of stop errors that have occurred
     */
    uint32_t rx_get_stop_errors();

    /**
     * Get the number of framing errors that have occurred
     *
     * @return number of framing errors that have occurred
     */
    uint32_t rx_get_framing_errors();

    /**
     * Start UART transmission
     */
    void tx_start(bool cts_enabled = false);

    /**
     * Stop UART transmission
     */
    void tx_stop();

    /**
     * Set the delay after the tx_start() call and before the actual start
     * of UART transmission
     *
     * @param delay in nanoseconds
     */
    void tx_set_delay(uint32_t delay_ns);

    /**
     * Set the number of bytes to send
     *
     * @param count Number of bytes to send when started
     */
    void tx_set_count(uint32_t count);

    /**
     * Set next sequence value to send
     *
     * When TX is started 'count' bytes will be sent. Each value will
     * be one greater than the previous.
     *
     * @param value Next value to send
     */
    void tx_set_next(uint16_t value);

    /**
     * Set the delay seen when deasserting the CTS line
     *
     * When delay is set to 0 then transmission will be immediately
     * stopped when CTS goes to 1.
     *
     * @param delay in nanoseconds
     */
    void cts_deassert_delay(uint32_t delay_ns);

};

#endif
