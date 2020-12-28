/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __S1SBP6A_WATCHDOG_H
#define __S1SBP6A_WATCHDOG_H


/* Control Register 0 */
#define WDOGCTRL_RESET_ENABLE_SHIFT            (0)
#define WDOGCTRL_INT_ENABLE_SHIFT              (1)

#define WDOGCTRL_RESET_ENABLE_MASK            (0x01 << WDOGCTRL_RESET_ENABLE_SHIFT)
#define WDOGCTRL_INT_ENABLE_MASK              (0x01 << WDOGCTRL_INT_ENABLE_SHIFT)

#define WDOGCTRL_RESET_ENABLE(c)              ((c) << WDOGCTRL_RESET_ENABLE_SHIFT)
#define WDOGCTRL_INT_ENABLE(c)                ((c) << WDOGCTRL_INT_ENABLE_SHIFT)

void bp6a_wdog_init(uint32_t timeout);
void bp6a_watchdog_enable(bool enable);
void bp6a_wdog_reset_enable(bool enable);
void bp6a_wdog_set_reload_time(uint32_t ms);


#endif /* __S1SBP6A_WATCHDOG_H */
