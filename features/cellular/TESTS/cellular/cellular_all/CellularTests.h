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
#ifndef CELLULAR_TESTS_H
#define CELLULAR_TESTS_H

#include "CellularUtil.h" // for CELLULAR_ helper macros
#include "CellularTargets.h"

#ifdef CELLULAR_DEVICE

#include "mbed_events.h"

#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "CellularLog.h"

#include CELLULAR_STRINGIFY(CELLULAR_DEVICE.h)
extern EventQueue queue;
extern CELLULAR_DEVICE cellularDevice;

extern UARTSerial serial;
extern CellularNetwork *network;

extern CellularSMS *sms;
extern CellularPower *pwr;
extern CellularSIM *sim;


/**
 * TEST CASES DEFINED HERE AND in main.cpp
 */

// power
void test_create_power(void);

// SIM
void test_get_sim_state(void);
void test_set_pin(void);
void test_change_pin(void);

// sms
void test_sms_init(void);

// network
void test_attach(void);
void test_connect(void);
void test_get_ip_address(void);
void test_disconnect(void);

// stack
void test_socket_open(void);
void test_socket_bind(void);
/*
void test_socket_set_blocking();
void test_socket_send_receive_blocking();
*/
void test_socket_set_non_blocking();
void test_socket_send_receive_non_blocking();
void test_socket_close(void);

// Test closing all interface via device
void test_close_interfaces(void);

#endif // CELLULAR_DEVICE

#endif // CELLULAR_TESTS_H
