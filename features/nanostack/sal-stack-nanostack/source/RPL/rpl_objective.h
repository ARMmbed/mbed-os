/*
 * Copyright (c) 2015, 2017-2018, Arm Limited and affiliates.
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

#ifndef RPL_OBJECTIVE_H_
#define RPL_OBJECTIVE_H_

#include "ns_list.h"

struct rpl_instance;
struct rpl_neighbour;

typedef struct rpl_objective {
    /* The Objective Code Point in DODAG Configuration Option */
    uint16_t ocp;
    /* Run the parent selection algorithm - see rpl_of0.c for detailed info */
    void (*parent_selection)(struct rpl_instance *);
    /* Return the path cost of a neighbour */
    uint16_t (*path_cost)(const struct rpl_neighbour *);
    bool (*neighbour_acceptable)(const struct rpl_instance *, const struct rpl_neighbour *);
    /* Could someone with specified rank be a significantly better candidate than the existing one? */
    /* (In future, this API could be extended to pass a metric pointer as well as rank) */
    bool (*possible_better_candidate)(const struct rpl_instance *, const struct rpl_neighbour *existing, uint16_t rank, uint16_t etx);
    ns_list_link_t link;
} rpl_objective_t;

/* Register a new objective function */
void rpl_objective_register(rpl_objective_t *objective);

/* Look up an objective function by OCP */
rpl_objective_t *rpl_objective_lookup(uint16_t ocp);

#endif /* RPL_OBJECTIVE_H_ */
