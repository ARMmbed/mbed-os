"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited
SPDX-License-Identifier: Apache-2.0

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
from __future__ import print_function, division, absolute_import
import sys
import inspect
import os
import argparse
import math
from os import listdir, remove, makedirs
from shutil import copyfile
from os.path import isdir, join, exists, split, relpath, splitext, abspath
from os.path import commonprefix, normpath, dirname
from subprocess import Popen, PIPE, STDOUT, call
from math import ceil
import json
from collections import OrderedDict
import logging
from intelhex import IntelHex
import io

try:
    unicode
except NameError:
    unicode = str

def remove_if_in(lst, thing):
    if thing in lst:
        lst.remove(thing)

def compile_worker(job):
    """Standard task runner used for compiling

    Positional argumets:
    job - a dict containing a list of commands and the remaining arguments
          to run_cmd
    """
    results = []
    for command in job['commands']:
        try:
            _, _stderr, _rc = run_cmd(command, work_dir=job['work_dir'],
                                      chroot=job['chroot'])
        except KeyboardInterrupt:
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

def cmd(command, check=True, verbose=False, shell=False, cwd=None):
    """A wrapper to run a command as a blocking job"""
    text = command if shell else ' '.join(command)
    if verbose:
        print(text)
    return_code = call(command, shell=shell, cwd=cwd)
    if check and return_code != 0:
        raise Exception('ERROR %d: "%s"' % (return_code, text))


def run_cmd(command, work_dir=None, chroot=None, redirect=False):
    """Run a command in the foreground

    Positional arguments:
    command - the command to run

    Keyword arguments:
    work_dir - the working directory to run the command in
    chroot - the chroot to run the command in
    redirect - redirect the stderr to a pipe to be used later
    """
    if chroot:
        # Conventions managed by the web team for the mbed.org build system
        chroot_cmd = [
            '/usr/sbin/chroot', '--userspec=33:33', chroot
        ]
        for element in command:
            chroot_cmd += [element.replace(chroot, '')]

        logging.debug("Running command %s", ' '.join(chroot_cmd))
        command = chroot_cmd
        work_dir = None

    try:
        process = Popen(command, stdout=PIPE,
                        stderr=STDOUT if redirect else PIPE, cwd=work_dir,
                        universal_newlines=True)
        _stdout, _stderr = process.communicate()
    except OSError:
        print("[OS ERROR] Command: "+(' '.join(command)))
        raise

    return _stdout, _stderr, process.returncode


def run_cmd_ext(command):
    """ A version of run command that checks if the command exists befor running

    Positional arguments:
    command - the command line you are trying to invoke
    """
    assert is_cmd_valid(command[0])
    process = Popen(command, stdout=PIPE, stderr=PIPE)
    _stdout, _stderr = process.communicate()
    return _stdout, _stderr, process.returncode


def is_cmd_valid(command):
    """ Verify that a command exists and is executable

    Positional arguments:
    command - the command to check
    """
    caller = get_caller_name()
    cmd_path = find_cmd_abspath(command)
    if not cmd_path:
        error("%s: Command '%s' can't be found" % (caller, command))
    if not is_exec(cmd_path):
        error("%s: Command '%s' resolves to file '%s' which is not executable"
              % (caller, command, cmd_path))
    return True


def is_exec(path):
    """A simple check to verify that a path to an executable exists

    Positional arguments:
    path - the executable
    """
    return os.access(path, os.X_OK) or os.access(path+'.exe', os.X_OK)


def find_cmd_abspath(command):
    """ Returns the absolute path to a command.
        None is returned if no absolute path was found.

    Positional arguhments:
    command - the command to find the path of
    """
    if exists(command) or exists(command + '.exe'):
        return os.path.abspath(command)
    if not 'PATH' in os.environ:
        raise Exception("Can't find command path for current platform ('%s')"
                        % sys.platform)
    path_env = os.environ['PATH']
    for path in path_env.split(os.pathsep):
        cmd_path = '%s/%s' % (path, command)
        if exists(cmd_path) or exists(cmd_path + '.exe'):
            return cmd_path


