"""
mbed SDK
Copyright (c) 2011-2018 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import unittest
import mock

from mbed_host_tests.host_tests_conn_proxy.conn_primitive_remote import RemoteConnectorPrimitive


class RemoteResourceMock(object):
    def __init__(self, requirements):
        self._is_allocated = True
        self._is_connected = True
        self.requirements = requirements
        self.open_connection = mock.MagicMock()
        self.close_connection = mock.MagicMock()
        self.write = mock.MagicMock()
        self.read = mock.MagicMock()
        self.read.return_value = "abc"
        self.disconnect = mock.MagicMock()
        self.flash = mock.MagicMock()
        self.reset = mock.MagicMock()
        self.release = mock.MagicMock()

    @property
    def is_connected(self):
        return self._is_connected

    @property
    def is_allocated(self):
        return self._is_allocated


class RemoteModuleMock(object):
    class SerialParameters(object):
        def __init__(self, baudrate):
            self.baudrate = baudrate

    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.is_allocated_mock = mock.MagicMock()
        self.allocate = mock.MagicMock()
        self.allocate.side_effect = lambda req: RemoteResourceMock(req)
        self.get_resources = mock.MagicMock()
        self.get_resources.return_value = [1]

    @staticmethod
    def create(host, port):
        return RemoteModuleMock(host, port)


class ConnPrimitiveRemoteTestCase(unittest.TestCase):

    def setUp(self):
        self.config = {
            "grm_module": "RemoteModuleMock",
            "tags": "a,b",
            "image_path": "test.bin",
            "platform_name": "my_platform",
        }
        self.importer = mock.MagicMock()
        self.importer.side_effect = lambda x: RemoteModuleMock
        self.remote = RemoteConnectorPrimitive("remote", self.config, self.importer)

    def test_constructor(self):
        self.importer.assert_called_once_with("RemoteModuleMock")

        self.remote.client.get_resources.assert_called_once()
        self.assertEqual(self.remote.remote_module, RemoteModuleMock)
        self.assertIsInstance(self.remote.client, RemoteModuleMock)
        self.assertIsInstance(self.remote.selected_resource, RemoteResourceMock)

        # allocate is called
        self.remote.client.allocate.assert_called_once_with({
            'platform_name': self.config.get('platform_name'),
            'power_on': True,
            'connected': True,
            'tags': {"a": True, "b": True}})

        # flash is called
        self.remote.selected_resource.flash.assert_called_once_with("test.bin", forceflash=True)

        # open_connection is called
        self.remote.selected_resource.open_connection.assert_called_once()
        connect = self.remote.selected_resource.open_connection.call_args[1]
        self.assertEqual(connect["parameters"].baudrate, 9600)

        # reset once
        self.remote.selected_resource.reset.assert_called_once_with()

    def test_write(self):
        self.remote.write("abc")
        self.remote.selected_resource.write.assert_called_once_with("abc")

    def test_read(self):
        data = self.remote.read(6)
        self.remote.selected_resource.read.assert_called_once_with(6)
        self.assertEqual(data, "abc")

    def test_reset(self):
        self.remote.reset()
        self.assertEqual(self.remote.selected_resource.reset.call_count, 2)

    def test_finish(self):
        resource = self.remote.selected_resource
        self.remote.finish()
        self.assertEqual(self.remote.selected_resource, None)
        resource.close_connection.assert_called_once()
        resource.release.assert_called_once()


if __name__ == '__main__':
    unittest.main()
