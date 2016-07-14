import sys
import os

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
sys.path.insert(0, ROOT)

from tools.toolchains import TOOLCHAIN_CLASSES, LEGACY_TOOLCHAIN_NAMES
from tools.targets import TARGET_MAP

def test_instantiation():
    for name, Class in  TOOLCHAIN_CLASSES.items():
        CLS = Class(TARGET_MAP["K64F"])
        assert name == CLS.name or name ==  LEGACY_TOOLCHAIN_NAMES[CLS.name]
