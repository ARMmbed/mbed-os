#!/usr/bin/env python2
"""
mbed SDK
Copyright (c) 2011-2015 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Przemyslaw Wirkus <Przemyslaw.Wirkus@arm.com>

"""

import sys
import os.path
import mbed_lstools
from prettytable import PrettyTable

try:
    from colorama import init
    from colorama import Fore, Back, Style
except:
    pass

COLORAMA = 'colorama' in sys.modules


class IOperTestCaseBase():
    """ Interoperability test case base class
        @return list of tuple (severity, Description)
                Example:      (result.append((IOperTestSeverity.INFO, ""))
    """

    def __init__(self):
        self.PASS = 'PASS'
        self.INFO = 'INFO'
        self.ERROR = 'ERROR'
        self.WARN = 'WARN'

    def test(self, param=None):
        result = []
        return result

    def RED(self, text):
        return self.color_text(text, color=Fore.RED, delim=Fore.RESET)

    def GREEN(self, text):
        return self.color_text(text, color=Fore.GREEN, delim=Fore.RESET)

    def YELLOW(self, text):
        return self.color_text(text, color=Fore.YELLOW, delim=Fore.RESET)

    def color_text(self, text, color='', delim=''):
        return color + text + color + delim

    def COLOR(self, severity, text):
        colors = {
            self.PASS : self.GREEN,
            self.ERROR : self.RED,
            self.WARN : self.YELLOW
        }
        if severity in colors:
            return colors[severity](text)
        return text

class IOperTest_FileStructure(IOperTestCaseBase):
    def if_file_exist(self, fname, fail_severity=None):
        file_path = os.path.join(self.param['mount_point'], fname)
        exist = os.path.isfile(file_path)
        if exist:
            self.result.append((self.PASS, "File '%s' exists" % file_path))
        else:
            self.result.append((fail_severity if fail_severity else self.ERROR, "File '%s' not found" % file_path))

    def test(self, param=None):
        self.param = param
        self.result = []
        if param:
            self.if_file_exist('mbed.htm', self.ERROR)
            self.if_file_exist('DETAILS.TXT', self.INFO)
            self.if_file_exist('FAIL.TXT', self.INFO)
        return self.result


class IOperTest_TargetID(IOperTestCaseBase):
    """ tests related to target_id value
    """

    TARGET_ID_LEN = 24

    def test_target_id_format(self, target_id, target_id_name):
        # Expected length == 24, eg. "02400203D94B0E7724B7F3CF"
        result = []
        target_id_len = len(target_id) if target_id else 0
        if target_id_len == self.TARGET_ID_LEN:
            result.append((self.PASS, "%s '%s' is %d chars long " % (target_id_name, target_id, target_id_len)))
            result.append((self.INFO, "%s Version String is %s.%s.%s " % (target_id_name, 
                                                                          target_id[0:4],
                                                                          target_id[4:8],
                                                                          target_id[8:24],
                                                                          )))
        else:
            result.append((self.ERROR, "%s '%s' is %d chars long. Expected %d chars" % (target_id_name, target_id, target_id_len, self.TARGET_ID_LEN)))
        return result

    def test_decode_target_id(self, target_id, target_id_name):
        result = []
        target_id_len = len(target_id) if target_id else 0
        if target_id_len >= 4:
            result.append((self.INFO, "%s Vendor Code is '%s'" % (target_id_name, target_id[0:2])))
            result.append((self.INFO, "%s Platform Code is '%s'" % (target_id_name, target_id[2:4])))
            result.append((self.INFO, "%s Firmware Version is '%s'" % (target_id_name, target_id[4:8])))
            result.append((self.INFO, "%s Hash of secret is '%s'" % (target_id_name, target_id[8:24])))
        return result

    def test(self, param=None):
        result = []

        if param:
            result.append((self.PASS, "TargetID '%s' found" % param['target_id']))

            # Check if target name can be decoded with mbed-ls
            if param['platform_name']:
                result.append((self.PASS, "TargetID '%s' decoded as '%s'" % (param['target_id'][0:4], param['platform_name'])))
            else:
                result.append((self.ERROR, "TargetID '%s'... not decoded" % (param['target_id'] if param['target_id'] else '')))

            # Test for USBID and mbed.htm consistency
            if param['target_id_mbed_htm'] == param['target_id_usb_id']:
                result.append((self.PASS, "TargetID (USBID) and TargetID (mbed.htm) match"))
            else:
                text = "TargetID (USBID) and TargetID (mbed.htm) don't match: '%s' != '%s'" % (param['target_id_usb_id'], param['target_id_mbed_htm'])
                result.append((self.WARN, text))

            # Target ID tests:
            result += self.test_target_id_format(param['target_id_usb_id'], "TargetId (USBID)")
            result += self.test_target_id_format(param['target_id_mbed_htm'], "TargetId (mbed.htm)")

            # Some extra info about TargetID itself
            result += self.test_decode_target_id(param['target_id_usb_id'], "TargetId (USBID)")
            result += self.test_decode_target_id(param['target_id_mbed_htm'], "TargetId (mbed.htm)")
        else:
            result.append((self.ERROR, "TargetID not found"))

        return result

##########################################################################

class IOperTestRunenr():
    """ Calls all i/face interoperability tests
    """

    def run(self):
        """ Run tests, calculate overall score and print test results
        """
        mbeds = mbed_lstools.create()
        muts_list = mbeds.list_mbeds()
        test_base = IOperTestCaseBase()

        for mut in muts_list:
            result = []
            print "MBEDLS: Detected %s, port: %s, mounted: %s"% (mut['platform_name'],
                                                                 mut['serial_port'],
                                                                 mut['mount_point'])
            for test_case in TEST_LIST:
                res = test_case.test(param=mut)
                result.extend(res)

            columns = ['Severity', 'Description']
            pt = PrettyTable(columns)
            for col in columns:
                pt.align[col] = 'l'

            for tr in result:
                severity, text = tr
                tr = (test_base.COLOR(severity, severity), test_base.COLOR(severity, text))
                pt.add_row(list(tr))
            print pt.get_string(border=False)


##########################################################################

TEST_LIST = [IOperTest_TargetID(),
             IOperTest_FileStructure()
            ]


if __name__ == '__main__':
    init()  # colorama

    tests = IOperTestRunenr()
    test_results = tests.run()
