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
                'sfid': 'SFID1',
                'signal': 'SFID1',
                'minor_version': 1,
                'minor_policy': 'relaxed',
                'non_secure_clients': True
            },
            {
                'sfid': 'SFID2',
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
                'sfid': 'SFID3',
                'signal': 'SFID3',
                'minor_version': 5,
                'minor_policy': 'relaxed',
                'non_secure_clients': True
            },
            {
                'sfid': 'SFID4',
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
        ],
        'extern_sfids': ['SFID1', 'SFID2']
    }
]

invalid_minor_version_policy_sf = [
    {
        'sfid': 'SFID1',
        'signal': 'SFID1',
        'minor_version': 1,
        'minor_policy': 'invalid_policy',
        'non_secure_clients': True
    }
]

invalid_nspe_callable_sf = [
    {
        'sfid': 'SFID1',
        'signal': 'SFID1',
        'minor_version': 1,
        'minor_policy': 'strict',
        'non_secure_clients': 'invalid_value'
    }
]

missing_nspe_callable_sf = [
    {
        'sfid': 'SFID1',
        'signal': 'SFID1',
        'minor_version': 1,
        'minor_policy': 'strict'
    }
]

duplicate_signal_secure_functions = [
    {
        'sfid': 'SFID3',
        'signal': 'SFID1',
        'minor_version': 5,
        'minor_policy': 'relaxed',
        'non_secure_clients': True
    },
    {
        'sfid': 'SFID4',
        'signal': 'SFID2',
        'minor_version': 12,
        'minor_policy': 'strict',
        'non_secure_clients': False
    },
]

missing_minor_version_sf = [
    {
        'sfid': 'SFID1',
        'signal': 'SFID1',
        'minor_policy': 'relaxed',
        'non_secure_clients': True
    }
]

missing_minor_version_policy_sf = [
    {
        'sfid': 'SFID2',
        'signal': 'SFID2',
        'minor_version': 1,
        'non_secure_clients': True
    }
]

missing_minor_completley_sf = [
    {'sfid': 'SFID2', 'signal': 'SFID2', 'non_secure_clients': False}
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
            "sfid": "{{sf.sfid}}",
            "signal": "{{sf.signal}}",
            "minor_version": {{sf.minor_version}},
            "minor_policy": "{{sf.minor_policy}}",
            "non_secure_clients": {{sf.nspe_callable|lower}}
        {{"}" if loop.last else "},"}}
{% endfor %}
    ],
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
    ],
    "extern_sfids": [
{% for ext_sfid in partition.extern_sfids %}
        "{{ext_sfid}}"{{"" if loop.last else ","}}
{% endfor %}
    ]
}
'''
