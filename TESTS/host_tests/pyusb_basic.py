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
            control_basic_test(dev, int(vendor_id), int(product_id), log=print)
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
    get_set_configuration_test(dev, log)
    get_set_interface_test(dev, log)
    get_status_test(dev, log)
    set_clear_feature_test(dev, log)
    get_descriptor_test(dev, vendor_id, product_id, log)
    set_descriptor_test(dev, log)


def get_set_configuration_test(dev, log):
    """
    Test device configuration/deconfiguration

    Given an initialized USB (HOST <---> DUT connection established)
    When device configuration is checked just after initialization
    Then get_configuration returns 1 (default configuration is set)
    When device is deconfigured
    Then get_configuration returns 0 (no configuration is set)
    When each from supported configurations is set
    Then the configuration is set correctly
    """

    print("<<< get_set_configuration_test >>>")
    # check if dafault(1) configuration set
    try:
        ret = usb.control.get_configuration(dev)
        raise_if_different(1, ret, lineno(), "FAILED - expected first configuration set")
    except usb.core.USBError as error:
        print(error)
        raise_unconditionally(lineno(), "FAILED - get_configuration !!!")

    cfg = dev.get_active_configuration()
    for intf in cfg:
        usb.util.release_interface(dev, intf)

    # deconfigure the device
    try:
        ret = dev.set_configuration(0)
    except usb.core.USBError as error:
        print(error)
        raise_unconditionally(lineno(), "FAILED - set_configuration(0) !!!")

    # check if deconfigured
    try:
        ret = usb.control.get_configuration(dev)
        raise_if_different(0, ret, lineno(), "FAILED - expected to be deconfigured")
        print("device deconfigured - OK")
    except usb.core.USBError as error:
        print(error)
        raise_unconditionally(lineno(), "FAILED - get_active_configuration !!!")

    # for every configuration
    for cfg in dev:
        try:
            # set configuration
            ret = cfg.set()
        except usb.core.USBError as error:
            print(error)
            raise_unconditionally(lineno(), "FAILED - set configuration")

        # check if configured
        try:
            ret = usb.control.get_configuration(dev)
            raise_if_different(cfg.bConfigurationValue, ret, lineno(), "FAILED - expected {} configuration set".format(cfg.bConfigurationValue))
            print("configuration {} set - OK ".format(cfg.bConfigurationValue))
        except usb.core.USBError as error:
            print(error)
            raise_unconditionally(lineno(), "FAILED - get_configuration !!!")
        # test control data transfer after configuration set
        control_data_test(dev, [64, 256], log)
    print("") # new line


def get_set_interface_test(dev, log):
    """
    Test device interface setting

    Given an initialized USB (HOST <---> DUT connection established)
    When each altsetting from every supported configuration is set
    Then the interface altsetting is set correctly
    """

    print("<<< get_set_interface_test >>>")
    # for every configuration
    for cfg in dev:
        cfg.set()
        # for every interface
        for intf in cfg:
            intf.set_altsetting();
            altsett = usb.control.get_interface(dev, intf.bInterfaceNumber)
            raise_if_different(intf.bAlternateSetting, altsett, lineno(), text='Wrong alternate setting for interface {}'.format(intf.bInterfaceNumber))
            print("cfg({}) inteface {}.{} set - OK".format(cfg.bConfigurationValue, intf.bInterfaceNumber, intf.bAlternateSetting))
            control_data_test(dev, [64, 256], log)

        release_interfaces(dev)

    restore_default_configuration(dev)

    # test control data transfer after default interface restoring
    control_data_test(dev, [64, 256], log)
    print("") # new line


