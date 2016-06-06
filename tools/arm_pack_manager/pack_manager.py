import argparse
from os.path import basename
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
        for key, value in process.extract(match, urls, limit=len(urls)) :
            choices.setdefault(key, 0)
            choices[key] += value
    choices = sorted([(v, k) for k, v in choices.iteritems()], reverse=True)
    if not choices : return []
    elif len(choices) == 1 : return choices[0][1]
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
                 help="don't ask for user input and assume download all"))
def command_cache (cache, matches, everything=False, descriptors=False, batch=False, verbose= False) :
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
        choices = fuzzy_find(matches, map(basename, urls))
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
                 help="print out part details with part"))
def command_find_part (cache, matches, long=False) :
    if long :
        import pprint
        pp = pprint.PrettyPrinter()
    parts = cache.load_index()
    choices = fuzzy_find(matches, parts.keys())
    for part in choices :
        print part
        if long :
            pp.pprint(cache.index[part])

@subcommand('cache-part',
            dict(name='matches', nargs="+", help="words to match to devices"))
def command_cache_part (cache, matches) :
    index = cache.load_index()
    choices = fuzzy_find(matches, index.keys())
    urls = [index[c]['file'] for c in choices]
    cache.cache_pack_list(urls)

def main() :
    args = parser.parse_args()
    args.command(args)

