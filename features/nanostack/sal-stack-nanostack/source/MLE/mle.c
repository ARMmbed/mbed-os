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
#include "MLE/mle.h"

#ifndef NO_MLE
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "socket_api.h"
#include "Core/include/socket.h"
#include "nsdynmemLIB.h"
#include "ns_trace.h"
#include "string.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/udp.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "platform/arm_hal_interrupt.h"
#include "platform/topo_trace.h"
#include "common_functions.h"
#include "MLE/mle_tlv.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/etx/etx.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "mac_api.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"

#define MLE_UNICAST_CHALLENGE_TIMEOUT 20

#define TRACE_GROUP "mle"

typedef enum {
    ARM_MLE_INIT = 0,
    ARM_MLE_TTL_TIMER
} arm_mle_event_id_e;

//MLE class structure
typedef struct mle_table_class {
    int8_t interfaceId;
    mle_class_user_mode mode;
    mle_neigh_table_list_t mle_table;       //Active Entry
    mle_neigh_table_list_t free_enty_list;
    mle_neigh_table_entry_t *allocated_buffer;
    uint8_t buffer_size;
    mle_entry_user_entry_remove_notify *remove_cb;
    mle_entry_link_keep_alive *keep_alive_cb;
    mle_entry_link_keep_alive *challenge_cb;
    mle_entry_interface_activate *interface_is_active;
    ns_list_link_t link; /*!< List link entry */
} mle_table_class_t;

static NS_LIST_DEFINE(mle_table_calss_list, mle_table_class_t, link);

static int8_t mle_tasklet_id = -1;

static arm_event_storage_t *mle_class_timer_storage = NULL;

static mle_neigh_table_entry_t *mle_class_neighbor_get(mle_neigh_table_list_t *mle_table, const uint8_t *address, addrtype_t type);
static mle_neigh_table_entry_t *mle_class_get_free_entry(mle_neigh_table_list_t *mle_table);
static bool mle_class_neighbor_validate(mle_neigh_table_list_t *mle_table, const mle_neigh_table_entry_t *entry);
static void mle_event_handler(arm_event_s *event);


static bool mle_table_timer_start(void)
{
    if (!mle_class_timer_storage) {

        arm_event_s event = {
                .receiver = mle_tasklet_id,
                .sender = 0,
                .event_id = 0,
                .data_ptr = NULL,
                .event_type = ARM_MLE_TTL_TIMER,
                .priority = ARM_LIB_LOW_PRIORITY_EVENT,
        };

        mle_class_timer_storage  = eventOS_event_timer_request_every(&event, eventOS_event_timer_ms_to_ticks(MLE_TIMER_TICKS_MS));
        if (!mle_class_timer_storage) {
            tr_error("Mle timer start fail");
            return false;
        }
    }

    return true;
}

static void mle_table_timer_stop(void)
{
    if (mle_class_timer_storage && ns_list_is_empty(&mle_table_calss_list)) {
        eventOS_cancel(mle_class_timer_storage);
        mle_class_timer_storage = NULL;
    }
}


static void mle_table_remove_free_indirect_table(int8_t interface_id, mle_neigh_table_entry_t *entry_ptr)
{
    protocol_interface_info_entry_t *cur_interface = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur_interface) {
        return;
    }
    //Free firts by defined short address
    if (entry_ptr->short_adr < 0xfffe) {
        uint8_t temp_address[2];
        common_write_16_bit(entry_ptr->short_adr, temp_address);
        lowpan_adaptation_indirect_free_messages_from_queues_by_address(cur_interface, temp_address, ADDR_802_15_4_SHORT);
    }
    lowpan_adaptation_indirect_free_messages_from_queues_by_address(cur_interface, entry_ptr->mac64, ADDR_802_15_4_LONG);
}

