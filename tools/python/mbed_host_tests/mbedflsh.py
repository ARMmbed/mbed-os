"""
mbed SDK
Copyright (c) 2011-2016 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Przemyslaw Wirkus <Przemyslaw.Wirkus@arm.com>
"""

import sys
import optparse

### Flashing/Reset API provided by mbed--host-tests (mbedhtrun)
from mbed_os_tools.test import host_tests_plugins


def cmd_parser_setup():
    """! Creates simple command line parser
    """
    parser = optparse.OptionParser()

    parser.add_option('-f', '--file',
                      dest='filename',
                      help='File to flash onto mbed device')

    parser.add_option("-d", "--disk",
                      dest="disk",
                      help="Target disk (mount point) path. Example: F:, /mnt/MBED",
                      metavar="DISK_PATH")

    copy_methods_str = "Plugin support: " + ', '.join(host_tests_plugins.get_plugin_caps('CopyMethod'))

    parser.add_option("-c", "--copy",
                      dest="copy_method",
                      default='shell',
                      help="Copy (flash the target) method selector. " + copy_methods_str,
                      metavar="COPY_METHOD")

    parser.add_option('', '--plugins',
                      dest='list_plugins',
                      default=False,
                      action="store_true",
                      help='Prints registered plugins and exits')

    parser.add_option('', '--version',
                      dest='version',
                      default=False,
                      action="store_true",
                      help='Prints package version and exits')

    parser.description = """Flash mbed devices from command line.""" \
        """This module is using build in to mbed-host-tests plugins used for flashing mbed devices"""
    parser.epilog = """Example: mbedflsh -d E: -f /path/to/file.bin"""

    (opts, args) = parser.parse_args()
    return (opts, args)


def main():
    """! Function wrapping flashing (copy) plugin
    @details USers can use mbedflsh command to flash mbeds from command line
    """
    errorlevel_flag = 0
    (opts, args) = cmd_parser_setup()

    if opts.version:
        import pkg_resources  # part of setuptools
        version = pkg_resources.require("mbed-host-tests")[0].version
        print(version)
        sys.exit(0)
    elif opts.list_plugins:    # --plugins option
        host_tests_plugins.print_plugin_info()
        sys.exit(0)
    else:
        pass

    if opts.filename:
        print("mbedflsh: opening file %s..."% opts.filename)
        result = host_tests_plugins.call_plugin('CopyMethod',
            opts.copy_method,
            image_path=opts.filename,
            destination_disk=opts.disk)
        errorlevel_flag = result == True

    return errorlevel_flag


if __name__ == '__main__':
    exit(main())
