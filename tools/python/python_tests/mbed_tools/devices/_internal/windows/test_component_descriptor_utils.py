#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import unittest
from mbed_tools.devices._internal.windows.component_descriptor_utils import (
    is_undefined_data_object,
    is_undefined_value,
    data_object_to_dict,
    UNKNOWN_VALUE,
)
from collections import namedtuple
import random


def generate_valid_values():
    return random.choice(["a test", 4646.454, 54, True])


def generate_undefined_values():
    return random.choice([0, None, False, UNKNOWN_VALUE])


class TestUtilities(unittest.TestCase):
    def test_is_value_undefined(self):
        self.assertTrue(is_undefined_value(generate_undefined_values()))
        self.assertFalse(is_undefined_value(generate_valid_values()))

    def test_is_data_object_undefined(self):
        field_number = 30
        DataObjectType = namedtuple("data_object_example", [f"field{i}" for i in range(0, field_number)])
        test1 = DataObjectType(*[generate_undefined_values() for i in range(0, field_number)])
        self.assertTrue(is_undefined_data_object(test1))
        test2 = DataObjectType(*[generate_valid_values() for i in range(0, field_number)])
        self.assertFalse(is_undefined_data_object(test2))

    def test_to_dict(self):
        field_number = 30
        DataObjectType = namedtuple("data_object_example", [f"field{i}" for i in range(0, field_number)])
        expected_dictionary = {
            f"field{i}": random.choice([generate_valid_values(), generate_undefined_values()])
            for i in range(0, field_number)
        }
        test = DataObjectType(**expected_dictionary)
        self.assertDictEqual(data_object_to_dict(test), expected_dictionary)
