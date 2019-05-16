"""
Copyright (c) 2016-2019 ARM Limited. All rights reserved.

SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations 
"""

import subprocess
import re
import os.path

"""A test that all code scores above a 9.25 in pylint"""

SCORE_REGEXP = re.compile(
    r'^Your\ code\ has\ been\ rated\ at\ (\-?[0-9\.]+)/10')

TOOLS_ROOT = os.path.dirname(os.path.dirname(__file__))


def parse_score(pylint_output):
    """Parse the score out of pylint's output as a float If the score is not
    found, return 0.0.
    """
    for line in pylint_output.splitlines():
        match = re.match(SCORE_REGEXP, line)
        if match:
            return float(match.group(1))
    return 0.0

def execute_pylint(filename):
    """Execute a pylint process and collect it's output
    """
    process = subprocess.Popen(
        ["pylint", filename],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        universal_newlines=True
    )
    stout, sterr = process.communicate()
    status = process.poll()
    return status, stout, sterr

FILES = ["build_api.py", "config.py", "colorize.py", "detect_targets.py",
         "hooks.py", "libraries.py", "memap.py", "options.py", "paths.py",
         "targets.py", "test/pylint.py"]

if __name__ == "__main__":
    for python_module in FILES:
        _, stdout, stderr = execute_pylint(os.path.join(TOOLS_ROOT,
                                                        python_module))
        score = parse_score(stdout)
        if score < 9.25:
            print(stdout)



