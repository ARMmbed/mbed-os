#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import pathlib

from unittest import TestCase, mock

from click.testing import CliRunner

from mbed_tools.cli.configure import configure


class TestConfigureCommand(TestCase):
    @mock.patch("mbed_tools.cli.configure.generate_config")
    @mock.patch("mbed_tools.cli.configure.MbedProgram")
    def test_generate_config_called_with_correct_arguments(self, program, generate_config):
        CliRunner().invoke(configure, ["-m", "k64f", "-t", "gcc_arm"])

        generate_config.assert_called_once_with("K64F", "GCC_ARM", program.from_existing())

    @mock.patch("mbed_tools.cli.configure.generate_config")
    @mock.patch("mbed_tools.cli.configure.MbedProgram")
    def test_generate_config_called_with_mbed_os_path(self, program, generate_config):
        CliRunner().invoke(configure, ["-m", "k64f", "-t", "gcc_arm", "--mbed-os-path", "./extern/mbed-os"])

        generate_config.assert_called_once_with("K64F", "GCC_ARM", program.from_existing())

    @mock.patch("mbed_tools.cli.configure.generate_config")
    @mock.patch("mbed_tools.cli.configure.MbedProgram")
    def test_custom_targets_location_used_when_passed(self, program, generate_config):
        program = program.from_existing()
        custom_targets_json_path = pathlib.Path("custom", "custom_targets.json")
        CliRunner().invoke(
            configure, ["-t", "gcc_arm", "-m", "k64f", "--custom-targets-json", custom_targets_json_path]
        )

        generate_config.assert_called_once_with("K64F", "GCC_ARM", program)
        self.assertEqual(program.files.custom_targets_json, custom_targets_json_path)

    @mock.patch("mbed_tools.cli.configure.generate_config")
    @mock.patch("mbed_tools.cli.configure.MbedProgram")
    def test_custom_output_directory_used_when_passed(self, program, generate_config):
        program = program.from_existing()
        output_dir = pathlib.Path("build")
        CliRunner().invoke(configure, ["-t", "gcc_arm", "-m", "k64f", "-o", output_dir])

        generate_config.assert_called_once_with("K64F", "GCC_ARM", program)
        self.assertEqual(program.files.cmake_build_dir, output_dir)

    @mock.patch("mbed_tools.cli.configure.generate_config")
    @mock.patch("mbed_tools.cli.configure.MbedProgram")
    def test_app_config_used_when_passed(self, program, generate_config):
        program = program.from_existing()
        app_config_path = pathlib.Path("alternative_config.json")
        CliRunner().invoke(
            configure, ["-t", "gcc_arm", "-m", "k64f", "--app-config", app_config_path]
        )

        generate_config.assert_called_once_with("K64F", "GCC_ARM", program)
        self.assertEqual(program.files.app_config_file, app_config_path)

    @mock.patch("mbed_tools.cli.configure.generate_config")
    @mock.patch("mbed_tools.cli.configure.MbedProgram")
    def test_profile_used_when_passed(self, program, generate_config):
        test_program = program.from_existing()
        program.reset_mock()  # clear call count from previous line

        toolchain = "gcc_arm"
        target = "k64f"
        profile = "release"

        CliRunner().invoke(
            configure, ["-t", toolchain, "-m", target, "--profile", profile]
        )

        program.from_existing.assert_called_once_with(
            pathlib.Path("."),
            pathlib.Path(target.upper(), profile, toolchain.upper())
        )
        generate_config.assert_called_once_with("K64F", "GCC_ARM", test_program)
