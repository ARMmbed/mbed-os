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


import sys
import logging
from functools import partial


class HtrunLogger(object):
    """! Yet another logger flavour """
    def __init__(self, name):
        logging.basicConfig(stream=sys.stdout,format='[%(created).2f][%(name)s]%(message)s', level=logging.DEBUG)
        self.logger = logging.getLogger(name)
        self.format_str = '[%(logger_level)s] %(message)s'

        def __prn_log(self, logger_level, text, timestamp=None):
            self.logger.debug(self.format_str% {
                'logger_level' : logger_level,
                'message' : text,
            })

        self.prn_dbg = partial(__prn_log, self, 'DBG')
        self.prn_wrn = partial(__prn_log, self, 'WRN')
        self.prn_err = partial(__prn_log, self, 'ERR')
        self.prn_inf = partial(__prn_log, self, 'INF')
        self.prn_txt = partial(__prn_log, self, 'TXT')
        self.prn_txd = partial(__prn_log, self, 'TXD')
        self.prn_rxd = partial(__prn_log, self, 'RXD')
