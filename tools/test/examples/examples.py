""" import and bulid a bunch of example programs """

import os
from os.path import dirname, abspath, basename
import os.path
import sys
import subprocess
import json

ROOT = abspath(dirname(dirname(dirname(dirname(__file__)))))
sys.path.insert(0, ROOT)

from tools.build_api import get_mbed_official_release
from tools.targets import TARGET_MAP


EXAMPLES = json.load(open(os.path.join(os.path.dirname(__file__),
                                       "examples.json")))

def print_stuff(name, lst):
    if list:
        print("#"*80)
        print("# {} example combinations".format(name))
        print("#")
    for thing in lst:
        print(thing)


def main():
    """Entry point"""
    failures = []
    sucesses = []
    for example, build_features in EXAMPLES.iteritems():
        subprocess.call(["mbed-cli", "import", example])
        os.chdir(basename(example))
        for target, toolchains in [(target, toolchains) for target, toolchains
                                   in get_mbed_official_release("5") if
                                   all(feature in TARGET_MAP[target].features
                                       for feature in build_features)]:
            for toolchain in toolchains:
                proc = subprocess.Popen(["mbed-cli", "compile", "-t",
                                         toolchain, "-m", target])
                proc.wait()
                example_name = "{} {} {}".format(basename(example), target,
                                                 toolchain)
                if proc.returncode:
                    failures.append(example_name)
                else:
                    sucesses.append(example_name)
        os.chdir("..")
    print_stuff("Passed", sucesses)
    print_stuff("Failed", failures)
    return len(failures)

if __name__ == "__main__":
    sys.exit(main())