def get_status_test(dev, log):
    """
    Test device/interface/endpoint status

    Given an initialized USB (HOST <---> DUT connection established)
    When device status is checked
    Then status is within allowed values (see status bits description below)
    When control endpoint status is checked
    Then control endpoint status is 0
    When status of each interface from every supported configuration is checked
    Then interface status is 0
    When status of each endpoint in every allowed device interface/configuration combination is checked
    Then endpoint status is 0 (not halted)
    """

    print("<<< get_status_test >>>")
    # check device status
    ret = get_status(dev, CTRL_RECIPIENT_DEVICE)
    # Status bits
    # ret == 0b01    (D0)Self Powered
    # ret == 0b10    (D1)Remote Wakeup
    # (D2 - D15 reserved) Must be set to 0
    if(ret < 0 or ret > 3):
        raise_unconditionally(lineno(), "GET_STATUS on DEVICE failed")

    # check endpoint 0 status
    ret = get_status(dev, CTRL_RECIPIENT_ENDPOINT, 0)
    # Status bits
    # ret == 0b1    (D0)endpoint Halt
    # (D1 - D15 reserved) Must be set to 0
    # endpoint 0 can't be halted ret == 0
    raise_if_different(0, ret, lineno(), "GET_STATUS on ENDPOINT 0 should return 0")

    # for every configuration
    for cfg in dev:
        cfg.set()
        raise_if_different(cfg.bConfigurationValue, usb.control.get_configuration(dev), lineno(), "Configuration {} set failed".format(cfg.bConfigurationValue))

        for intf in cfg:
            intf.set_altsetting()
            # check interface status
            ret = get_status(dev, CTRL_RECIPIENT_INTERFACE, intf.bInterfaceNumber)
            # Status bits
            # ret == 0b0
            # (D0 - D15 reserved) Must be set to 0
            if(ret != 0):
                raise_unconditionally(lineno(), "GET_STATUS on INTERFACE ({},{}) failed".format(intf.bInterfaceNumber, intf.bAlternateSetting))
            print("cfg({}) interface {}.{} status - OK".format(cfg.bConfigurationValue, intf.bInterfaceNumber, intf.bAlternateSetting))

            # on every ENDPOINT in this altsetting
            for ep in intf:
                ret = usb.control.get_status(dev, ep)
                # Status bits
                # ret == 0b1    (D0)endpoint Halt
                # (D1 - D15 reserved) Must be set to 0
                if(ret >= 1):
                    raise_unconditionally(lineno(), "GET_STATUS on ENDPOINT {} failed - endpoint halted".format(ep.bEndpointAddress))
                print("cfg({}) intf({}.{}) endpoint {} status - OK".format(cfg.bConfigurationValue, intf.bInterfaceNumber, intf.bAlternateSetting, ep.bEndpointAddress))

        release_interfaces(dev)
    restore_default_configuration(dev)
    print("") # new line


def set_clear_feature_test(dev, log):
    """
    Test set/clear feature on device/interface/endpoint

    Given an initialized USB (HOST <---> DUT connection established)
    When for each endpoint in every allowed interface/configuration combination the feature is set and then cleared
    Then selected feature is set/cleared accordingly
    """

    print("<<< set_clear_feature_test >>>")
    # TODO:
    # test set_feature on device (Remote wakeup feature not supported on DUT side)
    # test set_feature on interface (not supported at all)

    # for every configuration
    for cfg in dev:
        cfg.set()
        raise_if_different(cfg.bConfigurationValue, usb.control.get_configuration(dev), lineno(), "Configuration {} set failed".format(cfg.bConfigurationValue))

        for intf in cfg:
            intf.set_altsetting()
            # on every ENDPOINT
            for ep in intf:
                # halt endpoint
                try:
                    usb.control.set_feature(dev, FEATURE_ENDPOINT_HALT, ep)
                except usb.core.USBError as err:
                    print(err)
                    raise_unconditionally(lineno(), "endpoint {} halt failed".format(ep.bEndpointAddress))

                # check if endpoint was halted
                try:
                    ret = usb.control.get_status(dev, ep)
                except usb.core.USBError as err:
                    print(err)
                    raise_unconditionally(lineno(), "endpoint status failed".format(ep.bEndpointAddress))
                if(ret != 1):
                    raise_unconditionally(lineno(), "endpoint {} was not halted".format(ep.bEndpointAddress))
                print("cfg({}) intf({}.{})   ep {} halted - OK".format(cfg.bConfigurationValue, intf.bInterfaceNumber, intf.bAlternateSetting, ep.bEndpointAddress))

                # Control OUT CLEAR_FEATURE on endpoint - unhalt
                try:
                    usb.control.clear_feature(dev, FEATURE_ENDPOINT_HALT, ep)
                except usb.core.USBError as err:
                    print(err)
                    raise_unconditionally(lineno(), "endpoint {} unhalt failed".format(ep.bEndpointAddress))

                # check if endpoint was unhalted
                ret = usb.control.get_status(dev, ep)
                if(ret != 0):
                    raise_unconditionally(lineno(), "endpoint {} was not unhalted".format(ep.bEndpointAddress))
                print("cfg({}) intf({}.{})   ep {} unhalted - OK".format(cfg.bConfigurationValue, intf.bInterfaceNumber, intf.bAlternateSetting, ep.bEndpointAddress))

        release_interfaces(dev)

    restore_default_configuration(dev)
    print("") # new line


