# Copyright (c) 2017-2018 ARM Limited
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

manifests = [
    {
        'name': 'TEST_PARTITION',
        'id': "0x7FFFFFFF",
        "type": "APPLICATION-ROT",
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,  # 512 == 0x200
        'heap_size': 2048,
        'mmio_regions': [
            {
                'name': 'PERIPH1',
                'permission': 'READ-ONLY'
            },
            {
                'name': 'PERIPH2',
                'permission': 'READ-ONLY'
            },
            {
                'base': '0xCCCCCCCC',
                'size': 4096, 'permission': 'READ-ONLY'
            },
            {
                'base': '0xDDDDDDDD',
                'size': 33554432, 'permission': 'READ-WRITE'
            }
        ],
        'services': [
            {
                'name': 'SID1',
                'identifier': '0x00000001',
                'signal': 'SID1',
                'minor_version': 1,
                'minor_policy': 'RELAXED',
                'non_secure_clients': True
            },
            {
                'name': 'SID2',
                'identifier': '0x00000002',
                'signal': 'SID2',
                'minor_version': 2,
                'minor_policy': 'STRICT',
                'non_secure_clients': False
            },
        ],
        'source_files': ['src1.cpp', 'src2.cpp'],
        'irqs': [
            {"line_num": 20, "signal": "ISR20"},
            {"line_num": 21, "signal": "ISR21"}
        ],
        'extern_sids': ['SID3', 'SID4']
    },
    {
        'name': 'TEST_PARTITION2',
        'id': "0x7FFFFFFE",
        "type": "APPLICATION-ROT",
        'priority': 'NORMAL',
        'entry_point': 'test2_main',
        'stack_size': 512,  # 512 == 0x200
        'heap_size': 2048,
        'mmio_regions': [
            {
                'name': 'PERIPH1',
                'permission': 'READ-ONLY'
            },
            {
                'name': 'PERIPH3',
                'permission': 'READ-ONLY'
            },
            {
                'base': '0xAAAAAAAA',
                'size': 4096, 'permission': 'READ-ONLY'
            },
            {
                'base': '0xBBBBBBBB',
                'size': 33554432, 'permission': 'READ-WRITE'
            }
        ],
        'services': [
            {
                'name': 'SID3',
                'identifier': '0x00000003',
                'signal': 'SID3',
                'minor_version': 5,
                'minor_policy': 'RELAXED',
                'non_secure_clients': True
            },
            {
                'name': 'SID4',
                'identifier': '0x00000004',
                'signal': 'SID4',
                'minor_version': 12,
                'minor_policy': 'STRICT',
                'non_secure_clients': False
            },
        ],
        'source_files': ['src3.cpp', 'src4.cpp'],
        'irqs': [
            {"line_num": 22, "signal": "ISR22"},
            {"line_num": 23, "signal": "ISR23"}
        ]
    }
]

manifests_for_circular_call_dependency_checks = [
    {
        'name': 'PARTITION1',
        'id': '0x7FFFFFFF',
        'type': 'APPLICATION-ROT',
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,
        'heap_size': 2048,
        'source_files': ['src1.cpp'],
        'services': [
            {
                'name': 'SID1',
                'identifier': '0x00000001',
                'signal': 'SID1',
                'non_secure_clients': False
            },
            {
                'name': 'SID2',
                'identifier': '0x00000002',
                'signal': 'SID2',
                'non_secure_clients': False
            }
        ],
        'extern_sids': ['SID3', 'SID4']
    },
    {
        'name': 'PARTITION2',
        'id': '0x7FFFFFFE',
        'type': 'APPLICATION-ROT',
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,
        'heap_size': 2048,
        'source_files': ['src2.cpp'],
        'services': [
            {
                'name': 'SID3',
                'identifier': '0x00000003',
                'signal': 'SID3',
                'non_secure_clients': False
            },
            {
                'name': 'SID4',
                'identifier': '0x00000004',
                'signal': 'SID4',
                'non_secure_clients': False
            }
        ],
        'extern_sids': ['SID1', 'SID2']
    },
    {
        'name': 'PARTITION3',
        'id': '0x7FFFFFFD',
        'type': 'APPLICATION-ROT',
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,
        'heap_size': 2048,
        'source_files': ['src3.cpp'],
        'services': [
            {
                'name': 'SID5',
                'identifier': '0x00000005',
                'signal': 'SID5',
                'non_secure_clients': False
            }
        ],
        'extern_sids': ['SID7']
    },
    {
        'name': 'PARTITION4',
        'id': '0x7FFFFFFC',
        'type': 'APPLICATION-ROT',
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,
        'heap_size': 2048,
        'source_files': ['src4.cpp'],
        'services': [
            {
                'name': 'SID6',
                'identifier': '0x00000006',
                'signal': 'SID6',
                'non_secure_clients': False
            },
            {
                'name': 'SID7',
                'identifier': '0x00000007',
                'signal': 'SID7',
                'non_secure_clients': False
            },
        ],
        'extern_sids': ['SID9']
    },
    {
        'name': 'PARTITION5',
        'id': '0x7FFFFFFB',
        'type': 'APPLICATION-ROT',
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,
        'heap_size': 2048,
        'source_files': ['src5.cpp'],
        'services': [
            {
                'name': 'SID8',
                'identifier': '0x00000008',
                'signal': 'SID8',
                'non_secure_clients': False
            },
            {
                'name': 'SID9',
                'identifier': '0x00000009',
                'signal': 'SID9',
                'non_secure_clients': False
            }
        ],
        'extern_sids': ['SID5']
    },
    {
        'name': 'PARTITION6',
        'id': '0x7FFFFFFA',
        'type': 'APPLICATION-ROT',
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,
        'heap_size': 2048,
        'source_files': ['src6.cpp'],
        'services': [
            {
                'name': 'SID10',
                'identifier': '0x0000000A',
                'signal': 'SID10',
                'non_secure_clients': False
            },
            {
                'name': 'SID11',
                'identifier': '0x0000000B',
                'signal': 'SID11',
                'non_secure_clients': False
            }
        ],
        'extern_sids': ['SID7', 'SID5']
    },
    {
        'name': 'PARTITION7',
        'id': '0x7FFFFFF9',
        'type': 'APPLICATION-ROT',
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,
        'heap_size': 2048,
        'source_files': ['src6.cpp'],
        'services': [
            {
                'name': 'SID12',
                'identifier': '0x0000000C',
                'signal': 'SID12',
                'non_secure_clients': False
            }
        ]
    }
]

