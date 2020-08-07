/*
 * Copyright (c) 2015-2018, Arm Limited and affiliates.
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
/*  This is for linux router which use RF tunnel interface */
#include "base/cfg_lowpan_border_router.h"
#include "base/cfg_ethernet.h"
#include "base/cfg_local_socket.h"
#include "base/cfg_rf_tunnel.h"

#define HAVE_AES
#define FEA_TRACE_SUPPORT
#define EXTRA_CONSISTENCY_CHECKS
#define HAVE_DHCPV6
#define HAVE_RF_AP
#define PANA
#define PANA_SERVER_API
#define ECC
#define TCP_TEST
