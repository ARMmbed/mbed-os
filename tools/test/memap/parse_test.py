import sys
from io import open
from os.path import isfile, join, dirname
import json

import pytest

from tools.memap import MemapParser
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
    memap.parse_map_file_armcc(open(join(dirname(__file__), "arm.map")))
    assert memap.modules == PARSED_ARM_DATA

PARSED_IAR_GCC_DATA = {
    "startup/startup.o": {".text": 0xc0},
    "[lib]/d16M_tlf.a/__main.o": {".text": 8},
    "irqs/irqs.o": {".text": 0x98},
    "data/data.o": {".data": 0x18, ".bss": 0x198},
    "main.o": {".text": 0x36},
}

def test_parse_iar():
    memap = MemapParser()
    memap.parse_map_file_iar(open(join(dirname(__file__), "iar.map")))
    assert memap.modules == PARSED_IAR_GCC_DATA

def test_parse_gcc():
    memap = MemapParser()
    memap.parse_map_file_gcc(open(join(dirname(__file__), "gcc.map")))
    assert memap.modules == PARSED_IAR_GCC_DATA