def get_descriptor_test(dev, vendor_id, product_id, log):
    """
    Test device/configuration/interface/endpoint descriptors

    Given an initialized USB (HOST <---> DUT connection established)
    When device descriptor is read
    Then the descriptor content is valid
    When configuration descriptor is read
    Then the descriptor content is valid
    When interface descriptor is read
    Then the error is thrown since it is not directly accessible
    When endpoint descriptor is read
    Then the error is thrown since it is not directly accessible
    """

    print("<<< get_descriptor_test >>>")
    # device descriptor
    try:
        ret = get_descriptor(dev, (DESC_TYPE_DEVICE << 8) | (0 << 0), 0, DEVICE_DESC_SIZE)
        dev_desc = dict(zip(device_descriptor_keys, device_descriptor_parser.unpack(ret)))
        raise_if_different(DEVICE_DESC_SIZE, dev_desc['bLength'], lineno(), text='Wrong device descriptor size !!!')
        raise_if_different(vendor_id, dev_desc['idVendor'], lineno(), text='Wrong vendor id !!!')
        raise_if_different(product_id, dev_desc['idProduct'], lineno(), text='Wrong product id !!!')
    except usb.core.USBError:
        raise_unconditionally(lineno(), "Requesting device descriptor failed")

    # configuration descriptor
    try:
        ret = get_descriptor(dev, (DESC_TYPE_CONFIG << 8) | (0 << 0), 0, CONFIGURATION_DESC_SIZE)
        conf_desc = dict(zip(configuration_descriptor_keys, configuration_descriptor_parser.unpack(ret)))
        raise_if_different(CONFIGURATION_DESC_SIZE, conf_desc['bLength'], lineno(), text='Wrong configuration descriptor size !!!')
    except usb.core.USBError:
        raise_unconditionally(lineno(), "Requesting configuration descriptor failed")

    # interface descriptor
    try:
        ret = get_descriptor(dev, (DESC_TYPE_INTERFACE << 8) | (0 << 0), 0, INTERFACE_DESC_SIZE)
        raise_unconditionally(lineno(), "Requesting interface descriptor should fail since it is not directly accessible")
    except usb.core.USBError:
        log("interface descriptor is not directly accessible - OK")

    # endpoint descriptor
    try:
        ret = get_descriptor(dev, (DESC_TYPE_ENDPOINT << 8) | (0 << 0), 0, ENDPOINT_DESC_SIZE)
        raise_unconditionally(lineno(), "Requesting endpoint descriptor should fail since it is not directly accessible")
    except usb.core.USBError:
        log("endpoint descriptor is not directly accessible - OK")
    print("") # new line


def set_descriptor_test(dev, log):
    """
    Test descriptor setting

    Given an initialized USB (HOST <---> DUT connection established)
    When device descriptor is to be set
    Then error is thrown since descriptor setting command is not supported by Mbed
    """

    print("<<< set_descriptor_test >>>")
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
        log("SET_DESCRIPTOR is unsupported - OK")
    print("") # new line


def synch_frame_test(dev, log):
    """
    Test sync frame request

    Given an initialized USB (HOST <---> DUT connection established)
    When ...
    Then ...
    """

    print("<<< synch_frame_test >>>")
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
    print("") # new line


def control_stall_test(dev, log):
    """
    Test control endpoint stall on invalid request

    Given an initialized USB (HOST <---> DUT connection established)
    When unsupported request to control endpoint is to be sent
    Then the endpoint is stalled and error is thrown
    """

    print("<<< control_stall_test >>>")
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
        log("Invalid request stalled - OK")

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
        log("Invalid request stalled - OK")

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
        log("Invalid request stalled - OK")

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
        log("Invalid request stalled - OK")

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
            log("Requesting string %s failed - OK" % i)
    print("") # new line


