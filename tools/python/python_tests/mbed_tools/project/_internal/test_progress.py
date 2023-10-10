#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
from unittest import TestCase, mock

from mbed_tools.project._internal.progress import ProgressReporter, ProgressBar


class TestProgressBar(TestCase):
    @mock.patch("mbed_tools.project._internal.progress.ProgressBar.update")
    def test_updates_progress_bar_with_correct_block_size(self, mock_bar_update):
        bar = ProgressBar(total=100)
        bar.update_progress(1, 1)

        mock_bar_update.assert_called_once_with(1)

    def test_sets_total_attribute_to_value_of_total_size(self):
        bar = ProgressBar()

        self.assertIsNone(bar.total)

        bar.update_progress(1, 2, total_size=33)

        self.assertEqual(bar.total, 33)


@mock.patch("mbed_tools.project._internal.progress.ProgressBar", autospec=True)
class TestProgressReporter(TestCase):
    def test_creates_progress_bar_on_begin_opcode(self, mock_progress_bar):
        reporter = ProgressReporter()
        reporter._cur_line = "begin"
        reporter.update(reporter.BEGIN, 1)

        mock_progress_bar.assert_called_once()

    def test_closes_progress_bar_on_end_opcode(self, mock_progress_bar):
        reporter = ProgressReporter()
        reporter.bar = mock_progress_bar()
        reporter.update(reporter.END, 1)

        reporter.bar.close.assert_called_once()
        reporter.bar.update.assert_not_called()
