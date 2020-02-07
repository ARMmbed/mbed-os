/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
#include "mbed.h"
#include "rtos.h"
#include "sw_mac.h"
#include "ns_hal_init.h"
#define MBED_CMDLINE_MAX_LINE_LENGTH 250
#include "ns_cmdline.h"

#include "mac_commands.h"

#define HEAP_FOR_MAC_TESTER_SIZE 10000
#define RX_BUFFER_SIZE 512

#define ATMEL   1
#define MCR20   2
#define OTHER   3

#define TRACE_GROUP "Main"
#include "mbed-trace/mbed_trace.h"

#include "NanostackRfPhy.h"

#if !DEVICE_802_15_4_PHY
#error [NOT_SUPPORTED] No 802.15.4 RF driver found for this target
#endif

#ifndef ICETEA_MAC_TESTER_ENABLED
#error [NOT_SUPPORTED] Skipping Nanostack MAC tester application.
#endif

extern mac_api_s *mac_interface;
UnbufferedSerial pc(USBTX, USBRX);
osThreadId_t main_thread;
static CircularBuffer<uint8_t, RX_BUFFER_SIZE> rx_buffer;
static uint8_t ns_heap[HEAP_FOR_MAC_TESTER_SIZE];

static void app_heap_error_handler(heap_fail_t event)
{
    tr_error("Heap error (%d), app_heap_error_handler()", event);
    switch (event) {
        case NS_DYN_MEM_NULL_FREE:
        case NS_DYN_MEM_DOUBLE_FREE:
        case NS_DYN_MEM_ALLOCATE_SIZE_NOT_VALID:
        case NS_DYN_MEM_POINTER_NOT_VALID:
        case NS_DYN_MEM_HEAP_SECTOR_CORRUPTED:
        case NS_DYN_MEM_HEAP_SECTOR_UNITIALIZED:
            break;
        default:
            break;
    }
    while (1);
}

static void rx_interrupt(void)
{
    uint8_t c;
    pc.read(&c, 1);
    rx_buffer.push(c);
    if (main_thread != NULL) {
        osThreadFlagsSet(main_thread, 1);
    }
}

static void handle_rx_data(void)
{
    bool exit = false;
    uint8_t data;

    while (1) {
        exit = !rx_buffer.pop(data);
        if (exit) {
            break;
        }
        cmd_char_input(data);
    }
}


static int mac_prepare(void)
{
    NanostackRfPhy &rf_phy = NanostackRfPhy::get_default_instance();
    int8_t rf_driver_id = rf_phy.rf_register();
    uint8_t rf_eui64[8];

    if (rf_driver_id < 0) {
        tr_error("Failed to register RF driver.");
        return -1;
    }
    rf_phy.get_mac_address(rf_eui64);
    mac_description_storage_size_t mac_description;
    mac_description.device_decription_table_size = DEVICE_DESCRIPTOR_TABLE_SIZE;    /** MAC Device description list size */
    mac_description.key_description_table_size = KEY_DESCRIPTOR_TABLE_SIZE;         /** MAC Key description list size */
    mac_description.key_lookup_size = LOOKUP_DESCRIPTOR_TABLE_SIZE;                 /** Key description key lookup list size */
    mac_description.key_usage_size = USAGE_DESCRIPTOR_TABLE_SIZE;                   /** Key description key usage list size */
    tr_info("Registered RF driver with id: %hhu, EUI64: %s", rf_driver_id, mbed_trace_array(rf_eui64, 8));
    mac_interface = ns_sw_mac_create(rf_driver_id, &mac_description);
    if (!mac_interface) {
        tr_error("Failed to create SW MAC.");
        return -2;
    }

    return mac_interface->mac_initialize(mac_interface, mac_data_confirm_handler,
                                         mac_data_indication_handler, mac_purge_confirm_handler, mac_mlme_confirm_handler,
                                         mac_mlme_indication_handler, rf_driver_id);
}

static void cmd_ready_cb(int retcode)
{
    cmd_next(retcode);
}

static void trace_printer(const char *str)
{
    printf("%s\n", str);
    cmd_output();
    fflush(stdout);
}

int main(void)
{
    main_thread = ThisThread::get_id();
    pc.baud(MBED_CONF_PLATFORM_STDIO_BAUD_RATE);
    pc.attach(rx_interrupt);
    ns_hal_init(ns_heap, HEAP_FOR_MAC_TESTER_SIZE, app_heap_error_handler, NULL);
    mbed_trace_init();
    mbed_trace_print_function_set(trace_printer);
    cmd_init(&default_cmd_response_out);
    cmd_set_ready_cb(cmd_ready_cb);
    mac_commands_init();

    if (mac_prepare() != 0) {
        return -1;
    }
    tr_info("Created driver & SW MAC");

    while (true) {
        ThisThread::flags_wait_any(1);
        handle_rx_data();
    }
    return 0;
}
