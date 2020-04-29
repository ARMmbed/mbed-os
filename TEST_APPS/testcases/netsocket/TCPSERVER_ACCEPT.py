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

from icetea_lib.TestStepError import TestStepFail
from icetea_lib.bench import Bench
from interface import interfaceUp, interfaceDown


class Testcase(Bench):
    def __init__(self):
        Bench.__init__(self,
                       name="TCPSERVER_ACCEPT",
                       title="TCPSERVER_ACCEPT",
                       purpose="Test that TCPServer::bind(), TCPServer::listen() and TCPServer::accept() works",
                       status="released",
                       component=["mbed-os", "netsocket"],
                       author="Juha Ylinen <juha.ylinen@arm.com>",
                       type="smoke",
                       subtype="socket",
                       requirements={
                           "duts": {
                               '*': {  # requirements for all nodes
                                   "count": 2,
                                   "type": "hardware",
                                   "application": {"name": "TEST_APPS-device-socket_app"}
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
        time.sleep(5)  # wait accept from server
        self.command("dut2", "socket " + str(self.client_socket_id) + " open")
        self.command("dut2", "socket " + str(self.client_socket_id) + " connect " + str(self.server_ip) + " " + str(
            self.used_port))

    def case(self):
        self.used_port = 2000

        response = self.command("dut1", "socket new TCPServer")
        self.server_base_socket_id = int(response.parsed['socket_id'])

        self.command("dut1", "socket " + str(self.server_base_socket_id) + " open")
        self.command("dut1", "socket " + str(self.server_base_socket_id) + " bind port " + str(self.used_port))
        self.command("dut1", "socket " + str(self.server_base_socket_id) + " listen")

        response = self.command("dut1", "socket new TCPSocket")
        self.server_socket_id = int(response.parsed['socket_id'])
        self.command("dut1", "socket " + str(self.server_socket_id) + " open")

        response = self.command("dut2", "socket new TCPSocket")
        zero = response.timedelta
        self.client_socket_id = int(response.parsed['socket_id'])

        # Create a thread which calls client connect()
        t = threading.Thread(name='clientThread', target=self.clientThread)
        t.start()

        wait = 5
        response = self.command("dut1", "socket " + str(self.server_base_socket_id) + " accept " + str(self.server_socket_id))
        response.verify_response_duration(expected=wait, zero=zero, threshold_percent=10, break_in_fail=True)
        socket_id = int(response.parsed['socket_id'])

        t.join()
        self.command("dut1", "socket " + str(socket_id) + " send hello")

        response = self.command("dut2", "socket " + str(self.client_socket_id) + " recv 5")
        data = response.parsed['data'].replace(":", "")

        if data != "hello":
            raise TestStepFail("Received data doesn't match the sent data")

    def teardown(self):
        self.command("dut1", "socket " + str(self.server_socket_id) + " delete")
        self.command("dut1", "socket " + str(self.server_base_socket_id) + " delete")
        self.command("dut2", "socket " + str(self.client_socket_id) + " delete")
        
        interfaceDown(self, ["dut1"])
        interfaceDown(self, ["dut2"])
