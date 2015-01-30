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

from host_registry import HostRegistry
from default_auto import DefaultAuto
from hello_auto import HelloTest
from wait_us_auto import WaitusTest
from stdio_auto import StdioTest
from dev_null_auto import DevNullTest
from rtc_auto import RTCTest
from echo import EchoTest
from detect_auto import DetectPlatformTest
from tcpecho_server_auto import TCPEchoServerTest
from udpecho_server_auto import UDPEchoServerTest
from tcpecho_client_auto import TCPEchoClientTest
from udpecho_client_auto import UDPEchoClientTest

HOSTREGISTRY = HostRegistry()
HOSTREGISTRY.register_host_test("default", DefaultAuto())
HOSTREGISTRY.register_host_test("default_auto", DefaultAuto())
HOSTREGISTRY.register_host_test("hello_auto", HelloTest())
HOSTREGISTRY.register_host_test("wait_us_auto", WaitusTest())
HOSTREGISTRY.register_host_test("stdio_auto", StdioTest())
HOSTREGISTRY.register_host_test("dev_null_auto", DevNullTest())
HOSTREGISTRY.register_host_test("rtc_auto", RTCTest())
HOSTREGISTRY.register_host_test("echo", EchoTest())
HOSTREGISTRY.register_host_test("detect_auto", DetectPlatformTest())
HOSTREGISTRY.register_host_test("tcpecho_server_auto", TCPEchoServerTest())
HOSTREGISTRY.register_host_test("udpecho_server_auto", UDPEchoServerTest())
HOSTREGISTRY.register_host_test("tcpecho_client_auto", TCPEchoClientTest())
HOSTREGISTRY.register_host_test("udpecho_client_auto", UDPEchoClientTest())

###############################################################################
# Functional interface for test supervisor registry
###############################################################################


def get_host_test(ht_name):
    return HOSTREGISTRY.get_host_test(ht_name)

def is_host_test(ht_name):
    return HOSTREGISTRY.is_host_test(ht_name)
