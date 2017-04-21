#!python3
import os, shutil, json, pprint, sys, string, json, argparse
from collections import OrderedDict
from shutil import copyfile, copytree

def rename_sdk_old_dirs(path, dry_run = False):
    # I make assumption that all old sdk dirs have "sdk" names.
    sdk_dir_name = "sdk"
    path = "."
    
    for root, dirs, files in os.walk(path):
        for name in dirs:
            if name == "sdk":
                full_path = root + "\\" + name
                new_full_path = root + "\\_old_" + name
                print("rename " + full_path + " ---> " + new_full_path)
                if not dry_run:
                    os.rename(full_path, new_full_path)
                    os.mkdir(full_path)
    
def rename_dirs(sdk_dirs_in_mbed, new_name, dry_run=False):
    
    for dir_path in sdk_dirs_in_mbed:
        xdir_path = os.path.join('.',dir_path)
        new_dir_path = os.path.join(os.path.dirname(xdir_path), new_name)
        print("rename " + xdir_path + " ---> " + new_dir_path)
        if not dry_run:
            os.rename(xdir_path, new_dir_path)
 
def get_file_pathes_couples(path_sdk_componets, skip_dirs = [], skip_files = [], verbose = False):
    mbed_list = []
    cutted_roots = []
    cutted_files = []
    
    path_sdk_componets = path_sdk_componets + '\\'
    
    for root, dirs, files in os.walk(path_sdk_componets):
        procced = True
        cutted_root = root[len(path_sdk_componets):]
        
        for skip_d in skip_dirs:
            if 0 == string.find(cutted_root, skip_d):
                cutted_roots.append(cutted_root)
                procced = False
         
        if procced:
            for file_name in files:
            
                procced = True
                for skip_f in skip_files:
                    if (-1) != string.find(file_name, skip_f):
                        cutted_files.append(file_name)
                        procced = False
                        
                if procced:
                    if file_name.endswith((".c", ".h")):
                        #cutted_path = cutted_root + "\\" + file_name
                        cutted_path = os.path.join(cutted_root, file_name)
                        #full_path = root + "\\" + file_name
                        full_path = os.path.join(root, file_name)
                        item = {"full_path": full_path, "id": cutted_path, "cutted_root": cutted_root}
                        #mbed_list.append([full_path, cutted_path])
                        mbed_list.append(item)

    if verbose:                    
        print("\r\nskipped directories:  {0:#d}".format(len(cutted_roots)))
    
        for xitem in cutted_roots:
            print(xitem)

        print("\r\nskipped files:  {0:#d}".format(len(cutted_files)))

        for kitem in cutted_files:
            print(kitem)
    
    return mbed_list
    
def apply_replacement_id(mbed_list, replacemet_couples):
    for item in mbed_list:
        splited = os.path.split(item["id"])
        result = string.find(splited[1], replacemet_couples["old"])
        if result != -1:
            new_tail = replacemet_couples["new"] + splited[1][len(replacemet_couples["old"]):]
            item["id"] = os.path.join(splited[0],new_tail)
            #print('bingo!')
            #print(item)
        
    return mbed_list
            

def get_copying_automatic_list(list_mbed, list_sdk, mbed_port_path = '', verbose = False):
    copy_list = [] #list of copy items
    orphan_list = []

    licz = 0
    for pathes_mbed in list_mbed:
        empty = True
        for pathes_sdk in list_sdk:
            if pathes_mbed["id"] == pathes_sdk["id"]:
                dest_path = pathes_mbed["full_path"]

                dest_path = dest_path[ (len(mbed_port_path)):]
            
                item = {"id" : pathes_mbed["id"], "src_path": pathes_sdk["full_path"], "dest_path": dest_path, "old_path": pathes_mbed["full_path"]}
                copy_list.append(item)
                
                empty = False;
                
                
        if empty:
            orphan_list.append(pathes_mbed["full_path"])
    
    print("\r\nfitted files: {0:#d}".format(len(copy_list)))
    
    if verbose:
        for item in copy_list:
            str_verbose = "{0} --> {1}"
            print(str_verbose.format(item["id"], item["dest_path"]))
    
    
    print("\r\norphaned files:  {0:#d}".format(len(orphan_list)))

    if verbose:
        for xitem in orphan_list:
            print(xitem)
            
    return copy_list
    
def is_in_copying_list(copy_list, file_id):
    for pathes_copy in copy_list:
        if pathes_copy["id"] == file_id:

            return False
            
    return True

    
def upgrade_copying_list(copy_list, pathes_sdk, dest_mbed_dir_path, print_list):
    splited = os.path.split(pathes_sdk["id"])
    dest_path = os.path.join(dest_mbed_dir_path, splited[1])
    item = {"id" : pathes_sdk["id"], "src_path": pathes_sdk["full_path"], "dest_path": dest_path} #, "old_path": pathes_mbed["full_path"]}
    copy_list.append(item)    
    print_list.append(item)
    
    
    
    
