/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef EMAC_MEMBUF_H
#define EMAC_MEMBUF_H

int emac_if_memory_buffer_read(void *buf, unsigned char *eth_frame);
void emac_if_memory_buffer_write(void *buf, unsigned char *eth_frame, bool write_data);

#endif /* EMAC_MEMBUF_H */