invalid_minor_version_policy_rot_srv = [
    {
        'name': 'SID1',
        'identifier': '0x00000001',
        'signal': 'SID1',
        'minor_version': 1,
        'minor_policy': 'invalid_policy',
        'non_secure_clients': True
    }
]

invalid_nspe_callable_rot_srv = [
    {
        'name': 'SID1',
        'identifier': '0x00000001',
        'signal': 'SID1',
        'minor_version': 1,
        'minor_policy': 'STRICT',
        'non_secure_clients': 'invalid_value'
    }
]

missing_nspe_callable_rot_srv = [
    {
        'name': 'SID1',
        'identifier': '0x00000001',
        'signal': 'SID1',
        'minor_version': 1,
        'minor_policy': 'STRICT'
    }
]

duplicate_signal_rot_services = [
    {
        'name': 'SID3',
        'identifier': '0x00000001',
        'signal': 'SID1',
        'minor_version': 5,
        'minor_policy': 'RELAXED',
        'non_secure_clients': True
    },
    {
        'name': 'SID4',
        'identifier': '0x00000002',
        'signal': 'SID2',
        'minor_version': 12,
        'minor_policy': 'STRICT',
        'non_secure_clients': True
    },
]

duplicate_identifier_rot_services = [
    {
        'name': 'SID3',
        'identifier': '0x00000003',
        'signal': 'SID3',
        'minor_version': 5,
        'minor_policy': 'RELAXED',
        'non_secure_clients': True
    },
    {
        'name': 'SID4',
        'identifier': '0x00000002',
        'signal': 'SID4',
        'minor_version': 12,
        'minor_policy': 'STRICT',
        'non_secure_clients': True
    },
]

spe_contained_rot_services = [
    {
        'name': 'SID5',
        'identifier': '0x00000005',
        'signal': 'SID5',
        'minor_version': 5,
        'minor_policy': 'RELAXED',
        'non_secure_clients': False
    },
    {
        'name': 'SID6',
        'identifier': '0x00000006',
        'signal': 'SID6',
        'minor_version': 12,
        'minor_policy': 'STRICT',
        'non_secure_clients': False
    }
]

missing_minor_version_rot_srv = [
    {
        'name': 'SID1',
        'identifier': '0x00000001',
        'signal': 'SID1',
        'minor_policy': 'RELAXED',
        'non_secure_clients': True
    }
]

missing_minor_version_policy_rot_srv = [
    {
        'name': 'SID2',
        'identifier': '0x00000002',
        'signal': 'SID2',
        'minor_version': 1,
        'non_secure_clients': True
    }
]

missing_minor_completley_rot_srv = [
    {'name': 'SID2', 'identifier': '0x00000002', 'signal': 'SID2',
     'non_secure_clients': True}
]

duplicate_signal_irqs = [
    {"line_num": 22, "signal": "ISR20"}
]

duplicate_line_num_irqs = [
    {"line_num": 21, "signal": "ISR22"}
]

invalid_mmioregion_base = {
    'base': 'str',
    'size': 4096,
    'permission': 'READ-ONLY'
}

invalid_mmioregion_size = {
    'base': '0xEEEEEEEE',
    'size': 'str',
    'permission': 'READ-ONLY'
}

test_mock_files = {
    'manifest1': 1,
    'manifest2': 2,
    'template_common1': 3,
    'template_common2': 4,
    'template_NAME_3': 5,
    'template_NAME_4': 6,
    'gen1': 7,
    'gen2': 8,
    'gen3': 9,
    'gen4': 10,
    'gen5': 11,
    'gen6': 12
}

