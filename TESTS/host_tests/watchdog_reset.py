"""
mbed SDK
Copyright (c) 2017 ARM Limited

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
import collections
import threading
from mbed_host_tests import BaseHostTest

TestCaseData = collections.namedtuple('TestCaseData', ['index', 'data_to_send'])

DEFAULT_CYCLE_PERIOD = 4.0

MSG_VALUE_DUMMY = '0'
CASE_DATA_INVALID = 0xffffffff
CASE_DATA_PHASE2_OK = 0xfffffffe

MSG_KEY_SYNC = '__sync'
MSG_KEY_DEVICE_READY = 'ready'
MSG_KEY_START_CASE = 'start_case'
MSG_KEY_DEVICE_RESET = 'dev_reset'


class WatchdogReset(BaseHostTest):
    """Host side test that handles device reset.

    Given a device with a watchdog timer started.
    When the device notifies the host about an incoming reset.
    Then the host:
    * keeps track of the test case index of the current test suite,
    * performs a dev-host handshake.
    """

    def __init__(self):
        super(WatchdogReset, self).__init__()
        self.current_case = TestCaseData(0, CASE_DATA_INVALID)
        self.__handshake_timer = None
        cycle_s = self.get_config_item('program_cycle_s')
        self.program_cycle_s = cycle_s if cycle_s is not None else DEFAULT_CYCLE_PERIOD

    def handshake_timer_start(self, seconds=1.0):
        """Start a new handshake timer.

        Perform a dev-host handshake by sending a sync message.
        """
        self.__handshake_timer = threading.Timer(seconds, self.send_kv,
                                                 [MSG_KEY_SYNC, MSG_VALUE_DUMMY])
        self.__handshake_timer.start()

    def handshake_timer_cancel(self):
        """Cancel the current handshake timer."""
        try:
            self.__handshake_timer.cancel()
        except AttributeError:
            pass
        finally:
            self.__handshake_timer = None

    def setup(self):
        self.register_callback(MSG_KEY_DEVICE_READY, self.cb_device_ready)
        self.register_callback(MSG_KEY_DEVICE_RESET, self.cb_device_reset)

    def teardown(self):
        self.handshake_timer_cancel()

    def cb_device_ready(self, key, value, timestamp):
        """Advance the device test suite to a proper test case.

        Additionally, send test case data to the device.
        """
        self.handshake_timer_cancel()
        msg_value = '{0.index:02x},{0.data_to_send:08x}'.format(self.current_case)
        self.send_kv(MSG_KEY_START_CASE, msg_value)

    def cb_device_reset(self, key, value, timestamp):
        """Keep track of the test case number.

        Also set a new handshake timeout, so when the device gets
        restarted by the watchdog, the communication will be restored
        by the __handshake_timer.
        """
        self.handshake_timer_cancel()
        case_num, dev_reset_delay_ms = (int(i, base=16) for i in value.split(','))
        self.current_case = TestCaseData(case_num, CASE_DATA_PHASE2_OK)
        self.handshake_timer_start(self.program_cycle_s + dev_reset_delay_ms / 1000.0)
