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

import os.path
import mbed_lstools
from utils import construct_enum
from prettytable import PrettyTable


IOperTestSeverity = construct_enum(INFO='Info',
                                   WARN='Warning',
                                   ERROR='Error',
                                   PASS='Pass'
                                   )


class IOperTestCaseBase():
    """ Interoperability test case base class
        @return list of tuple (severity, Description)
                Example:      (result.append((IOperTestSeverity.INFO, ""))
    """
    def test(self, param=None):
        result = []
        return result


class IOperTest_FileStructure(IOperTestCaseBase):
    def if_file_exist(self, fname, fail_severity=None):
        file_path = os.path.join(self.param['mount_point'], fname)
        exist = os.path.isfile(file_path)
        if exist:
            self.result.append((IOperTestSeverity.PASS, "File '%s' exists" % file_path))
        else:
            self.result.append((fail_severity if fail_severity else IOperTestSeverity.ERROR, "File '%s' not found" % file_path))

    def test(self, param=None):
        self.param = param
        self.result = []
        if param:
            self.if_file_exist('mbed.htm', IOperTestSeverity.ERROR)
            self.if_file_exist('DETAILS.TXT', IOperTestSeverity.INFO)
            self.if_file_exist('FAIL.TXT', IOperTestSeverity.INFO)
        return self.result


class IOperTest_TargetID(IOperTestCaseBase):
    """ tests related to target_id value
    """
    
    def test(self, param=None):
        result = []

        if param:
            result.append((IOperTestSeverity.PASS, "TargetID '%s' found" % param['target_id']))
        
            # Check if target name can be decoded with mbed-ls
            if param['platform_name']:
                result.append((IOperTestSeverity.PASS, "TargetID '%s' decoded as '%s'" % (param['target_id'][0:4], param['platform_name'])))
            else:
                result.append((IOperTestSeverity.ERROR, "TargetID '%s'... not decoded"))

            # Test for USBID and mbed.htm consistency
            if param['target_id_mbed_htm'] == param['target_id_usb_id']:
                result.append((IOperTestSeverity.PASS, "TargetID in USBID and mbed.htm match"))
            else:
                result.append((IOperTestSeverity.WARN, "TargetID in USBID and mbed.htm do not match: '%s' != '%s'" % (param['target_id_mbed_htm'], param['target_id_usb_id'])))
        else:
            result.append((IOperTestSeverity.ERROR, "TargetID not found"))
            
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
                pt.add_row(list(tr))
            print pt.get_string()
                
                
        return result

##########################################################################

TEST_LIST = [IOperTest_TargetID(),
             IOperTest_FileStructure()
            ]


if __name__ == '__main__':
    tests = IOperTestRunenr()
    test_results = tests.run()
