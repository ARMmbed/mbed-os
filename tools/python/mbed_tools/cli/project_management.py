#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Project management commands: new, import_, deploy and libs."""
import os
import pathlib

from typing import Any, List

import click
import tabulate

from mbed_tools.project import initialise_project, import_project, get_known_libs, deploy_project
from mbed_tools.project._internal import git_utils


@click.command()
@click.option("--create-only", "-c", is_flag=True, show_default=True, help="Create a program without fetching mbed-os.")
@click.argument("path", type=click.Path(resolve_path=True))
def new(path: str, create_only: bool) -> None:
    """Creates a new Mbed project at the specified path. Downloads mbed-os and adds it to the project.

    PATH: Path to the destination directory for the project. Will be created if it does not exist.
    """
    click.echo(f"Creating a new Mbed program at path '{path}'.")
    if not create_only:
        click.echo("Downloading mbed-os and adding it to the project.")

    initialise_project(pathlib.Path(path), create_only)


@click.command()
@click.argument("url")
@click.argument("path", type=click.Path(), default="")
@click.option(
    "--skip-resolve-libs",
    "-s",
    is_flag=True,
    show_default=True,
    help="Skip resolving program library dependencies after cloning.",
)
def import_(url: str, path: Any, skip_resolve_libs: bool) -> None:
    """Clone an Mbed project and library dependencies.

    URL: The git url of the remote project to clone.

    PATH: Destination path for the clone. If not given the destination path is set to the project name in the cwd.
    """
    click.echo(f"Cloning Mbed program '{url}'")
    if not skip_resolve_libs:
        click.echo("Resolving program library dependencies.")

    if path:
        click.echo(f"Destination path is '{path}'")
        path = pathlib.Path(path)

    dst_path = import_project(url, path, not skip_resolve_libs)
    if not skip_resolve_libs:
        libs = get_known_libs(dst_path)
        _print_dependency_table(libs)


@click.command()
@click.argument("path", type=click.Path(), default=os.getcwd())
@click.option(
    "--force",
    "-f",
    is_flag=True,
    show_default=True,
    help="Forces checkout of all library repositories at specified commit in the .lib file, overwrites local changes.",
)
def deploy(path: str, force: bool) -> None:
    """Checks out Mbed program library dependencies at the revision specified in the ".lib" files.

    Ensures all dependencies are resolved and the versions are synchronised to the version specified in the library
    reference.

    PATH: Path to the Mbed project [default: CWD]
    """
    click.echo("Checking out all libraries to revisions specified in .lib files. Resolving any unresolved libraries.")
    root_path = pathlib.Path(path)
    deploy_project(root_path, force)
    libs = get_known_libs(root_path)
    _print_dependency_table(libs)


def _print_dependency_table(libs: List) -> None:
    click.echo("The following library dependencies were fetched: \n")
    table = []
    for lib in libs:
        table.append(
            [
                lib.reference_file.stem,
                lib.get_git_reference().repo_url,
                lib.source_code_path,
                git_utils.get_default_branch(git_utils.get_repo(lib.source_code_path))
                if not lib.get_git_reference().ref
                else lib.get_git_reference().ref,
            ]
        )

    headers = ("Library Name", "Repository URL", "Path", "Git Reference")
    click.echo(tabulate.tabulate(table, headers=headers))
