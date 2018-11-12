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

import os,sys
from icetea_lib.bench import Bench

class Testcase(Bench):
    def __init__(self):
        Bench.__init__(self, name = "create_and_join_PAN",
                        title = "Create a PAN and have a device join it",
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
                        purpose = "",
                        feature = ["MLME-START", "MLME-SCAN (active)"],
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

    def mask_from_channel_list(self, channels):
        res = 0
        for ch in channels:
            res = res | ( 1 << ch)
        return hex(res)

    def setUp(self):
        self.channel = 11

    def case(self):
        #Beacon payload & length
        self.command("First", "mlme-set --attr 0x45 --value_ascii mac-tester --value_size 10")
        self.command("First", "mlme-set --attr 0x46 --value_uint8 10 --value_size 1")

        self.command("Second", "mlme-set --attr 0x45 --value_ascii second-mac-tester --value_size 17")
        self.command("Second", "mlme-set --attr 0x46 --value_uint8 17 --value_size 1")

        self.command("First", "start --pan_coordinator true --logical_channel {}".format(self.channel))
        self.command("Second", "start --pan_coordinator true --logical_channel {}".format(int(self.channel)+1))
        self.delay(3)
        if self.channel == 11:
            channels = [11,12]
        elif self.channel == 26:
            channels = [25,26]
        else:
            channels = [self.channel, self.channel+1]
        self.command("Third", "scan --channel_mask {}".format(self.mask_from_channel_list(channels)))
        self.delay(0.2)
        self.command("Third", "find-beacon --data mac-tester")
        self.command("Third", "find-beacon --data second-mac-tester")

    def tearDown(self):
        self.reset_dut()
