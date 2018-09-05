"""
mbed SDK
Copyright (c) 2018 ARM Limited

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
import functools
import itertools
import time
import threading
import serial
import serial.tools.list_ports as stlp
import mbed_host_tests


MSG_KEY_PORT_OPEN_WAIT = 'port_open_wait'
MSG_KEY_PORT_OPEN_CLOSE = 'port_open_close'
MSG_KEY_SEND_BYTES_SINGLE = 'send_single'
MSG_KEY_SEND_BYTES_MULTIPLE = 'send_multiple'
MSG_KEY_LOOPBACK = 'loopback'
MSG_KEY_CHANGE_LINE_CODING = 'change_lc'

RX_BUFF_SIZE = 32
TERM_REOPEN_DELAY = 0.1
LINE_CODING_STRLEN = 13


def usb_serial_name(serial_number):
    """Get USB serial device name based on the device serial number."""
    for port_info in stlp.comports():
        if port_info.serial_number == serial_number:
            return port_info.device
    return None


class RetryError(Exception):
    """Exception raised by retry_fun_call()."""


def retry_fun_call(fun, num_retries=3, retry_delay=0.0):
    """Call fun and retry if any exception was raised.

    fun is called at most num_retries with a retry_dalay in between calls.
    Raises RetryError if the retry limit is exhausted.
    """
    verbose = False
    final_err = None
    for retry in range(1, num_retries + 1):
        try:
            return fun()  # pylint: disable=not-callable
        except Exception as exc:  # pylint: disable=broad-except
            final_err = exc
            if verbose:
                print('Retry {}/{} failed ({})'
                      .format(retry, num_retries, str(fun)))
            time.sleep(retry_delay)
    err_msg = 'Failed with "{}". Tried {} times.'
    raise RetryError(err_msg.format(final_err, num_retries))


class USBSerialTest(mbed_host_tests.BaseHostTest):
    """Host side test for USB CDC & Serial classes."""

    _BYTESIZES = {
        5: serial.FIVEBITS,
        6: serial.SIXBITS,
        7: serial.SEVENBITS,
        8: serial.EIGHTBITS}
    _PARITIES = {
        0: serial.PARITY_NONE,
        1: serial.PARITY_ODD,
        2: serial.PARITY_EVEN,
        3: serial.PARITY_MARK,
        4: serial.PARITY_SPACE}
    _STOPBITS = {
        0: serial.STOPBITS_ONE,
        1: serial.STOPBITS_ONE_POINT_FIVE,
        2: serial.STOPBITS_TWO}

    @staticmethod
    def get_usb_serial_name(usb_id_str):
        """Get USB serial device name as registered in the system.

        Search is based on the id received from the device itself.
        Raises RuntimeError if the device is not found.
        """
        port_name = usb_serial_name(usb_id_str)
        if port_name is None:
            err_msg = 'USB serial device (SN={}) not found.'
            raise RuntimeError(err_msg.format(usb_id_str))
        return port_name

    def __init__(self):
        super(USBSerialTest, self).__init__()
        self.__bg_task = None

    def port_open_wait(self, usb_id_str):
        """Open the serial and wait until it's closed by the device."""
        mbed_serial = serial.Serial()
        try:
            mbed_serial.port = retry_fun_call(
                fun=functools.partial(self.get_usb_serial_name, usb_id_str),  # pylint: disable=not-callable
                num_retries=20,
                retry_delay=0.05)
            retry_fun_call(
                fun=mbed_serial.open,
                num_retries=10,
                retry_delay=0.05)
            try:
                mbed_serial.read()  # wait until closed
            except (serial.portNotOpenError, serial.SerialException):
                pass
        except RetryError as exc:
            self.log('TEST ERROR: {}'.format(exc))
            self.notify_complete(False)
            return

    def port_open_close(self, usb_id_str):
        """Open the serial and close it with a delay."""
        mbed_serial = serial.Serial(timeout=0.5, write_timeout=0.1)
        try:
            mbed_serial.port = retry_fun_call(
                fun=functools.partial(self.get_usb_serial_name, usb_id_str),  # pylint: disable=not-callable
                num_retries=20,
                retry_delay=0.05)
            retry_fun_call(
                fun=mbed_serial.open,
                num_retries=10,
                retry_delay=0.05)
            mbed_serial.reset_output_buffer()
            time.sleep(TERM_REOPEN_DELAY)
            mbed_serial.close()
        except RetryError as exc:
            self.log('TEST ERROR: {}'.format(exc))
            self.notify_complete(False)
            return

    def send_data_sequence(self, usb_id_str, chunk_size=1):
        """Open the serial and send a sequence of values.

        chunk_size defines the size of data sent in each write operation.
        The input buffer content is discarded.
        """
        mbed_serial = serial.Serial(write_timeout=0.1)
        try:
            mbed_serial.port = retry_fun_call(
                fun=functools.partial(self.get_usb_serial_name, usb_id_str),  # pylint: disable=not-callable
                num_retries=20,
                retry_delay=0.05)
            retry_fun_call(
                fun=mbed_serial.open,
                num_retries=10,
                retry_delay=0.05)
        except RetryError as exc:
            self.log('TEST ERROR: {}'.format(exc))
            self.notify_complete(False)
            return
        mbed_serial.reset_output_buffer()
        for byteval in itertools.chain(reversed(range(0x100)), range(0x100)):
            try:
                payload = bytearray(chunk_size * (byteval,))
                mbed_serial.write(payload)
