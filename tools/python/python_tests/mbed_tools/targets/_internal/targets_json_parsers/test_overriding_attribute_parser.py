#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Tests for parsing the attributes for targets in targets.json that override."""
from unittest import TestCase, mock

from mbed_tools.targets._internal.targets_json_parsers.overriding_attribute_parser import (
    get_overriding_attributes_for_target,
    get_labels_for_target,
    _targets_override_hierarchy,
    _determine_overridden_attributes,
    _remove_unwanted_attributes,
    MERGING_ATTRIBUTES,
)
from mbed_tools.targets._internal.targets_json_parsers.accumulating_attribute_parser import ALL_ACCUMULATING_ATTRIBUTES


class TestGetOverridingAttributes(TestCase):
    @mock.patch(
        "mbed_tools.targets._internal.targets_json_parsers." "overriding_attribute_parser._targets_override_hierarchy"
    )
    @mock.patch(
        "mbed_tools.targets._internal.targets_json_parsers."
        "overriding_attribute_parser._determine_overridden_attributes"
    )
    def test_correctly_calls(self, _determine_overridden_attributes, _targets_override_hierarchy):
        target_name = "Target_Name"
        all_targets_data = {target_name: {"attribute_1": ["something"]}}
        result = get_overriding_attributes_for_target(all_targets_data, target_name)

        _targets_override_hierarchy.assert_called_once_with(all_targets_data, target_name)
        _determine_overridden_attributes.assert_called_once_with(_targets_override_hierarchy.return_value)
        self.assertEqual(result, _determine_overridden_attributes.return_value)


class TestParseHierarchy(TestCase):
    def test_overwrite_hierarchy_single_inheritance(self):
        all_targets_data = {
            "D": {"attribute_1": ["some things"]},
            "C": {"inherits": ["D"], "attribute_2": "something else"},
            "B": {},
            "A": {"inherits": ["C"], "attribute_3": ["even more things"]},
        }
        result = _targets_override_hierarchy(all_targets_data, "A")

        self.assertEqual(result, [all_targets_data["A"], all_targets_data["C"], all_targets_data["D"]])

    def test_overwrite_hierarchy_multiple_inheritance(self):
        all_targets_data = {
            "F": {"attribute_1": "some thing"},
            "E": {"attribute_2": "some other thing"},
            "D": {"inherits": ["F"]},
            "C": {"inherits": ["E"]},
            "B": {"inherits": ["C", "D"]},
            "A": {"inherits": ["B"]},
        }
        result = _targets_override_hierarchy(all_targets_data, "A")

        self.assertEqual(
            result,
            [
                all_targets_data["A"],
                all_targets_data["B"],
                all_targets_data["C"],
                all_targets_data["E"],
                all_targets_data["D"],
                all_targets_data["F"],
            ],
        )


class TestOverridingAttributes(TestCase):
    def test_determine_overwritten_attributes(self):
        override_order = [
            {"attribute_1": "1"},
            {"attribute_1": "I should be overridden", "attribute_2": "2"},
            {"attribute_3": "3"},
        ]
        expected_attributes = {"attribute_1": "1", "attribute_2": "2", "attribute_3": "3"}

        result = _determine_overridden_attributes(override_order)
        self.assertEqual(result, expected_attributes)

    def test_remove_accumulating_attributes(self):
        override_order = [
            {ALL_ACCUMULATING_ATTRIBUTES[0]: "1"},
            {"attribute": "Normal override attribute"},
            {ALL_ACCUMULATING_ATTRIBUTES[1]: "3"},
        ]
        expected_attributes = {"attribute": "Normal override attribute"}

        result = _determine_overridden_attributes(override_order)
        self.assertEqual(result, expected_attributes)

    def test_merging_attributes(self):
        override_order = [
            {MERGING_ATTRIBUTES[0]: {"FOO": "I should also remain"}},
            {MERGING_ATTRIBUTES[0]: {"FOO": "I should not remain"}},
            {MERGING_ATTRIBUTES[0]: {"BAR": "I should remain"}},
        ]
        expected_attributes = {MERGING_ATTRIBUTES[0]: {"BAR": "I should remain", "FOO": "I should also remain"}}

        result = _determine_overridden_attributes(override_order)
        self.assertEqual(result, expected_attributes)


class TestGetLabelsForTarget(TestCase):
    def test_linear_inheritance(self):
        all_targets_data = {
            "C": {"attribute_1": ["some things"]},
            "B": {"inherits": ["C"], "attribute_2": "something else"},
            "A": {"inherits": ["B"], "attribute_3": ["even more things"]},
        }
        target_name = "A"
        expected_result = {"A", "B", "C"}
        result = get_labels_for_target(all_targets_data, target_name)

        self.assertSetEqual(result, expected_result)

    def test_multiple_inheritance(self):
        all_targets_data = {
            "E": {"attribute_1": "some thing"},
            "D": {"attribute_2": "some other thing"},
            "C": {"inherits": ["E"]},
            "B": {"inherits": ["C", "D"]},
            "A": {"inherits": ["B"]},
        }
        target_name = "A"
        expected_result = {"A", "B", "C", "D", "E"}
        result = get_labels_for_target(all_targets_data, target_name)

        self.assertSetEqual(result, expected_result)

    def test_no_inheritance(self):
        all_targets_data = {
            "A": {"attribute_3": ["some things"]},
        }
        target_name = "A"
        expected_result = {"A"}
        result = get_labels_for_target(all_targets_data, target_name)

        self.assertSetEqual(result, expected_result)


class TestRemoveUnwantedAttributes(TestCase):
    def test_removes_accumulating_public_and_inherits(self):
        target_attributes = {
            ALL_ACCUMULATING_ATTRIBUTES[0]: "1",
            "public": False,
            "inherits": "SomeOtherBoard",
            "attribute": "I should remain",
        }
        expected_result = {"attribute": "I should remain"}

        result = _remove_unwanted_attributes(target_attributes)
        self.assertEqual(result, expected_result)
