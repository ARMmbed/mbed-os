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

from ioper_base import IOperTestCaseBase


class IOperTest_TargetID(IOperTestCaseBase):
    """ tests related to target_id value
    """

    def __init__(self, scope=None):
        IOperTestCaseBase.__init__(self, scope)
        self.TARGET_ID_LEN = 24

    def test_target_id_format(self, target_id, target_id_name):
        # Expected length == 24, eg. "02400203D94B0E7724B7F3CF"
        result = []
        target_id_len = len(target_id) if target_id else 0
        if target_id_len == self.TARGET_ID_LEN:
            result.append((self.PASS, "TARGET_ID_LEN", self.scope, "%s '%s' is %d chars long " % (target_id_name, target_id, target_id_len)))
            result.append((self.INFO, "FW_VER_STR", self.scope, "%s Version String is %s.%s.%s " % (target_id_name,
                                                                                      target_id[0:4],
                                                                                      target_id[4:8],
                                                                                      target_id[8:24],
                                                                                      )))
        else:
            result.append((self.ERROR, "TARGET_ID_LEN", self.scope, "%s '%s' is %d chars long. Expected %d chars" % (target_id_name, target_id, target_id_len, self.TARGET_ID_LEN)))
        return result

    def test_decode_target_id(self, target_id, target_id_name):
        result = []
        target_id_len = len(target_id) if target_id else 0
        if target_id_len >= 4:
            result.append((self.INFO, "FW_VEN_CODE", self.scope, "%s Vendor Code is '%s'" % (target_id_name, target_id[0:2])))
            result.append((self.INFO, "FW_PLAT_CODE", self.scope, "%s Platform Code is '%s'" % (target_id_name, target_id[2:4])))
            result.append((self.INFO, "FW_VER", self.scope, "%s Firmware Version is '%s'" % (target_id_name, target_id[4:8])))
            result.append((self.INFO, "FW_HASH_SEC", self.scope, "%s Hash of secret is '%s'" % (target_id_name, target_id[8:24])))
        return result

    def test(self, param=None):
        result = []
        if param:
            pass
        return result


class IOperTest_TargetID_Basic(IOperTest_TargetID):
    """ Basic interoperability tests checking TargetID compliance
    """

    def __init__(self, scope=None):
        IOperTest_TargetID.__init__(self, scope)

    def test(self, param=None):
        result = []

        if param:
            result.append((self.PASS, "TARGET_ID", self.scope, "TargetID '%s' found" % param['target_id']))

            # Check if target name can be decoded with mbed-ls
            if param['platform_name']:
                result.append((self.PASS, "TARGET_ID_DECODE", self.scope, "TargetID '%s' decoded as '%s'" % (param['target_id'][0:4], param['platform_name'])))
            else:
                result.append((self.ERROR, "TARGET_ID_DECODE", self.scope, "TargetID '%s'... not decoded" % (param['target_id'] if param['target_id'] else '')))

            # Test for USBID and mbed.htm consistency
            if param['target_id_mbed_htm'] == param['target_id_usb_id']:
                result.append((self.PASS, "TARGET_ID_MATCH", self.scope, "TargetID (USBID) and TargetID (mbed.htm) match"))
            else:
                text = "TargetID (USBID) and TargetID (mbed.htm) don't match: '%s' != '%s'" % (param['target_id_usb_id'], param['target_id_mbed_htm'])
                result.append((self.WARN, "TARGET_ID_MATCH", self.scope, text))
        else:
            result.append((self.ERROR, "TARGET_ID", self.scope, "TargetID not found"))
        return result

class IOperTest_TargetID_MbedEnabled(IOperTest_TargetID):
    """ Basic interoperability tests checking TargetID compliance
    """

    def __init__(self, scope=None):
        IOperTest_TargetID.__init__(self, scope)

    def test(self, param=None):
        result = []

        if param:
            # Target ID tests:
            result += self.test_target_id_format(param['target_id_usb_id'], "TargetId (USBID)")
            result += self.test_target_id_format(param['target_id_mbed_htm'], "TargetId (mbed.htm)")

            # Some extra info about TargetID itself
            result += self.test_decode_target_id(param['target_id_usb_id'], "TargetId (USBID)")
            result += self.test_decode_target_id(param['target_id_mbed_htm'], "TargetId (mbed.htm)")
        return result
