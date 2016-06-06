import argparse
from os.path import basename
from tools.arm_pack_manager import Cache
from os.path import basename, join, dirname, exists
from os import makedirs
from itertools import takewhile
from fuzzywuzzy import process
from tools.arm_pack_manager import Cache

parser = argparse.ArgumentParser(description='A Handy little utility for keeping your cache of pack files up to date.')
subparsers = parser.add_subparsers(title="Commands")

def subcommand(name, *args, **kwargs):
    def subcommand(command):
        subparser = subparsers.add_parser(name, **kwargs)

        for arg in args:
            arg = dict(arg)
            opt = arg['name']
            del arg['name']

            if isinstance(opt, basestring):
                subparser.add_argument(opt, **arg)
            else:
                subparser.add_argument(*opt, **arg)

        subparser.add_argument("-v", "--verbose", action="store_true", dest="verbose", help="Verbose diagnostic output")
        subparser.add_argument("-vv", "--very_verbose", action="store_true", dest="very_verbose", help="Very verbose diagnostic output")
        subparser.add_argument("--no-timeouts", action="store_true", help="Remove all timeouts and try to download unconditionally")
        subparser.add_argument("--and", action="store_true", dest="intersection", help="combine search terms as if with an and")
        subparser.add_argument("--or", action="store_false", dest="intersection", help="combine search terms as if with an or")
        subparser.add_argument("--union", action="store_false", dest="intersection", help="combine search terms as if with a set union")
        subparser.add_argument("--intersection", action="store_true", dest="intersection", help="combine search terms as if with a set intersection")
        
        def thunk(parsed_args):
            cache = Cache(not parsed_args.verbose, parsed_args.no_timeouts)
            argv = [arg['dest'] if 'dest' in arg else arg['name'] for arg in args]
            argv = [(arg if isinstance(arg, basestring) else arg[-1]).strip('-')
                    for arg in argv]
            argv = {arg: vars(parsed_args)[arg] for arg in argv
                    if vars(parsed_args)[arg] is not None}

            return command(cache, **argv)

        subparser.set_defaults(command=thunk)
        return command
    return subcommand

def user_selection (message, options) :
    print(message)
    for choice, index in zip(options, range(len(options))) :
        print("({}) {}".format(index, choice))
    pick = None
    while pick is None :
        stdout.write("please select an integer from 0 to {} or \"all\"".format(len(options)-1))
        input = raw_input()
        try :
            if input == "all" :
                pick = options
            else :
                pick = [options[int(input)]]
        except ValueError :
            print("I did not understand your input")
    return pick

def fuzzy_find(matches, urls) :
    choices = {}
    for match in matches :
        for key, value in process.extract(match, urls, limit=None) :
            choices.setdefault(key, 0)
            choices[key] += value
    choices = sorted([(v, k) for k, v in choices.iteritems()], reverse=True)
    if not choices : return []
    elif len(choices) == 1 : return [choices[0][1]]
    elif choices[0][0] > choices[1][0] : choices = choices[:1]
    else : choices = list(takewhile(lambda t: t[0] == choices[0][0], choices))
    return [v for k,v in choices]

@subcommand('cache',
            dict(name='matches', nargs="*",
                 help="a bunch of things to search for in part names"),
            dict(name=['-e','--everything'], action="store_true",
                 help="download everything possible"),
            dict(name=['-d','--descriptors'], action="store_true",
                 help="download all descriptors"),
            dict(name=["-b","--batch"], action="store_true",
                 help="don't ask for user input and assume download all"),
            help="Cache a group of PACK or PDSC files")
def command_cache (cache, matches, everything=False, descriptors=False, batch=False, verbose= False, intersection=True) :
    if everything :
        cache.cache_everything()
        return True
    if descriptors :
        cache.cache_descriptors()
        return True
    if not matches :
        print("No action specified nothing to do")
    else :
        urls = cache.get_urls()
        if intersection :
            choices = fuzzy_find(matches, map(basename, urls))
        else :
            choices = sum([fuzzy_find([m], map(basename, urls)) for m in matches], [])
        if not batch and len(choices) > 1 :
            choices = user_selection("Please select a file to cache", choices)
        to_download = []
        for choice in choices :
            for url in urls :
                if choice in url :
                    to_download.append(url)
        cache.cache_pack_list(to_download)
        return True


@subcommand('find-part',
            dict(name='matches', nargs="+", help="words to match to processors"),
            dict(name=['-l',"--long"], action="store_true",
                 help="print out part details with part"),
            dict(name=['-p', '--parts-only'], action="store_false", dest="print_aliases"),
            dict(name=['-a', '--aliases-only'], action="store_false", dest="print_parts"),
            help="Find a Part and it's description within the cache")
def command_find_part (cache, matches, long=False, intersection=True,
                       print_aliases=True, print_parts=True) :
    if long :
        import pprint
        pp = pprint.PrettyPrinter()
    parts = cache.index
    if intersection :
        choices = fuzzy_find(matches, parts.keys())
        aliases = fuzzy_find(matches, cache.aliases.keys())
    else :
        choices = sum([fuzzy_find([m], parts.keys()) for m in matches], [])
        aliases = sum([fuzzy_find([m], cache.aliases.keys()) for m in matches], [])
    if print_parts:
        for part in choices :
            print part
            if long :
                pp.pprint(cache.index[part])
    if print_aliases:
        for alias in aliases :
            print alias
            if long :
                pp.pprint(cache.index[cache.aliases[alias]])

@subcommand('dump-parts',
            dict(name='out', help='directory to dump to'),
            dict(name='parts', nargs='+', help='parts to dump'),
            help='Create a directory with an index.json describing the part and all of their associated flashing algorithms.'
)
def command_dump_parts (cache, out, parts, intersection=False) :
    index = {}
    if intersection :
        for part in fuzzy_find(parts, cache.index):
            index.update(cache.index[part])
    else :
        for part in parts :
            index.update(dict(cache.find_device(part)))
    for n, p in index.iteritems() :
        try :
            if not exists(join(out, dirname(p['algorithm']['file']))) :
                makedirs(join(out, dirname(p['algorithm']['file'])))
            with open(join(out, p['algorithm']['file']), "wb+") as fd :
                fd.write(cache.get_flash_algorthim_binary(n).read())
        except KeyError:
            print("[Warning] {} does not have an associated flashing algorithm".format(n))
    with open(join(out, "index.json"), "wb+") as fd :
        dump(index,fd)


@subcommand('cache-part',
            dict(name='matches', nargs="+", help="words to match to devices"),
            help='Cache PACK files associated with the parts matching the provided words')
def command_cache_part (cache, matches, intersection=True) :
    index = cache.index
    if intersection :
        choices = fuzzy_find(matches, index.keys())
        aliases = fuzzy_find(matches, cache.aliases.keys())
    else :
        choices = sum([fuzzy_find([m], index.keys()) for m in matches], [])
        aliases = sum([fuzzy_find([m], cache.aliases.keys()) for m in matches], [])
    urls = set([index[c]['pdsc_file'] for c in choices])
    urls += set([index[cache.aliasse[a]] for a in aliases])
    cache.cache_pack_list(list(urls))

def get_argparse() :
    return parser

def main() :
    args = parser.parse_args()
    args.command(args)

