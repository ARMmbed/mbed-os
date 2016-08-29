""" import and bulid a bunch of example programs """

import os
import os.path
import subprocess


EXAMPLES = [
    "https://developer.mbed.org/teams/mbed/code/mbed_blinky"
]

BUILD_TOOLCHAINS = [
    "ARM",
    "GCC_ARM",
    "IAR",
]

BUILD_TARGETS = [
    "K64F"
]

def main():
    """Entry point"""
    for example in EXAMPLES:
        subprocess.call(["mbed-cli", "import", example])
        os.chdir(os.path.basename(example))
        for toolchain in BUILD_TOOLCHAINS:
            for target in BUILD_TARGETS:
                subprocess.Popen(["mbed-cli", "compile", "-t", toolchain, "-m",
                                  target]).wait()
        os.chdir("..")

if __name__ == "__main__":
    main()
