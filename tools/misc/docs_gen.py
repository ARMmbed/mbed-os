"""An api for generating documentation from the codebase
"""

from os.path import dirname, join
from os import sep
from re import compile

import subprocess

def generate_documentation(dirs, output_dir):
    """Use doxygen to generate the documentation

    Positional arguments:
    dirs - the directories that doxygen should scan for documentation
    output_dir - location of the documentation after the return of this function
    """
    print dirs
    with open(join(dirname(__file__), "Doxyfile")) as doxyfile:
        proc = subprocess.Popen(["doxygen", "-"], stdin=subprocess.PIPE)
        proc.stdin.write(doxyfile.read())
        proc.stdin.write("OUTPUT_DIRECTORY={}\n".format(output_dir))
        proc.stdin.write("INPUT={}".format(" ".join(dirs)))
        proc.stdin.close()
        proc.wait()

EXCLUDES = ["targets", "features/FEATURE", "features/mbedtls",
           "features/nanostack", "features/storage"]

def is_not_excluded(src):
    return all(exclude not in src for exclude in EXCLUDES)

if __name__ == "__main__":
    import sys
    from os.path import abspath, dirname, join
    # Be sure that the tools directory is in the search path
    ROOT = abspath(join(dirname(__file__), "..", ".."))
    sys.path.insert(0, ROOT)

    from tools.toolchains.gcc import GCC_ARM
    from tools.targets import TARGET_MAP
    toolchain = GCC_ARM(TARGET_MAP["Super_Target"])
    resources = toolchain.scan_resources(".")
    generate_documentation(filter(is_not_excluded,
                                  sum(map(lambda x:x.headers,
                                          resources.features.values()),
                                      resources.headers)),
                           join(dirname(dirname(__file__)), "mbed-docs"))
