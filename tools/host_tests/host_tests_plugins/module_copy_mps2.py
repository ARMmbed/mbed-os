"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited
SPDX-License-Identifier: Apache-2.0

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
from __future__ import print_function

import re
import os, shutil
from os.path import join
from time import sleep
from .host_test_plugins import HostTestPluginBase


class HostTestPluginCopyMethod_MPS2(HostTestPluginBase):

    # MPS2 specific flashing / binary setup funcitons
    def mps2_set_board_image_file(self, disk, images_cfg_path, image0file_path, image_name='images.txt'):
        """ This function will alter image cfg file.
            Main goal of this function is to change number of images to 1, comment all
            existing image entries and append at the end of file new entry with test path.
            @return True when all steps succeed.
        """
        MBED_SDK_TEST_STAMP = 'test suite entry'
        image_path = join(disk, images_cfg_path, image_name)
        new_file_lines = [] # New configuration file lines (entries)

        # Check each line of the image configuration file
        try:
            with open(image_path, 'r') as file:
                for line in file:
                    if re.search('^TOTALIMAGES', line):
                        # Check number of total images, should be 1
                        new_file_lines.append(re.sub('^TOTALIMAGES:[\t ]*[\d]+', 'TOTALIMAGES: 1', line))
                    elif re.search('; - %s[\n\r]*$'% MBED_SDK_TEST_STAMP, line):
                        # Look for test suite entries and remove them
                        pass    # Omit all test suite entries
                    elif re.search('^IMAGE[\d]+FILE', line):
                        # Check all image entries and mark the ';'
                        new_file_lines.append(';' + line)   # Comment non test suite lines
                    else:
                        # Append line to new file
                        new_file_lines.append(line)
        except IOError as e:
            return False

        # Add new image entry with proper commented stamp
        new_file_lines.append('IMAGE0FILE: %s    ; - %s\r\n'% (image0file_path, MBED_SDK_TEST_STAMP))

        # Write all lines to file
        try:
            with open(image_path, 'w') as file:
                for line in new_file_lines:
                    file.write(line),
        except IOError:
            return False

        return True

    def mps2_select_core(self, disk, mobo_config_name=""):
        """ Function selects actual core
        """
        # TODO: implement core selection
        pass

    def mps2_switch_usb_auto_mounting_after_restart(self, disk, usb_config_name=""):
        """ Function alters configuration to allow USB MSD to be mounted after restarts
        """
        # TODO: implement USB MSD restart detection
        pass

    def copy_file(self, file, disk):
        if not file:
            return
            
        _, ext = os.path.splitext(file)
        ext = ext.lower()
        dfile = disk + "/SOFTWARE/mbed" + ext
        
        if os.path.isfile(dfile):
            print('Remove old binary %s' % dfile)
            os.remove(dfile)

        shutil.copy(file, dfile)
        return True
        
    def touch_file(self, file):
        """ Touch file and set timestamp to items
        """
        tfile = file+'.tmp'
        fhandle = open(tfile, 'a')
        try:
            fhandle.close()
        finally:
            os.rename(tfile, file)
            return True

    # Plugin interface
    name = 'HostTestPluginCopyMethod_MPS2'
    type = 'CopyMethod'
    capabilities = ['mps2-copy']
    required_parameters = ['image_path', 'destination_disk']

    def setup(self, *args, **kwargs):
        """ Configure plugin, this function should be called before plugin execute() method is used.
        """
        return True

    def execute(self, capabilitity, *args, **kwargs):
        """ Executes capability by name.
            Each capability may directly just call some command line
            program or execute building pythonic function
        """
        result = False
        if self.check_parameters(capabilitity, *args, **kwargs) is True:
            file = kwargs['image_path']
            disk = kwargs['destination_disk']
            
            """ Add a delay in case there a test just finished
                Prevents interface firmware hiccups
            """
            sleep(20)
            if capabilitity == 'mps2-copy' and self.copy_file(file, disk):
                sleep(3)
                if self.touch_file(disk + 'reboot.txt'):
                    """ Add a delay after the board was rebooted.
                        The actual reboot time is 20 seconds, but using 15 seconds
                        allows us to open the COM port and save a board reset.
                        This also prevents interface firmware hiccups.
                    """
                    sleep(7)
                    result = True
                    
        return result


def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginCopyMethod_MPS2()
