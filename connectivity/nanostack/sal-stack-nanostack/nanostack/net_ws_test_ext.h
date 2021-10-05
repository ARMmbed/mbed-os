/*
 * Copyright (c) 2021, Pelion and affiliates.
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

#ifndef NET_WS_TEST_EXT_H_
#define NET_WS_TEST_EXT_H_

/**
 * \file net_ws_test_ext.h
 * \brief Wi-SUN Library External Test API.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ns_types.h"

/* Test procedure triggers
 *
 * Example about using the triggers during bootstrap to trigger
 * messages and state transitions.
 *
 * Border Router              Node
 *
 *                            Join state 1 (select PAN)
 *
 * PROC_PA
 * ------- PAN Advertisement------------>
 *
 *                            PROC_EAPOL
 *                            Select EAPOL target
 *                            Join state 2 (authenticate)
 * <------ EAPOL authentication -------->
 *                            Join State 3 (acquire PAN configuration)
 *
 * PROC_PC
 * ------- PAN Configuration ----------->
 *                            Join state 4 (configure routing)
 *
 * PROC_DIO
 * ------- DIO ------------------------->
 *                            Neighbor discovery (NS probing for ETX)
 *                            Create RPL candidate parent set
 *
 *                            PROC_RPL
 *                            Select RPL parent
 * <------ DHCP ------------------------>
 *
 *                            PROC_DAO
 * <------ DAO --------------------------
 * ------- DAO acknowledge ------------->
 *
 *                            Join state 5 (operational)
 *
 *
 * On automatic mode the PROC_PAS, PROC_EAPOL, PROC_PCS, PROC_DIS and PROC_RPL
 * will be triggered automatically by the node during the bootstrap.
 *
 */

/**
 * @brief Test procedure triggers.
 */
typedef enum {
    PROC_DIS,       /* trigger DODAG information object solicit (node) */
    PROC_DIO,       /* trigger DODAG information object (BR, node) */
    PROC_DAO,       /* trigger Destination advertisement object (node) */

    PROC_PAS,       /* trigger PAN Advertisement Solicit (node) */
    PROC_PA,        /* trigger PAN Advertisement (BR, node) */
    PROC_PCS,       /* trigger PAN Configuration Solicit (node) */
    PROC_PC,        /* trigger PAN Configuration (BR, node) */

    PROC_EAPOL,     /* trigger EAPOL target selection (initiates authentication, node) */
    PROC_RPL,       /* trigger RPL parent selection (node) */

    PROC_AUTO_ON,   /* trigger bootstrap test procedures automatically */
    PROC_AUTO_OFF,  /* disable automatic bootstrap test procedure triggering */

    MSG_NONE
} ws_test_proc_t;

/**
 * Trigger a test procedure
 *
 * Can be used to trigger a test procedure, e.g. to send a message (DIS,
 * DIO, DAO, PAS, PS, PCS and PC) or to trigger bootstrap state change
 * on node e.g. EAPOL target selection.
 *
 * \param interface_id Network Interface ID >= 0 or -1 for Wi-SUN mesh interface
 *                     Default value is -1
 * \param procedure Triggered procedure
 * \param parameters Parameters for future extensions, shall be set to NULL
 *
 * \return 0                        Success
 * \return <0                       Failure
 */
int ws_test_procedure_trigger(int8_t interface_id, ws_test_proc_t procedure, void *parameters);

#ifdef __cplusplus
}
#endif

#endif /* NET_WS_TEST_EXT_H_ */
