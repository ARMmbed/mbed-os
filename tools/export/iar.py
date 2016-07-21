"""
mbed SDK
Copyright (c) 2011-2015 ARM Limited

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
import re
import os
from project_generator_definitions.definitions import ProGenDef

from tools.export.exporters import Exporter, ExporterTargetsProperty
from tools.targets import TARGET_MAP, TARGET_NAMES

# If you wish to add a new target, add it to project_generator_definitions, and then
# define progen_target name in the target class (`` self.progen_target = 'my_target_name' ``)
class IAREmbeddedWorkbench(Exporter):
    """
    Exporter class for IAR Systems. This class uses project generator.
    """
    # These 2 are currently for exporters backward compatiblity
    NAME = 'IAR'
    TOOLCHAIN = 'IAR'
    # PROGEN_ACTIVE contains information for exporter scripts that this is using progen
    PROGEN_ACTIVE = True

    MBED_CONFIG_HEADER_SUPPORTED = True

    @ExporterTargetsProperty
    def TARGETS(cls):
        if not hasattr(cls, "_targets_supported"):
            cls._targets_supported = []
            progendef = ProGenDef('iar')
            for target in TARGET_NAMES:
                try:
                    if (progendef.is_supported(str(TARGET_MAP[target])) or
                        progendef.is_supported(TARGET_MAP[target].progen['target'])):
                        cls._targets_supported.append(target)
                except AttributeError:
                    # target is not supported yet
                    continue
        return cls._targets_supported

    def generate(self, progen_build=False):
        """ Generates the project files """
        project_data = self.progen_get_project_data()
        tool_specific = {}
        # Expand tool specific settings by IAR specific settings which are required
        try:
            if TARGET_MAP[self.target].progen['iar']['template']:
                tool_specific['iar'] = TARGET_MAP[self.target].progen['iar']
        except KeyError:
            # use default template
            # by the mbed projects
            tool_specific['iar'] = {
                    # We currently don't use misc, template sets those for us
                    # 'misc': {
                    #     'cxx_flags': ['--no_rtti', '--no_exceptions'],
                    #     'c_flags': ['--diag_suppress=Pa050,Pa084,Pa093,Pa082'],
                    #     'ld_flags': ['--skip_dynamic_initialization'],
                    # },
                    'template': [os.path.join(os.path.dirname(__file__),  'iar_template.ewp.tmpl')],
            }

        project_data['tool_specific'] = {}
        project_data['tool_specific'].setdefault("iar", {})
        project_data['tool_specific']['iar'].setdefault("misc", {})
        project_data['tool_specific']['iar'].update(tool_specific['iar'])
        project_data['tool_specific']['iar']['misc'].update(self.progen_flags)
        # VLA is enabled via template IccAllowVLA
        project_data['tool_specific']['iar']['misc']['c_flags'].remove("--vla")
        project_data['common']['build_dir'] = os.path.join(project_data['common']['build_dir'], 'iar_arm')
        if progen_build:
            self.progen_gen_file('iar_arm', project_data, True)
        else:
            self.progen_gen_file('iar_arm', project_data)

# Currently not used, we should reuse folder_name to create virtual folders
class IarFolder():
    """
    This is a recursive folder object.
    To present the folder structure in the IDE as it is presented on the disk.
    This can be used for uvision as well if you replace the __str__ method.
    Example:
    files: ./main.cpp, ./apis/I2C.h, ./mbed/common/I2C.cpp
    in the project this would look like:
    main.cpp
    common/I2C.cpp
    input:
    folder_level : folder path to current folder
    folder_name : name of current folder
    source_files : list of source_files (all must be in same directory)
    """
    def __init__(self, folder_level, folder_name, source_files):
        self.folder_level = folder_level
        self.folder_name = folder_name
        self.source_files = source_files
        self.sub_folders = {}

    def __str__(self):
        """
        converts the folder structue to IAR project format.
        """
        group_start = ""
        group_end = ""
        if self.folder_name != "":
            group_start = "<group>\n<name>%s</name>\n" %(self.folder_name)
            group_end = "</group>\n"

        str_content = group_start
        #Add files in current folder
        if self.source_files:
            for src in self.source_files:
                str_content += "<file>\n<name>$PROJ_DIR$/%s</name>\n</file>\n" % src
        #Add sub folders
        if self.sub_folders:
            for folder_name in self.sub_folders.iterkeys():
                str_content += self.sub_folders[folder_name].__str__()

        str_content += group_end
        return str_content

    def insert_file(self, source_input):
        """
        Inserts a source file into the folder tree
        """
        if self.source_files:
            #All source_files in a IarFolder must be in same directory.
            dir_sources = IarFolder.get_directory(self.source_files[0])
            #Check if sources are already at their deepest level.
            if not self.folder_level == dir_sources:
                _reg_exp = r"^" + re.escape(self.folder_level) + r"[/\\]?([^/\\]+)"
                folder_name = re.match(_reg_exp, dir_sources).group(1)
                self.sub_folders[folder_name] = IarFolder(os.path.join(self.folder_level, folder_name), folder_name, self.source_files)
                self.source_files = []

        dir_input = IarFolder.get_directory(source_input)
        if dir_input == self.folder_level:
            self.source_files.append(source_input)
        else:
            _reg_exp = r"^" + re.escape(self.folder_level) + r"[/\\]?([^/\\]+)"
            folder_name = re.match(_reg_exp, dir_input).group(1)
            if self.sub_folders.has_key(folder_name):
                self.sub_folders[folder_name].insert_file(source_input)
            else:
                if self.folder_level == "":
                    #Top level exception
                    self.sub_folders[folder_name] = IarFolder(folder_name, folder_name, [source_input])
                else:
                    self.sub_folders[folder_name] = IarFolder(os.path.join(self.folder_level, folder_name), folder_name, [source_input])

    @staticmethod
    def get_directory(file_path):
        """
        Returns the directory of the file
        """
        return os.path.dirname(file_path)
