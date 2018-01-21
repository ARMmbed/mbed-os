/* Copyright (c) 2017 ARM Limited
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

/***********************************************************************************************************************
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * THIS FILE IS AN AUTO-GENERATED FILE - DO NOT MODIFY IT.
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 **********************************************************************************************************************/

#include "spm_panic.h"
#include "spm_internal.h"
#include "handles_manager.h"
{% for partition in partitions %}
#include "psa_{{partition.name|lower}}_partition.h"
{% endfor %} {# partition in partitions #}

{% for partition in partitions %}
{% if partition.extern_sfids|count > 0 %}
extern const uint32_t {{partition.name|lower}}_external_sfids[{{partition.extern_sfids|count}}];
{% endif %}
{% endfor %} {# partition in partitions #}

{% if partitions|count > 0 %}
partition_t g_partitions[{{partitions|count}}] = {
{% for partition in partitions %}
    {
        .partition_id = {{partition.id}},
        .partition_state = PARTITION_STATE_INVALID,
        .thread_id = 0,
        .flags_sf = {{partition.name|upper}}_WAIT_ANY_SFID_MSK,
        .flags_interrupts = {{partition.name|upper}}_WAIT_ANY_IRQ_MSK | {{partition.name|upper}}_DOORBELL_MSK,
        .sec_funcs = NULL,
        .sec_funcs_count = {{partition.name|upper}}_SF_COUNT,
    {% if partition.extern_sfids|count > 0 %}
        .extern_sfids = {{partition.name|lower}}_external_sfids,
    {% else %}
        .extern_sfids = NULL,
    {% endif %}
        .extern_sfids_count = {{partition.name|upper}}_EXT_SF_COUNT,
        .mutex = NULL,
        .semaphore = NULL,
        .active_msg = {0},
        .msg_handle = PSA_NULL_HANDLE
    },
{% endfor %}
};
{% else %}
partition_t *g_partitions = NULL;
{% endif %}

{% if partitions|count > 0 %}
psa_handle_item_t g_messages_handle_storage[{{partitions|count}}] = {0};
{% else %}
psa_handle_item_t *g_messages_handle_storage = NULL;
{% endif %}

/* Check all the defined MMIO regions for overlapping. */
{% for region_pair in region_pair_list %}
static_assert(
    ((uintptr_t)({{region_pair[0].base}}) + {{region_pair[0].size}} - 1 < (uintptr_t)({{region_pair[1].base}})) ||
    ((uintptr_t)({{region_pair[1].base}}) + {{region_pair[1].size}} - 1 < (uintptr_t)({{region_pair[0].base}})),
    "The region with base {{region_pair[0].base}} and size {{region_pair[0].size}} overlaps with the region with base {{region_pair[1].base}} and size {{region_pair[1].size}}!");
{% endfor %}

// forward declaration of partition initializers
{% for partition in partitions %}
void {{partition.name|lower}}_init(partition_t *partition);
{% endfor %} {# partition in partitions #}

uint32_t init_partitions(partition_t **partitions)
{
    if (NULL == partitions) {
        SPM_PANIC("partitions is NULL!\n");
    }

{% for partition in partitions %}
    {{partition.name|lower}}_init(&(g_partitions[{{loop.index0}}]));
{% endfor %} {# partition in partitions #}

    *partitions = g_partitions;
    return {{partitions|count}};
}
{# End of file #}
