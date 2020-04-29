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
from icetea_lib.TestStepError import TestStepFail

'''
This interface script is intended to be a common library to be used in testcase scripts by testers.
It delegates setUp and tearDown functions with different provided network interface types using setUp() and tearDown() methods.
'''


def interfaceUp(tc, duts):
    interfaces = {}
    for dut in duts:
        interface = {dut: {"ipv4": None, "ipv6": None}}

        resp = tc.command("%s" % dut, "ifup")

        ip = interface[dut]["ip"] = interface[dut]["ipv4"] = resp.parsed["address"]["ipv4"]
        if not ip:
            if resp.parsed["address"]["ipv6"]:
                ip = interface[dut]["ip"] = interface[dut]["ipv6"] = resp.parsed["address"]["ipv6"][0]
        if not ip:
            raise TestStepFail("Failed to parse IP address")

        interfaces.update(interface)
    return interfaces


def interfaceDown(tc, duts):
    for dut in duts:
        tc.command(dut, "ifdown")
