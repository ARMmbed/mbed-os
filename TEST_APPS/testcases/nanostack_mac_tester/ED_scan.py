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

class Testcase(Bench):
    def __init__(self):
        Bench.__init__(self, name = "ED_scan",
                        title = "ED scan test",
                        status = "development",
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
                                    "allowed_platforms": ["K64F", "K66F", "NUCLEO_F429ZI", "KW24D", "UBLOX_EVK_ODIN_W2"],
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
        self.command("First", "addr --64-bit 01:02:03:00:00:00:00:01")
        self.command("Second", "addr --64-bit 01:02:03:00:00:00:00:02")
        self.command("Third", "addr --64-bit 01:02:03:00:00:00:00:03")

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

    def case(self):
        self.lock_th = threading.Lock()
        self.payload = "01234567890123456789012345678901234567890123456789"

        self.command("First", "start --pan_coordinator true --logical_channel {}".format(self.channel))
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
            self.command("First", "mlme-reset")
            self.command("First", "start --pan_coordinator true --logical_channel {}".format(self.channel))
            self.command("Third", "mlme-reset")
            self.command("Third", "start --pan_coordinator false --logical_channel {}".format(self.channel))
            self.lock_th.release()
            self.command("Second", "scan --scan_type 0 --scan_duration 7 --channel_mask {}".format(self.mask_from_channel_list(channels)))
            self.command("Second", "analyze-ed --channel {} --above 100".format(self.channel))

    def tearDown(self):
        self.command("First", "silent-mode off")
        self.command("Third", "silent-mode off")
        self.stop_event.set()
        self.th.join()
        del self.th
        self.reset_dut()
