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

#ifndef PSA_{{partition.name|upper}}_PARTITION_H
#define PSA_{{partition.name|upper}}_PARTITION_H

#define {{partition.name|upper}}_ID {{partition.id}}

{% if partition.rot_services|count > 0 %}
#define {{partition.name|upper}}_ROT_SRV_COUNT ({{partition.rot_services|count}}UL)
{% endif %}
#define {{partition.name|upper}}_EXT_ROT_SRV_COUNT ({{partition.extern_sids|count}}UL)

/* {{partition.name}} event flags */
#define {{partition.name|upper}}_RESERVED1_POS (1UL)
#define {{partition.name|upper}}_RESERVED1_MSK (1UL << {{partition.name|upper}}_RESERVED1_POS)

#define {{partition.name|upper}}_RESERVED2_POS (2UL)
#define {{partition.name|upper}}_RESERVED2_MSK (1UL << {{partition.name|upper}}_RESERVED2_POS)

{% for irq in partition.irqs %}
#define {{irq.signal|upper}}_POS ({{loop.index + 3 }}UL)
#define {{irq.signal|upper}} (1UL << {{irq.signal|upper}}_POS)
{% endfor %}

{% if partition.irqs|count > 0 %}
#define {{partition.name|upper}}_WAIT_ANY_IRQ_MSK (\
{% for irq in partition.irqs %}
    {{irq.signal|upper}}{{")" if loop.last else " | \\"}}
{% endfor %}
{% endif %}

{% for rot_srv in partition.rot_services %}
#define {{rot_srv.signal|upper}}_POS ({{loop.index + 3 + partition.irqs|count}}UL)
#define {{rot_srv.signal|upper}} (1UL << {{rot_srv.signal|upper}}_POS)
{% endfor %}

{% if partition.rot_services|count > 0 %}
#define {{partition.name|upper}}_WAIT_ANY_SID_MSK (\
{% for rot_srv in partition.rot_services %}
    {{rot_srv.signal|upper}}{{")" if loop.last else " | \\"}}
{% endfor %}
{% endif %}

/*
#define {{partition.name|upper}}_WAIT_ANY_MSK (\
{% if partition.irqs|count > 0 %}
    {{partition.name|upper}}_WAIT_ANY_IRQ_MSK | \
{% endif %}
{% if partition.rot_services|count > 0 %}
    {{partition.name|upper}}_WAIT_ANY_SID_MSK) | \
{% endif %}
    PSA_DOORBELL)
*/

{% if partition.irqs|count > 0 %}
uint32_t spm_{{partition.name|lower}}_signal_to_irq_mapper(uint32_t signal);
{% endif %}

#endif // PSA_{{partition.name|upper}}_PARTITION_H
{# End of file #}
