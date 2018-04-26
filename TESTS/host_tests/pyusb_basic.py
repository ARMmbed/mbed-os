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
import inspect
from threading import Thread

import usb.core
from usb.util import build_request_type
from usb.util import CTRL_OUT, CTRL_IN
from usb.util import CTRL_TYPE_STANDARD, CTRL_TYPE_CLASS, CTRL_TYPE_VENDOR
from usb.util import (CTRL_RECIPIENT_DEVICE, CTRL_RECIPIENT_INTERFACE,
                      CTRL_RECIPIENT_ENDPOINT, CTRL_RECIPIENT_OTHER)
from usb.util import (DESC_TYPE_DEVICE, DESC_TYPE_CONFIG, DESC_TYPE_STRING,
                      DESC_TYPE_INTERFACE, DESC_TYPE_ENDPOINT)

import struct
from collections import namedtuple

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
VENDOR_TEST_CTRL_IN_SIZES = 9
VENDOR_TEST_CTRL_OUT_SIZES = 10
VENDOR_TEST_UNSUPPORTED_REQUEST = 32

REQUEST_GET_STATUS = 0
REQUEST_CLEAR_FEATURE = 1
REQUEST_SET_FEATURE = 3
REQUEST_SET_ADDRESS = 5
REQUEST_GET_DESCRIPTOR = 6
REQUEST_SET_DESCRIPTOR = 7
REQUEST_GET_CONFIGURATION = 8
REQUEST_SET_CONFIGURATION = 9
REQUEST_GET_INTERFACE = 10
REQUEST_SET_INTERFACE = 11
REQUEST_SYNCH_FRAME = 12

FEATURE_ENDPOINT_HALT = 0
FEATURE_DEVICE_REMOTE_WAKEUP = 1



DEVICE_QUALIFIER_DESC_SIZE = 10

DESC_TYPE_DEVICE_QUALIFIER = 0x06


DEVICE_DESC_SIZE = 18
device_descriptor_parser = struct.Struct('BBHBBBBHHHBBBB')
device_descriptor_keys = ['bLength', 'bDescriptorType', 'bcdUSB', 'bDeviceClass',
                          'bDeviceSubClass', 'bDeviceProtocol', 'bMaxPacketSize0',
                          'idVendor', 'idProduct', 'bcdDevice', 'iManufacturer',
                          'iProduct', 'iSerialNumber', 'bNumConfigurations']

CONFIGURATION_DESC_SIZE = 9
configuration_descriptor_parser = struct.Struct('BBHBBBBB')
configuration_descriptor_keys = ['bLength', 'bDescriptorType', 'wTotalLength',
                                 'bNumInterfaces', 'bConfigurationValue',
                                 'iConfiguration', 'bmAttributes', 'bMaxPower']

INTERFACE_DESC_SIZE = 9
interface_descriptor_parser = struct.Struct('BBBBBBBBB')
interface_descriptor_keys = ['bLength', 'bDescriptorType', 'bInterfaceNumber',
                             'bAlternateSetting', 'bNumEndpoints',
                             'bInterfaceClass', 'bInterfaceSubClass',
                             'bInterfaceProtocol', 'iInterface']

ENDPOINT_DESC_SIZE = 7
interface_descriptor_parser = struct.Struct('BBBBBHB')
interface_descriptor_keys = ['bLength', 'bDescriptorType', 'bEndpointAddress',
                             'bmAttributes', 'wMaxPacketSize', 'bInterval']

