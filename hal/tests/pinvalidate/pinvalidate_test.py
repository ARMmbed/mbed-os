"""
Copyright (c) 2020 ARM Limited

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

import pytest
import os
from pinvalidate import *

@pytest.fixture
def pin_name_content():
    pin_name_file = open("./test_files/PinNames_test.h")
    return pin_name_file.read()

@pytest.fixture
def pin_name_dict(pin_name_content):
    return pin_name_to_dict(pin_name_content)

def test_marker_check():
    expect = [
        {'file': os.path.abspath('./test_files/nonexistent_target/PinNames.h'), 'error': 'target not found'},
        {'file': os.path.abspath('./test_files/misformatted_marker/PinNames.h'), 'error': 'marker invalid or not found'},
        {'file': os.path.abspath('./test_files/missing_marker/PinNames.h'), 'error': 'marker invalid or not found'}
    ]
    
    assert check_markers(test_mode=True) == expect

def test_duplicate_pinnames_files_check():
    expect = [
        {'file': os.path.abspath('./test_files/PinNames.h'), 'error': 'duplicate file'},
        {'file': os.path.abspath('./test_files/duplicate_marker/PinNames.h'), 'error': 'duplicate file'}
    ]
    
    assert check_duplicate_pinnames_files(test_mode=True) == expect

def test_duplicate_markers_check():
    expect = [
        {'file': os.path.abspath('./test_files/duplicate_file/PinNames.h'), 'error': 'duplicate marker'},
        {'file': os.path.abspath('./test_files/duplicate_marker/PinNames.h'), 'error': 'duplicate marker'}
    ]

    assert check_duplicate_markers(test_mode=True) == expect

def test_pin_name_to_dict(pin_name_dict):
    expect = {
        "PA_0": "0x00",
        "PA_1": "0x01",
        "PA_2": "0x02",
        "PA_3": "0x03",
        "PA_4": "0x04",
        "PA_5": "0x05",
        "PA_6": "0x06",
        "PA_7": "0x07",
        "PA_8": "0x08",
        "PA_9": "0x09",
        "PA_10": "0x0A",
        "PA_11": "0x0B",
        "PA_12": "0x0C",
        "PA_13": "0x0D",
        "PA_14": "0x0E",
        "PA_15": "0x0F",
        "PB_0": "0x10",
        "PB_1": "0x11",
        "PB_2": "0x12",
        "PB_3": "0x13",
        "PB_4": "0x14",
        "PB_5": "0x15",
        "PB_6": "0x16",
        "PB_7": "0x17",
        "PB_8": "0x18",
        "PB_9": "0x19",
        "PB_10": "0x1A",
        "PB_11": "0x1B",
        "PB_12": "0x1C",
        "PB_13": "0x1D",
        "PB_14": "0x1E",
        "PB_15": "0x1F",
        "PC_0": "0x20",
        "PC_1": "0x21",
        "PC_2": "0x22",
        "PC_3": "0x23",
        "PC_4": "0x24",
        "PC_5": "0x25",
        "PC_6": "0x26",
        "PC_7": "0x27",
        "PC_8": "0x28",
        "PC_9": "0x29",
        "PC_10": "0x2A",
        "PC_11": "0x2B",
        "PC_12": "0x2C",
        "PC_13": "0x2D",
        "PC_14": "0x2E",
        "PC_15": "0x2F",
        "PD_0": "0x30",
        "PD_1": "0x31",
        "PD_2": "0x32",
        "PD_3": "0x33",
        "PD_4": "0x34",
        "PD_5": "0x35",
        "PD_6": "0x36",
        "PD_7": "0x37",
        "PD_8": "0x38",
        "PD_9": "0x39",
        "PD_10": "0x3A",
        "PD_11": "0x3B",
        "PD_12": "0x3C",
        "PD_13": "0x3D",
        "PD_14": "0x3E",
        "PD_15": "0x3F",
        "ARDUINO_UNO_A0": "PC_5",
        "ARDUINO_UNO_A1": "PC_4",
        "ARDUINO_UNO_A2": "PC_3",
        "ARDUINO_UNO_A3": "PC_2",
        "ARDUINO_UNO_A4": "PC_1",
        "ARDUINO_UNO_A5": "PC_0",
        "ARDUINO_UNO_D0": "PA_1",
        "ARDUINO_UNO_D1": "PA_0",
        "ARDUINO_UNO_D2": "PD_14",
        "ARDUINO_UNO_D3": "PB_0",
        "ARDUINO_UNO_D4": "PA_3",
        "ARDUINO_UNO_D5": "PB_4",
        "ARDUINO_UNO_D6": "PB_1",
        "ARDUINO_UNO_D8": "PB_1",
        "ARDUINO_UNO_D9": "PA_15",
        "ARDUINO_UNO_D10": "PA_2",
        "ARDUINO_UNO_D11": "NC",
        "ARDUINO_UNO_D12": "PA_6",
        "ARDUINO_UNO_D13": "PA_5",
        "ARDUINO_UNO_D14": "PB_9",
        "ARDUINO_UNO_D15": "PB_8",
        "CONSOLE_TX": "PB_6",
        "CONSOLE_RX": "PB_7",
        "LED1": "PA_5",
        "BUTTON1": "PC_2",
        "LED2": "PB_14",
        "LED3": "PC_9",
        "BUTTON2": "PC_13",
        "LED4": "LED3",
        "LED5": "PC_9",
        "LED6": "LED6",
        "LED7": "NC",
        "BUTTON3": "PC_13",
        "BUTTON4": "BUTTON1",
        "BUTTON5": "NC",
        "BUTTON6": "BUTTON6",
    }
    assert pin_name_dict == expect

def test_identity_assignment_check(pin_name_dict):
    expected_errors = [
        {'key': 'LED6', 'val': 'LED6', 'message': 'cannot assign value to itself'},
        {'key': 'BUTTON6', 'val': 'BUTTON6', 'message': 'cannot assign value to itself'},
    ]
    assert identity_assignment_check(pin_name_dict) == expected_errors

def test_nc_assignment_check(pin_name_dict):
    expected_errors = [
        {'key': 'LED7','message': 'cannot be NC','val': 'NC'},
        {'key': 'BUTTON5','message': 'cannot be NC','val': 'NC'},
    ]
    assert nc_assignment_check(pin_name_dict) == expected_errors

def test_duplicate_assignment_check(pin_name_dict):
    expected_errors = [
        {'key': 'LED4', 'val': 'LED3', 'message': 'already assigned to LED3 = PC_9'},
        {'key': 'LED5', 'val': 'PC_9', 'message': 'already assigned to LED3 = PC_9'},
        {'key': 'BUTTON3', 'val': 'PC_13', 'message': 'already assigned to BUTTON2 = PC_13'},
        {'key': 'BUTTON4', 'val': 'BUTTON1', 'message': 'already assigned to BUTTON1 = PC_2'},
    ]
    assert duplicate_assignment_check(pin_name_dict) == expected_errors

def test_legacy_assignment_check(pin_name_content):
    expected_errors = [
        {'key': 'LED1', 'val': 'PA_5', 'message': 'legacy assignment; LEDs and BUTTONs must be #define\'d'},
        {'key': 'BUTTON1', 'val': 'PC_2', 'message': 'legacy assignment; LEDs and BUTTONs must be #define\'d'},
    ]
    assert legacy_assignment_check(pin_name_content) == expected_errors

def test_arduino_duplicate_assignment_check(pin_name_dict):
    expected_errors = [
        {'key': 'ARDUINO_UNO_D8', 'val': 'PB_1', 'message': 'already assigned to ARDUINO_UNO_D6 = PB_1'},
    ]
    assert arduino_duplicate_assignment_check(pin_name_dict) == expected_errors

def test_arduino_existence_check(pin_name_dict):
    expected_errors = [
        {'key': 'ARDUINO_UNO_D7', 'val': '', 'message': 'ARDUINO_UNO_D7 not defined'},
    ]
    assert arduino_existence_check(pin_name_dict) == expected_errors

def test_arduino_nc_assignment_check(pin_name_dict):
    expected_errors = [
        {'key': 'ARDUINO_UNO_D11', 'val': 'NC', 'message': 'cannot be NC'},
    ]
    assert arduino_nc_assignment_check(pin_name_dict) == expected_errors
