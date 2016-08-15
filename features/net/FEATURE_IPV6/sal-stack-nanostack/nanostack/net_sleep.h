/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
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

#ifndef NET_SLEEP_H_
#define NET_SLEEP_H_

#include "ns_types.h"

/**
 * \file net_sleep.h
 * \brief Checks if there is a sleep possibility for the stack and the max sleep time.
 *
 * \section check-slp-poss Checking sleep possibility.
 * - arm_net_check_enter_deep_sleep_possibility(), Checks whether the stack can enter a sleep cycle.
 * - arm_net_enter_sleep(), A function to enter sleep cycle.
 * - arm_net_wakeup_and_timer_synch(), Restarts the stack and synchronizes the timer.
 */

/**
 *\brief Check sleep possibility.
 * \return Time in milliseconds for sleep.
 * \return 0,  No possibility to enter deep sleep.
 */
uint32_t arm_net_check_enter_deep_sleep_possibility(void);

/**
 * \brief Set stack to sleep.
 *
 * \return 0 Stack stopped.
 * \return -1 Not supported action at the moment.
 *
 */

int arm_net_enter_sleep(void);
/**
 * \brief Restart stack after sleep.
 *
 *  Stack enable and synch timers after sleep.
 *
 *  \param sleeped_time_in_ms The total sleep time in milliseconds. The stack needs to synch with this.
 *
 * \return 0 Stack restarted.
 * \return 1 Stack can continue sleep for the time defined in sleeped_time_in_ms.
 * \return -1 Stack already active.
 *
 */
int arm_net_wakeup_and_timer_synch(uint32_t sleeped_time_in_ms);


#endif /* NET_SLEEP_H_ */
