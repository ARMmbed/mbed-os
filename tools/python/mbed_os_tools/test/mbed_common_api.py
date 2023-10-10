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

from subprocess import call, Popen, PIPE


def run_cli_command(cmd, shell=True, verbose=False):
    """! Runs command from command line
    @param shell Shell command (e.g. ls, ps)
    @param verbose Verbose mode flag
    @return Returns (True, 0) if command was executed successfully else return (False, error code)
    """
    result = True
    ret = 0
    try:
        ret = call(cmd, shell=shell)
        if ret:
            result = False
            if verbose:
                print("mbedgt: [ret=%d] Command: %s"% (int(ret), cmd))
    except OSError as e:
        result = False
        if verbose:
            print("mbedgt: [ret=%d] Command: %s"% (int(ret), cmd))
            print(str(e))
    return (result, ret)

def run_cli_process(cmd):
    """! Runs command as a process and return stdout, stderr and ret code
    @param cmd Command to execute
    @return Tuple of (stdout, stderr, returncode)
    """
    try:
        p = Popen(cmd, stdout=PIPE, stderr=PIPE)
        _stdout, _stderr = p.communicate()
    except OSError as e:
        print("mbedgt: Command: %s"% (cmd))
        print(str(e))
        print("mbedgt: traceback...")
        print(e.child_traceback)
        return str(), str(), -1
    return _stdout, _stderr, p.returncode
