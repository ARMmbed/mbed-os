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
#include <string.h>
#include "nsdynmemLIB.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "fhss_channel_stub.h"
#include "fhss_beacon_tasklet_stub.h"
#include "Service_Libs/fhss/fhss.h"

fhss_beacon_tasklet_stub_def fhss_beacon_tasklet_stub;

int8_t fhss_beacon_create_tasklet(fhss_structure_t *fhss_structure)
{
    return fhss_beacon_tasklet_stub.int8_value;
}

void fhss_beacon_periodic_stop(fhss_structure_t *fhss_structure)
{

}

int fhss_beacon_periodic_start(fhss_structure_t *fhss_structure, uint32_t time_to_first_beacon)
{
    return 0;
}

void fhss_beacon_build(fhss_structure_t *fhss_structure, uint8_t* dest)
{

}

void fhss_beacon_received(fhss_structure_t *fhss_structure, const uint8_t *synch_info, const uint32_t elapsed_time)
{

}
