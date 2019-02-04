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

"""
 Wrap calls to nfcpi testing module, handle loading the driver
"""



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

    def connect(self):
        # note: only supporting type4
        time.sleep(0.5)
        after5s = lambda: time.time() - started > 5
        started = time.time()

        tag = self.clf.connect( rdwr={'on-connect': lambda tag: False},
            terminate = after5s)
        if tag: # None if timeout expires
            logging.info("NFCReader: connected " + str(tag))
        else:
            logging.info("NFCReader: warning, no tag detected ")
        return tag

    def mute(self):
        """turn off the reader radio"""
        if (self.clf.device is not None):
            logging.info("NFCReader: radio mute" + self.clf.device.product_name)
            self.clf.device.mute()
        else:
            logging.warning("NFCReader: reader not initialized!")

    def disconnect(self):
        logging.info("NFCReader: close frontend.")
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

