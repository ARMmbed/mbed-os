"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import os
import ctypes
import sys
from time import sleep
from os.path import join, basename
from host_test_plugins import HostTestPluginBase

class HostTestPluginCopyMethod_Sync(HostTestPluginBase):

    # Plugin interface
    name = 'HostTestPluginCopyMethod_Sync'
    type = 'CopyMethod'
    stable = True
    capabilities = ['sync']
    required_parameters = ['image_path', 'destination_disk']

    def setup(self, *args, **kwargs):
        """ Configure plugin, this function should be called before plugin execute() method is used.
        """
        return True

    def get_free_space_bytes(self, dirname):
        """ Return folder/drive free space (in bytes). """
        if sys.platform == 'win32':
            free_bytes = ctypes.c_ulonglong(0)

            # Disable Windows error box temporarily
            oldError = ctypes.windll.kernel32.SetErrorMode(1) #note that SEM_FAILCRITICALERRORS = 1

            try:
                ctypes.windll.kernel32.GetDiskFreeSpaceExW(ctypes.c_wchar_p(dirname), None, None, ctypes.pointer(free_bytes))
            except:
                return 0

            ctypes.windll.kernel32.SetErrorMode(oldError)

            return free_bytes.value
        else:
            try:
                st = os.statvfs(dirname)
            except:
                return 0

            return st.f_bavail

    def execute(self, capability, *args, **kwargs):
        """ Executes capability by name.
            Each capability may directly just call some command line
            program or execute building pythonic function
        """
        result = False
        if self.check_parameters(capability, *args, **kwargs) is True:
            image_path = kwargs['image_path']
            destination_disk = kwargs['destination_disk']

            can_print_disk_warning = True

            # Wait for mbed disk to be available for writing
            if self.get_free_space_bytes(destination_disk) <= 0:
                print 'MBED: Waiting for mbed disk to mount properly'

            while self.get_free_space_bytes(destination_disk) <= 0:
                pass

            # There are still issues with flashing the target to quickly after mounting
            # that I've had trouble tracking down. This sleep seems to help with stability for now.
            sleep(1)

            print 'MBED: mbed disk mounted properly'

            # Wait for mount point to be ready
            self.check_mount_point_ready(destination_disk)  # Blocking
            # Prepare correct command line parameter values
            image_base_name = basename(image_path)
            destination_path = join(destination_disk, image_base_name)

	    shell = False

            if sys.platform == 'win32':
                capability = 'copy'
            elif sys.platform == 'linux2' or sys.platform == 'cygwin':
                capability = 'dd'
            elif sys.platform == 'darwin':
                capability == 'ditto'

            if capability == 'copy':
                shell = True
                cmd = [capability, image_path, destination_path]
            elif capability == 'dd':
                cmd = [capability, 'if=' + image_path, 'of='+destination_path, 'conv=fsync']
            elif capability == 'ditto':
                cmd = [capability, '--nocache', image_path, destination_path]

            result = self.run_command(cmd, shell=shell)
        return result


def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginCopyMethod_Sync()
