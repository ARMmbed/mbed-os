/*
 * Copyright (c) 2017-2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SPM_PARTITION_DEFS_H__
#define __SPM_PARTITION_DEFS_H__

/* FixMe: allocations to be settled.
 * 8 bits reserved by TFM for secure partition Id in this prototype
 */
#define TFM_SP_BASE 256

/* A reserved partition ID that is used for uninitialised data */
#define INVALID_PARTITION_ID (~0U)

/* ***** partition ID-s internal to the TFM ***** */
#define TFM_INTERNAL_PARTITIONS (2)

/* From the SPM point of view the non secure processing environment is handled
 * as a special secure partition. This simplifies the context switch
 * operations.
 */
#define TFM_SP_NON_SECURE_ID (0)
/* A dummy partition for TFM_SP_CORE is created to handle secure partition
 * calls done directly from the core, before NS execution started.
 */
#define TFM_SP_CORE_ID (1)

#include "tfm_partition_defs.inc"

/* This limit is only used to define the size of the database reserved for
 * partitions. There's no requirement that it match the number of partitions
 * that get registered in a specific build
 */
#define SPM_MAX_PARTITIONS (TFM_MAX_USER_PARTITIONS + TFM_INTERNAL_PARTITIONS)

#endif /* __SPM_PARTITION_DEFS_H__ */
