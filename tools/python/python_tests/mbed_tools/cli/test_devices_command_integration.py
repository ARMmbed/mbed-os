#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
from unittest import TestCase, mock

import click
from click.testing import CliRunner

from mbed_tools.cli import main
from mbed_tools.cli.list_connected_devices import list_connected_devices
from mbed_tools.lib.exceptions import ToolsError


class TestDevicesCommandIntegration(TestCase):
    def test_devices_is_integrated(self):
        self.assertEqual(main.cli.commands["detect"], list_connected_devices)


class TestClickGroupWithExceptionHandling(TestCase):
    @mock.patch("mbed_tools.cli.main.LOGGER.error", autospec=True)
    def test_logs_tools_errors(self, logger_error):
        def callback():
            raise ToolsError()

        mock_cli = click.Command("test", callback=callback)
        main.cli.add_command(mock_cli, "test")

        runner = CliRunner()
        result = runner.invoke(main.cli, ["test"])

        self.assertEqual(1, result.exit_code)
        logger_error.assert_called_once()


class TestVersionCommand(TestCase):
    def test_version_command(self):
        runner = CliRunner()
        result = runner.invoke(main.cli, ["--version"])
        self.assertTrue(result.output)
        self.assertEqual(0, result.exit_code)
