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
from SocketServer import BaseRequestHandler, UDPServer
from host_test import Test
import socket
from sys import stdout

SERVER_IP = str(socket.gethostbyname(socket.getfqdn()))
SERVER_PORT = 7

class UDPEchoClientTest(Test):
    def __init__(self):
        Test.__init__(self)
        self.mbed.init_serial()

    def send_server_ip_port(self, ip_address, port_no):
        print "Resetting target..."
        self.mbed.reset()
        print "Sending server IP Address to target..."
        connection_str = ip_address + ":" + str(port_no) + "\n"
        self.mbed.serial.write(connection_str)


class UDPEchoClient_Handler(BaseRequestHandler):
    def print_result(self, result):
        print "\n{%s}\n{end}" % result

    def handle(self):
        """ One handle per connection """
        print "connection received"
        data, socket = self.request
        print "client: ", self.client_address
        print "data: ", data
        socket.sendto(data, self.client_address)
        stdout.flush()


server = UDPServer((SERVER_IP, SERVER_PORT), UDPEchoClient_Handler)
print "listening for connections"

mbed_test = UDPEchoClientTest();
mbed_test.send_server_ip_port(SERVER_IP, SERVER_PORT)

server.serve_forever()