static void mle_table_class_list_free(mle_table_class_t *mle_table_class) {

    ns_list_foreach_safe(mle_neigh_table_entry_t, cur, &mle_table_class->mle_table) {
        ns_list_remove(&mle_table_class->mle_table, cur);
        //Clean Indirect queue
        mle_table_remove_free_indirect_table(mle_table_class->interfaceId, cur);
        //Call Remove callback
        mle_table_class->remove_cb(mle_table_class->interfaceId, cur);
        //Removes ETX neighbor
        etx_neighbor_remove(mle_table_class->interfaceId, cur);
        ns_list_add_to_start(&mle_table_class->free_enty_list, cur);
    }
    topo_trace(TOPOLOGY_MLE, NULL, TOPO_CLEAR);
}

static void mle_table_class_free(mle_table_class_t *main_list)
{
    ns_list_remove(&mle_table_calss_list, main_list);
    mle_table_class_list_free(main_list);
    //Free list buffer
    ns_dyn_mem_free(main_list->allocated_buffer);
    ns_dyn_mem_free(main_list);
    mle_table_timer_stop();
}



static int8_t mle_table_class_table_buffer_allocate(mle_table_class_t *mle_class, uint8_t list_size)
{
    mle_neigh_table_entry_t *list_buffer = ns_dyn_mem_alloc(sizeof(mle_neigh_table_entry_t) * list_size);
    if (!list_buffer) {
        return -1;
    }

    mle_class->allocated_buffer = list_buffer; //Save storaged
    ns_list_init(&mle_class->free_enty_list);
    ns_list_init(&mle_class->mle_table);
    for (uint8_t i = 0; i< list_size; i++) {
        memset(list_buffer, 0, sizeof(mle_neigh_table_entry_t));

        list_buffer->attribute_index = i;
        //Add to list
        ns_list_add_to_end(&mle_class->free_enty_list,list_buffer);
        list_buffer++;
    }
    return 0;
}


static mle_table_class_t * mle_table_class_allocate(int8_t interfaceId, uint8_t list_size)
{
    mle_table_class_t *newClass = ns_dyn_mem_alloc(sizeof(mle_table_class_t));

    if (newClass) {
        memset(newClass, 0, sizeof(mle_table_class_t));
        if (mle_table_class_table_buffer_allocate(newClass,list_size) != 0) {
            ns_dyn_mem_free(newClass);
            return NULL;
        }
        newClass->interfaceId = interfaceId;
        newClass->buffer_size = list_size;
    }
    return newClass;
}

static mle_table_class_t * mle_table_class_discover(int8_t interface_id) {
    ns_list_foreach(mle_table_class_t, cur_mle_class, &mle_table_calss_list) {
        if (cur_mle_class->interfaceId == interface_id) {
            return cur_mle_class;
        }
    }
    return NULL;
}

static int8_t mle_class_event_handler_init(void) {
    if (mle_tasklet_id == -1) {
        //GENERATE TASKLET
        mle_tasklet_id = eventOS_event_handler_create(&mle_event_handler, ARM_MLE_INIT);

    }

    return mle_tasklet_id;
}

int8_t mle_class_init(int8_t interface_id, uint8_t table_size, mle_entry_user_entry_remove_notify *remove_cb, mle_entry_link_keep_alive *keep_alive_cb, mle_entry_interface_activate *interface_is_active)
{
    //Discover from the list
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);
    //Clean list and set function pointer call backs
    if (mle_class_ptr) {
        mle_class_ptr->remove_cb = remove_cb;
        mle_class_ptr->keep_alive_cb = keep_alive_cb;
        mle_class_ptr->challenge_cb = NULL;
        mle_class_ptr->interface_is_active = interface_is_active;
        if (mle_class_ptr->buffer_size != table_size) { //Clean tabs only when table size is different
            mle_table_class_list_free(mle_class_ptr);
            ns_dyn_mem_free(mle_class_ptr->allocated_buffer);
            mle_class_ptr->allocated_buffer = NULL;
            //Reallocate
            if (mle_table_class_table_buffer_allocate(mle_class_ptr,table_size) != 0) {
                ns_list_remove(&mle_table_calss_list, mle_class_ptr);
                ns_dyn_mem_free(mle_class_ptr);
                return -2;
            }
            mle_class_ptr->buffer_size = table_size;
        }
        return 0;
    }

    if (mle_class_event_handler_init() < 0) {
        return -2;
    }

    if (!mle_table_timer_start()) {
        return -2;
    }

    mle_class_ptr  = mle_table_class_allocate(interface_id, table_size);
    //Allocate new
    if (!mle_class_ptr) {
        return -2;
    }

    tr_debug("MLE service init size %d", table_size);
    mle_class_ptr->remove_cb = remove_cb;
    mle_class_ptr->keep_alive_cb = keep_alive_cb;
    mle_class_ptr->challenge_cb = NULL;
    mle_class_ptr->interface_is_active = interface_is_active;
    ns_list_add_to_end(&mle_table_calss_list, mle_class_ptr);
    return 0;

}

