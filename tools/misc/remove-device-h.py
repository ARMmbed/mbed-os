import json
import os
import stat
import re
from collections import OrderedDict
from subprocess import Popen

git_processes = []

class MyJSONEncoder(json.JSONEncoder):
    def __init__(self, *args, **kwargs):
        super(MyJSONEncoder, self).__init__(*args, **kwargs)
        self.current_indent = 0
        self.current_indent_str = ""


    def encode(self, o):
        #Special Processing for lists
        if isinstance(o, (list, tuple)):
            primitives_only = True
            for item in o:
                if isinstance(item, (list, tuple, dict)):
                    primitives_only = False
                    break
            output = []
            if primitives_only:
                for item in o:
                    output.append(json.dumps(item))
                return "[" + ", ".join(output) + "]"
            else:
                self.current_indent += self.indent
                self.current_indent_str = " " * self.current_indent
                for item in o:
                    output.append(self.current_indent_str + self.encode(item))
                self.current_indent -= self.indent
                self.current_indent_str = " " * self.current_indent
                return "[\n" + ",\n".join(output) + "\n" + self.current_indent_str + "]"
        elif isinstance(o, dict):
            primitives_only = True
            for item in o.values():
                if isinstance(item, (list, tuple, dict)):
                    primitives_only = False
                    break
            output = []
            if primitives_only and len(o) < 3:
                for key, value in o.iteritems():
                    output.append(json.dumps(key) + ": " + self.encode(value))
                return "{" + ", ".join(output) + "}"
            else:
                self.current_indent += self.indent
                self.current_indent_str = " " * self.current_indent
                for key, value in o.iteritems():
                    output.append(self.current_indent_str + json.dumps(key) + ": " + self.encode(value))
                self.current_indent -= self.indent
                self.current_indent_str = " " * self.current_indent
                return "{\n" + ",\n".join(output) + "\n" + self.current_indent_str + "}"
        else:
            return json.dumps(o)

def load(path):
    with open(path, 'r') as f :
        return json.load(f, object_pairs_hook=OrderedDict)

def dump(path, obj):
    with os.fdopen(os.open(path, os.O_WRONLY | os.O_CREAT, stat.S_IRUSR | stat.S_IWUSR), 'w') as f :
        os.chmod(path, stat.S_IRUSR | stat.S_IWUSR)
        f.write(MyJSONEncoder(indent=4).encode(obj))
        f.write(u'\n')
        f.truncate()

def find(stem, path) :
    for root, directories, files in os.walk(path, followlinks=True) :
        [dir for dir in directories if dir[0] != '.']
        if (stem_match(stem,os.path.basename(os.path.normpath(root))) and
            "device.h" in files) :
            return os.path.join(root, "device.h")

def find_all_devices(path, verbose=False) :
    for root, directories, files in os.walk(path, followlinks=True) :
        [dir for dir in directories if dir[0] != '.']
        if "device.h" in files :
            if verbose : print("[VERBOSE] found a device.h file in {}".format(root))
            yield os.path.join(root, "device.h")

mbed_matcher = re.compile('mbed', re.IGNORECASE)
def stem_match(stem, thing) :
    return (stem in thing or
            re.sub(mbed_matcher, '', stem) in thing)

attr_matcher = re.compile('^#define\W+DEVICE_(\w+)\W+1.*$')
def parse_attributes(path) :
    with open(path) as input :
        for line in input :
            m = re.match(attr_matcher, line)
            if m: yield m.group(1)

remove_matcher = re.compile('^#define\W+DEVICE_(\w+)\W+[10].*$')
def remove_attributes(path) :
    with open(path) as input :
        remainder = filter(lambda l: not re.match(remove_matcher, l), input)
    with open(path,"wb") as output :
        output.truncate(0)
        output.write("// The 'provides' section in 'target.json' is now used"+
                     " to create the device's hardware preprocessor switches.\n")
        output.write("// Check the 'provides' section of the target description"+
                     " in 'targets.json' for more details.\n")
        output.writelines(remainder)

