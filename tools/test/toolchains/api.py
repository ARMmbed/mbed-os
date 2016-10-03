"""Tests for the toolchain sub-system"""
import sys
import os
from string import printable
from copy import deepcopy
from mock import MagicMock, patch
from hypothesis import given
from hypothesis.strategies import text, lists, fixed_dictionaries

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..",
                                    ".."))
sys.path.insert(0, ROOT)

from tools.toolchains import TOOLCHAIN_CLASSES, LEGACY_TOOLCHAIN_NAMES,\
    Resources
from tools.targets import TARGET_MAP

def test_instantiation():
    """Test that all exported toolchain may be instantiated"""
    for name, tc_class in  TOOLCHAIN_CLASSES.items():
        cls = tc_class(TARGET_MAP["K64F"])
        assert name == cls.name or\
            name == LEGACY_TOOLCHAIN_NAMES[cls.name]

ALPHABET = [char for char in printable if char not in [u'.', u'/']]

@given(fixed_dictionaries({
    'common': lists(text()),
    'c': lists(text()),
    'cxx': lists(text()),
    'asm': lists(text()),
    'ld': lists(text())}),
       lists(text(min_size=1, alphabet=ALPHABET), min_size=1))
def test_toolchain_profile_c(profile, source_file):
    """Test that the appropriate profile parameters are passed to the
    C compiler"""
    filename = deepcopy(source_file)
    filename[-1] += ".c"
    to_compile = os.path.join(*filename)
    with patch('os.mkdir') as _mkdir:
        for _, tc_class in TOOLCHAIN_CLASSES.items():
            toolchain = tc_class(TARGET_MAP["K64F"], build_profile=profile)
            toolchain.inc_md5 = ""
            toolchain.build_dir = ""
            compile_command = toolchain.compile_command(to_compile,
                                                        to_compile + ".o", [])
            for parameter in profile['c'] + profile['common']:
                assert any(parameter in cmd for cmd in compile_command), \
                    "Toolchain %s did not propigate arg %s" % (toolchain.name,
                                                            parameter)

@given(fixed_dictionaries({
    'common': lists(text()),
    'c': lists(text()),
    'cxx': lists(text()),
    'asm': lists(text()),
    'ld': lists(text())}),
       lists(text(min_size=1, alphabet=ALPHABET), min_size=1))
def test_toolchain_profile_cpp(profile, source_file):
    """Test that the appropriate profile parameters are passed to the
    C++ compiler"""
    filename = deepcopy(source_file)
    filename[-1] += ".cpp"
    to_compile = os.path.join(*filename)
    with patch('os.mkdir') as _mkdir:
        for _, tc_class in TOOLCHAIN_CLASSES.items():
            toolchain = tc_class(TARGET_MAP["K64F"], build_profile=profile)
            toolchain.inc_md5 = ""
            toolchain.build_dir = ""
            compile_command = toolchain.compile_command(to_compile,
                                                        to_compile + ".o", [])
            for parameter in profile['cxx'] + profile['common']:
                assert any(parameter in cmd for cmd in compile_command), \
                    "Toolchain %s did not propigate arg %s" % (toolchain.name,
                                                            parameter)

@given(fixed_dictionaries({
    'common': lists(text()),
    'c': lists(text()),
    'cxx': lists(text()),
    'asm': lists(text()),
    'ld': lists(text())}),
       lists(text(min_size=1, alphabet=ALPHABET), min_size=1))
def test_toolchain_profile_asm(profile, source_file):
    """Test that the appropriate profile parameters are passed to the
    Assembler"""
    filename = deepcopy(source_file)
    filename[-1] += ".s"
    to_compile = os.path.join(*filename)
    with patch('os.mkdir') as _mkdir:
        for _, tc_class in TOOLCHAIN_CLASSES.items():
            toolchain = tc_class(TARGET_MAP["K64F"], build_profile=profile)
            toolchain.inc_md5 = ""
            toolchain.build_dir = ""
            compile_command = toolchain.compile_command(to_compile,
                                                        to_compile + ".o", [])
            if not compile_command:
                assert compile_command, to_compile
            for parameter in profile['asm']:
                assert any(parameter in cmd for cmd in compile_command), \
                    "Toolchain %s did not propigate arg %s" % (toolchain.name,
                                                            parameter)

    for name, Class in  TOOLCHAIN_CLASSES.items():
        CLS = Class(TARGET_MAP["K64F"])
        assert name == CLS.name or name ==  LEGACY_TOOLCHAIN_NAMES[CLS.name]


@given(lists(text(alphabet=ALPHABET, min_size=1), min_size=1))
def test_detect_duplicates(filenames):
    c_sources = [os.path.join(name, "dupe.c") for name in filenames]
    s_sources = [os.path.join(name, "dupe.s") for name in filenames]
    cpp_sources = [os.path.join(name, "dupe.cpp") for name in filenames]
    with MagicMock() as notify:
        toolchain = TOOLCHAIN_CLASSES["ARM"](TARGET_MAP["K64F"], notify=notify)
        res = Resources()
        res.c_sources = c_sources
        res.s_sources = s_sources
        res.cpp_sources = cpp_sources
        assert res.detect_duplicates(toolchain) == 1,\
            "Not Enough duplicates found"

        _, (notification, _), _ = notify.mock_calls[1]
        assert "dupe.o" in notification["message"]
        assert "dupe.s" in notification["message"]
        assert "dupe.c" in notification["message"]
        assert "dupe.cpp" in notification["message"]
