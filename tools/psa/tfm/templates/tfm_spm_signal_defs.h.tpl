/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_SPM_SIGNAL_DEFS_H__
#define __TFM_SPM_SIGNAL_DEFS_H__

{% for partition in partitions %}
{% for rot_srv in partition.rot_services %}
#define {{rot_srv.signal|upper}}_POS ({{loop.index + 3}}UL)
#define {{rot_srv.signal|upper}} (1UL << {{rot_srv.signal|upper}}_POS)
{% endfor %}

{% endfor %}

#endif
