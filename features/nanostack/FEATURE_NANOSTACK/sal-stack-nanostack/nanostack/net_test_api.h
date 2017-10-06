/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
 */

/**
 * \file net_ipv6_api.h
 * \brief IPv6 configuration API.
 */

#ifndef NET_TEST_API_H_
#define NET_TEST_API_H_

#include "ns_types.h"

/**
 * \brief Makes TCP protocol drop given number of packets from a particular state (TX side).
 *
 * Testing API for TCP retransmission mechanism after a packet is dropped in a particular state.
 *
 * \param state Particular TCP state - Identified by its number from 1-11. Numbering is from the SNMP MIB - RFC 4022.
 * \param count No. of packets to be dropped
 * \return 0 OK
 * \return <0 If request can't be fulfilled, i.e., Not test environment.
 */
int8_t arm_nwk_test_tcp_drop_tx(int state, uint8_t count);

/**
 * \brief Makes TCP protocol drop given number of packets from a particular state (RX side).
 *
 * Testing API for TCP to drop  received packets.
 *
 * \param state Particular TCP state - Identified by its number from 1-11. Numbering is from the SNMP MIB - RFC 4022.
 * \param count No. of packets to be dropped
 * \return 0 OK
 * \return <0 If request can't be fulfilled, i.e., Not test environment.
 */
int8_t arm_nwk_test_tcp_drop_rx(int state, uint8_t count);

/**
 * \brief Resets drop counters.
 *
 * Testing API for TCP reset any packet drop counters.
 */
void arm_nwk_test_tcp_drop_reset(void);

#endif //NET_TEST_API_H_
