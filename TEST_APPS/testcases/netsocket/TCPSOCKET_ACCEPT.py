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

import threading
import time

from icetea_lib.bench import Bench
from interface import interfaceUp, interfaceDown
#from mbed_clitest.tools import test_case
#from mbed_clitest.TestStepError import SkippedTestcaseException
from icetea_lib.TestStepError import TestStepFail

class Testcase(Bench):
    def __init__(self):
        Bench.__init__(self,
                       name="TCPSOCKET_ACCEPT",
                       title = "TCPSOCKET_ACCEPT",
                       purpose = "Test that TCPSocket::bind(), TCPSocket::listen() and TCPSocket::accept() works",
                       status = "released",
                       component= ["mbed-os", "netsocket"],
                       type="smoke",
                       subtype="socket",
                       requirements={
                           "duts": {
                               '*': { #requirements for all nodes
                                    "count":2,
                                    "type": "hardware",
                                    "application": {
                                       "name": "TEST_APPS-device-socket_app"
                                    }
                                },
                                "1": {"nick": "dut1"},
                                "2": {"nick": "dut2"}
                           }
                       }
        )

    def setup(self):
        interface = interfaceUp(self, ["dut1"])
        self.server_ip = interface["dut1"]["ip"]
        interface = interfaceUp(self, ["dut2"])
        self.client_ip = interface["dut2"]["ip"]

    def clientThread(self):
        self.logger.info("Starting")
        time.sleep(5) #wait accept from server
        self.command("dut2", "socket " + str(self.client_socket_id) + " open")
        self.command("dut2", "socket " + str(self.client_socket_id) + " connect " + str(self.server_ip) + " " + str(self.used_port))


    def case(self):
        self.used_port = 2000

        response = self.command("dut1", "socket new TCPSocket")
        self.server_base_socket_id = int(response.parsed['socket_id'])

        self.command("dut1", "socket " + str(self.server_base_socket_id) + " open")
        response = self.command("dut1", "socket " + str(self.server_base_socket_id) + " bind port " + str(self.used_port), report_cmd_fail = False)
        if response.retcode == -1:
            if (response.verify_trace("NSAPI_ERROR_UNSUPPORTED", break_in_fail = False)):
                raise SkippedTestcaseException("UNSUPPORTED")
            else:
                TestStepFail("Bind port failed")
        self.command("dut1", "socket " + str(self.server_base_socket_id) + " listen")

        response = self.command("dut2", "socket new TCPSocket")
        self.client_socket_id = int(response.parsed['socket_id'])

        #Create a thread which calls client connect()
        t = threading.Thread(name='clientThread', target=self.clientThread)
        t.start()

        response = self.command("dut1", "socket " + str(self.server_base_socket_id) + " accept")

        t.join()
        self.accept_socket_id = int(response.parsed['socket_id'])
        if response.timedelta < 5.0: # Check that socket accept call blocks
            raise TestStepFail("Longer response time expected")

        self.command("dut1", "socket " + str(self.accept_socket_id) + " send hello")

        response = self.command("dut2", "socket " + str(self.client_socket_id) + " recv 5")
        data = response.parsed['data'].replace(":","")

        if data != "hello":
            raise TestStepFail("Received data doesn't match the sent data")

    def teardown(self):
        response = self.command("dut2", "socket " + str(self.client_socket_id) + " delete")
        response = self.command("dut1", "socket " + str(self.server_base_socket_id) + " delete")
        response = self.command("dut1", "socket " + str(self.accept_socket_id) + " close") 
        
        interfaceDown(self, ["dut1"])
        interfaceDown(self, ["dut2"])
