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
import mbed_lstools
from prettytable import PrettyTable

try:
    from colorama import init
except:
    pass

COLORAMA = 'colorama' in sys.modules

from ioper_base import IOperTestCaseBase
from ioper_test_fs import IOperTest_FileStructure_Basic
from ioper_test_fs import IOperTest_FileStructure_MbedEnabled
from ioper_test_target_id import IOperTest_TargetID_Basic
from ioper_test_target_id import IOperTest_TargetID_MbedEnabled


TEST_LIST = [IOperTest_TargetID_Basic('basic'),
             IOperTest_TargetID_MbedEnabled('mbed-enabled'),
             IOperTest_FileStructure_Basic('basic'),
             IOperTest_FileStructure_MbedEnabled('mbed-enabled'),
             IOperTestCaseBase('all'),  # Dummy used to add 'all' option
            ]


class IOperTestRunner():
    """ Calls all i/face interoperability tests
    """

    def __init__(self, scope=None):
        """ Test scope:
            'pedantic' - all
            'mbed-enabled' - let's try to check if this device is mbed-enabled
            'basic' - just simple, passive tests (no device flashing)
        """
        self.requested_scope = scope    # Test scope given by user
        self.raw_test_results = {}      # Raw test results, can be used by exporters: { Platform: [test results]} 

        # Test scope definitions
        self.SCOPE_BASIC = 'basic'                  # Basic tests, sanity checks
        self.SCOPE_MBED_ENABLED = 'mbed-enabled'    # Let's try to check if this device is mbed-enabled
        self.SCOPE_PEDANTIC = 'pedantic'            # Extensive tests
        self.SCOPE_ALL = 'all'                      # All tests, equal to highest scope level

        # This structure will help us sort test scopes so we can include them
        # e.g. pedantic also includes basic and mbed-enabled tests
        self.scopes = {self.SCOPE_BASIC : 0,
                       self.SCOPE_MBED_ENABLED : 1,
                       self.SCOPE_PEDANTIC : 2,
                       self.SCOPE_ALL : 99,
                    }

        if COLORAMA:
            init()  # colorama.init()

    def run(self):
        """ Run tests, calculate overall score and print test results
        """
        mbeds = mbed_lstools.create()
        muts_list = mbeds.list_mbeds()
        test_base = IOperTestCaseBase()

        self.raw_test_results = {}
        for i, mut in enumerate(muts_list):
            result = []
            self.raw_test_results[mut['platform_name']] = []

            print "MBEDLS: Detected %s, port: %s, mounted: %s"% (mut['platform_name'],
                                                                 mut['serial_port'],
                                                                 mut['mount_point'])
            print "Running interoperability test suite, scope '%s'" % (self.requested_scope)
            for test_case in TEST_LIST:
                if self.scopes[self.requested_scope] >= self.scopes[test_case.scope]:
                    res = test_case.test(param=mut)
                    result.extend(res)
                    self.raw_test_results[mut['platform_name']].extend(res)

            columns = ['Platform', 'Test Case', 'Result', 'Scope', 'Description']
            pt = PrettyTable(columns)
            for col in columns:
                pt.align[col] = 'l'

            for tr in result:
                severity, tr_name, tr_scope, text = tr
                tr = (test_base.COLOR(severity, mut['platform_name']),
                      test_base.COLOR(severity, tr_name),
                      test_base.COLOR(severity, severity),
                      test_base.COLOR(severity, tr_scope),
                      test_base.COLOR(severity, text))
                pt.add_row(list(tr))
            print pt.get_string(border=True, sortby='Result')
            if i + 1 < len(muts_list):
                print
        return self.raw_test_results

def get_available_oper_test_scopes():
    """ Get list of available test scopes
    """
    scopes = set()
    for oper_test in TEST_LIST:
        if oper_test.scope is not None:
            scopes.add(oper_test.scope)
    return list(scopes)
