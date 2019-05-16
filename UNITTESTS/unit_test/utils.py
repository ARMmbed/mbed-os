"""
Copyright (c) 2018, Arm Limited
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


UNIT TEST UTILITIES
"""

import platform
import subprocess
import logging
import sys

def is_tool(name):
    """
    Test if tool is found in PATH

    @param name: executable name
    @return: true if tool found, false otherwise
    """
    cmd = "where" if platform.system() == "Windows" else "which"
    try:
        subprocess.check_output([cmd, name], stderr=subprocess.STDOUT)
        return True
    except subprocess.CalledProcessError:
        return False

def execute_program(args, error_msg="An error occurred!", success_msg=None):
    """
    Execute program in a subprocess with given arguments

    @param args: program and its arguments in a list
    @param success_msg: message to show in case of success
    @param error_msg: message to show in case of failure
    """

    try:
        process = subprocess.Popen(args,
                                   stdout=subprocess.PIPE,
                                   stderr=subprocess.STDOUT)

        # Output is stripped to remove newline character. logging adds its own
        # so we avoid double newlines.
        # Because the process can terminate before the loop has read all lines,
        # we read the output remnant just in case. Otherwise we lose it.
        while process.poll() is None:
            logging.info(process.stdout.readline().decode('utf8').rstrip('\n'))
        logging.info(process.stdout.read().decode('utf8').rstrip('\n'))

        retcode = process.wait()

        if retcode:
            raise subprocess.CalledProcessError(retcode, args)
        elif success_msg:
            logging.info(success_msg)

    except subprocess.CalledProcessError as error:
        logging.error(error_msg)
        sys.exit(error.returncode)
