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

import time
from icetea_lib.bench import Bench
from mbed_clitest.tools.tools import test_case
import icetea_lib.tools.asserts as asserts
import nfc_messages
from nfc_messages import NfcErrors
from nfc_cli_helper import CliHelper
from nfc_cli_helper import LARGE_BUFFLEN
import nfc

"""
Standalone (no NFC reader needed) tests, which cover API with no end-to-end checks.
"""
class CreamSconeSelfTests(Bench, CliHelper):
    def __init__(self, **kwargs):
        testcase_args = {
                       'title':"NFC tests with no reader",
                       'status':"development",
                       'purpose':"NFC target-only checks",
                       'component':["NFC"],
                       'type':"smoke",
                       'requirements':{
                           "duts": {
                               '*': {
                                   "count": 1,
                                   "type": "hardware",
                                   "application": {
                                       "name": "TEST_APPS-device-nfcapp"
                                   }
                               },
                               "1": {"nick": "dev1"}
                           }
                       }
        }
        testcase_args.update(kwargs)
        Bench.__init__(self, **testcase_args)

    def setup(self):
        pass

    def teardown(self):
        self.logger.info("Test teardown: Reboot target...")
        self.reset_dut()


"""
smoke - target app is running, and can exchange simple values
"""
@test_case(CreamSconeSelfTests)
def test_nfc_error_codes(self):
    wally = NfcErrors.nfc_err_not_found
    for x in range(0, 3):
        self.nfc_command("dev1", "setlastnfcerror %d" % wally.value, expected_nfc_error=wally)
        self.nfc_command("dev1", "getlastnfcerror", expected_nfc_error=wally)

        self.nfc_command("dev1", "setlastnfcerror %d" % 0)
        self.nfc_command("dev1", "getlastnfcerror")


"""
smoke - target app reports if NFC eeprom driver present
"""
@test_case(CreamSconeSelfTests)
def test_nfc_eeprom(self):

    response = self.command("dev1", "iseeprom") # will hold result from the init lib call
    self.logger.info("Target includes NFCEEPROM: %s" % response.parsed['iseeprom'])

"""
check - Assert discovery can be started/stopped
"""
@test_case(CreamSconeSelfTests)
def test_nfc_discovery(self):

    self.nfc_command("dev1", "initnfc")

    response = self.nfc_command("dev1", "iseeprom") # will hold result from the init lib call
    eeprom = response.parsed['iseeprom']
    self.logger.info("Target includes NFCEEPROM: %s" % eeprom)
    if not eeprom:
        self.nfc_command("dev1", "start")
        self.nfc_command("dev1", "stop")
        self.nfc_command("dev1", "start")
        self.nfc_command("dev1", "stop")
    else:
        # eeprom, so not supported
        self.nfc_command("dev1", "start", expected_retcode=-2, expected_nfc_error = NfcErrors.nfc_err_unsupported )
        self.nfc_command("dev1", "stop", expected_retcode=-2 , expected_nfc_error= NfcErrors.nfc_err_unsupported )

"""
check - Create a SmartPoster but does not read it back
"""
@test_case(CreamSconeSelfTests)
def test_nfc_setsmartposter(self):

    self.nfc_command("dev1", "initnfc")

    self.nfc_command("dev1", "setsmartposter https://www.mbed.com")

@test_case(CreamSconeSelfTests)
def test_nfc_erase(self):
    self.nfc_command("dev1", "initnfc")
    response = self.nfc_command("dev1", "iseeprom")
    eeprom = response.parsed['iseeprom']
    if eeprom:
        self.logger.info("Target includes NFCEEPROM: %s" % eeprom)
    self.nfc_command("dev1", "erase", timeout=30)
    response = self.nfc_command("dev1", "readmessage")
    asserts.assertEqual(response.parsed['nfcmessage'] is None, True)

'''
check - Build a long message by copying a string to stress the driver with a nominal buffer. Verify contents of entire message 
can be read back.
'''
@test_case(CreamSconeSelfTests)
def test_nfc_write_long(self):
    messageRep = 'thequickbrownfoxjumpedoverthelazydog' # repeating message written
    textLength = LARGE_BUFFLEN        # large values take longer
    # calculate actual message to compare to using the library
    message = nfc_messages.make_textrecord( nfc_messages.repeat_string_to_length(messageRep, textLength))
    expected_message = str(message)

    self.nfc_command("dev1", "initnfc")
    response = self.nfc_command("dev1", "iseeprom")
    eeprom = response.parsed['iseeprom']
    if eeprom:
        self.logger.info("Target includes NFCEEPROM: %s" % eeprom)
    self.nfc_command("dev1", "erase")
    self.nfc_command("dev1", "writelong %d %s" % (textLength,messageRep))
    response = self.nfc_command("dev1", "readmessage")
    # assert that read the eeprom contents gives textlength bytes (including framing bytes which will vary)
    self.assert_binary_equal(response.parsed['nfcmessage'], expected_message)

