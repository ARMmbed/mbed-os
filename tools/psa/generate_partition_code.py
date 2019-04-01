#!/usr/bin/python
# Copyright (c) 2017-2019 ARM Limited
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

import itertools
import json
import os
import sys
from os.path import join as path_join

# Be sure that the tools directory is in the search path
ROOT = os.path.abspath(path_join(os.path.dirname(__file__), os.pardir, os.pardir))
sys.path.insert(0, ROOT)

from tools.psa.mbed_spm_tfm_common import validate_partition_manifests, \
    manifests_discovery, parse_manifests, generate_source_files, \
    MBED_OS_ROOT, SERVICES_DIR, TESTS_DIR

__version__ = '1.0'
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
MANIFEST_FILE_PATTERN = '*_psa.json'
PSA_CORE_ROOT = path_join(MBED_OS_ROOT, 'components', 'TARGET_PSA')
TFM_TEMPLATES_DESC = path_join(SCRIPT_DIR, 'tfm', 'tfm_generated_file_list.json')
MBED_SPM_TEMPLATES_DESC = path_join(SCRIPT_DIR, 'mbed_spm', 'mbed_spm_generated_file_list.json')
MBED_SPM_TEMPLATES_DIR = path_join(SCRIPT_DIR, 'mbed_spm', 'templates')


def generate_partitions_sources(manifest_files, extra_filters=None):
    """
    Process all the given manifest files and generate C code from them

    :param manifest_files: List of manifest files
    :param extra_filters: Dictionary of extra filters to use in the rendering
           process
    :return: List of paths to the generated files
    """

    # Construct a list of all the manifests and sids.
    manifests, _ = parse_manifests(manifest_files, 'MBED_SPM')

    with open(MBED_SPM_TEMPLATES_DESC, 'r') as fh:
        template_data = json.load(fh)
        manifest_template_list = [path_join(MBED_OS_ROOT, t['template'])
                                  for t in template_data['partition']]

    generated_folders = set()
    for manifest in manifests:
        manifest_output_folder = manifest.autogen_folder
        render_args = {
            'partition': manifest,
            'dependent_partitions': manifest.find_dependencies(manifests),
            'script_ver': __version__
        }

        manifest_output_folder = generate_source_files(
            manifest.templates_to_files(manifest_template_list,
                                        MBED_SPM_TEMPLATES_DIR,
                                        manifest_output_folder),
            render_args,
            manifest_output_folder,
            extra_filters=extra_filters
        )

        generated_folders.add(manifest_output_folder)

    return list(generated_folders)


def generate_psa_setup(manifest_files, output_dir, weak_setup, extra_filters=None):
    """
Process all the given manifest files and generate C setup code from them
    :param manifest_files: List of manifest files
    :param output_dir: Output directory for the generated files
    :param weak_setup: Is the functions/data in the setup file weak
            (can be overridden by another setup file)
    :param extra_filters: Dictionary of extra filters to use in the rendering
           process
    :return: path to the setup generated files
    """
    with open(MBED_SPM_TEMPLATES_DESC, 'r') as fh:
        template_data = json.load(fh)
        templates_dict = {
            path_join(MBED_OS_ROOT, t['template']):
                path_join(output_dir, t['target'])
            for t in template_data['common']
        }

    # Construct lists of all the manifests and mmio_regions.
    manifests, region_list = parse_manifests(manifest_files, 'MBED_SPM')

    # Validate the correctness of the manifest collection.
    validate_partition_manifests(manifests)

    render_args = {
        'partitions': manifests,
        'regions': region_list,
        'region_pair_list': list(itertools.combinations(region_list, 2)),
        'weak': weak_setup,
        'script_ver': __version__
    }

    return generate_source_files(
        templates_dict,
        render_args,
        output_dir,
        extra_filters=extra_filters
    )


def generate_psa_code(service_files, test_files):
    # Generate partition code for each manifest file
    generate_partitions_sources(service_files + test_files)

    # Generate default system psa setup file (only system partitions)
    generate_psa_setup(service_files, PSA_CORE_ROOT, weak_setup=True)

    tests_dict = {}
    for test_manifest in test_files:
        test_dir = os.path.dirname(test_manifest)
        if test_dir not in tests_dict:
            tests_dict[test_dir] = [test_manifest]
        else:
            tests_dict[test_dir].append(test_manifest)

    for test_dir in tests_dict:
        generate_psa_setup(service_files + tests_dict[test_dir],
                           test_dir, weak_setup=False)


def generate_tfm_code(service_files, test_files):
    # Construct lists of all the manifests and mmio_regions.
    service_manifests, service_region_list = parse_manifests(
        service_files, 'TFM')
    test_manifests, test_region_list = parse_manifests(
        test_files, 'TFM')

    # Validate the correctness of the manifest collection.
    validate_partition_manifests(service_manifests + test_manifests)

    render_args = {
        'service_partitions': service_manifests,
        'test_partitions': test_manifests
    }

    with open(TFM_TEMPLATES_DESC, 'r') as fh:
        templates_data = json.load(fh)
        templates_dict = {
            path_join(MBED_OS_ROOT, t['template']):
                path_join(MBED_OS_ROOT, t['output']) for t in templates_data
        }

        generate_source_files(templates_dict, render_args, MBED_OS_ROOT)


def main():
    services, _ = manifests_discovery(root_dir=SERVICES_DIR)
    _, tests = manifests_discovery(root_dir=TESTS_DIR)
    generate_psa_code(services, tests)
    generate_tfm_code(services, tests)


if __name__ == '__main__':
    main()
