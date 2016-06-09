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
from ioper_base import IOperTestCaseBase


class IOperTest_FileStructure(IOperTestCaseBase):

    def __init__(self, scope=None):
        IOperTestCaseBase.__init__(self, scope)

    def if_file_exist(self, fname, fail_severity=None):
        file_path = os.path.join(self.param['mount_point'], fname)
        exist = os.path.isfile(file_path)
        tr_name = "FILE_EXIST(%s)" % fname.upper()
        if exist:
            self.result.append((self.PASS, tr_name, self.scope, "File '%s' exists" % file_path))
        else:
            self.result.append((fail_severity if fail_severity else self.ERROR, tr_name, self.scope, "File '%s' not found" % file_path))

    def test(self, param=None):
        self.result = []
        if param:
            pass
        return self.result


class IOperTest_FileStructure_Basic(IOperTest_FileStructure):
    def __init__(self, scope=None):
        IOperTest_FileStructure.__init__(self, scope)

    def test(self, param=None):
        self.param = param
        self.result = []
        if param:
            self.if_file_exist('mbed.htm', self.ERROR)
        return self.result


class IOperTest_FileStructure_MbedEnabled(IOperTest_FileStructure):
    def __init__(self, scope=None):
        IOperTest_FileStructure.__init__(self, scope)

    def test(self, param=None):
        self.param = param
        self.result = []
        if param:
            self.if_file_exist('mbed.htm', self.ERROR)
            self.if_file_exist('DETAILS.TXT', self.ERROR)
            self.if_file_exist('FAIL.TXT', self.INFO)
        return self.result
