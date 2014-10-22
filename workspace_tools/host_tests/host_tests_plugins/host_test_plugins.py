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

class HostTestPluginBase:
    """ Base class for all plug-ins used with host tests.
    """
    ###########################################################################
    # Interface:
    ###########################################################################

    ###########################################################################
    # Interface attributes defining plugin name, type etc.
    ###########################################################################
    name = "HostTestPluginBase" # Plugin name, can be plugin class name
    type = "BasePlugin" # Plugin type: ResetMethod, Copymethod etc.
    capabilities = []   # Capabilities names: what plugin can achieve (e.g. reset using some external command line tool)

    ###########################################################################
    # Interface methods
    ###########################################################################
    def setup(self, *args, **kwargs):
        """ Configure plugin, this function should be called before plugin execute() method is used.
        """
        pass

    def execute(self, capabilitity, *args, **kwargs):
        """ Executes capability by name.
            Each capability e.g. may directly just call some command line
            program or execute building pythonic function
        """
        return False
