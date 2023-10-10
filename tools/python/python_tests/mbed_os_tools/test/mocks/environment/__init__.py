# Copyright (c) 2018, Arm Limited and affiliates.
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

import tempfile
import os
import shutil

from mock import patch, MagicMock
from copy import copy
from ..serial import MockSerial
from ..mbed_device import MockMbedDevice
from ..process import MockProcess

class MockTestEnvironment(object):

    def __init__(self, test_case, platform_info, image_path):
        self._test_case = test_case
        self._tempdir = tempfile.mkdtemp()
        self._patch_definitions = []
        self.patches = {}
        self._platform_info = copy(platform_info)

        # Clean and retarget path to tempdir
        self._image_path = self._clean_path(image_path)
        self._platform_info['mount_point'] = self._clean_path(
            self._platform_info['mount_point']
        )

        # Need to remove the drive letter in this case
        self._platform_info['serial_port'] = os.path.splitdrive(
            self._clean_path(self._platform_info['serial_port'])
        )[1]

        args = (
            'mbedhtrun -m {} -p {}:9600 -f '
            '"{}" -e "TESTS/host_tests" -d {} -c default '
            '-t {} -r default '
            '-C 4 --sync 5 -P 60'
        ).format(
            self._platform_info['platform_name'],
            self._platform_info['serial_port'],
            self._image_path,
            self._platform_info['mount_point'],
            self._platform_info['target_id']
        ).split()
        self.patch('sys.argv', new=args)

        # Mock detect
        detect_mock = MagicMock()
        detect_mock.return_value.list_mbeds.return_value = [
            self._platform_info
        ]
        self.patch('mbed_os_tools.detect.create', new=detect_mock)

        # Mock process calls and move them to threads to preserve mocks
        self.patch(
            'mbed_os_tools.test.host_tests_runner.host_test_default.Process',
            new=MagicMock(side_effect=self._process_side_effect)
        )
        self.patch(
            'mbed_os_tools.test.host_tests_plugins.host_test_plugins.call',
            new=MagicMock(return_value=0)
        )

        mock_serial = MockSerial()
        mock_device = MockMbedDevice(mock_serial)
        self.patch(
            'mbed_os_tools.test.host_tests_conn_proxy.conn_primitive_serial.Serial',
            new=MagicMock(return_value=mock_serial)
        )

    def _clean_path(self, path):
        # Remove the drive letter and ensure separators are consistent
        path = os.path.splitdrive(os.path.normpath(path))[1]
        return os.path.join(self._tempdir, path.lstrip(os.sep))

    @staticmethod
    def _process_side_effect(target=None, args=None):
        return MockProcess(target=target, args=args)

    def patch(self, path, **kwargs):
        self._patch_definitions.append((path, patch(path, **kwargs)))

    def __enter__(self):
        os.makedirs(os.path.dirname(self._image_path))
        with open(self._image_path, 'w') as _:
            pass

        os.makedirs(self._platform_info['mount_point'])

        for path, patcher in self._patch_definitions:
            self.patches[path] = patcher.start()

    def __exit__(self, type, value, traceback):
        for _, patcher in self._patch_definitions:
            patcher.stop()

        shutil.rmtree(self._tempdir)
