"""
Copyright 2018 ARM Limited
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
            'getlastnfcerror':  self.trace_parser,
            'setlastnfcerror':  self.trace_parser,
            'iseeprom': self.trace_parser,
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

    def trace_parser(self, response):
        results = {'iseeprom': None,        # 'true' if EEPROM
                   'lastnfcerror':None,     # 0=OK >0 = error
                   'nfcmessage':None,       # NDEF array of bytes
                   'protocols':None,     # csv list
                   'uri_id':None}        # nfc URI type identifier
        respLines = response.lines
        for line in respLines:
            try:
                value = PluginBase.find_one(line, "{{lastnfcerror=([0-9]+)}}")
                if value is not False:
                    results['lastnfcerror'] = int(value)
                # iseeprom
                value = PluginBase.find_one(line, "{{iseeprom=([\w]+)}}")
                if value is not False:
                    if ("TRUE" == value.upper() or "1" == value):
                        results['iseeprom'] = True
                    else:
                        results['iseeprom'] = False
                # nfcmessage (hex data dumps)
                data = PluginBase.find_one(line, "{{nfcmessage=([0-9a-f\s]*)}}")
                if data is not False:
                    value = []
                    for byte in data.split(' '):
                        if bool(byte):
                            value.append( int(byte, 16))
                    results['nfcmessage'] = value
                # t1t,t2t,t3t,isodep,nfcdef,t5t
                value = PluginBase.find_one(line, "{{protocols=(([\w]*,?)*)}}")
                if value is not False:
                    results['protocols'] = value  # a csv list
                value = PluginBase.find_one(line, "{{uri_id=([0-9]+)}}")
                if value is not False:
                    results['uri_id'] = int(value)
            except re.error as e: # the framework gobbles errors in the plugin
                print("Regex error",e,"occured in",os.path.basename(__file__), "!!")
                raise e
        return results
