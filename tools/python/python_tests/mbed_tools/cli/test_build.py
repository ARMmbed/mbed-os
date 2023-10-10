#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import contextlib
import os
import pathlib

from tempfile import TemporaryDirectory
from unittest import TestCase, mock

from click.testing import CliRunner

import sys
print(sys.path)

from mbed_tools.cli.build import build
from mbed_tools.project._internal.project_data import BUILD_DIR
from mbed_tools.build.config import CMAKE_CONFIG_FILE


DEFAULT_BUILD_ARGS = ["-t", "GCC_ARM", "-m", "K64F"]
DEFAULT_BUILD_SUBDIR = pathlib.Path("K64F", "develop", "GCC_ARM")


@contextlib.contextmanager
def mock_project_directory(
    program, mbed_config_exists=False, build_tree_exists=False, build_subdir=DEFAULT_BUILD_SUBDIR
):
    with TemporaryDirectory() as tmp_dir:
        root = pathlib.Path(tmp_dir, "test-project")
        root.mkdir()
        program.root = root
        program.files.cmake_build_dir = root / BUILD_DIR / build_subdir
        program.files.cmake_config_file = root / BUILD_DIR / build_subdir / CMAKE_CONFIG_FILE
        if mbed_config_exists:
            program.files.cmake_config_file.parent.mkdir(exist_ok=True, parents=True)
            program.files.cmake_config_file.touch(exist_ok=True)

        if build_tree_exists:
            program.files.cmake_build_dir.mkdir(exist_ok=True, parents=True)

        yield


