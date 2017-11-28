/*
 * Copyright (c) 2014, 2016, Arm Limited and affiliates.
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

#include "nsconfig.h"
#ifdef HAVE_LOCAL_SOCKET

#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/rf_driver_storage.h"
#include "Core/include/ns_buffer.h"

#include "protocol_tun_driver_stub.h"

protocol_tun_driver_stub_def protocol_tun_driver_stub;

int8_t arm_tun_driver_tasklet_init(void)
{
    return protocol_tun_driver_stub.int8_value;
}

int8_t arm_tun_interface_tx_done_handler(arm_device_driver_list_s *driver)
{
    return protocol_tun_driver_stub.int8_value;
}


int arm_tun_driver_data_push(buffer_t *b, arm_device_driver_list_s *driver)
{
    return protocol_tun_driver_stub.int8_value;
}
#endif

