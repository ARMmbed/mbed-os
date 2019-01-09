"""
Copyright (c) 2017, Arm Limited and affiliates.
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

# ice-tea cli commands decorator class


from nfc_messages import NfcErrors
import logging
import icetea_lib.tools.asserts as asserts

# Values > 4 k incur large time costs
STRESS_BUFFLEN = 2050 # constant value for large buffers, this needs to be less than the target supported size.

class CliHelper():
    """
    Helper methods, checks the nfc SDK error-code for you, makes writing a negative test much easier
    Example:
        if (target_is_eeprom):
            nfc_command("dev1", "start", expected_retcode=-2, expected_nfc_error= NfcErrors.nfc_err_unsupported)
        else:
            nfc_command("dev1", "start")
    """

    def nfc_command(self, k, cmd,  # pylint: disable=invalid-name
                    wait=True,
                    timeout=10,
                    expected_retcode=0,
                    asynchronous=False,
                    report_cmd_fail=True,
                    expected_nfc_error=NfcErrors.nfc_ok):
        """
        By default will assert if the NFC result code is non-zero.
        """
        response = self.command(k, cmd, wait, timeout, expected_retcode, asynchronous, report_cmd_fail)
        asserts.assertEqual(int(response.parsed['lastnfcerror']), expected_nfc_error.value)
        return response

    @staticmethod
    def command_is(string, command):
        return string.split(' ')[0] == command

    @staticmethod
    def debug_nfc_data(key, value):
        """
        print useful data values for the host/user with a >> preamble to make it easy to spot
        """
        text = ">> %s=%s" % (key, value)
        logging.Logger.info(text)

    def assert_binary_equal(self, left, right, left_length, right_length):
        asserts.assertEqual(left_length, right_length, "Buffers are not same length")
        i = 0
        while i < left_length:
            asserts.assertEqual(left[i], ord(right[i]), ("Missmatch @offset %d 0x%x <> 0x%x" % (i, left[i], ord(right[i]))) )
            i = i + 1

    def assert_text_equal(self, left, right, left_length, right_length):
        """
        Asserts if the 2 buffers (Text) differ
        """
        asserts.assertEqual(left_length, right_length, "Buffers are not same length")
        i = 0
        while i < left_length:
            asserts.assertEqual(ord(left[i]), ord(right[i]), ("Missmatch @offset %d %d <> %d" % (i, ord(left[i]), ord(right[i]))) )
            i = i + 1

