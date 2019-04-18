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

import filecmp
import re
import shutil
import tempfile
import pytest
import jsonschema.exceptions as jexcep
from jinja2.defaults import DEFAULT_FILTERS
from tools.psa.mbed_spm_tfm_common import *
from tools.psa.generate_partition_code import *
from .test_data import *


SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))


def extract_test_name(line):
    return re.search(r'.*\[(.*)\]', line).group(1)


def dump_manifest_to_json(manifest, test_name, test_dir, create_files=True):
    """
    Create a JSON manifest file from a dictionary.

    :param manifest: The manifest dictionary.
    :param test_name: Name of the test.
    :param test_dir: Directory to contain the JSON file.
    :param create_files: Whether to create the source files listed in the
           manifest 'source_files' entry.
    :return: Path of the JSON file.
    """
    test_file_name = test_dir.join('{}.json'.format(test_name))
    with open(test_file_name.strpath, 'wt') as fh:
        json.dump(manifest, fh, indent=2)

    # Create all the partition source files
    if create_files:
        [test_dir.join(name).write(name) for name in
         manifest.get('source_files', [])]

    return test_file_name.strpath


def find_priority_key(value):
    """
    Finds the key in 'Manifest.PRIORITY' of a given value.

    :param value: The value.
    :return: The key of the given value.
    """
    return next(
        (key for key, val in Manifest.PRIORITY.items() if val == value),
        None
    )


def find_permission_key(value):
    """
    Finds the key in 'MmioRegion.MMIO_PERMISIONS' of a given value.

    :param value: The value.
    :return: The key of the given value.
    """
    return next(
        (key for key, val in MmioRegion.MMIO_PERMISSIONS.items() if
         val == value),
        None
    )


@pytest.fixture(scope="session")
def temp_test_data(tmpdir_factory):
    """
    Fixture (https://docs.pytest.org/en/latest/fixture.html) function to be
    used by the tests.
    This fixture function Creates a valid JSON manifest file in a temporary
    directory. The scope of this fixture is the entire test session.

    :param tmpdir_factory: Fixture used to create temporary directories.
           see: https://docs.pytest.org/en/latest/tmpdir.html#the-tmpdir-factory-fixture
    :return: A dictionary containing these keys:
             'dir': The temporary directory object created by this fixture.
             'json': The created valid manifest JSON file.
             'manifest': The manifest object read from the JSON file.
    """
    test_dir = tmpdir_factory.mktemp('test_data')
    fname = dump_manifest_to_json(manifests[0], 'valid_partition', test_dir)
    valid_manifest = Manifest.from_json(fname)
    return {'dir': test_dir, 'json': fname, 'manifest': valid_manifest}


"""
'modified_json_params' contain the parameters to be used in the
'modified_json' fixture.
Each key in the dictionary represents a different parameter to be used by
'modified_json', so for each test which uses
the 'modified_json' fixture, the test will run len(modified_json_params) times,
 each time with different parameters.
Each parameter is a dictionary which contains these keys:
  'partition': A modified partition dictionary.
  'assert': The expected assertion which must occur when running with this
  parameter.
"""
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
        'partition': {k: manifests[0][k] for k in manifests[0] if
                      k != 'priority'},
        'assert': jexcep.ValidationError
    },
    'missing_entry_point': {
        'partition': {k: manifests[0][k] for k in manifests[0] if
                      k != 'entry_point'},
        'assert': jexcep.ValidationError
    },
    'missing_stack_size': {
        'partition': {k: manifests[0][k] for k in manifests[0] if
                      k != 'stack_size'},
        'assert': jexcep.ValidationError
    },
    'missing_heap_size': {
        'partition': {k: manifests[0][k] for k in manifests[0] if
                      k != 'heap_size'},
        'assert': jexcep.ValidationError
    },
    'missing_source_files': {
        'partition': {k: manifests[0][k] for k in manifests[0] if
                      k != 'source_files'},
        'assert': jexcep.ValidationError
    },
    'missing_irqs_and_sids': {
        'partition': {k: manifests[0][k] for k in manifests[0] if
                      k not in ['services', 'irqs']},
        'assert': jexcep.ValidationError
    },
    'empty_source_files': {
        'partition': dict(manifests[0], source_files=[]),
        'assert': jexcep.ValidationError
    },
    'invalid_minor_policy': {
        'partition': dict(manifests[0],
                          services=invalid_minor_version_policy_rot_srv),
        'assert': jexcep.ValidationError
    },
    'invalid_nspe_callable': {
        'partition': dict(manifests[0],
                          services=invalid_nspe_callable_rot_srv),
        'assert': jexcep.ValidationError
    },
    'missing_nspe_callable': {
        'partition': dict(manifests[0],
                          services=missing_nspe_callable_rot_srv),
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
        'partition': dict(manifests[0],
                          mmio_regions=[invalid_mmioregion_base]),
        'assert': jexcep.ValidationError
    },
    'invalid_mmioregion_size': {
        'partition': dict(manifests[0],
                          mmio_regions=[invalid_mmioregion_size]),
        'assert': jexcep.ValidationError
    },
    'invalid_irq_num': {
        'partition': dict(manifests[0],
                          irqs=[{"line_num": "str", "signal": "ISR22"}]),
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
    'duplicates_extern_sids': {
        'partition': dict(manifests[0], extern_sids=['SID66', 'SID66']),
        'assert': jexcep.ValidationError
    },
    'exceeding_services': {
        'partition': dict(manifests[1], services=exceeding_services),
        'assert': AssertionError
    }
}


