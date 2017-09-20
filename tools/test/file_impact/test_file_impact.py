from __future__ import print_function

import pytest
import json

from os.path import join, dirname
from StringIO import StringIO
from mock import patch
from jsonschema import validate

import tools.file_impact
from tools.config import ConfigException
from tools.toolchains import Resources
from tools.targets import TARGET_MAP
from tools.toolchains import TOOLCHAIN_CLASSES

def test_false_should_be_built():
    with patch('tools.file_impact.prepare_toolchain') as pt,\
         patch('tools.file_impact.scan_resources') as sr:
        sr.return_value = Resources()
        assert not tools.file_impact.should_be_built("K64F", "GCC_ARM", "not/present")

def test_true_should_be_built():
    with patch('tools.file_impact.prepare_toolchain') as pt,\
         patch('tools.file_impact.scan_resources') as sr:
        sr.return_value = Resources()
        sr.return_value.c_sources.append("present.c")
        assert tools.file_impact.should_be_built("K64F", "GCC_ARM", ["present.c"])

def test_ignore_config_error():
    with patch('tools.file_impact.prepare_toolchain') as pt,\
         patch('tools.file_impact.scan_resources') as sr:
        sr.side_effect = ConfigException("Config failed")
        assert not tools.file_impact.should_be_built("K64F", "GCC_ARM", ["present.c"])

def test_pass_other_errors():
    with patch('tools.file_impact.prepare_toolchain') as pt,\
         patch('tools.file_impact.scan_resources') as sr:
        sr.side_effect = Exception("No config failure")
        with pytest.raises(Exception):
            tools.file_impact.should_be_built("K64F", "GCC_ARM", ["present.c"])

schema = json.load(open(join(dirname(__file__), "output-schema.json")))

@pytest.mark.parametrize("file_present", [False, True])
def test_main_schema(file_present):
    with patch('tools.file_impact.should_be_built') as sbb, \
         patch('sys.stdout', new_callable=StringIO) as out, \
         patch('sys.stdin', new_callable=StringIO) as input:
        sbb.return_value = file_present
        tools.file_impact.main()
        dumped = json.loads(out.getvalue())
        assert validate(dumped, schema) is None
        assert all(t in dumped for t in TARGET_MAP)
        for value in dumped.values():
            assert all(tc in value for tc in TOOLCHAIN_CLASSES)

@pytest.mark.parametrize("target", ["K64F", "LPC1768"])
def test_main_contains_required_targets(target):
    def mocked_sbb(in_target, _, __):
        return in_target == target
    with patch('tools.file_impact.should_be_built') as sbb, \
         patch('sys.stdout', new_callable=StringIO) as out, \
         patch('sys.stdin', new_callable=StringIO) as input:
        sbb.side_effect = mocked_sbb
        tools.file_impact.main()
        assert target in json.loads(out.getvalue())

@pytest.mark.parametrize("toolchain", ["IAR", "GCC_ARM", "ARM", "ARMC6"])
def test_main_contains_required_toolcahin(toolchain):
    def mocked_sbb(_, in_toolchain, __):
        return in_toolchain == toolchain
    with patch('tools.file_impact.should_be_built') as sbb, \
         patch('sys.stdout', new_callable=StringIO) as out, \
         patch('sys.stdin', new_callable=StringIO) as input:
        sbb.side_effect = mocked_sbb
        tools.file_impact.main()
        assert all(toolchain in tcs for tcs in json.loads(out.getvalue()).values())

