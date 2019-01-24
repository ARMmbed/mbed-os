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

import fnmatch
import shutil
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.realpath(os.path.join(SCRIPT_DIR, '..', '..', '..', '..', '..'))
DELIVERY_DIR = os.path.join(ROOT_DIR, 'DELIVERY', 'TARGET_FUTURE_SEQUANA_PSA')

for f in fnmatch.filter(os.listdir(DELIVERY_DIR), '*.hex'):
    test_suite_name = os.path.splitext(f)[0]
    test_directory = os.path.join(ROOT_DIR, 'TESTS', 'psa', test_suite_name)

    if os.path.exists(test_directory):
        target_dir = os.path.join(test_directory, 'TARGET_FUTURE_SEQUANA_PSA', f)
        if not os.path.exists(os.path.join(test_directory, 'TARGET_FUTURE_SEQUANA_PSA')):
            continue
    else:
        target_dir = os.path.join(SCRIPT_DIR, f)

    shutil.copyfile(os.path.join(DELIVERY_DIR, f), target_dir)
