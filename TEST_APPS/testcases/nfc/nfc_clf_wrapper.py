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
import nfc
from nfc.clf import RemoteTarget
import logging

logprefixnfc = "NFCPY: "

"""
 Wrap calls to nfcpi testing module, handle loading the driver
"""


def command_is(string, command):
    return string.split(' ')[0] == command


def debug_nfc_data(key, value):
    """
    print useful data values for the host/user {{in between}} easy to spot brackets.
    """
    text = "{{%s=%s}}" % (key, value)
    logger.info(text)


logger = logging.getLogger()


class NfcWrapper:
    """
    Finds the NFC reader USB front-end and prepares it for use.
    """
    def __init__(self):
        # will need a parameter here to help with libusb detection?
        # clf.open("usb:04e6:5591") will open the SCL3711-NFC
        logger.info("Initializing the NFC tag reader...")
        self.clf = nfc.ContactlessFrontend()
        if (self.clf.open("usb") ):  # the NFC reader was not detected on any USB port!
            logger.info("NFC Frontend found OK")
        else:
            logger.error("The NFC reader was not detected on any USB port!")
        self.clfResponse = None

    def clf_response(self):
        return self.clfResponse

    def parse(self,line):
        logging.debug(line)
        parseok = False
        # find command and call the needed nfcWrapper method
        if command_is(line, "ping"):
            self.pong()
            parseok = True
        if command_is(line, "connect"):
            detectedTag = self.connect()
            debug_nfc_data("Connectedtag", detectedTag)
            parseok = True
        if command_is(line, "mute"):
            detectedTag = self.mute()
            parseok = True
        if command_is(line, "disconnect"):
            self.disconnect()
            parseok = True
        return parseok

    """return the detected tag, else timeout after interval"""
    def sense(self, target_options = ("106A","106B","212F")):
        logging.info(logprefixnfc + "detecting tags with options " + target_options)
        # todo filter using the target_options
        targets = self.clf.sense(RemoteTarget('106A'), RemoteTarget('106B'), RemoteTarget('212F'))
        self.clfResponse = targets
        return targets

    def connect(self, target_options = ("106A","106B","212F")):
        # todo: decide on tag types to allow/filter
        after5s = lambda: time.time() - started > 5
        started = time.time()
        tag = self.clf.connect( rdwr={'on-connect': lambda tag: False},
            llcp={}, terminate = after5s)
        self.clfResponse = tag
        if tag: # None if timeout expires
            logging.info(logprefixnfc + str(tag))
        return tag

    def mute(self):
        """turn off the reader radio"""
        if self.clf.device:
            logging.info(logprefixnfc + "radio mute" + self.clf.device.product_name)
            self.clf.device.mute()

    def disconnect(self):
        logging.info(logprefixnfc + "close frontend.")
        self.clf.close()

"""
Handle interactions with the NFC reader, and singleton
"""
class ContactlessCommandRunner():

    """
    Lazy initialization singleton to open the reader once only - else when the framework scans for 
    tests, it causes us to open the reader. This breaks the Windows driver.
    """
    def __getattr__(self, name):
        if name == 'nfc':

            if ContactlessCommandRunner.__nfc_wrapper is None:
                ContactlessCommandRunner.__nfc_wrapper = NfcWrapper()
            return ContactlessCommandRunner.__nfc_wrapper

    __nfc_wrapper = None

    # plumbing, calls a static instance for the reader object.
    def parse(self, line):
        return self.nfc.parse(line)

    def clf_response(self):
        return self.nfc.clf_response()
