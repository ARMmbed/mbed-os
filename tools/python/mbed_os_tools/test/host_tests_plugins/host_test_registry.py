# Copyright (c) 2018, Arm Limited and affiliates.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

class HostTestRegistry:
    """ Simple class used to register and store
        host test plugins for further usage
    """
    # Here we actually store all the plugins
    PLUGINS = {}    # 'Plugin Name' : Plugin Object

    def print_error(self, text):
        """! Prints error directly on console

        @param text Error message text message
        """
        print("Plugin load failed. Reason: %s"% text)

    def register_plugin(self, plugin):
        """! Registers and stores plugin inside registry for further use.

        @param plugin Plugin name

        @return True if plugin setup was successful and plugin can be registered, else False

        @details Method also calls plugin's setup() function to configure plugin if needed.
                 Note: Different groups of plugins may demand different extra parameter. Plugins
                 should be at least for one type of plugin configured with the same parameters
                 because we do not know which of them will actually use particular parameter.
        """
        # TODO:
        # - check for unique caps for specified type
        if plugin.name not in self.PLUGINS:
            if plugin.setup(): # Setup plugin can be completed without errors
                self.PLUGINS[plugin.name] = plugin
                return True
            else:
                self.print_error("%s setup failed"% plugin.name)
        else:
            self.print_error("%s already loaded"% plugin.name)
        return False

    def call_plugin(self, type, capability, *args, **kwargs):
        """! Execute plugin functionality respectively to its purpose
        @param type Plugin type
        @param capability Plugin capability name
        @param args Additional plugin parameters
        @param kwargs Additional plugin parameters
        @return Returns result from plugin's execute() method
        """
        for plugin_name in self.PLUGINS:
            plugin = self.PLUGINS[plugin_name]
            if plugin.type == type and capability in plugin.capabilities:
                return plugin.execute(capability, *args, **kwargs)
        return False

    def get_plugin_caps(self, type):
        """! Returns list of all capabilities for plugin family with the same type
        @param type Plugin type
        @return Returns list of capabilities for plugin. If there are no capabilities empty list is returned
        """
        result = []
        for plugin_name in self.PLUGINS:
            plugin = self.PLUGINS[plugin_name]
            if plugin.type == type:
                result.extend(plugin.capabilities)
        return sorted(result)

    def load_plugin(self, name):
        """! Used to load module from system (by import)
        @param name name of the module to import
        @return Returns result of __import__ operation
        """
        mod = __import__("module_%s"% name)
        return mod

    def get_string(self):
        """! User friendly printing method to show hooked plugins
        @return Returns string formatted with PrettyTable
        """
        from prettytable import PrettyTable, HEADER
        column_names = ['name', 'type', 'capabilities', 'stable', 'os_support', 'required_parameters']
        pt = PrettyTable(column_names, junction_char="|", hrules=HEADER)
        for column in column_names:
            pt.align[column] = 'l'
        for plugin_name in sorted(self.PLUGINS.keys()):
            name = self.PLUGINS[plugin_name].name
            type = self.PLUGINS[plugin_name].type
            stable = self.PLUGINS[plugin_name].stable
            capabilities  = ', '.join(self.PLUGINS[plugin_name].capabilities)
            is_os_supported  = self.PLUGINS[plugin_name].is_os_supported()
            required_parameters = ', '.join(self.PLUGINS[plugin_name].required_parameters)
            row = [name, type, capabilities, stable, is_os_supported, required_parameters]
            pt.add_row(row)
        return pt.get_string()

    def get_dict(self):
        column_names = ['name', 'type', 'capabilities', 'stable']
        result = {}
        for plugin_name in sorted(self.PLUGINS.keys()):
            name = self.PLUGINS[plugin_name].name
            type = self.PLUGINS[plugin_name].type
            stable = self.PLUGINS[plugin_name].stable
            capabilities = self.PLUGINS[plugin_name].capabilities
            is_os_supported = self.PLUGINS[plugin_name].is_os_supported()
            required_parameters = self.PLUGINS[plugin_name].required_parameters
            result[plugin_name] = {
                "name" : name,
                "type" : type,
                "stable" : stable,
                "capabilities" : capabilities,
                "os_support" : is_os_supported,
                "required_parameters" : required_parameters
            }
        return result

    def __str__(self):
        return self.get_string()
