"""
mbed SDK
Copyright (c) 2016 ARM Limited

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

""" This python file is responsible for generating colorized notifiers.
"""

import sys
import re
from colorama import init
init()

colors = {
    'none' : "",
    'default' : "\033[.0m",
    'bold' : "\033[.1m",
    'underline' : "\033[.4m",
    'blink' : "\033[.5m",
    'reverse' : "\033[.7m",
    'concealed' : "\033[.8m",

    'black' : "\033[.30m",
    'red' : "\033[.31m",
    'green' : "\033[.32m",
    'yellow' : "\033[.33m",
    'blue' : "\033[.34m",
    'magenta' : "\033[.35m",
    'cyan' : "\033[.36m",
    'white' : "\033[.37m",

    'on_black' : "\033[.40m",
    'on_red' : "\033[.41m",
    'on_green' : "\033[.42m",
    'on_yellow' : "\033[.43m",
    'on_blue' : "\033[.44m",
    'on_magenta' : "\033[.45m",
    'on_cyan' : "\033[46m",
    'on_white' : "\033[47m",
}

# Convert a color string from a string into an ascii escape code that will print
# that color on the terminal.
color_matcher = re.compile(r"(\w+)(\W+on\W+\w+)?")
def colorstring_to_escapecode(color_string):
    match = re.match(color_matcher, color_string)
    if match:
        return colors[match.group(1)] + (colors[match.group(2).strip().replace(" ","_")] if match.group(2) else "")
    else:
        return corols['default']

# Wrap a toolchain notifier in a colorizer. This colorizer will wrap notifications
# in a color if the severity matches a color in the *color_map*.
def print_in_color_notifier (color_map, print_fn):
    def wrap(event, silent=False):
        fd = sys.stdout
        self = event['toolchain']
        if fd.isatty() and 'severity' in event and event['severity'] in color_map:
            fd.write(colorstring_to_escapecode(color_map[event['severity']]))
            print_fn(self, event, silent)
            fd.write(colorstring_to_escapecode('default'))
        else:
            print_fn(self, event, silent)
    return wrap
