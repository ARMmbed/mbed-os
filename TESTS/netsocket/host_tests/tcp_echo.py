# Copyright 2015 ARM Limited, All rights reserved
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import sys
import select
import socket
import logging
from threading import Thread
from sys import stdout
from SocketServer import BaseRequestHandler, TCPServer
from mbed_host_tests import BaseHostTest, event_callback


class TCPEchoClientHandler(BaseRequestHandler):
    def handle(self):
        """
        Handles a connection. Test starts by client(i.e. mbed) connecting to server.
        This connection handler receives data and echoes back to the client util
        {{end}} is received. Then it sits on recv() for client to terminate the
        connection.

        Note: reason for not echoing data back after receiving {{end}} is that send
              fails raising a SocketError as client closes connection.
        """
        while self.server.isrunning():
            try:
                data = self.recv()
                if not data: break
            except Exception as e:
                break

            try:
                # echo data back to the client
                self.send(data)
            except Exception as e:
                break

    def recv(self):
        """
        Try to receive until server is shutdown
        """
        while self.server.isrunning():
            rl, wl, xl = select.select([self.request], [], [], 1)
            if len(rl):
                return self.request.recv(1024)

    def send(self, data):
        """
        Try to send until server is shutdown
        """
        while self.server.isrunning():
            rl, wl, xl = select.select([], [self.request], [], 1)
            if len(wl):
                self.request.sendall(data)
                break


class TCPServerWrapper(TCPServer):
    """
    Wrapper over TCP server to implement server initiated shutdown.
    Adds a flag:= running that a request handler can check and come out of
    recv loop when shutdown is called.
    """

    def __init__(self, addr, request_handler):
        # hmm, TCPServer is not sub-classed from object!
        if issubclass(TCPServer, object):
            super(TCPServerWrapper, self).__init__(addr, request_handler)
        else:
            TCPServer.__init__(self, addr, request_handler)
        self.running = False

    def serve_forever(self):
        self.running = True
        if issubclass(TCPServer, object):
            super(TCPServerWrapper, self).serve_forever()
        else:
            TCPServer.serve_forever(self)

    def shutdown(self):
        self.running = False
        if issubclass(TCPServer, object):
            super(TCPServerWrapper, self).shutdown()
        else:
            TCPServer.shutdown(self)

    def isrunning(self):
        return self.running


class TCPEchoClientTest(BaseHostTest):

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

    def setup_tcp_server(self):
        """
        sets up a TCP server for target to connect and send test data.

        :return:
        """
        # !NOTE: There should mechanism to assert in the host test
        if self.SERVER_IP is None:
            self.log("setup_tcp_server() called before determining server IP!")
            self.notify_complete(False)

        # Returning none will suppress host test from printing success code
        self.server = TCPServerWrapper((self.SERVER_IP, self.SERVER_PORT), TCPEchoClientHandler)
        ip, port = self.server.server_address
        self.SERVER_PORT = port
        self.server.allow_reuse_address = True
        self.log("HOST: Listening for TCP connections: " + self.SERVER_IP + ":" + str(self.SERVER_PORT))
        self.server_thread = Thread(target=TCPEchoClientTest.server_thread_func, args=(self,))
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
        self.setup_tcp_server()

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
