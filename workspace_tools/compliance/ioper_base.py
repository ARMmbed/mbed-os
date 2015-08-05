"""
mbed SDK
Copyright (c) 2011-2015 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Przemyslaw Wirkus <Przemyslaw.Wirkus@arm.com>

"""

import sys

try:
    from colorama import Fore
except:
    pass

COLORAMA = 'colorama' in sys.modules


class IOperTestCaseBase():
    """ Interoperability test case base class
        @return list of tuple (severity, Description)
                Example:      (result.append((IOperTestSeverity.INFO, ""))
    """

    def __init__(self, scope=None):
        self.PASS = 'PASS'
        self.INFO = 'INFO'
        self.ERROR = 'ERROR'
        self.WARN = 'WARN'

        self.scope = scope   # Default test scope (basic, pedantic, mbed-enabled etc...)

    def test(self, param=None):
        result = []
        return result

    def RED(self, text):
        return self.color_text(text, color=Fore.RED, delim=Fore.RESET) if COLORAMA else text

    def GREEN(self, text):
        return self.color_text(text, color=Fore.GREEN, delim=Fore.RESET) if COLORAMA else text

    def YELLOW(self, text):
        return self.color_text(text, color=Fore.YELLOW, delim=Fore.RESET) if COLORAMA else text

    def color_text(self, text, color='', delim=''):
        return color + text + color + delim

    def COLOR(self, severity, text):
        colors = {
            self.PASS : self.GREEN,
            self.ERROR : self.RED,
            self.WARN : self.YELLOW
        }
        if severity in colors:
            return colors[severity](text)
        return text