@pytest.fixture(params=modified_json_params.values(),
                ids=modified_json_params.keys())
def modified_json(request, temp_test_data):
    """
    Fixture (https://docs.pytest.org/en/latest/fixture.html) function to be
    used by the tests.
    This fixture function Creates a JSON manifest file from a given partition
    dictionary and save it
    to a temporary directory.
    This fixture uses the 'temp_test_data' fixture.
    This fixture is a parametrized fixture
    (https://docs.pytest.org/en/latest/fixture.html#parametrizing-fixtures).
    The scope of this fixture is a specific test.

    :param request: Request object which contain the current parameter from
           'modified_json_params'.
    :param temp_test_data: The 'temp_test_data' fixture.
    :return: A list containing these values:
             - The created manifest JSON file for the current parameter.
             - The expected assertion for the current parameter.
    """
    testname = extract_test_name(request.node.name)
    test_file = dump_manifest_to_json(request.param['partition'], testname,
                                      temp_test_data['dir'], False)
    return test_file, request.param['assert']


def test_invalid_json(modified_json):
    """
    Test which gets an invalid JSON manifest file (from the
    'modified_json' fixture) and tries to create a
    Manifest object from it.
    The test expects an assertion to happen.

    :param modified_json: The 'modified_json' fixture.
    :return:
    """
    with pytest.raises(modified_json[1]):
        Manifest.from_json(modified_json[0])


def test_valid_json(temp_test_data):
    """
    Test which gets a valid JSON manifest file (from the 'temp_test_data'
    fixture) and tries to create a Manifest object from it.
    The test expects the Manifest to be same as the Manifest created by the
    'temp_test_data' fixture.

    :param temp_test_data: The 'temp_test_data' fixture.
    :return:
    """
    manifest = Manifest.from_json(temp_test_data['json'])
    assert manifest == temp_test_data['manifest']


