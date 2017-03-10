""" The new way of doing exports """
import sys
from os.path import join, abspath, dirname, exists
from os.path import basename, relpath, normpath, splitext
from os import makedirs, walk
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)
import copy
from shutil import rmtree
import zipfile

from tools.build_api import prepare_toolchain
from tools.build_api import scan_resources
from tools.export import EXPORTERS
from tools.toolchains import Resources


def get_exporter_toolchain(ide):
    """ Return the exporter class and the toolchain string as a tuple

    Positional arguments:
    ide - the ide name of an exporter
    """
    return EXPORTERS[ide], EXPORTERS[ide].TOOLCHAIN


def rewrite_basepath(file_name, resources, export_path, loc):
    """ Replace the basepath of filename with export_path

    Positional arguments:
    file_name - the absolute path to a file
    resources - the resources object that the file came from
    export_path - the final destination of the file after export
    """
    new_f = join(loc, relpath(file_name, resources.file_basepath[file_name]))
    resources.file_basepath[join(export_path, new_f)] = export_path
    return new_f


def subtract_basepath(resources, export_path, loc=""):
    """ Rewrite all of the basepaths with the export_path

    Positional arguments:
    resources - the resource object to rewrite the basepaths of
    export_path - the final destination of the resources with respect to the
      generated project files
    """
    keys = ['s_sources', 'c_sources', 'cpp_sources', 'hex_files',
            'objects', 'libraries', 'inc_dirs', 'headers', 'linker_script',
            'lib_dirs']
    for key in keys:
        vals = getattr(resources, key)
        if isinstance(vals, set):
            vals = list(vals)
        if isinstance(vals, list):
            new_vals = []
            for val in vals:
                new_vals.append(rewrite_basepath(val, resources, export_path,
                                                 loc))
            if isinstance(getattr(resources, key), set):
                setattr(resources, key, set(new_vals))
            else:
                setattr(resources, key, new_vals)
        elif vals:
            setattr(resources, key, rewrite_basepath(vals, resources,
                                                     export_path, loc))


def generate_project_files(resources, export_path, target, name, toolchain, ide,
                           macros=None):
    """Generate the project files for a project

    Positional arguments:
    resources - a Resources object containing all of the files needed to build
      this project
    export_path - location to place project files
    name - name of the project
    toolchain - a toolchain class that corresponds to the toolchain used by the
      IDE or makefile
    ide - IDE name to export to

    Optional arguments:
    macros - additional macros that should be defined within the exported
      project
    """
    exporter_cls, _ = get_exporter_toolchain(ide)
    exporter = exporter_cls(target, export_path, name, toolchain,
                            extra_symbols=macros, resources=resources)
    exporter.generate()
    files = exporter.generated_files
    return files, exporter


def zip_export(file_name, prefix, resources, project_files, inc_repos):
    """Create a zip file from an exported project.

    Positional Parameters:
    file_name - the file name of the resulting zip file
    prefix - a directory name that will prefix the entire zip file's contents
    resources - a resources object with files that must be included in the zip
    project_files - a list of extra files to be added to the root of the prefix
      directory
    """
    with zipfile.ZipFile(file_name, "w") as zip_file:
        for prj_file in project_files:
            zip_file.write(prj_file, join(prefix, basename(prj_file)))
        for loc, res in resources.iteritems():
            to_zip = (
                res.headers + res.s_sources + res.c_sources +\
                res.cpp_sources + res.libraries + res.hex_files + \
                [res.linker_script] + res.bin_files + res.objects + \
                res.json_files + res.lib_refs + res.lib_builds)
            if inc_repos:
                for directory in res.repo_dirs:
                    for root, _, files in walk(directory):
                        for repo_file in files:
                            source = join(root, repo_file)
                            to_zip.append(source)
                            res.file_basepath[source] = res.base_path
                to_zip += res.repo_files
            for source in to_zip:
                if source:
                    zip_file.write(
                        source,
                        join(prefix, loc,
                             relpath(source, res.file_basepath[source])))
            for source in res.lib_builds:
                target_dir, _ = splitext(source)
                dest = join(prefix, loc,
                            relpath(target_dir, res.file_basepath[source]),
                            ".bld", "bldrc")
                zip_file.write(source, dest)



def export_project(src_paths, export_path, target, ide, libraries_paths=None,
                   linker_script=None, notify=None, verbose=False, name=None,
                   inc_dirs=None, jobs=1, silent=False, extra_verbose=False,
                   config=None, macros=None, zip_proj=None, inc_repos=False,
                   build_profile=None):
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
    verbose - assigns the notify function to toolchains print_notify_verbose
    name - project name
    inc_dirs - additional include directories
    jobs - number of threads
    silent - silent build - no output
    extra_verbose - assigns the notify function to toolchains
      print_notify_verbose
    config - toolchain's config object
    macros - User-defined macros
    zip_proj - string name of the zip archive you wish to creat (exclude arg
     if you do not wish to create an archive
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
        notify=notify, silent=silent, verbose=verbose,
        extra_verbose=extra_verbose, config=config, build_profile=build_profile)
    # The first path will give the name to the library
    if name is None:
        name = basename(normpath(abspath(src_paths[0])))

    # Call unified scan_resources
    resource_dict = {loc: scan_resources(path, toolchain, inc_dirs=inc_dirs)
                     for loc, path in src_paths.iteritems()}
    resources = Resources()
    toolchain.build_dir = export_path
    config_header = toolchain.get_config_header()
    resources.headers.append(config_header)
    resources.file_basepath[config_header] = dirname(config_header)

    if zip_proj:
        subtract_basepath(resources, export_path)
        for loc, res in resource_dict.iteritems():
            temp = copy.deepcopy(res)
            subtract_basepath(temp, export_path, loc)
            resources.add(temp)
    else:
        for _, res in resource_dict.iteritems():
            resources.add(res)

    # Change linker script if specified
    if linker_script is not None:
        resources.linker_script = linker_script

    files, exporter = generate_project_files(resources, export_path,
                                             target, name, toolchain, ide,
                                             macros=macros)
    files.append(config_header)
    if zip_proj:
        for resource in resource_dict.values():
            for label, res in resource.features.iteritems():
                if label not in toolchain.target.features:
                    resource.add(res)
        if isinstance(zip_proj, basestring):
            zip_export(join(export_path, zip_proj), name, resource_dict, files,
                       inc_repos)
        else:
            zip_export(zip_proj, name, resource_dict, files, inc_repos)

    return exporter


def print_results(successes, failures, skips=None):
    """ Print out the results of an export process

    Positional arguments:
    successes - The list of exports that succeeded
    failures - The list of exports that failed

    Keyword arguments:
    skips - The list of exports that were skipped
    """
    print
    if successes:
        print "Successful: "
        for success in successes:
            print "  * %s" % success
    if failures:
        print "Failed: "
        for failure in failures:
            print "  * %s" % failure
    if skips:
        print "Skipped: "
        for skip in skips:
            print "  * %s" % skip

