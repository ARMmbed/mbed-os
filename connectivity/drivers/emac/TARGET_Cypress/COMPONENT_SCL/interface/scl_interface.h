/*
 * Copyright 2018-2020 Cypress Semiconductor Corporation
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

#ifndef SCL_INTERFACE_H
#define SCL_INTERFACE_H

#include "rtos/Mutex.h"
#include "OnboardNetworkStack.h"


/** SclSTAInterface class
 *  Shared information 
 */
#define IF_STATUS_ALL_IF_DOWN   0x0
#define IF_STATUS_STA_UP        0x1

enum scl_default_interface_config 
{
    DEFAULT_IF_NOT_SET,
    DEFAULT_IF_STA,
};

struct scl_interface_shared_info_t {
    rtos::Mutex mutex;
    scl_default_interface_config default_if_cfg;
    uint32_t if_status_flags;
    OnboardNetworkStack::Interface *iface_sta;
    scl_interface_shared_info_t() : default_if_cfg(DEFAULT_IF_NOT_SET), if_status_flags(IF_STATUS_ALL_IF_DOWN),
                                    iface_sta(NULL)
    {}
};

extern scl_interface_shared_info_t scl_iface_shared;

#endif
