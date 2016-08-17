# mbed SDK
# Copyright (c) 2016 ARM Limited
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

""" This python file is responsible for generating colorized notifiers.
"""

import sys
import re
from colorama import init, Fore, Back, Style
init()

COLORS = {
    'none' : "",
    'default' : Style.RESET_ALL,

    'black'   : Fore.BLACK,
    'red'     : Fore.RED,
    'green'   : Fore.GREEN,
    'yellow'  : Fore.YELLOW,
    'blue'    : Fore.BLUE,
    'magenta' : Fore.MAGENTA,
    'cyan'    : Fore.CYAN,
    'white'   : Fore.WHITE,

    'on_black'   : Back.BLACK,
    'on_red'     : Back.RED,
    'on_green'   : Back.GREEN,
    'on_yellow'  : Back.YELLOW,
    'on_blue'    : Back.BLUE,
    'on_magenta' : Back.MAGENTA,
    'on_cyan'    : Back.CYAN,
    'on_white'   : Back.WHITE,
}

COLOR_MATCHER = re.compile(r"(\w+)(\W+on\W+\w+)?")
def colorstring_to_escapecode(color_string):
    """ Convert a color string from a string into an ascii escape code that
    will print that color on the terminal.

    Positional arguments:
    color_string - the string to parse
    """
    match = re.match(COLOR_MATCHER, color_string)
    if match:
        return COLORS[match.group(1)] + \
            (COLORS[match.group(2).strip().replace(" ", "_")]
             if match.group(2) else "")
    else:
        return COLORS['default']


def print_in_color_notifier(color_map, print_fn):
    """ Wrap a toolchain notifier in a colorizer. This colorizer will wrap
    notifications in a color if the severity matches a color in the *color_map*.
    """
    def wrap(event, silent=False):
        """The notification function inself"""
        file_desc = sys.stdout
        self = event['toolchain']
        if file_desc.isatty() and 'severity' in event and \
           event['severity'] in color_map:
            file_desc.write(colorstring_to_escapecode(
                color_map[event['severity']]))
            print_fn(self, event, silent)
            file_desc.write(colorstring_to_escapecode('default'))
        else:
            print_fn(self, event, silent)
    return wrap
