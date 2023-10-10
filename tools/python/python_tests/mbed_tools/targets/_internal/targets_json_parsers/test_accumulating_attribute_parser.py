#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Tests for parsing the attributes for targets in targets.json that accumulate."""
from unittest import TestCase, mock

from mbed_tools.targets._internal.targets_json_parsers.accumulating_attribute_parser import (
    ALL_ACCUMULATING_ATTRIBUTES,
    get_accumulating_attributes_for_target,
    _targets_accumulate_hierarchy,
    _determine_accumulated_attributes,
    _remove_attribute_element,
    _element_matches,
)


class ListAllAccumulatingAttributes(TestCase):
    def test_expected_list(self):
        expected_attributes = (
            "extra_labels",
            "macros",
            "device_has",
            "features",
            "components",
            "extra_labels_add",
            "extra_labels_remove",
            "macros_add",
            "macros_remove",
            "device_has_add",
            "device_has_remove",
            "features_add",
            "features_remove",
            "components_add",
            "components_remove",
        )
        self.assertEqual(ALL_ACCUMULATING_ATTRIBUTES, expected_attributes)


class TestGetAccumulatingAttributes(TestCase):
    @mock.patch(
        "mbed_tools.targets._internal.targets_json_parsers."
        "accumulating_attribute_parser._targets_accumulate_hierarchy"
    )
    @mock.patch(
        "mbed_tools.targets._internal.targets_json_parsers."
        "accumulating_attribute_parser._determine_accumulated_attributes"
    )
    def test_correctly_calls(self, _determine_accumulated_attributes, _targets_accumulate_hierarchy):
        target_name = "Target_Name"
        all_targets_data = {target_name: {"attribute_1": ["something"]}}
        result = get_accumulating_attributes_for_target(all_targets_data, target_name)

        _targets_accumulate_hierarchy.assert_called_once_with(all_targets_data, target_name)
        _determine_accumulated_attributes.assert_called_once_with(_targets_accumulate_hierarchy.return_value)
        self.assertEqual(result, _determine_accumulated_attributes.return_value)


class TestParseHierarchy(TestCase):
    def test_accumulate_hierarchy_single_inheritance(self):
        all_targets_data = {
            "D": {"attribute_1": ["some things"]},
            "C": {"inherits": ["D"], "attribute_2": "something else"},
            "B": {},
            "A": {"inherits": ["C"], "attribute_3": ["even more things"]},
        }
        result = _targets_accumulate_hierarchy(all_targets_data, "A")

        self.assertEqual(result, [all_targets_data["A"], all_targets_data["C"], all_targets_data["D"]])

    def test_accumulate_hierarchy_multiple_inheritance(self):
        all_targets_data = {
            "F": {"attribute_1": "some thing"},
            "E": {"attribute_2": "some other thing"},
            "D": {"inherits": ["F"]},
            "C": {"inherits": ["E"]},
            "B": {"inherits": ["C", "D"]},
            "A": {"inherits": ["B"]},
        }
        result = _targets_accumulate_hierarchy(all_targets_data, "A")

        self.assertEqual(
            result,
            [
                all_targets_data["A"],
                all_targets_data["B"],
                all_targets_data["C"],
                all_targets_data["D"],
                all_targets_data["E"],
                all_targets_data["F"],
            ],
        )


