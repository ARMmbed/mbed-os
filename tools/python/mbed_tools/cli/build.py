#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Command to build/compile an Mbed project using CMake."""
import os
import pathlib
import shutil

from typing import Optional, Tuple

import click

from mbed_tools.build import build_project, generate_build_system, generate_config, flash_binary
from mbed_tools.devices import find_connected_device, find_all_connected_devices
from mbed_tools.project import MbedProgram
from mbed_tools.sterm import terminal


@click.command(name="compile", help="Build an Mbed project.")
@click.option(
    "-t",
    "--toolchain",
    type=click.Choice(["ARM", "GCC_ARM"], case_sensitive=False),
    required=True,
    help="The toolchain you are using to build your app.",
)
@click.option("-m", "--mbed-target", required=True, help="A build target for an Mbed-enabled device, e.g. K64F.")
@click.option("-b", "--profile", default="develop", help="The build type (release, develop or debug).")
@click.option("-c", "--clean", is_flag=True, default=False, help="Perform a clean build.")
@click.option(
    "-p",
    "--program-path",
    default=os.getcwd(),
    help="Path to local Mbed program. By default it is the current working directory.",
)
@click.option(
    "--mbed-os-path", type=click.Path(), default=None, help="Path to local Mbed OS directory.",
)
@click.option(
    "--custom-targets-json", type=click.Path(), default=None, help="Path to custom_targets.json.",
)
@click.option(
    "--app-config", type=click.Path(), default=None, help="Path to application configuration file.",
)
@click.option(
    "-f", "--flash", is_flag=True, default=False, help="Flash the binary onto a device",
)
@click.option(
    "-s", "--sterm", is_flag=True, default=False, help="Launch a serial terminal to the device.",
)
@click.option(
    "--baudrate",
    default=9600,
    show_default=True,
    help="Change the serial baud rate (ignored unless --sterm is also given).",
)
def build(
    program_path: str,
    profile: str,
    toolchain: str,
    mbed_target: str,
    clean: bool,
    flash: bool,
    sterm: bool,
    baudrate: int,
    mbed_os_path: str,
    custom_targets_json: str,
    app_config: str,
) -> None:
    """Configure and build an Mbed project using CMake and Ninja.

    If the CMake configuration step has already been run previously (i.e a CMake build tree exists), then just try to
    build the project immediately using Ninja.

    Args:
       program_path: Path to the Mbed project.
       mbed_os_path: The path to the local Mbed OS directory.
       profile: The Mbed build profile (debug, develop or release).
       custom_targets_json: Path to custom_targets.json.
       toolchain: The toolchain to use for the build.
       mbed_target: The name of the Mbed target to build for.
       app_config: the path to the application configuration file
       clean: Perform a clean build.
       flash: Flash the binary onto a device.
       sterm: Open a serial terminal to the connected target.
       baudrate: Change the serial baud rate (ignored unless --sterm is also given).
    """
    mbed_target, target_id = _get_target_id(mbed_target)

    cmake_build_subdir = pathlib.Path(mbed_target.upper(), profile.lower(), toolchain.upper())
    if mbed_os_path is None:
        program = MbedProgram.from_existing(pathlib.Path(program_path), cmake_build_subdir)
    else:
        program = MbedProgram.from_existing(pathlib.Path(program_path), cmake_build_subdir, pathlib.Path(mbed_os_path))
    build_tree = program.files.cmake_build_dir
    if clean and build_tree.exists():
        shutil.rmtree(build_tree)

    click.echo("Configuring project and generating build system...")
    if custom_targets_json is not None:
        program.files.custom_targets_json = pathlib.Path(custom_targets_json)
    if app_config is not None:
        program.files.app_config_file = pathlib.Path(app_config)
    config, _ = generate_config(mbed_target.upper(), toolchain, program)
    generate_build_system(program.root, build_tree, profile)

    click.echo("Building Mbed project...")
    build_project(build_tree)

    if flash or sterm:
        if target_id is not None or sterm:
            devices = [find_connected_device(mbed_target, target_id)]
        else:
            devices = find_all_connected_devices(mbed_target)

    if flash:
        for dev in devices:
            hex_file = "OUTPUT_EXT" in config and config["OUTPUT_EXT"] == "hex"
            flashed_path = flash_binary(dev.mount_points[0].resolve(), program.root, build_tree, mbed_target, hex_file)
        click.echo(f"Copied {str(flashed_path.resolve())} to {len(devices)} device(s).")

    if sterm:
        dev = devices[0]
        if dev.serial_port is None:
            raise click.ClickException(
                f"The connected device {dev.mbed_board.board_name} does not have an associated serial port."
                " Reconnect the device and try again."
            )

        terminal.run(dev.serial_port, baudrate)


def _get_target_id(target: str) -> Tuple[str, Optional[int]]:
    if "[" in target:
        target_name, target_id = target.replace("]", "").split("[", maxsplit=1)
        if target_id.isdigit() and int(target_id) >= 0:
            return (target_name, int(target_id))
        raise click.ClickException("When using the format mbed-target[ID], ID must be a positive integer or 0.")
    return (target, None)
