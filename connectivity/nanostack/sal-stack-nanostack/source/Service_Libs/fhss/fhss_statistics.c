/*
 * Copyright (c) 2017-2019, Pelion and affiliates.
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
#include "ns_types.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "fhss.h"
#include "fhss_common.h"
#include "fhss_statistics.h"

void fhss_stats_update(fhss_structure_t *fhss_structure, fhss_stats_type_t type, uint32_t update_val)
{
    if (fhss_structure->fhss_stats_ptr) {
        switch (type) {
            case STATS_FHSS_DRIFT_COMP:
                fhss_structure->fhss_stats_ptr->fhss_drift_compensation = update_val;
                break;
            case STATS_FHSS_HOP_COUNT:
                fhss_structure->fhss_stats_ptr->fhss_hop_count = update_val;
                break;
            case STATS_FHSS_SYNCH_INTERVAL:
                fhss_structure->fhss_stats_ptr->fhss_synch_interval = update_val;
                break;
            case STATS_FHSS_AVG_SYNCH_FIX:
                fhss_structure->fhss_stats_ptr->fhss_prev_avg_synch_fix = update_val;
                break;
            case STATS_FHSS_SYNCH_LOST:
                fhss_structure->fhss_stats_ptr->fhss_synch_lost += update_val;
                break;
            case STATS_FHSS_UNKNOWN_NEIGHBOR:
                fhss_structure->fhss_stats_ptr->fhss_unknown_neighbor += update_val;
                break;
            case STATS_FHSS_CHANNEL_RETRY:
                fhss_structure->fhss_stats_ptr->fhss_channel_retry += update_val;
                break;
        }
    }
}

int fhss_statistics_start(fhss_structure_t *fhss_structure, fhss_statistics_t *fhss_statistics)
{
    fhss_structure->fhss_stats_ptr = fhss_statistics;
    return 0;
}