# Test parametrization decorator
# See https://docs.pytest.org/en/latest/parametrize.html#pytest-mark-parametrize-parametrizing-test-functions
# Contain the parameters to be used in the 'test_validate_partition_manifest'
# test. It defines a list of (manifest, assertion) tuples which each entry
# will be the input of the 'test_validate_partition_manifest' test, the test
# will run len(LIST_OF_TUPPLES) times, each time with different (manifest,
# assertion) tuple.
# The tuple fields are:
#   'manifest': A modified partition dictionary.
#   'assertion': A tuple containing the expected assertion and assertion
#                string which must occur when running with this parameter.
@pytest.mark.parametrize(
    'manifests, assertion',
    [
        pytest.param(
            [manifests[0], dict(manifests[1], name=manifests[0]['name'])],
            (ValueError, r'Partition name .* is not unique, .*'),
            id='duplicate_partition_name'
        ),
        pytest.param(
            [manifests[0], dict(manifests[1], id=manifests[0]['id'])],
            (ValueError, r'Partition id .* is not unique, .*'),
            id='duplicate_partition_id'
        ),
        pytest.param(
            [manifests[0], dict(manifests[1], services=manifests[0]['services'])],
            (ValueError, r'Root of Trust Service name .* is found in both .*'),
            id='duplicate_rot_srv_name'
        ),
        pytest.param(
            [manifests[0], dict(manifests[1], services=duplicate_signal_rot_services)],
            (ValueError, r'Root of Trust Service signal .* is found in both .*'),
            id='duplicate_rot_srv_signal'
        ),
        pytest.param(
            [manifests[0], dict(manifests[1], services=duplicate_identifier_rot_services)],
            (ValueError, r'Root of Trust Service identifier .* is found in both .*'),
            id='duplicate_rot_srv_identifier'
        ),
        pytest.param(
            [manifests[0], dict(manifests[1], irqs=duplicate_signal_irqs)],
            (ValueError, r'IRQ signal .* is found in both .*'),
            id='duplicate_irq_signal'
        ),
        pytest.param(
            [manifests[0], dict(manifests[1], irqs=duplicate_line_num_irqs)],
            (ValueError, r'IRQ line number .* is found in both .*'),
            id='duplicate_irq_line_num'
        ),
        pytest.param(
            [manifests[0], dict(manifests[1], extern_sids=['SID66', 'SID999'])],
            (ValueError, r'External SID\(s\) .* can\'t be found in any partition manifest.'),
            id='orphan_extern_ids'
        ),
        pytest.param(
            [manifests[0], dict(manifests[1], extern_sids=[manifests[0]['services'][0]['name']])],
            (ValueError, r'Detected a circular call dependency between the partitions.'),
            id='circular_call_dependency'
        ),
        pytest.param(
            [{k: manifests[0][k] for k in manifests[0] if k != 'extern_sids'},
             dict({k: manifests[1][k] for k in manifests[1] if k != 'services'
                   and k != 'irqs'}, services=spe_contained_rot_services)],
            (ValueError, r'Partition .* is not accessible from NSPE '
                         'and not referenced by any other partition.'),
            id='dead_partition'
        )
    ]
)
def test_validate_partition_manifest(request, temp_test_data, manifests, assertion):
    """
    Test which creates an invalid manifest object (after passing JSON schema
    validation) and call
    validate_partition_manifests() with it and with a valid manifest object.
    The test expects an assertion to happen.

    :param request: Request object.
    :param temp_test_data: The 'temp_test_data' fixture.
    :param manifest: The manifest value from the (manifest, assertion) tuple
           for the current parameter.
    :param assertion: The assertion value from the (manifest, assertion) tuple
           for the current parameter.
    :return:
    """
    test_name = extract_test_name(request.node.name)
    jsons = [dump_manifest_to_json(m, '%s_%d' % (test_name, i), temp_test_data['dir']) for i, m in enumerate(manifests)]
    created_manifests, _ = parse_manifests(jsons)

    with pytest.raises(assertion[0], match=assertion[1]):
        validate_partition_manifests(created_manifests)


"""
'verify_json_params' contain the parameters to be used in the 'verify_json'
fixture. Each key in the dictionary represents a different parameter to be used
by 'verify_json', so for each test which uses the 'verify_json' fixture, the
test will run len(verify_json_params) times, each time with different
parameters.
Each parameter is a dictionary which contains these keys:
  'partition': A modified partition dictionary.
  'field': The modified field name.
  'expected': The expected field object.
"""
verify_json_params = {
    'missing_minor_version_rot_services': {
        'partition': dict(manifests[0],
                          services=missing_minor_version_rot_srv),
        'field': 'rot_services',
        'expected': [
            RotService(
                name='SID1', identifier='0x00000001',signal='SID1',
                minor_policy='RELAXED', non_secure_clients=True, minor_version=1
            )
        ]
    },
    'missing_minor_version_policy_rot_services': {
        'partition': dict(manifests[0],
                          services=missing_minor_version_policy_rot_srv),
        'field': 'rot_services',
        'expected': [
            RotService(
                name='SID2', identifier='0x00000002', signal='SID2',
                minor_policy='STRICT', non_secure_clients=True, minor_version=1
            )
        ]
    },
    'missing_minor_completley_rot_services': {
        'partition': dict(manifests[0],
                          services=missing_minor_completley_rot_srv),
        'field': 'rot_services',
        'expected': [
            RotService(
                name='SID2', identifier='0x00000002', signal='SID2',
                minor_policy='STRICT', non_secure_clients=True, minor_version=1
            )
        ]
    }
}


@pytest.fixture(params=verify_json_params.values(),
                ids=verify_json_params.keys())
