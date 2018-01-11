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

#define {{partition.name|upper}}_SF_COUNT ({{partition.secure_functions|count}}UL)
#define {{partition.name|upper}}_EXT_SF_COUNT ({{partition.extern_sfids|count}}UL)

/* {{partition.name}} event flags */
#define {{partition.name|upper}}_RESERVED1_POS (1UL)
#define {{partition.name|upper}}_RESERVED1_MSK (1UL << {{partition.name|upper}}_RESERVED1_POS)

#define {{partition.name|upper}}_RESERVED2_POS (2UL)
#define {{partition.name|upper}}_RESERVED2_MSK (1UL << {{partition.name|upper}}_RESERVED2_POS)

#define {{partition.name|upper}}_DOORBELL_POS (3UL)
#define {{partition.name|upper}}_DOORBELL_MSK (1UL << {{partition.name|upper}}_DOORBELL_POS)

{% for sf in partition.secure_functions %}
#define {{sf.signal|upper}}_POS ({{loop.index + 3}}UL)
#define {{sf.signal|upper}} (1UL << {{sf.signal|upper}}_POS)
{% endfor %}

#define {{partition.name|upper}}_WAIT_ANY_SFID_MSK (\
{% for sf in partition.secure_functions %}
    {{sf.signal|upper}}{{")" if loop.last else " | \\"}}
{% endfor %}

#define {{partition.name|upper}}_WAIT_ANY_IRQ_MSK (0) // TODO: Implement when IRQ logic is ready

#define {{partition.name|upper}}_WAIT_ANY_MSK (\
    {{partition.name|upper}}_DOORBELL_MSK | \
    {{partition.name|upper}}_WAIT_ANY_SFID_MSK | \
    {{partition.name|upper}}_WAIT_ANY_IRQ_MSK)

#endif // PSA_{{partition.name|upper}}_PARTITION_H
{# End of file #}
