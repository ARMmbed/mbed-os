/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

#ifdef HAVE_RPL

#include "ns_list.h"
#include "RPL/rpl_objective.h"

static NS_LIST_DEFINE(rpl_objectives, rpl_objective_t, link);

void rpl_objective_register(rpl_objective_t *objective)
{
    ns_list_add_to_end(&rpl_objectives, objective);
}

rpl_objective_t *rpl_objective_lookup(uint16_t ocp)
{
    ns_list_foreach(rpl_objective_t, of, &rpl_objectives) {
        if (of->ocp == ocp) {
            return of;
        }
    }
    return NULL;
}

#endif /* HAVE_RPL */