class PyusbBasicTest(BaseHostTest):

    def _callback_control_basic_test(self, key, value, timestamp):
        serial_number, vendor_id, product_id = value.split(' ')
        self.log("Received serial %s" % (serial_number))
        self.log("Received vendor_id %s" % (vendor_id))
        self.log("Received product_id %s" % (product_id))

        dev = self.find_device(serial_number)
        if(dev == None):
            return

        try:
            control_basic_test(dev, vendor_id, product_id, log=print)
            self.report_success()
        except (RuntimeError) as exc:
            self.report_error(exc)


    def _callback_control_stall_test(self, key, value, timestamp):
        self.log("Received serial %s" % (value))

        dev = self.find_device(value)
        if(dev == None):
            return

        try:
            control_stall_test(dev, log=print)
            self.report_success()
        except (RuntimeError) as exc:
            self.report_error(exc)


    def _callback_control_sizes_test(self, key, value, timestamp):
        self.log("Received serial %s" % (value))

        dev = self.find_device(value)
        if(dev == None):
            return

        try:
            control_sizes_test(dev, log=print)
            self.report_success()
        except (RuntimeError) as exc:
            self.report_error(exc)


    def _callback_control_stress_test(self, key, value, timestamp):
        self.log("Received serial %s" % (value))

        dev = self.find_device(value)
        if(dev == None):
            return

        try:
            control_stress_test(dev, log=print)
            self.report_success()
        except (RuntimeError) as exc:
            self.report_error(exc)


    def _callback_device_reset_test(self, key, value, timestamp):
        self.log("Received serial %s" % (value))

        dev = self.find_device(value)
        if(dev == None):
            return

        try:
            self.device_reset_test.send(dev)
            self.report_success()
        except (RuntimeError) as exc:
            self.report_error(exc)

    def _callback_device_soft_reconnection_test(self, key, value, timestamp):
        self.log("Received serial %s" % (value))

        dev = self.find_device(value)
        if(dev == None):
            return

        try:
            self.device_soft_reconnection_test.send(dev)
            self.report_success()
        except (RuntimeError) as exc:
            self.report_error(exc)


    def _callback_device_suspend_resume_test(self, key, value, timestamp):
        self.log("Received serial %s" % (value))

        dev = self.find_device(value)
        if(dev == None):
            return

        try:
            self.device_suspend_resume_test.send(dev)
            self.report_success()
        except (RuntimeError) as exc:
            self.report_error(exc)


    def _callback_repeated_construction_destruction_test(self, key, value, timestamp):
        self.log("Received serial %s" % (value))

        dev = self.find_device(value)
        if(dev == None):
            return

        try:
            self.repeated_construction_destruction_test.send(dev)
            self.report_success()
        except (RuntimeError) as exc:
            self.report_error(exc)

    def _callback_reset_support(self, key, value, timestamp):
        status = "false" if sys.platform == "darwin" else "true"
        self.log("Reset supported: %s" % status)
        self.send_kv("placeholder", status)

    def find_device(self, serial_number):
        # to make it more reliable, 20 retries in 2[s]
        for _ in range(20):
            dev = usb.core.find(custom_match=TestMatch(serial_number))
            if dev is not None:
                break
            time.sleep(0.1)

        if dev is None:
            self.log("Device not found")
            self.send_kv("failed", "0")
        return dev


    def report_success(self):
        self.send_kv("pass", "0")

    def report_error(self, msg):
        self.log('TEST FAILED: {}'.format(msg))
        self.send_kv("failed", "0")

    def setup(self):
        self.__result = False

        self.device_reset_test = device_reset_test(log=print)
        self.device_reset_test.send(None)

        self.device_soft_reconnection_test = device_soft_reconnection_test(log=print)
        self.device_soft_reconnection_test.send(None)

        self.device_suspend_resume_test = device_suspend_resume_test(log=print)
        self.device_suspend_resume_test.send(None)

        self.repeated_construction_destruction_test = repeated_construction_destruction_test(log=print)
        self.repeated_construction_destruction_test.send(None)

        self.register_callback('control_basic_test', self._callback_control_basic_test)
        self.register_callback('control_stall_test', self._callback_control_stall_test)
        self.register_callback('control_sizes_test', self._callback_control_sizes_test)
        self.register_callback('control_stress_test', self._callback_control_stress_test)
        self.register_callback('device_reset_test', self._callback_device_reset_test)
        self.register_callback('device_soft_reconnection_test', self._callback_device_soft_reconnection_test)
        self.register_callback('device_suspend_resume_test', self._callback_device_suspend_resume_test)
        self.register_callback('repeated_construction_destruction_test', self._callback_repeated_construction_destruction_test)

        self.register_callback('reset_support', self._callback_reset_support)


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


def lineno():
    """Returns the current line number in our program."""
    return inspect.currentframe().f_back.f_lineno


