/* WHD implementation of NetworkInterfaceAPI
 * Copyright (c) 2017-2019 ARM Limited
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

#ifndef WHD_INTERFACE_H
#define WHD_INTERFACE_H

#include "rtos/Mutex.h"
#include "OnboardNetworkStack.h"

/** WhdSTAInterface class
 *  Shared information 
 */
#define IF_STATUS_ALL_IF_DOWN   0x0
#define IF_STATUS_STA_UP        0x1
#define IF_STATUS_SOFT_AP_UP    0x2

enum whd_default_interface_config 
{
    DEFAULT_IF_NOT_SET,
    DEFAULT_IF_STA,
    DEFAULT_IF_SOFT_AP
};

struct whd_interface_shared_info_t {
    rtos::Mutex mutex;
    whd_default_interface_config default_if_cfg;
    uint32_t if_status_flags;
    OnboardNetworkStack::Interface *iface_sta;
    OnboardNetworkStack::Interface *iface_softap;
    whd_interface_shared_info_t() : default_if_cfg(DEFAULT_IF_NOT_SET), if_status_flags(IF_STATUS_ALL_IF_DOWN),
                                    iface_sta(NULL), iface_softap(NULL)
    {}
};

extern whd_interface_shared_info_t whd_iface_shared;

#endif
