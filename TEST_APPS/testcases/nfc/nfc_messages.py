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

import nfc
from enum import Enum
import logging


class NfcErrors(Enum):
    nfc_ok =                   0
    nfc_err_unknown =          1
    nfc_err_length =           2
    nfc_err_not_found =        3
    nfc_err_unsupported =      4
    nfc_err_params =           5
    nfc_err_buffer_too_small = 6
    nfc_err_timeout =          7
    nfc_err_crc =              8
    nfc_err_nopeer =           9
    nfc_err_parity =          10
    nfc_err_field =           11
    nfc_err_collision =       12
    nfc_err_wrong_comm =      13
    nfc_err_protocol =        14
    nfc_err_busy =            15
    nfc_err_controller =      16
    nfc_err_halted =          17
    nfc_err_mac =             18
    nfc_err_underflow =       19
    nfc_err_disconnected =    20
    nfc_err_aborted =         21


'''
return a 'T'ext text ndef record
'''
def make_textrecord(text, language='en-US'):
    return nfc.ndef.Message(nfc.ndef.TextRecord(text, language))

'''
Return an NDEF message
resource -- url
titles -- list of : colon delimited titles where an optional language code precedes the title - 
if lang codes are omitted, 'en' is assumed
action -- one of default/save/exec/edit
'''
def make_smartposter(resource, titles, action = 'default'):
    record = nfc.ndef.SmartPosterRecord(resource)
    for title in titles:
        lang, text = title.split(':', 1) if ':' in title else ('en', title)
        record.title[lang] = text
    if not action in ('default', 'exec', 'save', 'edit'):
        logging.error("action not one of 'default', 'exec', 'save', 'edit'")
        return
    record.action = action

    return nfc.ndef.Message(record)


'''
Program the provided NDEF messsage into the tag (authentication is not required)
'''
def program_remote_tag(message, tag):
    if not tag.ndef.is_writeable:
        logging.error("This Tag is not writeable.")
        return False
    tag.ndef.message = message
    logging.info("Programmed tag OK.")
    return True

'''
Builds a long string by repeating a shorter string up to the required length
'''
def repeat_string_to_length(string_to_expand, length):
    return (string_to_expand * ((length/len(string_to_expand))+1))[:length]
