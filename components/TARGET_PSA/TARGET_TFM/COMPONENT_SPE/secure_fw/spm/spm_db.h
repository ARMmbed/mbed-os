/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SPM_DB_H__
#define __SPM_DB_H__


#ifdef TFM_PSA_API
#include "tfm_thread.h"
#endif

struct spm_partition_desc_t;
struct spm_partition_db_t;

typedef psa_status_t(*sp_init_function)(void);

#define TFM_PARTITION_TYPE_APP   "APPLICATION-ROT"
#define TFM_PARTITION_TYPE_PSA   "PSA-ROT"

#ifdef TFM_PSA_API
enum tfm_partition_priority {
    TFM_PRIORITY_LOW = THRD_PRIOR_LOWEST,
    TFM_PRIORITY_NORMAL = THRD_PRIOR_MEDIUM,
    TFM_PRIORITY_HIGH = THRD_PRIOR_HIGHEST,
};
#else
enum tfm_partition_priority {
    TFM_PRIORITY_LOW = 0xFF,
    TFM_PRIORITY_NORMAL = 0x7F,
    TFM_PRIORITY_HIGH = 0,
};
#endif

#define TFM_PRIORITY(LEVEL)      TFM_PRIORITY_##LEVEL

/**
 * Holds the fields of the partition DB used by the SPM code. The values of
 * these fields are calculated at compile time, and set during initialisation
 * phase.
 */
struct spm_partition_static_data_t {
    uint32_t partition_id;
    uint32_t partition_flags;
    uint32_t partition_priority;
    sp_init_function partition_init;
};

/**
 * Holds the fields that define a partition for SPM. The fields are further
 * divided to structures, to keep the related fields close to each other.
 */
struct spm_partition_desc_t {
    struct spm_partition_static_data_t static_data;
    struct spm_partition_runtime_data_t runtime_data;
    struct tfm_spm_partition_platform_data_t *platform_data;
#if (TFM_LVL != 1) || defined(TFM_PSA_API)
    struct tfm_spm_partition_memory_data_t memory_data;
#endif
#ifdef TFM_PSA_API
    struct tfm_thrd_ctx sp_thrd;
#endif
};

/* Macros to pick linker symbols and allow to form the partition data base */
#define REGION(a, b, c) a##b##c
#define REGION_NAME(a, b, c) REGION(a, b, c)
/* Changed from #if (TFM_LVL == 1) && !defined(TFM_PSA_API) to #if (TFM_LVL == 1) to avoid linker error.
   TF-M build autogenerates region details (code, ro, rw, zi and stack ) using linker scripts. We do not
   hve that in mbed-os build yet.
*/
#if (TFM_LVL == 1)
#define REGION_DECLARE(a, b, c)
#else
#define REGION_DECLARE(a, b, c) extern uint32_t REGION_NAME(a, b, c)
#define PART_REGION_ADDR(partition, region) \
    (uint32_t)&REGION_NAME(Image$$, partition, region)
#endif

#endif /* __SPM_DB_H__ */
