from optparse import OptionParser

from workspace_tools.toolchains import *

# Targets and Toolchains

TARGETS = (
    "LPC2368",
    "LPC1768",
    "LPC11U24",
    "KL25Z",
    "LPC812"
)

def get_default_options_parser():
    parser = OptionParser()
    
    parser.add_option("-m", "--mcu", dest="mcu",
                  help="build for the given MCU (%s)" % ', '.join(TARGETS),
                  metavar="MCU")
    
    parser.add_option("-t", "--tool", dest="tool",
                  help="build using the given TOOLCHAIN (%s)" % ', '.join(TOOLCHAINS),
                  metavar="TOOLCHAIN")
    
    parser.add_option("-c", "--clean",
                  action="store_true", dest="clean", default=False,
                  help="clean the build directory")
    
    return parser
