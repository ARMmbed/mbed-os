/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef EMAC_TESTS_H
#define EMAC_TESTS_H

void test_emac_initialize();
void test_emac_broadcast();
void test_emac_unicast();
void test_emac_unicast_frame_len();
void test_emac_unicast_burst();
void test_emac_unicast_long();
void test_emac_multicast_filter();
void test_emac_memory();

#endif /* EMAC_TESTS_H */