def verify_json(request, tmpdir_factory):
    """
    Fixture (https://docs.pytest.org/en/latest/fixture.html) function to be
    used by the tests.
    This fixture function Creates 2 JSON manifest files (The 1st from
    'verify_json_params', the 2nd from manifests[1]) and saves them to a
    temporary directory. This fixture is a parametrized fixture
    (https://docs.pytest.org/en/latest/fixture.html#parametrizing-fixtures).
    The scope of this fixture is a specific test.

    :param request: Request object which contain the current parameter from
           'verify_json_params'.
    :param tmpdir_factory: The 'tmpdir_factory' fixture.
    :return: A dictionary containing these keys:
             'files_list': A list of the created manifest JSON files.
             'field': The changed field in the 1st manifest.
              'expected': The expected 'field' object.
    """
    test_dir = tmpdir_factory.mktemp('test_data')
    test_name = extract_test_name(request.node.name)
    files_list = [
        dump_manifest_to_json(request.param['partition'], '%s1' % test_name,
                              test_dir),
        dump_manifest_to_json(dict(manifests[1], extern_sids=[]),
                              '%s2' % test_name, test_dir)
    ]
    return {'files_list': files_list, 'field': request.param['field'],
            'expected': request.param['expected']}


def test_verify_json(verify_json):
    """
    Test which gets 2 JSON manifest files (from the 'verify_json' fixture),
    create Manifest objects from them, call validate_partition_manifests() on
    the manifest objects and check that the 1st Manifest object is as expected.

    :param verify_json: The 'verify_json' fixture.
    :return:
    """
    test_manifests, _ = parse_manifests(verify_json['files_list'])
    validate_partition_manifests(test_manifests)
    assert getattr(test_manifests[0], verify_json['field']) == verify_json['expected']


@pytest.fixture(scope="function")
def test_template_setup(tmpdir_factory):
    """
    Fixture (https://docs.pytest.org/en/latest/fixture.html) function to be
    used by the tests. This fixture function Creates JSON manifest files,
    Manifest objects from 'manifest' and template files in a temporary
    directory. The scope of this fixture is the entire test session.

    :param tmpdir_factory: Fixture used to create temporary directories.
           see: https://docs.pytest.org/en/latest/tmpdir.html#the-tmpdir-factory-fixture
    :return: A dictionary containing these keys:
             'dir': The temporary directory object created by this fixture.
             'template_files': List of the created template files.
             'manifest_files': List of the created manifest JSON files.
             'manifests': List of the created Manifest objects.
             'filters': Dictionary with additional filters for
             generate_source_files()
    """

    def find_priority_key(value):
        """
        Finds the key in 'Manifest.PRIORITY' of a given value.

        :param value: The value.
        :return: The key of the given value.
        """
        return next(
            (key for key, val in Manifest.PRIORITY.items() if val == value),
            None)

    def find_permission_key(value):
        """
        Finds the key in 'MmioRegion.MMIO_PERMISIONS' of a given value.

        :param value: The value.
        :return: The key of the given value.
        """
        return next((key for key, val in MmioRegion.MMIO_PERMISSIONS.items() if
                     val == value), None)

    test_dir = tmpdir_factory.mktemp('test_data')
    manifest_files = [
        dump_manifest_to_json(manifest, manifest['name'], test_dir) for
        manifest in manifests]
    manifest_objects, regions = parse_manifests(manifest_files)
    filters = {
        'basename': os.path.basename,
        'find_priority_key': find_priority_key,
        'find_permission_key': find_permission_key
    }
    template_files = [test_dir.join('_NAME_data.json.tpl'),
                      test_dir.join('common.json.tpl')]
    for template, _file in [(test_partition_template, template_files[0]),
                            (test_common_template, template_files[1])]:
        _file.write(template)
    template_files = [_file.strpath for _file in template_files]

    expected_common_files = [test_dir.join('common.json')]
    for output, _file in [(test_common_expected, expected_common_files[0])]:
        _file.write(output)
    expected_common_files = [_file.strpath for _file in expected_common_files]

    return {
        'dir': test_dir.strpath,
        'template_files': template_files,
        'manifest_files': manifest_files,
        'common_files': expected_common_files,
        'manifests': manifest_objects,
        'region_list': regions,
        'filters': filters
    }