int8_t mle_class_router_challenge(int8_t interface_id,mle_entry_link_keep_alive *challenge_cb)
{
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);
    //Clean list and set function pointer call backs
    if (!mle_class_ptr) {
        return -1;
    }
    mle_class_ptr->challenge_cb = challenge_cb;
    return 0;
}

bool mle_class_exists_for_interface(int8_t interface_id) {

    if (mle_table_class_discover(interface_id)) {
        return true;
    }

    return false;
}

int8_t mle_class_deallocate(int8_t interface_id)
{
    //Discover from the list
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);
    //Clean list and set function pointer call backs
    if (!mle_class_ptr) {
        return -1;
    }

    mle_table_class_free(mle_class_ptr);
    return 0;

}

int8_t mle_class_list_clean(int8_t interface_id)
{
    //Discover from the list
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);
    //Clean list and set function pointer call backs
    if (!mle_class_ptr) {
        return -1;
    }

    mle_table_class_list_free(mle_class_ptr);
    return 0;

}

int8_t mle_class_mode_set(int8_t interface_id,mle_class_user_mode mode)
{
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);
    //Clean list and set function pointer call backs
    if (!mle_class_ptr) {
        return -1;
    }
    mle_class_ptr->mode = mode;

    return 0;
}

int8_t mle_class_set_new_key_pending(int8_t interface_id)
{
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);

    if (!mle_class_ptr) {
        return -1;
    }

    ns_list_foreach_safe(mle_neigh_table_entry_t, cur, &mle_class_ptr->mle_table) {
        cur->new_key_pending = true;
    }

    return 0;
}

int16_t mle_class_free_entry_count_get(int8_t interface_id)
{
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);
    //Clean list and set function pointer call backs
    if (!mle_class_ptr) {
        return 0;
    }
    return ns_list_count(&(mle_class_ptr->free_enty_list));

}

int16_t mle_class_sleepy_entry_count_get(int8_t interface_id)
{
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);

    if (!mle_class_ptr) {
        return 0;
    }

    uint16_t count = 0;

    ns_list_foreach(mle_neigh_table_entry_t, entry, &mle_class_ptr->mle_table) {
        if (!(entry->mode & MLE_RX_ON_IDLE)) {
            count++;
        }
    }

    return count;
}

int16_t mle_class_rfd_entry_count_get(int8_t interface_id)
{
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);

    if (!mle_class_ptr) {
        return 0;
    }

    uint16_t count = 0;

    ns_list_foreach(mle_neigh_table_entry_t, entry, &mle_class_ptr->mle_table) {
        if ((entry->mode & MLE_DEV_MASK) == MLE_RFD_DEV) {
            count++;
        }
    }

    return count;
}

