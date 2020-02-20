"""
Copyright 2018 ARM Limited
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

from icetea_lib.bench import Bench


class Testcase(Bench):
    def __init__(self):
        Bench.__init__(self,
                       name="test_cmdline",
                       title="Smoke test for command line interface",
                       status="released",
                       purpose="Verify Command Line Interface",
                       component=["cmdline"],
                       type="smoke",
                       requirements={
                           "duts": {
                               '*': {
                                   "count": 1,
                                   "type": "hardware",
                                   "application": {
                                       "name": "TEST_APPS-device-exampleapp"
                                   }
                               },
                               "1": {"nick": "dut1"},
                           }
                       }
                       )

    def setup(self):
        pass

    def case(self):
        self.command("dut1", "echo hello world")
        self.command("dut1", "help")

    def teardown(self):
        pass