def raise_if_different(expected, actual, line, text=''):
    """Raise a RuntimeError if actual is different than expected."""
    if expected != actual:
        raise RuntimeError('[{}]:{}, {} Got {!r}, expected {!r}'.format(__file__, line, text, actual, expected))


def raise_unconditionally(line, text=''):
    """Raise a RuntimeError unconditionally."""
    raise RuntimeError('[{}]:{}, {}'.format(__file__, line, text))


def control_basic_test(dev, vendor_id, product_id, log):
    get_status_test(dev, log)
    set_clear_feature_test(dev, log)
    get_set_interface_test(dev, log)
    get_set_configuration_test(dev, log)
    get_descriptor_test(dev, vendor_id, product_id, log)
    set_descriptor_test(dev, log)
    #synch_frame_test(dev, log) wait for isochronous endpoint


def get_status_test(dev, log):
    # Control IN GET_STATUS on DEVICE
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_DEVICE)
    request = REQUEST_GET_STATUS
    value = 0                                   # Always 0 for this request
    index = 0                                   # 0 if recipient is device
    length = 2                                  # Always 2 for this request (size of return data)
    ret = dev.ctrl_transfer(request_type, request, value, index, length)
    ret = ret[0] | (ret[1] << 8)
    # Status bits
    # ret == 0b01    (D0)Self Powered
    # ret == 0b10    (D1)Remote Wakeup
    # (D2 - D15 reserved) Must be set to 0
    if(ret < 0 or ret > 3):
        raise_unconditionally(lineno(), "GET_STATUS on DEVICE failed")

    # Control IN GET_STATUS on INTERFACE
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_INTERFACE)
    request = REQUEST_GET_STATUS
    value = 0                                   # Always 0 for this request
    index = 0                                   # interface index
    length = 2                                  # Always 2 for this request (size of return data)
    ret = dev.ctrl_transfer(request_type, request, value, index, length)
    ret = ret[0] | (ret[1] << 8)
    # Status bits
    # ret == 0b0
    # (D0 - D15 reserved) Must be set to 0
    if(ret != 0):
        raise_unconditionally(lineno(), "GET_STATUS on INTERFACE failed")

    # Control IN GET_STATUS on ENDPOINT 0
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_ENDPOINT)
    request = REQUEST_GET_STATUS
    value = 0                                   # Always 0 for this request
    index = 0                                   # endpoint index
    length = 2                                  # Always 2 for this request (size of return data)
    ret = dev.ctrl_transfer(request_type, request, value, index, length)
    ret = ret[0] | (ret[1] << 8)
    # Status bits
    # ret == 0b1    (D0)endpoint Halt
    # (D1 - D15 reserved) Must be set to 0
    # endpoint 0 can't be halted ret == 0
    if(ret != 0):
        raise_unconditionally(lineno(), "GET_STATUS on ENDPOINT failed")


def set_clear_feature_test(dev, log):
    # Control OUT SET_FEATURE on endpoint - halt
    request_type = build_request_type(CTRL_OUT, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_ENDPOINT)
    request = REQUEST_SET_FEATURE
    value = FEATURE_ENDPOINT_HALT
    index = 1                                   # Endpoint index
    length = 0                                  # Always 0 for this request
    try:
        dev.ctrl_transfer(request_type, request, value, index, length)
    except usb.core.USBError:
        raise_unconditionally(lineno(), "endpoint halt failed")

    # check if endpoint was halted
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_ENDPOINT)
    request = REQUEST_GET_STATUS
    value = 0                                   # Always 0 for this request
    index = 1                                   # Endpoint index
    length = 2                                  # Always 2 for this request (size of return data)
    ret = dev.ctrl_transfer(request_type, request, value, index, length)
    ret = ret[0] | (ret[1] << 8)
    if(ret != 1):
        raise_unconditionally(lineno(), "endpoint was not halted")

    # Control OUT CLEAR_FEATURE on endpoint - unhalt
    request_type = build_request_type(CTRL_OUT, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_ENDPOINT)
    request = REQUEST_CLEAR_FEATURE
    value = FEATURE_ENDPOINT_HALT
    index = 1                                   # Endpoint index
    length = 0                                  # Always 0 for this request
    try:
        dev.ctrl_transfer(request_type, request, value, index, length)
    except usb.core.USBError:
        raise_unconditionally(lineno(), "endpoint was not unhalted")

    # check if endpoint was unhalted
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_ENDPOINT)
    request = REQUEST_GET_STATUS
    value = 0                                   # Always 0 for this request
    index = 1                                   # Endpoint index
    length = 2                                  # Always 2 for this request (size of return data)
    ret = dev.ctrl_transfer(request_type, request, value, index, length)
    ret = ret[0]
    if(ret != 0):
        raise_unconditionally(lineno(), "endpoint unhalthalt failed")


