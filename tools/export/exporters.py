"""Just a template for subclassing"""
import uuid, shutil, os, logging, fnmatch
from os import walk, remove
from os.path import join, dirname, isdir, split
from copy import copy
from jinja2 import Template, FileSystemLoader
from jinja2.environment import Environment
from contextlib import closing
from zipfile import ZipFile, ZIP_DEFLATED
from operator import add

from tools.utils import mkdir
from tools.toolchains import TOOLCHAIN_CLASSES
from tools.targets import TARGET_MAP

from project_generator.generate import Generator
from project_generator.project import Project
from project_generator.settings import ProjectSettings

from tools.config import Config

class OldLibrariesException(Exception): pass

class FailedBuildException(Exception) : pass

# Exporter descriptor for TARGETS
# TARGETS as class attribute for backward compatibility (allows: if in Exporter.TARGETS)
class ExporterTargetsProperty(object):
    def __init__(self, func):
        self.func = func
    def __get__(self, inst, cls):
        return self.func(cls)

class Exporter(object):
    TEMPLATE_DIR = dirname(__file__)
    DOT_IN_RELATIVE_PATH = False

    def __init__(self, target, inputDir, program_name, build_url_resolver, extra_symbols=None, sources_relative=True):
        self.inputDir = inputDir
        self.target = target
        self.program_name = program_name
        self.toolchain = TOOLCHAIN_CLASSES[self.get_toolchain()](TARGET_MAP[target])
        self.build_url_resolver = build_url_resolver
        jinja_loader = FileSystemLoader(os.path.dirname(os.path.abspath(__file__)))
        self.jinja_environment = Environment(loader=jinja_loader)
        self.extra_symbols = extra_symbols if extra_symbols else []
        self.config_macros = []
        self.sources_relative = sources_relative
        self.config_header = None

    def get_toolchain(self):
        return self.TOOLCHAIN

    @property
    def flags(self):
        return self.toolchain.flags

    @property
    def progen_flags(self):
        if not hasattr(self, "_progen_flag_cache") :
            self._progen_flag_cache = dict([(key + "_flags", value) for key,value in self.flags.iteritems()])
            asm_defines = ["-D"+symbol for symbol in self.toolchain.get_symbols(True)]
            c_defines = ["-D" + symbol for symbol in self.toolchain.get_symbols()]
            self._progen_flag_cache['asm_flags'] += asm_defines
            self._progen_flag_cache['c_flags'] += c_defines
            self._progen_flag_cache['cxx_flags'] += c_defines
            if self.config_header:
                self._progen_flag_cache['c_flags'] += self.toolchain.get_config_option(self.config_header)
                self._progen_flag_cache['cxx_flags'] += self.toolchain.get_config_option(self.config_header)
        return self._progen_flag_cache

    def __scan_and_copy(self, src_path, trg_path):
        resources = self.toolchain.scan_resources(src_path)

        for r_type in ['headers', 's_sources', 'c_sources', 'cpp_sources',
            'objects', 'libraries', 'linker_script',
            'lib_builds', 'lib_refs', 'hex_files', 'bin_files']:
            r = getattr(resources, r_type)
            if r:
                self.toolchain.copy_files(r, trg_path, resources=resources)
        return resources

    @staticmethod
    def _get_dir_grouped_files(files):
        """ Get grouped files based on the dirname """
        files_grouped = {}
        for file in files:
            rel_path = os.path.relpath(file, os.getcwd())
            dir_path = os.path.dirname(rel_path)
            if dir_path == '':
                # all files within the current dir go into Source_Files
                dir_path = 'Source_Files'
            if not dir_path in files_grouped.keys():
                files_grouped[dir_path] = []
            files_grouped[dir_path].append(file)
        return files_grouped

    def progen_get_project_data(self):
        """ Get ProGen project data  """
        # provide default data, some tools don't require any additional
        # tool specific settings
        code_files = []
        for r_type in ['c_sources', 'cpp_sources', 's_sources']:
            for file in getattr(self.resources, r_type):
                code_files.append(file)

        sources_files = code_files + self.resources.hex_files + self.resources.objects + \
            self.resources.libraries
        sources_grouped = Exporter._get_dir_grouped_files(sources_files)
        headers_grouped = Exporter._get_dir_grouped_files(self.resources.headers)

        project_data = {
            'common': {
                'sources': sources_grouped,
                'includes': headers_grouped,
                'build_dir':'.build',
                'target': [TARGET_MAP[self.target].progen['target']],
                'macros': self.get_symbols(),
                'export_dir': [self.inputDir],
                'linker_file': [self.resources.linker_script],
            }
        }
        return project_data

    def progen_gen_file(self, tool_name, project_data, progen_build=False):
        """ Generate project using ProGen Project API """
        settings = ProjectSettings()
        project = Project(self.program_name, [project_data], settings)
        # TODO: Fix this, the inc_dirs are not valid (our scripts copy files), therefore progen
        # thinks it is not dict but a file, and adds them to workspace.
        project.project['common']['include_paths'] = self.resources.inc_dirs
        project.generate(tool_name, copied=not self.sources_relative)
        if progen_build:
            print("Project exported, building...")
            result = project.build(tool_name)
            if result == -1:
                raise FailedBuildException("Build Failed")

    def __scan_all(self, path):
        resources = []

        for root, dirs, files in walk(path):
            for d in copy(dirs):
                if d == '.' or d == '..':
                    dirs.remove(d)

            for file in files:
                file_path = join(root, file)
                resources.append(file_path)

        return resources

    def scan_and_copy_resources(self, prj_paths, trg_path, relative=False):
        # Copy only the file for the required target and toolchain
        lib_builds = []
        # Create the configuration object
        if isinstance(prj_paths, basestring):
            prj_paths = [prj_paths]
        config = Config(self.target, prj_paths)
        for src in ['lib', 'src']:
            resources = self.__scan_and_copy(join(prj_paths[0], src), trg_path)
            for path in prj_paths[1:]:
                resources.add(self.__scan_and_copy(join(path, src), trg_path))

            lib_builds.extend(resources.lib_builds)

            # The repository files
            #for repo_dir in resources.repo_dirs:
            #    repo_files = self.__scan_all(repo_dir)
            #    for path in prj_paths:
            #        self.toolchain.copy_files(repo_files, trg_path, rel_path=join(path, src))

        # The libraries builds
        for bld in lib_builds:
            build_url = open(bld).read().strip()
            lib_data = self.build_url_resolver(build_url)
            lib_path = lib_data['path'].rstrip('\\/')
            self.__scan_and_copy(lib_path, join(trg_path, lib_data['name']))

            # Create .hg dir in mbed build dir so it's ignored when versioning
            hgdir = join(trg_path, lib_data['name'], '.hg')
            mkdir(hgdir)
            fhandle = file(join(hgdir, 'keep.me'), 'a')
            fhandle.close()

        if not relative:
            # Final scan of the actual exported resources
            resources = self.toolchain.scan_resources(trg_path)
            resources.relative_to(trg_path, self.DOT_IN_RELATIVE_PATH)
        else:
            # use the prj_dir (source, not destination)
            resources = self.toolchain.scan_resources(prj_paths[0])
            for path in prj_paths[1:]:
                resources.add(toolchain.scan_resources(path))

        # Loads the resources into the config system which might expand/modify resources based on config data
        self.resources = config.load_resources(resources)

        if hasattr(self, "MBED_CONFIG_HEADER_SUPPORTED") and self.MBED_CONFIG_HEADER_SUPPORTED :
            # Add the configuration file to the target directory
            self.config_header = self.toolchain.MBED_CONFIG_FILE_NAME
            config.get_config_data_header(join(trg_path, self.config_header))
            self.config_macros = []
            self.resources.inc_dirs.append(".")
        else:
            # And add the configuration macros to the toolchain
            self.config_macros = config.get_config_data_macros()

    def gen_file(self, template_file, data, target_file):
        template_path = join(Exporter.TEMPLATE_DIR, template_file)
        template = self.jinja_environment.get_template(template_file)
        target_text = template.render(data)

        target_path = join(self.inputDir, target_file)
        logging.debug("Generating: %s" % target_path)
        open(target_path, "w").write(target_text)

    def get_symbols(self, add_extra_symbols=True):
        """ This function returns symbols which must be exported.
            Please add / overwrite symbols in each exporter separately
        """

        # We have extra symbols from e.g. libraries, we want to have them also added to export
        extra = self.extra_symbols if add_extra_symbols else []
        if hasattr(self, "MBED_CONFIG_HEADER_SUPPORTED") and self.MBED_CONFIG_HEADER_SUPPORTED:
            # If the config header is supported, we will preinclude it and do not not
            # need the macros as preprocessor flags
            return extra

        symbols = self.toolchain.get_symbols(True) + self.toolchain.get_symbols() \
                  + self.config_macros + extra
        return symbols

def zip_working_directory_and_clean_up(tempdirectory=None, destination=None, program_name=None, clean=True):
    uid = str(uuid.uuid4())
    zipfilename = '%s.zip'%uid

    logging.debug("Zipping up %s to %s" % (tempdirectory,  join(destination, zipfilename)))
    # make zip
    def zipdir(basedir, archivename):
        assert isdir(basedir)
        fakeroot = program_name + '/'
        with closing(ZipFile(archivename, "w", ZIP_DEFLATED)) as z:
            for root, _, files in os.walk(basedir):
                # NOTE: ignore empty directories
                for fn in files:
                    absfn = join(root, fn)
                    zfn = fakeroot + '/' +  absfn[len(basedir)+len(os.sep):]
                    z.write(absfn, zfn)

    zipdir(tempdirectory, join(destination, zipfilename))

    if clean:
        shutil.rmtree(tempdirectory)

    return join(destination, zipfilename)
