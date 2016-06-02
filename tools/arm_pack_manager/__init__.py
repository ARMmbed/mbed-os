from xdg.BaseDirectory import save_data_path
from pycurl import Curl
from bs4 import BeautifulSoup
from os.path import join, dirname
from os import makedirs
from errno import EEXIST
from threading import Thread
from Queue import Queue
from re import compile, sub
from sys import stderr, stdout


RootPackURL = "http://www.keil.com/pack/index.idx"

protocol_matcher = compile("\w*://")
def strip_protocol(url) :
    return sub(protocol_matcher, "", url)

def largest_version(content) :
    return sorted([t['version'] for t in content.package.releases('release')], reverse=True)[0]

def do_queue(function, interable) :
    q = Queue()
    threads = [Cacher(q, function) for each in range(20)]
    for each in threads :
        each.setDaemon(True)
        each.start()
    for thing in interable :
        q.put(thing)
    q.join()


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
        self.no_timeouts = no_timeouts

    def display_counter (self) :
        stdout.write("Caching Files {}/{}\r".format(self. counter,self.total))
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
            except Exception as e:
                stderr.write("[ ERROR ] file {} did not download {}\n".format(url, str(e)))
        self.counter += 1
        self.display_counter()

    def cache_pdsc_and_pack (self, curl, url) :
        content = self.cache_and_parse(url)
        try :
            new_url = content.package.url.get_text()
            if not new_url.endswith("/") :
                new_url = new_url + "/"
            self. cache_file(curl,
                             new_url +
                             content.package.vendor.get_text() + "." +
                             content.package.find('name').get_text() + "." +
                             largest_version(content) + ".pack")
        except AttributeError :
            stderr.write("[ ERROR ] {} does not appear to be a conforming .pdsc file\n".format(url))
            self.counter += 1


    def cache_everything(self) :
        urls = [join(pdsc.get('url'), pdsc.get('name')) for pdsc in self.cache_and_parse(RootPackURL).find_all("pdsc")]
        self.total = len(urls) * 2
        self.display_counter()
        do_queue(self.cache_pdsc_and_pack, urls)

    def pull_from_cache(self, url) :
        dest = join(save_data_path('arm-pack-manager'), strip_protocol(url))
        with open(dest, "r") as fd :
            return BeautifulSoup(fd, "html.parser")

    def gen_dict_from_cache() :
        pdsc_files = pull_from_cache(RootPackUrl)

    def cache_and_parse(self, url) :
        self.cache_file(Curl(), url)
        return self.pull_from_cache(url)


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
            cache = Cache(not parsed_args.verbose, parserd_args.no_timeouts)
            argv = [arg['dest'] if 'dest' in arg else arg['name'] for arg in args]
            argv = [(arg if isinstance(arg, basestring) else arg[-1]).strip('-')
                    for arg in argv]
            argv = {arg: vars(parsed_args)[arg] for arg in argv
                    if vars(parsed_args)[arg] is not None}

            return command(**argv)

        subparser.set_defaults(command=thunk)
        return command
    return subcommand

if __name__ == "__main__" :
    import argparse
    parser = argparse.ArgumentParser(description='A Handy little utility for keeping your cache of pack files up to date.')
    foo = Cache(True, False)
    foo.cache_everything()