def get_set_interface_test(dev, log):
    # Control IN GET_INTERFACE
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_INTERFACE)
    request = REQUEST_GET_INTERFACE
    value = 0                                   # Always 0 for this request
    index = 0                                   # Interface index
    length = 1                                  # Always 1 for this request (size of return data)
    try:
        ret = dev.ctrl_transfer(request_type, request, value, index, length)
        print("GET_INTERFACE ret: %d" % (ret[0]))
        if(ret[0] != 0):
            raise_unconditionally(lineno(), "Wrong interface was set expected: 0")
    except usb.core.USBError:
        raise_unconditionally(lineno(), "GET_INTERFACE failed")
    # test control data transfer
    control_data_test(dev, [64, 256], log)

    # Control IN SET_INTERFACE
    request_type = build_request_type(CTRL_OUT, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_INTERFACE)
    request = REQUEST_SET_INTERFACE
    value = 1                                   # Alternative interface setting index
    index = 0                                   # Interface index
    length = 0                                  # Always 0 for this request
    try:
        dev.ctrl_transfer(request_type, request, value, index, length)
    except usb.core.USBError:
        raise_unconditionally(lineno(), "SET_INTERFACE failed")
    # test control data transfer after alternative interface set
    control_data_test(dev, [64, 256], log)

    # Control IN GET_INTERFACE - check if alternative interface setting was set
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_INTERFACE)
    request = REQUEST_GET_INTERFACE
    value = 0                                   # Always 0 for this request
    index = 0                                   # Interface index
    length = 1                                  # Always 1 for this request (size of return data)
    try:
        ret = dev.ctrl_transfer(request_type, request, value, index, length)
        if(ret[0] != 1):
            raise_unconditionally(lineno(), "Alternative interface setting was not set properly")
    except usb.core.USBError:
        raise_unconditionally(lineno(), "GET_INTERFACE failed")

    # Control IN SET_INTERFACE restore interfejs settings
    request_type = build_request_type(CTRL_OUT, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_INTERFACE)
    request = REQUEST_SET_INTERFACE
    value = 0                                   # Interface setting index
    index = 0                                   # Interface index
    length = 0                                  # Always 0 for this request
    try:
        dev.ctrl_transfer(request_type, request, value, index, length)
    except usb.core.USBError:
        raise_unconditionally(lineno(), "SET_INTERFACE request failed")
    # test control data transfer after interface restoring
    control_data_test(dev, [64, 256], log)

    # Control IN GET_INTERFACE - check if alternative interface setting was restored properly
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_INTERFACE)
    request = REQUEST_GET_INTERFACE
    value = 0                                   # Always 0 for this request
    index = 0                                   # Interface index
    length = 1                                  # Always 1 for this request (size of return data)
    try:
        ret = dev.ctrl_transfer(request_type, request, value, index, length)
        if(ret[0] != 0):
            raise_unconditionally(lineno(), "Alternative interface setting was not restored properly")
    except usb.core.USBError:
        raise_unconditionally(lineno(), "GET_INTERFACE failed")