mle_neigh_table_entry_t *mle_class_get_entry_by_ll64(int8_t interface_id, uint8_t linkMargin, const uint8_t *ipv6Address, bool allocateNew, bool *new_entry_allocated)
{
    // Check it really is LL64 (not LL16)

    if (memcmp(ipv6Address, ADDR_LINK_LOCAL_PREFIX , 8) != 0) {
        return NULL;    //Mot Link Local Address
    }

    if (memcmp((ipv6Address + 8), ADDR_SHORT_ADR_SUFFIC , 6) == 0) {
        return NULL;
    }
    // map
    uint8_t temporary_mac64[8];
    memcpy(temporary_mac64, (ipv6Address + 8), 8);
    temporary_mac64[0] ^= 2;

    return mle_class_get_entry_by_mac64(interface_id, linkMargin, temporary_mac64, allocateNew, new_entry_allocated);
}

mle_neigh_table_entry_t *mle_class_discover_entry_by_ll64(int8_t interface_id, const uint8_t *ipv6Address)
{

    // Check it really is LL64 (not LL16)

    if (memcmp(ipv6Address, ADDR_LINK_LOCAL_PREFIX , 8) != 0) {
        return NULL;    //Mot Link Local Address
    }

    if (memcmp((ipv6Address + 8), ADDR_SHORT_ADR_SUFFIC , 6) == 0) {
        return NULL;
    }
    // map
    uint8_t temporary_mac64[8];
    memcpy(temporary_mac64, (ipv6Address + 8), 8);
    temporary_mac64[0] ^= 2;

    return mle_class_get_by_link_address(interface_id, temporary_mac64, ADDR_802_15_4_LONG);
}


mle_neigh_table_entry_t *mle_class_get_entry_by_mac64(int8_t interface_id, uint8_t linkMargin, const uint8_t *mac64, bool allocateNew, bool *new_entry_allocated)
{
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);
    //Clean list and set function pointer call backs
    if (!mle_class_ptr) {
        return NULL;
    }

    if (new_entry_allocated) {
        *new_entry_allocated = false;
    }

    mle_neigh_table_entry_t *ret_val = mle_class_neighbor_get(&mle_class_ptr->mle_table, mac64, ADDR_802_15_4_LONG);


    /* Look for existing entry */
    if (ret_val) {
        ret_val->link_margin = ret_val->link_margin + linkMargin - (ret_val->link_margin >> THREAD_LINK_MARGIN_SCALING);
        return ret_val;
    }

    if (allocateNew) {
        ret_val = mle_class_get_free_entry(&mle_class_ptr->free_enty_list);

        if (ret_val) {
            //Add to active list
            ns_list_add_to_start(&mle_class_ptr->mle_table, ret_val);
            topo_trace(TOPOLOGY_MLE, mac64, TOPO_ADD);
            ret_val->link_margin = linkMargin << THREAD_LINK_MARGIN_SCALING;
            memcpy(ret_val->mac64, mac64, 8);
            if (new_entry_allocated) {
                *new_entry_allocated = true;
            }
        }
    }
    return ret_val;
}

static mle_neigh_table_entry_t *mle_class_neighbor_get_by_attribute_index(mle_neigh_table_list_t *mle_table, uint8_t attribute_index)
{

    ns_list_foreach(mle_neigh_table_entry_t, cur, mle_table) {
        if (cur->attribute_index == attribute_index) {
            return cur;
        }
    }
    return NULL;
}

static bool mle_class_neighbor_validate(mle_neigh_table_list_t *mle_table, const mle_neigh_table_entry_t *entry)
{

    ns_list_foreach(mle_neigh_table_entry_t, cur, mle_table) {
        if (cur == entry) {
            return true;
        }
    }
    return false;
}

static mle_neigh_table_entry_t *mle_class_neighbor_get(mle_neigh_table_list_t *mle_table, const uint8_t *address, addrtype_t type)
{
    uint16_t short_id;
    if (type == ADDR_802_15_4_SHORT) {
        short_id = common_read_16_bit(address);
    }
    ns_list_foreach(mle_neigh_table_entry_t, cur, mle_table) {

        if (type == ADDR_802_15_4_SHORT) {
            if (cur->short_adr == short_id) {
                return cur;
            }
        } else  {
            if (memcmp(cur->mac64, address, 8) == 0) {
                return cur;
            }
        }

    }
    return NULL;
}

