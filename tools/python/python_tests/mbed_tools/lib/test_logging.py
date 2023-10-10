#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import logging
from unittest import TestCase, mock

from mbed_tools.lib.exceptions import ToolsError
from mbed_tools.lib.logging import _exception_message, MbedToolsHandler, log_exception, set_log_level, LOGGING_FORMAT


class SubclassedToolsError(ToolsError):
    """An exception subclassing ToolsError."""


class TestExceptionMessage(TestCase):
    def test_critical_log_level_with_traceback(self):
        message = _exception_message(ToolsError("unlikely string"), logging.CRITICAL, True)
        self.assertTrue("'-v'" in message)
        self.assertTrue("'--traceback'" not in message)
        self.assertTrue("unlikely string" in message)

    def test_critical_log_level_without_traceback(self):
        message = _exception_message(ToolsError("unlikely string"), logging.CRITICAL, False)
        self.assertTrue("'-v'" in message)
        self.assertTrue("'--traceback'" not in message)
        self.assertTrue("unlikely string" in message)

    def test_error_log_level_with_traceback(self):
        message = _exception_message(ToolsError("unlikely string"), logging.ERROR, True)
        self.assertTrue("'-v'" in message)
        self.assertTrue("'--traceback'" not in message)
        self.assertTrue("unlikely string" in message)

    def test_error_log_level_without_traceback(self):
        message = _exception_message(ToolsError("unlikely string"), logging.ERROR, False)
        self.assertTrue("'-v'" in message)
        self.assertTrue("'--traceback'" not in message)
        self.assertTrue("unlikely string" in message)

    def test_warning_log_level_with_traceback(self):
        message = _exception_message(ToolsError("unlikely string"), logging.WARNING, True)
        self.assertTrue("'-vv'" in message)
        self.assertTrue("'--traceback'" not in message)
        self.assertTrue("unlikely string" in message)

    def test_warning_log_level_without_traceback(self):
        message = _exception_message(ToolsError("unlikely string"), logging.WARNING, False)
        self.assertTrue("'-vv'" in message)
        self.assertTrue("'--traceback'" not in message)
        self.assertTrue("unlikely string" in message)

    def test_info_log_level_with_traceback(self):
        message = _exception_message(ToolsError("unlikely string"), logging.INFO, True)
        self.assertTrue("'-vvv'" in message)
        self.assertTrue("'--traceback'" not in message)
        self.assertTrue("unlikely string" in message)

    def test_info_log_level_without_traceback(self):
        message = _exception_message(ToolsError("unlikely string"), logging.INFO, False)
        self.assertTrue("'-vvv'" in message)
        self.assertTrue("'--traceback'" not in message)
        self.assertTrue("unlikely string" in message)

    def test_debug_log_level_with_traceback(self):
        message = _exception_message(ToolsError("unlikely string"), logging.DEBUG, True)
        self.assertTrue("-v" not in message)
        self.assertTrue("'--traceback'" not in message)
        self.assertTrue("unlikely string" in message)

    def test_debug_log_level_without_traceback(self):
        message = _exception_message(ToolsError("unlikely string"), logging.DEBUG, False)
        self.assertTrue("-v" not in message)
        self.assertTrue("'--traceback'" in message)
        self.assertTrue("unlikely string" in message)

    def test_log_level_not_set_with_traceback(self):
        message = _exception_message(ToolsError("unlikely string"), logging.NOTSET, True)
        self.assertTrue("'-v'" in message)
        self.assertTrue("'--traceback'" not in message)
        self.assertTrue("unlikely string" in message)

    def test_log_level_not_set_without_traceback(self):
        message = _exception_message(ToolsError("unlikely string"), logging.NOTSET, False)
        self.assertTrue("'-v'" in message)
        self.assertTrue("'--traceback'" not in message)
        self.assertTrue("unlikely string" in message)


class TestMbedToolsHandler(TestCase):
    exception_string: str = "A Message"
    expected_log_message: str = "A Message\n\nMore information may be available by using the command line option '-vv'."

    def test_no_exception_raised(self):
        mock_logger = mock.Mock(spec_set=logging.Logger)
        with MbedToolsHandler(mock_logger, traceback=False):
            pass
        self.assertFalse(mock_logger.error.called, "Error should not be logger when an exception is not raised.")

    def test_tools_error_with_traceback(self):
        mock_logger = mock.Mock(spec_set=logging.Logger)
        with MbedToolsHandler(mock_logger, traceback=True):
            raise ToolsError(self.exception_string)
        mock_logger.error.assert_called_once_with(self.expected_log_message, exc_info=True)

    def test_tools_error_without_traceback(self):
        mock_logger = mock.Mock(spec_set=logging.Logger)
        with MbedToolsHandler(mock_logger, traceback=False):
            raise ToolsError(self.exception_string)
        mock_logger.error.assert_called_once_with(self.expected_log_message, exc_info=False)

    def test_subclassed_tools_error_with_traceback(self):
        mock_logger = mock.Mock(spec_set=logging.Logger)
        with MbedToolsHandler(mock_logger, traceback=True):
            raise SubclassedToolsError(self.exception_string)
        mock_logger.error.assert_called_once_with(self.expected_log_message, exc_info=True)

    def test_subclassed_tools_error_without_traceback(self):
        mock_logger = mock.Mock(spec_set=logging.Logger)
        with MbedToolsHandler(mock_logger, traceback=False):
            raise SubclassedToolsError(self.exception_string)
        mock_logger.error.assert_called_once_with(self.expected_log_message, exc_info=False)

    def test_other_exceptions(self):
        mock_logger = mock.Mock(spec_set=logging.Logger)
        with self.assertRaises(ValueError):
            with MbedToolsHandler(mock_logger, traceback=False):
                raise ValueError(self.exception_string)
        self.assertFalse(mock_logger.error.called, "Error should not be logger when a tools error is not raised.")


class TestLogException(TestCase):
    def test_log_error(self):
        mock_logger = mock.Mock(spec_set=logging.Logger)
        mock_exception = mock.Mock(spec_set=Exception)

        log_exception(mock_logger, mock_exception)

        mock_logger.error.assert_called_once_with(mock_exception, exc_info=False)

    def test_log_error_with_traceback(self):
        mock_logger = mock.Mock(spec_set=logging.Logger)
        mock_exception = mock.Mock(spec_set=Exception)

        log_exception(mock_logger, mock_exception, True)

        mock_logger.error.assert_called_once_with(mock_exception, exc_info=True)


@mock.patch("mbed_tools.lib.logging.logging", return_value=mock.Mock(spec_set=logging))
class TestSetLogLevel(TestCase):
    def test_debug(self, mocked_logging):
        set_log_level(verbose_count=3)
        mocked_logging.basicConfig.assert_called_once_with(level=mocked_logging.DEBUG, format=LOGGING_FORMAT)

    def test_info(self, mocked_logging):
        set_log_level(verbose_count=2)
        mocked_logging.basicConfig.assert_called_once_with(level=mocked_logging.INFO, format=LOGGING_FORMAT)

    def test_warning(self, mocked_logging):
        set_log_level(verbose_count=1)
        mocked_logging.basicConfig.assert_called_once_with(level=mocked_logging.WARNING, format=LOGGING_FORMAT)

    def test_error(self, mocked_logging):
        set_log_level(verbose_count=0)
        mocked_logging.basicConfig.assert_called_once_with(level=mocked_logging.ERROR, format=LOGGING_FORMAT)
