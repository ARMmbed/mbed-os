#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Mbed Program abstraction layer."""
import logging

from pathlib import Path
from typing import Dict
from urllib.parse import urlparse

from mbed_tools.project.exceptions import ProgramNotFound, ExistingProgram, MbedOSNotFound
from mbed_tools.project._internal.project_data import (
    MbedProgramFiles,
    MbedOS,
    MBED_OS_REFERENCE_FILE_NAME,
    MBED_OS_DIR_NAME,
)

logger = logging.getLogger(__name__)


class MbedProgram:
    """Represents an Mbed program.

    An `MbedProgram` consists of:
        * A copy of, or reference to, `MbedOS`
        * A set of `MbedProgramFiles`
        * A collection of references to external libraries, defined in .lib files located in the program source tree
    """

    def __init__(self, program_files: MbedProgramFiles, mbed_os: MbedOS) -> None:
        """Initialise the program attributes.

        Args:
            program_files: Object holding paths to a set of files that define an Mbed program.
            mbed_os: An instance of `MbedOS` holding paths to locations in the local copy of the Mbed OS source.
        """
        self.files = program_files
        self.root = self.files.mbed_os_ref.parent
        self.mbed_os = mbed_os

    @classmethod
    def from_new(cls, dir_path: Path) -> "MbedProgram":
        """Create an MbedProgram from an empty directory.

        Creates the directory if it doesn't exist.

        Args:
            dir_path: Directory in which to create the program.

        Raises:
            ExistingProgram: An existing program was found in the path.
        """
        if _tree_contains_program(dir_path):
            raise ExistingProgram(
                f"An existing Mbed program was found in the directory tree {dir_path}. It is not possible to nest Mbed "
                "programs. Please ensure there is no mbed-os.lib file in the cwd hierarchy."
            )

        logger.info(f"Creating Mbed program at path '{dir_path.resolve()}'")
        dir_path.mkdir(exist_ok=True)
        program_files = MbedProgramFiles.from_new(dir_path)
        logger.info(f"Creating git repository for the Mbed program '{dir_path}'")
        mbed_os = MbedOS.from_new(dir_path / MBED_OS_DIR_NAME)
        return cls(program_files, mbed_os)

    @classmethod
    def from_existing(
        cls, dir_path: Path, build_subdir: Path, mbed_os_path: Path = None, check_mbed_os: bool = True,
    ) -> "MbedProgram":
        """Create an MbedProgram from an existing program directory.

        Args:
            dir_path: Directory containing an Mbed program.
            build_subdir: The subdirectory for the CMake build tree.
            mbed_os_path: Directory containing Mbed OS.
            check_mbed_os: If True causes an exception to be raised if the Mbed OS source directory does not
                           exist.

        Raises:
            ProgramNotFound: An existing program was not found in the path.
        """
        if mbed_os_path is None:
            program_root = _find_program_root(dir_path)
            mbed_os_path = program_root / MBED_OS_DIR_NAME
        else:
            program_root = dir_path

        logger.info(f"Found existing Mbed program at path '{program_root}'")
        program = MbedProgramFiles.from_existing(program_root, build_subdir)

        try:
            mbed_os = MbedOS.from_existing(mbed_os_path, check_mbed_os)
        except ValueError as mbed_os_err:
            raise MbedOSNotFound(
                f"Mbed OS was not found due to the following error: {mbed_os_err}"
                "\nYou may need to resolve the mbed-os.lib reference. You can do this by performing a `deploy`."
            )

        return cls(program, mbed_os)


def parse_url(name_or_url: str) -> Dict[str, str]:
    """Create a valid github/armmbed url from a program name.

    Args:
        url: The URL, or a program name to turn into an URL.

    Returns:
        Dictionary containing the remote url and the destination path for the clone.
    """
    url_obj = urlparse(name_or_url)
    if url_obj.hostname:
        url = url_obj.geturl()
    elif ":" in name_or_url.split("/", maxsplit=1)[0]:
        # If non-standard and no slashes before first colon, git will recognize as scp ssh syntax
        url = name_or_url
    else:
        url = f"https://github.com/armmbed/{url_obj.path}"
    # We need to create a valid directory name from the url path section.
    return {"url": url, "dst_path": url_obj.path.rsplit("/", maxsplit=1)[-1].replace("/", "")}


def _tree_contains_program(path: Path) -> bool:
    """Check if the current path or its ancestors contain an mbed-os.lib file.

    Args:
        path: The starting path for the search. The search walks up the tree from this path.

    Returns:
        `True` if an mbed-os.lib file is located between `path` and filesystem root.
        `False` if no mbed-os.lib file was found.
    """
    try:
        _find_program_root(path)
        return True
    except ProgramNotFound:
        return False


def _find_program_root(cwd: Path) -> Path:
    """Walk up the directory tree, looking for an mbed-os.lib file.

    Programs contain an mbed-os.lib file at the root of the source tree.

    Args:
        cwd: The directory path to search for a program.

    Raises:
        ProgramNotFound: No mbed-os.lib file found in the path.

    Returns:
        Path containing the mbed-os.lib file.
    """
    potential_root = cwd.absolute().resolve()
    while str(potential_root) != str(potential_root.anchor):
        logger.debug(f"Searching for mbed-os.lib file at path {potential_root}")
        root_file = potential_root / MBED_OS_REFERENCE_FILE_NAME
        if root_file.exists() and root_file.is_file():
            logger.debug(f"mbed-os.lib file found at {potential_root}")
            return potential_root

        potential_root = potential_root.parent

    logger.debug("No mbed-os.lib file found.")
    raise ProgramNotFound(
        f"No program found from {cwd.resolve()} to {cwd.resolve().anchor}. Please set the directory to a program "
        "directory containing an mbed-os.lib file. You can also set the directory to a program subdirectory if there "
        "is an mbed-os.lib file at the root of your program's directory tree."
    )