def mkdir(path):
    """ a wrapped makedirs that only tries to create a directory if it does not
    exist already

    Positional arguments:
    path - the path to maybe create
    """
    if not exists(path):
        makedirs(path)


def write_json_to_file(json_data, file_name):
    """
    Write json content in file
    :param json_data:
    :param file_name:
    :return:
    """
    # Create the target dir for file if necessary
    test_spec_dir = os.path.dirname(file_name)

    if test_spec_dir:
        mkdir(test_spec_dir)

    try:
        with open(file_name, 'w') as f:
            f.write(json.dumps(json_data, indent=2))
    except IOError as e:
        print("[ERROR] Error writing test spec to file")
        print(e)


def copy_file(src, dst):
    """ Implement the behaviour of "shutil.copy(src, dst)" without copying the
    permissions (this was causing errors with directories mounted with samba)

    Positional arguments:
    src - the source of the copy operation
    dst - the destination of the copy operation
    """
    if isdir(dst):
        _, base = split(src)
        dst = join(dst, base)
    copyfile(src, dst)


def copy_when_different(src, dst):
    """ Only copy the file when it's different from its destination.

    Positional arguments:
    src - the source of the copy operation
    dst - the destination of the copy operation
    """
    if isdir(dst):
        _, base = split(src)
        dst = join(dst, base)
    if exists(dst):
        with open(src, 'rb') as srcfd, open(dst, 'rb') as dstfd:
            if srcfd.read() == dstfd.read():
                return
    copyfile(src, dst)


def delete_dir_files(directory):
    """ A function that does rm -rf

    Positional arguments:
    directory - the directory to remove
    """
    if not exists(directory):
        return

    for element in listdir(directory):
        to_remove = join(directory, element)
        if not isdir(to_remove):
            remove(to_remove)


def get_caller_name(steps=2):
    """
    When called inside a function, it returns the name
    of the caller of that function.

    Keyword arguments:
    steps - the number of steps up the stack the calling function is
    """
    return inspect.stack()[steps][3]


def error(msg):
    """Fatal error, abort hard

    Positional arguments:
    msg - the message to print before crashing
    """
    print("ERROR: %s" % msg)
    sys.exit(1)


def rel_path(path, base, dot=False):
    """Relative path calculation that optionaly always starts with a dot

    Positional arguments:
    path - the path to make relative
    base - what to make the path relative to

    Keyword arguments:
    dot - if True, the path will always start with a './'
    """
    final_path = relpath(path, base)
    if dot and not final_path.startswith('.'):
        final_path = './' + final_path
    return final_path


class ToolException(Exception):
    """A class representing an exception throw by the tools"""
    pass

class NotSupportedException(Exception):
    """A class a toolchain not supporting a particular target"""
    pass

class InvalidReleaseTargetException(Exception):
    pass

class NoValidToolchainException(Exception):
    """A class representing no valid toolchain configurations found on
    the system"""
    pass

def split_path(path):
    """spilt a file name into it's directory name, base name, and extension

    Positional arguments:
    path - the file name to split
    """
    base, has_ext = split(path)
    name, ext = splitext(has_ext)
    return base, name, ext


def get_path_depth(path):
    """ Given a path, return the number of directory levels present.
        This roughly translates to the number of path separators (os.sep) + 1.
        Ex. Given "path/to/dir", this would return 3
        Special cases: "." and "/" return 0

    Positional arguments:
    path - the path to calculate the depth of
    """
    normalized_path = normpath(path)
    path_depth = 0
    head, tail = split(normalized_path)

    while tail and tail != '.':
        path_depth += 1
        head, tail = split(head)

    return path_depth


def args_error(parser, message):
    """Abort with an error that was generated by the arguments to a CLI program

    Positional arguments:
    parser - the ArgumentParser object that parsed the command line
    message - what went wrong
    """
    parser.exit(status=2, message=message+'\n')


def construct_enum(**enums):
    """ Create your own pseudo-enums

    Keyword arguments:
    * - a member of the Enum you are creating and it's value
    """
    return type('Enum', (), enums)