def get_set_configuration_test(dev, log):
    # Set Configuration can also be used, with wValue set to 0, to deconfigure the device
    # Control IN GET_CONFIGURATION
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_INTERFACE)
    request = REQUEST_GET_CONFIGURATION
    value = 0                                   # Always 0 for this request
    index = 0                                   # Always 0 for this request
    length = 1                                  # Always 1 for this request (size of return data)
    try:
        ret = dev.ctrl_transfer(request_type, request, value, index, length)
        if(ret[0] != 1):
            raise_unconditionally(lineno(), "Expected first configuration set")
    except usb.core.USBError:
        raise_unconditionally(lineno(), "GET_CONFIGURATION failed")
    # test control data transfer
    control_data_test(dev, [64, 256], log)

    # Control OUT SET_CONFIGURATION 0 - deconfigure the device
    request_type = build_request_type(CTRL_OUT, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_INTERFACE)
    request = REQUEST_SET_CONFIGURATION
    value = 0                                   # Configuration Value (0 - deconfigure the device)
    index = 0                                   # Always 0 for this request
    length = 0                                  # Always 0 for this request
    try:
        ret = dev.ctrl_transfer(request_type, request, value, index, length)
    except usb.core.USBError:
        raise_unconditionally(lineno(), "SET_CONFIGURATION failed")

    # Control IN GET_CONFIGURATION - check if deconfigured
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_INTERFACE)
    request = REQUEST_GET_CONFIGURATION
    value = 0                                   # Always 0 for this request
    index = 0                                   # Always 0 for this request
    length = 1                                  # Always 1 for this request (size of return data)
    try:
        ret = dev.ctrl_transfer(request_type, request, value, index, length)
        if(ret[0] != 0):
            raise_unconditionally(lineno(), "Expected to be deconfigured")
    except usb.core.USBError:
        raise_unconditionally(lineno(), "GET_CONFIGURATION failed")

    # Control OUT SET_CONFIGURATION 1 - restore first configuration
    request_type = build_request_type(CTRL_OUT, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_INTERFACE)
    request = REQUEST_SET_CONFIGURATION
    value = 1                                   # Configuration Value
    index = 0                                   # Always 0 for this request
    length = 0                                  # Always 0 for this request
    try:
        ret = dev.ctrl_transfer(request_type, request, value, index, length)
    except usb.core.USBError:
        raise_unconditionally(lineno(), "SET_CONFIGURATION failed")
    # test control data transfer after configured back
    control_data_test(dev, [64, 256], log)

    # Control IN GET_CONFIGURATION - check if configured back
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_INTERFACE)
    request = REQUEST_GET_CONFIGURATION
    value = 0                                   # Always 0 for this request
    index = 0                                   # Always 0 for this request
    length = 1                                  # Always 1 for this request (size of return data)
    try:
        ret = dev.ctrl_transfer(request_type, request, value, index, length)
        if(ret[0] != 1):
            raise_unconditionally(lineno(), "Expected to be deconfigured: 1")
    except usb.core.USBError:
        raise_unconditionally(lineno(), "GET_CONFIGURATION failed")
    control_data_test(dev, [64, 256], log)


def get_descriptor_test(dev, vendor_id, product_id, log):
    # Control IN GET_DESCRIPTOR - device
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_DEVICE)
    request = REQUEST_GET_DESCRIPTOR
    value = (DESC_TYPE_DEVICE << 8) | (0 << 0)  # Descriptor Type (H) and Descriptor Index (L)
    index = 0                                   # 0 or Language ID for this request
    length = DEVICE_DESC_SIZE                   # Descriptor Length
    try:
        ret = dev.ctrl_transfer(request_type, request, value, index, length)
        #print("### DEVICE_DESC ####################################################")
        #dev_desc = dict(zip(device_descriptor_keys, device_descriptor_parser.unpack(ret)))
        #for key in dev_desc:
        #    print("%s: %d" % (key, dev_desc[key]))
        #assert vendor_id != dev_desc['idVendor']
        #assert product_id != dev_desc['idProduct']
    except usb.core.USBError:
        raise_unconditionally(lineno(), "Requesting device descriptor failed")

    # Control IN GET_DESCRIPTOR - configuration
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_DEVICE)
    request = REQUEST_GET_DESCRIPTOR
    value = (DESC_TYPE_CONFIG << 8) | (0 << 0)  # Descriptor Type (H) and Descriptor Index (L)
    index = 0                                   # 0 or Language ID for this request
    length = CONFIGURATION_DESC_SIZE            # Descriptor Length
    try:
        ret = dev.ctrl_transfer(request_type, request, value, index, length)
        #print("### CONFIGURATION_DESC ####################################################")
        #conf_desc = dict(zip(configuration_descriptor_keys, configuration_descriptor_parser.unpack(ret)))
        #for key in conf_desc:
        #    print("%s: %d" % (key, conf_desc[key]))
        #print("#######################################################")
    except usb.core.USBError:
        raise_unconditionally(lineno(), "Requesting configuration descriptor failed")

    # Control IN GET_DESCRIPTOR - interface
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_DEVICE)
    request = REQUEST_GET_DESCRIPTOR
    value = (DESC_TYPE_INTERFACE << 8) | (0 << 0)   # Descriptor Type (H) and Descriptor Index (L)
    index = 0                                       # 0 or Language ID for this request
    length = INTERFACE_DESC_SIZE                    # Descriptor Length
    try:
        ret = dev.ctrl_transfer(request_type, request, value, index, length)
        raise_unconditionally(lineno(), "Requesting interface descriptor should fail since it is not directly accessible")
    except usb.core.USBError:
        log("interface descriptor is not directly accessible")

    # Control IN GET_DESCRIPTOR - interface
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_DEVICE)
    request = REQUEST_GET_DESCRIPTOR
    value = (DESC_TYPE_ENDPOINT << 8) | (0 << 0)   # Descriptor Type (H) and Descriptor Index (L)
    index = 0                                       # 0 or Language ID for this request
    length = INTERFACE_DESC_SIZE                    # Descriptor Length
    try:
        ret = dev.ctrl_transfer(request_type, request, value, index, length)
        raise_unconditionally(lineno(), "Requesting endpoint descriptor should fail since it is not directly accessible")
    except usb.core.USBError:
        log("endpoint descriptor is not directly accessible")


