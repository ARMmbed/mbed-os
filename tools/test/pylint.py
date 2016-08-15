"""A test that all code scores above an 8.5 in pylint"""

import subprocess
import re
import os.path
import sys

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
        stderr=subprocess.PIPE
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
        if parse_score(stdout) < 8.5:
            print(stdout)
            sys.exit(1)



