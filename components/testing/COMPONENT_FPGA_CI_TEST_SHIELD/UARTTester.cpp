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

#include "UARTTester.h"
#include "fpga_config.h"

void UARTTester::set_baud(uint32_t baudrate)
{
    uint32_t divisor = TESTER_CLOCK_FREQUENCY_HZ / baudrate;
    // Baud divisor is only 16 bits
    MBED_ASSERT((divisor & 0xFFFF0000) == 0);
    write(TESTER_UART_BAUD_DIVISOR, (uint8_t *)&divisor, TESTER_UART_BAUD_DIVISOR_SIZE);
}

void UARTTester::set_bits(uint8_t data_bits)
{
    // Check for supported range
    MBED_ASSERT((data_bits >= 1) && (data_bits <= 16));
    write(TESTER_UART_BIT_COUNT, &data_bits, sizeof(data_bits));
}

void UARTTester::set_stops(uint8_t stop_bits)
{
    // Check for supported range
    MBED_ASSERT((stop_bits >= 1) && (stop_bits <= 16));
    write(TESTER_UART_STOP_COUNT, &stop_bits, sizeof(stop_bits));
}

void UARTTester::set_parity(bool enable, bool odd_n_even)
{
    uint8_t parity = (enable ? TESTER_UART_PARITY_ENABLE : 0) |
                     (odd_n_even ? TESTER_UART_PARITY_ODD_N_EVEN : 0);
    write(TESTER_UART_PARITY, &parity, sizeof(parity));
}

void UARTTester::rx_start()
{
    uint8_t data = TESTER_UART_RX_CONTROL_ENABLE;
    write(TESTER_UART_RX_CONTROL, &data, sizeof(data));
}

void UARTTester::rx_stop()
{
    uint8_t data = 0;
    write(TESTER_UART_RX_CONTROL, &data, sizeof(data));
}

uint32_t UARTTester::rx_get_checksum()
{
    uint32_t checksum = 0;
    read(TESTER_UART_RX_CHECKSUM, (uint8_t *)&checksum, sizeof(checksum));
    return checksum;
}

uint32_t UARTTester::rx_get_count()
{
    uint32_t count = 0;
    read(TESTER_UART_RX_COUNT, (uint8_t *)&count, sizeof(count));
    return count;
}

uint16_t UARTTester::rx_get_data(int prev)
{
    MBED_ASSERT((prev >= 1) && (prev <= 4));
    uint16_t data = 0;
    read(TESTER_UART_RX_PREV_1 - (prev - 1) * 2, (uint8_t *)&data, sizeof(data));
    return data;
}

uint32_t UARTTester::rx_get_parity_errors()
{
    uint32_t errors = 0;
    read(TESTER_UART_RX_PARITY_ERRORS, (uint8_t *)&errors, sizeof(errors));
    return errors;
}

uint32_t UARTTester::rx_get_stop_errors()
{
    uint32_t errors = 0;
    read(TESTER_UART_RX_STOP_ERRORS, (uint8_t *)&errors, sizeof(errors));
    return errors;
}

uint32_t UARTTester::rx_get_framing_errors()
{
    uint32_t errors = 0;
    read(TESTER_UART_RX_FRAMING_ERRORS, (uint8_t *)&errors, sizeof(errors));
    return errors;
}

void UARTTester::tx_start(bool cts_enabled)
{
    uint32_t control = TESTER_UART_TX_CONTROL_ENABLE | (cts_enabled ? TESTER_UART_TX_CONTROL_ENABLE_CTS : 0);
    write(TESTER_UART_TX_CONTROL, (uint8_t *)&control, sizeof(control));
}

void UARTTester::tx_stop()
{
    uint32_t control = 0;
    write(TESTER_UART_TX_CONTROL, (uint8_t *)&control, sizeof(control));
}

void UARTTester::tx_set_delay(uint32_t delay_ns)
{
    uint32_t delay_clks = (delay_ns + TESTER_CLOCK_PERIOD_NS - 1) / TESTER_CLOCK_PERIOD_NS;
    write(TESTER_UART_TX_DELAY, (uint8_t *)&delay_clks, sizeof(delay_clks));
}

void UARTTester::tx_set_count(uint32_t count)
{
    write(TESTER_UART_TX_COUNT, (uint8_t *)&count, sizeof(count));
}

void UARTTester::tx_set_next(uint16_t value)
{
    write(TESTER_UART_TX_NEXT, (uint8_t *)&value, sizeof(value));
}

void UARTTester::cts_deassert_delay(uint32_t delay_ns)
{
    uint32_t delay_clks = delay_ns / TESTER_CLOCK_PERIOD_NS;
    write(TESTER_UART_CTS_DEACTIVATE_DELAY, (uint8_t *)&delay_clks, sizeof(delay_clks));
}
