#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Writes out files to specified locations."""
import pathlib
from mbed_tools.build.exceptions import InvalidExportOutputDirectory


def write_file(file_path: pathlib.Path, file_contents: str) -> None:
    """Writes out a string to a file.

    If the intermediate directories to the output directory don't exist,
    this function will create them.

    This function will overwrite any existing file of the same name in the
    output directory.

    Raises:
        InvalidExportOutputDirectory: it's not possible to export to the output directory provided
    """
    output_directory = file_path.parent
    if output_directory.is_file():
        raise InvalidExportOutputDirectory("Output directory cannot be a path to a file.")

    output_directory.mkdir(parents=True, exist_ok=True)
    file_path.write_text(file_contents)
