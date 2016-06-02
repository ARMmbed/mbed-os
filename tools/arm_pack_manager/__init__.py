from xdg.BaseDirectory import save_data_path
from pycurl import Curl
from bs4 import BeautifulSoup
from os.path import join, dirname
from os import makedirs
from errno import EEXIST
from threading import Thread
from Queue import Queue
from re import compile, sub


RootPackURL = "http://www.keil.com/pack/index.idx"

class Cacher (Thread) :
    def __init__(self, queue, silent, func) :
        Thread.__init__(self)
        self.queue = queue
        self.curl = Curl()
        self.curl.setopt(self.curl.FOLLOWLOCATION, True)
        self.silent = silent
        self.func = func
    def run(self) :
        while True :
            url = self.queue.get()
            self.func(self.curl, url, self.silent)
            self.queue.task_done()

protocol_matcher = compile("\w*://")
def strip_protocol(url) :
    return sub(protocol_matcher, "", url)


def cache_file (curl, url, silent=False) :
    if not silent : print("Caching {}...".format(url))
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
        curl.setopt(curl.TIMEOUT, 5)
        try :
            curl.perform()
        except Exception:
            pass

def largest_version(content) :
    return sorted([t['version'] for t in content.package.releases('release')], reverse=True)[0]

def cache_pdsc_and_pack (curl, url, silent=False) :
    content = cache_and_parse(url)
    try :
        new_url = content.package.url.get_text()
        if not new_url.endswith("/") :
            new_url = new_url + "/"
        cache_file(curl,
                   new_url +
                   content.package.vendor.get_text() + "." +
                   content.package.find('name').get_text() + "." +
                   largest_version(content) + ".pack",
                   silent)
    except AttributeError :
        print("[WARNING] {} does not appear to be a conforming .pdsc file".format(url))

def do_queue(function, interable, silent=False) :
    q = Queue()
    threads = [Cacher(q, silent, function) for each in range(10)]
    for each in threads :
        each.setDaemon(True)
        each.start()
    for thing in interable :
        q.put(thing)
    q.join()

def cache_everything(silent=False) :
    urls = [join(pdsc.get('url'), pdsc.get('name')) for pdsc in cache_and_parse(RootPackURL,silent).find_all("pdsc")]
    do_queue(cache_pdsc_and_pack, urls, silent)

def pull_from_cache(url) :
    dest = join(save_data_path('arm-pack-manager'), strip_protocol(url))
    with open(dest, "r") as fd :
        return BeautifulSoup(fd, "html.parser")

def gen_dict_from_cache() :
    pdsc_files = pull_from_cache(RootPackUrl)

def cache_and_parse(url, silent=False) :
    cache_file(Curl(), url, silent)
    return pull_from_cache(url)

if __name__ == "__main__" :
    cache_everything()


