from optparse import OptionParser

from workspace_tools.toolchains import *

# Targets and Toolchains

TARGET_OPTIONS = {
    "ARM7": "LPC2368",
    "M3": "LPC1768",
    "M0": "LPC11U24",
    "M0+": "KL25Z",
}
TARGETS = TARGET_OPTIONS.values()

def get_default_options_parser():
    parser = OptionParser()
    
    parser.add_option("-m", "--mcu", dest="mcu",
                  help="build for the given MCU (%s)" % ', '.join(TARGET_OPTIONS.keys()),
                  metavar="MCU")
    
    parser.add_option("-t", "--tool", dest="tool",
                  help="build using the given TOOLCHAIN (%s)" % ', '.join(TOOLCHAINS),
                  metavar="TOOLCHAIN")
    
    parser.add_option("-c", "--clean",
                  action="store_true", dest="clean", default=False,
                  help="clean the build directory")
    
    return parser
