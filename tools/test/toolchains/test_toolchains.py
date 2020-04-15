#!/usr/bin/env python
# Copyright (c) 2019 Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0

"""Test the arm toolchain."""

import os
import sys
from unittest import TestCase

import mock


ROOT = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "..", "..", "..")
)
sys.path.insert(0, ROOT)

from tools.toolchains.arm import ARM_STD, ARM_MICRO, ARMC6
from tools.toolchains.gcc import GCC_ARM
from tools.toolchains.iar import IAR
from tools.toolchains.mbed_toolchain import UNSUPPORTED_C_LIB_EXCEPTION_STRING
from tools.utils import NotSupportedException

class TestArmToolchain(TestCase):
    """Test Arm classes."""

    def test_arm_minimal_printf(self):
        """Test that linker flags are correctly added to an instance of ARM."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.printf_lib = "minimal-printf"
        mock_target.c_lib = "std"
        mock_target.supported_c_libs = {"arm": ["std"]}
        mock_target.supported_toolchains = ["ARM", "uARM", "ARMC5"]
        del mock_target.default_lib
        arm_std_obj = ARM_STD(mock_target)
        arm_micro_obj = ARM_MICRO(mock_target)
        arm_c6_obj = ARMC6(mock_target)

        self.assertIn("-DMBED_MINIMAL_PRINTF", arm_std_obj.flags["common"])
        self.assertIn("-DMBED_MINIMAL_PRINTF", arm_micro_obj.flags["common"])
        self.assertIn("-DMBED_MINIMAL_PRINTF", arm_c6_obj.flags["common"])

    def test_arm_c_lib(self):
        """Test that linker flags are correctly added to an instance of ARM."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.supported_c_libs = {"arm": ["small"]}
        mock_target.c_lib = "sMALL"
        del mock_target.default_lib
        mock_target.default_toolchain = "ARM"
        mock_target.supported_toolchains = ["ARM", "uARM", "ARMC5", "ARMC6"]
        arm_std_obj = ARM_STD(mock_target)
        arm_micro_obj = ARM_MICRO(mock_target)

        mock_target.default_toolchain = "ARMC6"
        arm_c6_obj = ARMC6(mock_target)

        self.assertIn("-D__MICROLIB", arm_std_obj.flags["common"])
        self.assertIn("-D__MICROLIB", arm_micro_obj.flags["common"])
        self.assertIn("-D__MICROLIB", arm_c6_obj.flags["common"])

        self.assertIn("--library_type=microlib", arm_std_obj.flags["ld"])
        self.assertIn("--library_type=microlib", arm_micro_obj.flags["ld"])
        self.assertIn("--library_type=microlib", arm_c6_obj.flags["ld"])        
        self.assertIn("--library_type=microlib", arm_c6_obj.flags["asm"])

    def test_arm_c_lib_std_exception(self):
        """Test that an exception is raised if the std C library is not supported for a target on the ARM toolchain."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.supported_toolchains = ["ARM", "uARM", "ARMC5"]
        mock_target.default_toolchain = "ARM"
        mock_target.c_lib = "std"
        del mock_target.default_lib
        mock_target.supported_c_libs = {"arm": ["small"]}
        with self.assertRaisesRegexp(NotSupportedException, UNSUPPORTED_C_LIB_EXCEPTION_STRING.format(mock_target.c_lib)):
            ARM_STD(mock_target)
        with self.assertRaisesRegexp(NotSupportedException, UNSUPPORTED_C_LIB_EXCEPTION_STRING.format(mock_target.c_lib)):
            ARMC6(mock_target)


    def test_arm_c_lib_small_exception(self):
        """Test that an exception is raised if the small and std C library are not supported for a target on the ARM toolchain."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.c_lib = "small"
        del mock_target.default_lib
        mock_target.supported_c_libs = {"arm": [""]}
        mock_target.default_toolchain = "ARM"
        mock_target.supported_toolchains = ["ARM", "uARM", "ARMC5"]
        with self.assertRaisesRegexp(NotSupportedException, UNSUPPORTED_C_LIB_EXCEPTION_STRING.format(mock_target.c_lib)):
            ARM_STD(mock_target)
        mock_target.default_toolchain = "ARMC6"
        with self.assertRaisesRegexp(NotSupportedException, UNSUPPORTED_C_LIB_EXCEPTION_STRING.format(mock_target.c_lib)):
            ARMC6(mock_target)

    def test_arm_small_c_lib_swap_std_lib(self):
        """Test that no exception is raised when small c lib is not supported but std lib is supported."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.c_lib = "small"
        del mock_target.default_lib
        mock_target.supported_c_libs = {"arm": ["std"]}
        mock_target.supported_toolchains = ["ARM", "uARM", "ARMC5"]

        mock_target.default_toolchain = "ARM"
        try:
            ARM_STD(mock_target)
        except NotSupportedException:
            self.fail(UNSUPPORTED_C_LIB_EXCEPTION_STRING.format(mock_target.c_lib))

        mock_target.default_toolchain = "ARMC6"
        try:
            ARMC6(mock_target)
        except NotSupportedException:
            self.fail(UNSUPPORTED_C_LIB_EXCEPTION_STRING.format(mock_target.c_lib))

class TestGccToolchain(TestCase):
    """Test the GCC class."""

    def test_gcc_minimal_printf(self):
        """Test that linker flags are correctly added to an instance of GCC_ARM."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.printf_lib = "minimal-printf"
        mock_target.supported_toolchains = ["GCC_ARM"]
        mock_target.c_lib = "std"
        del mock_target.default_lib
        mock_target.supported_c_libs = {"gcc_arm": ["std"]}
        mock_target.is_TrustZone_secure_target = False

        gcc_obj = GCC_ARM(mock_target)

        self.assertIn("-DMBED_MINIMAL_PRINTF", gcc_obj.flags["common"])

        minimal_printf_wraps = [
            "-Wl,--wrap,printf",
            "-Wl,--wrap,sprintf",
            "-Wl,--wrap,snprintf",
            "-Wl,--wrap,vprintf",
            "-Wl,--wrap,vsprintf",
            "-Wl,--wrap,vsnprintf",
            "-Wl,--wrap,fprintf",
            "-Wl,--wrap,vfprintf",
        ]

        for i in minimal_printf_wraps:
            self.assertIn(i, gcc_obj.flags["ld"])

    def test_gcc_arm_c_lib(self):
        """Test that linker flags are correctly added to an instance of GCC_ARM."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.supported_c_libs = {"gcc_arm": ["small"]}
        mock_target.c_lib = "sMALL"
        del mock_target.default_lib
        mock_target.supported_toolchains = ["GCC_ARM"]
        mock_target.is_TrustZone_secure_target = False
        gcc_arm_obj = GCC_ARM(mock_target)
        self.assertIn("-DMBED_RTOS_SINGLE_THREAD", gcc_arm_obj.flags["common"])
        self.assertIn("-D__NEWLIB_NANO", gcc_arm_obj.flags["common"])
        self.assertIn("--specs=nano.specs", gcc_arm_obj.flags["ld"])

    def test_gcc_arm_c_lib_std_exception(self):
        """Test that an exception is raised if the std C library is not supported for a target on the GCC_ARM toolchain."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.default_toolchain = "GCC_ARM"
        mock_target.c_lib = "std"
        del mock_target.default_lib
        mock_target.supported_c_libs = {"gcc_arm": ["small"]}
        with self.assertRaisesRegexp(NotSupportedException, UNSUPPORTED_C_LIB_EXCEPTION_STRING.format(mock_target.c_lib)):
            GCC_ARM(mock_target)

    def test_gcc_arm_c_lib_small_exception(self):
        """Test that an exception is raised if the small and std C library are not supported for a target on the GCC_ARM toolchain."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.c_lib = "small"
        del mock_target.default_lib
        mock_target.supported_c_libs = {"gcc_arm": [""]}
        mock_target.default_toolchain = "GCC_ARM"
        mock_target.supported_toolchains = ["GCC_ARM"]
        with self.assertRaisesRegexp(NotSupportedException, UNSUPPORTED_C_LIB_EXCEPTION_STRING.format(mock_target.c_lib)):
            GCC_ARM(mock_target)

    def test_gcc_arm_small_c_lib_swap_std_lib(self):
        """Test that no exception is raised when small c lib is not supported but std lib is supported."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.supported_c_libs = {"gcc_arm": ["std"]}
        mock_target.c_lib = "small"
        del mock_target.default_lib
        mock_target.supported_toolchains = ["GCC_ARM"]
        mock_target.is_TrustZone_secure_target = False
        mock_target.default_toolchain = "GCC_ARM"
        try:
            GCC_ARM(mock_target)
        except NotSupportedException:
            self.fail(UNSUPPORTED_C_LIB_EXCEPTION_STRING.format(mock_target.c_lib))
