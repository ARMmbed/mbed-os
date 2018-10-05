/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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
#include "string.h"
#include "Security/PANA/pana_relay_table.h"
#include "nsdynmemLIB.h"

#ifdef PANA

static NS_LARGE relay_table_entry_t *relay_first_info = 0;


void pana_relay_table_init(void)
{
    /* Init Double linked Routing Table */

    if (relay_first_info) {
        relay_table_entry_t *cur = 0;
        while (relay_first_info) {
            cur = relay_first_info;
            relay_first_info = cur->next_info;
            ns_dyn_mem_free(cur);
        }
    }
}


relay_table_entry_t *pana_relay_table_update(uint8_t *address)
{
    relay_table_entry_t *ret_val = 0;

    relay_table_entry_t *cur = 0;
    if (relay_first_info) {
        cur = relay_first_info;
        while (cur) {

            if (memcmp((uint8_t *)cur->relay_address, (uint8_t *) address, 16) == 0) {
                /* General Update part */
                cur->ttl = 20;
                ret_val = cur;
                break;
            }
            if (cur->next_info) {
                cur = cur->next_info;
            } else {
                break;
            }
        }
    }

    //allocate new
    if (ret_val == 0) {
        ret_val = (relay_table_entry_t *) ns_dyn_mem_alloc(sizeof(relay_table_entry_t));
        if (ret_val) {
            if (cur != 0) {
                cur->next_info = ret_val;
                ret_val->prev_info = cur;
            } else {
                ret_val->prev_info = 0;
            }
            ret_val->next_info = 0;
            /* Copy Adrress IPv6 Address */
            memcpy((uint8_t *)ret_val->relay_address, address, 16);
            ret_val->ttl = 20;
            if (relay_first_info == 0) {
                relay_first_info = ret_val;
            }
        }
    }
    return ret_val;
}

relay_table_entry_t *pana_relay_detect(uint8_t *address)
{
    relay_table_entry_t *cur = 0;
    if (relay_first_info) {
        cur = relay_first_info;
        while (cur) {
            if (memcmp((uint8_t *)cur->relay_address, (uint8_t *) address, 16) == 0) {
                /* General Update part */
                return cur;
            }
            cur = cur->next_info;
        }
    }
    return 0;
}


void pana_relay_ttl_update(uint16_t ttl_time)
{
    relay_table_entry_t *cur = 0;
    if (relay_first_info) {
        relay_table_entry_t *prev = 0;
        relay_table_entry_t *next = 0;
        cur = relay_first_info;
        while (cur) {
            if (cur->ttl < ttl_time) {
                cur->ttl -= ttl_time;
                cur = cur->next_info;
            } else {
                prev = cur->prev_info;
                next = cur->next_info;
                if (prev) { //Not First

                    if (next) { //Not Last
                        prev->next_info = next;
                        next->prev_info = prev; //Link next to new prev
                    } else { //Last One
                        prev->next_info = 0;/* New last entry */
                    }

                } else { //FIRST
                    if (next == 0) { /* Last entry */
                        relay_first_info = 0; //Reset Route table
                    } else { //New First
                        relay_first_info = next; // Setup new First
                        next->prev_info = 0;
                    }
                }
                ns_dyn_mem_free(cur);
                cur = next;
            }

        }
    }
}
#endif