def set_descriptor_test(dev, log):
    # SET_DESCRIPTOR is optional and not implemented in Mbed
    # command should fail with no action on device side

    # Control OUT SET_DESCRIPTOR
    request_type = build_request_type(CTRL_OUT, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_DEVICE)
    request = REQUEST_SET_DESCRIPTOR
    value = (DESC_TYPE_DEVICE << 8) | (0 << 0)  # Descriptor Type (H) and Descriptor Index (L)
    index = 0                                   # 0 or Language ID for this request
    data = bytearray(DEVICE_DESC_SIZE)          # Descriptor data
    try:
        dev.ctrl_transfer(request_type, request, value, index, data)
        raise_unconditionally(lineno(), "SET_DESCRIPTOR should fail since it is not implemented")
    except usb.core.USBError:
        log("SET_DESCRIPTOR is unsupported")


def synch_frame_test(dev, log):
    # only for isochronous endpoints
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_ENDPOINT)
    request = REQUEST_SYNCH_FRAME
    value = 0                                   # Always 0 for this request
    index = 1                                   # Endpoint index
    length = 2                                  # Always 2 for this request (size of return data)
    try:
        ret = dev.ctrl_transfer(request_type, request, value, index, length)
        ret = ret[0] | (ret[1] << 8)
        log("synch frame ret: %d" % (ret))
    except usb.core.USBError:
        raise_unconditionally(lineno(), "SYNCH_FRAME failed")


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
        raise_unconditionally(lineno(), "Invalid request not stalled")
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
        raise_unconditionally(lineno(), "Invalid request not stalled")
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
        raise_unconditionally(lineno(), "Invalid request not stalled")
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
        raise_unconditionally(lineno(), "Invalid request not stalled")
    except usb.core.USBError:
        log("Invalid request stalled")

    for i in (3, 4, 5):
        try:
            request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                              CTRL_RECIPIENT_DEVICE)
            request = 0x6                   # GET_DESCRIPTOR
            value = (0x03 << 8) | (i << 0)  # String descriptor index
            index = 0                       # Communication interface
            length = 255
            resp = dev.ctrl_transfer(request_type, request, value, index, length, 5000)
        except usb.core.USBError:
            raise_unconditionally(lineno(), "Requesting string failed i: " + str(i))

    for i in (6, 7):
        try:
            request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                              CTRL_RECIPIENT_DEVICE)
            request = 0x6                   # GET_DESCRIPTOR
            value = (0x03 << 8) | (i << 0)  # String descriptor index
            index = 0                       # Communication interface
            length = 255
            resp = dev.ctrl_transfer(request_type, request, value, index, length, 5000)
            raise_unconditionally(lineno(), "Requesting string passed i: " + str(i))
        except usb.core.USBError:
            log("Requesting string %s failed" % i)