def check_required_modules(required_modules, verbose=True):
    """ Function checks for Python modules which should be "importable"
        before test suite can be used.
        @return returns True if all modules are installed already
    """
    import imp
    not_installed_modules = []
    for module_name in required_modules:
        try:
            imp.find_module(module_name)
        except ImportError:
            # We also test against a rare case: module is an egg file
            try:
                __import__(module_name)
            except ImportError as exc:
                not_installed_modules.append(module_name)
                if verbose:
                    print("Error: %s" % exc)

    if verbose:
        if not_installed_modules:
            print("Warning: Module(s) %s not installed. Please install "
                  "required module(s) before using this script."
                  % (', '.join(not_installed_modules)))

    if not_installed_modules:
        return False
    else:
        return True


def _ordered_dict_collapse_dups(pair_list):
    to_ret = OrderedDict()
    for key, value in pair_list:
        if key in to_ret:
            if isinstance(to_ret[key], dict):
                to_ret[key].update(value)
            elif isinstance(to_ret[key], list):
                to_ret[key].extend(value)
            else:
                raise ValueError(
                    "Key %s found twice and is not mergeable" % key
                )
        else:
            to_ret[key] = value
    return to_ret


def json_file_to_dict(fname):
    """ Read a JSON file and return its Python representation, transforming all
    the strings from Unicode to ASCII. The order of keys in the JSON file is
    preserved.

    Positional arguments:
    fname - the name of the file to parse
    """
    try:
        with io.open(fname, encoding='ascii',
                     errors='ignore') as file_obj:
            return json.load(
                file_obj,  object_pairs_hook=_ordered_dict_collapse_dups
            )
    except (ValueError, IOError) as e:
        sys.stderr.write("Error parsing '%s': %s\n" % (fname, e))
        raise

# Wowza, double closure
def argparse_type(casedness, prefer_hyphen=False):
    def middle(lst, type_name):
        def parse_type(string):
            """ validate that an argument passed in (as string) is a member of
            the list of possible arguments. Offer a suggestion if the case of
            the string, or the hyphens/underscores do not match the expected
            style of the argument.
            """
            if not isinstance(string, unicode):
                string = string.decode()
            if prefer_hyphen:
                newstring = casedness(string).replace("_", "-")
            else:
                newstring = casedness(string).replace("-", "_")
            if string in lst:
                return string
            elif string not in lst and newstring in lst:
                raise argparse.ArgumentTypeError(
                    "{0} is not a supported {1}. Did you mean {2}?".format(
                        string, type_name, newstring))
            else:
                raise argparse.ArgumentTypeError(
                    "{0} is not a supported {1}. Supported {1}s are:\n{2}".
                    format(string, type_name, columnate(lst)))
        return parse_type
    return middle

# short cuts for the argparse_type versions
argparse_uppercase_type = argparse_type(unicode.upper, False)
argparse_lowercase_type = argparse_type(unicode.lower, False)
argparse_uppercase_hyphen_type = argparse_type(unicode.upper, True)
argparse_lowercase_hyphen_type = argparse_type(unicode.lower, True)

def argparse_force_type(case):
    """ validate that an argument passed in (as string) is a member of the list
    of possible arguments after converting it's case.
    """
    def middle(lst, type_name):
        """ The parser type generator"""
        if not isinstance(lst[0], unicode):
            lst = [o.decode() for o in lst]
        def parse_type(string):
            """ The parser type"""
            if not isinstance(string, unicode):
                string = string.decode()
            for option in lst:
                if case(string) == case(option):
                    return option
            raise argparse.ArgumentTypeError(
                "{0} is not a supported {1}. Supported {1}s are:\n{2}".
                format(string, type_name, columnate(lst)))
        return parse_type
    return middle

# these two types convert the case of their arguments _before_ validation
argparse_force_uppercase_type = argparse_force_type(unicode.upper)
argparse_force_lowercase_type = argparse_force_type(unicode.lower)