'''
check - Query supported protocols if we have a controller
'''
@test_case(CreamSconeSelfTests)
def test_nfc_get_controller_protocols(self):
    self.nfc_command("dev1", "initnfc")

    response = self.nfc_command("dev1", "iseeprom")
    eeprom = response.parsed['iseeprom']
    if eeprom:
        self.logger.info("Test ignore - target includes NFCEEPROM: %s" % eeprom)
    else:
        response = self.nfc_command("dev1", "getprotocols")
        self.logger.info("Protocols = %s" % response.parsed['protocols'])
        self.assertNotEqual(len(response.parsed['protocols']), 0, "Expected at least 1 protocol supported")


'''
check - Can set used protocols if we have a controller
Note: Currently only support Typ4 tags in PN512 driver
'''
@test_case(CreamSconeSelfTests)
def test_nfc_set_controller_protocols(self):
    self.nfc_command("dev1", "initnfc")

    response = self.nfc_command("dev1", "iseeprom")
    eeprom = response.parsed['iseeprom']
    if eeprom:
        # eeproms do not allow target control
        self.logger.info("Test ignore - target includes NFCEEPROM: %s" % eeprom)
    else:
        self.nfc_command("dev1", "setprotocols t1t")
        self.nfc_command("dev1", "setprotocols t2t")
        self.nfc_command("dev1", "setprotocols t3t")
        self.nfc_command("dev1", "setprotocols isodep")
        self.nfc_command("dev1", "setprotocols nfcdep")
        self.nfc_command("dev1", "setprotocols t5t")
        self.nfc_command("dev1", "setprotocols t1t t2t t3t isodep nfcdep t5t")

'''
check - SmartPoster URI forms are supported (in the test-app)
'''
@test_case(CreamSconeSelfTests)
def test_nfc_check_smartposter_uri_forms(self):
    def enum(**enums):
        return type('Enum', (), enums)

    IDS = enum(NA=0x00,  # Not applicable
               HTTP_WWW=0x01,  # http://www.
               HTTPS_WWW=0x02,  # https://www.
               HTTP=0x03,  # http://
               HTTPS=0x04,  # https://
               TEL=0x05,  # tel:
               MAILTO=0x06,  # mailto:
               FTP_ANONYMOUS=0x07,  # ftp://anonymous:anonymous@
               FTP_FTP=0x08,  # ftp://ftp.
               FTPS=0x09,  # ftps://
               SFTP=0x0A,  # sftp://
               SMB=0x0B,  # smb://
               NFS=0x0C,  # nfs://
               FTP=0x0D,  # ftp://
               DAV=0x0E,  # dav://
               NEWS=0x0F,  # news:
               TELNET=0x10,  # telnet://
               IMAP=0x11,  # imap:
               RSTP=0x12,  # rstp://
               URN=0x13,  # urn:
               POP=0x14,  # pop:
               SIP=0x15,  # sip:
               SIPS=0x16,  # sips:
               TFTP=0x17,  # tftp:
               BTSPP=0x18,  # btspp://
               BTL2CAP=0x19,  # btl2cap://
               BTGOEP=0x1A,  # btgoep://
               TCPOBEX=0x1B,  # tcpobex://
               IRDAOBEX=0x1C,  # irdaobex://
               FILE=0x1D,  # file://
               URN_EPC_ID=0x1E,  # urn:epc:id:
               URN_EPC_TAG=0x1F,  # urn:epc:tag:
               URN_EPC_PAT=0x20,  # urn:epc:pat:
               URN_EPC_RAW=0x21,  # urn:epc:raw:
               URN_EPC=0x22,  # urn:epc:
               URN_NFC=0x23,  # urn:nfc:
               )
    self.nfc_command("dev1", "initnfc")
    result = self.nfc_command("dev1", "setsmartposter https://www.mbed.com")
    asserts.assertEqual(result.parsed['uri_id'], IDS.HTTPS_WWW, "uri type expected HTTPS_WWW")
    result = self.nfc_command("dev1", "setsmartposter http://www.mbed.com")
    asserts.assertEqual(result.parsed['uri_id'], IDS.HTTP_WWW)
    result = self.nfc_command("dev1", "setsmartposter https://www.topleveldomain")
    asserts.assertEqual(result.parsed['uri_id'], IDS.HTTPS_WWW)
    result = self.nfc_command("dev1", "setsmartposter tel:555-5551234")
    asserts.assertEqual(result.parsed['uri_id'], IDS.TEL)
    result = self.nfc_command("dev1", "setsmartposter ftp://www.mbed.com/files/")
    asserts.assertEqual(result.parsed['uri_id'], IDS.FTP )

'''
smoke - driver buffer size can be retrieved
'''
@test_case(CreamSconeSelfTests)
def test_nfc_get_max_ndef(self):
    self.nfc_command("dev1", "initnfc")
    max = self.nfc_command("dev1", "getmaxndef").parsed['maxndef']
    self.logger.info("Target NDEF max buffer size %d" % max)
    self.logger.info("Teststress size %d" % LARGE_BUFFLEN)