def test_generate_source_files(test_template_setup):
    """
    Test which calls generate_source_files() with the data from
    'test_template_setup' fixture and checks normal output.

    :param test_template_setup: The 'test_template_setup' fixture.
    :return:
    """

    before_file_list = set(os.listdir(test_template_setup['dir']))
    partition_templates = filter(lambda filename: '_NAME_' in filename, test_template_setup['template_files'])
    common_templates = filter(lambda filename: '_NAME_' not in filename, test_template_setup['template_files'])
    common_templates = {
        t: path_join(test_template_setup['dir'], os.path.basename(os.path.splitext(t)[0])) for t in common_templates
    }

    region_pair_list = list(itertools.combinations(test_template_setup['region_list'], 2))
    for manifest in test_template_setup['manifests']:
        generate_source_files(
            templates=manifest.templates_to_files(partition_templates, test_template_setup['dir'], test_template_setup['dir']),
            render_args={
                'partition': manifest,
                'dependent_partitions': manifest.find_dependencies(test_template_setup['manifests'])
            },
            extra_filters=test_template_setup['filters']
        )

    generate_source_files(
        common_templates,
        render_args={
            'partitions': test_template_setup['manifests'],
            'region_pair_list': region_pair_list
        },
        extra_filters=test_template_setup['filters']
    )

    after_file_list = set(os.listdir(test_template_setup['dir']))
    generated_files = list(after_file_list.difference(before_file_list))

    for gen_file in [os.path.join(test_template_setup['dir'], f) for f in generated_files]:
        """
        For each generated json file in 'autogen_dir':
          1. Load the json file to a dictionary named 'generated'.
          2. If it was generated from a partition template ('generated' has a 'name' key):
               a) Read the original manifest json from the test temp dir.
               b) Load the manifest json file to a dictionary named 'expected'.
             Else (generated from a common template):
               a) Calculate 'region_list'.
               b) Build the 'expected' dictionary with values from the original manifest objects.
          3. Compare 'generated' with 'expected'.
        """
        with open(gen_file) as fh:
            generated = json.load(fh)

        if 'name' in generated:
            input_file = os.path.join(test_template_setup['dir'],
                                      generated['name'] + '.json')
            assert os.path.isfile(input_file)
            assert input_file in test_template_setup['manifest_files']
            with open(input_file) as fh:
                expected = json.load(fh)
        else:
            expected = {
                'num_of_partitions': len(test_template_setup['manifests']),
                'partition_names': [manifest.name for manifest in
                                    test_template_setup['manifests']],
                'num_of_region_pairs': len(region_pair_list)
            }
        assert generated == expected


circular_call_dependency_params = {
    'no manifests': {
        'manifests': [],
        'result': False
    },
    'one manifest': {
        'manifests': ['PARTITION1'],
        'result': False
    },
    '2 manifests with dependency': {
        'manifests': ['PARTITION1', 'PARTITION2'],
        'result': True
    },
    '2 manifests without dependency': {
        'manifests': ['PARTITION1', 'PARTITION3'],
        'result': False
    },
    '5 manifests with dependency': {
        'manifests': ['PARTITION1', 'PARTITION3', 'PARTITION4', 'PARTITION5', 'PARTITION6'],
        'result': True
    },
    '5 manifests without dependency': {
        'manifests': ['PARTITION1', 'PARTITION3', 'PARTITION4', 'PARTITION6', 'PARTITION7'],
        'result': False
    }
}


@pytest.fixture(params=circular_call_dependency_params.values(),
                ids=circular_call_dependency_params.keys())
def circular_dependencies(request, tmpdir_factory):
    """
    Fixture (https://docs.pytest.org/en/latest/fixture.html) function to be
    used by the tests.
    This fixture function Creates a JSON manifest file from a given partition
    dictionary and save it
    to a temporary directory.
    This fixture uses the 'temp_test_data' fixture.
    This fixture is a parametrized fixture
    (https://docs.pytest.org/en/latest/fixture.html#parametrizing-fixtures).
    The scope of this fixture is a specific test.

    :param request: Request object which contain the current parameter from
           'circular_call_dependency_params'.
    :param temp_test_data: The 'temp_test_data' fixture.
    :return: A Dictionary containing these values:
             - files - list of manifest filesgenerated
             - The expected result from check_circular_call_dependencies
    """
    test_dir = tmpdir_factory.mktemp('test_data')

    test_manifests = filter(lambda x: x['name'] in request.param['manifests'],
                            manifests_for_circular_call_dependency_checks)
    manifest_files = [
        dump_manifest_to_json(manifest, manifest['name'], test_dir) for
        manifest in test_manifests]

    return {'files': manifest_files, 'result': request.param['result']}


def test_check_circular_call_dependencies(circular_dependencies):
    """
    Test detection of circular call dependencies between the partitions.
    The test performs the circular call dependency check in a few
    predefined partition topologies and compares the result with the expected value.

    :param circular_dependencies: the 'circular_dependencies' fixture
    :return:
    """
    objects, _ = parse_manifests(circular_dependencies['files'])
    assert check_circular_call_dependencies(objects) == circular_dependencies[
        'result']
