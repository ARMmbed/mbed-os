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
import string

from icetea_lib.Randomize.randomize import Randomize
from icetea_lib.bench import Bench, TestStepFail


class Testcase(Bench):
    def __init__(self):
        Bench.__init__(self,
                       name="TCPSOCKET_ECHOTEST_BURST_SHORT",
                       title="TCPSOCKET_ECHOTEST_BURST_SHORT",
                       purpose="Verify that TCPSocket can send burst of packets to echo server and read incoming packets",
                       status="released",
                       component=["mbed-os", "netsocket"],
                       author="Juha Ylinen <juha.ylinen@arm.com>",
                       type="smoke",
                       subtype="socket",
                       requirements={
                           "duts": {
                               '*': {  # requirements for all nodes
                                   "count": 1,
                                   "type": "hardware",
                                   "application": {
                                       "name": "TEST_APPS-device-socket_app"
                                   }
                               },
                               "1": {"nick": "dut1"},
                           }
                       }
                       )

    def setup(self):
        self.command("dut1", "ifup")

    def case(self):
        response = self.command("dut1", "socket new TCPSocket")
        self.socket_id = int(response.parsed['socket_id'])

        self.command("dut1", "socket " + str(self.socket_id) + " open")
        self.command("dut1", "socket " + str(self.socket_id) + " connect echo.mbedcloudtesting.com 7")

        for i in range(2):
            sentData = ""
            for size in (100, 200, 300, 120, 500):
                packet = Randomize.random_string(max_len=size, min_len=size, chars=string.ascii_uppercase)
                sentData += packet
                response = self.command("dut1", "socket " + str(self.socket_id) + " send " + str(packet))
                response.verify_trace("Socket::send() returned: " + str(size))

            received = 0
            data = ""
            totalSize = 1220
            while received < totalSize:
                response = self.command("dut1", "socket " + str(self.socket_id) + " recv " + str(totalSize))
                data += response.parsed['data'].replace(":", "")
                received += int(response.parsed['received_bytes'])

            if data != sentData:
                raise TestStepFail("Received data doesn't match the sent data")

    def teardown(self):
        self.command("dut1", "socket " + str(self.socket_id) + " delete")
        self.command("dut1", "ifdown")
