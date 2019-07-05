"""The generic interface for all exporters.
"""
# mbed SDK
# Copyright (c) 2011-2016 ARM Limited
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

from __future__ import print_function, division, absolute_import

import sys
from os.path import join, abspath, dirname, exists, isfile
from os.path import basename, relpath, normpath, splitext
from os import makedirs, walk
import copy
from shutil import rmtree, copyfile
import zipfile
from past.builtins import basestring

from ..resources import Resources, FileType, FileRef
from ..config import ALLOWED_FEATURES
from ..build_api import prepare_toolchain
from ..targets import TARGET_NAMES
from . import (lpcxpresso, iar, makefile, embitz, coide, kds, simplicity,
               atmelstudio, mcuxpresso, sw4stm32, e2studio, zip, cmsis, uvision,
               cdt, vscode, gnuarmeclipse, qtcreator, cmake, nb, cces, codeblocks)

EXPORTERS = {
    u'uvision6': uvision.UvisionArmc6,
    u'uvision5': uvision.UvisionArmc5,
    u'make_gcc_arm': makefile.GccArm,
    u'make_armc5': makefile.Armc5,
    u'make_armc6': makefile.Armc6,
    u'make_iar': makefile.IAR,
    u'iar': iar.IAR,
    u'embitz' : embitz.EmBitz,
    u'sw4stm32'    : sw4stm32.Sw4STM32,
    u'e2studio' : e2studio.E2Studio,
    u'eclipse_gcc_arm'  : cdt.EclipseGcc,
    u'eclipse_iar'      : cdt.EclipseIAR,
    u'eclipse_armc5'    : cdt.EclipseArmc5,
    u'gnuarmeclipse': gnuarmeclipse.GNUARMEclipse,
    u'mcuxpresso': mcuxpresso.MCUXpresso,
    u'netbeans':     nb.GNUARMNetbeans,
    u'qtcreator': qtcreator.QtCreator,
    u'vscode_gcc_arm' : vscode.VSCodeGcc,
    u'vscode_iar' : vscode.VSCodeIAR,
    u'vscode_armc5' : vscode.VSCodeArmc5,
    u'cmake_gcc_arm': cmake.GccArm,
    u'cces' : cces.CCES,
    u'codeblocks': codeblocks.CodeBlocks
}

ERROR_MESSAGE_UNSUPPORTED_TOOLCHAIN = """
Sorry, the target %s is not currently supported on the %s toolchain.
Please refer to <a href='/handbook/Exporting-to-offline-toolchains' target='_blank'>Exporting to offline toolchains</a> for more information.
"""

ERROR_MESSAGE_NOT_EXPORT_LIBS = """
To export this project please <a href='http://mbed.org/compiler/?import=http://mbed.org/users/mbed_official/code/mbed-export/k&mode=lib' target='_blank'>import the export version of the mbed library</a>.
"""

def mcu_ide_list():
    """Shows list of exportable ides

    """
    supported_ides = sorted(EXPORTERS.keys())
    return "\n".join(supported_ides)


def mcu_ide_matrix(verbose_html=False):
    """Shows target map using prettytable

    Keyword argumets:
    verbose_html - print the matrix in html format
    """
    supported_ides = sorted(EXPORTERS.keys())
    # Only use it in this function so building works without extra modules
    from prettytable import PrettyTable, HEADER

    # All tests status table print
    table_printer = PrettyTable(["Platform"] + supported_ides, junction_char="|", hrules=HEADER)
    # Align table
    for col in supported_ides:
        table_printer.align[col] = "c"
    table_printer.align["Platform"] = "l"

    perm_counter = 0
    for target in sorted(TARGET_NAMES):
        row = [target]  # First column is platform name
        for ide in supported_ides:
            text = "-"
            if EXPORTERS[ide].is_target_supported(target):
                if verbose_html:
                    text = "&#10003;"
                else:
                    text = "x"
                perm_counter += 1
            row.append(text)
        table_printer.add_row(row)

    # creates a html page in a shorter format suitable for readme.md
    if verbose_html:
        result = table_printer.get_html_string()
    else:
        result = table_printer.get_string()
    result += "\n\n"
    result += "Total IDEs: %d\n"% (len(supported_ides))
    if verbose_html:
        result += "<br>"
    result += "Total platforms: %d\n"% (len(TARGET_NAMES))
    if verbose_html:
        result += "<br>"
    result += "Total permutations: %d"% (perm_counter)
    if verbose_html:
        result = result.replace("&amp;", "&")
    return result


def get_exporter_toolchain(ide):
    """ Return the exporter class and the toolchain string as a tuple

    Positional arguments:
    ide - the ide name of an exporter
    """
    return EXPORTERS[ide], EXPORTERS[ide].TOOLCHAIN


def generate_project_files(resources, export_path, target, name, toolchain, ide,
                           zip, macros=None):
    """Generate the project files for a project

    Positional arguments:
    resources - a Resources object containing all of the files needed to build
      this project
    export_path - location to place project files
    name - name of the project
    toolchain - a toolchain class that corresponds to the toolchain used by the
      IDE or makefile
    ide - IDE name to export to
    zip - True if the exported project will be zipped

    Optional arguments:
    macros - additional macros that should be defined within the exported
      project
    """
    exporter_cls, _ = get_exporter_toolchain(ide)
    exporter = exporter_cls(target, export_path, name, toolchain, zip,
                            extra_symbols=macros, resources=resources)
    exporter.generate()
    files = exporter.generated_files
    return files, exporter


