# Registry storing plugins

"""
module_example.py:

def plugin_main(*args, **kwargs):
    print args, kwargs

loader.py:

def load_plugin(name):
    mod = __import__("module_%s" % name)
    return mod

def call_plugin(name, *args, **kwargs):
    plugin = load_plugin(name)
    plugin.plugin_main(*args, **kwargs)

call_plugin("example", 1234)
"""


class HostTestRegistry:
    """ Simple class used to register and store
        host test plugins for further usage
    """
    # Here we actually store all the plugins
    PLUGINS = {}

    def print_error(self, text):
        print "Plugin load failed. Reason: %s"% text

    def register_plugin(self, plugin):
        """ Registers and stores plugin inside registry for further use.
            Method also calls plugin's setup() function to configure plugin if needed.

            Note: Different groups of plugins may demand different extra parameter. Plugins
            should be at least for one type of plugin configured with the same parameters
            because we do not know which of them will actually use particular parameter.
        """
        # TODO:
        # - check for unique caps for specified type
        if plugin.name not in self.PLUGINS:
            plugin.setup() # Setup plugin
            self.PLUGINS[plugin.name] = plugin
            return True
        self.print_error("%s already loaded"% plugin.name)
        return False

    def call_plugin(self, type, capability, *args, **kwargs):
        """ Execute plugin functionality respectively to its purpose
        """
        for plugin_name in self.PLUGINS:
            plugin = self.PLUGINS[plugin_name]
            if plugin.type == type and capability in plugin.capabilities:
                return plugin.execute(capability, *args, **kwargs)
        return False

    def load_plugin(self.name):
        """ Used to load module from
        """
        mod = __import__("module_%s"% name)
        return mod

    def __str__(self):
        """ User friendly printing method to show hooked plugins
        """
        from prettytable import PrettyTable
        column_names = ['name', 'type', 'capabilities']
        pt = PrettyTable(column_names)
        for column in column_names:
            pt.align[column] =  'l'
        for plugin_name in self.PLUGINS:
            name  = self.PLUGINS[plugin_name].name
            type  = self.PLUGINS[plugin_name].type
            capabilities  = ', '.join(self.PLUGINS[plugin_name].capabilities)
            row = [name, type, capabilities]
            pt.add_row(row)
        return pt.get_string()
