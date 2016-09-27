import sys
import os
from string import printable
from copy import deepcopy
from hypothesis import given
from hypothesis.strategies import text, lists, fixed_dictionaries

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
sys.path.insert(0, ROOT)

from tools.toolchains import TOOLCHAIN_CLASSES, LEGACY_TOOLCHAIN_NAMES
from tools.targets import TARGET_MAP

def test_instantiation():
    for name, Class in  TOOLCHAIN_CLASSES.items():
        CLS = Class(TARGET_MAP["K64F"])
        assert name == CLS.name or name ==  LEGACY_TOOLCHAIN_NAMES[CLS.name]

ALPHABET = [char for char in printable if char not in [u'.', u'/']]

@given(fixed_dictionaries({
    'common': lists(text()),
    'c': lists(text()),
    'cxx': lists(text()),
    'asm': lists(text()),
    'ld': lists(text())}),
       lists(text(min_size=1, alphabet=ALPHABET), min_size=1)
)
def test_toolchain_profile_c(profile, source_file):
    filename = deepcopy(source_file)
    filename[-1] += ".c"
    to_compile = os.path.join(*filename)
    for name, Class in TOOLCHAIN_CLASSES.items():
        CLS = Class(TARGET_MAP["K64F"], build_profile=profile)
        CLS.inc_md5 = ""
        CLS.build_dir = ""
        compile_command = CLS.compile_command(to_compile, to_compile + ".o", [])
        for parameter in profile['c'] + profile['common']:
            assert any(parameter in cmd for cmd in compile_command), \
                "Toolchain %s did not propigate arg %s" % (CLS.name, parameter)

@given(fixed_dictionaries({
    'common': lists(text()),
    'c': lists(text()),
    'cxx': lists(text()),
    'asm': lists(text()),
    'ld': lists(text())}),
       lists(text(min_size=1, alphabet=ALPHABET), min_size=1)
)
def test_toolchain_profile_cpp(profile, source_file):
    filename = deepcopy(source_file)
    filename[-1] += ".cpp"
    to_compile = os.path.join(*filename)
    for name, Class in TOOLCHAIN_CLASSES.items():
        CLS = Class(TARGET_MAP["K64F"], build_profile=profile)
        CLS.inc_md5 = ""
        CLS.build_dir = ""
        compile_command = CLS.compile_command(to_compile, to_compile + ".o", [])
        for parameter in profile['cxx'] + profile['common']:
            assert any(parameter in cmd for cmd in compile_command), \
                "Toolchain %s did not propigate arg %s" % (CLS.name, parameter)

@given(fixed_dictionaries({
    'common': lists(text()),
    'c': lists(text()),
    'cxx': lists(text()),
    'asm': lists(text()),
    'ld': lists(text())}),
       lists(text(min_size=1, alphabet=ALPHABET), min_size=1)
)
def test_toolchain_profile_asm(profile, source_file):
    filename = deepcopy(source_file)
    filename[-1] += ".s"
    to_compile = os.path.join(*filename)
    for name, Class in TOOLCHAIN_CLASSES.items():
        CLS = Class(TARGET_MAP["K64F"], build_profile=profile)
        CLS.inc_md5 = ""
        CLS.build_dir = ""
        compile_command = CLS.compile_command(to_compile, to_compile + ".o", [])
        if not compile_command:
            assert compile_command, to_compile
        for parameter in profile['asm']:
            assert any(parameter in cmd for cmd in compile_command), \
                "Toolchain %s did not propigate arg %s" % (CLS.name, parameter)

