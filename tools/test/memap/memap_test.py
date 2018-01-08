"""
mbed SDK
Copyright (c) 2017 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
import sys
from os.path import isfile, join
import json

import pytest

import tools.memap
from tools.memap import MemapParser
from copy import deepcopy

"""
Tests for test_api.py
"""

@pytest.fixture
def memap_parser():
    """
    Called before each test case

    :return:
    """
    memap_parser = MemapParser()

    memap_parser.modules = {
        "mbed-os/targets/TARGET/TARGET_MCUS/api/pinmap.o": {
            ".text": 1,
            ".data": 2,
            ".bss": 3,
            ".heap": 0,
            ".stack": 0,
            ".interrupts_ram":0,
            ".init":0,
            ".ARM.extab":0,
            ".ARM.exidx":0,
            ".ARM.attributes":0,
            ".eh_frame":0,
            ".init_array":0,
            ".fini_array":0,
            ".jcr":0,
            ".stab":0,
            ".stabstr":0,
            ".ARM.exidx":0,
            ".ARM":0,
            ".interrupts":0,
            ".flash_config":0,
            "unknown":0,
            "OUTPUT":0,
        },
        "[lib]/libc.a/lib_a-printf.o": {
            ".text": 4,
            ".data": 5,
            ".bss": 6,
            ".heap": 0,
            ".stack": 0,
            ".interrupts_ram":0,
            ".init":0,
            ".ARM.extab":0,
            ".ARM.exidx":0,
            ".ARM.attributes":0,
            ".eh_frame":0,
            ".init_array":0,
            ".fini_array":0,
            ".jcr":0,
            ".stab":0,
            ".stabstr":0,
            ".ARM.exidx":0,
            ".ARM":0,
            ".interrupts":0,
            ".flash_config":0,
            "unknown":0,
            "OUTPUT":0,
        },
        "main.o": {
            ".text": 7,
            ".data": 8,
            ".bss": 0,
            ".heap": 0,
            ".stack": 0,
            ".interrupts_ram":0,
            ".init":0,
            ".ARM.extab":0,
            ".ARM.exidx":0,
            ".ARM.attributes":0,
            ".eh_frame":0,
            ".init_array":0,
            ".fini_array":0,
            ".jcr":0,
            ".stab":0,
            ".stabstr":0,
            ".ARM.exidx":0,
            ".ARM":0,
            ".interrupts":0,
            ".flash_config":0,
            "unknown":0,
            "OUTPUT":0,
        },
        "test.o": {
            ".text": 0,
            ".data": 0,
            ".bss": 0,
            ".heap": 0,
            ".stack": 0,
            ".interrupts_ram":0,
            ".init":0,
            ".ARM.extab":0,
            ".ARM.exidx":0,
            ".ARM.attributes":0,
            ".eh_frame":0,
            ".init_array":0,
            ".fini_array":0,
            ".jcr":0,
            ".stab":0,
            ".stabstr":0,
            ".ARM.exidx":0,
            ".ARM":0,
            ".interrupts":0,
            ".flash_config":0,
            "unknown":0,
            "OUTPUT":0,
        },
    }
    return memap_parser


def generate_test_helper(memap_parser, format, depth, sep, file_output=None):
    """
    Helper that tests that the member variables "modules" is
    unchanged after calling "generate_output"

    :param memap_parser: the parser object
    :param depth: how much detail to put in the report
    :param format:  the file type to output
    :param file_output: the file to output to
    """
    old_modules = deepcopy(memap_parser.modules)

    tools.memap.sep = sep
    memap_parser.generate_output(format, depth, file_output=file_output)

    assert memap_parser.modules == old_modules,\
        "generate_output modified the 'modules' property"

    for file_name in memap_parser.short_modules:
        assert(len(file_name.split(tools.memap.sep)) <= depth)


@pytest.mark.parametrize("depth", [1, 2, 20])
@pytest.mark.parametrize("sep", ["\\", "/"])
def test_report_computed(memap_parser, depth, sep):
    """
    Test that a report and summary are computed

    :param memap_parser: Mocked parser
    :param depth: the detail of the output
    """

    memap_parser.generate_output('table', depth, sep)

    # Report is created after generating output
    assert memap_parser.mem_summary
    assert memap_parser.mem_report


@pytest.mark.parametrize("depth", [1, 2, 20])
@pytest.mark.parametrize("sep", ["\\", "/"])
def test_generate_output_table(memap_parser, depth, sep):
    """
    Test that an output of type "table" can be generated correctly
    :param memap_parser: Mocked parser
    :param depth: the detail of the output
    """
    generate_test_helper(memap_parser, 'table', depth, sep)


@pytest.mark.parametrize("depth", [1, 2, 20])
@pytest.mark.parametrize("sep", ["\\", "/"])
def test_generate_output_json(memap_parser, tmpdir, depth, sep):
    """
    Test that an output of type "json" can be generated correctly
    :param memap_parser: Mocked parser
    :param tmpdir: a unique location to place an output file
    :param depth: the detail of the output
    """
    file_name = str(tmpdir.join('output.json').realpath())
    generate_test_helper(memap_parser, 'json', depth, sep, file_name)
    assert isfile(file_name), "Failed to create json file"
    json.load(open(file_name))


@pytest.mark.parametrize("depth", [1, 2, 20])
@pytest.mark.parametrize("sep", ["\\", "/"])
def test_generate_output_csv_ci(memap_parser, tmpdir, depth, sep):
    """
    Test ensures that an output of type "csv-ci" can be generated correctly

    :param memap_parser: Mocked parser
    :param tmpdir: a unique location to place an output file
    :param depth: the detail of the output
    """
    file_name = str(tmpdir.join('output.csv').realpath())
    generate_test_helper(memap_parser, 'csv-ci', depth, sep, file_name)
    assert isfile(file_name), "Failed to create csv-ci file"
