import json
import re

import jsonschema.exceptions as jexcep
import os
import pytest

from test_data import *
from tools.spm.generate_partition_code import Manifest, validate_partition_manifests, SecureFunction

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))


def extract_test_name(line):
    return re.search(r'.*\[(.*)\]', line).group(1)


def dump_manifest_to_json(manifest, test_name, test_dir, create_files=True):
    test_file_name = test_dir.join('{}.json'.format(test_name))
    with open(test_file_name.strpath, 'wt') as fh:
        json.dump(manifest, fh, indent=2)
    # Create all the partition source files
    if create_files:
        [test_dir.join(name).write(name) for name in manifest.get('source_files', [])]

    return test_file_name.strpath


@pytest.fixture(scope="session")
def temp_test_data(tmpdir_factory):
    test_dir = tmpdir_factory.mktemp('test_data')
    fname = dump_manifest_to_json(manifests[0], 'valid_partition', test_dir)
    valid_manifest = Manifest.from_json(fname)
    return {'dir': test_dir, 'json': fname, 'manifest': valid_manifest}


modified_json_params = {
    'missing_partition_name': {
        'partition': {k: manifests[0][k] for k in manifests[0] if k != 'name'},
        'assert': jexcep.ValidationError
    },
    'missing_partition_id': {
        'partition': {k: manifests[0][k] for k in manifests[0] if k != 'id'},
        'assert': jexcep.ValidationError
    },
    'missing_partition_priority': {
        'partition': {k: manifests[0][k] for k in manifests[0] if k != 'priority'},
        'assert': jexcep.ValidationError
    },
    'missing_code': {
        'partition': {k: manifests[0][k] for k in manifests[0] if k != 'entry_point'},
        'assert': jexcep.ValidationError
    },
    'missing_stack': {
        'partition': {k: manifests[0][k] for k in manifests[0] if k != 'stack_size'},
        'assert': jexcep.ValidationError
    },
    'missing_heap': {
        'partition': {k: manifests[0][k] for k in manifests[0] if k != 'heap_size'},
        'assert': jexcep.ValidationError
    },
    'missing_src': {
        'partition': {k: manifests[0][k] for k in manifests[0] if k != 'source_files'},
        'assert': jexcep.ValidationError
    },
    'missing_irqs_and_sfids': {
        'partition': {k: manifests[0][k] for k in manifests[0] if k not in ['secure_functions', 'irqs']},
        'assert': AssertionError
    },
    'missing_src_filename': {
        'partition': dict(manifests[0], source_files=[]),
        'assert': jexcep.ValidationError
    },
    'invalid_minor_policy': {
        'partition': dict(manifests[0], secure_functions=invalid_minor_version_policy_secure_functions),
         'assert': jexcep.ValidationError
    },
    'invalid_nspe_callable': {
        'partition': dict(manifests[0], secure_functions=invalid_nspe_callable_secure_functions),
        'assert': jexcep.ValidationError
    },
    'missing_nspe_callable': {
        'partition': dict(manifests[0], secure_functions=missing_nspe_callable_secure_functions),
        'assert': jexcep.ValidationError
    },
    'invalid_stack_size': {
        'partition': dict(manifests[0], stack_size='str'),
        'assert': jexcep.ValidationError
    },
    'invalid_heap_size': {
        'partition': dict(manifests[0], heap_size='str'),
        'assert': jexcep.ValidationError
    },
    'invalid_priority': {
        'partition': dict(manifests[0], priority='invalid_priority'),
        'assert': jexcep.ValidationError
    },
    'invalid_mmioregion_base': {
        'partition': dict(manifests[0], mmio_regions=[{'base': 'str', 'size': 4096, 'permission': 'READ-ONLY'}]),
        'assert': jexcep.ValidationError
    },
    'invalid_mmioregion_size': {
        'partition': dict(manifests[0], mmio_regions=[{'base': '0xEEEEEEEE', 'size': 'str', 'permission': 'READ-ONLY'}]),
        'assert': jexcep.ValidationError
    },
    'invalid_irq_num': {
        'partition': dict(manifests[0], irqs=[{"line_num": "str", "signal": "ISR22"}]),
        'assert': jexcep.ValidationError
    },
    'not_exist_src_filename': {
        'partition': dict(manifests[0], source_files=['missing.cpp']),
        'assert': AssertionError
    },
    'invalid_partition_id_decimal': {
        'partition': dict(manifests[0], id=-1),
        'assert': jexcep.ValidationError
    },
    'invalid_partition_id_hex': {
        'partition': dict(manifests[0], id='0xFFFFFFFF'),
        'assert': jexcep.ValidationError
    },
    'duplicates_extern_sfids': {
        'partition': dict(manifests[0], extern_sfids=['SFID66', 'SFID66']),
        'assert': jexcep.ValidationError
    }
}