static mle_neigh_table_entry_t *mle_class_get_free_entry(mle_neigh_table_list_t *mle_table)
{
    mle_neigh_table_entry_t *mle_entry = ns_list_get_first(mle_table);
    if (mle_entry) {
        //Remove from the list
        ns_list_remove(mle_table, mle_entry);
        uint8_t attribute_id = mle_entry->attribute_index;
        memset(mle_entry, 0, sizeof(mle_neigh_table_entry_t));
        mle_entry->attribute_index = attribute_id;
        mle_entry->short_adr = 0xffff;
        mle_entry->mode = MLE_FFD_DEV | MLE_RX_ON_IDLE;
        mle_entry->holdTime = 7;
        mle_entry->last_contact_time = protocol_core_monotonic_time;
        mle_entry->mle_frame_counter = 0;
        mle_entry->last_key_sequence = 0;
        mle_entry->new_key_pending = false;
        mle_entry->medium_ttl_challenge = false;
    }

    return mle_entry;
}



mle_neigh_table_entry_t *mle_class_get_by_link_address(int8_t interface_id, const uint8_t *address, addrtype_t type)
{
    switch (type) {
        case ADDR_802_15_4_SHORT:
        case ADDR_802_15_4_LONG:

            break;
        default:
            return NULL;
    }

    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);
    //Clean list and set function pointer call backs
    if (!mle_class_ptr) {
        return NULL;
    }

    return mle_class_neighbor_get(&mle_class_ptr->mle_table, address, type);

}

mle_neigh_table_entry_t *mle_class_get_by_device_attribute_id(int8_t interface_id, uint8_t attribute_index)
{
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);
    //Clean list and set function pointer call backs
    if (!mle_class_ptr) {
        return NULL;
    }

    return mle_class_neighbor_get_by_attribute_index(&mle_class_ptr->mle_table, attribute_index);

}

int8_t mle_class_remove_entry(int8_t interface_id, mle_neigh_table_entry_t *entry)
{
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);
    //Clean list and set function pointer call backs
    if (!mle_class_ptr) {
        return -1;
    }

    //Validate Pointer
    if (!mle_class_neighbor_validate(&mle_class_ptr->mle_table, entry)) {
        return -2;
    }
    //Remove from list
    ns_list_remove(&mle_class_ptr->mle_table, entry);
    //Free Indirect Queue
    mle_table_remove_free_indirect_table(mle_class_ptr->interfaceId, entry);
    //Call Remove callback
    mle_class_ptr->remove_cb(mle_class_ptr->interfaceId, entry);
    topo_trace(TOPOLOGY_MLE, entry->ext64, TOPO_REMOVE);

    //Removes ETX neighbor
    etx_neighbor_remove(interface_id, entry);
    //Add to free list
    ns_list_add_to_start(&mle_class_ptr->free_enty_list, entry);
    return 0;
}

int8_t mle_class_remove_neighbour(int8_t interface_id, const uint8_t *address, addrtype_t type)
{
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);
    //Clean list and set function pointer call backs
    if (!mle_class_ptr) {
        return -1;
    }

    mle_neigh_table_entry_t * entry = mle_class_get_by_link_address(interface_id, address, type);
    if (!entry) {
        return -2;
    }
    //Remove from list
    ns_list_remove(&mle_class_ptr->mle_table, entry);
    //Free Indirect Queue
    mle_table_remove_free_indirect_table(mle_class_ptr->interfaceId, entry);
    //Call Remove callback
    mle_class_ptr->remove_cb(mle_class_ptr->interfaceId, entry);

    topo_trace(TOPOLOGY_MLE, entry->ext64, TOPO_REMOVE);

     //Removes ETX neighbor
     etx_neighbor_remove(interface_id, entry);
    //Add to free list
    ns_list_add_to_start(&mle_class_ptr->free_enty_list, entry);

    return 0;

}