def user_select(things, message) :
    print(message)
    for thing, number in zip(things, range(len(things))):
        print("{} : {}".format(number, thing))
    selection = None
    while selection is None :
        print("please select an integer [0..{}] or specify all".format(len(things) - 1))
        try :
            i = raw_input()
            if i == "all" :
                selection = "all"
            else :
                selection = int(i)
                if (selection > len(things) or
                    selection < 0) :
                    print("selection {} out of range".format(selection))
                    selection = None
        except (ValueError, SyntaxError) :
            print("selection not understood")
    if selection == "all" :
        return things
    else :
        return [things[selection]]

target_matcher = re.compile("TARGET_")
def strip_target(str) :
    return re.sub(target_matcher, "", str)

def add_to_targets(targets, device_file, verbose=False, remove=False) :
    if verbose : print("[VERBOSE] trying target {}".format(device_file))
    device = strip_target(os.path.basename(os.path.normpath(os.path.dirname(device_file))))
    if not device :
        print("[WARNING] device {} did not have an associated device.h".format(device))
    else :
        possible_matches = set([key for key in targets.keys() if stem_match(device, key)])
        for key, value in targets.iteritems() :
            for alt in value['extra_labels'] if 'extra_labels' in value else [] :
                if stem_match(device, alt) : possible_matches.add(key)
            for alt in value['extra_labels_add'] if 'extra_labels_add' in value else [] :
                if stem_match(device, alt) : possible_matches.add(key)
        possible_matches = list(possible_matches)
        for match in possible_matches :
            if device == match : possible_matches = [match]
        if not possible_matches :
            print("[WARNING] device {} did not have an associated entry in targets.json".format(device))
            return None
        elif len(possible_matches) > 1 :
            message = ("possible matches for file {}".format(device_file))
            target = user_select(possible_matches, message)
        else :
            target = possible_matches
        attrs = list(parse_attributes(device_file))
        if attrs :
            for t in target :
                targets[t]["device_has"] = sorted(list(set(targets[t].setdefault("device_has",[]) + attrs)))
                if verbose : print("[VERBOSE] target {} now device_has {}".format(t, attrs))
            if remove is True:
                remove_attributes(device_file)

if __name__ == '__main__' :
    import argparse
    parser = argparse.ArgumentParser(description='A helpful little script for converting' +
                                     ' device.h files to parts of the targets.json file')
    parser.add_argument('-a', '--all', action='store_true',
                        help='find and convert all available device.h files in the'+
                        ' directory tree starting at the current directory')
    parser.add_argument('-f', '--file', nargs='+', help='specify an individual file to '+
                        'convert from device.h format to a piece of targets.json')
    parser.add_argument('-t', '--target', nargs='+', help='specify an individual target'+
                        ' to convert from device.h format to a piece of targets.json')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help="print out every target that is updated in the targets.json")
    parser.add_argument('-r', '--rm', action='store_true',
                        help="remove the used attributes from a device.h file")
    args = parser.parse_args()
    if not args.target and not args.file and not args.all :
        print("[WARNING] no action specified; auto-formatting targets.json")

    targets_file_name = os.path.join(os.curdir, "hal", "targets.json")
    try :
        targets = load(targets_file_name)
    except OSError :
        print("[ERROR] did not find targets.json where I expected it {}".format(targets_file_name))
        exit(1)
    except ValueError :
        print("[ERROR] invalid json found in {}".format(targets_file_name))
        exit(2)

    if args.target :
        for target in args.target :
            device_file = find(target, os.curdir)
            if device_file :
                add_to_targets(targets, device_file, verbose=args.verbose, remove=args.rm)
            else :
                print("[WARNING] could not locate a device file for target {}".format(target))

    if args.file :
        for file in args.file :
            add_to_targets(targets, file, verbose=args.verbose, remove=args.rm)

    if args.all :
        for file in find_all_devices(os.curdir, verbose=args.verbose) :
            add_to_targets(targets, file, verbose=args.verbose, remove=args.rm)

    dump(targets_file_name, targets)

    for process in git_processes :
        process.wait()
