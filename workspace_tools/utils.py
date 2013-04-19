import sys
from subprocess import Popen, PIPE, call
from os import listdir, remove, makedirs
from os.path import isdir, join, exists, split, relpath, splitext
from shutil import copyfile
import logging


def cmd(l, check=True, verbose=False, shell=False, cwd=None):
    text = l if shell else ' '.join(l)
    if verbose: print text
    
    rc = call(l, shell=shell, cwd=cwd)
    
    if check and rc != 0:
        raise Exception('ERROR %d: "%s"' % (rc, text))


def run_cmd(command, wd=None, chroot=None):
    if chroot:
        # Conventions managed by the web team for the mbed.org build system
        if not wd:
            wd = '/src'
        else:
            wd = wd.replace(chroot, '')
        orig_cmd = ' '.join([c.replace(chroot, '') for c in command])
        command = []
        command.append('/usr/sbin/chroot')
        command.append(chroot)
        command.append('/bin/run_v2.sh')
        command.append('%s'%wd)
        command.append('%s'%orig_cmd)
        logging.debug("Running command %s"%' '.join(command))
        p = Popen(command, stdout=PIPE, stderr=PIPE, cwd=chroot)
    else:
        p = Popen(command, stdout=PIPE, stderr=PIPE, cwd=wd)
    stdout, stderr = p.communicate()
    
    return stdout, stderr, p.returncode


def copy_file(src, dst):
    """
    Implement the behaviour of "shutil.copy(src, dst)" without copying the
    permissions (this was causing errors with directories mounted with samba)
    """
    if isdir(dst):
        _, file = split(src)
        dst = join(dst, file)
    copyfile(src, dst)


def delete_dir_files(dir):
    if not exists(dir):
        return
    
    for f in listdir(dir):
        file = join(dir, f)
        if not isdir(file):
            remove(file)


def mkdir(path):
    if not exists(path):
        makedirs(path)


def error(msg):
    print msg
    sys.exit(1)


def rel_path(path, base, dot=False):
    p = relpath(path, base)
    if dot and not p.startswith('.'):
        p = './' + p
    return p


class ToolException(Exception):
    pass


def split_path(path):
    base, file = split(path)
    name, ext = splitext(file)
    return base, name, ext
