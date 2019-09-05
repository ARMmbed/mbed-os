#!/usr/bin/env python
# Copyright (c) 2019 Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0

"""Script for checking for floating point symbols in ELF files."""

import argparse
import logging
import re
import subprocess
import sys
from enum import Enum

class ReturnCode(Enum):
    """Return codes."""

    SUCCESS = 0
    ERROR = 1
    INVALID_OPTIONS = 2


log = logging.getLogger(__name__)

LOG_FORMAT = "%(asctime)s - %(levelname)s - %(message)s"
FLOATING_POINT_SYMBOL_REGEX = r"__aeabi_(cd.+|cf.+|h2f.+|d.+|f.+|.+2d|.+2f)"
OBJECT_FILE_ANALYSIS_CMD = ["objdump", "-t"]

class SymbolParser:
    """Parse the given ELF format file."""

    def get_symbols_from_table(self, symbol_table, symbol_regex):
        """Get symbols matching a regular expression pattern from a table."""
        pattern = re.compile(symbol_regex, re.X)
        matched_symbols = []
        symbol_table_lines = symbol_table.split("\n")
        for symbol_table_line in symbol_table_lines:
            match = pattern.search(symbol_table_line)
            if match:
                log.debug("Symbol line: {}".format(symbol_table_line))
                log.debug("Match found: {}".format(match))
                matched_symbols.append(match.group(0))

        log.debug("Symbols found:\n'{}'".format(matched_symbols))
        return matched_symbols

    def get_symbol_table(self, elf_file):
        """Get the symbol table from an ELF format file."""
        log.debug(
            "Get the symbol table for ELF format file '{}'".format(elf_file)
        )

        cmd = [OBJECT_FILE_ANALYSIS_CMD[0], OBJECT_FILE_ANALYSIS_CMD[1], elf_file]
        log.debug("command: '{}'".format(cmd))
        try:
            process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        except OSError as error:
            raise SymbolTableError(
                "Getting symbol table for ELF format file '{}' failed,"
                " error: {}".format(elf_file, error)
            )

        stdout, _ = process.communicate()

        if process.returncode:
            raise SymbolTableError(
                "Getting symbol table for ELF format file '{}' failed,"
                " error: {}".format(elf_file, stdout.decode())
            )

        symbol_table = stdout.decode()

        log.debug("Symbol table:\n{}\n".format(symbol_table))

        return symbol_table


class SymbolTableError(Exception):
    """An exception for a failure to obtain a symbol table."""


class FloatSymbolsFound(Exception):
    """An exception generated when floating point symbols are found."""

class ArgumentParserWithDefaultHelp(argparse.ArgumentParser):
    """Subclass that always shows the help message on invalid arguments."""

    def error(self, message):
        """Error handler."""
        sys.stderr.write("error: {}\n".format(message))
        self.print_help()
        raise SystemExit(ReturnCode.INVALID_OPTIONS.value)


def set_log_verbosity(increase_verbosity):
    """Set the verbosity of the log output."""
    log_level = logging.DEBUG if increase_verbosity else logging.INFO

    log.setLevel(log_level)
    logging.basicConfig(level=log_level, format=LOG_FORMAT)


def check_float_symbols(elf_file):
    """Check for floating point symbols in ELF format file.

    Return the floating point symbols found.
    """
    print("Checking {} for floating point symbols".format(elf_file))
    parser = SymbolParser()
    symbol_table = parser.get_symbol_table(elf_file)

    float_symbols = parser.get_symbols_from_table(
        symbol_table, FLOATING_POINT_SYMBOL_REGEX
    )

    return float_symbols


def check_action(args):
    """Entry point for checking the ELF file."""
    float_symbols = check_float_symbols(args.elf_file)
    if float_symbols:
        print("Failed - Found float symbols:")
        for float_symbol in float_symbols:
            print(float_symbol)
        raise FloatSymbolsFound("Found float symbols in {}".format(args.elf_file))
    else:
        print("Passed - No float symbols found.")


def parse_args():
    """Parse the command line args."""
    parser = ArgumentParserWithDefaultHelp(
        description="ELF floats checker",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )

    parser.add_argument(
        "elf_file",
        type=str,
        help=(
            "the Executable and Linkable Format (ELF) file to check"
            " for floating point instruction inclusion."
        ),
    )

    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="increase verbosity of status information.",
    )

    parser.set_defaults(func=check_action)

    args_namespace = parser.parse_args()

    # We want to fail gracefully, with a consistent
    # help message, in the no argument case.
    # So here's an obligatory hasattr hack.
    if not hasattr(args_namespace, "func"):
        parser.error("No arguments given!")
    else:
        return args_namespace


def run_elf_floats_checker():
    """Application main algorithm."""
    args = parse_args()

    set_log_verbosity(args.verbose)

    log.debug("Starting elf-floats-checker")
    log.debug("Command line arguments:{}".format(args))

    args.func(args)


def _main():
    """Run elf-floats-checker."""
    try:
        run_elf_floats_checker()
    except Exception as error:
        print(error)
        return ReturnCode.ERROR.value
    else:
        return ReturnCode.SUCCESS.value

if __name__ == "__main__":
    sys.exit(_main())
