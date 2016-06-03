from xdg.BaseDirectory import save_data_path
from pycurl import Curl
from bs4 import BeautifulSoup
from os.path import join, dirname, basename
from os import makedirs
from errno import EEXIST
from threading import Thread
from Queue import Queue
from re import compile, sub
from sys import stderr, stdout
from fuzzywuzzy import process
from itertools import takewhile
import argparse
from json import dump, load
from zipfile import ZipFile


RootPackURL = "http://www.keil.com/pack/index.idx"


protocol_matcher = compile("\w*://")
def strip_protocol(url) :
    return protocol_matcher.sub("", str(url))

def largest_version(content) :
    return sorted([t['version'] for t in content.package.releases('release')], reverse=True)[0]

def do_queue(Class, function, interable) :
    q = Queue()
    threads = [Class(q, function) for each in range(20)]
    for each in threads :
        each.setDaemon(True)
        each.start()
    for thing in interable :
        q.put(thing)
    q.join()

class Reader (Thread) :
    def __init__(self, queue, func) :
        Thread.__init__(self)
        self.queue = queue
        self.func = func
    def run(self) :
        while True :
            url = self.queue.get()
            self.func(url)
            self.queue.task_done()

class Cacher (Thread) :
    def __init__(self, queue, func) :
        Thread.__init__(self)
        self.queue = queue
        self.curl = Curl()
        self.curl.setopt(self.curl.FOLLOWLOCATION, True)
        self.func = func
    def run(self) :
        while True :
            url = self.queue.get()
            self.func(self.curl, url)
            self.queue.task_done()


class Cache () :
    def __init__ (self, silent, no_timeouts) :
        self.silent = silent
        self.counter = 0
        self.total = 1
        self.index = None
        self.urls = None
        self.no_timeouts = no_timeouts

    def display_counter (self, message) :
        stdout.write("{} {}/{}\r".format(message, self.counter, self.total))
        stdout.flush()

    def cache_file (self, curl, url) :
        if not self.silent : print("Caching {}...".format(url))
        dest = join(save_data_path('arm-pack-manager'), strip_protocol(url))
        try :
            makedirs(dirname(dest))
        except OSError as exc :
            if exc.errno == EEXIST : pass
            else : raise
        with open(dest, "wb+") as fd :
            curl.setopt(curl.URL, url)
            curl.setopt(curl.FOLLOWLOCATION, True)
            curl.setopt(curl.WRITEDATA, fd)
            if not self.no_timeouts :
                curl.setopt(curl.CONNECTTIMEOUT, 2)
                curl.setopt(curl.LOW_SPEED_LIMIT, 50 * 1024)
                curl.setopt(curl.LOW_SPEED_TIME, 2)
            try :
                curl.perform()
            except Exception as e :
                stderr.write("[ ERROR ] file {} did not download {}\n".format(url, str(e)))
        self.counter += 1
        self.display_counter("Caching Files")

    def pdsc_to_pack (self, url) :
        content = self.pull_from_cache(url)
        new_url = content.package.url.get_text()
        if not new_url.endswith("/") :
            new_url = new_url + "/"
        return (new_url + content.package.vendor.get_text() + "." +
                content.package.find('name').get_text() + "." +
                largest_version(content) + ".pack")

    def cache_pdsc_and_pack (self, curl, url) :
        self.cache_file(curl, url)
        try :
            self. cache_file(curl, self.pdsc_to_pack(url))
        except AttributeError :
            stderr.write("[ ERROR ] {} does not appear to be a conforming .pdsc file\n".format(url))
            self.counter += 1

    def get_urls(self):
        if not self.urls :
            try : root_data = self.pull_from_cache(RootPackURL)
            except IOError : root_data = self.cache_and_parse(RootPackURL)
            self.urls = [join(pdsc.get('url'), pdsc.get('name')) for pdsc in root_data.find_all("pdsc")]
        return self.urls

    def _extract_dict(self, device, filename) :
        to_ret = dict(file=filename)
        try :
            to_ret["memory"] = dict([(m["id"], dict(start=m["start"],
                                                    size=m["size"]))
                                     for m in device("memory")])
            to_ret["algorithm"] = device.algorithm["name"]
            to_ret["debug"] = device.debug["svd"]
            to_ret["compile"] = (device.compile["header"], device.compile["define"])
        except (KeyError, TypeError) :
            pass
        return to_ret

    def _generate_index_helper(self, d) :
        try :
            self.index.update(dict([(dev['dname'], self._extract_dict(dev, d)) for dev in
                                    (self.pull_from_cache(d)("device"))]))
        except AttributeError as e :
            print(e)
        self.counter += 1
        self.display_counter("Generating Index")

    def get_flash_algorthim_binary(device_name) :
        self.load_index()
        device = self.find_device(device_name)
        pack = ZipFile(self.pdsc_to_pack(device['file']))
        return pack.open(device['algorithm'])

    def generate_index(self) :
        self.index = {}
        self.counter = 0
        do_queue(Reader, self._generate_index_helper, self.get_urls())
        with open(join(save_data_path('arm-pack-manager'), "index.json"), "wb+") as out:
            dump(self.index, out)
        stdout.write("\n")

    def find_device(self, match) :
        self.load_index()
        choices = process.extract(match, self.index.keys(), limit=len(urls))
        choices = sorted([(v, k) for k, v in choices.iteritems()], reverse=True)
        if not choices : return []
        else : choices = list(takewhile(lambda t: t[0] == choices[0][0], choices))
        return [(v, self.index[v]) for k,v in choices]

    def dump_index_to_file(self, file) :
        self.load_index()
        with open(file, "wb+") as out:
            dump(self.index, out)

    def load_index(self) :
        if not self.index :
            try :
                with open(join(save_data_path('arm-pack-manager'), "index.json")) as i :
                    self.index = load(i)
            except IOError :
                self.generate_index()
        return self.index

    def cache_everything(self) :
        self.cache_pack_list(self.get_urls())
        self.generate_index()

    def cache_descriptors(self) :
        self.cache_descriptor_list(self.get_urls())
        self.generate_index()

    def cache_descriptor_list(self, list) :
        self.total = len(list)
        self.display_counter("Caching Files")
        do_queue(Cacher, self.cache_file, list)
        stdout.write("\n")

    def cache_pack_list(self, list) :
        self.total = len(list) * 2
        self.display_counter("Caching Files")
        do_queue(Cacher, self.cache_pdsc_and_pack, list)
        stdout.write("\n")

    def pull_from_cache(self, url) :
        dest = join(save_data_path('arm-pack-manager'), strip_protocol(url))
        with open(dest, "r") as fd :
            return BeautifulSoup(fd, "html.parser")

    def gen_dict_from_cache() :
        pdsc_files = pull_from_cache(RootPackUrl)

    def cache_and_parse(self, url) :
        self.cache_file(Curl(), url)
        return self.pull_from_cache(url)

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
            dict(name='matches', nargs="+",
                 help="a bunch of things to search for in part names"),
            dict(name=["-b","--batch"], action="store_true",
                 help="don't ask for user input and assume download all"))
def command_cache (cache, matches, batch=False, verbose= False) :
    if len(matches) == 1 and matches[0] == "everything" :
        cache.cache_everything()
        return True
    if len(matches) == 1 and matches[0] == "descriptors" :
        cache.cache_descriptors()
        return True
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

if __name__ == "__main__" :
    args = parser.parse_args()
    args.command(args)