def control_sizes_test(dev, log):
    """
    Test various data sizes in control transfer

    Given an initialized USB (HOST <---> DUT connection established)
    When control data in each tested size is sent
    Then read data should match sent data
    """

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
    """
    Test various patterns of control transfers

    Given an initialized USB (HOST <---> DUT connection established)
    When stress control transfer with a data in stage is performed
    Then transfer ends with success
    When stress control transfer with a data out stage followed by a control transfer with a data in stage is performed
    Then transfer ends with success
    When stress control transfer with a data out stage is performed
    Then transfer ends with success
    """

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
    """
    Test USB implementation against repeated reset

    Given an initialized USB (HOST <---> DUT connection established)
    When USB device is reset repeatedly
    Then the USB is operational with no errors
    """

    dev = yield
    dev.reset();
    dev = yield
    # run other test to check if USB works fine after reset
    control_data_test(dev, [64, 256], log)
    dev.reset();
    dev = yield
    # run other test to check if USB works fine after reset
    control_data_test(dev, [64, 256], log)
    dev.reset();
    dev = yield
    # run other test to check if USB works fine after reset
    control_data_test(dev, [64, 256], log)
    yield


def device_soft_reconnection_test(log):
    """
    Test USB implementation against repeated reconnection

    Given an initialized USB (HOST <---> DUT connection established)
    When USB device is disconnected and then connected repeatedly
    Then the USB is operational with no errors
    """

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
    """
    Test USB implementation against repeated suspend and resume

    Given an initialized USB (HOST <---> DUT connection established)
    When USB device is suspended and then resumed repeatedly
    Then the USB is operational with no errors
    """

    dev = yield
    control_data_test(dev, [64, 256], log)
    # suspend code goes here
    # ...
    # resume code here
    # ...
    # run other test to check if USB works fine after resume
    control_data_test(dev, [64, 256], log)
    # suspend code here
    # ...
    # resume code here
    # ...
    # run other test to check if USB works fine after resume
    control_data_test(dev, [64, 256], log)
    # suspend code here
    # ...
    # resume code here
    # ...
    # run other test to check if USB works fine after resume
    control_data_test(dev, [64, 256], log)
    yield


def repeated_construction_destruction_test(log):
    """
    Test USB implementation against repeated initialization and deinitialization

    Given an initialized USB (HOST <---> DUT connection established)
    When USB device is deinitialized and then initialized repeatedly
    Then the USB is operational with no errors
    """

    list = [64, 256]
    dev = yield
    # run other test to check if USB works fine after repeated construction/destruction
    control_data_test(dev, list, log)
    dev = yield
    # run other test to check if USB works fine after repeated construction/destruction
    control_data_test(dev, list, log)
    dev = yield
    # run other test to check if USB works fine after repeated construction/destruction
    control_data_test(dev, list, log)
    yield


def release_interfaces(dev):
    """ Releases interfaces to allow configuration switch

        Fixes error while configuration change(on Windows machines):
        USBError: [Errno None] libusb0-dll:err [set_configuration] can't change configuration, an interface is still in use (claimed)
    """
    cfg = dev.get_active_configuration()
    for i in range(0, cfg.bNumInterfaces):
        usb.util.release_interface(dev, i)


def restore_default_configuration(dev):
    """ Set default configuration """

    cfg = dev[1]
    cfg.set()


def get_status(dev, recipient, index = 0):
    """ Get status of the recipient

    Args:
        dev - pyusb device
        recipient - CTRL_RECIPIENT_DEVICE/CTRL_RECIPIENT_INTERFACE/CTRL_RECIPIENT_ENDPOINT
        index - 0 if recipient is device, interface index if recipient is interface, endpoint index if recipient is endpoint

    Returns:
        status flag 32b int
    """
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      recipient)
    request = REQUEST_GET_STATUS
    value = 0                                   # Always 0 for this request
    index = index                               # recipient index
    length = 2                                  # Always 2 for this request (size of return data)
    ret = dev.ctrl_transfer(request_type, request, value, index, length)
    ret = ret[0] | (ret[1] << 8)

    return ret


def get_descriptor(dev, type_index, lang_id, length):
    # Control IN GET_DESCRIPTOR - device
    request_type = build_request_type(CTRL_IN, CTRL_TYPE_STANDARD,
                                      CTRL_RECIPIENT_DEVICE)
    request = REQUEST_GET_DESCRIPTOR
    value = type_index                          # Descriptor Type (H) and Descriptor Index (L)
    index = lang_id                             # 0 or Language ID for this request
    length = length                             # Descriptor Length
    ret = dev.ctrl_transfer(request_type, request, value, index, length)

    return ret


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
