/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

/**
 * \file ns_monitor.c
 * \brief Utility functions for nanostack maintenance
 *
 * This module tracks stack current heap usage and triggers GC if heap usage is too high.
 * GC is triggered by:
 *  1. Heap usage is above HEAP_USAGE_HIGH
 *  2. Heap usage is above HEAP_USAGE_CRITICAL
 *  3. If nsdynmemLIB memory allocation has failed since last check
 */

#include "nsconfig.h"
#include "ns_types.h"
#define HAVE_DEBUG
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "ipv6_stack/ipv6_routing_table.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"
#include "NWK_INTERFACE/Include/protocol.h"

#define TRACE_GROUP "mntr"

typedef enum {
    NS_MONITOR_STATE_HEAP_GC_IDLE = 0,
    NS_MONITOR_STATE_HEAP_GC_HIGH,
    NS_MONITOR_STATE_GC_CRITICAL
} ns_monitor_state_e;

#define DEFAULT_HEAP_PERCENTAGE_THRESHOLD_HIGH      95
#define DEFAULT_HEAP_PERCENTAGE_THRESHOLD_CRITICAL  99

#define SET_WATERMARK(SECTOR_SIZE, THRESHOLD)   (SECTOR_SIZE * THRESHOLD / 100)

#define NS_MAINTENANCE_TIMER_INTERVAL   10  // Maintenance interval

typedef struct ns_monitor__s {
    ns_mem_heap_size_t heap_high_watermark;
    ns_mem_heap_size_t heap_critical_watermark;
    uint32_t prev_heap_alloc_fail_cnt;
    ns_monitor_state_e ns_monitor_heap_gc_state;
    const mem_stat_t *mem_stats;
    uint16_t ns_maintenance_timer;
} ns_monitor_t;

static ns_monitor_t *ns_monitor_ptr = NULL;

static ns_mem_heap_size_t ns_dyn_mem_rate_limiting_threshold = 0; // amount of free memory required to allow routing 0 = disabled

typedef void (ns_maintenance_gc_cb)(bool full_gc);

/*
 * Garbage collection functions.
 * Add more GC performing functions to the table
 *
 */
static ns_maintenance_gc_cb *ns_maintenance_gc_functions[] = {
    ipv6_destination_cache_forced_gc,
    ws_pae_controller_forced_gc,
    lowpan_adaptation_free_heap
};

static void ns_monitor_heap_gc(bool full_gc)
{
    (void) full_gc;

    for (unsigned int i = 0; i < sizeof(ns_maintenance_gc_functions) / sizeof(ns_maintenance_gc_functions[0]); i++) {
        if (ns_maintenance_gc_functions[i]) {
            (ns_maintenance_gc_functions[i])(full_gc);
        }
    }
}

static void ns_monitor_periodic_heap_health_check(void)
{
    if (ns_monitor_ptr->mem_stats->heap_sector_allocated_bytes > ns_monitor_ptr->heap_critical_watermark) {
        // Heap usage above CRITICAL
        if (ns_monitor_ptr->ns_monitor_heap_gc_state != NS_MONITOR_STATE_GC_CRITICAL) {
            ns_mem_heap_size_t prev_heap_sector_allocated_bytes = ns_monitor_ptr->mem_stats->heap_sector_allocated_bytes;
            tr_debug("heap %lu/%lu", (unsigned long)ns_monitor_ptr->mem_stats->heap_sector_allocated_bytes, (unsigned long)ns_monitor_ptr->mem_stats->heap_sector_size);
            ns_monitor_heap_gc(true);
            ns_monitor_ptr->ns_monitor_heap_gc_state = NS_MONITOR_STATE_GC_CRITICAL;
            tr_info("Stack GC critical: freed %lu bytes", (unsigned long)(prev_heap_sector_allocated_bytes - ns_monitor_ptr->mem_stats->heap_sector_allocated_bytes));
        }
    } else if (ns_monitor_ptr->mem_stats->heap_sector_allocated_bytes > ns_monitor_ptr->heap_high_watermark) {
        // Heap usage above HIGH
        if (ns_monitor_ptr->ns_monitor_heap_gc_state == NS_MONITOR_STATE_HEAP_GC_IDLE) {
            ns_mem_heap_size_t prev_heap_sector_allocated_bytes = ns_monitor_ptr->mem_stats->heap_sector_allocated_bytes;
            tr_debug("heap %lu/%lu", (unsigned long)ns_monitor_ptr->mem_stats->heap_sector_allocated_bytes, (unsigned long)ns_monitor_ptr->mem_stats->heap_sector_size);
            ns_monitor_heap_gc(false);
            ns_monitor_ptr->ns_monitor_heap_gc_state = NS_MONITOR_STATE_HEAP_GC_HIGH;
            tr_info("Stack GC high: freed %lu bytes", (unsigned long)(prev_heap_sector_allocated_bytes - ns_monitor_ptr->mem_stats->heap_sector_allocated_bytes));
        }
    } else if (ns_monitor_ptr->mem_stats->heap_sector_allocated_bytes <= ns_monitor_ptr->heap_high_watermark) {
        // Heap usage in normal range
        ns_monitor_ptr->ns_monitor_heap_gc_state = NS_MONITOR_STATE_HEAP_GC_IDLE;
    }
}

