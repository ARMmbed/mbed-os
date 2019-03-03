#!/usr/bin/python
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

import json
import os
import sys
from os.path import join as path_join
from jinja2 import Environment, FileSystemLoader, StrictUndefined

# Be sure that the tools directory is in the search path
ROOT = os.path.abspath(path_join(os.path.dirname(__file__), os.pardir, os.pardir))
sys.path.insert(0, ROOT)

from tools.psa.mbed_spm_tfm_common import \
    Manifest, validate_partition_manifests, manifests_discovery, MBED_OS_ROOT, SERVICES_DIR, TESTS_DIR

__version__ = '1.0'
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TEMPLATES_LIST_FILE = path_join(SCRIPT_DIR, 'tfm', 'tfm_generated_file_list.json')

SERVICES_MANIFESTS = [
    path_join(SERVICES_DIR, 'storage', 'its', 'pits_psa.json'),
    path_join(SERVICES_DIR, 'platform', 'platform_psa.json'),
    path_join(SERVICES_DIR, 'crypto', 'crypto_partition_psa.json'),
    path_join(SERVICES_DIR, 'attestation', 'attestation_partition_psa.json')
]


def parse_manifests(manifests_files):
    region_list = []
    manifests = []
    for manifest_file in manifests_files:
        manifest_obj = Manifest.from_json(manifest_file, psa_type='TFM')
        manifests.append(manifest_obj)
        for region in manifest_obj.mmio_regions:
            region_list.append(region)

    return manifests, region_list


def generate_partition_source_files(service_manifest_files, test_manifest_files, extra_filters=None):
    """
    Process all the given manifest files and generate C code from them.

    :param service_manifest_files: List of PSA serices manifest files
    :param test_manifest_files: List of tests manifest files
    :param extra_filters: Dictionary of extra filters to use in the rendering
           process
    :return: path to the setup generated files
    """

    # Construct lists of all the manifests and mmio_regions.
    service_manifests, service_region_list = parse_manifests(service_manifest_files)
    test_manifests, test_region_list = parse_manifests(test_manifest_files)

    # Validate the correctness of the manifest collection.
    validate_partition_manifests(service_manifests + test_manifests)

    render_args = {
        'service_partitions': service_manifests,
        'test_partitions': test_manifests
    }

    # Load templates for the code generation.
    with open(TEMPLATES_LIST_FILE, 'r') as fh:
        templates_data = json.load(fh)

    env = Environment(
        loader=FileSystemLoader(MBED_OS_ROOT),
        lstrip_blocks=True,
        trim_blocks=True,
        undefined=StrictUndefined
    )

    if extra_filters:
        env.filters.update(extra_filters)

    # Generate code for each template
    for tpl in templates_data:
        template = env.get_template(tpl['template'])
        data = template.render(**render_args)
        output_path = os.path.join(MBED_OS_ROOT, tpl['output'])
        output_folder = os.path.dirname(output_path)

        if not os.path.exists(output_folder):
            os.makedirs(output_folder)

        with open(output_path, 'wt') as fh:
            fh.write(data)


def generate_tfm_code():
    _, tests_manifests = manifests_discovery(TESTS_DIR)
    generate_partition_source_files(SERVICES_MANIFESTS, tests_manifests)


if __name__ == '__main__':
    generate_tfm_code()
