""" Configurable hooks in the build system. Can be used by various platforms
to customize the build process.
"""

################################################################################
# Hooks for the various parts of the build process

# Internal mapping of hooks per tool
_HOOKS = {}

# Internal mapping of running hooks
_RUNNING_HOOKS = {}

# Available hook types
_HOOK_TYPES = ["binary", "compile", "link", "assemble"]

# Available hook steps
_HOOK_STEPS = ["pre", "replace", "post"]

# Hook the given function. Use this function as a decorator
def hook_tool(function):
    """Decorate a function as a tool that may be hooked"""
    tool = function.__name__
    tool_flag = "_" + tool + "_done"
    def wrapper(t_self, *args, **kwargs):
        """The hooked function itself"""
        # if a hook for this tool is already running, it's most likely
        # coming from a derived class, so don't hook the super class version
        if _RUNNING_HOOKS.get(tool, False):
            return function(t_self, *args, **kwargs)
        _RUNNING_HOOKS[tool] = True
        # If this tool isn't hooked, return original function
        if tool not in _HOOKS:
            res = function(t_self, *args, **kwargs)
            _RUNNING_HOOKS[tool] = False
            return res
        tooldesc = _HOOKS[tool]
        setattr(t_self, tool_flag, False)
        # If there is a replace hook, execute the replacement instead
        if "replace" in tooldesc:
            res = tooldesc["replace"](t_self, *args, **kwargs)
        # If the replacement has set the "done" flag, exit now
        # Otherwise continue as usual
        if getattr(t_self, tool_flag, False):
            _RUNNING_HOOKS[tool] = False
            return res
        # Execute pre-function before main function if specified
        if "pre" in tooldesc:
            tooldesc["pre"](t_self, *args, **kwargs)
        # Execute the main function now
        res = function(t_self, *args, **kwargs)
        # Execute post-function after main function if specified
        if "post" in tooldesc:
            post_res = tooldesc["post"](t_self, *args, **kwargs)
            _RUNNING_HOOKS[tool] = False
            return post_res or res
        else:
            _RUNNING_HOOKS[tool] = False
            return res
    return wrapper

class Hook(object):
    """A compiler class that may be hooked"""
    def __init__(self, target, toolchain):
        _HOOKS.clear()
        self._cmdline_hooks = {}
        self.toolchain = toolchain
        target.init_hooks(self, toolchain)

    # Hook various functions directly
    @staticmethod
    def _hook_add(hook_type, hook_step, function):
        """Add a hook to a compile function

        Positional arguments:
        hook_type - one of the _HOOK_TYPES
        hook_step - one of the _HOOK_STEPS
        function - the function to add to the list of hooks
        """
        if hook_type not in _HOOK_TYPES or hook_step not in _HOOK_STEPS:
            return False
        if hook_type not in _HOOKS:
            _HOOKS[hook_type] = {}
        _HOOKS[hook_type][hook_step] = function
        return True

    def hook_add_compiler(self, hook_step, function):
        """Add a hook to the compiler

        Positional Arguments:
        hook_step - one of the _HOOK_STEPS
        function - the function to add to the list of hooks
        """
        return self._hook_add("compile", hook_step, function)

    def hook_add_linker(self, hook_step, function):
        """Add a hook to the linker

        Positional Arguments:
        hook_step - one of the _HOOK_STEPS
        function - the function to add to the list of hooks
        """
        return self._hook_add("link", hook_step, function)

    def hook_add_assembler(self, hook_step, function):
        """Add a hook to the assemble

        Positional Arguments:
        hook_step - one of the _HOOK_STEPS
        function - the function to add to the list of hooks
        """
        return self._hook_add("assemble", hook_step, function)

    def hook_add_binary(self, hook_step, function):
        """Add a hook to the elf to binary tool

        Positional Arguments:
        hook_step - one of the _HOOK_STEPS
        function - the function to add to the list of hooks
        """
        return self._hook_add("binary", hook_step, function)

    # Hook command lines
    def _hook_cmdline(self, hook_type, function):
        """Add a hook to a command line function

        Positional arguments:
        hook_type - one of the _HOOK_TYPES
        function - the function to add to the list of hooks
        """
        if hook_type not in _HOOK_TYPES:
            return False
        self._cmdline_hooks[hook_type] = function
        return True

    def hook_cmdline_compiler(self, function):
        """Add a hook to the compiler command line

        Positional arguments:
        function - the function to call
        """
        return self._hook_cmdline("compile", function)

    def hook_cmdline_linker(self, function):
        """Add a hook to the linker command line

        Positional arguments:
        function - the function to call
        """
        return self._hook_cmdline("link", function)

    def hook_cmdline_assembler(self, function):
        """Add a hook to the assembler command line

        Positional arguments:
        function - the function to call
        """
        return self._hook_cmdline("assemble", function)

    def hook_cmdline_binary(self, function):
        """Add a hook to the elf to bin tool command line

        Positional arguments:
        function - the function to call
        """
        return self._hook_cmdline("binary", function)

    # Return the command line after applying the hook
    def _get_cmdline(self, hook_type, cmdline):
        """Get the command line after running all hooks

        Positional arguments:
        hook_type - one of the _HOOK_TYPES
        cmdline - the initial command line
        """
        if hook_type in  self._cmdline_hooks:
            cmdline = self._cmdline_hooks[hook_type](
                self.toolchain.__class__.__name__, cmdline)
        return cmdline

    def get_cmdline_compiler(self, cmdline):
        """Get the compiler command line after running all hooks

        Positional arguments:
        cmdline - the initial command line
        """
        return self._get_cmdline("compile", cmdline)

    def get_cmdline_linker(self, cmdline):
        """Get the linker command line after running all hooks

        Positional arguments:
        cmdline - the initial command line
        """
        return self._get_cmdline("link", cmdline)

    def get_cmdline_assembler(self, cmdline):
        """Get the assmebler command line after running all hooks

        Positional arguments:
        cmdline - the initial command line
        """
        return self._get_cmdline("assemble", cmdline)

    def get_cmdline_binary(self, cmdline):
        """Get the binary command line after running all hooks

        Positional arguments:
        cmdline - the initial command line
        """
        return self._get_cmdline("binary", cmdline)

################################################################################

