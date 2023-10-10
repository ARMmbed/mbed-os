#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Helpers for logging errors according to severity of the exception."""
from typing import Type, Optional, cast
from types import TracebackType
import logging
from mbed_tools.lib.exceptions import ToolsError

LOGGING_FORMAT = "%(levelname)s: %(message)s"

VERBOSITY_HELP = {
    logging.CRITICAL: "-v",
    logging.ERROR: "-v",
    logging.WARNING: "-vv",
    logging.INFO: "-vvv",
    logging.DEBUG: "--traceback",
}


def _exception_message(err: BaseException, log_level: int, traceback: bool) -> str:
    """Generate a user facing message with help on how to get more information from the logs."""
    error_msg = str(err)
    if log_level != logging.DEBUG or not traceback:
        cli_option = VERBOSITY_HELP.get(log_level, "-v")
        error_msg += f"\n\nMore information may be available by using the command line option '{cli_option}'."
    return error_msg


class MbedToolsHandler:
    """Context Manager to catch Mbed Tools exceptions and generate a helpful user facing message."""

    def __init__(self, logger: logging.Logger, traceback: bool = False):
        """Keep track of the logger to use and whether or not a traceback should be generated."""
        self._logger = logger
        self._traceback = traceback
        self.exit_code = 0

    def __enter__(self) -> "MbedToolsHandler":
        """Return the Context Manager."""
        return self

    def __exit__(
        self,
        exc_type: Optional[Type[BaseException]],
        exc_value: Optional[BaseException],
        exc_traceback: Optional[TracebackType],
    ) -> bool:
        """Handle any raised exceptions, suppressing Tools errors and generating an error message instead."""
        if exc_type and issubclass(exc_type, ToolsError):
            error_msg = _exception_message(cast(BaseException, exc_value), logging.root.level, self._traceback)
            self._logger.error(error_msg, exc_info=self._traceback)
            # Do not propagate exceptions derived from ToolsError
            self.exit_code = 1
            return True

        # Propagate all other exceptions
        return False


def log_exception(logger: logging.Logger, exception: Exception, show_traceback: bool = False) -> None:
    """Logs an exception in both normal and verbose forms.

    Args:
        logger: logger
        exception: exception to log
        show_traceback: show the full traceback.
    """
    logger.error(exception, exc_info=show_traceback)


def set_log_level(verbose_count: int) -> None:
    """Sets the log level.

    Args:
        verbose_count: number of `-v` flags used
    """
    if verbose_count > 2:
        log_level = logging.DEBUG
    elif verbose_count == 2:
        log_level = logging.INFO
    elif verbose_count == 1:
        log_level = logging.WARNING
    else:
        log_level = logging.ERROR
    logging.basicConfig(level=log_level, format=LOGGING_FORMAT)
