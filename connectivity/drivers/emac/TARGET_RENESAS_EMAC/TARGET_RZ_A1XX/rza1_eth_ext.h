/* Copyright (c) 2020 Renesas Electronics Corporation.
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
#ifndef ETHERNETEXT_H
#define ETHERNETEXT_H

#ifdef __cplusplus
extern "C" {
#endif

/* PHY link mode */
#define NEGO_FAIL               (0)
#define HALF_10M                (1)
#define FULL_10M                (2)
#define HALF_TX                 (3)
#define FULL_TX                 (4)

typedef void (ethernetext_cb_fnc)(void);

typedef struct tag_rza1_ethernet_cfg {
    int                  int_priority;
    ethernetext_cb_fnc   *recv_cb;
    char                 *ether_mac;
} rza1_ethernet_cfg_t;

extern int ethernetext_init(rza1_ethernet_cfg_t *p_ethcfg);
extern void ethernetext_start_stop(int32_t mode);
extern int ethernetext_chk_link_mode(void);
extern void ethernetext_set_link_mode(int32_t link);
extern void ethernetext_add_multicast_group(const uint8_t *addr);
extern void ethernetext_remove_multicast_group(const uint8_t *addr);
extern void ethernetext_set_all_multicast(int all);
#ifdef __cplusplus
}
#endif

#endif
