"""Just a template for subclassing"""
import os
import sys
import logging
from os.path import join, dirname, relpath
from itertools import groupby
from jinja2 import FileSystemLoader
from jinja2.environment import Environment

from tools.targets import TARGET_MAP
from project_generator.tools import tool
from project_generator.tools_supported import ToolsSupported
from project_generator.settings import ProjectSettings
from project_generator_definitions.definitions import ProGenDef


class OldLibrariesException(Exception):
    """Exception that indicates an export can not complete due to an out of date
    library version.
    """
    pass

class FailedBuildException(Exception):
    """Exception that indicates that a build failed"""
    pass

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

class Exporter(object):
    """Exporter base class

    This class is meant to be extended by individual exporters, and provides a
    few helper methods for implementing an exporter with either jinja2 or
    progen.
    """
    TEMPLATE_DIR = dirname(__file__)
    DOT_IN_RELATIVE_PATH = False
    NAME = None
    TARGETS = None
    TOOLCHAIN = None

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
        self.generated_files = [join(self.TEMPLATE_DIR,"GettingStarted.html")]
        self.builder_files_dict = {}

    def get_toolchain(self):
        """A helper getter function that we should probably eliminate"""
        return self.TOOLCHAIN

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
        config_header = self.toolchain.get_config_header()
        flags = {key + "_flags": value for key, value
                 in self.toolchain.flags.iteritems()}
        asm_defines = ["-D" + symbol for symbol in self.toolchain.get_symbols(True)]
        c_defines = ["-D" + symbol for symbol in self.toolchain.get_symbols()]
        flags['asm_flags'] += asm_defines
        flags['c_flags'] += c_defines
        flags['cxx_flags'] += c_defines
        if config_header:
            config_header = relpath(config_header,
                                    self.resources.file_basepath[config_header])
            flags['c_flags'] += self.toolchain.get_config_option(config_header)
            flags['cxx_flags'] += self.toolchain.get_config_option(
                config_header)
        return flags

    def get_source_paths(self):
        """Returns a list of the directories where source files are contained"""
        source_keys = ['s_sources', 'c_sources', 'cpp_sources', 'hex_files',
                       'objects', 'libraries']
        source_files = []
        for key in source_keys:
            source_files.extend(getattr(self.resources, key))
        return list(set([os.path.dirname(src) for src in source_files]))

    def progen_get_project_data(self):
        """ Get ProGen project data  """
        # provide default data, some tools don't require any additional
        # tool specific settings

        if not self.check_supported(self.NAME):
            raise TargetNotSupportedException("Target not supported")

        def make_key(src):
            """turn a source file into it's group name"""
            key = os.path.basename(os.path.dirname(src))
            if not key or relpath(key, self.export_dir) == '.':
                key = self.project_name
            return key

        def grouped(sources):
            """Group the source files by their encompassing directory"""
            data = sorted(sources, key=make_key)
            return {k: list(g) for k, g in groupby(data, make_key)}

        if self.toolchain.get_config_header():
            config_header = self.toolchain.get_config_header()
            config_header = relpath(config_header,
                                    self.resources.file_basepath[config_header])
        else:
            config_header = None

        # we want to add this to our include dirs
        config_dir = os.path.dirname(config_header) if config_header else []

        project_data = tool.get_tool_template()

        project_data['target'] = TARGET_MAP[self.target].progen['target']
        project_data['source_paths'] = self.get_source_paths()
        project_data['include_paths'] = self.resources.inc_dirs + [config_dir]
        project_data['include_files'] = grouped(self.resources.headers)
        project_data['source_files_s'] = grouped(self.resources.s_sources)
        project_data['source_files_c'] = grouped(self.resources.c_sources)
        project_data['source_files_cpp'] = grouped(self.resources.cpp_sources)
        project_data['source_files_obj'] = grouped(self.resources.objects)
        project_data['source_files_lib'] = grouped(self.resources.libraries)
        project_data['output_dir']['path'] = self.export_dir
        project_data['linker_file'] = self.resources.linker_script
        project_data['macros'] = []
        project_data['build_dir'] = 'build'
        project_data['template'] = None
        project_data['name'] = self.project_name
        project_data['output_type'] = 'exe'
        project_data['debugger'] = None
        return project_data

    def progen_gen_file(self, project_data):
        """ Generate project using ProGen Project API
        Positional arguments:
        tool_name    - the tool for which to generate project files
        project_data - a dict whose base key, values are specified in
                       progen_get_project_data, the items will have been
                       modified by Exporter subclasses

        Keyword arguments:
        progen_build - A boolean that determines if the tool will build the
                       project
        """
        if not self.check_supported(self.NAME):
            raise TargetNotSupportedException("Target not supported")
        settings = ProjectSettings()
        exporter = ToolsSupported().get_tool(self.NAME)
        self.builder_files_dict = {self.NAME:exporter(project_data, settings).export_project()}
        for  middle in self.builder_files_dict.values():
            for field, thing in middle.iteritems():
                if field == "files":
                    for filename in thing.values():
                        self.generated_files.append(filename)

    def progen_build(self):
        """Build a project that was already generated by progen"""
        builder = ToolsSupported().get_tool(self.NAME)
        result = builder(self.builder_files_dict[self.NAME], ProjectSettings()).build_project()
        if result == -1:
            raise FailedBuildException("Build Failed")

    def check_supported(self, ide):
        """Indicated if this combination of IDE and MCU is supported"""
        if self.target not in self.TARGETS or \
           self.TOOLCHAIN not in TARGET_MAP[self.target].supported_toolchains:
            return False
        if not ProGenDef(ide).is_supported(
                TARGET_MAP[self.target].progen['target']):
            return False
        return True

    def gen_file(self, template_file, data, target_file):
        """Generates a project file from a template using jinja"""
        jinja_loader = FileSystemLoader(
            os.path.dirname(os.path.abspath(__file__)))
        jinja_environment = Environment(loader=jinja_loader)

        template = jinja_environment.get_template(template_file)
        target_text = template.render(data)

        target_path = join(self.export_dir, target_file)
        logging.debug("Generating: %s", target_path)
        open(target_path, "w").write(target_text)
        self.generated_files += [target_path]