@mock.patch("mbed_tools.cli.build.generate_build_system")
@mock.patch("mbed_tools.cli.build.build_project")
@mock.patch("mbed_tools.cli.build.MbedProgram")
@mock.patch("mbed_tools.cli.build.generate_config")
class TestBuildCommand(TestCase):
    def test_searches_for_mbed_program_at_default_project_path(
        self, generate_config, mbed_program, build_project, generate_build_system
    ):
        runner = CliRunner()
        runner.invoke(build, DEFAULT_BUILD_ARGS)

        mbed_program.from_existing.assert_called_once_with(pathlib.Path(os.getcwd()), DEFAULT_BUILD_SUBDIR)

    def test_searches_for_mbed_program_at_user_defined_project_root(
        self, generate_config, mbed_program, build_project, generate_build_system
    ):
        project_path = "my-blinky"

        runner = CliRunner()
        runner.invoke(build, ["-p", project_path, *DEFAULT_BUILD_ARGS])

        mbed_program.from_existing.assert_called_once_with(pathlib.Path(project_path), DEFAULT_BUILD_SUBDIR)

    def test_calls_generate_build_system_if_build_tree_nonexistent(
        self, generate_config, mbed_program, build_project, generate_build_system
    ):
        program = mbed_program.from_existing()
        with mock_project_directory(program, mbed_config_exists=True, build_tree_exists=False):
            generate_config.return_value = [mock.MagicMock(), mock.MagicMock()]

            runner = CliRunner()
            runner.invoke(build, DEFAULT_BUILD_ARGS)

            generate_build_system.assert_called_once_with(program.root, program.files.cmake_build_dir, "develop")

    def test_generate_config_called_if_config_script_nonexistent(
        self, generate_config, mbed_program, build_project, generate_build_system
    ):
        program = mbed_program.from_existing()
        with mock_project_directory(
            program, mbed_config_exists=False, build_subdir=pathlib.Path("K64F", "develop", "GCC_ARM")
        ):
            target = "k64f"
            toolchain = "gcc_arm"

            runner = CliRunner()
            runner.invoke(build, ["-t", toolchain, "-m", target])

            generate_config.assert_called_once_with(target.upper(), toolchain.upper(), program)

    def test_raises_if_gen_config_toolchain_not_passed_when_required(
        self, generate_config, mbed_program, build_project, generate_build_system
    ):
        program = mbed_program.from_existing()
        with mock_project_directory(program, mbed_config_exists=False):
            target = "k64f"

            runner = CliRunner()
            result = runner.invoke(build, ["-m", target])

            self.assertIsNotNone(result.exception)
            self.assertRegex(result.output, "--toolchain")

    def test_raises_if_gen_config_target_not_passed_when_required(
        self, generate_config, mbed_program, build_project, generate_build_system
    ):
        program = mbed_program.from_existing()
        with mock_project_directory(program, mbed_config_exists=False):
            toolchain = "gcc_arm"

            runner = CliRunner()
            result = runner.invoke(build, ["-t", toolchain])

            self.assertIsNotNone(result.exception)
            self.assertRegex(result.output, "--mbed-target")

    def test_raises_if_target_identifier_not_int(
        self, generate_config, mbed_program, build_project, generate_build_system
    ):
        target = "K64F[foo]"

        result = CliRunner().invoke(build, ["-m", target, "-t", "gcc_arm"])
        self.assertIsNotNone(result.exception)
        self.assertRegex(result.output, "ID")

    def test_raises_if_target_identifier_negative(
        self, generate_config, mbed_program, build_project, generate_build_system
    ):
        target = "K64F[-1]"

        result = CliRunner().invoke(build, ["-m", target, "-t", "gcc_arm"])
        self.assertIsNotNone(result.exception)
        self.assertRegex(result.output, "ID")

    def test_build_system_regenerated_when_mbed_os_path_passed(
        self, generate_config, mbed_program, build_project, generate_build_system
    ):
        program = mbed_program.from_existing()
        with mock_project_directory(
            program,
            mbed_config_exists=True,
            build_tree_exists=True,
            build_subdir=pathlib.Path("K64F", "develop", "GCC_ARM"),
        ):
            generate_config.return_value = [mock.MagicMock(), mock.MagicMock()]

            toolchain = "gcc_arm"
            target = "k64f"
            mbed_os_path = "./extern/mbed-os"

            runner = CliRunner()
            runner.invoke(build, ["-t", toolchain, "-m", target, "--mbed-os-path", mbed_os_path])

            generate_config.assert_called_once_with(target.upper(), toolchain.upper(), program)
            generate_build_system.assert_called_once_with(program.root, program.files.cmake_build_dir, "develop")

    def test_custom_targets_location_used_when_passed(
        self, generate_config, mbed_program, build_project, generate_build_system
    ):
        program = mbed_program.from_existing()
        with mock_project_directory(program, mbed_config_exists=True, build_tree_exists=True):
            toolchain = "gcc_arm"
            target = "k64f"
            custom_targets_json_path = pathlib.Path("custom", "custom_targets.json")

            runner = CliRunner()
            runner.invoke(build, ["-t", toolchain, "-m", target, "--custom-targets-json", custom_targets_json_path])

            generate_config.assert_called_once_with(target.upper(), toolchain.upper(), program)
            self.assertEqual(program.files.custom_targets_json, custom_targets_json_path)

    def test_app_config_used_when_passed(
        self, generate_config, mbed_program, build_project, generate_build_system
    ):
        program = mbed_program.from_existing()
        with mock_project_directory(program, mbed_config_exists=True, build_tree_exists=True):
            toolchain = "gcc_arm"
            target = "k64f"
            app_config_path = pathlib.Path("alternative_config.json")

            runner = CliRunner()
            runner.invoke(build, ["-t", toolchain, "-m", target, "--app-config", app_config_path])

            generate_config.assert_called_once_with(target.upper(), toolchain.upper(), program)
            self.assertEqual(program.files.app_config_file, app_config_path)

    def test_profile_used_when_passed(
        self, generate_config, mbed_program, build_project, generate_build_system
    ):
        program = mbed_program.from_existing()
        mbed_program.reset_mock()  # clear call count from previous line

        with mock_project_directory(program, mbed_config_exists=True, build_tree_exists=True):
            generate_config.return_value = [mock.MagicMock(), mock.MagicMock()]

            toolchain = "gcc_arm"
            target = "k64f"
            profile = "release"

            runner = CliRunner()
            runner.invoke(build, ["-t", toolchain, "-m", target, "--profile", profile])

            mbed_program.from_existing.assert_called_once_with(
                pathlib.Path(os.getcwd()),
                pathlib.Path(target.upper(), profile, toolchain.upper())
            )
            generate_config.assert_called_once_with(target.upper(), toolchain.upper(), program)
            generate_build_system.assert_called_once_with(program.root, program.files.cmake_build_dir, profile)

    def test_build_folder_removed_when_clean_flag_passed(
        self, generate_config, mbed_program, build_project, generate_build_system
    ):
        program = mbed_program.from_existing()
        with mock_project_directory(
            program,
            mbed_config_exists=True,
            build_tree_exists=True,
            build_subdir=pathlib.Path("K64F", "develop", "GCC_ARM"),
        ):
            generate_config.return_value = [mock.MagicMock(), mock.MagicMock()]

            toolchain = "gcc_arm"
            target = "k64f"

            runner = CliRunner()
            runner.invoke(build, ["-t", toolchain, "-m", target, "-c"])

            generate_config.assert_called_once_with(target.upper(), toolchain.upper(), program)
            generate_build_system.assert_called_once_with(program.root, program.files.cmake_build_dir, "develop")
            self.assertFalse(program.files.cmake_build_dir.exists())

    @mock.patch("mbed_tools.cli.build.flash_binary")
    @mock.patch("mbed_tools.cli.build.find_all_connected_devices")
    def test_build_flash_options_bin_target(
        self,
        mock_find_devices,
        flash_binary,
        generate_config,
        mbed_program,
        build_project,
        generate_build_system,
    ):
        # A target with bin images does not need OUTPUT_EXT defined
        generate_config.return_value = [mock.MagicMock(), mock.MagicMock()]
        mock_find_devices.return_value = [mock.MagicMock()]
        runner = CliRunner()
        runner.invoke(build, ["--flash", *DEFAULT_BUILD_ARGS])
        call_args = flash_binary.call_args
        args, kwargs = call_args
        flash_binary.assert_called_once_with(args[0], args[1], args[2], args[3], False)

    @mock.patch("mbed_tools.cli.build.flash_binary")
    @mock.patch("mbed_tools.cli.build.find_all_connected_devices")
    def test_build_flash_options_hex_target(
        self,
        mock_find_devices,
        flash_binary,
        generate_config,
        mbed_program,
        build_project,
        generate_build_system,
    ):
        generate_config.return_value = [{"OUTPUT_EXT": "hex"}, mock.MagicMock()]
        mock_find_devices.return_value = [mock.MagicMock()]
        runner = CliRunner()
        runner.invoke(build, ["--flash", *DEFAULT_BUILD_ARGS])
        call_args = flash_binary.call_args
        args, kwargs = call_args
        flash_binary.assert_called_once_with(args[0], args[1], args[2], args[3], True)

    @mock.patch("mbed_tools.cli.build.flash_binary")
    @mock.patch("mbed_tools.cli.build.find_all_connected_devices")
    def test_build_flash_both_two_devices(
        self,
        mock_find_devices,
        flash_binary,
        generate_config,
        mbed_program,
        build_project,
        generate_build_system,
    ):
        generate_config.return_value = [mock.MagicMock(), mock.MagicMock()]
        mock_find_devices.return_value = [mock.MagicMock(), mock.MagicMock()]
        runner = CliRunner()
        runner.invoke(build, ["--flash", *DEFAULT_BUILD_ARGS])
        self.assertEqual(flash_binary.call_count, 2)

    @mock.patch("mbed_tools.cli.build.flash_binary")
    @mock.patch("mbed_tools.cli.build.find_connected_device")
    def test_build_flash_only_identifier_device(
        self,
        mock_find_device,
        flash_binary,
        generate_config,
        mbed_program,
        build_project,
        generate_build_system,
    ):
        generate_config.return_value = [mock.MagicMock(), mock.MagicMock()]
        mock_find_device.return_value = mock.MagicMock()
        runner = CliRunner()
        runner.invoke(build, ["--flash", "-m", "K64F[1]", "-t", "GCC_ARM"])
        self.assertEqual(flash_binary.call_count, 1)

    @mock.patch("mbed_tools.cli.build.terminal")
    @mock.patch("mbed_tools.cli.build.find_connected_device")
    def test_sterm_is_started_when_flag_passed(
        self, mock_find_device, mock_terminal, generate_config, mbed_program, build_project, generate_build_system
    ):
        target = "K64F"
        serial_port = "tty.k64f"
        baud = 115200
        mock_find_device.return_value = mock.Mock(serial_port=serial_port)
        generate_config.return_value = [mock.MagicMock(), mock.MagicMock()]

        CliRunner().invoke(build, ["-m", target, "-t", "gcc_arm", "--sterm", "--baudrate", baud])

        mock_find_device.assert_called_once_with(target, None)
        mock_terminal.run.assert_called_once_with(serial_port, baud)

    @mock.patch("mbed_tools.cli.build.terminal")
    @mock.patch("mbed_tools.cli.build.find_connected_device")
    def test_raises_if_device_does_not_have_serial_port_and_sterm_flag_given(
        self, mock_find_device, mock_terminal, generate_config, mbed_program, build_project, generate_build_system
    ):
        target = "K64F"
        serial_port = None
        mock_find_device.return_value = mock.Mock(serial_port=serial_port)
        generate_config.return_value = [mock.MagicMock(), mock.MagicMock()]

        output = CliRunner().invoke(build, ["-m", target, "-t", "gcc_arm", "--sterm"])
        self.assertEqual(type(output.exception), SystemExit)
        mock_terminal.assert_not_called()
