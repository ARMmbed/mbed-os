#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Main cli entry point."""
import logging
import sys

from typing import Union, Any

import click

from mbed_tools.lib.logging import set_log_level, MbedToolsHandler

from mbed_tools.cli.configure import configure
from mbed_tools.cli.list_connected_devices import list_connected_devices
from mbed_tools.cli.project_management import new, import_, deploy
from mbed_tools.cli.build import build
from mbed_tools.cli.sterm import sterm

CONTEXT_SETTINGS = dict(help_option_names=["-h", "--help"])
LOGGER = logging.getLogger(__name__)


class GroupWithExceptionHandling(click.Group):
    """A click.Group which handles ToolsErrors and logging."""

    def invoke(self, context: click.Context) -> None:
        """Invoke the command group.

        Args:
            context: The current click context.
        """
        # Use the context manager to ensure tools exceptions (expected behaviour) are shown as messages to the user,
        # but all other exceptions (unexpected behaviour) are shown as errors.
        with MbedToolsHandler(LOGGER, context.params["traceback"]) as handler:
            super().invoke(context)

        sys.exit(handler.exit_code)


def print_version(context: click.Context, param: Union[click.Option, click.Parameter], value: bool) -> Any:
    """Print the version of mbed-tools."""
    if not value or context.resilient_parsing:
        return

    # Mbed CE: changed this to be hardcoded for now.
    version_string = "7.60.0"
    click.echo(version_string)
    context.exit()


@click.group(cls=GroupWithExceptionHandling, context_settings=CONTEXT_SETTINGS)
@click.option(
    "--version",
    is_flag=True,
    callback=print_version,
    expose_value=False,
    is_eager=True,
    help="Display versions of all Mbed Tools packages.",
)
@click.option(
    "-v",
    "--verbose",
    default=0,
    count=True,
    help="Set the verbosity level, enter multiple times to increase verbosity.",
)
@click.option("-t", "--traceback", is_flag=True, show_default=True, help="Show a traceback when an error is raised.")
def cli(verbose: int, traceback: bool) -> None:
    """Command line tool for interacting with Mbed OS."""
    set_log_level(verbose)


cli.add_command(configure, "configure")
cli.add_command(list_connected_devices, "detect")
cli.add_command(new, "new")
cli.add_command(deploy, "deploy")
cli.add_command(import_, "import")
cli.add_command(build, "compile")
cli.add_command(sterm, "sterm")

if __name__ == '__main__':
    cli()
