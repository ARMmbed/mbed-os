"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


One repository to update them all
On mbed.org the mbed SDK is split up in multiple repositories, this script takes
care of updating them all.
"""
import sys
from copy import copy
from os import walk, remove, makedirs
from os.path import join, abspath, dirname, relpath, exists, splitext
from shutil import copyfile
from optparse import OptionParser

ROOT = abspath(join(dirname(__file__), ".."))
sys.path.append(ROOT)

from workspace_tools.settings import MBED_ORG_PATH, MBED_ORG_USER, BUILD_DIR
from workspace_tools.paths import LIB_DIR
from workspace_tools.utils import cmd, run_cmd


MBED_URL = "mbed.org"
# MBED_URL = "world2.dev.mbed.org"

MBED_REPO_EXT = (".lib", ".bld")

# mbed_official code that does have a mirror in the mbed SDK
OFFICIAL_CODE = ( 
    ("mbed-src" , "mbed"),
    ("mbed-rtos", "rtos"),
    ("mbed-dsp" , "dsp"),
    ("mbed-rpc" , "rpc"),
    
    ("lwip"    , "net/lwip/lwip"),
    ("lwip-sys", "net/lwip/lwip-sys"),
    ("Socket"  , "net/lwip/Socket"),
    
    ("lwip-eth"         , "net/eth/lwip-eth"),
    ("EthernetInterface", "net/eth/EthernetInterface"),
    
    ("USBDevice", "USBDevice"),
    ("USBHost"  , "USBHost"),
)


# mbed_official code that does have dependencies to libraries should point to
# the latest revision. By default, they point to a specific revision.
CODE_WITH_DEPENDENCIES = (
    # Libraries
    "EthernetInterface",
    
    # RTOS Examples
    "rtos_basic",
    "rtos_isr",
    "rtos_mail",
    "rtos_mutex",
    "rtos_queue",
    "rtos_semaphore",
    "rtos_signals",
    "rtos_timer",
    
    # Net Examples
    "TCPEchoClient",
    "TCPEchoServer",
    "TCPSocket_HelloWorld",
    "UDPSocket_HelloWorld",
    "UDPEchoClient",
    "UDPEchoServer",
    "BroadcastReceive",
    "BroadcastSend",
    
    # mbed sources
    "mbed-src-program",
)


class MbedOfficialRepository:
    URL = "http://" + MBED_URL + "/users/mbed_official/code/%s/"
    
    def __init__(self, name):
        self.name = name
        self.path = join(MBED_ORG_PATH, name)
        
        if not exists(self.path):
            # Checkout code
            if not exists(MBED_ORG_PATH):
                makedirs(MBED_ORG_PATH)
            
            cmd(['hg', 'clone', MbedOfficialRepository.URL % name], cwd=MBED_ORG_PATH)
        
        else:
            # Update
            cmd(['hg', 'pull'], cwd=self.path)
            cmd(['hg', 'update'], cwd=self.path)
    
    def publish(self):
        # The maintainer has to evaluate the changes first and explicitly accept them
        cmd(['hg', 'addremove'], cwd=self.path)
        stdout, _, _ = run_cmd(['hg', 'status'], wd=self.path)
        if stdout == '':
            print "No changes"
            return
        
        print stdout
        commit = raw_input("Do you want to commit and push? Y/N: ")
        if commit == 'Y':
            cmd(['hg', 'commit', '-u', MBED_ORG_USER], cwd=self.path)
            cmd(['hg', 'push'], cwd=self.path)


def visit_files(path, visit, ignore=None, select=None):
    for root, dirs, files in walk(path):
        # Ignore hidden directories
        for d in copy(dirs):
            if d.startswith('.'):
                dirs.remove(d)
        
        for file in files:
            ext = splitext(file)[1]
            
            if ignore is not None:
                if ext in ignore: continue
            
            if select is not None:
                if ext not in select: continue
            
            visit(join(root, file))


def update_repo(repo_name, sdk_path):
    repo = MbedOfficialRepository(repo_name)
    # copy files from mbed SDK to mbed_official repository
    def visit_mbed_sdk(sdk_file):
        repo_file = join(repo.path, relpath(sdk_file, sdk_path))
        
        repo_dir = dirname(repo_file)
        if not exists(repo_dir):
            makedirs(repo_dir)
        
        copyfile(sdk_file, repo_file)
    visit_files(sdk_path, visit_mbed_sdk, ['.json'])
    
    # remove repository files that do not exist in the mbed SDK
    def visit_repo(repo_file):
        sdk_file = join(sdk_path, relpath(repo_file, repo.path))
        if not exists(sdk_file):
            remove(repo_file)
            print "remove: %s" % repo_file
    visit_files(repo.path, visit_repo, MBED_REPO_EXT)
    
    repo.publish()


def update_code(repositories):
    for repo_name, sdk_dir in repositories:
        print '\n=== Updating "%s" ===' % repo_name
        sdk_path = join(LIB_DIR, sdk_dir)
        update_repo(repo_name, sdk_path)


def update_dependencies(repositories):
    for repo_name in repositories:
        print '\n=== Updating "%s" ===' % repo_name
        repo = MbedOfficialRepository(repo_name)
        
        # point to the latest libraries
        def visit_repo(repo_file):
            with open(repo_file, "r") as f:
                url = f.read()
            with open(repo_file, "w") as f:
                f.write(url[:(url.rindex('/')+1)])
        visit_files(repo.path, visit_repo, None, MBED_REPO_EXT)
        
        repo.publish()


def update_mbed():
    update_repo("mbed", join(BUILD_DIR, "mbed"))


if __name__ == '__main__':
    parser = OptionParser()
    
    parser.add_option("-c", "--code",
                  action="store_true",  default=False,
                  help="Update the mbed_official code")
    
    parser.add_option("-d", "--dependencies",
                  action="store_true",  default=False,
                  help="Update the mbed_official code dependencies")
    
    parser.add_option("-m", "--mbed",
                  action="store_true",  default=False,
                  help="Release a build of the mbed library")
    
    (options, args) = parser.parse_args()
    
    if options.code:
        update_code(OFFICIAL_CODE)
    
    if options.dependencies:
        update_dependencies(CODE_WITH_DEPENDENCIES)
    
    if options.mbed:
        update_mbed()