class TestIarToolchain(TestCase):
    """Test the IAR class."""

    def test_iar_minimal_printf(self):
        """Test that linker flags are correctly added to an instance of IAR."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.printf_lib = "minimal-printf"
        mock_target.supported_toolchains = ["IAR"]
        del mock_target.default_lib
        mock_target.c_lib = "std"
        mock_target.supported_c_libs = {"iar": ["std"]}
        mock_target.is_TrustZone_secure_target = False

        iar_obj = IAR(mock_target)
        var = "-DMBED_MINIMAL_PRINTF"
        self.assertIn("-DMBED_MINIMAL_PRINTF", iar_obj.flags["common"])

    def test_iar_c_lib(self):
        """Test that no exception is raised when a supported c library is specified."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.supported_c_libs = {"iar": ["std"]}
        mock_target.c_lib = "sTD"
        del mock_target.default_lib
        mock_target.supported_toolchains = ["IAR"]
        mock_target.is_TrustZone_secure_target = False
        try:
            IAR(mock_target)
        except NotSupportedException:
            self.fail(UNSUPPORTED_C_LIB_EXCEPTION_STRING.format(mock_target.c_lib))

    def test_iar_c_lib_std_exception(self):
        """Test that an exception is raised if the std C library is not supported for a target on the IAR toolchain."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.c_lib = "std"
        del mock_target.default_lib
        mock_target.supported_c_libs = {"iar": ["small"]}
        mock_target.supported_toolchains = ["IAR"]
        with self.assertRaisesRegexp(NotSupportedException, UNSUPPORTED_C_LIB_EXCEPTION_STRING.format(mock_target.c_lib)):
            IAR(mock_target)

    def test_iar_c_lib_small_exception(self):
        """Test that an exception is raised if the small and std C library are not supported for a target on the IAR toolchain."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.c_lib = "small"
        del mock_target.default_lib
        mock_target.supported_c_libs = {"iar": [""]}
        mock_target.supported_toolchains = ["IAR"]
        with self.assertRaisesRegexp(NotSupportedException, UNSUPPORTED_C_LIB_EXCEPTION_STRING.format(mock_target.c_lib)):
            IAR(mock_target)

    def test_iar_small_c_lib_swap_std_lib(self):
        """Test that no exception is raised when small c lib is not supported but std lib is supported."""
        mock_target = mock.MagicMock()
        mock_target.core = "Cortex-M4"
        mock_target.supported_c_libs = {"iar": ["std"]}
        mock_target.c_lib = "small"
        del mock_target.default_lib
        mock_target.supported_toolchains = ["IAR"]
        mock_target.is_TrustZone_secure_target = False
        try:
            IAR(mock_target)
        except NotSupportedException:
            self.fail(UNSUPPORTED_C_LIB_EXCEPTION_STRING.format(mock_target.c_lib))
