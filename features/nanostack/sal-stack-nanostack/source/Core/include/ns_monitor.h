/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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
 * \file ns_monitor.h
 * \brief Utility functions concerning IPv6 stack monitoring.
 *
 * Module can monitor nanostack heap usage and release memory if heap usage is too high.
 * Mmeory monitoring can work if memory statistics are enabled in nsdynmemLIB.
 *
 */

#ifndef _NS_MONITOR_H
#define _NS_MONITOR_H

int ns_monitor_init(void);

int ns_monitor_clear(void);

void ns_monitor_timer(uint16_t seconds);

int ns_monitor_heap_gc_threshold_set(uint8_t percentage_high, uint8_t percentage_critical);

#endif // _NS_MONITOR_H

