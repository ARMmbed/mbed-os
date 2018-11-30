/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
#ifndef MAC_COMMANDS_H_
#define MAC_COMMANDS_H_

#include <cstring>
#include <cstdlib>

#include "ns_cmdline.h"
#include "nsdynmemLIB.h"
#include "mbed_trace.h"
#include "mac_api.h"
#include "mlme.h"
#include "mac_mcps.h"
#include "mac_common_defines.h"
#include "mac_filter_api.h"
#include "mac_tester_util.h"

#define LOOKUP_DESCRIPTOR_TABLE_SIZE 2
#define DEVICE_DESCRIPTOR_TABLE_SIZE 2
#define USAGE_DESCRIPTOR_TABLE_SIZE 2
#define KEY_DESCRIPTOR_TABLE_SIZE 2

void mac_commands_init(void);

void mac_data_confirm_handler(const mac_api_t *api, const mcps_data_conf_t *data);
void mac_data_indication_handler(const mac_api_t *api, const mcps_data_ind_t *data);
void mac_purge_confirm_handler(const mac_api_t *api, mcps_purge_conf_t *data);
void mac_mlme_confirm_handler(const mac_api_t *api, mlme_primitive id, const void *data);
void mac_mlme_indication_handler(const mac_api_t *api, mlme_primitive id, const void *data);

int mac_start_command(int argc, char *argv[]);
int mac_scan_command(int argc, char *argv[]);
int mac_data_command(int argc, char *argv[]);
int mac_poll_command(int argc, char *argv[]);
int mac_purge_command(int argc, char *argv[]);
int mac_set_command(int argc, char *argv[]);
int mac_get_command(int argc, char *argv[]);
int mac_reset_command(int argc, char *argv[]);
int mac_address_command(int argc, char *argv[]);
int mac_key_command(int argc, char *argv[]);
int mac_add_neighbour_command(int argc, char *argv[]);
int mac_filter_command(int argc, char *argv[]);
int mac_config_status_command(int argc, char *argv[]);
int mac_find_beacon_command(int argc, char *argv[]);
int mac_wait_command(int argc, char *argv[]);
int mac_analyze_ed_command(int argc, char *argv[]);
int reset_command(int argc, char *argv[]);
int silent_mode_command(int argc, char *argv[]);

#endif
