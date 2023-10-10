#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
from typing import NamedTuple, Any
from unittest import TestCase

from mbed_tools.devices._internal.windows.component_descriptor_utils import is_undefined_data_object, is_undefined_value
from python_tests.mbed_tools.devices._internal.windows.test_component_descriptor_utils import (
    generate_undefined_values,
    generate_valid_values,
)
from python_tests.mbed_tools.devices.markers import windows_only


class ComponentDefinition(NamedTuple):
    field1: str
    field2: str
    field3: str
    field4: Any
    field5: bool
    field6: int


def get_test_class():
    from mbed_tools.devices._internal.windows.component_descriptor import ComponentDescriptor

    class AComponentForTest(ComponentDescriptor):
        def __init__(self) -> None:
            """Initialiser."""
            super().__init__(ComponentDefinition, win32_class_name="Win32_ComputerSystem")

        @property
        def component_id(self) -> str:
            """Returns the device id field."""
            return self.get("field1")

    return AComponentForTest


@windows_only
class TestComponentDescriptor(TestCase):
    def test_init(self):
        self.assertIsNotNone(get_test_class()())
        self.assertTrue(get_test_class()().__class__, "AComponentForTest")

    def test_parameters(self):
        self.assertListEqual([name for name in ComponentDefinition._fields], get_test_class()().field_names)

    def test_set_values(self):

        a_component = get_test_class()()
        self.assertTrue(is_undefined_data_object(a_component.to_tuple()))
        valid_values = {k: generate_valid_values() for k in a_component.field_names}
        a_component.set_data_values(valid_values)
        self.assertFalse(is_undefined_data_object(a_component.to_tuple()))
        self.assertTupleEqual(a_component.to_tuple(), tuple(valid_values.values()))

    def test_is_undefined(self):
        a_component = get_test_class()()
        self.assertTrue(a_component.is_undefined)
        self.assertTrue(is_undefined_value(a_component.component_id))
        a_component_with_undefined_values = get_test_class()()
        undefined_values = {k: generate_undefined_values() for k in a_component.field_names}
        a_component_with_undefined_values.set_data_values(undefined_values)
        self.assertTrue(a_component_with_undefined_values.is_undefined)
        a_defined_component = get_test_class()()
        valid_values = {k: generate_valid_values() for k in a_component.field_names}
        a_defined_component.set_data_values(valid_values)
        self.assertIsNotNone(a_defined_component.component_id)
        self.assertFalse(a_defined_component.is_undefined)

    def test_iterator(self):
        from mbed_tools.devices._internal.windows.component_descriptor import ComponentDescriptorWrapper

        generator = ComponentDescriptorWrapper(get_test_class()).element_generator()
        self.assertIsNotNone(generator)

        for element in generator:
            # The generator should be defined but none of the element should be defined as fields should not exist
            self.assertTrue(element.is_undefined)
