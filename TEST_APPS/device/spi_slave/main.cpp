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
#include <stdio.h>
#include <stdarg.h>
#include "mbed.h"
#include "mbed-client-cli/ns_cmdline.h"
#include "spi_slave.h"

/**
 * Macros for setting console flow control.
 */
#define CONSOLE_FLOWCONTROL_RTS     1
#define CONSOLE_FLOWCONTROL_CTS     2
#define CONSOLE_FLOWCONTROL_RTSCTS  3
#define mbed_console_concat_(x) CONSOLE_FLOWCONTROL_##x
#define mbed_console_concat(x) mbed_console_concat_(x)
#define CONSOLE_FLOWCONTROL mbed_console_concat(MBED_CONF_TARGET_CONSOLE_UART_FLOW_CONTROL)

#define SERIAL_CONSOLE_BAUD_RATE 115200

static config_test_case_t tc_config;
static spi_t spi_slave = { 0 };

void cmd_ready_cb(int retcode)
{
    cmd_next(retcode);
}

void wrap_printf(const char *f, va_list a)
{
    vprintf(f, a);
}

int validate_config_callback(int argc, char *argv[])
{
    int32_t duplex_buf;
    int32_t mode_buf;

    cmd_parameter_int(argc, argv, "symbol_size", (int32_t*)&tc_config.symbol_size);
    cmd_parameter_int(argc, argv, "mode", &mode_buf);
    cmd_parameter_int(argc, argv, "bit_ordering", (int32_t*)&tc_config.bit_ordering);
    cmd_parameter_int(argc, argv, "freq_hz", (int32_t*)&tc_config.freq_hz);
    cmd_parameter_int(argc, argv, "master_tx_cnt", (int32_t*)&tc_config.master_tx_cnt);
    cmd_parameter_int(argc, argv, "master_rx_cnt", (int32_t*)&tc_config.master_rx_cnt);
    cmd_parameter_int(argc, argv, "slave_tx_cnt", (int32_t*)&tc_config.slave_tx_cnt);
    cmd_parameter_int(argc, argv, "slave_rx_cnt", (int32_t*)&tc_config.slave_rx_cnt);
    cmd_parameter_bool(argc, argv, "master_tx_defined", &tc_config.master_tx_defined);
    cmd_parameter_bool(argc, argv, "master_rx_defined", &tc_config.master_rx_defined);
    cmd_parameter_bool(argc, argv, "slave_tx_defined", &tc_config.slave_tx_defined);
    cmd_parameter_bool(argc, argv, "slave_rx_defined", &tc_config.slave_rx_defined);
    cmd_parameter_bool(argc, argv, "auto_ss", &tc_config.auto_ss);
    cmd_parameter_int(argc, argv, "duplex", &duplex_buf);
    cmd_parameter_bool(argc, argv, "sync", &tc_config.sync);
    tc_config.duplex = (duplex_t)duplex_buf;
    tc_config.mode = (_spi_mode_t)mode_buf;

    dump_config(&tc_config);

    return check_capabilities(tc_config.symbol_size, true, tc_config.duplex, tc_config.sync);
}

int init_test_callback(int argc, char *argv[])
{
    int result = test_init_slave(&spi_slave, &tc_config);

    return result;
}

int exec_test_callback(int argc, char *argv[])
{
    int result = test_transfer_slave(&spi_slave, &tc_config);

    return result;
}

int finish_test_callback(int argc, char *argv[])
{
    int result = test_finish_slave(&spi_slave, &tc_config);

    return result;
}

int main()
{
    cmd_init(&wrap_printf);
    cmd_add("validate_config", validate_config_callback, 0, 0);
    cmd_add("init_test", init_test_callback, 0, 0);
    cmd_add("exec_test", exec_test_callback, 0, 0);
    cmd_add("finish_test", finish_test_callback, 0, 0);

    int c;
    while ((c = getchar()) != EOF) {
        cmd_char_input(c);
    }
    return 0;
}

FileHandle *mbed::mbed_override_console(int)
{
    static UARTSerial console(STDIO_UART_TX, STDIO_UART_RX, SERIAL_CONSOLE_BAUD_RATE);
#if CONSOLE_FLOWCONTROL == CONSOLE_FLOWCONTROL_RTS
    console.set_flow_control(SerialBase::RTS, STDIO_UART_RTS, NC);
#elif CONSOLE_FLOWCONTROL == CONSOLE_FLOWCONTROL_CTS
    console.set_flow_control(SerialBase::CTS, NC, STDIO_UART_CTS);
#elif CONSOLE_FLOWCONTROL == CONSOLE_FLOWCONTROL_RTSCTS
    console.set_flow_control(SerialBase::RTSCTS, STDIO_UART_RTS, STDIO_UART_CTS);
#endif
    return &console;
}

