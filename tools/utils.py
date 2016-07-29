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
"""
import sys
import inspect
import os
import argparse
import math
from os import listdir, remove, makedirs
from shutil import copyfile
from os.path import isdir, join, exists, split, relpath, splitext, abspath, commonprefix, normpath
from subprocess import Popen, PIPE, STDOUT, call
import json
from collections import OrderedDict
import logging

def compile_worker(job):
    results = []
    for command in job['commands']:
        try:
            _, _stderr, _rc = run_cmd(command, work_dir=job['work_dir'], chroot=job['chroot'])
        except KeyboardInterrupt as e:
            raise ToolException

        results.append({
            'code': _rc,
            'output': _stderr,
            'command': command
        })

    return {
        'source': job['source'],
        'object': job['object'],
        'commands': job['commands'],
        'results': results
    }

def cmd(l, check=True, verbose=False, shell=False, cwd=None):
    text = l if shell else ' '.join(l)
    if verbose:
        print text
    rc = call(l, shell=shell, cwd=cwd)
    if check and rc != 0:
        raise Exception('ERROR %d: "%s"' % (rc, text))


def run_cmd(command, work_dir=None, chroot=None, redirect=False):
    if chroot:
        # Conventions managed by the web team for the mbed.org build system
        chroot_cmd = [
            '/usr/sbin/chroot', '--userspec=33:33', chroot
        ]
        for c in command:
            chroot_cmd += [c.replace(chroot, '')]

        logging.debug("Running command %s"%' '.join(chroot_cmd))
        command = chroot_cmd
        work_dir = None

    try:
        p = Popen(command, stdout=PIPE, stderr=STDOUT if redirect else PIPE, cwd=work_dir)
        _stdout, _stderr = p.communicate()
    except OSError as e:
        print "[OS ERROR] Command: "+(' '.join(command))
        raise

    return _stdout, _stderr, p.returncode


def run_cmd_ext(command):
    assert is_cmd_valid(command[0])
    p = Popen(command, stdout=PIPE, stderr=PIPE)
    _stdout, _stderr = p.communicate()
    return _stdout, _stderr, p.returncode


def is_cmd_valid(cmd):
    caller = get_caller_name()
    abspath = find_cmd_abspath(cmd)
    if not abspath:
        error("%s: Command '%s' can't be found" % (caller, cmd))
    if not is_exec(abspath):
        error("%s: Command '%s' resolves to file '%s' which is not executable" % (caller, cmd, abspath))
    return True


def is_exec(path):
    return os.access(path, os.X_OK) or os.access(path+'.exe', os.X_OK)


def find_cmd_abspath(cmd):
    """ Returns the absolute path to a command.
        None is returned if no absolute path was found.
    """
    if exists(cmd) or exists(cmd + '.exe'):
        return os.path.abspath(cmd)
    if not 'PATH' in os.environ:
        raise Exception("Can't find command path for current platform ('%s')" % sys.platform)
    PATH=os.environ['PATH']
    for path in PATH.split(os.pathsep):
        abspath = '%s/%s' % (path, cmd)
        if exists(abspath) or exists(abspath + '.exe'):
            return abspath


def mkdir(path):
    if not exists(path):
        makedirs(path)


def copy_file(src, dst):
    """ Implement the behaviour of "shutil.copy(src, dst)" without copying the
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


def get_caller_name(steps=2):
    """
    When called inside a function, it returns the name
    of the caller of that function.
    """
    return inspect.stack()[steps][3]


def error(msg):
    print("ERROR: %s" % msg)
    sys.exit(1)


def rel_path(path, base, dot=False):
    p = relpath(path, base)
    if dot and not p.startswith('.'):
        p = './' + p
    return p


class ToolException(Exception):
    pass

class NotSupportedException(Exception):
    pass

class InvalidReleaseTargetException(Exception):
    pass

def split_path(path):
    base, file = split(path)
    name, ext = splitext(file)
    return base, name, ext


def get_path_depth(path):
    """ Given a path, return the number of directory levels present.
        This roughly translates to the number of path separators (os.sep) + 1.
        Ex. Given "path/to/dir", this would return 3
        Special cases: "." and "/" return 0
    """
    normalized_path = normpath(path)
    path_depth = 0
    head, tail = split(normalized_path)

    while(tail and tail != '.'):
        path_depth += 1
        head, tail = split(head)

    return path_depth


def args_error(parser, message):
    print "\n\n%s\n\n" % message
    parser.print_help()
    sys.exit()


def construct_enum(**enums):
    """ Create your own pseudo-enums """
    return type('Enum', (), enums)