def upgrade_copying_list_by_dirs(copy_list, list_sdk, force_copy_dirs_list, port_relative_dir = '',verbose = False):
    print_list = []
    
    for pathes_sdk in list_sdk:
        if is_in_copying_list(copy_list, pathes_sdk["id"]):
            
            make_hard_copy = False
            
            for hard_copy_dir in force_copy_dirs_list:
            
                if 0 == string.find(pathes_sdk["cutted_root"], hard_copy_dir["sdk_dir"]):
            
                    make_hard_copy = True

                    post_path = os.path.relpath(pathes_sdk["cutted_root"], hard_copy_dir["sdk_dir"])
                    
                    if post_path == '.':
                        corect_hard_copy_dir = hard_copy_dir["mbed_dir"]
                    if post_path != '.': # destynation is a nested directory
                        corect_hard_copy_dir = os.path.join(hard_copy_dir["mbed_dir"], post_path)
                    
                    corect_hard_copy_dir = os.path.join(port_relative_dir, corect_hard_copy_dir)
                    
                    upgrade_copying_list(copy_list, pathes_sdk, corect_hard_copy_dir, print_list)
                    break
                    
                
    print("\r\nforced copy of files by directories: {0:#d}".format(len(print_list)))
    
    if verbose:
        for item in print_list:
            str_verbose = "{0} --> {1}"
            print(str_verbose.format(item["id"], item["dest_path"]))
                        
def upgrade_copying_list_by_files(copy_list, list_sdk, force_copy_files_list, port_relative_dir ='',verbose = False):
    print_list = []
    
    for pathes_sdk in list_sdk:
        if is_in_copying_list(copy_list, pathes_sdk["id"]):
            
            make_hard_copy = False
            
            for hard_copy_file in force_copy_files_list:
                if pathes_sdk["id"] == hard_copy_file["sdk_file"]:
                    make_hard_copy = True
                    
                    corect_hard_copy_dir = os.path.join(port_relative_dir, hard_copy_file["mbed_dir"])
                    
                    upgrade_copying_list(copy_list, pathes_sdk, corect_hard_copy_dir, print_list)
                    break
                
    print("\r\nforced copy of files by files: {0:#d}".format(len(print_list)))
    
    if verbose:
        for item in print_list:
            str_verbose = "{0} --> {1}"
            print(str_verbose.format(item["id"], item["dest_path"]))
            
def copy_one_file(src, dest, verbose=False,dry_run=False):
    dirs_to_created = os.path.dirname(dest)
    
    if not os.path.exists(dirs_to_created):
        if not dry_run:
            os.makedirs(dirs_to_created)
            
        if verbose:
            print('makerdirs: {0}'.format(dirs_to_created))
        
    if not dry_run:
        shutil.copyfile(src, dest)
        
    if verbose:        
        print('copy: {0} --> {1}'.format(src, dest))



if __name__ == '__main__':
    argument_parser = argparse.ArgumentParser()
    argument_parser.add_argument('-r', '--run', help='run', action='store_true')
    argument_parser.add_argument('-v', '--verbose', help='Verbose mode', action='store_true')
    #argument_parser.add_argument('-r', '--rename_only', help='rename only', action='store_true')
    
    parser_args = vars(argument_parser.parse_args())    
    
    verbose = False
    
    if parser_args['verbose'] or not parser_args['run']:
        verbose = True
                        
    with open('update_desc.json') as data_file:    
        update_desc = json.load(data_file)

    
    #if not parser_args ['rename_only']:
    ignore_file_list = update_desc['ignore_file_list']
    ignore_dirs_list = update_desc['ignore_dirs_list']
    id_replacements  = update_desc['id_replacements']
    force_copy_files_list = update_desc['force_copy_files_list']
    force_copy_dirs_list = update_desc['force_copy_dirs_list']
    sdk_dirs_in_mbed  = update_desc['sdk_dirs_in_mbed']
    sdk_component_path = update_desc['sdk_component_path']
    port_relative_dir = update_desc['port_relative_dir_in_mbed']
     
    list_sdk = get_file_pathes_couples(sdk_component_path,
                                       ignore_dirs_list,
                                       ignore_file_list,
                                       verbose)
    list_mbed = []
    for directory in sdk_dirs_in_mbed:
        list_mbed.extend(get_file_pathes_couples(directory))

    list_mbed = apply_replacement_id(list_mbed, id_replacements)

    mbed_port_path = ''

    copy_list = get_copying_automatic_list(list_mbed, list_sdk, mbed_port_path, verbose)
            
    upgrade_copying_list_by_dirs(copy_list, list_sdk, force_copy_dirs_list, port_relative_dir, verbose)
    upgrade_copying_list_by_files(copy_list, list_sdk, force_copy_files_list, port_relative_dir, verbose)
    
    rename_dirs(sdk_dirs_in_mbed, '_old_sdk', not parser_args['run'])
    
    for copy_item in copy_list:
        src = os.path.join('.',copy_item["src_path"])
        dest = os.path.join('.',copy_item["dest_path"])
            
        copy_one_file(src, dest, verbose, not parser_args['run'])
            
    with open('sdk_update_result.json', 'w') as fp:
        json.dump(copy_list, fp)