static void mle_class_table_ttl(uint16_t ticks, mle_table_class_t *mle_class_ptr)
{
    uint16_t new_ttl;
    bool challengeCheck;
    bool remove_entry;

    //validate that interface is still active
    if (!mle_class_ptr->interface_is_active(mle_class_ptr->interfaceId)) {
        return;
    }

    ns_list_foreach_safe(mle_neigh_table_entry_t, cur, &mle_class_ptr->mle_table) {
        new_ttl = 0;
        remove_entry = false;
        switch (mle_class_ptr->mode) {
            case MLE_CLASS_END_DEVICE:
                if (cur->priorityFlag) {
                    challengeCheck = true;
                } else {
                    challengeCheck = false;
                }
                break;
            case MLE_CLASS_ROUTER: //Router and sleepy end device never do challenge
            default:
                challengeCheck = false;
                break;
        }

        if (challengeCheck) {
            if (cur->ttl > MLE_TABLE_CHALLENGE_TIMER || cur->ttl < MLE_TABLE_CHALLENGE_TIMER) {
                new_ttl = ticks + MLE_TABLE_CHALLENGE_TIMER;
            }

            if (cur->ttl <= new_ttl) {
                if (cur->ttl != 1) {
                    if (mle_class_ptr->keep_alive_cb(mle_class_ptr->interfaceId, cur->mac64) != 0) {
                        cur->ttl--;
                        if (cur->ttl == 1) {
                            remove_entry = true;
                        }
                    } else {
                        cur->ttl = 1; //Lock retries here
                    }
                }
            } else {
                cur->ttl -= ticks;
            }
        } else {
            if (ticks >= cur->ttl) {

                remove_entry = true;
            } else {
                cur->ttl -= ticks;
            }
        }

        if (remove_entry) {
            //Silence delete
            //Remove from list
            ns_list_remove(&mle_class_ptr->mle_table, cur);
            //Free Indirect Queue
            mle_table_remove_free_indirect_table(mle_class_ptr->interfaceId, cur);
            mle_class_ptr->remove_cb(mle_class_ptr->interfaceId, cur);
            topo_trace(TOPOLOGY_MLE, cur->ext64, TOPO_REMOVE);
            //Removes ETX neighbor
            etx_neighbor_remove(mle_class_ptr->interfaceId, cur);
            //Add to free list
            ns_list_add_to_start(&mle_class_ptr->free_enty_list, cur);

        }
    } // for each entry

    //Router to Router Challenge timeout for big network and FHSS systems which could loose broadcast messages.
    if (mle_class_ptr->challenge_cb && mle_class_ptr->mode == MLE_CLASS_ROUTER) {
        //Calculate timeout trigger
        uint8_t challenge_count = 0;
        ns_list_foreach_safe(mle_neigh_table_entry_t, cur, &mle_class_ptr->mle_table) {
            if (((cur->mode & MLE_DEV_MASK) == MLE_FFD_DEV)  && !cur->medium_ttl_challenge) {
                //Challenge Neighbour
                if (cur->ttl < (cur->timeout_rx / 2)) {
                    if (mle_class_ptr->challenge_cb(mle_class_ptr->interfaceId, cur->mac64) != 0) {
                        tr_error("Router2Router challenge start fail");
                        return;
                    }
                    cur->medium_ttl_challenge = true;
                    if (++challenge_count == 2) {
                        //trig only 2 active / 4 second period.
                        return;
                    }
                }
            }
        }
    }
}


mle_neigh_table_list_t *mle_class_active_list_get(int8_t interface_id)
{
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);
    //Clean list and set function pointer call backs
    if (!mle_class_ptr) {
        return NULL;
    }
    return &mle_class_ptr->mle_table;
}

uint16_t mle_class_active_neigh_counter(int8_t interface_id)
{
    mle_table_class_t *mle_class_ptr = mle_table_class_discover(interface_id);
    //Clean list and set function pointer call backs
    if (!mle_class_ptr) {
        return 0xffff;
    }


    return ns_list_count(&mle_class_ptr->mle_table);
}

