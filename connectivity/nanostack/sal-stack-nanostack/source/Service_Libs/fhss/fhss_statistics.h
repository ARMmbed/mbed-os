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
#ifndef FHSS_STATISTICS_H_
#define FHSS_STATISTICS_H_

typedef enum {
    STATS_FHSS_DRIFT_COMP,
    STATS_FHSS_HOP_COUNT,
    STATS_FHSS_SYNCH_INTERVAL,
    STATS_FHSS_AVG_SYNCH_FIX,
    STATS_FHSS_SYNCH_LOST,
    STATS_FHSS_UNKNOWN_NEIGHBOR,
    STATS_FHSS_CHANNEL_RETRY
} fhss_stats_type_t;

void fhss_stats_update(fhss_structure_t *fhss_structure, fhss_stats_type_t type, uint32_t update_val);
int fhss_statistics_start(fhss_structure_t *fhss_structure, fhss_statistics_t *fhss_statistics);

#endif /* FHSS_STATISTICS_H_ */
