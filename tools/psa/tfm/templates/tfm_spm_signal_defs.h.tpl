/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __TFM_SPM_SIGNAL_DEFS_H__
#define __TFM_SPM_SIGNAL_DEFS_H__

{% for partition in service_partitions %}
/******** {{partition.name|upper}} ********/
{% for rot_srv in partition.rot_services %}
#define {{rot_srv.signal|upper}}_POS ({{loop.index + 3}}UL)
#define {{rot_srv.signal|upper}} (1UL << {{rot_srv.signal|upper}}_POS)
{% endfor %}

{% endfor %}
#ifdef USE_PSA_TEST_PARTITIONS

{% for partition in test_partitions %}
#ifdef USE_{{partition.name|upper}}
/******** {{partition.name|upper}} ********/
{% for rot_srv in partition.rot_services %}
#define {{rot_srv.signal|upper}}_POS ({{loop.index + 3}}UL)
#define {{rot_srv.signal|upper}} (1UL << {{rot_srv.signal|upper}}_POS)
{% endfor %}
#endif // USE_{{partition.name|upper}}

{% endfor %}
#endif // USE_PSA_TEST_PARTITIONS

#endif // __TFM_SPM_SIGNAL_DEFS_H__
