/*
 * Copyright (c) 2014-2020, Pelion and affiliates.
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

#ifndef NET_WS_TEST_H_
#define NET_WS_TEST_H_

/* Prevent this file being inserted in public Doxygen generated file
 * this is not part of our external API. */
#ifndef DOXYGEN

/**
 * \file net_ws_test.h
 * \brief Wi-SUN Library Test API.
 *
 * \warning NOTICE! This is test API must not be used externally.
 *
 * \warning This file is not part of the version number control and can change any time.
 *
 */


#ifdef __cplusplus
extern "C" {
#endif

#include "ns_types.h"

/**
 * \brief Set Pan size.
 *
 * Pan size is reported to advertisement. Using this function
 * actual value can be overridden.
 *
 * Set pan_size to 0xffff to stop override
 *
 * \param interface_id               Network Interface
 * \param pan_size                   Pan size reported in advertisements
 *
 * \return 0                         OK
 * \return <0                        Failure
 */
int ws_test_pan_size_set(int8_t interface_id, uint16_t pan_size);

/**
 * \brief Set maximum child count.
 *
 * Maximum amount of children allowed for this device
 *
 * Values above MAC neighbor table - RPL parents - temporary entries will cause undefined behaviour
 *
 * Set child count to 0xffff to stop override
 *
 * \param interface_id               Network Interface
 * \param child_count                   Pan size reported in advertisements
 *
 * \return 0                         OK
 * \return <0                        Failure
 */
int ws_test_max_child_count_set(int8_t interface_id, uint16_t child_count);

/**
 * Sets Group Transient Keys.
 *
 * Sets Group Transient Keys (GTKs). Up to four GTKs can be set (GTKs from index
 * 0 to 3). At least one GTK must be set. GTKs provided in this function call are
 * compared to current GTKs on node or border router GTK storage. If GTK is new
 * or modified it is updated to GTK storage. If GTK is same as previous one, no
 * changes are made. If GTK is NULL then it is removed from GTK storage. When a
 * new GTK is inserted or GTK is modified, GTK lifetime is set to default. If GTKs
 * are set to border router after bootstrap, border router initiates GTK update
 * to network.
 *
 * \param interface_id Network interface ID.
 * \param gtk GTK array, if GTK is not set, pointer for the index shall be NULL.
 *
 * \return 0                         GTKs are set
 * \return <0                        GTK set has failed
 */
int ws_test_gtk_set(int8_t interface_id, uint8_t *gtk[4]);

/**
 * Sets index of active key.
 *
 * Sets index of active Group Transient Key (GTK) to border router. If index is
 * set after bootstrap, initiates dissemination of new key index to network.
 *
 * \param interface_id Network interface ID.
 * \param index Key index
 *
 * \return 0                         Active key index has been set
 * \return <0                        Active key index set has failed
 */
int ws_test_active_key_set(int8_t interface_id, uint8_t index);

/**
 * Sets lifetime for keys
 *
 * Sets Group Transient Key (GTK), Pairwise Master Key (PMK) and
 * Pairwise Transient Key (PTK) lifetimes.
 *
 * \param interface_id Network interface ID.
 * \param gtk_lifetime GTK lifetime in minutes or zero if value is not changed
 * \param pmk_lifetime PMK lifetime in minutes or zero if value is not changed
 * \param ptk_lifetime PTK lifetime in minutes or zero if value is not changed
 *
 * \return 0                         Lifetimes are set
 * \return <0                        Lifetime set has failed
 */
int ws_test_key_lifetime_set(
    int8_t interface_id,
    uint32_t gtk_lifetime,
    uint32_t pmk_lifetime,
    uint32_t ptk_lifetime
);

/**
 * Sets time configurations for GTK keys
 *
 * Sets GTK Revocation Lifetime Reduction and GTK New Activation Time values
 * as parts of the GTK lifetime (e.g. value 3 is 1/3 * lifetime). Sets GTK
 * maximum mismatch time in minutes.
 *
 * \param interface_id Network interface ID.
 * \param revocat_lifetime_reduct GTK Revocation Lifetime Reduction (1 / value * GTK lifetime) or zero if value is not changed
 * \param new_activation_time GTK New Activation Time (1 / value * GTK lifetime) or zero if value is not changed
 * \param new_install_req GTK New Install Required (percent * GTK lifetime) or zero if value is not changed
 * \param max_mismatch GTK maximum mismatch in minutes or zero if value is not changed
 *
 * \return 0                         Lifetimes are set
 * \return <0                        Lifetime set has failed.
 */
int ws_test_gtk_time_settings_set(
    int8_t interface_id,
    uint8_t revocat_lifetime_reduct,
    uint8_t new_activation_time,
    uint8_t new_install_req,
    uint32_t max_mismatch
);

/**
 * Sets Next Group Transient Keys used during GTK life cycle
 *
 * Sets next Group Transient Keys (GTKs) used during GTK life cycle. Up to four
 * GTKs can be set (GTKs from index 0 to 3). When next GTK(s) are set, border
 * router inserts GTKs from the next GTK list into use during GTK update
 * procedure.
 *
 * \param interface_id Network interface ID.
 * \param gtk GTK array, if GTK is not set, pointer for the index shall be NULL.
 *
 * \return 0                         GTKs are set
 * \return <0                        GTK set has failed
 */
int ws_test_next_gtk_set(int8_t interface_id, uint8_t *gtk[4]);

/**
 * Sets 6LoWPAN fragmentation MTU size to test fragmentation
 *
 * \param interface_id Network interface ID.
 * \param mtu_size Size of 6LoWPAN MTU when fragmentation occurs.
 *
 * \return 0                        Success
 * \return <0                       Failure
 */
int ws_test_6lowpan_fragmentation_mtu_size_set(int8_t interface_id, uint16_t mtu_size);

/**
 * Disable First EDFE data packet send.
 *
 * Made only for test purpose for test EDFE client Data wait timeout.
 *
 * \param interface_id Network interface ID.
 * \param skip True for skip first data packet false disable unused flag.
 *
 * \return 0                        Success
 * \return <0                       Failure
 */
void ws_test_skip_edfe_data_send(int8_t interface_id, bool skip);

/**
 * Drop configured EDFE data packets.
 *
 * Made only for test purpose for test EDFE data sender retry send logic.
 *
 * \param interface_id Network interface ID.
 * \param number_of_dropped_frames How many packets will be dropped.
 *
 * \return 0                        Success
 * \return <0                       Failure
 */
int8_t  ws_test_drop_edfe_data_frames(int8_t interface_id, uint8_t number_of_dropped_frames);

/**
 * Set neighbour temporary timeout value.
 *
 * Made only for test purpose for test EDFE certificatiomn test harness.
 *
 * \param interface_id Network interface ID.
 * \param temporary_lifetime 0 to disable test harness, 240-2200 enable longer temporary neighbour lifetime. Values bigger than 2200 will be capped to 2200.
 *
 * \return 0                        Success
 * \return <0                       Failure
 */
int ws_test_neighbour_temporary_lifetime_set(int8_t interface_id, uint32_t temporary_lifetime);

#ifdef __cplusplus
}
#endif

#endif /* DOXYGEN */
#endif /* NET_THREAD_TEST_H_ */
