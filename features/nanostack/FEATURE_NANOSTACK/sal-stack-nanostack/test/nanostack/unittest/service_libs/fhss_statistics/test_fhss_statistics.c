/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "nsconfig.h"
#include "mac_api.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "net_fhss.h"
#include "Service_Libs/fhss/fhss.h"
#include "Service_Libs/fhss/fhss_statistics.h"


bool test_fhss_stats_update()
{
    fhss_structure_t fhss_struct;
    fhss_statistics_t fhss_stats_ptr;
    memset(&fhss_stats_ptr, 0, sizeof(fhss_statistics_t));
    fhss_struct.fhss_stats_ptr = &fhss_stats_ptr;
    // Test drift compensation
    fhss_stats_update(&fhss_struct, STATS_FHSS_DRIFT_COMP, 10);
    if (fhss_struct.fhss_stats_ptr->fhss_drift_compensation != 10) {
        return false;
    }
    // Test hop count
    fhss_stats_update(&fhss_struct, STATS_FHSS_HOP_COUNT, 1);
    if (fhss_struct.fhss_stats_ptr->fhss_hop_count != 1) {
        return false;
    }
    // Test synch interval
    fhss_stats_update(&fhss_struct, STATS_FHSS_SYNCH_INTERVAL, 960);
    if (fhss_struct.fhss_stats_ptr->fhss_synch_interval != 960) {
        return false;
    }
    // Test average synch fix
    fhss_stats_update(&fhss_struct, STATS_FHSS_AVG_SYNCH_FIX, 1000);
    if (fhss_struct.fhss_stats_ptr->fhss_prev_avg_synch_fix != 1000) {
        return false;
    }
    // Test synch lost count
    fhss_struct.fhss_stats_ptr->fhss_synch_lost = 1;
    fhss_stats_update(&fhss_struct, STATS_FHSS_SYNCH_LOST, 1);
    if (fhss_struct.fhss_stats_ptr->fhss_synch_lost != 2) {
        return false;
    }
    return true;
}
