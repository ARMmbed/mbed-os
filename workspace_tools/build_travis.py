# Travis-CI build script

import os
import sys

################################################################################
# Configure builds here
# "libs" can contain "dsp", "rtos", "eth", "usb_host", "usb", "ublox"

build_list = (
    { "target": "LPC1768",  "toolchains": "GCC_ARM", "libs": ["dsp", "rtos", "eth", "usb_host", "usb", "ublox"] },
    { "target": "KL25Z",    "toolchains": "GCC_ARM", "libs": ["dsp", "rtos", "usb"] },
    { "target": "LPC4088",  "toolchains": "GCC_ARM", "libs": ["dsp", "rtos", "usb"] },
)

################################################################################
# Driver

def run_builds(dry_run):
    for build in build_list:
        toolchain_list = build["toolchains"]
        if type(toolchain_list) != type([]): toolchain_list = [toolchain_list]
        for toolchain in toolchain_list:
            cmdline = "python workspace_tools/build.py -m %s -t %s -c " % (build["target"], toolchain)
            libs = build.get("libs", [])
            if libs:
                cmdline = cmdline + " ".join(["--" + l for l in libs])
            print "Executing: " + cmdline
            if not dry_run:
                if os.system(cmdline) != 0:
                    sys.exit(1)

if __name__ == "__main__":
    run_builds("-s" in sys.argv)
