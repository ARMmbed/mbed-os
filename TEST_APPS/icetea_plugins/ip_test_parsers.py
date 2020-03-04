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
import re
import time
from collections import OrderedDict
from datetime import datetime

from icetea_lib.Plugin.PluginBase import PluginBase


class IpTestParsers(PluginBase):
    # constructor
    def __init__(self):
        super(IpTestParsers, self).__init__()

    def get_parsers(self):
        return {
            'ifconfig': self.__ifconfigParser,
            'ifup': self.__ifconfigParser,
            'ethup': self.__ifconfigParser,
            'dut1': self.__ifconfigParser,
            'dut2': self.__ifconfigParser,
            'dut3': self.__ifconfigParser,
            'dut4': self.__ifconfigParser,
            'dut5': self.__ifconfigParser,
            'dut6': self.__ifconfigParser,
            'socket': self.__mbedossocketParser,
            'ticker': self.__ticker_parser
        }

    # socket command for mbedos
    def __mbedossocketParser(self, response):
        results = {'socket_id': None,
                   'data_type': None,
                   'data': "",
                   'printed_bytes': None,
                   'sent_bytes': None,
                   'received_bytes': None,
                   'address': None,
                   'port': None,
                   'loop_id': None
                   }
        respLines = response.lines
        part = None
        for line in respLines:
            ret = PluginBase.find_one(line, ".*sid: ([0-9]+)")
            if ret is not False:
                results['socket_id'] = ret

            ret = PluginBase.find_one(line, ".*(hex|string) data, printing .* bytes:")
            if ret is not False:
                results['data_type'] = ret

            ret = PluginBase.find_one(line, ".*data, printing (.*) bytes:")
            if ret is not False:
                part = "data"

            ret = PluginBase.find_one(line, "^Printed (.*) bytes$")
            if ret is not False:
                results['printed_bytes'] = int(ret)
                part = None

            if part == "data":
                ret = PluginBase.find_one(line, "^\d{4}:  (.*)$")
                if ret is not False:
                    results['data'] = results['data'] + ret

            ret = PluginBase.find_one(line, ".*sent: ([0-9]+) bytes")
            if ret is not False:
                results['sent_bytes'] = int(ret)

            ret = PluginBase.find_one(line, ".*received: ([0-9]+) bytes")
            if ret is not False:
                results['received_bytes'] = int(ret)

            ret = PluginBase.find_one(line, ".*address: ([0-9a-fxA-F:.]+)")
            if ret is not False:
                results['address'] = ret

            ret = PluginBase.find_one(line, ".*port: ([0-9]+)")
            if ret is not False:
                results['port'] = ret

            ret = PluginBase.find_one(line, ".*lid: ([0-9]+)")
            if ret is not False:
                results['loop_id'] = ret

        return results

    # response parser for ifup
    def __ifconfigParser(self, response):
        results = {}
        lines = response.traces
        part = None

        results['address'] = {
            'll': '',
            'globals': [],
            'ipv4': None,
            'ipv6': []
        }

        for line in lines:
            # print "ifcfgparser: %s" % line
            match = re.search('IPv6 if addr', line)
            if match:
                part = "address"
            match = re.search('IPv4 if addr', line)
            if match:
                part = "address"
            match = re.search('MAC-48\:[\W]{1,}(.*)', line)
            if match:
                mac48 = match.group(1)
                # Validate the MAC-48 syntax as well
                match = re.search("([0-9a-fA-F]{2}:??){5}([0-9a-fA-F]{2})", mac48)
                if match:
                    results['MAC'] = mac48

            if part == "address":
                match = re.search('.*\[(\d)\]:\W([abcdefg\d\:]{5,})', line)
                if match:
                    addr = match.group(2)
                    if re.search('^fe80\:\:', addr):
                        results['address']['ll'] = addr
                    else:
                        results['address']["globals"].append(addr)

                match = re.search('\[(\d)\]:\W([a-fA-F\d\:]{5,})', line)
                if match:
                    results['address']['ipv6'].append(match.group(2))

                match = re.search('(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})$', line)
                if match:
                    results['address']['ipv4'] = match.group(1)

        return results

    def __ticker_parser(self, response):
        results = {}
        respLines = response.lines
        for line in respLines:
            ret = PluginBase.find_one(line, 'Ticker id: ([0-9]+)')
            if ret is not False:
                results['ticker_id'] = ret
        return results
