#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
from unittest import TestCase, mock
from mbed_tools.targets.get_target import get_target_by_board_type, get_target_by_name
from mbed_tools.targets.exceptions import TargetError
from mbed_tools.targets._internal.target_attributes import TargetAttributesError


class TestGetTarget(TestCase):
    @mock.patch("mbed_tools.targets.get_target.target_attributes.get_target_attributes")
    def test_get_by_name(self, mock_target_attrs):
        target_name = "Target"
        targets_json_file_path = "targets.json"

        result = get_target_by_name(target_name, targets_json_file_path)

        self.assertEqual(result, mock_target_attrs.return_value)
        mock_target_attrs.assert_called_once_with(targets_json_file_path, target_name)

    @mock.patch("mbed_tools.targets.get_target.target_attributes.get_target_attributes")
    def test_get_by_name_raises_target_error_when_target_json_not_found(self, mock_target_attrs):
        target_name = "Target"
        targets_json_file_path = "not-targets.json"
        mock_target_attrs.side_effect = FileNotFoundError

        with self.assertRaises(TargetError):
            get_target_by_name(target_name, targets_json_file_path)

    @mock.patch("mbed_tools.targets.get_target.target_attributes.get_target_attributes")
    def test_get_by_name_raises_target_error_when_target_attr_collection_fails(self, mock_target_attrs):
        target_name = "Target"
        targets_json_file_path = "targets.json"
        mock_target_attrs.side_effect = TargetAttributesError

        with self.assertRaises(TargetError):
            get_target_by_name(target_name, targets_json_file_path)

    @mock.patch("mbed_tools.targets.get_target.get_target_by_name")
    def test_get_by_board_type(self, mock_get_target_by_name):
        board_type = "Board"
        path_to_mbed_program = "somewhere"

        result = get_target_by_board_type(board_type, path_to_mbed_program)

        self.assertEqual(result, mock_get_target_by_name.return_value)
        mock_get_target_by_name.assert_called_once_with(board_type, path_to_mbed_program)
