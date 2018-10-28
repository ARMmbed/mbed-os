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
#ifndef CMD_IFCONFIG_H
#define CMD_IFCONFIG_H

#include "NetworkInterface.h"
#include "NetworkStack.h"

/** Get a pointer to a network interface instance
 *
 * Allowed interface types (depend on application configurations):
 * cell0, wlan0, eth0, mesh0
 *
 *  @return         pointer to the network interface, or NULL if unrecognized or ambiguous
 */
NetworkInterface *get_interface(void);

void cmd_ifconfig_init(void);
const char *networkstack_error_to_str(int errorcode);

#endif
