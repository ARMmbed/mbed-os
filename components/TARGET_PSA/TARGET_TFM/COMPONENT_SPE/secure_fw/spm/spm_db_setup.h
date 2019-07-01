/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SPM_DB_SETUP_H__
#define __SPM_DB_SETUP_H__

#include <stdint.h>
#include "spm_db.h"

/**
 * \brief Get the index of a partition.
 *
 * Gets the index of a partition in the partition db based on the partition ID
 * provided as a parameter.
 *
 * \param[in] partition_idx    The index of the partition
 *
 * \return \ref INVALID_PARTITION_IDX if the provided index is invalid. The
 *         index of the partition otherwise.
 */
uint32_t get_partition_idx(uint32_t partition_id);

struct spm_partition_db_t {
    uint32_t is_init;
    uint32_t partition_count;
    uint32_t running_partition_idx;
    struct spm_partition_desc_t partitions[SPM_MAX_PARTITIONS];
};

#define PARTITION_INIT_STATIC_DATA(data, partition, flags, id, priority)      \
    do {                                                                      \
        data.partition_id    = partition##_ID;                                \
        data.partition_flags = flags;                                         \
        data.partition_priority = TFM_PRIORITY(priority);                     \
    } while (0)

/* Changed from #if (TFM_LVL == 1) && !defined(TFM_PSA_API) to #if (TFM_LVL == 1) to avoid linker error.
   TF-M build autogenerates region details (code, ro, rw, zi and stack ) using linker scripts. We do not
   hve that in mbed-os build yet.
*/
#if (TFM_LVL == 1)
#define PARTITION_INIT_MEMORY_DATA(data, partition)
#else
#define PARTITION_INIT_MEMORY_DATA(data, partition)                            \
    do {                                                                       \
        data.code_start      = PART_REGION_ADDR(partition, $$Base);            \
        data.code_limit      = PART_REGION_ADDR(partition, $$Limit);           \
        data.ro_start        = PART_REGION_ADDR(partition, $$RO$$Base);        \
        data.ro_limit        = PART_REGION_ADDR(partition, $$RO$$Limit);       \
        data.rw_start        = PART_REGION_ADDR(partition, _DATA$$RW$$Base);   \
        data.rw_limit        = PART_REGION_ADDR(partition, _DATA$$RW$$Limit);  \
        data.zi_start        = PART_REGION_ADDR(partition, _DATA$$ZI$$Base);   \
        data.zi_limit        = PART_REGION_ADDR(partition, _DATA$$ZI$$Limit);  \
        data.stack_bottom    = PART_REGION_ADDR(partition, _STACK$$ZI$$Base);  \
        data.stack_top       = PART_REGION_ADDR(partition, _STACK$$ZI$$Limit); \
    } while (0)
#endif

#if TFM_LVL == 1
#define PARTITION_INIT_RUNTIME_DATA(data, partition)            \
    do {                                                        \
        data.partition_state      = SPM_PARTITION_STATE_UNINIT; \
    } while (0)
#else
#define PARTITION_INIT_RUNTIME_DATA(data, partition)                \
    do {                                                            \
        data.partition_state      = SPM_PARTITION_STATE_UNINIT;     \
        /* The top of the stack is reserved for the iovec        */ \
        /* parameters of the service called. That's why in       */ \
        /* data.stack_ptr we extract sizeof(struct iovec_args_t) */ \
        /* from the limit.                                       */ \
        data.stack_ptr            =                                 \
                PART_REGION_ADDR(partition, _STACK$$ZI$$Limit -     \
                                  sizeof(struct iovec_args_t));     \
    } while (0)
#endif

#define PARTITION_DECLARE(partition, flag, type, id, priority, part_stack_size)                     \
    do {                                                                                            \
        REGION_DECLARE(Image$$, partition, $$Base);                                                 \
        REGION_DECLARE(Image$$, partition, $$Limit);                                                \
        REGION_DECLARE(Image$$, partition, $$RO$$Base);                                             \
        REGION_DECLARE(Image$$, partition, $$RO$$Limit);                                            \
        REGION_DECLARE(Image$$, partition, _DATA$$RW$$Base);                                        \
        REGION_DECLARE(Image$$, partition, _DATA$$RW$$Limit);                                       \
        REGION_DECLARE(Image$$, partition, _DATA$$ZI$$Base);                                        \
        REGION_DECLARE(Image$$, partition, _DATA$$ZI$$Limit);                                       \
        REGION_DECLARE(Image$$, partition, _STACK$$ZI$$Base);                                       \
        REGION_DECLARE(Image$$, partition, _STACK$$ZI$$Limit);                                      \
        int32_t flags = flag;                                                                       \
        if (tfm_memcmp(type, TFM_PARTITION_TYPE_APP,                                                \
            strlen(TFM_PARTITION_TYPE_APP)) == 0) {                                                 \
            flags |= SPM_PART_FLAG_APP_ROT;                                                         \
        } else if (tfm_memcmp(type, TFM_PARTITION_TYPE_PSA,                                         \
                   strlen(TFM_PARTITION_TYPE_PSA)) == 0) {                                          \
            flags |= SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT;                                 \
        } else {                                                                                    \
            return SPM_ERR_INVALID_CONFIG;                                                          \
        }                                                                                           \
        struct spm_partition_desc_t *part_ptr;                                                      \
        if (g_spm_partition_db.partition_count >= SPM_MAX_PARTITIONS) {                             \
            return SPM_ERR_INVALID_CONFIG;                                                          \
        }                                                                                           \
        __attribute__((section(".data.partitions_stacks")))                                         \
        static uint8_t partition##_stack[part_stack_size] __attribute__((aligned(8)));              \
        part_ptr = &(g_spm_partition_db.partitions[                                                 \
            g_spm_partition_db.partition_count]);                                                   \
        part_ptr->memory_data.stack_bottom = (uint32_t)partition##_stack;                           \
        part_ptr->memory_data.stack_top = part_ptr->memory_data.stack_bottom + part_stack_size;     \
        PARTITION_INIT_STATIC_DATA(part_ptr->static_data, partition, flags,                         \
                                   id, priority);                                                   \
        PARTITION_INIT_RUNTIME_DATA(part_ptr->runtime_data, partition);                             \
        PARTITION_INIT_MEMORY_DATA(part_ptr->memory_data, partition);                               \
        ++g_spm_partition_db.partition_count;                                                       \
    } while (0)

#define PARTITION_ADD_INIT_FUNC(partition, init_func)                                               \
    do {                                                                                            \
        extern int32_t init_func(void);                                                             \
        uint32_t partition_idx = get_partition_idx(partition##_ID);                                 \
        struct spm_partition_desc_t *part_ptr =                                                     \
            &(g_spm_partition_db.partitions[partition_idx]);                                        \
        part_ptr->static_data.partition_init = init_func;                                           \
    } while (0)

#define PARTITION_ADD_PERIPHERAL(partition, peripheral)                                             \
    do {                                                                                            \
        uint32_t partition_idx = get_partition_idx(partition##_ID);                                 \
        struct spm_partition_desc_t *part_ptr =                                                     \
            &(g_spm_partition_db.partitions[partition_idx]);                                        \
        part_ptr->platform_data = peripheral;                                                       \
    } while (0)

#endif /* __SPM_DB_SETUP_H__ */
