/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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

#ifndef __RF_DRIVER_STORAGE_STUB_H__
#define __SOME_STUB_H__

#include "stdint.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/rf_driver_storage.h"
#include "platform/arm_hal_phy.h"

typedef struct rf_driver_storage {
    int8_t int8_value;
    int int_value;
    uint16_t uint16_value;
    uint32_t uint32_value;
    bool bool_value;
    arm_device_driver_list_s *list_value;
    arm_net_phy_rx_fn *rx_cb;
    arm_net_phy_tx_done_fn *done_cb;
    uint8_t *uint8_ptr;
    internal_mib_observer *mlme_observer;
    phy_device_driver_s *registered_driver;
}rf_driver_storage_def;

extern rf_driver_storage_def rf_driver_storage_stub;
#endif //__SOME_STUB_H__
