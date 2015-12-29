"""Just a template for subclassing"""
import uuid, shutil, os, logging, fnmatch
from os import walk, remove
from os.path import join, dirname, isdir, split
from copy import copy
from jinja2 import Template, FileSystemLoader
from jinja2.environment import Environment
from contextlib import closing
from zipfile import ZipFile, ZIP_DEFLATED

from workspace_tools.utils import mkdir
from workspace_tools.toolchains import TOOLCHAIN_CLASSES
from workspace_tools.targets import TARGET_MAP

from project_generator.generate import Generator
from project_generator.project import Project
from project_generator.settings import ProjectSettings

class OldLibrariesException(Exception): pass

class Exporter(object):
    TEMPLATE_DIR = dirname(__file__)
    DOT_IN_RELATIVE_PATH = False

    # mapping mbed to progen targets
    PROGEN_TARGETS = {
        'LPC1768': 'lpc1768',
        'LPC11U24': 'lpc11u24_201',
        'LPC1347':'lpc1347',
        'LPC1114':'lpc1114_102',
        'LPC11C24':'lpc11c24_301',
        'LPC4088':'lpc4088',
        'LPC4088_DM':'lpc4088',
        'LPC4330_M4':'lpc4330',
        'LPC4337':'lpc4337',
        'LPC812':'lpc812m101',
        'LPC1549':'lpc1549',
        'LPC11U68':'lpc11u68',
        'LPC11U35_501':'lpc11u35_501',
        'LPC11U37H_401':'lpc11u37_401',

        'KL05Z':'mkl05z32xxx4',
        'KL25Z':'mkl25z128xxx4',
        'KL43Z':'mkl43z256xxx4',
        'KL46Z':'mkl46z256xxx4',
        'K64F': 'mk64fn1m0xxx12',
        'K22F': 'mk22dn512xxx5',
        'K20D50M': 'frdm-k20d50m',
        'TEENSY3_1':'mk20dx256xxx7',

        'NUCLEO_F030R8':'nucleo-f030r8',
        'NUCLEO_F031K6':'nucleo-f031k6',
        'NUCLEO_F042K6':'nucleo-f042k6',
        'NUCLEO_F070RB':'nucleo-f070rb',
        'NUCLEO_F072RB':'nucleo-f072rb',
        'NUCLEO_F091RC':'nucleo-f091rc',
        'NUCLEO_F103RB':'nucleo-f103rb',
        'NUCLEO_F302R8':'nucleo-f302r8',
        'NUCLEO_F303K8':'nucleo-f303k8',
        'NUCLEO_F303RE':'nucleo-f303re',
        'NUCLEO_F334R8':'nucleo-f334r8',
        'NUCLEO_F401RE':'nucleo-f401re',
        'NUCLEO_F410RB':'nucleo-f410rb',
        'NUCLEO_F411RE':'nucleo-f411re',
        'NUCLEO_F446RE':'nucleo-f446re',
        'NUCLEO_L053R8':'nucleo-l053r8',
        'NUCLEO_L073RZ':'nucleo-l073rz',
        'NUCLEO_L152RE':'nucleo-l152re',
        'NUCLEO_L476RG':'nucleo-l476rg',

        'DISCO_F407VG':'stm32f407vg',
        'DISCO_F429ZI':'disco-f429zi',
        'DISCO_L053C8':'disco-l053c8',
        'DISCO_F334C8':'disco-f334c8',
        'DISCO_F469NI':'disco-f469ni',
        'DISCO_F746NG':'disco-f746ng',
        'DISCO_L476VG':'disco-l476vg',

        'UBLOX_C027':'ublox-c027',
        'NRF51822':'mkit',
        'HRM1017':'hrm1017',
        'RBLAB_NRF51822':'nrf51',
        'ARCH_PRO':'arch-pro',
        'ARCH_BLE':'arch-ble',
        'MTS_GAMBIT':'mts-gambit',
        'ARCH_MAX':'lpc1768',
        'MTS_MDOT_F405RG':'stm32f405rg',
        'NRF51_DK':'nrf51',
        'NRF51_DONGLE':'nrf51',
        'SEEED_TINY_BLE':'nrf51',

        'DELTA_DFCM_NNN40':'dfcm-nnn40',
        'MAXWSNENV':'maxwsnenv',
        'MAX32600MBED':'max32600mbed',
        'MOTE_L152RC':'stm32l151rc',
        'NZ32SC151':'stm32l151rc',

        'SAMR21G18A':'samr21g18a',
        'SAMD21J18A':'samd21j18a',
        'SAMD21G18A':'samd21g18a',
        'SAML21J18A':'samr21j18a',
    }

    def __init__(self, target, inputDir, program_name, build_url_resolver, extra_symbols=None):
        self.inputDir = inputDir
        self.target = target
        self.program_name = program_name
        self.toolchain = TOOLCHAIN_CLASSES[self.get_toolchain()](TARGET_MAP[target])
        self.build_url_resolver = build_url_resolver
        jinja_loader = FileSystemLoader(os.path.dirname(os.path.abspath(__file__)))
        self.jinja_environment = Environment(loader=jinja_loader)
        self.extra_symbols = extra_symbols

    def get_toolchain(self):
        return self.TOOLCHAIN

    def __scan_and_copy(self, src_path, trg_path):
        resources = self.toolchain.scan_resources(src_path)

        for r_type in ['headers', 's_sources', 'c_sources', 'cpp_sources',
            'objects', 'libraries', 'linker_script',
            'lib_builds', 'lib_refs', 'repo_files', 'hex_files', 'bin_files']:
            r = getattr(resources, r_type)
            if r:
                self.toolchain.copy_files(r, trg_path, rel_path=src_path)
        return resources

    def get_project_data(self):
        """ Get ProGen project data  """
        # provide default data, some tools don't require any additional
        # tool specific settings
        sources = []
        for r_type in ['c_sources', 'cpp_sources', 's_sources']:
            for file in getattr(self.resources, r_type):
                sources.append(file)

        # TODO: sources in groups
        project_data = {
            'common': {
                'sources': sources + self.resources.hex_files + 
                    self.resources.objects + self.resources.libraries,
                # 'includes': self.resources.inc_dirs,
                'target': [self.PROGEN_TARGETS[self.target]],
                'debugger': ['cmsis-dap'], #TODO: per target to set this
                'macros': self.get_symbols(),
                'export_dir': [self.inputDir],
                'linker_file': [self.resources.linker_script],
                'template': [''],
            }
        }
        return project_data

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

    def scan_and_copy_resources(self, prj_path, trg_path):
        # Copy only the file for the required target and toolchain
        lib_builds = []
        for src in ['lib', 'src']:
            resources = self.__scan_and_copy(join(prj_path, src), trg_path)
            lib_builds.extend(resources.lib_builds)

            # The repository files
            for repo_dir in resources.repo_dirs:
                repo_files = self.__scan_all(repo_dir)
                self.toolchain.copy_files(repo_files, trg_path, rel_path=join(prj_path, src))

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

        # Final scan of the actual exported resources
        self.resources = self.toolchain.scan_resources(trg_path)
        self.resources.relative_to(trg_path, self.DOT_IN_RELATIVE_PATH)
        # Check the existence of a binary build of the mbed library for the desired target
        # This prevents exporting the mbed libraries from source
        # if not self.toolchain.mbed_libs:
        #    raise OldLibrariesException()

    def gen_file_progen(self, tool_name, project_data):
        """" Generate project using ProGen Project API """
        settings = ProjectSettings()
        project = Project(self.program_name, [project_data], settings)
        # TODO: fix
        # this hack is for these scripts which know too much
        project.project['common']['include_paths'] = self.resources.inc_dirs
        project.generate(tool_name, copied=True)

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
        symbols = self.toolchain.get_symbols()
        # We have extra symbols from e.g. libraries, we want to have them also added to export
        if add_extra_symbols:
            if self.extra_symbols is not None:
                symbols.extend(self.extra_symbols)
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