@pytest.fixture(params=modified_json_params.values(), ids=modified_json_params.keys())
def modified_json(request, temp_test_data):
    testname = extract_test_name(request.node.name)
    test_file = dump_manifest_to_json(request.param['partition'], testname, temp_test_data['dir'], False)
    return test_file, request.param['assert']


def test_invalid_json(modified_json):
    with pytest.raises(modified_json[1]):
        Manifest.from_json(modified_json[0])


def test_valid_json(temp_test_data):
    manifest = Manifest.from_json(temp_test_data['json'])
    assert manifest == temp_test_data['manifest']


@pytest.mark.parametrize(
    'manifest, assertion',
    [
        pytest.param(
            dict(manifests[1], name=manifests[0]['name']),
            (ValueError, r'Partition name .* is not unique, .*'),
            id='duplicate_partition_name'
        ),
        pytest.param(
            dict(manifests[1], id=manifests[0]['id']),
            (ValueError, r'Partition id .* is not unique, .*'),
            id='duplicate_partition_id'
        ),
        pytest.param(
            dict(manifests[1], secure_functions=manifests[0]['secure_functions']),
            (ValueError, r'Secure function sfid .* is found in both .*'),
            id='duplicate_sf_sfid'
        ),
        pytest.param(
            dict(manifests[1], secure_functions=duplicate_signal_secure_functions),
            (ValueError, r'Secure function signal .* is found in both .*'),
            id='duplicate_sf_signal'
        ),
        pytest.param(
            dict(manifests[1], irqs=manifests[0]['irqs']),
            (ValueError, r'IRQ signal .* is found in both .*'),
            id='duplicate_irqs'
        ),
        pytest.param(
            dict(manifests[1], extern_sfids=['SFID66', 'SFID999']),
            (ValueError, r'External SFID\(s\) .* can\'t be found in any partition manifest.'),
            id='orphan_extern_ids'
        )
    ]
)
def test_validate_partition_manifest(request, temp_test_data, manifest, assertion):
    # Create the json manifest file
    test_name = extract_test_name(request.node.name)
    test_json = dump_manifest_to_json(manifest, test_name, temp_test_data['dir'])
    faulty_manifest = Manifest.from_json(test_json)

    # Test the faulty manifest with a valid one
    with pytest.raises(assertion[0], match=assertion[1]):
        validate_partition_manifests([faulty_manifest, temp_test_data['manifest']])


verify_json_params = {
    'missing_minor_version_secure_functions': {
        'partition': dict(manifests[0], secure_functions=missing_minor_version_secure_functions),
        'field': 'secure_functions',
        'expected': [SecureFunction(sfid='SFID1', signal='SFID1', minor_policy='relaxed', non_secure_clients=True, minor_version=1)]
    },
    'missing_minor_version_policy_secure_functions': {
        'partition': dict(manifests[0], secure_functions=missing_minor_version_policy_secure_functions),
        'field': 'secure_functions',
        'expected': [SecureFunction(sfid='SFID2', signal='SFID2', minor_policy='strict', non_secure_clients=True, minor_version=1)]
    },
    'missing_minor_completley_secure_functions': {
        'partition': dict(manifests[0], secure_functions=missing_minor_completley_secure_functions),
        'field': 'secure_functions',
        'expected': [SecureFunction(sfid='SFID2', signal='SFID2', minor_policy='strict', non_secure_clients=False, minor_version=1)]
    }
}


@pytest.fixture(params=verify_json_params.values(), ids=verify_json_params.keys())
def verify_json(request, tmpdir_factory):
    test_dir = tmpdir_factory.mktemp('test_data')
    test_name = extract_test_name(request.node.name)
    files_list = [
        dump_manifest_to_json(request.param['partition'], '%s1' % test_name, test_dir),
        dump_manifest_to_json(dict(manifests[1], extern_sfids=[]), '%s2' % test_name, test_dir)
    ]

    return files_list[0], files_list[1], request.param['field'], request.param['expected']


def test_verify_json(verify_json):
    manifest1 = Manifest.from_json(verify_json[0])
    manifest2 = Manifest.from_json(verify_json[1])

    validate_partition_manifests([manifest1, manifest2])
    assert getattr(manifest1, verify_json[2]) == verify_json[3]

