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
            {'name': 'PERIPH1', 'permission': 'READ-ONLY'},
            {'name': 'PERIPH2', 'permission': 'READ-ONLY'},
            {'base': '0xCCCCCCCC', 'size': 4096, 'permission': 'READ-ONLY'},
            {'base': '0xDDDDDDDD', 'size': 33554432, 'permission': 'READ-WRITE'}
        ],
        'secure_functions': [
            {'sfid': 'SFID1', 'signal': 'SFID1', 'minor_version': 1, 'minor_policy': 'relaxed', 'non_secure_clients': True},
            {'sfid': 'SFID2', 'signal': 'SFID2', 'minor_version': 2, 'minor_policy': 'strict', 'non_secure_clients': False},
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
            {'name': 'PERIPH1', 'permission': 'READ-ONLY'},
            {'name': 'PERIPH3', 'permission': 'READ-ONLY'},
            {'base': '0xAAAAAAAA', 'size': 4096, 'permission': 'READ-ONLY'},
            {'base': '0xBBBBBBBB', 'size': 33554432, 'permission': 'READ-WRITE'}
        ],
        'secure_functions': [
            {'sfid': 'SFID3', 'signal': 'SFID3', 'minor_version': 5, 'minor_policy': 'relaxed', 'non_secure_clients': True},
            {'sfid': 'SFID4', 'signal': 'SFID4', 'minor_version': 12, 'minor_policy': 'strict', 'non_secure_clients': False},
        ],
        'source_files': ['src3.cpp', 'src4.cpp'],
        'irqs': [
            {"line_num": 22, "signal": "ISR22"},
            {"line_num": 23, "signal": "ISR23"}
        ],
        'extern_sfids': ['SFID1', 'SFID2']
    }
]

invalid_minor_version_policy_secure_functions = [
    {'sfid': 'SFID1', 'signal': 'SFID1', 'minor_version': 1, 'minor_policy': 'invalid_policy', 'non_secure_clients': True}
]

invalid_nspe_callable_secure_functions = [
    {'sfid': 'SFID1', 'signal': 'SFID1', 'minor_version': 1, 'minor_policy': 'strict', 'non_secure_clients': 'invalid_value'}
]

missing_nspe_callable_secure_functions = [
    {'sfid': 'SFID1', 'signal': 'SFID1', 'minor_version': 1, 'minor_policy': 'strict'}
]


duplicate_signal_secure_functions = [
    {'sfid': 'SFID3', 'signal': 'SFID1', 'minor_version': 5, 'minor_policy': 'relaxed', 'non_secure_clients': True},
    {'sfid': 'SFID4', 'signal': 'SFID2', 'minor_version': 12, 'minor_policy': 'strict', 'non_secure_clients': False},
]


missing_minor_version_secure_functions = [
    {'sfid': 'SFID1', 'signal': 'SFID1', 'minor_policy': 'relaxed', 'non_secure_clients': True}
]

missing_minor_version_policy_secure_functions = [
    {'sfid': 'SFID2', 'signal': 'SFID2', 'minor_version': 1, 'non_secure_clients': True}
]

missing_minor_completley_secure_functions = [
    {'sfid': 'SFID2', 'signal': 'SFID2', 'non_secure_clients': False}
]
