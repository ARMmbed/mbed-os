#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Tests for `mbed_tools.targets.boards`."""

import json
from dataclasses import asdict
from unittest import mock, TestCase

from mbed_tools.targets import Board
from mbed_tools.targets.boards import Boards
from mbed_tools.targets.exceptions import UnknownBoard
from python_tests.mbed_tools.targets.factories import make_dummy_internal_board_data


@mock.patch("mbed_tools.targets._internal.board_database.get_online_board_data")
class TestBoards(TestCase):
    """Tests for the class `Boards`."""

    def test_iteration_is_repeatable(self, mocked_get_board_data):
        """Test Boards is an iterable and not an exhaustible iterator."""
        fake_board_data = make_dummy_internal_board_data()
        mocked_get_board_data.return_value = fake_board_data

        boards = Boards.from_online_database()
        tgts_a = [b for b in boards]
        tgts_b = [b for b in boards]

        self.assertEqual(tgts_a, tgts_b, "The lists are equal as boards was not exhausted on the first pass.")

    def test_board_found_in_boards_membership_test(self, mocked_get_board_data):
        """Tests the __contains__ method was implemented correctly."""
        board_data = make_dummy_internal_board_data()
        mocked_get_board_data.return_value = board_data

        boards = Boards.from_online_database()
        board, *_ = boards

        self.assertIn(board, boards)

    def test_membership_test_returns_false_for_non_board(self, mocked_get_board_data):
        """Tests the __contains__ method was implemented correctly."""
        board_data = make_dummy_internal_board_data()
        mocked_get_board_data.return_value = board_data

        boards = Boards.from_online_database()

        self.assertFalse("a" in boards)

    def test_len_boards(self, mocked_get_board_data):
        """Test len(Boards()) matches len(board_database)."""
        board_data = make_dummy_internal_board_data()
        mocked_get_board_data.return_value = board_data

        self.assertEqual(len(Boards.from_online_database()), len(board_data))

    def test_get_board_success(self, mocked_get_board_data):
        """Check a Board can be looked up by arbitrary parameters."""
        fake_board_data = [
            {"attributes": {"product_code": "0300"}},
            {"attributes": {"product_code": "0200"}},
            {"attributes": {"product_code": "0100"}},
        ]
        mocked_get_board_data.return_value = fake_board_data

        boards = Boards.from_online_database()
        board = boards.get_board(lambda b: b.product_code == "0100")

        self.assertEqual(board.product_code, "0100", "Board's product code should match the given product code.")

    def test_get_board_failure(self, mocked_get_board_data):
        """Check Boards handles queries without a match."""
        mocked_get_board_data.return_value = []

        boards = Boards.from_online_database()

        with self.assertRaises(UnknownBoard):
            boards.get_board(lambda b: b.product_code == "unknown")

    @mock.patch("mbed_tools.targets._internal.board_database.get_offline_board_data")
    def test_json_dump_from_raw_and_filtered_data(self, mocked_get_offline_board_data, mocked_get_online_board_data):
        raw_board_data = [
            {"attributes": {"product_code": "0200", "board": "test"}},
            {"attributes": {"product_code": "0100", "board": "test2"}},
        ]
        mocked_get_online_board_data.return_value = raw_board_data

        boards = [Board.from_online_board_entry(b) for b in raw_board_data]
        filtered_board_data = [asdict(board) for board in boards]
        mocked_get_offline_board_data.return_value = filtered_board_data

        # Boards.from_online_database handles "raw" board entries from the online db
        boards = Boards.from_online_database()
        json_str_from_raw = boards.json_dump()
        t1_raw, t2_raw = boards

        # Boards.from_offline_database expects the data to have been "filtered" through the Boards interface
        offline_boards = Boards.from_offline_database()
        json_str_from_filtered = offline_boards.json_dump()
        t1_filt, t2_filt = offline_boards

        self.assertEqual(
            json_str_from_raw,
            json.dumps([asdict(t1_raw), asdict(t2_raw)], indent=4),
            "JSON string should match serialised board __dict__.",
        )

        self.assertEqual(json_str_from_filtered, json.dumps([t1_filt.__dict__, t2_filt.__dict__], indent=4))
