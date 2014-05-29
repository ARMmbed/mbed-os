#!/usr/bin/env python -u

import subprocess
import time
import sys
import argparse

# == Script configuration ==

# TODO: Refactor this into a preferences file

# We want to process files with these extensions.
sourceFileExtensions = [ 'java', 'rb', 'php', 'js', 'scala', 'c', 'cpp', 'h', 'py']



# == Command line options ==

parser = argparse.ArgumentParser(description='Remove trailing whitespaces from source files', formatter_class=argparse.RawTextHelpFormatter)

# ==== Generic options ====
parser.add_argument('--debug', dest='debug', action='store_true',
                    help='Enable debug mode')
parser.add_argument('--ext', '-e', dest='extensions', action='append',
                    help='Add an additional file extension that should be processed')


# ==== File mode options ====
group = parser.add_argument_group('"Specific files"-mode')
group.add_argument('files', nargs='*', metavar='FILE',
                    help='Specifies files to be processed.')

# ==== Git mode options ====
group = parser.add_argument_group('Git-mode', 'NOTE: Always operates recursively due to a limitation in git ls-files!')
group.add_argument('--force-git', dest='forceGit', action='store_true')
group.add_argument('--not-only-modified', '-m', dest='onlyModified', action='store_false',
                    help='Process only files that have been modified')


# ==== Find mode options ====
group = parser.add_argument_group('Find-mode', 'Find files to process using the "find"-command')
group.add_argument('--force-find', dest='forceFind', action='store_true')


group.add_argument('--recursive', '-r', dest='recursive', action='store_true')



# == Initialization ==

args = parser.parse_args()

# Used to ignore subprocess output as explained
# [here](http://mail.python.org/pipermail/python-dev/2006-June/066111.html).
FNULL = open('/dev/null', 'w')

# Are we currently in a directory under Git version control?
# [(See Stackoverflow)](http://stackoverflow.com/a/2044677/124257)
gitRevParse = subprocess.Popen(["git", "rev-parse"], stdout=FNULL, stderr=FNULL)
gitRevParse.wait()
inGitDir = gitRevParse.returncode == 0



# == List command generation ==

# These functions generate the shell commands that will be used to gather
# a the list of files that should be processed. Only one of them is called,
# depending on the mode `mow.py` is running in.

def gitLsFilesCommand(wildcards, onlyModified=False):
    command = ["git", "ls-files"]

    if onlyModified:
        command.append('--modified')

    for wildcard in wildcards:
        command.append(wildcard)

    return command


def findFilesCommand(wildcards, paths=['.'], recursive=True):
    command = ["find"]
    command.extend(paths)

    command.extend(['-type', 'f'])

    if not recursive:
        command.extend(['-depth', '1'])

    command.append('(')
    for wildcard in wildcards:
        command.extend(['-name', wildcard, '-or'])

    command.pop() # Remove the dangling "-or" at the end
    command.append(')')

    return command


def specificFilesCommand(files):
    command = ['printf']

    filesString = ""
    for file in files:
        filesString += "%s\n" % file

    return command


# == Processing functions ==

# This is where the actual space trimming happens.

def processFiles(files, listFilesCommand, debug=False):
    sed = None

    if(debug):
        print "Data that would be passed to sed:"
        sed = subprocess.Popen([ 'cat' ], stdin=subprocess.PIPE)
    else:
        sys.stdout.write("Processing files")
        # Don't remove the empty argument after -i
        # [as Mac OS X doesn't allow -i without parameter](http://blog.mpdaugherty.com/2010/05/27/difference-with-sed-in-place-editing-on-mac-os-x-vs-linux/).
        sed = subprocess.Popen([ 'xargs', 'sed', '-i', '', '-e', 's/[[:space:]]*$//' ], stdin=subprocess.PIPE)

    while len(files) > 0:
        sed.stdin.writelines(files.pop())

        if not debug:
            sys.stdout.write('.')

    if not debug:
        print ' Done!'



# == User interaction functions ==

# These functions present the interactive dialogs to the user.

def askProcessFiles(files, listFilesCommand):
    options = {
        'y': lambda a, b: processFiles(a, b, args.debug),
        'n': lambda a, b: exit("Bye!"),
        'f': lambda a, b: lambdaPrint(a) or True,
        'c': lambda a, b: lambdaPrint(b) or True,
        'd': lambda a, b: printDebug(a, b) or True,
        'q': lambda a, b: exit("Bye!"),
        # The ? is handled in the eval-loop, and thus doesn't need a callback function.
        '?': None
    }

    choice = None
    while choice == None:
        sys.stdout.write("\033[1;94m" + "About to process %d files! Continue? [y,n,f,c,q,?] \033[0m" % len(files))
        choice = raw_input()

        if not choice in options.keys() or choice == '?':
            choice = None
            printProcessHelp()
        else:
            result = options[choice](files, listFilesCommand)
            if result: # We continue the loop if the callback functions returns True.
                choice = None


def lambdaPrint(x):
    print x


def printDebug(files, listFilesCommand):
    print "\nArguments as found by argparse:"
    print args

    print "\nFiles to be processed:"
    print files

    print "\nCommand that generated the file list:"
    print listFilesCommand



def printProcessHelp():
    print "\033[1;91m" + """y - Start processing files
n - Abort execution, equivalent to "q"
f - List files to be processed
c - Show command used to generate the file list
d - Print debug information
q - Abort execution
? - Print help \033[0m"""



# == Script execution ==

# Generate wildcards from the extensions that should be processed
if args.extensions != None:
    sourceFileExtensions.extend(args.extensions)

sourceFileWildcards = [ "*.%s" % x for x in sourceFileExtensions]


# This is where we decide which functions are actually invoked depending on input parameters.
listFilesCommand = None
if args.files != None and len(args.files) > 0:
    listFilesCommand = specificFilesCommand(args.files)

elif args.forceGit:
    if not args.recursive:
        exit("Git-mode does not currently support non-recursive processing! Exiting.")

    if not inGitDir:
        exit("Not a git repository (or any of the parent directories)! Exiting.")
    listFilesCommand = gitLsFilesCommand(sourceFileWildcards, onlyModified=args.onlyModified)

else:
    listFilesCommand = findFilesCommand(sourceFileWildcards, recursive=args.recursive)


# **Process those files!**
files = subprocess.Popen(listFilesCommand, stdout=subprocess.PIPE).stdout.readlines()
askProcessFiles(files, listFilesCommand)

