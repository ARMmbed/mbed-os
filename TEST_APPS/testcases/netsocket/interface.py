#
# Copyright (c) 2016-2018, Arm Limited and affiliates.
#
from icetea_lib.TestStepError import TestStepFail
'''
This interface script is intended to be a common library to be used in testcase scripts by testers.
It delegates setUp and tearDown functions with different provided network interface types using setUp() and tearDown() methods.
'''

def interfaceUp(tc, duts):
    interfaces = {}
    for dut in duts:
        interface = {dut:{"ipv4": None, "ipv6": None}}

        resp = tc.command("%s" % dut, "ifup")

        ip = interface[dut]["ip"] = interface[dut]["ipv4"] = resp.parsed["address"]["ipv4"]
        if not ip:
            if resp.parsed["address"]["ipv6"] != []:
                ip = interface[dut]["ip"] = interface[dut]["ipv6"] = resp.parsed["address"]["ipv6"][0]
        if not ip:
            raise TestStepFail("Failed to parse IP address")

        interfaces.update(interface)
    return interfaces

def interfaceDown(tc, duts):
    for dut in duts:
        tc.command(dut, "ifdown")
