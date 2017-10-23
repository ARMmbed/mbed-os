"""Tests for the arm toolchain supported checks"""
import sys
import os
from string import printable
from copy import deepcopy
from mock import MagicMock, patch
from hypothesis import given, settings
from hypothesis.strategies import text, lists, sampled_from

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..",
                                    ".."))
sys.path.insert(0, ROOT)

from tools.toolchains.arm import ARM_STD, ARM_MICRO, ARMC6
from tools.utils import NotSupportedException

ARMC5_CORES = ["Cortex-M0", "Cortex-M0+", "Cortex-M3", "Cortex-M4",
               "Cortex-M4F", "Cortex-M7", "Cortex-M7F", "Cortex-M7FD"]
ARMC6_CORES = ARMC5_CORES + ["Cortex-M23", "Cortex-M23-NS",
                             "Cortex-M33", "CortexM33-NS"]

CORE_SUF_ALPHA = ["MDFNS02347-+"]

@given(lists(sampled_from(["ARM", "uARM", "GCC_ARM", "ARMC6", "IAR", "GARBAGE"])),
       text(alphabet=CORE_SUF_ALPHA))
def test_arm_std(supported_toolchains, core):
    mock_target = MagicMock()
    mock_target.core = "Cortex-" + core
    mock_target.supported_toolchains = supported_toolchains
    try:
        ARM_STD(mock_target)
        assert "ARM" in supported_toolchains
        assert mock_target.core in ARMC5_CORES
    except NotSupportedException:
        assert "ARM" not in supported_toolchains or mock_target.core not in ARMC5_CORES


@given(lists(sampled_from(["ARM", "uARM", "GCC_ARM", "ARMC6", "IAR", "GARBAGE"])),
       text(alphabet=CORE_SUF_ALPHA))
def test_arm_micro(supported_toolchains, core):
    mock_target = MagicMock()
    mock_target.core = "Cortex-" + core
    mock_target.supported_toolchains = supported_toolchains
    try:
        ARM_MICRO(mock_target)
        assert "ARM" in supported_toolchains or "uARM" in supported_toolchains
        assert mock_target.core in ARMC5_CORES
    except NotSupportedException:
        assert ("ARM" not in supported_toolchains and "uARM" not in supported_toolchains)\
            or mock_target.core not in ARMC5_CORES


@given(lists(sampled_from(["ARM", "uARM", "GCC_ARM", "ARMC6", "IAR", "GARBAGE"])),
       text(alphabet=CORE_SUF_ALPHA))
def test_armc6(supported_toolchains, core):
    mock_target = MagicMock()
    mock_target.core = "Cortex-" + core
    mock_target.supported_toolchains = supported_toolchains
    try:
        ARMC6(mock_target)
        assert "ARM" in supported_toolchains or "ARMC6" in supported_toolchains
        assert mock_target.core in ARMC6_CORES
    except NotSupportedException:
        assert ("ARM" not in supported_toolchains and "ARMC6" not in supported_toolchains)\
            or mock_target.core not in ARMC6_CORES
