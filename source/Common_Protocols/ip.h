/*
 * Copyright (c) 2015, 2017, Pelion and affiliates.
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

#ifndef _COMMON_PROTOCOLS_IP_H
#define _COMMON_PROTOCOLS_IP_H

/* Traffic class octet defines */
#define IP_TCLASS_DSCP_MASK     0xFC
#define IP_TCLASS_DSCP_SHIFT    2
#define IP_TCLASS_ECN_MASK      0x03

/* Differentiated Services Code Points in bits 0-5 */
#define IP_DSCP_CS0         0
#define IP_DSCP_CS1         8
#define IP_DSCP_AF11        10
#define IP_DSCP_AF12        12
#define IP_DSCP_AF13        14
#define IP_DSCP_CS2         16
#define IP_DSCP_AF21        18
#define IP_DSCP_AF22        20
#define IP_DSCP_AF23        22
#define IP_DSCP_CS3         24
#define IP_DSCP_AF31        26
#define IP_DSCP_AF32        28
#define IP_DSCP_AF33        30
#define IP_DSCP_CS4         32
#define IP_DSCP_AF41        34
#define IP_DSCP_AF42        36
#define IP_DSCP_AF43        38
#define IP_DSCP_CS5         40
#define IP_DSCP_VOICE_ADMIT 44
#define IP_DSCP_EF          46
#define IP_DSCP_CS6         48
#define IP_DSCP_CS7         56

/* Explicit Congestion Notification codepoint in bits 6+7 of traffic class */

#define IP_ECN_NOT_ECT      0 // Not ECN-Capable Transport
#define IP_ECN_ECT_1        1 // ECN-Capable Transport(1)
#define IP_ECN_ECT_0        2 // ECN-Capable Transport(0)
#define IP_ECN_CE           3 // Congestion Experienced

#endif /* _COMMON_PROTOCOLS_IP_H */