void ns_monitor_timer(uint16_t seconds)
{
    if (ns_monitor_ptr) {
        ns_monitor_ptr->ns_maintenance_timer += seconds;

        if (ns_monitor_ptr->mem_stats->heap_alloc_fail_cnt > ns_monitor_ptr->prev_heap_alloc_fail_cnt) {
            // Heap allocation failure occurred since last check
            ns_monitor_ptr->prev_heap_alloc_fail_cnt = ns_monitor_ptr->mem_stats->heap_alloc_fail_cnt;
            if (ns_monitor_ptr->ns_monitor_heap_gc_state != NS_MONITOR_STATE_GC_CRITICAL) {
                ns_monitor_ptr->ns_monitor_heap_gc_state = NS_MONITOR_STATE_GC_CRITICAL;
                ns_monitor_heap_gc(true);
                ns_monitor_ptr->ns_maintenance_timer = 0;
            }
        }

        if (ns_monitor_ptr->ns_maintenance_timer >= NS_MAINTENANCE_TIMER_INTERVAL) {
            ns_monitor_ptr->ns_maintenance_timer -= NS_MAINTENANCE_TIMER_INTERVAL;
            ns_monitor_periodic_heap_health_check();
        }
    }
}

int ns_monitor_init(void)
{
    if (ns_monitor_ptr || !ns_dyn_mem_get_mem_stat()) {
        // already initialized or memory statistics not available
        return -2;
    }

    ns_monitor_ptr = ns_dyn_mem_alloc(sizeof(ns_monitor_t));

    if (ns_monitor_ptr) {
        ns_monitor_ptr->mem_stats = ns_dyn_mem_get_mem_stat();
        ns_monitor_ptr->heap_high_watermark = SET_WATERMARK(
                                                  ns_monitor_ptr->mem_stats->heap_sector_size,
                                                  DEFAULT_HEAP_PERCENTAGE_THRESHOLD_HIGH
                                              );
        ns_monitor_ptr->heap_critical_watermark = SET_WATERMARK(
                                                      ns_monitor_ptr->mem_stats->heap_sector_size,
                                                      DEFAULT_HEAP_PERCENTAGE_THRESHOLD_CRITICAL
                                                  );
        ns_monitor_ptr->ns_monitor_heap_gc_state = NS_MONITOR_STATE_HEAP_GC_IDLE;
        ns_monitor_ptr->ns_maintenance_timer = 0;
        ns_monitor_ptr->prev_heap_alloc_fail_cnt = 0;
        return 0;
    }

    return -1;
}

int ns_monitor_clear(void)
{
    if (ns_monitor_ptr) {
        ns_dyn_mem_free(ns_monitor_ptr);
        ns_monitor_ptr = NULL;
        return 0;
    }

    return -1;
}

