/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __TFM_PARTITION_LIST_INC__
#define __TFM_PARTITION_LIST_INC__

#ifdef TFM_PSA_API
/******** ITS ********/
PARTITION_DECLARE(ITS, 0
    | SPM_PART_FLAG_IPC
    , "APPLICATION-ROT", 10, NORMAL, 2048);
PARTITION_ADD_INIT_FUNC(ITS, its_entry);
#endif /* TFM_PSA_API */

#ifdef TFM_PSA_API
/******** PLATFORM ********/
PARTITION_DECLARE(PLATFORM, 0
    | SPM_PART_FLAG_IPC
    , "APPLICATION-ROT", 8, NORMAL, 1024);
PARTITION_ADD_INIT_FUNC(PLATFORM, platform_partition_entry);
#endif /* TFM_PSA_API */

#ifdef TFM_PSA_API
/******** CRYPTO_SRV ********/
PARTITION_DECLARE(CRYPTO_SRV, 0
    | SPM_PART_FLAG_IPC
    , "APPLICATION-ROT", 35, NORMAL, 16384);
PARTITION_ADD_INIT_FUNC(CRYPTO_SRV, crypto_main);
#endif /* TFM_PSA_API */

#endif /* __TFM_PARTITION_LIST_INC__ */