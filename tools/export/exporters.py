"""Just a template for subclassing"""
import os
from abc import abstractmethod, ABCMeta
import logging
from os.path import join, dirname, relpath, basename, realpath, normpath, exists
from itertools import groupby
from jinja2 import FileSystemLoader, StrictUndefined
from jinja2.environment import Environment
import copy

from tools.targets import TARGET_MAP
from tools.utils import mkdir
from tools.resources import FileType


class TargetNotSupportedException(Exception):
    """Indicates that an IDE does not support a particular MCU"""
    pass

class ExporterTargetsProperty(object):
    """ Exporter descriptor for TARGETS
    TARGETS as class attribute for backward compatibility
    (allows: if in Exporter.TARGETS)
    """
    def __init__(self, func):
        self.func = func
    def __get__(self, inst, cls):
        return self.func(cls)

def deprecated_exporter(CLS):
    old_init = CLS.__init__
    old_name = CLS.NAME
    def __init__(*args, **kwargs):
        print("==================== DEPRECATION NOTICE ====================")
        print("The exporter %s is no longer maintained, and deprecated." % old_name)
        print("%s will be removed from mbed OS for the mbed OS 5.6 release." % old_name)
        old_init(*args, **kwargs)
    CLS.__init__ = __init__
    CLS.NAME = "%s (DEPRECATED)" % old_name
    return CLS