def argparse_many(func):
    """ An argument parser combinator that takes in an argument parser and
    creates a new parser that accepts a comma separated list of the same thing.
    """
    def wrap(string):
        """ The actual parser"""
        return [func(s) for s in string.split(",")]
    return wrap

def argparse_filestring_type(string):
    """ An argument parser that verifies that a string passed in corresponds
    to a file"""
    if exists(string):
        return string
    else:
        raise argparse.ArgumentTypeError(
            "{0}"" does not exist in the filesystem.".format(string))

def argparse_profile_filestring_type(string):
    """ An argument parser that verifies that a string passed in is either
    absolute path or a file name (expanded to
    mbed-os/tools/profiles/<fname>.json) of a existing file"""
    fpath = join(dirname(__file__), "profiles/{}.json".format(string))

    # default profiles are searched first, local ones next.
    if exists(fpath):
        return fpath
    elif exists(string):
        return string
    else:
        raise argparse.ArgumentTypeError(
            "{0} does not exist in the filesystem.".format(string))

def columnate(strings, separator=", ", chars=80):
    """ render a list of strings as a in a bunch of columns

    Positional arguments:
    strings - the strings to columnate

    Keyword arguments;
    separator - the separation between the columns
    chars - the maximum with of a row
    """
    col_width = max(len(s) for s in strings)
    total_width = col_width + len(separator)
    columns = math.floor(chars / total_width)
    output = ""
    for i, string in zip(range(len(strings)), strings):
        append = string
        if i != len(strings) - 1:
            append += separator
        if i % columns == columns - 1:
            append += "\n"
        else:
            append = append.ljust(total_width)
        output += append
    return output

def argparse_dir_not_parent(other):
    """fail if argument provided is a parent of the specified directory"""
    def parse_type(not_parent):
        """The parser type"""
        abs_other = abspath(other)
        abs_not_parent = abspath(not_parent)
        if abs_not_parent == commonprefix([abs_not_parent, abs_other]):
            raise argparse.ArgumentTypeError(
                "{0} may not be a parent directory of {1}".format(
                    not_parent, other))
        else:
            return not_parent
    return parse_type

def argparse_deprecate(replacement_message):
    """fail if argument is provided with deprecation warning"""
    def parse_type(_):
        """The parser type"""
        raise argparse.ArgumentTypeError("Deprecated." + replacement_message)
    return parse_type

def print_large_string(large_string):
    """ Breaks a string up into smaller pieces before print them

    This is a limitation within Windows, as detailed here:
    https://bugs.python.org/issue11395

    Positional arguments:
    large_string - the large string to print
    """
    string_limit = 1000
    large_string_len = len(large_string)
    num_parts = int(ceil(float(large_string_len) / float(string_limit)))
    for string_part in range(num_parts):
        start_index = string_part * string_limit
        if string_part == num_parts - 1:
            sys.stdout.write(large_string[start_index:])
        else:
            sys.stdout.write(large_string[start_index:
                                          start_index + string_limit])
    sys.stdout.write("\n")

def intelhex_offset(filename, offset):
    """Load a hex or bin file at a particular offset"""
    _, inteltype = splitext(filename)
    ih = IntelHex()
    if inteltype == ".bin":
        ih.loadbin(filename, offset=offset)
    elif inteltype == ".hex":
        ih.loadhex(filename)
    else:
        raise ToolException("File %s does not have a known binary file type"
                            % filename)
    return ih

def integer(maybe_string, base):
    """Make an integer of a number or a string"""
    if isinstance(maybe_string, int):
        return maybe_string
    else:
        return int(maybe_string, base)

def generate_update_filename(name, target):
    return "%s_update.%s" % (
                    name,
                    getattr(target, "OUTPUT_EXT_UPDATE", "bin")
                )

def print_end_warnings(end_warnings):
    """ Print a formatted list of warnings

    Positional arguments:
    end_warnings - A list of warnings (strings) to print
    """
    if end_warnings:
        warning_separator = "-" * 60
        print(warning_separator)
        for end_warning in end_warnings:
            print(end_warning)
        print(warning_separator)
