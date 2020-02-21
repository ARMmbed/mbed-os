"""
Copyright 2018 ARM Limited
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


from icetea_lib.Plugin.PluginBase import PluginBase
import re
import os

class NfcTestParsers(PluginBase):
    # constructor
    def __init__(self):
        super(NfcTestParsers, self).__init__()

    def get_parsers(self):
        return {
            'iseeprom': self.status_parser,
            'getmaxndef': self.status_parser,
            'getlastnfcerror':  self.trace_parser,
            'setlastnfcerror':  self.trace_parser,
            'initnfc':  self.trace_parser, # all commands that return an NFC error code
            'readmessage' : self.trace_parser,
            'erase' : self.trace_parser,
            'writelong' : self.trace_parser,
            'stop' : self.trace_parser,
            'start' : self.trace_parser,
            'setsmartposter':   self.trace_parser,
            'getprotocols':   self.trace_parser,
            'setprotocols':   self.trace_parser
        }

    def status_parser(self, response):
        """
        commands that do NOT return an NFC result-code
        """
        results = {'iseeprom': None,        # 'true' if EEPROM
                   'lastnfcerror':0,        # ignored here
                    'maxndef':None          # actual driver buffer size
                   }
        respLines = response.lines
        for line in respLines:
            try:
                # iseeprom
                value = PluginBase.find_one(line, "{{iseeprom=([\w]+)}}")
                if value is not False:
                    if ("TRUE" == value.upper() or "1" == value):
                        results['iseeprom'] = True
                    else:
                        results['iseeprom'] = False
                # max ndef
                value = PluginBase.find_one(line, "{{maxndef=([0-9]+)}}")
                if value is not False:
                    results['maxndef'] = int(value)
            except re.error as e: # the framework gobbles errors in the plugin
                print("Regex error",e,"occured in",os.path.basename(__file__), "!!")
                raise e
        return results

    def convert_from_hex(self, data):
        value = []
        for byte in data.split(' '):
            if bool(byte):
                if len(byte) > 2: # the line wrapping code for huge lines appears to munch the space character between 2 bytes
                    value.append( int(byte[:2], 16))
                    value.append( int(byte[2:4], 16))
                else:
                    value.append( int(byte, 16))
        return value

    # regex findall with additional filtering
    @staticmethod
    def find_all_hex_data(line, lookup):
        """
        regexp search with one value to return.

        :param line: Line
        :param lookup: regexp
        :return: List of match groups or False
        """
        results = re.findall(lookup, line)
        if results is not False:
            results = [m for m in results if len(m) > 2] # remove empty matches and
            # non hex-data result entries
            return results
        return False

    def trace_parser(self, response):
        results = {'iseeprom': None,        # 'true' if EEPROM
                   'lastnfcerror':None,     # 0=OK >0 = error
                   'nfcmessage':None,       # NDEF array of bytes
                   'protocols':None,     # csv list
                   'uri_id':None}        # nfc URI type identifier
        respLines = response.lines
        started_read_data = False
        partial_data = ""
        for line in respLines:
            try:
                value = PluginBase.find_one(line, "{{lastnfcerror=([0-9]+)}}")
                if value is not False:
                    results['lastnfcerror'] = int(value)
                # {{nfcmessage=([0-9a-f\s]*)}}
                # message may be split over multiple lines, so we will start a search and wait until we get a }} pair
                data = PluginBase.find_one(line, "{{nfcmessage=([0-9a-f\s]*).*")
                if data is not False:
                    started_read_data = True
                if started_read_data:  # read data until we see a }} pair
                    if "{{nfcmessage=" in line:
                        line = line[13:] # trim header
                    if PluginBase.find_one(line, ".*(}})") is not False: # search for end marker
                        started_read_data = False
                        line = line[:-2] # trim closing }}
                    partial_data += line
                    if not started_read_data:
                        results['nfcmessage'] = self.convert_from_hex(partial_data)
                # t1t,t2t,t3t,isodep,nfcdef,t5t
                value = PluginBase.find_one(line, "{{protocols=(([\w]*,?)*)}}")
                if value is not False:
                    results['protocols'] = value  # a csv list
                # smartposter uri
                value = PluginBase.find_one(line, "{{uri_id=([0-9]+)}}")
                if value is not False:
                    results['uri_id'] = int(value)
            except re.error as e: # the framework gobbles errors in the plugin
                print("Regex error",e,"occured in",os.path.basename(__file__), "!!")
                raise e
        return results
