# Configurable hooks in the build system. Can be used by various platforms
# to customize the build process.

################################################################################
# Hooks for the various parts of the build process

# Internal mapping of hooks per tool
_hooks = {}

# Internal mapping of running hooks
_running_hooks = {}

# Available hook types
_hook_types = ["binary"]

# Available hook steps
_hook_steps = ["pre", "replace", "post"]

# Hook the given function. Use this function as a decorator
def hook_tool(function):
    tool = function.__name__
    tool_flag = "_" + tool + "_done"
    def wrapper(t_self, *args, **kwargs):
        # if a hook for this tool is already running, it's most likely
        # coming from a derived class, so don't hook the super class version
        if _running_hooks.get(tool, False):
            return function(t_self, *args, **kwargs)
        _running_hooks[tool] = True
        # If this tool isn't hooked, return original function
        if not _hooks.has_key(tool):
            res = function(t_self, *args, **kwargs)
            _running_hooks[tool] = False
            return res
        tooldesc = _hooks[tool]
        setattr(t_self, tool_flag, False)
        # If there is a replace hook, execute the replacement instead
        if tooldesc.has_key("replace"):
            res = tooldesc["replace"](t_self, *args, **kwargs)
        # If the replacement has set the "done" flag, exit now
        # Otherwise continue as usual
        if getattr(t_self, tool_flag, False):
            _running_hooks[tool] = False
            return res
        # Execute pre-function before main function if specified
        if tooldesc.has_key("pre"):
            tooldesc["pre"](t_self, *args, **kwargs)
        # Execute the main function now
        res = function(t_self, *args, **kwargs)
        # Execute post-function after main function if specified
        if tooldesc.has_key("post"):
            post_res = tooldesc["post"](t_self, *args, **kwargs)
            _running_hooks[tool] = False
            return post_res or res
        else:
            _running_hooks[tool] = False
            return res
    return wrapper

class Hook:
    def __init__(self, target, toolchain):
        _hooks.clear()
        self.toolchain = toolchain
        target.init_hooks(self, toolchain.__class__.__name__)

    def hook_add(self, hook_type, hook_step, function):
        if not hook_type in _hook_types or not hook_step in _hook_steps:
            return False
        if not hook_type in _hooks:
            _hooks[hook_type] = {}
        _hooks[hook_type][hook_step] = function
        return True

    def hook_add_binary(self, hook_step, function):
        return self.hook_add("binary", hook_step, function)

################################################################################

