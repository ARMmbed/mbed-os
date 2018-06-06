/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
#ifndef TOPO_TRACE_H
#define TOPO_TRACE_H

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file topo_trace.h
 * \brief test API to trace the network topology
 */

#ifndef HAVE_TOPO_TRACE
/* Use DUMMY functions if not required. */
/** \cond DOXYGEN_IGNORE */
#define topo_trace(type, address, update_type) ((void) 0)
/** \endcond */

#else /* HAVE_TOPO_TRACE */

enum topo_type {
    TOPOLOGY_RPL,
    TOPOLOGY_MLE,
    TOPOLOGY_THREAD,
};

enum topo_update {
    TOPO_CLEAR = 2, // Clear all links of specified type to any address.
    TOPO_ADD = 1,   // Add a link of specified type to specified address.
    TOPO_REMOVE = 0 // Remove a link of specified type to specified address.
};

/** Modify topology tables.
 *
 * Add or remove entries to external topology tracer.
 *
 * \param topo_type Topology link type.
 * \param address EUI-64 address.
 * \param update_type Add or remove address.
 */
void topo_trace(enum topo_type type, const uint8_t address[static 8], enum topo_update update_type);

#endif

#ifdef __cplusplus
}
#endif

#endif /* TOPO_TRACE_H */
