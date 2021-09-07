// ----------------------------------------------------------------------------
// Copyright 2015-2021 Pelion.
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------------------------------------------------------

/**
 * \file ns_trace.h
 * Trace interface abstraction for NanoStack library as well as application.
 *
 * This file was moved to this library for legacy compatibility
 * Actual used trace library is mbed-trace. For usage details check mbed_trace.h.
 *
 */
#ifndef NS_TRACE_H_
#define NS_TRACE_H_

#if defined(HAVE_DEBUG) && !defined(FEA_TRACE_SUPPORT)
#define FEA_TRACE_SUPPORT
#endif

#include "ns_types.h"
#include "mbed-trace/mbed_trace.h"

#endif /* NS_TRACE_H_ */
