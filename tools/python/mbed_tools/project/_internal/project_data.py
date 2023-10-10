#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Objects representing Mbed program and library data."""
import json
import logging

from dataclasses import dataclass
from pathlib import Path
from typing import Optional

from mbed_tools.project._internal.render_templates import (
    render_cmakelists_template,
    render_main_cpp_template,
    render_gitignore_template,
)

logger = logging.getLogger(__name__)

# Mbed program file names and constants.
APP_CONFIG_FILE_NAME = "mbed_app.json"
BUILD_DIR = "cmake_build"
CMAKELISTS_FILE_NAME = "CMakeLists.txt"
MAIN_CPP_FILE_NAME = "main.cpp"
MBED_OS_REFERENCE_FILE_NAME = "mbed-os.lib"
MBED_OS_DIR_NAME = "mbed-os"
TARGETS_JSON_FILE_PATH = Path("targets", "targets.json")
CUSTOM_TARGETS_JSON_FILE_NAME = "custom_targets.json"

# Information written to mbed-os.lib
MBED_OS_REFERENCE_URL = "https://github.com/ARMmbed/mbed-os"
MBED_OS_REFERENCE_ID = "master"

# For some reason Mbed OS expects the default mbed_app.json to contain some target_overrides
# for the K64F target. TODO: Find out why this wouldn't be defined in targets.json.
DEFAULT_APP_CONFIG = {"target_overrides": {"K64F": {"platform.stdio-baud-rate": 9600}}}


@dataclass
class MbedProgramFiles:
    """Files defining an MbedProgram.

    This object holds paths to the various files which define an MbedProgram.

    MbedPrograms must contain an mbed-os.lib reference file, defining Mbed OS as a program dependency. A program can
    optionally include an mbed_app.json file which defines application level config.

    Attributes:
        app_config_file: Path to mbed_app.json file. This can be `None` if the program doesn't set any custom config.
        mbed_os_ref: Library reference file for MbedOS. All programs require this file.
        cmakelists_file: A top-level CMakeLists.txt containing build definitions for the application.
        cmake_build_dir: The CMake build tree.
    """

    app_config_file: Optional[Path]
    mbed_os_ref: Path
    cmakelists_file: Path
    cmake_build_dir: Path
    custom_targets_json: Path

    @classmethod
    def from_new(cls, root_path: Path) -> "MbedProgramFiles":
        """Create MbedProgramFiles from a new directory.

        A "new directory" in this context means it doesn't already contain an Mbed program.

        Args:
            root_path: The directory in which to create the program data files.

        Raises:
            ValueError: A program .mbed or mbed-os.lib file already exists at this path.
        """
        app_config = root_path / APP_CONFIG_FILE_NAME
        mbed_os_ref = root_path / MBED_OS_REFERENCE_FILE_NAME
        cmakelists_file = root_path / CMAKELISTS_FILE_NAME
        main_cpp = root_path / MAIN_CPP_FILE_NAME
        gitignore = root_path / ".gitignore"
        cmake_build_dir = root_path / BUILD_DIR
        custom_targets_json = root_path / CUSTOM_TARGETS_JSON_FILE_NAME

        if mbed_os_ref.exists():
            raise ValueError(f"Program already exists at path {root_path}.")

        app_config.write_text(json.dumps(DEFAULT_APP_CONFIG, indent=4))
        mbed_os_ref.write_text(f"{MBED_OS_REFERENCE_URL}#{MBED_OS_REFERENCE_ID}")
        render_cmakelists_template(cmakelists_file, root_path.stem)
        render_main_cpp_template(main_cpp)
        render_gitignore_template(gitignore)
        return cls(
            app_config_file=app_config,
            mbed_os_ref=mbed_os_ref,
            cmakelists_file=cmakelists_file,
            cmake_build_dir=cmake_build_dir,
            custom_targets_json=custom_targets_json,
        )

    @classmethod
    def from_existing(cls, root_path: Path, build_subdir: Path) -> "MbedProgramFiles":
        """Create MbedProgramFiles from a directory containing an existing program.

        Args:
            root_path: The path containing the MbedProgramFiles.
            build_subdir: The subdirectory of BUILD_DIR to use for CMake build.
        """
        app_config: Optional[Path]
        app_config = root_path / APP_CONFIG_FILE_NAME
        if not app_config.exists():
            logger.info("This program does not contain an mbed_app.json config file.")
            app_config = None

        custom_targets_json = root_path / CUSTOM_TARGETS_JSON_FILE_NAME
        mbed_os_file = root_path / MBED_OS_REFERENCE_FILE_NAME

        cmakelists_file = root_path / CMAKELISTS_FILE_NAME
        if not cmakelists_file.exists():
            logger.warning("No CMakeLists.txt found in the program root.")
        cmake_build_dir = root_path / BUILD_DIR / build_subdir

        return cls(
            app_config_file=app_config,
            mbed_os_ref=mbed_os_file,
            cmakelists_file=cmakelists_file,
            cmake_build_dir=cmake_build_dir,
            custom_targets_json=custom_targets_json,
        )


@dataclass
class MbedOS:
    """Metadata associated with a copy of MbedOS.

    This object holds information about MbedOS used by MbedProgram.

    Attributes:
        root: The root path of the MbedOS source tree.
        targets_json_file: Path to a targets.json file, which contains target data specific to MbedOS revision.
    """

    root: Path
    targets_json_file: Path

    @classmethod
    def from_existing(cls, root_path: Path, check_root_path_exists: bool = True) -> "MbedOS":
        """Create MbedOS from a directory containing an existing MbedOS installation."""
        targets_json_file = root_path / TARGETS_JSON_FILE_PATH

        if check_root_path_exists and not root_path.exists():
            raise ValueError("The mbed-os directory does not exist.")

        if root_path.exists() and not targets_json_file.exists():
            raise ValueError("This MbedOS copy does not contain a targets.json file.")

        return cls(root=root_path, targets_json_file=targets_json_file)

    @classmethod
    def from_new(cls, root_path: Path) -> "MbedOS":
        """Create MbedOS from an empty or new directory."""
        return cls(root=root_path, targets_json_file=root_path / TARGETS_JSON_FILE_PATH)
