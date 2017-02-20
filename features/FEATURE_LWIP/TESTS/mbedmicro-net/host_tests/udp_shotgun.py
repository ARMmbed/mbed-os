"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

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

import sys
import socket
import json
import random
import itertools
import time
from sys import stdout
from threading import Thread
from SocketServer import BaseRequestHandler, UDPServer
from mbed_host_tests import BaseHostTest, event_callback


class UDPEchoClientHandler(BaseRequestHandler):
    def handle(self):
        """ UDP packet handler. Responds with multiple simultaneous packets
        """
        data, sock = self.request
        pattern = [ord(d) << 4 for d in data]

        # Each byte in request indicates size of packet to recieve
        # Each packet size is shifted over by 4 to fit in a byte, which
        # avoids any issues with endianess or decoding
        for packet in pattern:
            data = [random.randint(0, 255) for _ in range(packet-1)]
            data.append(reduce(lambda a,b: a^b, data))
            data = ''.join(map(chr, data))
            sock.sendto(data, self.client_address)

            # Sleep a tiny bit to compensate for local network
            time.sleep(0.01)


class UDPEchoClientTest(BaseHostTest):
    def __init__(self):
        """
        Initialise test parameters.

        :return:
        """
        BaseHostTest.__init__(self)
        self.SERVER_IP = None # Will be determined after knowing the target IP
        self.SERVER_PORT = 0  # Let TCPServer choose an arbitrary port
        self.server = None
        self.server_thread = None
        self.target_ip = None

    @staticmethod
    def find_interface_to_target_addr(target_ip):
        """
        Finds IP address of the interface through which it is connected to the target.

        :return:
        """
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        try:
            s.connect((target_ip, 0)) # Target IP, any port
        except socket.error:
            s.connect((target_ip, 8000)) # Target IP, 'random' port
        ip = s.getsockname()[0]
        s.close()
        return ip

    def setup_udp_server(self):
        """
        sets up a UDP server for target to connect and send test data.

        :return:
        """
        # !NOTE: There should mechanism to assert in the host test
        if self.SERVER_IP is None:
            self.log("setup_udp_server() called before determining server IP!")
            self.notify_complete(False)

        # Returning none will suppress host test from printing success code
        self.server = UDPServer((self.SERVER_IP, self.SERVER_PORT), UDPEchoClientHandler)
        ip, port = self.server.server_address
        self.SERVER_PORT = port
        self.server.allow_reuse_address = True
        self.log("HOST: Listening for UDP packets: " + self.SERVER_IP + ":" + str(self.SERVER_PORT))
        self.server_thread = Thread(target=UDPEchoClientTest.server_thread_func, args=(self,))
        self.server_thread.start()

    @staticmethod
    def server_thread_func(this):
        """
        Thread function to run TCP server forever.

        :param this:
        :return:
        """
        this.server.serve_forever()

    @event_callback("target_ip")
    def _callback_target_ip(self, key, value, timestamp):
        """
        Callback to handle reception of target's IP address.

        :param key:
        :param value:
        :param timestamp:
        :return:
        """
        self.target_ip = value
        self.SERVER_IP = self.find_interface_to_target_addr(self.target_ip)
        self.setup_udp_server()

    @event_callback("host_ip")
    def _callback_host_ip(self, key, value, timestamp):
        """
        Callback for request for host IP Addr

        """
        self.send_kv("host_ip", self.SERVER_IP)

    @event_callback("host_port")
    def _callback_host_port(self, key, value, timestamp):
        """
        Callback for request for host port
        """
        self.send_kv("host_port", self.SERVER_PORT)

    def teardown(self):
        if self.server:
            self.server.shutdown()
            self.server_thread.join()
