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
import argparse
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
    MBED_OS_ROOT

__version__ = '1.1'
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
MANIFEST_FILE_PATTERN = '*_psa.json'
PSA_CORE_ROOT = path_join(MBED_OS_ROOT, 'components', 'TARGET_PSA')
TEMPLATES_DESC = path_join(SCRIPT_DIR, 'spm_template_file_list.json')


def _get_timestamp(f):
    return os.path.getmtime(f) if os.path.isfile(f) else 0


def is_up_to_date(manifest_files, out_files):
    manifest_timestamp = max(_get_timestamp(f) for f in manifest_files)
    out_timestamps = min(_get_timestamp(f) for f in out_files)
    return manifest_timestamp <= out_timestamps


def generate_spm_code(service_files, app_files, output_dir):
    with open(TEMPLATES_DESC, 'r') as fh:
        templates_data = json.load(fh)
        templates_dict = {
            path_join(MBED_OS_ROOT, t['template']):
                path_join(output_dir, t['output']) for t in templates_data
        }

    if is_up_to_date(service_files + app_files, list(templates_dict.values())):
        return

    # Construct lists of all the manifests and mmio_regions.
    service_manifests, service_region_list = parse_manifests(service_files)
    test_manifests, test_region_list = parse_manifests(app_files)

    # Validate the correctness of the manifest collection.
    validate_partition_manifests(service_manifests + test_manifests)

    region_list = service_region_list + test_region_list

    render_args = {
        'service_partitions': service_manifests,
        'test_partitions': test_manifests,
        'script_ver': __version__,
        'regions': region_list,
        'region_pair_list': list(itertools.combinations(region_list, 2)),
    }

    generate_source_files(templates_dict, render_args)


class AppendReadableDir(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        prosp_dir = os.path.abspath(values)
        if not os.path.isdir(prosp_dir):
            raise argparse.ArgumentTypeError("{} is missing".format(prosp_dir))
        if not os.access(prosp_dir, os.R_OK):
            raise argparse.ArgumentTypeError(
                "{} is not a accessible for read".format(prosp_dir))
        if not getattr(namespace, self.dest):
            setattr(namespace, self.dest, [])
        getattr(namespace, self.dest).append(prosp_dir)


def get_parser():
    parser = argparse.ArgumentParser(
        description='PSA SPM code generator',
        formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )
    parser.add_argument(
        '-u', '--user-app',
        action=AppendReadableDir,
        default=[ROOT],
        help='Root directory for recursive PSA manifest scan. Use for adding '
             'application specific secure partitions. Can be supplied more '
             'than once',
        metavar='DIR'
    )

    parser.add_argument(
        '-o', '--output-dir',
        default=ROOT,
        help='Root directory for generating the sources',
        metavar='DIR'
    )

    return parser


def main():
    parser = get_parser()
    args = parser.parse_args()

    services, apps = manifests_discovery(root_dirs=args.user_app,
                                         ignore_paths=['BUILD', '.git'])

    generate_spm_code(services, apps, args.output_dir)


if __name__ == '__main__':
    main()
