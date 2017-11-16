/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "test_protocol_6lowpan_interface.h"
#include <string.h>
#include "NWK_INTERFACE/Include/protocol.h"


bool test_nwk_6lowpan_up()
{
    protocol_interface_info_entry_t info;
    arm_15_4_mac_parameters_t mac_params;
    memset(&info, 0, sizeof(protocol_interface_info_entry_t));
    memset(&mac_params, 0, sizeof(arm_15_4_mac_parameters_t));
    info.mac_parameters = &mac_params;
    nwk_6lowpan_up(&info);
    return true;
}