def _inner_zip_export(resources, prj_files, inc_repos):
    to_zip = sum((resources.get_file_refs(ftype) for ftype
                  in Resources.ALL_FILE_TYPES),
                 [])
    to_zip.extend(prj_files)
    for dest, source in resources.get_file_refs(FileType.BLD_REF):
        target_dir, _ = splitext(dest)
        dest = join(target_dir, ".bld", "bldrc")
        to_zip.append(FileRef(dest, source))
    if inc_repos:
        for dest, source in resources.get_file_refs(FileType.REPO_DIR):
            for root, _, files in walk(source):
                for repo_file in files:
                    file_source = join(root, repo_file)
                    file_dest = join(dest, relpath(file_source, source))
                    to_zip.append(FileRef(file_dest, file_source))
    return to_zip

def zip_export(file_name, prefix, resources, project_files, inc_repos, notify):
    """Create a zip file from an exported project.

    Positional Parameters:
    file_name - the file name of the resulting zip file
    prefix - a directory name that will prefix the entire zip file's contents
    resources - a resources object with files that must be included in the zip
    project_files - a list of extra files to be added to the root of the prefix
      directory
    """
    to_zip_list = sorted(set(_inner_zip_export(
        resources, project_files, inc_repos)))
    total_files = len(to_zip_list)
    zipped = 0
    with zipfile.ZipFile(file_name, "w") as zip_file:
        for dest, source in to_zip_list:
            if source and isfile(source):
                zip_file.write(source, join(prefix, dest))
                zipped += 1
                notify.progress("Zipping", source,
                                100 * (zipped / total_files))
            else:
                zipped += 1


def export_project(src_paths, export_path, target, ide, libraries_paths=None,
                   linker_script=None, notify=None, name=None, inc_dirs=None,
                   jobs=1, config=None, macros=None, zip_proj=None,
                   inc_repos=False, build_profile=None, app_config=None,
                   ignore=None, resource_filter=None):
    """Generates a project file and creates a zip archive if specified

    Positional Arguments:
    src_paths - a list of paths from which to find source files
    export_path - a path specifying the location of generated project files
    target - the mbed board/mcu for which to generate the executable
    ide - the ide for which to generate the project fields

    Keyword Arguments:
    libraries_paths - paths to additional libraries
    linker_script - path to the linker script for the specified target
    notify - function is passed all events, and expected to handle notification
      of the user, emit the events to a log, etc.
    name - project name
    inc_dirs - additional include directories
    jobs - number of threads
    config - toolchain's config object
    macros - User-defined macros
    zip_proj - string name of the zip archive you wish to creat (exclude arg
     if you do not wish to create an archive
    ignore - list of paths to add to mbedignore
    resource_filter - can be used for filtering out resources after scan
    """

    # Convert src_path to a list if needed
    if isinstance(src_paths, dict):
        paths = sum(src_paths.values(), [])
    elif isinstance(src_paths, list):
        paths = src_paths[:]
    else:
        paths = [src_paths]

    # Extend src_paths wit libraries_paths
    if libraries_paths is not None:
        paths.extend(libraries_paths)
    if not isinstance(src_paths, dict):
        src_paths = {"": paths}
    # Export Directory
    if not exists(export_path):
        makedirs(export_path)

    _, toolchain_name = get_exporter_toolchain(ide)

    # Pass all params to the unified prepare_resources()
    toolchain = prepare_toolchain(
        paths, "", target, toolchain_name, macros=macros, jobs=jobs,
        notify=notify, config=config, build_profile=build_profile,
        app_config=app_config, ignore=ignore)

    toolchain.RESPONSE_FILES = False
    if name is None:
        name = basename(normpath(abspath(src_paths[0])))

    resources = Resources(notify, collect_ignores=True)
    resources.add_toolchain_labels(toolchain)
    for loc, path in src_paths.items():
        for p in path:
            resources.add_directory(p, into_path=loc)
    toolchain.build_dir = export_path
    toolchain.config.load_resources(resources)
    toolchain.set_config_data(toolchain.config.get_config_data())
    config_header = toolchain.get_config_header()
    resources.add_file_ref(FileType.HEADER, basename(config_header), config_header)

    # Change linker script if specified
    if linker_script is not None:
        resources.linker_script = linker_script

    if toolchain.config.name:
        name = toolchain.config.name

    resources.filter(resource_filter)

    files, exporter = generate_project_files(
        resources, export_path, target, name, toolchain, ide, zip_proj, macros=macros
    )
    if zip_proj:
        resources.add_features(ALLOWED_FEATURES)
        if isinstance(zip_proj, basestring):
            zip_export(join(export_path, zip_proj), name, resources,
                       files + list(exporter.static_files), inc_repos, notify)
        else:
            zip_export(zip_proj, name, resources,
                       files + list(exporter.static_files), inc_repos, notify)
    else:
        for static_file in exporter.static_files:
            if not exists(join(export_path, basename(static_file.name))):
                copyfile(static_file.path, join(export_path, static_file.name))

    return exporter
