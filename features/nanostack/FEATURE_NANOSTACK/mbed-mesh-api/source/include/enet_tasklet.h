/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
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

#ifndef ENET_TASKLET_H
#define ENET_TASKLET_H


#ifdef __cplusplus
extern "C" {
#endif

void enet_tasklet_init(void);
uint8_t enet_tasklet_network_init(int8_t);
int8_t enet_tasklet_connect(void (*)(mesh_connection_status_t mesh_status), int8_t nwk_interface_id);
void enet_tasklet_disconnect();
int8_t enet_tasklet_get_ip_address(char *address, int8_t len);

#ifdef __cplusplus
}
#endif

#endif // ENET_TASKLET_H
