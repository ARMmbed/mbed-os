# Copyright (c) 2019 ARM Limited
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

import pytest
import os
from tools.notifier.mock import MockNotifier
from tools.resources import Resources, FileType
from tools.psa import find_secure_image


def test_find_secure_image():
    mock_notifier = MockNotifier()
    mock_resources = Resources(mock_notifier)
    ns_image_path = os.path.join('BUILD', 'TARGET_NS', 'app.bin')
    ns_test_path = os.path.join('BUILD', 'TARGET_NS', 'test.bin')
    config_s_image_name = 'target_config.bin'
    default_bin = os.path.join('prebuilt', config_s_image_name)
    test_bin = os.path.join('prebuilt', 'test.bin')

    with pytest.raises(Exception, match='ns_image_path and configured_s_image_path are mandatory'):
        find_secure_image(mock_notifier, mock_resources, None, None, FileType.BIN)
        find_secure_image(mock_notifier, mock_resources, ns_image_path, None, FileType.BIN)
        find_secure_image(mock_notifier, mock_resources, None, config_s_image_name, FileType.BIN)

    with pytest.raises(Exception, match='image_type must be of type BIN or HEX'):
        find_secure_image(mock_notifier, mock_resources, ns_image_path, config_s_image_name, None)
        find_secure_image(mock_notifier, mock_resources, ns_image_path, config_s_image_name, FileType.C_SRC)

    with pytest.raises(Exception, match='No image files found for this target'):
        find_secure_image(mock_notifier, mock_resources, ns_image_path, config_s_image_name, FileType.BIN)

    dummy_bin = os.path.join('path', 'to', 'dummy.bin')
    mock_resources.add_file_ref(FileType.BIN, dummy_bin, dummy_bin)

    with pytest.raises(Exception, match='Required secure image not found'):
        find_secure_image(mock_notifier, mock_resources, ns_image_path, config_s_image_name, FileType.BIN)

    mock_resources.add_file_ref(FileType.BIN, default_bin, default_bin)
    mock_resources.add_file_ref(FileType.BIN, test_bin, test_bin)
    secure_image = find_secure_image(mock_notifier, mock_resources, ns_image_path, config_s_image_name, FileType.BIN)
    assert secure_image == default_bin

    secure_image = find_secure_image(mock_notifier, mock_resources, ns_test_path, config_s_image_name, FileType.BIN)
    assert secure_image == test_bin
