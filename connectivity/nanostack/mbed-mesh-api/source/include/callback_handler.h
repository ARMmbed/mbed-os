/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
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

#ifndef __INCLUDE_CALLBACK_HANDLER_H__
#define __INCLUDE_CALLBACK_HANDLER_H__

#include "mesh_interface_types.h"

#ifdef __cplusplus
#include "MeshInterfaceNanostack.h"
extern "C" {
#endif

void __mesh_handler_c_callback(mesh_connection_status_t state);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
void __mesh_handler_set_callback(Nanostack::Interface *handler);
#endif

#endif /* __INCLUDE_CALLBACK_HANDLER_H__ */
