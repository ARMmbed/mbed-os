#!/usr/bin/env python3
# Copyright (c) 2019 Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0

"""Pytest for testing elf-float-checker."""

import importlib
import mock
import os
import subprocess


TARGET = importlib.import_module("elf_float_checker")

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
    " Symbol table '.symtab' contains 2723 entries:\n"+
    "Num:    Value  Size Type    Bind   Vis      Ndx Name\n"+
    "  0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND \n"+
    "  1: 000045fd    16 FUNC    GLOBAL HIDDEN     3 {}\n".format(FLOAT_SYMBOLS[0])+
    "  2: 00004609    16 FUNC    GLOBAL HIDDEN     3 lp_ticker_disable_interrupt\n"+
    "  3: 00004615    16 FUNC    GLOBAL HIDDEN     3 {}\n".format(FLOAT_SYMBOLS[1])+
    "  4: 00004625    36 FUNC    GLOBAL HIDDEN     3 {}\n".format(FLOAT_SYMBOLS[2])+
    "  5: 00004645     8 FUNC    GLOBAL HIDDEN     3 lp_ticker_get_info\n"+
    "  6: 0000464d   116 FUNC    GLOBAL HIDDEN     3 {}\n".format(FLOAT_SYMBOLS[3])+
    "  7: 000046bd    20 FUNC    GLOBAL HIDDEN     3 lp_ticker_irq_handler\n"+
    "  8: 000046d1    16 FUNC    GLOBAL HIDDEN     3 {}\n".format(FLOAT_SYMBOLS[4])+
    "  9: 000046e1    52 FUNC    GLOBAL HIDDEN     3 {}\n".format(FLOAT_SYMBOLS[5])+
    " 10: 000046f1    52 FUNC    GLOBAL HIDDEN     3 {}\n".format(FLOAT_SYMBOLS[6])
)


ELF_FORMAT_FILE = "mbed-os-example.elf"
OBJECT_FILE_ANALYSIS_CMD = [
    TARGET.OBJECT_FILE_ANALYSIS_CMD[0],
    TARGET.OBJECT_FILE_ANALYSIS_CMD[1],
    "{}".format(ELF_FORMAT_FILE)
]

class TestElfFloatChecker:
    """Test class"""

    @classmethod
    def setup_class(cls):
        # Create a dummy ELF format file
        if not os.path.exists(ELF_FORMAT_FILE):
            with open(ELF_FORMAT_FILE, 'w'): pass

    @classmethod
    def teardown_class(cls):
        # Remove the dummy ELF format file
        if os.path.exists(ELF_FORMAT_FILE):
            os.remove(ELF_FORMAT_FILE)

    @mock.patch("subprocess.Popen")
    def test_correctly_detect_absence_of_float_symbols(
        self, mock_subprocess_popen
    ):
        """Test that no false positive occur."""
        process_mock = mock.Mock()
        attrs = {
            "communicate.return_value":(
                SYMBOL_TABLE_WITHOUT_FLOATS.encode(), None
            ),
            "returncode": 0,
        }
        process_mock.configure_mock(**attrs)
        mock_subprocess_popen.return_value = process_mock
        assert [] == TARGET.check_float_symbols(ELF_FORMAT_FILE)
        mock_subprocess_popen.assert_called_with(
            OBJECT_FILE_ANALYSIS_CMD,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )

    @mock.patch("subprocess.Popen")
    def test_correctly_detect_presence_of_float_symbols(
        self, mock_subprocess_popen
    ):
        """Test that float symbols can be discovered in a symbol table."""
        process_mock = mock.Mock()
        attrs = {
            "communicate.return_value":(
                SYMBOL_TABLE_WITH_FLOATS.encode(), None
            ),
            "returncode": 0,
        }
        process_mock.configure_mock(**attrs)
        mock_subprocess_popen.return_value = process_mock
        assert FLOAT_SYMBOLS == TARGET.check_float_symbols(
            ELF_FORMAT_FILE
        )
        mock_subprocess_popen.assert_called_with(
            OBJECT_FILE_ANALYSIS_CMD,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )
