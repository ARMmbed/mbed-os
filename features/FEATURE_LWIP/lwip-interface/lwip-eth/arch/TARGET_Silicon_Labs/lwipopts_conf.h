/*
 * Copyright (c) 2012-2015, ARM Limited, All Rights Reserved
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

#ifndef LWIPOPTS_CONF_H
#define LWIPOPTS_CONF_H

#define LWIP_TRANSPORT_ETHERNET       (1)
#define CHECKSUM_GEN_IP               (0)
#define CHECKSUM_CHECK_IP             (0)
#define CHECKSUM_GEN_UDP              (0)
#define CHECKSUM_CHECK_UDP            (0)
#define CHECKSUM_GEN_TCP              (0)
#define CHECKSUM_CHECK_TCP            (0)

//#define MEM_ALIGNMENT                 (8)

/* Set up lwIP heap size. The RX and TX buffers get allocated from this! */
#define MEM_SIZE                      (24*1600)

#endif
