""" import and bulid a bunch of example programs """

import os
import os.path
import sys
import subprocess
import json


EXAMPLES = json.load(open(os.path.join(os.path.dirname(__file__),
                                       "examples.json")))

BUILD_TOOLCHAINS = [
    "ARM",
    "GCC_ARM",
    "IAR",
]

def main():
    """Entry point"""
    failures = []
    for example, build_targets in EXAMPLES.iteritems():
        subprocess.call(["mbed-cli", "import", example])
        os.chdir(os.path.basename(example))
        for toolchain in BUILD_TOOLCHAINS:
            for target in build_targets:
                proc = subprocess.Popen(["mbed-cli", "compile", "-t",
                                         toolchain, "-m", target])
                proc.wait()
                if proc.returncode:
                    failures.append("{} {} {}".format(example, target,
                                                      toolchain))
        os.chdir("..")
    if failures:
        print("#"*80)
        print("# Failed example combinations")
        print("#")
    for fail in failures:
        print(fail)
    return len(failures)

if __name__ == "__main__":
    sys.exit(main())
