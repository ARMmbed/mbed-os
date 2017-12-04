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

/**
 * \file net_polling_api.h
 * \brief ZigBeeIP Sleepy Host Data Poll API.
 *
 *  - arm_nwk_host_mode_set(), Set sleepy host new state.
 *  - arm_nwk_host_mode_get(), Read current host state.
 *  - net_host_enter_sleep_state_set(), Enable deep sleep state.
 *
 * The sleepy host default state is NET_HOST_FAST_POLL_MODE after bootstrap.
 * The stack always disables radio automatically between data polls.
 * The stack can also enable deep sleep when application net_host_enter_sleep_state_set().
 *
 * A sleepy host can change the host state to normal, which saves time and resources when the client is waiting for a large amount of data.
 *  - nwk_host_mode_set(NET_HOST_RX_ON_IDLE,0) A function call to trigger MLE handshake update automatically.
 * From NET_HOST_RX_ON_IDLE state back to polling state causes an MLE handshake with parent:
 *  - nwk_host_mode_set (NET_HOST_FAST_POLL_MODE,0), Enter fast mode.
 *  - nwk_host_mode_set (NET_HOST_SLOW_POLL_MODE,10), Enter slow poll mode by 10 seconds max data poll period.
 *
 * The stack will tell max sleepy time to the application by selecting min values from the following cases:
 *  - Next active system timer trigger
 *  - ND protocol next state trigger
 *  - MLE handshake trigger
 *  - PANA key pull trigger
 *  - Next data poll period
 *
 */

#ifndef NET_POLLING_H_
#define NET_POLLING_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \enum net_host_mode_t
 * \brief Sleepy host states.
 */
typedef enum net_host_mode {
    NET_HOST_FAST_POLL_MODE,    /**< Sleepy host fast poll state, max poll period 400ms. */
    NET_HOST_SLOW_POLL_MODE,    /**< Sleepy host slow poll state, user-defined max period in seconds. */
    NET_HOST_RX_ON_IDLE,        /**< Disable polling and enable direct data flow from parent to host. */
    NET_HOST_MODE_NOT_ACTIVE,   /**< Host mode is not active. */
} net_host_mode_t;

/**
  * \brief Set new host state.
  *
  * \param interface_id Network interface ID.
  * \param mode New host state.
  * \param poll_time Poll time in seconds, only handled when NET_HOST_SLOW_POLL_MODE is enabled.
  *
  * Valid poll time for NET_HOST_SLOW_POLL_MODE is 0 < poll_time poll_time < 864001 (1 Day)
  *
  * \return 0, State update OK.
  * \return -1, Unknown state.
  * \return -2, Invalid time.
  * \return -3 MLE handshake trigger failure.
  *
  */

extern int8_t arm_nwk_host_mode_set(int8_t interface_id, net_host_mode_t mode, uint32_t poll_time);
/**
  * \brief Read current host state.
  *
  * \param interface_id Network interface ID.
  * \param mode A pointer to the location for saving the host state.

  * \return 0, State read update OK.
  * \return -1, The host role is router or the stack is idle.
  *
  */
extern int8_t arm_nwk_host_mode_get(int8_t interface_id, net_host_mode_t *mode);

/**
  * \brief Host sleep state control.
  *
  * \param interface_id Network interface ID.
  * \param state >0 enables power saving between data polling. Otherwise, only radio is disabled.
  *
  * When the application wants to save more power it can call net_host_enter_sleep_state_set(nwk_interface_id, 1).
  * When the application wants to disable sleep it just calls net_host_enter_sleep_state_set(nwk_interface_id, 0).
  */
extern void arm_net_host_enter_sleep_state_set(int8_t interface_id, uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* NET_POLLING_H_ */
