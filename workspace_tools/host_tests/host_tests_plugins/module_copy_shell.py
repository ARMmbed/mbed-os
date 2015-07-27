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
from os.path import join, basename
from host_test_plugins import HostTestPluginBase


class HostTestPluginCopyMethod_Shell(HostTestPluginBase):

    # Plugin interface
    name = 'HostTestPluginCopyMethod_Shell'
    type = 'CopyMethod'
    stable = True
    capabilities = ['shell', 'cp', 'copy', 'xcopy']
    required_parameters = ['image_path', 'destination_disk']

    def setup(self, *args, **kwargs):
        """ Configure plugin, this function should be called before plugin execute() method is used.
        """
        return True

    def execute(self, capability, *args, **kwargs):
        """ Executes capability by name.
            Each capability may directly just call some command line
            program or execute building pythonic function
        """
	print 'running copy'
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

            if capability == 'shell':
                if sys.platform == 'win32': capability = 'copy'
                elif sys.platform == 'linux2' or sys.platform == 'cygwin': capability = 'dd'
                elif sys.platform == 'darwin': capability == 'ditto'

            if capability == 'copy' or capability == 'xcopy':
                cmd = [capability, image_path, destination_path]
            elif capability == 'dd':
                cmd = [capability, 'if=' + image_path, 'of='+destination_path, 'conv=fsync']
		print 'CMD: '
		print cmd
            elif capability == 'ditto':
                cmd = [capability, '--nocache', image_path, destination_path]

            result = self.run_command(cmd, shell=shell)
	    print 'FINISHED'
        return result


def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginCopyMethod_Shell()