#                 self.log('SENT: {!r}'.format(payload))
                # Discard input buffer content. The data received from the
                # device during the concurrent rx/tx test is irrelevant.
                mbed_serial.reset_input_buffer()
            except serial.SerialException as exc:
                self.log('TEST ERROR: {}'.format(exc))
                self.notify_complete(False)
                return
        while mbed_serial.out_waiting > 0:
            time.sleep(0.001)
        mbed_serial.close()

    def loopback(self, usb_id_str):
        """Open the serial and send back every byte received."""
        mbed_serial = serial.Serial(timeout=0.5, write_timeout=0.1)
        try:
            mbed_serial.port = retry_fun_call(
                fun=functools.partial(self.get_usb_serial_name, usb_id_str),  # pylint: disable=not-callable
                num_retries=20,
                retry_delay=0.05)
            retry_fun_call(
                fun=mbed_serial.open,
                num_retries=10,
                retry_delay=0.05)
        except RetryError as exc:
            self.log('TEST ERROR: {}'.format(exc))
            self.notify_complete(False)
            return
        mbed_serial.reset_output_buffer()
        try:
            payload = mbed_serial.read(1)
            while len(payload) == 1:
                mbed_serial.write(payload)
#                 self.log('SENT: {!r}'.format(payload))
                payload = mbed_serial.read(1)
        except serial.SerialException as exc:
            self.log('TEST ERROR: {}'.format(exc))
            self.notify_complete(False)
            return
        while mbed_serial.out_waiting > 0:
            time.sleep(0.001)
        mbed_serial.close()

    def change_line_coding(self, usb_id_str):
        """Open the serial and change serial params according to device request.

        New line coding params are read from the device serial data.
        """
        mbed_serial = serial.Serial(timeout=0.5)
        try:
            mbed_serial.port = retry_fun_call(
                fun=functools.partial(self.get_usb_serial_name, usb_id_str),  # pylint: disable=not-callable
                num_retries=20,
                retry_delay=0.05)
            retry_fun_call(
                fun=mbed_serial.open,
                num_retries=10,
                retry_delay=0.05)
        except RetryError as exc:
            self.log('TEST ERROR: {}'.format(exc))
            self.notify_complete(False)
            return
        mbed_serial.reset_output_buffer()
        try:
            payload = mbed_serial.read(LINE_CODING_STRLEN)
            while len(payload) == LINE_CODING_STRLEN:
                baud, bits, parity, stop = (int(i) for i in payload.split(','))
                new_line_coding = {
                    'baudrate': baud,
                    'bytesize': self._BYTESIZES[bits],
                    'parity': self._PARITIES[parity],
                    'stopbits': self._STOPBITS[stop]}
                mbed_serial.apply_settings(new_line_coding)
                payload = mbed_serial.read(LINE_CODING_STRLEN)
        except serial.SerialException as exc:
            self.log('TEST ERROR: {}'.format(exc))
            self.notify_complete(False)
            return
        mbed_serial.close()

    def setup(self):
        self.register_callback(MSG_KEY_PORT_OPEN_WAIT, self.cb_port_open_wait)
        self.register_callback(MSG_KEY_PORT_OPEN_CLOSE, self.cb_port_open_close)
        self.register_callback(MSG_KEY_SEND_BYTES_SINGLE, self.cb_send_bytes_single)
        self.register_callback(MSG_KEY_SEND_BYTES_MULTIPLE, self.cb_send_bytes_multiple)
        self.register_callback(MSG_KEY_LOOPBACK, self.cb_loopback)
        self.register_callback(MSG_KEY_CHANGE_LINE_CODING, self.cb_change_line_coding)

    def start_bg_task(self, **thread_kwargs):
        """Start a new daemon thread.

        The callbacks delegate serial handling to a background task to
        prevent any delays in the device side assert handling. Only one
        background task is kept running to prevent multiple access
        to serial.
        """
        try:
            self.__bg_task.join()
        except (AttributeError, RuntimeError):
            pass
        self.__bg_task = threading.Thread(**thread_kwargs)
        self.__bg_task.daemon = True
        self.__bg_task.start()

    def cb_port_open_wait(self, key, value, timestamp):
        """Open the serial and wait until it's closed by the device."""
        self.start_bg_task(
            target=self.port_open_wait,
            args=(value, ))

    def cb_port_open_close(self, key, value, timestamp):
        """Open the serial and close it with a delay."""
        self.start_bg_task(
            target=self.port_open_close,
            args=(value, ))

    def cb_send_bytes_single(self, key, value, timestamp):
        """Open the serial and send a sequence of values."""
        self.start_bg_task(
            target=self.send_data_sequence,
            args=(value, 1))

    def cb_send_bytes_multiple(self, key, value, timestamp):
        """Open the serial and send a sequence of one byte values."""
        self.start_bg_task(
            target=self.send_data_sequence,
            args=(value, RX_BUFF_SIZE))

    def cb_loopback(self, key, value, timestamp):
        """Open the serial and send a sequence of multibyte values."""
        self.start_bg_task(
            target=self.loopback,
            args=(value, ))

    def cb_change_line_coding(self, key, value, timestamp):
        """Open the serial and change the line coding."""
        self.start_bg_task(
            target=self.change_line_coding,
            args=(value, ))
