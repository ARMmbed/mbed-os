/*
 * Copyright (c) 2014-2019, Arm Limited and affiliates.
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
 * Build definitions, for now we define the default configuration here.
 */

#ifndef _NANOSTACK_SOURCE_CONFIG_H
#define _NANOSTACK_SOURCE_CONFIG_H

#include "ns_types.h"

#define __ns_cfg_header(x) #x
#define _ns_cfg_header(x) __ns_cfg_header(configs/cfg_##x.h)
#define ns_cfg_header(x) _ns_cfg_header(x)

#ifdef MBED_CONF_NANOSTACK_CONFIGURATION
#define NSCONFIG MBED_CONF_NANOSTACK_CONFIGURATION
#endif

#ifndef NSCONFIG
#define NSCONFIG nanostack_full
#endif

#include ns_cfg_header(NSCONFIG)

#if defined(HAVE_THREAD)
/** For thread HAVE_MESH and HAVE_DHCPV6 must be defined minimal */
#ifndef HAVE_MESH
#define HAVE_MESH
#endif
#ifndef HAVE_DHCPV6
#define HAVE_DHCPV6
#endif
#endif /* HAVE_THREAD */

#if defined(HAVE_WS)
#ifndef HAVE_DHCPV6
#define HAVE_DHCPV6
#endif
#endif /* HAVE_WS */

/* Configure trace level for Nanostack */
#ifdef MBED_CONF_NANOSTACK_TRACE_MAX_LEVEL
#ifdef MBED_TRACE_MAX_LEVEL
#undef MBED_TRACE_MAX_LEVEL
#endif /* MBED_TRACE_MAX_LEVEL */
#define MBED_TRACE_MAX_LEVEL MBED_CONF_NANOSTACK_TRACE_MAX_LEVEL
#endif /* MBED_CONF_NANOSTACK_TRACE_MAX_LEVEL */

#endif // ifndef _NANOSTACK_SOURCE_CONFIG_H

