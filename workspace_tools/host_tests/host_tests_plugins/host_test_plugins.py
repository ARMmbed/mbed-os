# Interfaces and utils for host test plugin architecture


class HostTestPluginBase:
    """ Base class for all plug-ins used with host tests.
    """

    ###########################################################################
    # Interface
    ###########################################################################

    ###########################################################################
    # Attributes defining plugin name, type etc.
    ###########################################################################
    name = "HostTestPluginBase" # Plugin name, can be plugin class name 
    type = "BasePlugin" # Plugin type: ResetMethod, Copymethod etc.
    capabilities = []   # Capabilities names: what plugin can achieve (e.g. reset using some external command line tool)

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