test_common_template = '''{
    "num_of_partitions": {{partitions|count}},
    "partition_names": [
{% for partition in partitions %}
        "{{partition.name}}"{{"" if loop.last else ","}}
{% endfor %}
    ],
    "num_of_region_pairs": {{region_pair_list|count}}
}
'''

test_common_expected = '''{
    "num_of_partitions": 2,
    "partition_names": [
        "TEST_PARTITION",
        "TEST_PARTITION2"
    ],
    "num_of_region_pairs": 28
}
'''

test_partition_template = '''{
    "name": "{{partition.name}}",
    "id": "0x{{"%0x"|format(partition.id|int)|upper}}",
    "type": "{{partition.type}}",
    "priority": "{{partition.priority_mbed|find_priority_key}}",
    "entry_point": "{{partition.entry_point}}",
    "stack_size": {{partition.stack_size}},
    "heap_size": {{partition.heap_size}},
    "mmio_regions": [
{% for mmio in partition.mmio_regions %}
        {
        {% if mmio.size|int %}
            "base": "{{mmio.base}}",
            "size": {{mmio.size}},
        {% else %}
            "name": "{{mmio.base}}",
        {% endif %}
            "permission": "{{mmio.permission|find_permission_key}}"
        {{"}" if loop.last else "},"}}
{% endfor %}
    ],
    "services": [
{% for rot_srv in partition.rot_services %}
        {
            "name": "{{rot_srv.name}}",
            "identifier": "{{rot_srv.id}}",
            "signal": "{{rot_srv.signal}}",
            "minor_version": {{rot_srv.minor_version}},
            "minor_policy": "{{rot_srv.minor_policy}}",
            "non_secure_clients": {{rot_srv.nspe_callable|lower}}
        {{"}" if loop.last else "},"}}
{% endfor %}
    ],
{% if partition.extern_sids %}
    "extern_sids": [
{% for ext_sid in partition.extern_sids %}
        "{{ext_sid}}"{{"" if loop.last else ","}}
{% endfor %}
    ],
{% endif %}
    "source_files": [
{% for src in partition.source_files %}
        "{{src|basename}}"{{"" if loop.last else ","}}
{% endfor %}
    ],
    "irqs": [
{% for irq in partition.irqs %}
        {
            "line_num": {{irq.line_num}},
            "signal": "{{irq.signal}}"
        {{"}" if loop.last else "},"}}
{% endfor %}
    ]
}
'''

exceeding_services = [
    {
        "name": "XSID1",
        "signal": "XSID1",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000009"
    },
    {
        "name": "XSID2",
        "signal": "XSID2",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x0000000a"
    }, {
        "name": "XSID3",
        "signal": "XSID3",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x0000000b"
    }, {
        "name": "XSID4",
        "signal": "XSID4",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x0000000c"
    }, {
        "name": "XSID5",
        "signal": "XSID5",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x0000000d"
    }, {
        "name": "XSID6",
        "signal": "XSID6",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x0000000e"
    }, {
        "name": "XSID7",
        "signal": "XSID7",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x0000000f"
    }, {
        "name": "XSID8",
        "signal": "XSID8",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000010"
    }, {
        "name": "XSID9",
        "signal": "XSID9",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000011"
    }, {
        "name": "XSID10",
        "signal": "XSID10",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000012"
    }, {
        "name": "XSID11",
        "signal": "XSID11",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000013"
    }, {
        "name": "XSID12",
        "signal": "XSID12",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000014"
    }, {
        "name": "XSID13",
        "signal": "XSID13",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000015"
    }, {
        "name": "XSID14",
        "signal": "XSID14",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000016"
    }, {
        "name": "XSID15",
        "signal": "XSID15",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000017"
    }, {
        "name": "XSID16",
        "signal": "XSID16",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000018"
    }, {
        "name": "XSID17",
        "signal": "XSID17",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000019"
    }, {
        "name": "XSID18",
        "signal": "XSID18",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x0000001a"
    }, {
        "name": "XSID19",
        "signal": "XSID19",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x0000001b"
    }, {
        "name": "XSID20",
        "signal": "XSID20",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x0000001c"
    }, {
        "name": "XSID21",
        "signal": "XSID21",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x0000001d"
    }, {
        "name": "XSID22",
        "signal": "XSID22",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x0000001e"
    }, {
        "name": "XSID23",
        "signal": "XSID23",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x0000001f"
    }, {
        "name": "XSID24",
        "signal": "XSID24",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000020"
    }, {
        "name": "XSID25",
        "signal": "XSID25",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000021"
    }, {
        "name": "XSID26",
        "signal": "XSID26",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000022"
    }, {
        "name": "XSID27",
        "signal": "XSID27",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000023"
    }, {
        "name": "XSID28",
        "signal": "XSID28",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000024"
    }, {
        "name": "XSID29",
        "signal": "XSID29",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000025"
    }, {
        "name": "XSID30",
        "signal": "XSID30",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000026"
    }, {
        "name": "XSID31",
        "signal": "XSID31",
        "non_secure_clients": True,
        "minor_version": 5,
        "minor_policy": "RELAXED",
        "identifier": "0x00000027"
    }
]
