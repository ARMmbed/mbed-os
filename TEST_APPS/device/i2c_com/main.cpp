/*
 * Copyright (c) 2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdarg.h>

#include "common.h"
#include "mbed.h"
#include "mbed-client-cli/ns_cmdline.h"
#include "master.h"
#if DEVICE_I2CSLAVE
#include "slave.h"
#endif // DEVICE_I2CSLAVE

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


#define BLOCKING_WRITE_READ     "blocking_write-read"
#if DEVICE_I2C_ASYNCH
#define ASYNC_WRITE_READ        "async_write-read"
#define ASYNC_TRANSFER          "async_transfer"
#endif


#define GET_PARAM_STR(key, str) \
    if (!cmd_parameter_val(argc, argv, key, &str)) { \
        printf("invalid parameter key \"%s\" not found!!!\r\n", key); \
        return CMDLINE_RETCODE_INVALID_PARAMETERS; \
    }

#define GET_PARAM_INT(key, value, optional) \
    if (!cmd_parameter_int(argc, argv, key, &value) && !optional) { \
        printf("invalid parameter key \"%s\" not found!!!\r\n", key); \
        return CMDLINE_RETCODE_INVALID_PARAMETERS;  \
    }

#define GET_PARAM_BOOL(key, value, optional) \
    if (!cmd_parameter_bool(argc, argv, key, &value) && !optional) {    \
        printf("invalid parameter key \"%s\" not found!!!\r\n", key); \
        return CMDLINE_RETCODE_INVALID_PARAMETERS;  \
    }


bool supports_async()
{
#if DEVICE_I2C_ASYNCH
    return true;
#else
    return false;
#endif
}

int send_capabilities(int argc, char *argv[])
{
    i2c_capabilities_t cap;
    i2c_get_capabilities(&cap);
    cmd_printf("min_freq: %u max_freq: %u slave_mode: %s 10bit_addressing: %s multi_master: %s clock_stretching: %s async: %s\r\n",
               cap.minimum_frequency, cap.maximum_frequency, cap.supports_slave_mode ? "True" : "False", cap.supports_10bit_addressing ? "True" : "False",
               cap.supports_multi_master ? "True" : "False", cap.supports_clock_stretching ? "True" : "False", supports_async() ? "True" : "False");

    return CMDLINE_RETCODE_SUCCESS;
}

int test_init_master(int argc, char *argv[])
{
    bool ret = false;

    int32_t frequency;
    GET_PARAM_INT("frequency:", frequency, false);

    ret = master_init((uint32_t)frequency);

    return ret ? CMDLINE_RETCODE_SUCCESS : CMDLINE_RETCODE_FAIL;
}

int test_deinit_master(int argc, char *argv[])
{
    bool ret = false;

    ret = master_deinit();

    return ret ? CMDLINE_RETCODE_SUCCESS : CMDLINE_RETCODE_FAIL;
}

int test_exec_master(int argc, char *argv[])
{
    char *com_type;
    GET_PARAM_STR("communication:", com_type);

    int32_t write_size;
    GET_PARAM_INT("write_size:", write_size, false);

    int32_t write_result_size = write_size;
    GET_PARAM_INT("write_resulting_size:", write_result_size, true);

    int32_t read_size;
    GET_PARAM_INT("read_size:", read_size, false);

    int32_t read_result_size = read_size;
    GET_PARAM_INT("read_resulting_size:", read_result_size, true);

    int32_t iterations;
    GET_PARAM_INT("iterations:", iterations, false);

    int32_t address7 = -1;
    GET_PARAM_INT("address7:", address7, true);

    int32_t address10 = -1;
    GET_PARAM_INT("address10:", address10, true);

    uint32_t write_address = address7 != -1 ? MAKE_7BIT_WRITE_ADDRESS(address7) : MAKE_10BIT_WRITE_ADDRESS(address10);
    uint32_t read_address = address7 != -1 ? MAKE_7BIT_READ_ADDRESS(address7) : MAKE_10BIT_READ_ADDRESS(address10);

    bool ret = false;
    if (strcmp(BLOCKING_WRITE_READ, com_type) == 0) {
        if (write_size && read_size) {
            ret = master_write_read(write_size, read_size, write_address, read_address, iterations);
        } else if (write_size) {
            ret = master_write(write_size, write_result_size, write_address, iterations);
        } else if (read_size) {
            ret = master_read(read_size, read_result_size, read_address, iterations);
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
#if DEVICE_I2C_ASYNCH
    else if (strcmp(ASYNC_WRITE_READ, com_type) == 0) {
        if (write_size && read_size) {
            ret = master_write_read_async(write_size, read_size, address7, iterations);
        } else if (write_size) {
            ret = master_write_async(write_size, write_result_size, address7, iterations);
        } else if (read_size) {
            ret = master_read_async(write_size, read_result_size, address7, iterations);
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else if (strcmp(ASYNC_TRANSFER, com_type) == 0) {
        ret = master_transfer_async(write_size, read_size, address7, iterations);
    }
#endif
    else {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }

    return ret ? CMDLINE_RETCODE_SUCCESS : CMDLINE_RETCODE_FAIL;
}

#if DEVICE_I2CSLAVE
int test_init_slave(int argc, char *argv[])
{
    bool ret = false;

    int32_t frequency;
    GET_PARAM_INT("frequency:", frequency, false);

    ret = slave_init((uint32_t)frequency);

    return ret ? CMDLINE_RETCODE_SUCCESS : CMDLINE_RETCODE_FAIL;
}

int test_deinit_slave(int argc, char *argv[])
{
    bool ret = false;

    ret = slave_deinit();

    return ret ? CMDLINE_RETCODE_SUCCESS : CMDLINE_RETCODE_FAIL;
}

int test_exec_slave(int argc, char *argv[])
{
    int32_t write_size;
    GET_PARAM_INT("write_size:", write_size, false);

    int32_t write_result_size = write_size;
    GET_PARAM_INT("write_resulting_size:", write_result_size, true);

    int32_t read_size;
    GET_PARAM_INT("read_size:", read_size, false);

    int32_t read_result_size = read_size;
    GET_PARAM_INT("read_resulting_size:", read_result_size, true);

    int32_t iterations;
    GET_PARAM_INT("iterations:", iterations, false);

    int32_t address7 = -1;
    GET_PARAM_INT("address7:", address7, true);

    int32_t address10 = -1;
    GET_PARAM_INT("address10:", address10, true);

    bool ret = false;
    uint32_t address = address7 != -1 ? MAKE_7BIT_SLAVE_ADDRESS(address7) : address10;
    ret = slave_transfer(write_size, write_result_size, read_size, read_result_size, address, iterations);

    return ret ? CMDLINE_RETCODE_SUCCESS : CMDLINE_RETCODE_FAIL;
}

int test_finish_slave(int argc, char *argv[])
{
    slave_finish();

    return CMDLINE_RETCODE_SUCCESS;
}
#endif // DEVICE_I2CSLAVE

void cmd_ready_cb(int retcode)
{
    cmd_next(retcode);
}

void wrap_printf(const char *f, va_list a)
{
    vprintf(f, a);
}

int main()
{
#if I2C_DEBUG_PIN_MASTER==1 || I2C_DEBUG_PIN_SLAVE==1
    test_pin_init();
#endif

    cmd_init(&wrap_printf);
    cmd_add("get_capabilities", send_capabilities, "Send capabilities.", 0);

    cmd_add("test_init_master", test_init_master, "Test initialize.", 0);
    cmd_add("test_exec_master", test_exec_master, "Test execute.", 0);
    cmd_add("test_deinit_master", test_deinit_master, "Test deinitialize.", 0);
#if DEVICE_I2CSLAVE
    cmd_add("test_init_slave", test_init_slave, "Test initialize.", 0);
    cmd_add("test_exec_slave", test_exec_slave, "Test execute.", 0);
    cmd_add("test_deinit_slave", test_deinit_slave, "Test deinitialize.", 0);
    cmd_add("test_stop_slave", test_finish_slave, "Test deinitialize.", 0);
#endif // DEVICE_I2CSLAVE

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
