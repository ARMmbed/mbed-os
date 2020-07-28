/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef EVENTLOOP_CONFIG_H_
#define EVENTLOOP_CONFIG_H_

/*
 * Options can be picked up from mbed-cli JSON configuration or from
 * a user configuration file - see below.
 *
 * Undefine all internal flags before evaluating the configuration.
 */

/* Use platform-provided low-resolution tick timer for eventloop (requires "platform_tick_timer" API) */
#undef NS_EVENTLOOP_USE_TICK_TIMER
/* Exclude high resolution timer from build (removes need for "platform_timer" API) */
#undef NS_EXCLUDE_HIGHRES_TIMER

/*
 * mbedOS 5 specific configuration flag mapping to internal flags
 */
#ifdef MBED_CONF_NANOSTACK_EVENTLOOP_USE_PLATFORM_TICK_TIMER
#define NS_EVENTLOOP_USE_TICK_TIMER     1
#endif

#ifdef MBED_CONF_NANOSTACK_EVENTLOOP_EXCLUDE_HIGHRES_TIMER
#define NS_EXCLUDE_HIGHRES_TIMER        1
#endif

/*
 * Include the user config file if defined
 */
#ifdef NS_EVENTLOOP_USER_CONFIG_FILE
#include NS_EVENTLOOP_USER_CONFIG_FILE
#endif

#endif /* EVENTLOOP_CONFIG_H_ */