class Exporter(object):
    """Exporter base class

    This class is meant to be extended by individual exporters, and provides a
    few helper methods for implementing an exporter with either jinja2 or
    progen.
    """
    __metaclass__ = ABCMeta
    TEMPLATE_DIR = dirname(__file__)
    DOT_IN_RELATIVE_PATH = False
    NAME = None
    TARGETS = set()
    TOOLCHAIN = None
    CLEAN_FILES = ("GettingStarted.html",)


    def __init__(self, target, export_dir, project_name, toolchain,
                 extra_symbols=None, resources=None):
        """Initialize an instance of class exporter
        Positional arguments:
        target        - the target mcu/board for this project
        export_dir    - the directory of the exported project files
        project_name  - the name of the project
        toolchain     - an instance of class toolchain

        Keyword arguments:
        extra_symbols - a list of extra macros for the toolchain
        resources     - an instance of class Resources
        """
        self.export_dir = export_dir
        self.target = target
        self.project_name = project_name
        self.toolchain = toolchain
        jinja_loader = FileSystemLoader(os.path.dirname(os.path.abspath(__file__)))
        self.jinja_environment = Environment(loader=jinja_loader)
        self.resources = resources
        self.generated_files = []
        self.static_files = (
            join(self.TEMPLATE_DIR, "GettingStarted.html"),
            join(self.TEMPLATE_DIR, ".mbed"),
        )
        self.builder_files_dict = {}
        self.add_config()

    def get_toolchain(self):
        """A helper getter function that we should probably eliminate"""
        return self.TOOLCHAIN

    def add_config(self):
        """Add the containing directory of mbed_config.h to include dirs"""
        pass

    @property
    def flags(self):
        """Returns a dictionary of toolchain flags.
        Keys of the dictionary are:
        cxx_flags    - c++ flags
        c_flags      - c flags
        ld_flags     - linker flags
        asm_flags    - assembler flags
        common_flags - common options
        """
        flags = self.toolchain_flags(self.toolchain)
        asm_defines = self.toolchain.get_compile_options(
            self.toolchain.get_symbols(for_asm=True),
            filter(None, self.resources.inc_dirs),
            for_asm=True)
        c_defines = ["-D" + symbol for symbol in self.toolchain.get_symbols()]
        flags['asm_flags'] += asm_defines
        flags['c_flags'] += c_defines
        flags['cxx_flags'] += c_defines
        config_header = self.config_header_ref
        if config_header:
            config_option = self.toolchain.get_config_option(
                config_header.name)
            flags['c_flags'] += config_option
            flags['cxx_flags'] += config_option
        return flags

    @property
    def libraries(self):
        return [l for l in self.resources.get_file_names(FileType.LIB)
                if l.endswith(self.toolchain.LIBRARY_EXT)]

    def toolchain_flags(self, toolchain):
        """Returns a dictionary of toolchain flags.
        Keys of the dictionary are:
        cxx_flags    - c++ flags
        c_flags      - c flags
        ld_flags     - linker flags
        asm_flags    - assembler flags
        common_flags - common options

        The difference from the above is that it takes a parameter.
        """
        flags = {key + "_flags": copy.deepcopy(value) for key, value
                 in toolchain.flags.items()}
        config_header = self.config_header_ref
        if config_header:
            header_options = self.toolchain.get_config_option(
                config_header.name)
            flags['c_flags'] += header_options
            flags['cxx_flags'] += header_options
        return flags

    @property
    def config_header_ref(self):
        config_header = self.toolchain.get_config_header()
        if config_header:
            def is_config_header(f):
                return f.path == config_header
            return list(filter(
                is_config_header, self.resources.get_file_refs(FileType.HEADER)
            ))[0]
        else:
            return None

    def get_source_paths(self):
        """Returns a list of the directories where source files are contained"""
        source_keys = ['s_sources', 'c_sources', 'cpp_sources', 'hex_files',
                       'objects', 'libraries']
        source_files = []
        for key in source_keys:
            source_files.extend(getattr(self.resources, key))
        return list(set([os.path.dirname(src) for src in source_files]))

    def gen_file_dest(self, target_file):
        """Generate the project file location in an exported project"""
        return join(self.export_dir, target_file)

    def gen_file(self, template_file, data, target_file, **kwargs):
        """Generates a project file from a template using jinja"""
        target_text = self._gen_file_inner(template_file, data, target_file, **kwargs)
        target_path = self.gen_file_dest(target_file)
        mkdir(dirname(target_path))
        logging.debug("Generating: %s", target_path)
        open(target_path, "w").write(target_text)
        self.generated_files += [target_path]

    def gen_file_nonoverwrite(self, template_file, data, target_file, **kwargs):
        """Generates a project file from a template using jinja"""
        target_text = self._gen_file_inner(template_file, data, target_file, **kwargs)
        target_path = self.gen_file_dest(target_file)
        if exists(target_path):
            with open(target_path) as fdin:
                old_text = fdin.read()
            if target_text not in old_text:
                with open(target_path, "a") as fdout:
                    fdout.write(target_text)
        else:
            logging.debug("Generating: %s", target_path)
            open(target_path, "w").write(target_text)
        self.generated_files += [target_path]

    def _gen_file_inner(self, template_file, data, target_file, **kwargs):
        """Generates a project file from a template using jinja"""
        jinja_loader = FileSystemLoader(
            os.path.dirname(os.path.abspath(__file__)))
        jinja_environment = Environment(loader=jinja_loader,
                                        undefined=StrictUndefined, **kwargs)

        template = jinja_environment.get_template(template_file)
        target_text = template.render(data)
        return target_text

        target_path = join(self.export_dir, target_file)
        logging.debug("Generating: %s", target_path)
        open(target_path, "w").write(target_text)
        self.generated_files += [target_path]

    def make_key(self, src):
        """From a source file, extract group name
        Positional Arguments:
        src - the src's location
        """
        path_list = os.path.normpath(src).split(os.sep)
        assert len(path_list) >= 1
        if len(path_list) == 1:
            key = self.project_name
        else:
            key = path_list[0]
        return key

    def group_project_files(self, sources):
        """Group the source files by their encompassing directory
        Positional Arguments:
        sources - array of source locations

        Returns a dictionary of {group name: list of source locations}
        """
        data = sorted(sources, key=self.make_key)
        return {k: list(g) for k,g in groupby(data, self.make_key)}

    @staticmethod
    def build(project_name, log_name='build_log.txt', cleanup=True):
        """Invoke exporters build command within a subprocess.
        This method is assumed to be executed at the same level as exporter
        project files and project source code.
        See uvision/__init__.py, iar/__init__.py, and makefile/__init__.py for
        example implemenation.

        Positional Arguments:
        project_name - the name of the project to build; often required by
        exporter's build command.

        Keyword Args:
        log_name - name of the build log to create. Written and printed out,
        deleted if cleanup = True
        cleanup - a boolean dictating whether exported project files and
        build log are removed after build

        Returns -1 on failure and 0 on success
        """
        raise NotImplementedError("Implement in derived Exporter class.")

    @staticmethod
    def clean(project_name):
        """Clean a previously exported project
        This method is assumed to be executed at the same level as exporter
        project files and project source code.
        See uvision/__init__.py, iar/__init__.py, and makefile/__init__.py for
        example implemenation.

        Positional Arguments:
        project_name - the name of the project to build; often required by
        exporter's build command.

        Returns nothing. May raise exceptions
        """
        raise NotImplementedError("Implement in derived Exporter class.")

    @abstractmethod
    def generate(self):
        """Generate an IDE/tool specific project file"""
        raise NotImplementedError("Implement a generate function in Exporter child class")

    @classmethod
    def is_target_supported(cls, target_name):
        """Query support for a particular target

        NOTE: override this method if your exporter does not provide a static list of targets

        Positional Arguments:
        target_name - the name of the target.
        """
        target = TARGET_MAP[target_name]
        return bool(set(target.resolution_order_names).intersection(set(cls.TARGETS))) \
            and cls.TOOLCHAIN in target.supported_toolchains


    @classmethod
    def all_supported_targets(cls):
        return [t for t in TARGET_MAP.keys() if cls.is_target_supported(t)]

    @staticmethod
    def filter_dot(str):
        """
        Remove the './' or '.\\' prefix, if present.
        """
        if str == None:
            return None
        if str[:2] == './':
            return str[2:]
        if str[:2] == '.\\':
            return str[2:]
        return str



def apply_supported_whitelist(compiler, whitelist, target):
    """Generate a list of supported targets for a given compiler and post-binary hook
    white-list."""
    if compiler not in target.supported_toolchains:
        return False
    if not hasattr(target, "post_binary_hook"):
        return True
    if target.post_binary_hook['function'] in whitelist:
        return True
    else:
        return False
