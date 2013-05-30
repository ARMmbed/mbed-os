""" One repository to update them all
On mbed.org the mbed SDK is split up in multiple repositories, this script takes
care of updating them all.
"""
import sys
from copy import copy
from os import walk, remove, makedirs
from os.path import join, abspath, dirname, relpath, exists, splitext
from shutil import copyfile

ROOT = abspath(join(dirname(__file__), ".."))
sys.path.append(ROOT)

from workspace_tools.settings import MBED_ORG_PATH, MBED_ORG_USER
from workspace_tools.paths import LIB_DIR
from workspace_tools.utils import cmd


MBED_OFFICIAL = "http://217.140.101.22/users/mbed_official/code/%s/"

REPOSITORIES = ( 
    ("mbed-src"      , "mbed/src"),
    ("mbed-NXP"      , "mbed/vendor/NXP"),
    ("mbed-Freescale", "mbed/vendor/Freescale"),
    ("mbed-rpc"      , "mbed/rpc"),
    
    ("mbed-rtos", "rtos"),
    ("mbed-dsp" , "dsp"),
    
    ("lwip"    , "net/lwip/lwip"),
    ("lwip-sys", "net/lwip/lwip-sys"),
    ("Socket"  , "net/lwip/Socket"),
    
    ("lwip-eth"         , "net/eth/lwip-eth"),
    ("EthernetInterface", "net/eth/EthernetInterface"),
    
    ("USBDevice", "USBDevice"),
    ("USBHost", "USBHost"),
)


def synch_repositories(repositories):
    if not exists(MBED_ORG_PATH):
        makedirs(MBED_ORG_PATH)
    
    for remote, local in repositories:
        print '\n=== Updating "%s" ===' % remote
        remote_path = join(MBED_ORG_PATH, remote)
        local_path = join(LIB_DIR, local)
        
        # checkout remote repository
        if not exists(remote_path):
            cmd(['hg', 'clone', MBED_OFFICIAL % remote], cwd=MBED_ORG_PATH)
        
        # copy files from local to remote
        for root, dirs, files in walk(local_path):
            for d in copy(dirs):
                if d.startswith('.'):
                    dirs.remove(d)
            
            for file in files:
                if splitext(file)[1] == '.json': continue
                
                local_file = join(root, file)
                rel_path = relpath(local_file, local_path)
                remote_file = join(remote_path, rel_path)
                
                remote_dir = dirname(remote_file)
                if not exists(remote_dir):
                    makedirs(remote_dir)
                
                copyfile(local_file, remote_file)
        
        # remove remote files that do not exist in local
        for root, dirs, files in walk(remote_path):
            for d in copy(dirs):
                if d.startswith('.'):
                    dirs.remove(d)
            
            for file in files:
                if splitext(file)[1] == '.lib': continue
                
                remote_file = join(root, file)
                rel_path = relpath(remote_file, remote_path)
                local_file = join(local_path, rel_path)
                if not exists(local_file):
                    remove(remote_file)
                    print "remove: %s" % remote_file
        
        # Actual Mercurial synch
        cmd(['hg', 'addremove'], cwd=remote_path)
        cmd(['hg', 'status'], verbose=True, cwd=remote_path)
        commit = raw_input("Do you want to commit and push? Y/N: ")
        if commit == 'Y':
            cmd(['hg', 'commit', '-u', MBED_ORG_USER], verbose=True, cwd=remote_path)
            cmd(['hg', 'push'], verbose=True, cwd=remote_path)


if __name__ == '__main__':
    synch_repositories(REPOSITORIES)
