#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Parses the Mbed configuration system and generates a CMake config script."""
import pathlib

from typing import Any, Tuple

from mbed_tools.lib.json_helpers import decode_json_file
from mbed_tools.project import MbedProgram
from mbed_tools.targets import get_target_by_name
from mbed_tools.build._internal.cmake_file import render_mbed_config_cmake_template
from mbed_tools.build._internal.config.assemble_build_config import Config, assemble_config
from mbed_tools.build._internal.write_files import write_file
from mbed_tools.build.exceptions import MbedBuildError

CMAKE_CONFIG_FILE = "mbed_config.cmake"
MBEDIGNORE_FILE = ".mbedignore"


def generate_config(target_name: str, toolchain: str, program: MbedProgram) -> Tuple[Config, pathlib.Path]:
    """Generate an Mbed config file after parsing the Mbed config system.

    Args:
        target_name: Name of the target to configure for.
        toolchain: Name of the toolchain to use.
        program: The MbedProgram to configure.

    Returns:
        Config object (UserDict).
        Path to the generated config file.
    """
    targets_data = _load_raw_targets_data(program)
    target_build_attributes = get_target_by_name(target_name, targets_data)
    config = assemble_config(
        target_build_attributes, [program.root, program.mbed_os.root], program.files.app_config_file
    )
    cmake_file_contents = render_mbed_config_cmake_template(
        target_name=target_name, config=config, toolchain_name=toolchain,
    )
    cmake_config_file_path = program.files.cmake_build_dir / CMAKE_CONFIG_FILE
    write_file(cmake_config_file_path, cmake_file_contents)
    mbedignore_path = program.files.cmake_build_dir / MBEDIGNORE_FILE
    write_file(mbedignore_path, "*")
    return config, cmake_config_file_path


def _load_raw_targets_data(program: MbedProgram) -> Any:
    targets_data = decode_json_file(program.mbed_os.targets_json_file)
    if program.files.custom_targets_json.exists():
        custom_targets_data = decode_json_file(program.files.custom_targets_json)
        for custom_target in custom_targets_data:
            if custom_target in targets_data:
                raise MbedBuildError(
                    f"Error found in {program.files.custom_targets_json}.\n"
                    f"A target with the name '{custom_target}' already exists in targets.json. "
                    "Please give your custom target a unique name so it can be identified."
                )

        targets_data.update(custom_targets_data)

    return targets_data
