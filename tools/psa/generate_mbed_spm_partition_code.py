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

import itertools
import os
import sys
from os.path import join as path_join
from jinja2 import Environment, FileSystemLoader, StrictUndefined

# Be sure that the tools directory is in the search path
ROOT = os.path.abspath(path_join(os.path.dirname(__file__), os.pardir, os.pardir))
sys.path.insert(0, ROOT)

from tools.psa.mbed_spm_tfm_common import \
    Manifest, validate_partition_manifests, manifests_discovery, MBED_OS_ROOT

__version__ = '1.0'
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TEMPLATES_DIR = path_join(SCRIPT_DIR, 'mbed_spm', 'templates')
MANIFEST_TEMPLATES = [filename for filename in
                      [os.path.join(dp, f) for dp, dn, fn in
                       os.walk(TEMPLATES_DIR) for f in fn if f.endswith('.tpl')]
                      if '_NAME_' in filename]
COMMON_TEMPLATES = [filename for filename in
                    [os.path.join(dp, f) for dp, dn, fn in
                     os.walk(TEMPLATES_DIR) for f in fn if f.endswith('.tpl')]
                    if '_NAME_' not in filename]
MANIFEST_FILE_PATTERN = '*_psa.json'
SPM_CORE_ROOT = path_join(MBED_OS_ROOT, 'components', 'TARGET_PSA')


def generate_source_files(
        templates,
        render_args,
        output_folder,
        extra_filters=None
):
    """
    Generate SPM common C code from manifests using given templates

    :param templates: Dictionary of template and their auto-generated products
    :param render_args: Dictionary of arguments that should be passed to render
    :param output_folder: Output directory for file generation
    :param extra_filters: Dictionary of extra filters to use in the rendering
           process
    :return: Path to generated folder containing common generated files
    """

    rendered_files = []
    templates_dirs = list(
        set([os.path.dirname(path) for path in templates])
    )
    template_files = {os.path.basename(t): t for t in templates}

    # Load templates for the code generation.
    env = Environment(
        loader=FileSystemLoader(templates_dirs),
        lstrip_blocks=True,
        trim_blocks=True,
        undefined=StrictUndefined
    )
    if extra_filters:
        env.filters.update(extra_filters)

    for tf in template_files:
        template = env.get_template(tf)
        rendered_files.append(
            (templates[template_files[tf]], template.render(**render_args)))
        rendered_file_dir = os.path.dirname(templates[template_files[tf]])
        if not os.path.exists(rendered_file_dir):
            os.makedirs(rendered_file_dir)

    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    for fname, data in rendered_files:
        with open(fname, 'wt') as fh:
            fh.write(data)

    return output_folder


def generate_partitions_sources(manifest_files, extra_filters=None):
    """
    Process all the given manifest files and generate C code from them

    :param manifest_files: List of manifest files
    :param extra_filters: Dictionary of extra filters to use in the rendering
           process
    :return: List of paths to the generated files
    """

    # Construct a list of all the manifests and sids.
    manifests = []
    for manifest_file in manifest_files:
        manifest = Manifest.from_json(manifest_file)
        manifests.append(manifest)

    generated_folders = set()
    for manifest in manifests:
        manifest_output_folder = manifest.autogen_folder

        render_args = {
            'partition': manifest,
            'dependent_partitions': manifest.find_dependencies(manifests),
            'script_ver': __version__
        }
        manifest_output_folder = generate_source_files(
            manifest.templates_to_files(MANIFEST_TEMPLATES,
                                        TEMPLATES_DIR,
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
    autogen_folder = output_dir
    templates_dict = {
        t: path_join(autogen_folder,
                     os.path.relpath(os.path.splitext(t)[0], TEMPLATES_DIR))
        for t in COMMON_TEMPLATES
    }

    complete_source_list = list(templates_dict.values())

    # Construct lists of all the manifests and mmio_regions.
    region_list = []
    manifests = []
    for manifest_file in manifest_files:
        manifest_obj = Manifest.from_json(manifest_file)
        manifests.append(manifest_obj)
        for region in manifest_obj.mmio_regions:
            region_list.append(region)
        complete_source_list.extend(
            list(manifest_obj.templates_to_files(
                MANIFEST_TEMPLATES,
                TEMPLATES_DIR,
                manifest_obj.autogen_folder).values())
        )

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
        autogen_folder,
        extra_filters=extra_filters
    )


def generate_psa_code():
    # Find all manifest files in the mbed-os tree
    service_manifest_files, test_manifest_files = manifests_discovery(root_dir=MBED_OS_ROOT)

    # Generate partition code for each manifest file
    generate_partitions_sources(service_manifest_files + test_manifest_files)

    # Generate default system psa setup file (only system partitions)
    generate_psa_setup(service_manifest_files,
        SPM_CORE_ROOT, weak_setup=True)

    tests_dict = {}
    for test_manifest in test_manifest_files:
        test_dir = os.path.dirname(test_manifest)
        if test_dir not in tests_dict:
            tests_dict[test_dir] = [test_manifest]
        else:
            tests_dict[test_dir].append(test_manifest)

    for test_dir in tests_dict:
        generate_psa_setup(service_manifest_files + tests_dict[test_dir],
                           test_dir, weak_setup=False)


if __name__ == '__main__':
    generate_psa_code()
