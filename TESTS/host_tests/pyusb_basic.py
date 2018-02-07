"""
mbed SDK
Copyright (c) 2018-2018 ARM Limited

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
from __future__ import print_function

from mbed_host_tests import BaseHostTest
from argparse import ArgumentParser
import time
import sys
from threading import Thread

import usb.core
from usb.util import build_request_type
from usb.util import CTRL_OUT, CTRL_IN
from usb.util import CTRL_TYPE_STANDARD, CTRL_TYPE_CLASS, CTRL_TYPE_VENDOR
from usb.util import (CTRL_RECIPIENT_DEVICE, CTRL_RECIPIENT_INTERFACE,
                      CTRL_RECIPIENT_ENDPOINT, CTRL_RECIPIENT_OTHER)

def get_interface(dev, interface, alternate=0):
    intf = None
    for active_if in dev.get_active_configuration():
        if active_if.bInterfaceNumber == interface and active_if.bAlternateSetting == alternate:
            assert intf is None, "duplicate interface"
            intf = active_if
    return intf

VENDOR_TEST_CTRL_IN = 1
VENDOR_TEST_CTRL_OUT = 2
VENDOR_TEST_CTRL_NONE = 3
VENDOR_TEST_CTRL_IN_DELAY = 4
VENDOR_TEST_CTRL_OUT_DELAY = 5
VENDOR_TEST_CTRL_NONE_DELAY = 6
VENDOR_TEST_CTRL_IN_STATUS_DELAY = 7
VENDOR_TEST_CTRL_OUT_STATUS_DELAY = 8
VENDOR_TEST_UNSUPPORTED_REQUEST = 32

class PyusbBasicTest(BaseHostTest):
    """
    """
    def _callback_usb_enumeration_done(self, key, value, timestamp):
        print("Received key %s = %s" % (key, value))
        self.log("Received key %s = %s" % (key, value))
        test_device(value, self.log)
        passed = True
        results = "pass" if passed else "fail"
        self.send_kv(results, "0")

    def setup(self):
        self.__result = False
        self.register_callback('usb_enumeration_done', self._callback_usb_enumeration_done)

    def result(self):
        return self.__result

    def teardown(self):
        pass


class TestMatch(object):

    def __init__(self, serial):
        self.serial = serial

    def __call__(self, dev):
        try:
            return dev.serial_number == self.serial
        except ValueError:
            return False


def test_device(serial_number, log=print):
        dev = usb.core.find(custom_match=TestMatch(serial_number))
        if dev is None:
            log("Device not found")
            return

        ## --Control Tests-- ##
        #control_basic_test(dev, log)
            # Test control IN/OUT/NODATA
        control_stall_test(dev, log)
            # Invalid control in/out/nodata requests are stalled
            # Stall during different points in the control transfer
        #control_sizes_test(dev, log)
            # Test control requests of various data stage sizes (1,8,16,32,64,255,256,...)
        control_stress_test(dev, log)
            # normal and delay mode

        ## --Endpoint test-- ##
        #for each endpoint
            #-test all allowed wMaxPacketSize sizes and transfer types
            #-stall tests
                #-set/clear stall control request
                #-stall at random points of sending/receiveing data
            #-test aborting an in progress transfer
        #test as many endpoints at once as possible
        #test biggest configuration possible

        ## --physical test-- ##
        #-reset notification/handling
        #-connect/disconnect tests - have device disconnect and then reconnect
            #-disconnect during various phases of control transfers and endpoint transfers
        #-suspend/resume tests (may not be possible to test with current framework)
            #-suspend/resume notifications

        ## -- Stress tests-- ##
        #-concurrent tests (all endpoints at once including control)
        #-concurrent tests + reset + delay

        ## -- other tests-- ##
        #-report throughput for in/out of control, bulk, interrupt and iso transfers
        #-verify that construction/destruction repeatedly works gracefully


        intf = get_interface(dev, 0, 0)

        # Find endpoints
        bulk_in = None
        bulk_out = None 
        int_in = None
        int_out = None
        
        for endpoint in intf:
            log("Processing endpoint %s" % endpoint)
            ep_type = endpoint.bmAttributes & 0x3
            if  ep_type == 2:
                if endpoint.bEndpointAddress & 0x80:
                    assert bulk_in is None
                    bulk_in = endpoint
                else:
                    assert bulk_out is None
                    bulk_out = endpoint
            elif ep_type == 3:
                if endpoint.bEndpointAddress & 0x80:
                    assert int_in is None
                    int_in = endpoint
                else:
                    assert int_out is None
                    int_out = endpoint
        assert bulk_in is not None
        assert bulk_out is not None
        assert int_in is not None
        assert int_out is not None
        bulk_out.write("hello" + "x" *256);
        int_out.write("world" + "x" *256);

        dev.set_interface_altsetting(0, 1)

        intf = get_interface(dev, 0, 0)

        # Find endpoints
        bulk_in = None
        bulk_out = None
        int_in = None
        int_out = None

        for endpoint in intf:
            log("Processing endpoint %s" % endpoint)
            ep_type = endpoint.bmAttributes & 0x3
            if  ep_type == 2:
                if endpoint.bEndpointAddress & 0x80:
                    assert bulk_in is None
                    bulk_in = endpoint
                else:
                    assert bulk_out is None
                    bulk_out = endpoint
            elif ep_type == 3:
                if endpoint.bEndpointAddress & 0x80:
                    assert int_in is None
                    int_in = endpoint
                else:
                    assert int_out is None
                    int_out = endpoint
        assert bulk_in is not None
        assert bulk_out is not None
        assert int_in is not None
        assert int_out is not None
        bulk_out.write("hello2" + "x" *256);
        int_out.write("world2" + "x" *256);


        t = Thread(target=write_data, args=(bulk_out,))
        t.start()

        for _ in range(10):
            request_type = build_request_type(CTRL_OUT, CTRL_TYPE_VENDOR,
                                              CTRL_RECIPIENT_DEVICE)
            request = VENDOR_TEST_CTRL_NONE_DELAY
            value = 0               # Always 0 for this request
            index = 0               # Communication interface
            length = 0              # No data
            dev.ctrl_transfer(request_type, request, value, index, length, 5000)

        t.join()

        return True

def write_data(pipe):
    print("Write data running")
    count = 0
    for _ in range(40):
        pipe.write("Value is %s" % count)
        count += 1
        print("Count %s" % count)
        time.sleep(0.5)
    
    
def control_stall_test(dev, log):

    # Control OUT stall
    try:
        request_type = build_request_type(CTRL_OUT, CTRL_TYPE_VENDOR,
                                          CTRL_RECIPIENT_DEVICE)
        request = VENDOR_TEST_UNSUPPORTED_REQUEST
        value = 0                           # Always 0 for this request
        index = 0                           # Communication interface
        data = bytearray(64)                # Dummy data
        dev.ctrl_transfer(request_type, request, value, index, data, 5000)
        raise Exception("Invalid request not stalled")
    except usb.core.USBError:
        log("Invalid request stalled")

    # Control request with no data stage (Device-to-host)
    try:
        request_type = build_request_type(CTRL_IN, CTRL_TYPE_VENDOR,
                                          CTRL_RECIPIENT_DEVICE)
        request = VENDOR_TEST_UNSUPPORTED_REQUEST
        value = 0                           # Always 0 for this request
        index = 0                           # Communication interface
        length = 0
        dev.ctrl_transfer(request_type, request, value, index, length, 5000)
        raise Exception("Invalid request not stalled")
    except usb.core.USBError:
        log("Invalid request stalled")

    # Control request with no data stage (Host-to-device)
    try:
        request_type = build_request_type(CTRL_OUT, CTRL_TYPE_VENDOR,
                                          CTRL_RECIPIENT_DEVICE)
        request = VENDOR_TEST_UNSUPPORTED_REQUEST
        value = 0                           # Always 0 for this request
        index = 0                           # Communication interface
        length = 0
        dev.ctrl_transfer(request_type, request, value, index, length, 5000)
        raise Exception("Invalid request not stalled")
    except usb.core.USBError:
        log("Invalid request stalled")

    # Control IN stall
    try:
        request_type = build_request_type(CTRL_IN, CTRL_TYPE_VENDOR,
                                          CTRL_RECIPIENT_DEVICE)
        request = VENDOR_TEST_UNSUPPORTED_REQUEST
        value = 0                           # Always 0 for this request
        index = 0                           # Communication interface
        length = 255
        dev.ctrl_transfer(request_type, request, value, index, length, 5000)
        raise Exception("Invalid request not stalled")
    except usb.core.USBError:
        log("Invalid request stalled")

    for i in (6, 7, 5):
        try:
            request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                              CTRL_RECIPIENT_DEVICE)
            request = 0x6                   # GET_DESCRIPTOR
            value = (0x03 << 8) | (i << 0)  # String descriptor index
            index = 0                       # Communication interface
            length = 255
            resp = dev.ctrl_transfer(request_type, request, value, index, length, 5000)
            log("Requesting string %s passed" % i)
        except usb.core.USBError:
            log("Requesting string %s failed" % i)
                
                
def control_stress_test(dev, log):

    # Test various patterns of control transfers
    #
    # Some devices have had problems with back-to-back
    # control transfers. Intentionally send these sequences
    # to make sure they are properly handled.
    count = 0
    for _ in range(100):
        # Control transfer with a data in stage
        request_type = build_request_type(CTRL_IN, CTRL_TYPE_VENDOR,
                                          CTRL_RECIPIENT_DEVICE)
        request = VENDOR_TEST_CTRL_IN
        value = 8                           # Size of data the device should actually send
        index = count                       # Unused - set for debugging only
        length = 255
        dev.ctrl_transfer(request_type, request, value, index, length, 5000)
        count += 1

    for _ in range(100):
        # Control transfer with a data out stage followed
        # by a control transfer with a data in stage
        request_type = build_request_type(CTRL_OUT, CTRL_TYPE_VENDOR,
                                          CTRL_RECIPIENT_DEVICE)
        request = VENDOR_TEST_CTRL_OUT
        value = 8                           # Size of data the device should actually read
        index = count                       # Unused - set for debugging only
        data = bytearray(8)                 # Dummy data
        dev.ctrl_transfer(request_type, request, value, index, data, 5000)
        count += 1

        request_type = build_request_type(CTRL_IN, CTRL_TYPE_VENDOR,
                                          CTRL_RECIPIENT_DEVICE)
        request = VENDOR_TEST_CTRL_IN
        value = 8                           # Size of data the device should actually send
        index = count                       # Unused - set for debugging only
        length = 255
        dev.ctrl_transfer(request_type, request, value, index, length, 5000)
        count += 1

    for _ in range(100):
        # Control transfer with a data out stage
        request_type = build_request_type(CTRL_OUT, CTRL_TYPE_VENDOR,
                                          CTRL_RECIPIENT_DEVICE)
        request = VENDOR_TEST_CTRL_OUT
        value = 8                           # Size of data the device should actually read
        index = count                       # Unused - set for debugging only
        data = bytearray(8)                 # Dummy data
        dev.ctrl_transfer(request_type, request, value, index, data, 5000)
        count += 1


def main():
    parser = ArgumentParser(description="USB basic test")
    parser.add_argument('serial', help='USB serial number of DUT')
    args = parser.parse_args()
    ret = test_device(args.serial)
    print("Test %s" % "passed" if ret else "failed")


if __name__ == "__main__":
    main()
