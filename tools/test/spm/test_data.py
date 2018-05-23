manifests = [
    {
        'name': 'TEST_PARTITION',
        'id': "0x7FFFFFFF",
        "type": "SECURE",
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
        'secure_functions': [
            {
                'name': 'SFID1',
                'identifier': '0x00000001',
                'signal': 'SFID1',
                'minor_version': 1,
                'minor_policy': 'relaxed',
                'non_secure_clients': True
            },
            {
                'name': 'SFID2',
                'identifier': '0x00000002',
                'signal': 'SFID2',
                'minor_version': 2,
                'minor_policy': 'strict',
                'non_secure_clients': False
            },
        ],
        'source_files': ['src1.cpp', 'src2.cpp'],
        'irqs': [
            {"line_num": 20, "signal": "ISR20"},
            {"line_num": 21, "signal": "ISR21"}
        ],
        'extern_sfids': ['SFID3', 'SFID4']
    },
    {
        'name': 'TEST_PARTITION2',
        'id': "0x7FFFFFFE",
        "type": "SECURE",
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
        'secure_functions': [
            {
                'name': 'SFID3',
                'identifier': '0x00000003',
                'signal': 'SFID3',
                'minor_version': 5,
                'minor_policy': 'relaxed',
                'non_secure_clients': True
            },
            {
                'name': 'SFID4',
                'identifier': '0x00000004',
                'signal': 'SFID4',
                'minor_version': 12,
                'minor_policy': 'strict',
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
        'type': 'SECURE',
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,
        'heap_size': 2048,
        'source_files': ['src1.cpp'],
        'secure_functions': [
            {
                'name': 'SFID1',
                'identifier': '0x00000001',
                'signal': 'SFID1',
                'non_secure_clients': False
            },
            {
                'name': 'SFID2',
                'identifier': '0x00000002',
                'signal': 'SFID2',
                'non_secure_clients': False
            }
        ],
        'extern_sfids': ['SFID3', 'SFID4']
    },
    {
        'name': 'PARTITION2',
        'id': '0x7FFFFFFE',
        'type': 'SECURE',
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,
        'heap_size': 2048,
        'source_files': ['src2.cpp'],
        'secure_functions': [
            {
                'name': 'SFID3',
                'identifier': '0x00000003',
                'signal': 'SFID3',
                'non_secure_clients': False
            },
            {
                'name': 'SFID4',
                'identifier': '0x00000004',
                'signal': 'SFID4',
                'non_secure_clients': False
            }
        ],
        'extern_sfids': ['SFID1', 'SFID2']
    },
    {
        'name': 'PARTITION3',
        'id': '0x7FFFFFFD',
        'type': 'SECURE',
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,
        'heap_size': 2048,
        'source_files': ['src3.cpp'],
        'secure_functions': [
            {
                'name': 'SFID5',
                'identifier': '0x00000005',
                'signal': 'SFID5',
                'non_secure_clients': False
            }
        ],
        'extern_sfids': ['SFID7']
    },
    {
        'name': 'PARTITION4',
        'id': '0x7FFFFFFC',
        'type': 'SECURE',
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,
        'heap_size': 2048,
        'source_files': ['src4.cpp'],
        'secure_functions': [
            {
                'name': 'SFID6',
                'identifier': '0x00000006',
                'signal': 'SFID6',
                'non_secure_clients': False
            },
            {
                'name': 'SFID7',
                'identifier': '0x00000007',
                'signal': 'SFID7',
                'non_secure_clients': False
            },
        ],
        'extern_sfids': ['SFID9']
    },
    {
        'name': 'PARTITION5',
        'id': '0x7FFFFFFB',
        'type': 'SECURE',
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,
        'heap_size': 2048,
        'source_files': ['src5.cpp'],
        'secure_functions': [
            {
                'name': 'SFID8',
                'identifier': '0x00000008',
                'signal': 'SFID8',
                'non_secure_clients': False
            },
            {
                'name': 'SFID9',
                'identifier': '0x00000009',
                'signal': 'SFID9',
                'non_secure_clients': False
            }
        ],
        'extern_sfids': ['SFID5']
    },
    {
        'name': 'PARTITION6',
        'id': '0x7FFFFFFA',
        'type': 'SECURE',
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,
        'heap_size': 2048,
        'source_files': ['src6.cpp'],
        'secure_functions': [
            {
                'name': 'SFID10',
                'identifier': '0x0000000A',
                'signal': 'SFID10',
                'non_secure_clients': False
            },
            {
                'name': 'SFID11',
                'identifier': '0x0000000B',
                'signal': 'SFID11',
                'non_secure_clients': False
            }
        ],
        'extern_sfids': ['SFID7', 'SFID5']
    },
    {
        'name': 'PARTITION7',
        'id': '0x7FFFFFF9',
        'type': 'SECURE',
        'priority': 'NORMAL',
        'entry_point': 'test_main',
        'stack_size': 512,
        'heap_size': 2048,
        'source_files': ['src6.cpp'],
        'secure_functions': [
            {
                'name': 'SFID12',
                'identifier': '0x0000000C',
                'signal': 'SFID12',
                'non_secure_clients': False
            }
        ]
    }
]



invalid_minor_version_policy_sf = [
    {
        'name': 'SFID1',
        'identifier': '0x00000001',
        'signal': 'SFID1',
        'minor_version': 1,
        'minor_policy': 'invalid_policy',
        'non_secure_clients': True
    }
]

invalid_nspe_callable_sf = [
    {
        'name': 'SFID1',
        'identifier': '0x00000001',
        'signal': 'SFID1',
        'minor_version': 1,
        'minor_policy': 'strict',
        'non_secure_clients': 'invalid_value'
    }
]

missing_nspe_callable_sf = [
    {
        'name': 'SFID1',
        'identifier': '0x00000001',
        'signal': 'SFID1',
        'minor_version': 1,
        'minor_policy': 'strict'
    }
]

duplicate_signal_secure_functions = [
    {
        'name': 'SFID3',
        'identifier': '0x00000001',
        'signal': 'SFID1',
        'minor_version': 5,
        'minor_policy': 'relaxed',
        'non_secure_clients': True
    },
    {
        'name': 'SFID4',
        'identifier': '0x00000002',
        'signal': 'SFID2',
        'minor_version': 12,
        'minor_policy': 'strict',
        'non_secure_clients': False
    },
]

duplicate_identifier_secure_functions = [
    {
        'name': 'SFID3',
        'identifier': '0x00000003',
        'signal': 'SFID3',
        'minor_version': 5,
        'minor_policy': 'relaxed',
        'non_secure_clients': True
    },
    {
        'name': 'SFID4',
        'identifier': '0x00000002',
        'signal': 'SFID4',
        'minor_version': 12,
        'minor_policy': 'strict',
        'non_secure_clients': False
    },
]

missing_minor_version_sf = [
    {
        'name': 'SFID1',
        'identifier': '0x00000001',
        'signal': 'SFID1',
        'minor_policy': 'relaxed',
        'non_secure_clients': True
    }
]

missing_minor_version_policy_sf = [
    {
        'name': 'SFID2',
        'identifier': '0x00000002',
        'signal': 'SFID2',
        'minor_version': 1,
        'non_secure_clients': True
    }
]

missing_minor_completley_sf = [
    {'name': 'SFID2', 'identifier': '0x00000002', 'signal': 'SFID2', 'non_secure_clients': False}
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
    "priority": "{{partition.priority|find_priority_key}}",
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
    "secure_functions": [
{% for sf in partition.secure_functions %}
        {
            "name": "{{sf.name}}",
            "identifier": "{{sf.id}}",
            "signal": "{{sf.signal}}",
            "minor_version": {{sf.minor_version}},
            "minor_policy": "{{sf.minor_policy}}",
            "non_secure_clients": {{sf.nspe_callable|lower}}
        {{"}" if loop.last else "},"}}
{% endfor %}
    ],
{% if partition.extern_sfids %}
    "extern_sfids": [
{% for ext_sfid in partition.extern_sfids %}
        "{{ext_sfid}}"{{"" if loop.last else ","}}
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
