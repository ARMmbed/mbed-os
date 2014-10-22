# Interfaces and utils for host test plugin architecture


def construct_enum(**enums):
    """ Create your own pseudo-enums
    """
    return type('Enum', (), enums)


class HostTestPluginBase:
    """ Base class for all plug-ins used with host tests.
    """
    def register(self, *args, **kwargs):
        pass

    def unregister(self):
        pass

    # Attributes defining plugin name, type etc.
    # This values across plugin should be unique
    name = "HostTestPluginBase"
    type = "BasePlugin"
    capabilities = []