def check_required_modules(required_modules, verbose=True):
    """ Function checks for Python modules which should be "importable" (installed)
        before test suite can be used.
        @return returns True if all modules are installed already
    """
    import imp
    not_installed_modules = []
    for module_name in required_modules:
        try:
            imp.find_module(module_name)
        except ImportError as e:
            # We also test against a rare case: module is an egg file
            try:
                __import__(module_name)
            except ImportError as e:
                not_installed_modules.append(module_name)
                if verbose:
                    print "Error: %s" % e

    if verbose:
        if not_installed_modules:
            print "Warning: Module(s) %s not installed. Please install required module(s) before using this script."% (', '.join(not_installed_modules))

    if not_installed_modules:
        return False
    else:
        return True

# Utility function: traverse a dictionary and change all the strings in the dictionary to
# ASCII from Unicode. Useful when reading ASCII JSON data, because the JSON decoder always
# returns Unicode string.
# Based on http://stackoverflow.com/a/13105359
def dict_to_ascii(input):
    if isinstance(input, dict):
        return OrderedDict([(dict_to_ascii(key), dict_to_ascii(value)) for key, value in input.iteritems()])
    elif isinstance(input, list):
        return [dict_to_ascii(element) for element in input]
    elif isinstance(input, unicode):
        return input.encode('ascii')
    else:
        return input

# Read a JSON file and return its Python representation, transforming all the strings from Unicode
# to ASCII. The order of keys in the JSON file is preserved.
def json_file_to_dict(fname):
    try:
        with open(fname, "rt") as f:
            return dict_to_ascii(json.load(f, object_pairs_hook=OrderedDict))
    except (ValueError, IOError):
        sys.stderr.write("Error parsing '%s':\n" % fname)
        raise

# Wowza, double closure
def argparse_type(casedness, prefer_hyphen=False) :
    def middle(list, type_name):
        # validate that an argument passed in (as string) is a member of the list of possible
        # arguments. Offer a suggestion if the case of the string, or the hyphens/underscores
        # do not match the expected style of the argument.
        def parse_type(string):
            if prefer_hyphen: newstring = casedness(string).replace("_","-")
            else:             newstring = casedness(string).replace("-","_")
            if string in list:
                return string
            elif string not in list and newstring in list:
                raise argparse.ArgumentTypeError("{0} is not a supported {1}. Did you mean {2}?".format(string, type_name, newstring))
            else:
                raise argparse.ArgumentTypeError("{0} is not a supported {1}. Supported {1}s are:\n{2}".format(string, type_name, columnate(list)))
        return parse_type
    return middle

# short cuts for the argparse_type versions
argparse_uppercase_type = argparse_type(str.upper, False)
argparse_lowercase_type = argparse_type(str.lower, False)
argparse_uppercase_hyphen_type = argparse_type(str.upper, True)
argparse_lowercase_hyphen_type = argparse_type(str.lower, True)

def argparse_force_type(case):
    def middle(list, type_name):
        # validate that an argument passed in (as string) is a member of the list of possible
        # arguments after converting it's case. Offer a suggestion if the hyphens/underscores
        # do not match the expected style of the argument.
        def parse_type(string):
            for option in list:
                if case(string) == case(option):
                    return option
            raise argparse.ArgumentTypeError("{0} is not a supported {1}. Supported {1}s are:\n{2}".format(string, type_name, columnate(list)))
        return parse_type
    return middle

# these two types convert the case of their arguments _before_ validation
argparse_force_uppercase_type = argparse_force_type(str.upper)
argparse_force_lowercase_type = argparse_force_type(str.lower)

# An argument parser combinator that takes in an argument parser and creates a new parser that
# accepts a comma separated list of the same thing.
def argparse_many(fn):
    def wrap(string):
        return [fn(s) for s in string.split(",")]
    return wrap

# An argument parser that verifies that a string passed in corresponds to a file
def argparse_filestring_type(string) :
    if exists(string) :
        return string
    else :
        raise argparse.ArgumentTypeError("{0}"" does not exist in the filesystem.".format(string))

# render a list of strings as a in a bunch of columns
def columnate(strings, seperator=", ", chars=80):
    col_width = max(len(s) for s in strings)
    total_width = col_width + len(seperator)
    columns = math.floor(chars / total_width)
    output = ""
    for i, s in zip(range(len(strings)), strings):
        append = s
        if i != len(strings) - 1:
            append += seperator
        if i % columns == columns - 1:
            append += "\n"
        else:
            append = append.ljust(total_width)
        output += append
    return output

# fail if argument provided is a parent of the specified directory
def argparse_dir_not_parent(other):
    def parse_type(not_parent):
        abs_other = abspath(other)
        abs_not_parent = abspath(not_parent)
        if abs_not_parent == commonprefix([abs_not_parent, abs_other]):
            raise argparse.ArgumentTypeError("{0} may not be a parent directory of {1}".format(not_parent, other))
        else:
            return not_parent
    return parse_type
