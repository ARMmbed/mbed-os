#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import unittest
from unittest.mock import patch

from python_tests.mbed_tools.devices.markers import windows_only


@windows_only
class TestSystemDataLoader(unittest.TestCase):
    @patch("mbed_tools.devices._internal.windows.system_data_loader.load_all")
    def test_system_data_load(self, load_all):
        from mbed_tools.devices._internal.windows.system_data_loader import SystemDataLoader, SYSTEM_DATA_TYPES

        def mock_system_element_fetcher(arg):
            return (arg, list())

        load_all.side_effect = mock_system_element_fetcher

        loader = SystemDataLoader()
        for type in SYSTEM_DATA_TYPES:
            self.assertIsNotNone(loader.get_system_data(type))
            self.assertTrue(isinstance(loader.get_system_data(type), list))
        load_all.assert_called()
