/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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

#ifndef UDP_H_
#define UDP_H_

extern void udp_checksum_write(buffer_t *buf);
extern buffer_t *udp_down(buffer_t *buf);
extern buffer_t *udp_up(buffer_t *buf);

/* The network stack has some inbuilt special behaviour for these known ports */

#define UDP_PORT_ECHO              7        /* Echo Protocol - RFC 862 */
#define UDP_PORT_PANA              716      /* Protocol for carrying Authentication for Network Access - RFC 5191 */
#define UDP_PORT_MLE               19788    /* Mesh Link Establishment - draft */

#endif /* UDP_H_ */
