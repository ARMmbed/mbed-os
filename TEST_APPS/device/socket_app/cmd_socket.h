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
#ifndef CMD_SOCKET_H_
#define CMD_SOCKET_H_

#include "nsapi_types.h"

int handle_nsapi_error(const char *function, nsapi_error_t ret);
int handle_nsapi_size_or_error(const char *function, nsapi_size_or_error_t ret);
void cmd_socket_init(void);

#endif