uint8_t *mle_general_write_source_address(uint8_t *ptr, protocol_interface_info_entry_t *cur)
{
    if (cur->global_address_available) {
        uint16_t mac16 = mac_helper_mac16_address_get(cur);
        if (mac16 < 0xfffe) {
            ptr = mle_tlv_write_source_address(ptr, mac16);
        }
    }
    return ptr;
}

uint8_t *mle_general_write_link_layer_framecounter(uint8_t *ptr, protocol_interface_info_entry_t *cur)
{
    uint32_t temp_counter;
    mac_helper_link_frame_counter_read(cur->id, &temp_counter);
    return mle_tlv_write_link_layer_framecount(ptr, temp_counter);
}

static void mle_event_handler(arm_event_s *event)
{
    switch (event->event_type) {
        case ARM_MLE_INIT:
            tr_debug("MLE Tasklet Generated");
            break;

        case ARM_MLE_TTL_TIMER:
            //Do list in future for each of mle user
            //Set here mle class ttl update
            ns_list_foreach_safe(mle_table_class_t, mle_clas_entry, &mle_table_calss_list) {
                mle_class_table_ttl(1, mle_clas_entry);
            }


            break;
    }
}

bool mle_neigh_entry_frame_counter_update(mle_neigh_table_entry_t *entry_temp, uint8_t *tlv_ptr, uint16_t tlv_length, protocol_interface_info_entry_t *cur, uint8_t key_id)
{
    mle_tlv_info_t mle_tlv_info;
    uint32_t frame_counter;
    if (mle_tlv_option_discover(tlv_ptr, tlv_length, MLE_TYPE_LL_FRAME_COUNTER, &mle_tlv_info)  != 4) {
        if (cur->mac_parameters->SecurityEnabled) {
            return false;
        } else {
            frame_counter = 0;
        }
    } else {
        frame_counter = common_read_32_bit(mle_tlv_info.dataPtr);
    }

    mac_helper_devicetable_set(entry_temp, cur, frame_counter, key_id, false);
    return true;
}

void mle_entry_timeout_update(mle_neigh_table_entry_t *entry_temp, uint32_t timeout_tlv)
{
    if (timeout_tlv > 86400) {
        timeout_tlv = 86400;
    } else if (timeout_tlv == 0) {
        timeout_tlv = 500;
    }
    timeout_tlv /= MLE_TIMER_TICKS_SECONDS;
    timeout_tlv++;
    entry_temp->timeout_rx = timeout_tlv;
    mle_entry_timeout_refresh(entry_temp);
}

void mle_entry_timeout_refresh(mle_neigh_table_entry_t *entry_temp)
{
    entry_temp->ttl = entry_temp->timeout_rx;
    entry_temp->last_contact_time = protocol_core_monotonic_time;
    entry_temp->medium_ttl_challenge = false;
}

static void mle_refresh_entry(mle_neigh_table_entry_t *neig_info, bool dataPollConfirmation)
{
    if (!neig_info) {
        return;
    }
    if (!neig_info->handshakeReady) {
        tr_debug("refresh:Link Handshake not ready yet");
        return;
    }

    neig_info->last_contact_time = protocol_core_monotonic_time;
    neig_info->medium_ttl_challenge = false;

    if (dataPollConfirmation) {
        if (neig_info->ttl > MLE_TABLE_CHALLENGE_TIMER) {
            neig_info->ttl = neig_info->timeout_rx;
        }
    } else {
        neig_info->ttl = neig_info->timeout_rx;
    }
}

mle_neigh_table_entry_t *mle_refresh_entry_timeout(int8_t interfaceId, const uint8_t *addressPtr, addrtype_t addressType, bool dataPollConfirmation)
{
    mle_neigh_table_entry_t * neigh_info =  mle_class_get_by_link_address(interfaceId, addressPtr, addressType);
    mle_refresh_entry(neigh_info, dataPollConfirmation);
    return neigh_info;
}

#endif /* NO_MLE */
