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
import sys
import plistlib
from os.path import join, basename
from subprocess import Popen, PIPE
from time import sleep
from host_test_plugins import HostTestPluginBase


class HostTestPluginCopyMethod_Remount(HostTestPluginBase):

    # Plugin interface
    name = 'HostTestPluginCopyMethod_Remount'
    type = 'CopyMethod'
    stable = True
    required_parameters = ['image_path', 'destination_disk']
    capabilities = ['remount']

    def setup(self, *args, **kwargs):
        """ Configure plugin, this function should be called before plugin execute() method is used.
        """
        return True

    def execute(self, capability, *args, **kwargs):
        """ Executes capability by name.
            Each capability may directly just call some command line
            program or execute building pythonic function
        """
        result = False
        if self.check_parameters(capability, *args, **kwargs) is True:
            image_path = kwargs['image_path']
            destination_disk = kwargs['destination_disk']
            # Wait for mount point to be ready
            self.check_mount_point_ready(destination_disk)  # Blocking
            # Prepare correct command line parameter values
            image_base_name = basename(image_path)
            destination_path = join(destination_disk, image_base_name)
	
            shell = False
            
            # Remount as a synchronous file system
            if sys.platform == 'linux2':
                device = ""

                while not device:
                    p1 = Popen('df %s --output=source | sed -n 2p' % destination_disk, shell=True, stdout=PIPE)
                    device = p1.communicate()[0].strip()
                    sleep(1)
                
                if self.run_command('sudo umount %s' %  (destination_disk), shell=True):
                    self.print_plugin_info('UNMOUNT OK')
                else:
                    self.print_plugin_info('UNMOUNT FAIL')
                
                sleep(1)

                if self.run_command('sudo mkdir -p %s' % (destination_disk), shell=True):
                    self.print_plugin_info('MKDIR OK')
                else :
                    self.print_plugin_info('MKDIR FAIL')

                sleep(1)

                if self.run_command('sudo mount -w -o uid=%s,gid=%s %s %s' % (os.geteuid(), os.getgid(), device, destination_disk), shell=True):
                    self.print_plugin_info('MOUNT OK')
                else:
                    self.print_plugin_info('MOUNT FAIL')

                sleep(1)

	        cmd = ['dd', 'if=%s' % image_path, 'of=%s' % (destination_path), 'conv=fsync']
	    elif sys.platform == 'darwin':
                p1 = Popen('diskutil info -plist \'%s\'' % (destination_disk), shell=True, stdout=PIPE)
                diskutil_output = p1.communicate()[0]
		plist = plistlib.readPlistFromString(diskutil_output)
		device_node = plist['DeviceNode']

		if self.run_command('diskutil unmount \'%s\'' % (destination_disk), shell=True):
                    self.print_plugin_info('UNMOUNT OK')
                else:
                    self.print_plugin_info('UNMOUNT FAIL')

                if self.run_command('sudo mkdir -p \'%s\'' % (destination_disk), shell=True):
                    self.print_plugin_info('MKDIR OK')
                else :
                    self.print_plugin_info('MKDIR FAIL')

                if self.run_command('diskutil mount -mountPoint \'%s\' \'%s\'' % (destination_disk, device_node), shell=True):
                    self.print_plugin_info('MOUNT OK')
                else:
                    self.print_plugin_info('MOUNT FAIL')

	        cmd = ['ditto', '--nocache', image_path, destination_path]

            result = self.run_command(cmd, shell=shell)

            sleep(3)
        
        return result

def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginCopyMethod_Remount()
