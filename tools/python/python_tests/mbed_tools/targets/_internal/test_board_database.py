#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Tests for `mbed_tools.targets._internal.board_database`."""

from unittest import mock

import logging
import pytest

import mbed_tools.targets._internal.board_database as board_database
from mbed_tools.targets.env import env


class TestGetOnlineBoardData:
    """Tests for the method `board_database.get_online_board_data`."""

    def test_401(self, caplog, requests_mock):
        """Given a 401 error code, BoardAPIError is raised."""
        caplog.set_level(logging.DEBUG)
        requests_mock.get(board_database._BOARD_API, status_code=401, text="Who are you?")
        with pytest.raises(board_database.BoardAPIError):
            board_database.get_online_board_data()
        assert any(
            x for x in caplog.records if x.levelno == logging.WARNING and "MBED_API_AUTH_TOKEN" in x.msg
        ), "Auth token should be mentioned"
        assert any(
            x for x in caplog.records if x.levelno == logging.DEBUG and "Who are you?" in x.msg
        ), "Message content should be in the debug message"

    def test_404(self, caplog, requests_mock):
        """Given a 404 error code, TargetAPIError is raised."""
        caplog.set_level(logging.DEBUG)
        requests_mock.get(board_database._BOARD_API, status_code=404, text="Not Found")
        with pytest.raises(board_database.BoardAPIError):
            board_database.get_online_board_data()
        assert any(
            x for x in caplog.records if x.levelno == logging.WARNING and "404" in x.msg
        ), "HTTP status code should be mentioned"
        assert any(
            x for x in caplog.records if x.levelno == logging.DEBUG and "Not Found" in x.msg
        ), "Message content should be in the debug message"

    def test_200_invalid_json(self, caplog, requests_mock):
        """Given a valid response but invalid json, JSONDecodeError is raised."""
        caplog.set_level(logging.DEBUG)
        requests_mock.get(board_database._BOARD_API, text="some text")
        with pytest.raises(board_database.ResponseJSONError):
            board_database.get_online_board_data()
        assert any(
            x for x in caplog.records if x.levelno == logging.WARNING and "Invalid JSON" in x.msg
        ), "Invalid JSON should be mentioned"
        assert any(
            x for x in caplog.records if x.levelno == logging.DEBUG and "some text" in x.msg
        ), "Message content should be in the debug message"

    def test_200_no_data_field(self, caplog, requests_mock):
        """Given a valid response but no data field, ResponseJSONError is raised."""
        caplog.set_level(logging.DEBUG)
        requests_mock.get(board_database._BOARD_API, json={"notdata": [], "stillnotdata": {}})
        with pytest.raises(board_database.ResponseJSONError):
            board_database.get_online_board_data()
        assert any(
            x for x in caplog.records if x.levelno == logging.WARNING and "missing the 'data' field" in x.msg
        ), "Data field should be mentioned"
        assert any(
            x for x in caplog.records if x.levelno == logging.DEBUG and "notdata, stillnotdata" in x.msg
        ), "JSON keys from message should be in the debug message"

    def test_200_value_data(self, requests_mock):
        """Given a valid response, target data is set from the returned json."""
        requests_mock.get(board_database._BOARD_API, json={"data": 42})
        board_data = board_database.get_online_board_data()
        assert 42 == board_data, "Target data should match the contents of the target API data"

    @mock.patch("mbed_tools.targets._internal.board_database.requests")
    @mock.patch("mbed_tools.targets._internal.board_database.env", spec_set=env)
    def test_auth_header_set_with_token(self, env, requests):
        """Given an authorization token env variable, get is called with authorization header."""
        env.MBED_API_AUTH_TOKEN = "token"
        header = {"Authorization": "Bearer token"}
        board_database._get_request()
        requests.get.assert_called_once_with(board_database._BOARD_API, headers=header)

    @mock.patch("mbed_tools.targets._internal.board_database.requests")
    def test_no_auth_header_set_with_empty_token_var(self, requests):
        """Given no authorization token env variable, get is called with no header."""
        board_database._get_request()
        requests.get.assert_called_once_with(board_database._BOARD_API, headers=None)

    @mock.patch("mbed_tools.targets._internal.board_database.requests.get")
    def test_logs_no_warning_on_success(self, get, caplog):
        board_database._get_request()
        assert not caplog.records

    @mock.patch("mbed_tools.targets._internal.board_database.requests.get")
    def test_raises_tools_error_on_connection_error(self, get, caplog):
        get.side_effect = board_database.requests.exceptions.ConnectionError
        with pytest.raises(board_database.BoardAPIError):
            board_database._get_request()
        assert "Unable to connect" in caplog.text
        assert len(caplog.records) == 1

    @mock.patch("mbed_tools.targets._internal.board_database.requests.get")
    def test_logs_error_on_requests_ssl_error(self, get, caplog):
        get.side_effect = board_database.requests.exceptions.SSLError
        with pytest.raises(board_database.BoardAPIError):
            board_database._get_request()
        assert "verify an SSL" in caplog.text

    @mock.patch("mbed_tools.targets._internal.board_database.requests.get")
    def test_logs_error_on_requests_proxy_error(self, get, caplog):
        get.side_effect = board_database.requests.exceptions.ProxyError
        with pytest.raises(board_database.BoardAPIError):
            board_database._get_request()
        assert "connect to proxy" in caplog.text

    @mock.patch.dict("os.environ", {"http_proxy": "http://proxy:8080", "https_proxy": "https://proxy:8080"})
    def test_requests_uses_proxy_variables(self, requests_mock):
        requests_mock.get(board_database._BOARD_API)
        board_database._get_request()
        assert requests_mock.last_request.proxies == {"http": "http://proxy:8080", "https": "https://proxy:8080"}


class TestGetOfflineTargetData:
    """Tests for the method get_offline_target_data."""

    def test_local_target_database_file_found(self):
        """Test local database is found and loaded."""
        data = board_database.get_offline_board_data()
        assert len(data), "Some data should be returned from local database file."

    @mock.patch("mbed_tools.targets._internal.board_database.get_board_database_path")
    def test_raises_on_invalid_json(self, mocked_get_file):
        """Test raises an error when the file contains invalid JSON."""
        invalid_json = "None"
        path_mock = mock.Mock()
        path_mock.read_text.return_value = invalid_json
        mocked_get_file.return_value = path_mock
        with pytest.raises(board_database.ResponseJSONError):
            board_database.get_offline_board_data()


class TestGetLocalTargetDatabaseFile:
    def test_returns_path_to_targets(self):
        path = board_database.get_board_database_path()
        assert path.exists(), "Path to boards should exist in the package data folder."
