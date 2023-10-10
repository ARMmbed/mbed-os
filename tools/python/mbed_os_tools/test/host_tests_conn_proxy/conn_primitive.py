# Copyright (c) 2018, Arm Limited and affiliates.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from ..host_tests_logger import HtrunLogger


class ConnectorPrimitiveException(Exception):
    """
    Exception in connector primitive module.
    """
    pass


class ConnectorPrimitive(object):

    def __init__(self, name):
        self.LAST_ERROR = None
        self.logger = HtrunLogger(name)
        self.polling_timeout = 60

    def write_kv(self, key, value):
        """! Forms and sends Key-Value protocol message.
        @details On how to parse K-V sent from DUT see KiViBufferWalker::KIVI_REGEX
                 On how DUT sends K-V please see greentea_write_postamble() function in greentea-client
        @return Returns buffer with K-V message sent to DUT on success, None on failure
        """
        # All Key-Value messages ends with newline character
        kv_buff = "{{%s;%s}}"% (key, value) + '\n'

        if self.write(kv_buff):
            self.logger.prn_txd(kv_buff.rstrip())
            return kv_buff
        else:
            return None

    def read(self, count):
        """! Read data from DUT
        @param count Number of bytes to read
        @return Bytes read
        """
        raise NotImplementedError

    def write(self, payload, log=False):
        """! Read data from DUT
        @param payload Buffer with data to send
        @param log Set to True if you want to enable logging for this function
        @return Payload (what was actually sent - if possible to establish that)
        """
        raise NotImplementedError

    def flush(self):
        """! Flush read/write channels of DUT """
        raise NotImplementedError

    def reset(self):
        """! Reset the dut
        """
        raise NotImplementedError

    def connected(self):
        """! Check if there is a connection to DUT
        @return True if there is conenction to DUT (read/write/flush API works)
        """
        raise NotImplementedError

    def error(self):
        """! Returns LAST_ERROR value
        @return Value of self.LAST_ERROR
        """
        return self.LAST_ERROR

    def finish(self):
        """! Handle DUT dtor like (close resource) operations here
        """
        raise NotImplementedError
