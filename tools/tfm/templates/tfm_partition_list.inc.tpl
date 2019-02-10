/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __TFM_PARTITION_LIST_INC__
#define __TFM_PARTITION_LIST_INC__

{% for partition in partitions %}
#ifdef TFM_PSA_API
/******** {{partition.name|upper}} ********/
PARTITION_DECLARE({{partition.name|upper}}, 0
    | SPM_PART_FLAG_IPC
    , "{{partition.type}}", {{partition.id}}, {{partition.priority}}, {{partition.stack_size}});
PARTITION_ADD_INIT_FUNC({{partition.name|upper}}, {{partition.entry_point}});
#endif /* TFM_PSA_API */

{% endfor %}
#endif /* __TFM_PARTITION_LIST_INC__ */
