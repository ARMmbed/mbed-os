/* Copyright (c) 2019 ARM Limited
 *
 * SPDX-License-Identifier: Apache-2.0
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

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

{% for partition in service_partitions %}
/******** {{partition.name|upper}} ********/
{% for rot_srv in partition.rot_services %}
#define {{rot_srv.name|upper}} {{rot_srv.id}}
{% endfor %}

{% endfor %}

{% for partition in test_partitions %}

/******** {{partition.name|upper}} ********/
{% for rot_srv in partition.rot_services %}
#define {{rot_srv.name|upper}} {{rot_srv.id}}
{% endfor %}

{% endfor %}
