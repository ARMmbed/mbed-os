"""
Copyright (c) 2017, Arm Limited and affiliates.
SPDX-License-Identifier: Apache-2.0

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

import threading
import os,sys
from icetea_lib.bench import Bench
from icetea_lib.TestStepError import TestStepFail

class Testcase(Bench):
    def __init__(self):
        Bench.__init__(self, name = "ED_scan",
                        title = "ED scan test",
                        status = "released",
                        type = "smoke",
                        subtype = "",
                        execution = {
                            "skip": {
                                "value": False,
                                "reason": ""
                            }
                        },
                        author = "Valtteri Erkkila",
                        purpose = "Tests reading the ED values from channels 11-16",
                        feature = ["MLME-SCAN (ED)"],
                        component = ["MAC"],
                        requirements = {
                            "duts": {
                                '*': {
                                    "count":3,
                                    "type": "hardware",
                                    "allowed_platforms": ["K64F", "K66F", "NUCLEO_F429ZI", "KW24D", "KW41Z"],
                                    "application": {
                                       "name": "TEST_APPS-device-nanostack_mac_tester"
                                    }
                                },
                                "1":{"nick": "First"},
                                "2":{"nick": "Second"},
                                "3":{"nick": "Third"}
                        }}
        )

    def setUp(self):
        self.channel = 11

    def spam_channel(self, event):
        while not event.wait(0.1):
            self.lock_th.acquire()
            self.command("First", "data --dst_addr 01:02:03:00:00:00:00:03 --msdu {} --msdu_length {} --wait_for_confirm false".format(self.payload, len(self.payload)))
            self.command("Third", "data --dst_addr 01:02:03:00:00:00:00:01 --msdu {} --msdu_length {} --wait_for_confirm false".format(self.payload, len(self.payload)))
            self.lock_th.release()

    def mask_from_channel_list(self, channels):
        res = 0
        for ch in channels:
            res = res | ( 1 << ch)
        return hex(res)

    def do_test_iteration(self):
        self.lock_th = threading.Lock()
        self.command("First", "mlme-reset")
        self.command("Second", "mlme-reset")
        self.command("Third", "mlme-reset")

        self.command("First", "addr --64-bit 01:02:03:00:00:00:00:01")
        self.command("Second", "addr --64-bit 01:02:03:00:00:00:00:02")
        self.command("Third", "addr --64-bit 01:02:03:00:00:00:00:03")

        self.payload = "01234567890123456789012345678901234567890123456789"

        # Start PAN coordinator
        self.command("First", "start --pan_coordinator true --logical_channel {}".format(self.channel))
        # Start PAN beacon
        self.command("Second", "start --pan_coordinator false --logical_channel {}".format(self.channel))
        self.command("Third", "start --pan_coordinator false --logical_channel {}".format(self.channel))

        #No reason to print their spamming
        self.command("First", "silent-mode on")
        self.command("Third", "silent-mode on")

        self.stop_event = threading.Event()
        self.th = threading.Thread(target=self.spam_channel, args=(self.stop_event,))
        self.th.start()
        self.stopped = True
        channels = range(11,27)
        for i in range(0, 3):
            self.lock_th.acquire()
            # Reset MAC settings
            self.command("First", "mlme-reset")
            # Start PAN coordinator
            self.command("First", "start --pan_coordinator true --logical_channel {}".format(self.channel))
            self.command("Third", "mlme-reset")
            # Start PAN beacon
            self.command("Third", "start --pan_coordinator false --logical_channel {}".format(self.channel))
            self.lock_th.release()
            # Scan all channels
            self.command("Second", "scan --scan_type 0 --scan_duration 7 --channel_mask {}".format(self.mask_from_channel_list(channels)))
            # Energy detection analysis
            self.command("Second", "analyze-ed --channel {} --above 100".format(self.channel))

    def case(self):
        # Try tests few times because of potential RF failures
        loop = 0
        while loop < 5:
            try:
                self.do_test_iteration()
                break
            except TestStepFail:
                self.logger.info("Warning, iteration failed #"  + str(loop+1))
                loop = loop + 1
                if (loop < 5):
                    self.stop_event.set()
                    self.th.join()
                    self.delay(5)

        else:
             raise TestStepFail("Too many failed iterations!")

    def tearDown(self):
        self.command("First", "silent-mode off")
        self.command("Third", "silent-mode off")
        self.stop_event.set()
        self.th.join()
        del self.th
        self.reset_dut()

