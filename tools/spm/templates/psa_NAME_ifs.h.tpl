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

#ifndef PSA_{{partition.name|upper}}_PARTITION_SECURE_FUNCTIONS_H
#define PSA_{{partition.name|upper}}_PARTITION_SECURE_FUNCTIONS_H

{% for sf in partition.secure_functions %}
#define {{sf.name|upper}} {{sf.id}}
{% endfor %}

#endif // PSA_{{partition.name|upper}}_PARTITION_SECURE_FUNCTIONS_H
{# End of file #}
