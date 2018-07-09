"""
Copyright (c) 2018 ARM Limited
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

"""
This script is the host script for trng test sequence, it send the 
step signaling sequence and receive and transmit data to the device after 
reset if necesarry (default loading and storing mechanism while reseting the device
is NVstore, in case NVstore isn't enabled we'll use current infrastructure,
for more details see main.cpp file)
"""

import time
from mbed_host_tests import BaseHostTest
from mbed_host_tests.host_tests_runner.host_test_default import DefaultTestSelector

DEFAULT_CYCLE_PERIOD      = 1.0
MSG_VALUE_DUMMY           = '0'
MSG_TRNG_READY            = 'ready'
MSG_TRNG_BUFFER           = 'buffer'
MSG_TRNG_FINISH           = 'finish'
MSG_TRNG_TEST_STEP1       = 'check_step1'
MSG_TRNG_TEST_STEP2       = 'check_step2'
MSG_KEY_SYNC              = '__sync'
MSG_KEY_TEST_SUITE_ENDED  = 'Test suite ended'

class TRNGResetTest(BaseHostTest):
    """Test for the TRNG API.
    """

    def __init__(self):
        super(TRNGResetTest, self).__init__()
        self.reset = False
        self.finish = False
        self.suite_ended = False
        self.buffer = 0
        cycle_s = self.get_config_item('program_cycle_s')
        self.program_cycle_s = cycle_s if cycle_s is not None else DEFAULT_CYCLE_PERIOD
        self.test_steps_sequence = self.test_steps()
        # Advance the coroutine to it's first yield statement.
        self.test_steps_sequence.send(None)

    #define callback functions for msg handling
    def setup(self):
        self.register_callback(MSG_TRNG_READY, self.cb_device_ready)
        self.register_callback(MSG_TRNG_BUFFER, self.cb_trng_buffer)
        self.register_callback(MSG_TRNG_FINISH, self.cb_device_finish)
        self.register_callback(MSG_KEY_TEST_SUITE_ENDED, self.cb_device_test_suit_ended)

    #receive sent data from device before reset
    def cb_trng_buffer(self, key, value, timestamp):
        """Acknowledge device rebooted correctly and feed the test execution
        """
        self.buffer = value

    def cb_device_ready(self, key, value, timestamp):
        """Acknowledge device rebooted correctly and feed the test execution
        """
        self.reset = True

        try:
            if self.test_steps_sequence.send(value):
                self.notify_complete(True)
        except (StopIteration, RuntimeError) as exc:
            self.notify_complete(False)

    def cb_device_finish(self, key, value, timestamp):
        """Acknowledge device finished a test step correctly and feed the test execution
        """
        self.finish = True

        try:
            if self.test_steps_sequence.send(value):
                self.notify_complete(True)
        except (StopIteration, RuntimeError) as exc:
            self.notify_complete(False)

    def cb_device_test_suit_ended(self, key, value, timestamp):
        """Acknowledge device finished a test step correctly and feed the test execution
        """
        self.suite_ended = True

        try:
            if self.test_steps_sequence.send(value):
                self.notify_complete(True)
        except (StopIteration, RuntimeError) as exc:
            self.notify_complete(False)

    #define test steps and actions
    def test_steps(self):
        """Test step 1
        """
        wait_for_communication = yield

        self.reset = False
        self.send_kv(MSG_TRNG_TEST_STEP1, MSG_VALUE_DUMMY)
        time.sleep(self.program_cycle_s)
        self.send_kv(MSG_KEY_SYNC, MSG_VALUE_DUMMY)

        wait_for_communication = yield

        if self.reset == False:
            raise RuntimeError('Phase 1: Platform did not reset as expected.')
            
        """Test step 2 (After reset)
        """
        self.finish = False
        self.send_kv(MSG_TRNG_TEST_STEP2, self.buffer)
        time.sleep(self.program_cycle_s)

        wait_for_communication = yield

        if self.finish == False:
            raise RuntimeError('Test failed.')

        wait_for_communication = yield

        if self.suite_ended == False:
            raise RuntimeError('Test failed.')

        # The sequence is correct -- test passed.
        yield True