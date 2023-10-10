# Copyright (c) 2018, Arm Limited and affiliates.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from sys import stdout
from .mbed_base import Mbed
from . import __version__


class HostTestResults(object):
    """! Test results set by host tests """

    def enum(self, **enums):
        return type('Enum', (), enums)

    def __init__(self):
        self.TestResults = self.enum(
            RESULT_SUCCESS = 'success',
            RESULT_FAILURE = 'failure',
            RESULT_ERROR = 'error',
            RESULT_END = 'end',
            RESULT_UNDEF = 'undefined',
            RESULT_TIMEOUT = 'timeout',
            RESULT_IOERR_COPY = "ioerr_copy",
            RESULT_IOERR_DISK = "ioerr_disk",
            RESULT_IO_SERIAL = 'ioerr_serial',
            RESULT_NO_IMAGE = 'no_image',
            RESULT_NOT_DETECTED = "not_detected",
            RESULT_MBED_ASSERT = "mbed_assert",
            RESULT_PASSIVE = "passive",
            RESULT_BUILD_FAILED = 'build_failed',
            RESULT_SYNC_FAILED = 'sync_failed'
        )

        # Magically creates attributes in this class corresponding
        # to RESULT_ elements in self.TestResults enum
        for attr in self.TestResults.__dict__:
            if attr.startswith('RESULT_'):
                setattr(self, attr, self.TestResults.__dict__[attr])

        # Indexes of this list define string->int mapping between
        # actual strings with results
        self.TestResultsList = [
            self.TestResults.RESULT_SUCCESS,
            self.TestResults.RESULT_FAILURE,
            self.TestResults.RESULT_ERROR,
            self.TestResults.RESULT_END,
            self.TestResults.RESULT_UNDEF,
            self.TestResults.RESULT_TIMEOUT,
            self.TestResults.RESULT_IOERR_COPY,
            self.TestResults.RESULT_IOERR_DISK,
            self.TestResults.RESULT_IO_SERIAL,
            self.TestResults.RESULT_NO_IMAGE,
            self.TestResults.RESULT_NOT_DETECTED,
            self.TestResults.RESULT_MBED_ASSERT,
            self.TestResults.RESULT_PASSIVE,
            self.TestResults.RESULT_BUILD_FAILED,
            self.TestResults.RESULT_SYNC_FAILED
        ]

    def get_test_result_int(self, test_result_str):
        """! Maps test result string to unique integer """
        if test_result_str in self.TestResultsList:
            return self.TestResultsList.index(test_result_str)
        return -1

    def __getitem__(self, test_result_str):
        """! Returns numerical result code """
        return self.get_test_result_int(test_result_str)


class Test(HostTestResults):
    """ Base class for host test's test runner
    """
    def __init__(self, options):
        """ ctor
        """
        HostTestResults.__init__(self)
        self.mbed = Mbed(options)

    def run(self):
        """ Test runner for host test. This function will start executing
            test and forward test result via serial port to test suite
        """
        pass

    def setup(self):
        """! Setup and check if configuration for test is correct.
        @details This function can for example check if serial port is already opened
        """
        pass

    def notify(self, msg):
        """! On screen notification function
        @param msg Text message sent to stdout directly
        """
        stdout.write(msg)
        stdout.flush()

    def print_result(self, result):
        """! Test result unified printing function
        @param result Should be a member of HostTestResults.RESULT_* enums
        """
        self.notify("{{%s}}\n"% result)
        self.notify("{{%s}}\n"% self.RESULT_END)

    def finish(self):
        """ dctor for this class, finishes tasks and closes resources
        """
        pass

    def get_hello_string(self):
        """ Hello string used as first print
        """
        return "host test executor ver. " + __version__


class DefaultTestSelectorBase(Test):
    """! Test class with serial port initialization
    @details This is a base for other test selectors, initializes
    """
    def __init__(self, options):
        Test.__init__(self, options=options)
