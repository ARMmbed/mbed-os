import sys
from io import open
from os import sep
from os.path import isfile, join, dirname
import json

import pytest

from tools.memap import MemapParser, _ArmccParser
from copy import deepcopy


PARSED_ARM_DATA = {
    "startup/startup.o": {".text": 0xc0},
    "[lib]/c_p.l/__main.o": {".text": 8},
    "irqs/irqs.o": {".text": 0x98},
    "data/data.o": {".data": 0x18, ".bss": 0x198},
    "main.o": {".text": 0x36},
}

def test_parse_armcc():
    memap = MemapParser()
    memap.parse(join(dirname(__file__), "arm.map"), "ARM")

    parsed_data_os_agnostic = dict()
    for k in PARSED_ARM_DATA:
        parsed_data_os_agnostic[k.replace('/', sep)] = PARSED_ARM_DATA[k]

    assert memap.modules == parsed_data_os_agnostic

PARSED_IAR_DATA = {
    "startup/startup.o": {".text": 0xc0},
    "[lib]/d16M_tlf.a/__main.o": {".text": 8},
    "irqs/irqs.o": {".text": 0x98},
    "data/data.o": {".data": 0x18, ".bss": 0x198},
    "main.o": {".text": 0x36},
}

def test_parse_iar():
    memap = MemapParser()
    memap.parse(join(dirname(__file__), "iar.map"), "IAR")

    parsed_data_os_agnostic = dict()
    for k in PARSED_IAR_DATA:
        parsed_data_os_agnostic[k.replace('/', sep)] = PARSED_IAR_DATA[k]

    assert memap.modules == parsed_data_os_agnostic

PARSED_GCC_DATA = {
    "startup/startup.o": {".text": 0xc0},
    "[lib]/d16M_tlf.a/__main.o": {".text": 8},
    "[lib]/misc/foo.o": {".text": 8},
    "irqs/irqs.o": {".text": 0x98},
    "data/data.o": {".data": 0x18, ".bss": 0x198},
    "main.o": {".text": 0x36},
}

def test_parse_gcc():
    memap = MemapParser()
    memap.parse(join(dirname(__file__), "gcc.map"), "GCC_ARM")

    parsed_data_os_agnostic = dict()
    for k in PARSED_GCC_DATA:
        parsed_data_os_agnostic[k.replace('/', sep)] = PARSED_GCC_DATA[k]

    assert memap.modules == parsed_data_os_agnostic


def test_add_empty_module():
    memap = _ArmccParser()
    old_modules = deepcopy(memap.modules)
    memap.module_add("", 8, ".data")
    assert(old_modules == memap.modules)
    memap.module_add("main.o", 0, ".text")
    assert(old_modules == memap.modules)
    memap.module_add("main.o", 8, "")
    assert(old_modules == memap.modules)

def test_add_full_module():
    memap = _ArmccParser()
    old_modules = deepcopy(memap.modules)
    memap.module_add("main.o", 8, ".data")
    assert(old_modules != memap.modules)
    assert("main.o" in memap.modules)
    assert(".data" in memap.modules["main.o"])
    assert(memap.modules["main.o"][".data"] == 8)
