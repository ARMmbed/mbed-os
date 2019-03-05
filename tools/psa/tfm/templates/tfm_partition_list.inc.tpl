/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __TFM_PARTITION_LIST_INC__
#define __TFM_PARTITION_LIST_INC__

{% for partition in service_partitions %}
/******** {{partition.name|upper}} ********/
PARTITION_DECLARE({{partition.name|upper}}, 0
    | SPM_PART_FLAG_IPC
    , "{{partition.type}}", {{partition.id}}, {{partition.priority}}, {{partition.stack_size}});
PARTITION_ADD_INIT_FUNC({{partition.name|upper}}, {{partition.entry_point}});

{% endfor %}
#ifdef USE_PSA_TEST_PARTITIONS

{% for partition in test_partitions %}
#ifdef USE_{{partition.name|upper}}
/******** {{partition.name|upper}} ********/
PARTITION_DECLARE({{partition.name|upper}}, 0
    | SPM_PART_FLAG_IPC
    , "{{partition.type}}", {{partition.id}}, {{partition.priority}}, {{partition.stack_size}});
PARTITION_ADD_INIT_FUNC({{partition.name|upper}}, {{partition.entry_point}});
#endif // USE_{{partition.name|upper}}

{% endfor %}
#endif // USE_PSA_TEST_PARTITIONS

#endif // __TFM_PARTITION_LIST_INC__
