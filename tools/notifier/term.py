# mbed SDK
# Copyright (c) 2011-2013 ARM Limited
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

from __future__ import print_function, division, absolute_import

from os.path import basename

from . import Notifier
from ..settings import PRINT_COMPILER_OUTPUT_AS_LINK

class TerminalNotifier(Notifier):
    """
    Writes notifications to a terminal based on a silent and verbose flag.
    """

    def __init__(self, verbose=False, silent=False):
        self.verbose = verbose
        self.silent = silent
        self.output = ""

    def get_output(self):
        return self.output

    def notify(self, event):
        if self.verbose:
            msg = self.print_notify_verbose(event)
        else:
            msg = self.print_notify(event)
        if msg:
            if not self.silent:
                print(msg)
            self.output += msg + "\n"

    def print_notify(self, event):
        """ Default command line notification
        """
        if not self.verbose and event['type'] == 'tool_error':
            return event['message']

        elif event['type'] in ['info']:
            return event['message']

        elif event['type'] == 'cc':
            event['severity'] = event['severity'].title()

            if PRINT_COMPILER_OUTPUT_AS_LINK:
                event['file'] = getcwd() + event['file'].strip('.')
                return '[%(severity)s] %(file)s:%(line)s:%(col)s: %(message)s' % event
            else:
                event['file'] = basename(event['file'])
                return '[%(severity)s] %(file)s@%(line)s,%(col)s: %(message)s' % event

        elif event['type'] == 'progress':
            if 'percent' in event:
                return '{} [{:>5.1f}%]: {}'.format(event['action'].title(),
                                                  event['percent'],
                                                  basename(event['file']))
            else:
                return '{}: {}'.format(event['action'].title(),
                                      basename(event['file']))

    def print_notify_verbose(self, event):
        """ Default command line notification with more verbose mode
        """
        if event['type'] in ['info', 'debug']:
            return event['message']

        elif event['type'] == 'cc':
            event['severity'] = event['severity'].title()
            event['file'] = basename(event['file'])
            event['mcu_name'] = "None"
            event['target_name'] = event['target_name'].upper() if event['target_name'] else "Unknown"
            event['toolchain_name'] = event['toolchain_name'].upper() if event['toolchain_name'] else "Unknown"
            return '[%(severity)s] %(target_name)s::%(toolchain_name)s::%(file)s@%(line)s: %(message)s' % event

        elif event['type'] == 'progress':
            return self.print_notify(event) # standard handle