class TestAccumulatingAttributes(TestCase):
    def test_determine_accumulated_attributes_basic_add(self):
        accumulation_order = [
            {"attribute_1": "something"},
            {f"{ALL_ACCUMULATING_ATTRIBUTES[0]}_add": ["2", "3"]},
            {ALL_ACCUMULATING_ATTRIBUTES[0]: ["1"]},
        ]
        expected_attributes = {ALL_ACCUMULATING_ATTRIBUTES[0]: ["1", "2", "3"]}
        result = _determine_accumulated_attributes(accumulation_order)
        self.assertEqual(result, expected_attributes)

    def test_determine_accumulated_attributes_basic_remove(self):
        accumulation_order = [
            {"attribute_1": "something"},
            {f"{ALL_ACCUMULATING_ATTRIBUTES[0]}_remove": ["2", "3"]},
            {ALL_ACCUMULATING_ATTRIBUTES[0]: ["1", "2", "3"]},
        ]
        expected_attributes = {ALL_ACCUMULATING_ATTRIBUTES[0]: ["1"]}
        result = _determine_accumulated_attributes(accumulation_order)
        self.assertEqual(result, expected_attributes)

    def test_combination_multiple_attributes(self):
        accumulation_order = [
            {f"{ALL_ACCUMULATING_ATTRIBUTES[0]}_add": ["2", "3"]},
            {f"{ALL_ACCUMULATING_ATTRIBUTES[1]}_remove": ["B", "C"]},
            {ALL_ACCUMULATING_ATTRIBUTES[0]: ["1"]},
            {ALL_ACCUMULATING_ATTRIBUTES[1]: ["A", "B", "C"]},
        ]
        expected_attributes = {
            ALL_ACCUMULATING_ATTRIBUTES[0]: ["1", "2", "3"],
            ALL_ACCUMULATING_ATTRIBUTES[1]: ["A"],
        }
        result = _determine_accumulated_attributes(accumulation_order)
        self.assertEqual(result, expected_attributes)

    def test_combination_later_check_no_unwanted_overrides(self):
        accumulation_order = [
            {f"{ALL_ACCUMULATING_ATTRIBUTES[0]}_add": ["2", "3"]},
            {f"{ALL_ACCUMULATING_ATTRIBUTES[1]}_remove": ["B", "C"]},
            {ALL_ACCUMULATING_ATTRIBUTES[0]: ["1"]},
            {ALL_ACCUMULATING_ATTRIBUTES[1]: ["A", "B", "C"]},
            {ALL_ACCUMULATING_ATTRIBUTES[1]: []},
        ]
        expected_attributes = {
            ALL_ACCUMULATING_ATTRIBUTES[0]: ["1", "2", "3"],
            ALL_ACCUMULATING_ATTRIBUTES[1]: ["A"],
        }
        result = _determine_accumulated_attributes(accumulation_order)
        self.assertEqual(result, expected_attributes)


class TestElementMatches(TestCase):
    def test_element_matches_exactly(self):
        element_to_remove = "SOMETHING"
        element_to_check = "SOMETHING"

        self.assertTrue(_element_matches(element_to_remove, element_to_check))

    def test_element_no_match(self):
        element_to_remove = "SOMETHING"
        element_to_check = "SOMETHING_ELSE"

        self.assertFalse(_element_matches(element_to_remove, element_to_check))

    def test_element_matches_with_number_arg(self):
        element_to_remove = "SOMETHING"
        element_to_check = "SOMETHING=5"

        self.assertTrue(_element_matches(element_to_remove, element_to_check))

    def test_element_no_match_with_number_arg(self):
        element_to_remove = "SOMETHING"
        element_to_check = "SOMETHING_DIFFERENT=5"

        self.assertFalse(_element_matches(element_to_remove, element_to_check))


class TestRemoveAttributeElement(TestCase):
    def test_remove_element_without_numbers(self):
        current_attribute_state = {"attribute_1": ["ONE", "TWO=2", "THREE"]}
        elements_to_remove = ["ONE", "THREE"]
        expected_result = {"attribute_1": ["TWO=2"]}
        result = _remove_attribute_element(current_attribute_state, "attribute_1", elements_to_remove)

        self.assertEqual(result, expected_result)

    def test_remove_element_with_numbers(self):
        current_attribute_state = {"attribute_1": ["ONE", "TWO=2", "THREE"]}
        elements_to_remove = ["TWO"]
        expected_result = {"attribute_1": ["ONE", "THREE"]}
        result = _remove_attribute_element(current_attribute_state, "attribute_1", elements_to_remove)

        self.assertEqual(result, expected_result)
