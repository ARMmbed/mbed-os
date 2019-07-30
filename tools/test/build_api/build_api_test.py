"""
mbed SDK
Copyright (c) 2016 ARM Limited

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
from collections import namedtuple
from mock import patch, MagicMock
from tools.build_api import prepare_toolchain, build_project, build_library
from tools.regions import merge_region_list
from tools.resources import Resources, FileRef
from tools.toolchains import TOOLCHAINS, mbedToolchain
from tools.notifier.mock import MockNotifier
from tools.config import Region, Config, ConfigException
from tools.utils import ToolException
from intelhex import IntelHex

"""
Tests for build_api.py
"""
make_mock_target = namedtuple(
    "Target", "get_post_build_hook name features core supported_toolchains build_tools_metadata")
#Add ARMC5 to the supported_toolchains list as ARMC5 actually refers ARM Compiler 5 and is needed by ARM/ARM_STD classes when it checks for supported toolchains
TOOLCHAINS.add("ARMC5")
#Make a mock build_tools_metadata
mock_build_tools_metadata = {u'version':0, u'public':False}

class BuildApiTests(unittest.TestCase):
    """
    Test cases for Build Api
    """

    def setUp(self):
        """
        Called before each test case

        :return:
        """
        self.target = "K64F"
        self.src_paths = ['.']
        self.toolchain_name = "ARM"
        self.build_path = "build_path"

    def tearDown(self):
        """
        Called after each test case

        :return:
        """
        pass

    @patch('tools.toolchains.arm.ARM_STD.parse_dependencies',
           return_value=["foo"])
    @patch('tools.toolchains.mbedToolchain.need_update',
           side_effect=[i % 2 for i in range(3000)])
    @patch('os.mkdir')
    @patch('tools.toolchains.mbedToolchain.dump_build_profile')
    @patch('tools.utils.run_cmd', return_value=(b'', b'', 0))
    def test_always_complete_build(self, *_):
        notify = MockNotifier()
        toolchain = prepare_toolchain(self.src_paths, self.build_path, self.target,
                                      self.toolchain_name, notify=notify)

        res = Resources(MockNotifier()).scan_with_toolchain(
            self.src_paths, toolchain)

        toolchain.RESPONSE_FILES=False
        toolchain.config_processed = True
        toolchain.config_file = "junk"
        toolchain.compile_sources(res)

        assert any('percent' in msg and msg['percent'] == 100.0
                   for msg in notify.messages if msg)

    @patch('tools.toolchains.arm.ARM_STD.parse_dependencies',
           return_value=["foo"])
    @patch('tools.toolchains.mbedToolchain.need_update',
           side_effect=[i % 2 for i in range(3000)])
    @patch('os.mkdir')
    @patch('tools.toolchains.mbedToolchain.dump_build_profile')
    @patch('tools.utils.run_cmd', return_value=(b'', b'', 0))
    def test_compile_legacy_sources_always_complete_build(self, *_):
        """Test that compile_legacy_sources() completes."""
        notify = MockNotifier()
        toolchain = prepare_toolchain(self.src_paths, self.build_path, self.target,
                                      self.toolchain_name, notify=notify)

        res = Resources(MockNotifier()).scan_with_toolchain(
            self.src_paths, toolchain)

        toolchain.RESPONSE_FILES=False
        toolchain.config_processed = True
        toolchain.config_file = "junk"
        toolchain.compile_legacy_sources(res)

        assert any('percent' in msg and msg['percent'] == 100.0
                   for msg in notify.messages if msg)

    def test_dirs_exclusion_from_file_to_compile(self):
        """Test that dirs can be excluded from the build."""
        files_to_compile = [
            FileRef(
                name="platform/TARGET_CORTEX_M/TOOLCHAIN_ARM/except.S",
                path="./platform/TARGET_CORTEX_M/TOOLCHAIN_ARM/except.S",
            ),
            FileRef(
                name="rtos/TARGET_CORTEX/rtx5/RTX/Source/TOOLCHAIN_ARM/TARGET_RTOS_M4_M7/targets_irq_cm4f.S",
                path="./rtos/TARGET_CORTEX/rtx5/RTX/Source/TOOLCHAIN_ARM/TARGET_RTOS_M4_M7/targets_irq_cm4f.S",
            ),
        ]
        exclude_dirs = ["platform/", "drivers/", "targets/"]
        expected_compilation_queue = [
            FileRef(
                name="rtos/TARGET_CORTEX/rtx5/RTX/Source/TOOLCHAIN_ARM/TARGET_RTOS_M4_M7/targets_irq_cm4f.S",
                path="./rtos/TARGET_CORTEX/rtx5/RTX/Source/TOOLCHAIN_ARM/TARGET_RTOS_M4_M7/targets_irq_cm4f.S",
            )
        ]
        compilation_queue = mbedToolchain._exclude_files_from_build(
            files_to_compile, exclude_dirs
        )
        self.assertEqual(compilation_queue, expected_compilation_queue)

    @patch('tools.build_api.Config')
    def test_prepare_toolchain_app_config(self, mock_config_init):
        """
        Test that prepare_toolchain uses app_config correctly

        :param mock_config_init: mock of Config __init__
        :return:
        """
        app_config = "app_config"
        mock_target = make_mock_target(lambda _ : None,
                                       "Junk", [], "Cortex-M3", TOOLCHAINS, mock_build_tools_metadata)
        mock_config_init.return_value = namedtuple(
            "Config", "target has_regions name")(mock_target, False, None)

        prepare_toolchain(self.src_paths, None, self.target, self.toolchain_name,
                          app_config=app_config)

        mock_config_init.assert_called_once_with(self.target, self.src_paths,
                                                 app_config=app_config)

    @patch('tools.build_api.Config')
    def test_prepare_toolchain_no_app_config(self, mock_config_init):
        """
        Test that prepare_toolchain correctly deals with no app_config

        :param mock_config_init: mock of Config __init__
        :return:
        """
        mock_target = make_mock_target(lambda _ : None,
                                       "Junk", [], "Cortex-M3", TOOLCHAINS, mock_build_tools_metadata)
        mock_config_init.return_value = namedtuple(
            "Config", "target has_regions name")(mock_target, False, None)

        prepare_toolchain(self.src_paths, None, self.target, self.toolchain_name)

        mock_config_init.assert_called_once_with(self.target, self.src_paths,
                                                 app_config=None)

    @patch('tools.build_api.Resources')
    @patch('tools.build_api.mkdir')
    @patch('os.path.exists')
    @patch('tools.build_api.prepare_toolchain')
    def test_build_project_app_config(self, mock_prepare_toolchain, mock_exists, _, __):
        """
        Test that build_project uses app_config correctly

        :param mock_prepare_toolchain: mock of function prepare_toolchain
        :param mock_exists: mock of function os.path.exists
        :param _: mock of function mkdir (not tested)
        :param __: mock of class Resources (not tested)
        :return:
        """
        notify = MockNotifier()
        app_config = "app_config"
        mock_exists.return_value = False
        mock_prepare_toolchain().link_program.return_value = 1, 2
        mock_prepare_toolchain().config = MagicMock(
            has_regions=None,
            name=None,
            lib_config_data=None,
        )
        mock_prepare_toolchain().config.deliver_into.return_value = (None, None)
        mock_prepare_toolchain().config.name = None

        build_project(self.src_paths, self.build_path, self.target,
                      self.toolchain_name, app_config=app_config, notify=notify)

        args = mock_prepare_toolchain.call_args
        self.assertTrue('app_config' in args[1],
                        "prepare_toolchain was not called with app_config")
        self.assertEqual(args[1]['app_config'], app_config,
                         "prepare_toolchain was called with an incorrect app_config")

    @patch('tools.build_api.Resources')
    @patch('tools.build_api.mkdir')
    @patch('os.path.exists')
    @patch('tools.build_api.prepare_toolchain')
    def test_build_project_no_app_config(self, mock_prepare_toolchain, mock_exists, _, __):
        """
        Test that build_project correctly deals with no app_config

        :param mock_prepare_toolchain: mock of function prepare_toolchain
        :param mock_exists: mock of function os.path.exists
        :param _: mock of function mkdir (not tested)
        :param __: mock of class Resources (not tested)
        :return:
        """
        notify = MockNotifier()
        mock_exists.return_value = False
        # Needed for the unpacking of the returned value
        mock_prepare_toolchain().link_program.return_value = 1, 2
        mock_prepare_toolchain().config = MagicMock(
            has_regions=None,
            name=None,
            lib_config_data=None,
        )
        mock_prepare_toolchain().config.deliver_into.return_value = (None, None)
        mock_prepare_toolchain().config.name = None

        build_project(self.src_paths, self.build_path, self.target,
                      self.toolchain_name, notify=notify)

        args = mock_prepare_toolchain.call_args
        self.assertTrue('app_config' in args[1],
                        "prepare_toolchain was not called with app_config")
        self.assertEqual(args[1]['app_config'], None,
                         "prepare_toolchain was called with an incorrect app_config")

    @patch('tools.build_api.Resources')
    @patch('tools.build_api.mkdir')
    @patch('os.path.exists')
    @patch('tools.build_api.prepare_toolchain')
    def test_build_library_app_config(self, mock_prepare_toolchain, mock_exists, _, __):
        """
        Test that build_library uses app_config correctly

        :param mock_prepare_toolchain: mock of function prepare_toolchain
        :param mock_exists: mock of function os.path.exists
        :param _: mock of function mkdir (not tested)
        :param __: mock of class Resources (not tested)
        :return:
        """
        notify = MockNotifier()
        app_config = "app_config"
        mock_exists.return_value = False

        build_library(self.src_paths, self.build_path, self.target,
                      self.toolchain_name, app_config=app_config, notify=notify)

        args = mock_prepare_toolchain.call_args
        self.assertTrue('app_config' in args[1],
                        "prepare_toolchain was not called with app_config")
        self.assertEqual(args[1]['app_config'], app_config,
                         "prepare_toolchain was called with an incorrect app_config")

    @patch('tools.build_api.Resources')
    @patch('tools.build_api.mkdir')
    @patch('os.path.exists')
    @patch('tools.build_api.prepare_toolchain')
    def test_build_library_no_app_config(self, mock_prepare_toolchain, mock_exists, _, __):
        """
        Test that build_library correctly deals with no app_config

        :param mock_prepare_toolchain: mock of function prepare_toolchain
        :param mock_exists: mock of function os.path.exists
        :param _: mock of function mkdir (not tested)
        :param __: mock of class Resources (not tested)
        :return:
        """
        notify = MockNotifier()
        mock_exists.return_value = False

        build_library(self.src_paths, self.build_path, self.target,
                      self.toolchain_name, notify=notify)

        args = mock_prepare_toolchain.call_args
        self.assertTrue('app_config' in args[1],
                        "prepare_toolchain was not called with app_config")
        self.assertEqual(args[1]['app_config'], None,
                         "prepare_toolchain was called with an incorrect app_config")

    @patch('tools.regions.intelhex_offset')
    def test_merge_region_no_fit(self, mock_intelhex_offset):
        """
        Test that merge_region_list call fails when part size overflows region size.
        :param mock_intelhex_offset: mocked intel_hex_offset call.
        :return:
        """
        max_addr = 87444
        # create a dummy hex file with above max_addr
        mock_intelhex_offset.return_value = IntelHex({0:2, max_addr:0})

        # create application and post-application regions and merge.
        region_application = Region("application", 10000, 86000, True, "random.hex")
        region_post_application = Region("postapplication", 100000, 90000, False, None)

        notify = MockNotifier()
        region_list = [region_application, region_post_application]
        # path to store the result in, should not get used as we expect exception.
        res = "./"
        toolexception = False

        try:
            merge_region_list(region_list, res, notify, restrict_size=90000)
        except ToolException:
            toolexception = True

        self.assertTrue(toolexception, "Expected ToolException not raised")


    @patch('tools.config.exists')
    @patch('tools.config.isabs')
    @patch('tools.config.intelhex_offset')
    def test_bl_pieces(self, mock_intelhex_offset, mock_exists, mock_isabs):
        """

        :param mock_intelhex_offset: mock intel_hex_ofset call
        :param mock_exists: mock the file exists call
        :param mock_isabs: mock the isabs call
        :return:
        """
        """
        Test that merge region fails as expected when part size overflows region size.
        """
        cfg = Config('NRF52_DK')
        mock_exists.return_value = True
        mock_isabs.return_value = True
        max = 0x960
        #create mock MBR and BL and merge them
        mbr = IntelHex()
        for v in range(max):
            mbr[v] = v

        bl = IntelHex()
        min = 0x16000
        max = 0x22000
        for v in range(min, max):
            bl[v] = v
        mbr.merge(bl)

        mock_intelhex_offset.return_value = mbr

        # Place application within the bootloader and verify
        # that config exception is generated
        cfg.target.bootloader_img = True
        cfg.target.app_offset = min + 0x200
        cfg.target.restrict_size = '4096'

        ce = False
        if cfg.has_regions:
            try:
                for r in list(cfg.regions):
                    print(r)
            except ConfigException:
                    ce = True

        self.assertTrue(ce)

    @patch('tools.config.exists')
    @patch('tools.config.isabs')
    @patch('tools.config.intelhex_offset')
    def test_bl_too_large(self, mock_intelhex_offset, mock_exists, mock_isabs):
        """
        Create a BL that's too large to fit in ROM and test that exception is
        generated.
        :param mock_intelhex_offset: mock intel hex
        :param mock_exists: mock the file exists call
        :param mock_isabs: mock the isabs call
        :return:
        """
        cfg = Config('NRF52_DK')
        mock_exists.return_value = True
        mock_isabs.return_value = True

        # setup the hex file
        bl = IntelHex()
        min = 0x0
        max = 0x88000
        for v in range(max):
            bl[v] = v
        mock_intelhex_offset.return_value = bl
        cfg.target.bootloader_img = True
        ce = False

        if cfg.has_regions:
            try:
                for r in list(cfg.regions):
                    print(r)
            except ConfigException as e:
                print("args %s" % (e.args))
                if (e.args[0] == "bootloader segments don't fit within rom"):
                    ce = True

        self.assertTrue(ce)

if __name__ == '__main__':
    unittest.main()