def control_sizes_test(dev, log):
    list = [1, 2, 3, 7, 8, 9, 15, 16, 17, 31, 32, 33, 63, 64, 65, 127, 128, 129, 255, 256, 257, 511, 512, 513, 1023, 1024, 1025, 2047, 2048]
    control_data_test(dev, list, log)


def control_data_test(dev, sizes_list, log):
    # Test control requests of various data stage sizes (1,8,16,32,64,255,256,...)
    count = 1
    for i in sizes_list:
        request_type = build_request_type(CTRL_OUT, CTRL_TYPE_VENDOR,
                                              CTRL_RECIPIENT_DEVICE)
        request = VENDOR_TEST_CTRL_OUT_SIZES
        value = i                           # Size of data the device should actually read
        index = 0                           # Unused - set for debugging only
        data = bytearray(i)                 # Dummy data
        if i == 1:
            data[0] = count
        else:
            data[0] = count - 1
            data[i - 1] = count + 1
        try:
            dev.ctrl_transfer(request_type, request, value, index, data, 5000)
        except usb.core.USBError:
            raise_unconditionally(lineno(), "VENDOR_TEST_CTRL_OUT_SIZES failed ")

        request_type = build_request_type(CTRL_IN, CTRL_TYPE_VENDOR,
                                              CTRL_RECIPIENT_DEVICE)
        request = VENDOR_TEST_CTRL_IN_SIZES
        value = 0                           # Size of data the device should actually send
        index = 0                           # Unused - set for debugging only
        length = i
        try:
            ret = dev.ctrl_transfer(request_type, request, value, index, length, 5000)
            if i == 1:
                raise_if_different(count, ret[0], lineno(), "send/receive data not match")
            else:
                raise_if_different(count - 1, ret[0], lineno(), "send/receive data not match")
                raise_if_different(count + 1, ret[i - 1], lineno(), "send/receive data not match")
        except usb.core.USBError:
            raise_unconditionally(lineno(), "VENDOR_TEST_CTRL_IN_SIZES failed")
        count += 1


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


def device_reset_test(log):
    dev = yield
    dev.reset();
    dev = yield
    dev.reset();
    dev = yield
    dev.reset();
    dev = yield
    # run other test to check if USB works fine after reset
    control_data_test(dev, [64, 256], log)
    yield


def device_soft_reconnection_test(log):
    list = [64, 256]
    dev = yield
    # run other test to check if USB works fine before reconnection
    control_data_test(dev, list, log)
    dev = yield
    # run other test to check if USB works fine after reconnection
    control_data_test(dev, list, log)
    dev = yield
    # run other test to check if USB works fine after reconnection
    control_data_test(dev, list, log)
    dev = yield
    # run other test to check if USB works fine after reconnection
    control_data_test(dev, list, log)
    dev = yield
    # run other test to check if USB works fine after reconnection
    control_data_test(dev, list, log)
    yield


def device_suspend_resume_test(log):
    dev = yield
    time.sleep(0.1)
    control_data_test(dev, [64, 256], log)
    time.sleep(0.1)
    control_data_test(dev, [64, 256], log)
    time.sleep(0.1)
    control_data_test(dev, [64, 256], log)
    time.sleep(0.1)
    control_data_test(dev, [64, 256], log)
    time.sleep(0.1)
    yield


def repeated_construction_destruction_test(log):
    # run other test to check if USB works fine after repeated construction/destruction
    list = [64, 256]
    dev = yield
    control_data_test(dev, list, log)
    dev = yield
    control_data_test(dev, list, log)
    dev = yield
    control_data_test(dev, list, log)
    yield


"""
For documentation purpose until test writing finished
TODO: remove this if not needed anymore


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


def write_data(pipe):
    print("Write data running")
    count = 0
    for _ in range(40):
        pipe.write("Value is %s" % count)
        count += 1
        print("Count %s" % count)
        time.sleep(0.5)


def main():
    parser = ArgumentParser(description="USB basic test")
    parser.add_argument('serial', help='USB serial number of DUT')
    args = parser.parse_args()
    ret = test_device(args.serial)
    print("Test %s" % "passed" if ret else "failed")

if __name__ == "__main__":
    main()
"""
