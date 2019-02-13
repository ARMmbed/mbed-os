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

from tools.psa.mbed_spm_tfm_common import Manifest, validate_partition_manifests

__version__ = '1.0'
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
MBED_OS_ROOT = os.path.abspath(path_join(SCRIPT_DIR, os.pardir, os.pardir))
TEMPLATES_LIST_FILE = path_join(SCRIPT_DIR, 'tfm', 'tfm_generated_file_list.json')
SERVICES_DIR = os.path.join(MBED_OS_ROOT, "components", "TARGET_PSA", "services")

SERVICES_MANIFESTS = [
    path_join(SERVICES_DIR, 'psa_prot_internal_storage', 'pits_psa.json'),
    path_join(SERVICES_DIR, 'platform', 'platform_psa.json'),
    path_join(SERVICES_DIR, 'crypto', 'crypto_partition_psa.json')
]


def generate_partition_source_files(manifest_files, extra_filters=None):
    """
    Process all the given manifest files and generate C code from them.

    :param manifest_files: List of manifest files
    :param extra_filters: Dictionary of extra filters to use in the rendering
           process
    :return: path to the setup generated files
    """

    # Construct lists of all the manifests and mmio_regions.
    region_list = []
    manifests = []
    for manifest_file in manifest_files:
        manifest_obj = Manifest.from_json(manifest_file, psa_type='TFM')
        manifests.append(manifest_obj)
        for region in manifest_obj.mmio_regions:
            region_list.append(region)

    # Validate the correctness of the manifest collection.
    validate_partition_manifests(manifests)

    render_args = {
        'partitions': manifests,
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
    generate_partition_source_files(SERVICES_MANIFESTS)


if __name__ == '__main__':
    generate_tfm_code()
