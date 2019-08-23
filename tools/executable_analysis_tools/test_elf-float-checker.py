#!/usr/bin/env python3
# Copyright (c) 2019 Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0

"""Pytest for testing elf-float-checker."""

import importlib
import mock
import subprocess
from pathlib import Path


TARGET = importlib.import_module("elf-float-checker")

SYMBOL_TABLE_WITHOUT_FLOATS = (
    " Symbol table '.symtab' contains 2723 entries:\n"
    "Num:    Value  Size Type    Bind   Vis      Ndx Name\n"
    "  0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND \n"
    "  1: 000045fd    16 FUNC    GLOBAL HIDDEN     3 lp_ticker_clear_interrupt\n"
    "  2: 00004609    16 FUNC    GLOBAL HIDDEN     3 __aeabi_uwrite4\n"
    "  3: 00004615    16 FUNC    GLOBAL HIDDEN     3 lp_ticker_fire_interrupt\n"
    "  4: 00004625    36 FUNC    GLOBAL HIDDEN     3 lp_ticker_free\n"
    "  5: 00004645     8 FUNC    GLOBAL HIDDEN     3 lp_ticker_get_info\n"
    "  6: 0000464d   116 FUNC    GLOBAL HIDDEN     3 __aeabi_lasr\n"
    "  7: 000046bd    20 FUNC    GLOBAL HIDDEN     3 lp_ticker_irq_handler\n"
    "  8: 000046d1    16 FUNC    GLOBAL HIDDEN     3 lp_ticker_read\n"
    "  9: 000046e1    52 FUNC    GLOBAL HIDDEN     3 __aeabi_lmul\n"
)

FLOAT_SYMBOLS = [
    "__aeabi_cdcmpeq",
    "__aeabi_cfcmpeq",
    "__aeabi_f2iz",
    "__aeabi_h2f_alt",
    "__aeabi_i2d",
    "__aeabi_d2iz",
    "__aeabi_i2f",
]

SYMBOL_TABLE_WITH_FLOATS = (
    " Symbol table '.symtab' contains 2723 entries:\n"
    "Num:    Value  Size Type    Bind   Vis      Ndx Name\n"
    f"  0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND \n"
    f"  1: 000045fd    16 FUNC    GLOBAL HIDDEN     3 {FLOAT_SYMBOLS[0]}\n"
    f"  2: 00004609    16 FUNC    GLOBAL HIDDEN     3 lp_ticker_disable_interrupt\n"
    f"  3: 00004615    16 FUNC    GLOBAL HIDDEN     3 {FLOAT_SYMBOLS[1]}\n"
    f"  4: 00004625    36 FUNC    GLOBAL HIDDEN     3 {FLOAT_SYMBOLS[2]}\n"
    f"  5: 00004645     8 FUNC    GLOBAL HIDDEN     3 lp_ticker_get_info\n"
    f"  6: 0000464d   116 FUNC    GLOBAL HIDDEN     3 {FLOAT_SYMBOLS[3]}\n"
    f"  7: 000046bd    20 FUNC    GLOBAL HIDDEN     3 lp_ticker_irq_handler\n"
    f"  8: 000046d1    16 FUNC    GLOBAL HIDDEN     3 {FLOAT_SYMBOLS[4]}\n"
    f"  9: 000046e1    52 FUNC    GLOBAL HIDDEN     3 {FLOAT_SYMBOLS[5]}\n"
    f" 10: 000046f1    52 FUNC    GLOBAL HIDDEN     3 {FLOAT_SYMBOLS[6]}\n"
)


ELF_FORMAT_FILE = "mbed-os-example.elf"
OBJECT_FILE_ANALYSIS_CMD = [*(TARGET.OBJECT_FILE_ANALYSIS_CMD), f"{ELF_FORMAT_FILE}"]

class TestElfFloatChecker:
    """Test class"""

    @classmethod
    def setup_class(cls):
        # Create a dummy ELF format file
        Path(ELF_FORMAT_FILE).touch()

    @classmethod
    def teardown_class(cls):
        # Remove the dummy ELF format file
        Path(ELF_FORMAT_FILE).unlink()

    @mock.patch("subprocess.run")
    def test_correctly_detect_absence_of_float_symbols(
        self, mock_subprocess_run
    ):
        """Test that no false positive occur."""
        mock_subprocess_run.return_value = subprocess.CompletedProcess(
            args=(
                f"{OBJECT_FILE_ANALYSIS_CMD}"
                " check=True, stderr=-2, stdin=None, stdout=-1"
            ),
            returncode=0,
            stdout=SYMBOL_TABLE_WITHOUT_FLOATS.encode(),
            stderr=None,
        )
        assert [] == TARGET.check_float_symbols(ELF_FORMAT_FILE)
        mock_subprocess_run.assert_called_with(
            OBJECT_FILE_ANALYSIS_CMD,
            check=True,
            stdin=None,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )

    @mock.patch("subprocess.run")
    def test_correctly_detect_presence_of_float_symbols(
        self, mock_subprocess_run
    ):
        """Test that float symbols can be discovered in a symbol table."""
        mock_subprocess_run.return_value = subprocess.CompletedProcess(
            args=(
                f"{OBJECT_FILE_ANALYSIS_CMD}"
                " check=True, stderr=-2, stdin=None, stdout=-1"
            ),
            returncode=0,
            stdout=SYMBOL_TABLE_WITH_FLOATS.encode(),
            stderr=None,
        )
        assert FLOAT_SYMBOLS == TARGET.check_float_symbols(
            ELF_FORMAT_FILE
        )
        mock_subprocess_run.assert_called_with(
            OBJECT_FILE_ANALYSIS_CMD,
            check=True,
            stdin=None,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )
