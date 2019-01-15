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


from icetea_lib.bench import Bench
from nfc_clf_wrapper import ContactlessCommandRunner
import nfc_messages
import time
from mbed_clitest.tools.tools import test_case
import icetea_lib.tools.asserts as asserts
from nfc_messages import NfcErrors
from nfc_cli_helper import CliHelper
from nfc_cli_helper import STRESS_BUFFLEN


class CreamSconeTests(Bench, CliHelper):
    """
    This test wrapper requires a usb connected contactless card reader dongle, and allows E2E testing.
    See readme file for details
    """
    def __init__(self, **kwargs):
        testcase_args = {
                       'title':"NFC tests with a reader",
                       'status':"development",
                       'purpose':"NFC e2e",
                       'component':["NFC"],
                       'type':"regression",
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
        try:
            self.clf = ContactlessCommandRunner()
            self.clf.parse("mute")
        except:
            raise asserts.TestStepFail("Could not find NFC reader")

    def teardown(self):
        self.logger.info("Test teardown: Reboot target...")
        self.reset_dut()



@test_case(CreamSconeTests)
def test_nfce2e_target_found(self):
    """
    smoke - Our emulated tag is detectable
    """

    response = self.nfc_command("dev1", "iseeprom")
    eeprom = response.parsed['iseeprom']

    # Invokes icetea command, this method also checks the NFC api result expected_nfc_error=NFC_OK
    # Tester can supply the expected_nfc_error=NFC_XXX parameter, to override.
    self.nfc_command("dev1", "initnfc")
    if not eeprom:
        self.nfc_command("dev1", "start")

    self.clf.parse("connect")
    tag = self.clf.clf_response()
    asserts.assertNotNone(tag, "Could not connect to any tag")


@test_case(CreamSconeTests)
def test_nfce2e_type4_found(self):
    """
    check - Type 4 tag is detected wirelessly
	NOTE: If the tage emulation does not default to type4, this test needs to be modified
	   +++ self.nfc_command("dev1", "setprotocols t4t")
	   or the assertion be adapated according to support desired level
    """

    response = self.nfc_command("dev1", "iseeprom")
    eeprom = response.parsed['iseeprom']
    self.nfc_command("dev1", "initnfc")
    if not eeprom:
        self.nfc_command("dev1", "start")

    self.clf.parse("connect")
    tag = self.clf.clf_response()
    asserts.assertNotNone(tag, "Could not connect to any tag")

    asserts.assertEqual(tag.type, 'Type4Tag', "Tag of type Type4Tag not found")


@test_case(CreamSconeTests)
def test_nfce2e_smartposter(self):
    """
    check - Tag can be set and read via contactless 
    """
    expectedURI = "https://www.mbed.com"    # ensure that these differ per test case

    self.nfc_command("dev1", "initnfc")
    self.nfc_command("dev1", "erase")
    response = self.nfc_command("dev1", "iseeprom")
    eeprom = response.parsed['iseeprom']

    self.logger.info("Target includes NFCEEPROM: %s" % eeprom)
    if not eeprom:
        self.nfc_command("dev1", "start")

    # write poster tag to target
    self.command("dev1", "setsmartposter %s" % expectedURI)

    self.clf.parse("connect")
    tag = self.clf.clf_response()
    asserts.assertNotNone(tag, "Could not connect to any tag")
    asserts.assertEqual(1, len(tag.ndef.records), "expected number NDEF records")

    asserts.assertEqual(tag.ndef.records[0].__class__.__name__, "SmartposterRecord", "expected SmartposterRecord")
    asserts.assertEqual(expectedURI, tag.ndef.records[0].uri_records[0].uri, "expected exact URI")


@test_case(CreamSconeTests)
def test_nfce2e_reprogrammed(self):
    """
    check - Tag can be programmed from a remote and read via contactless
    """
    expectedURI = "https://www.google.com"

    response = self.nfc_command("dev1", "iseeprom")
    eeprom = response.parsed['iseeprom']
    self.logger.info("Target includes NFCEEPROM: %s" % eeprom)

    self.nfc_command("dev1", "initnfc")
    if not eeprom:
        self.nfc_command("dev1", "start")
    self.nfc_command("dev1", "erase")

    # program a poster tag to target
    self.clf.parse("connect")
    tag = self.clf.clf_response()
    asserts.assertNotNone(tag, "Could not connect to any tag")
    smartposter = nfc_messages.make_smartposter(expectedURI, ["en-US:Other search engines exist"])
    nfc_messages.program_remote_tag(smartposter, tag)
    self.logger.info("Remote programmed %d bytes Smartposter" % len(str(smartposter)))

    # read device
    self.clf.parse("connect")
    tag = self.clf.clf_response()
    asserts.assertNotNone(tag, "Could not re-connect to any tag")

    asserts.assertEqual(tag.ndef.records[0].__class__.__name__, "SmartposterRecord", "expected SmartposterRecord")
    asserts.assertEqual(expectedURI, tag.ndef.records[0].uri_records[0].uri, "expected exact URI")
    self.clf.parse("mute")    # disable radio, to allow a local session

    # verify in target
    response = self.nfc_command("dev1", "readmessage")

    # check contents
    expected_message = str(smartposter)
    self.assert_binary_equal(response.parsed['nfcmessage'], expected_message)


@test_case(CreamSconeTests)
def test_nfce2e_read_stress(self):
    """
    check - Large record can be programmed in and read via contactless
    """
    messageRep = 'thequickbrownfoxjumpedoverthelazydog' # repeating message written

    response = self.nfc_command("dev1", "iseeprom")
    eeprom = response.parsed['iseeprom']
    self.logger.info("Target includes NFCEEPROM: %s" % eeprom)

    self.nfc_command("dev1", "initnfc")
    if not eeprom:
        self.nfc_command("dev1", "start")
        textLength = STRESS_BUFFLEN
    else:
        max_ndef = self.nfc_command("dev1", "getmaxndef").parsed['maxndef']
        textLength =  max_ndef / 2 # large values slow down test runs and may time out
    self.nfc_command("dev1", "erase")
    # calculate actual message to compare to using the library
    expected_text = nfc_messages.repeat_string_to_length(messageRep, textLength)

    # write a large message to the tag via API, then read it wirelessly
    self.nfc_command("dev1", "writelong %d %s" % (textLength,messageRep))
    self.clf.parse("connect")
    tag = self.clf.clf_response()
    asserts.assertNotNone(tag, "Could not connect to any tag")

    # assert that read the eeprom contents gives correct data and length
    asserts.assertEqual(tag.ndef.records[0].__class__.__name__, "TextRecord", "expected TextRecord")
    self.assert_text_equal(tag.ndef.records[0].text, expected_text)


@test_case(CreamSconeTests)
def test_nfce2e_reprogrammed_stress(self):
    """
    check - Large record can be programmed from a remote and read via contactless
    """
    messageRep = 'thequickbrownfoxjumpedoverthelazydog' # repeating message written


    response = self.nfc_command("dev1", "iseeprom")
    eeprom = response.parsed['iseeprom']
    self.logger.info("Target includes NFCEEPROM: %s" % eeprom)

    self.nfc_command("dev1", "initnfc")
    if not eeprom:
        self.nfc_command("dev1", "start")
        textLength = STRESS_BUFFLEN
    else:
        max_ndef = self.nfc_command("dev1", "getmaxndef").parsed['maxndef']
        textLength =  max_ndef / 2  # large values slow down test runs and may time out
    # calculate actual message to compare to using the library
    message = nfc_messages.make_textrecord( nfc_messages.repeat_string_to_length(messageRep, textLength))
    expected_message = str(message)
    self.nfc_command("dev1", "erase")

    # program a large tag to target remotely
    self.clf.parse("connect")
    tag = self.clf.clf_response()
    asserts.assertNotNone(tag, "Could not connect to any tag")
    nfc_messages.program_remote_tag(message, tag)
    self.logger.info("%d bytes chunk of data written to tag remotely" %  len(str(message)))

    # read device locally
    self.clf.parse("connect")
    tag = self.clf.clf_response()
    asserts.assertNotNone(tag, "Could not re-connect to any tag")
    self.clf.parse("mute") # disable the reader radio, to allow local access

    # verify in target
    response = self.nfc_command("dev1", "readmessage")
    self.assert_binary_equal(response.parsed['nfcmessage'], expected_message)


@test_case(CreamSconeTests)
def test_nfce2e_discovery_loop(self):
    """
    check - Controller discovery loop stop/start
    fails : blocked by an issue on NFC controllers only
    """
    expectedURI = "https://www.nasa.com"    # ensure that these differ per test case

    response = self.command("dev1", "iseeprom") # will hold result from the init lib call
    eeprom = response.parsed['iseeprom']
    self.logger.info("Target includes NFCEEPROM: %s" % eeprom)

    self.nfc_command("dev1", "initnfc") # this NOT automatically start discovery at the same time, the test command
    # "start" must be used on a controller. (Eeeproms always have the loop enabled.)
	# By default, the test app automatically starts discovery loop again after a reader disconnects from the controller.
	# Automatic resume after disconnect can be turned off by using command "start man" , the default is "start auto" .

    if not eeprom:
        self.clf.parse("connect")
        tag = self.clf.clf_response()
        asserts.assertNone(tag, "post-init: Tag discovery loop should be stopped!")
        self.nfc_command("dev1", "stop")
        time.sleep(1)

        self.clf.parse("connect")
        tag = self.clf.clf_response()
        asserts.assertNone(tag, "post-stop: Tag discovery loop should be stopped!")
        self.nfc_command("dev1", "start")
        time.sleep(1)

        self.clf.parse("connect")
        tag = self.clf.clf_response()
        asserts.assertNotNone(tag, "Could not connect to any tag")

        self.clf.parse("mute")
        self.nfc_command("dev1", "stop")
        time.sleep(10)
        self.clf.parse("connect")
        tag = self.clf.clf_response()
        # test blocked by issue raised IOTPAN313 NFC Controller discovery can stop but cannot restart - PN512
        asserts.assertNone(tag, "post-restart: Tag discovery loop should be stopped!")

    else:
        # eeprom, so not supported
        self.nfc_command("dev1", "start", expected_retcode=-2, expected_nfc_error= NfcErrors.nfc_err_unsupported )
        self.nfc_command("dev1", "stop", expected_retcode=-2, expected_nfc_error= NfcErrors.nfc_err_unsupported )

