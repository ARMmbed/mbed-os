from workspace_tools.targets import TARGETS

DEFAULT_SUPPORT = {}
CORTEX_ARM_SUPPORT = {}

for target in TARGETS:
    DEFAULT_SUPPORT[target.name] = target.supported_toolchains
    
    if target.core.startswith('Cortex'):
        CORTEX_ARM_SUPPORT[target.name] = [t for t in target.supported_toolchains
                                           if (t=='ARM' or t=='uARM')]