int ns_monitor_heap_gc_threshold_set(uint32_t high_min, uint32_t high_max, uint8_t high_percentage, uint32_t critical_min, uint32_t critical_max, uint8_t critical_percentage)
{
    if (ns_monitor_ptr && (critical_percentage <= 100) && (high_percentage < critical_percentage)) {
        ns_monitor_ptr->heap_high_watermark = SET_WATERMARK(
                                                  ns_monitor_ptr->mem_stats->heap_sector_size,
                                                  high_percentage
                                              );
        if (ns_monitor_ptr->mem_stats->heap_sector_size - ns_monitor_ptr->heap_high_watermark < high_min) {
            ns_monitor_ptr->heap_high_watermark = ns_monitor_ptr->mem_stats->heap_sector_size - high_min;
        }

        if (high_max && ns_monitor_ptr->mem_stats->heap_sector_size - ns_monitor_ptr->heap_high_watermark > high_max) {
            ns_monitor_ptr->heap_high_watermark = ns_monitor_ptr->mem_stats->heap_sector_size - high_max;
        }

        ns_monitor_ptr->heap_critical_watermark = SET_WATERMARK(
                                                      ns_monitor_ptr->mem_stats->heap_sector_size,
                                                      critical_percentage
                                                  );
        if (ns_monitor_ptr->mem_stats->heap_sector_size - ns_monitor_ptr->heap_critical_watermark < critical_min) {
            ns_monitor_ptr->heap_critical_watermark = ns_monitor_ptr->mem_stats->heap_sector_size - critical_min;
        }

        if (critical_max && ns_monitor_ptr->mem_stats->heap_sector_size - ns_monitor_ptr->heap_critical_watermark > critical_max) {
            ns_monitor_ptr->heap_critical_watermark = ns_monitor_ptr->mem_stats->heap_sector_size - critical_max;
        }

        tr_info("Monitor set high:%lu, critical:%lu total:%lu", (unsigned long)ns_monitor_ptr->heap_high_watermark, (unsigned long)ns_monitor_ptr->heap_critical_watermark, (unsigned long)ns_monitor_ptr->mem_stats->heap_sector_size);
        return 0;
    }

    return -1;
}

int ns_monitor_packet_ingress_rate_limit_by_memory(uint32_t minimum_required, uint32_t Maximum_allowed, uint8_t free_heap_percentage)
{
    /* To make this function dynamic and useful in larger range of memories the minimum value can be given
     *
     * example limit(1024, 1)
     * 32k RAM      Limit = 1024
     * 64k RAM      Limit = 1024
     * 128k RAM     Limit = 1280
     * 320k RAM     Limit = 3200
    */
    if (free_heap_percentage == 0 && minimum_required == 0) {
        // Disable rate limiting
        ns_dyn_mem_rate_limiting_threshold = 0;
        return 0;
    }
    if (free_heap_percentage > 100) {
        // Sanity check this should not be high at all, but dont want to limit without any good reason
        return -1;
    }

    const mem_stat_t *ns_dyn_mem_stat = ns_dyn_mem_get_mem_stat();
    if (ns_dyn_mem_stat && free_heap_percentage) {
        ns_dyn_mem_rate_limiting_threshold = ns_dyn_mem_stat->heap_sector_size / 100 * free_heap_percentage;
    }

    if (ns_dyn_mem_rate_limiting_threshold < minimum_required) {
        ns_dyn_mem_rate_limiting_threshold = minimum_required;
    }

    if (Maximum_allowed && ns_dyn_mem_rate_limiting_threshold > Maximum_allowed) {
        ns_dyn_mem_rate_limiting_threshold = Maximum_allowed;
    }
    tr_info("Monitor rate limit incoming packets at:%lu", (unsigned long)ns_dyn_mem_rate_limiting_threshold);

    return 0;
}

bool ns_monitor_packet_allocation_allowed(void)
{
    // If there is no packets to forward this should not be blocked.
    // There should be cleanup routine enabled that will remove unneeded memory to prevent locks
    // this could trigger a function to clean packets from routing and allow newest packets

    const mem_stat_t *ns_dyn_mem_stat = ns_dyn_mem_get_mem_stat();

    if (ns_dyn_mem_stat && ns_dyn_mem_rate_limiting_threshold) {
        if (ns_dyn_mem_stat->heap_sector_size - ns_dyn_mem_stat->heap_sector_allocated_bytes < ns_dyn_mem_rate_limiting_threshold) {
            // Packet allocation not allowed as memory is running low.
            return false;
        }
    }
    return true;
}

