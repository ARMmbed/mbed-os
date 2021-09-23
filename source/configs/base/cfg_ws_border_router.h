/*
 * Copyright (c) 2019, Pelion and affiliates.
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

/* Config Flags for WS Border router */
#include "cfg_ws_router.h"

#define HAVE_RPL_ROOT
#define HAVE_RPL_DAO_HANDLING
#define HAVE_6LOWPAN_BORDER_ROUTER
#define HAVE_WS_BORDER_ROUTER
#define HAVE_ND_PROXY
#define MULTICAST_FORWARDING
#define HAVE_PAE_AUTH
#define HAVE_EAPOL_RELAY
