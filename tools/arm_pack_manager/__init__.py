from xdg.BaseDirectory import save_data_path
from pycurl import Curl
from bs4 import BeautifulSoup
from os.path import join, dirname
from os import makedirs
from errno import EEXIST
from threading import Thread
from Queue import Queue


RootPackURL = "http://www.keil.com/pack/index.idx"

class Cacher (Thread) :
    def __init__(self, queue, silent) :
        Thread.__init__(self)
        self.queue = queue
        self.curl = Curl()
        self.curl.setopt(self.curl.FOLLOWLOCATION, True)
        self.silent = silent
    def run(self) :
        while True :
            url = self.queue.get()
            cache_file(self.curl, url, self.silent)
            self.queue.task_done()


def cache_file (curl, url, silent=False) :
    if not silent : print("Caching {}...".format(url))
    dest = join(save_data_path('arm-pack-manager'), url)
    try :
        makedirs(dirname(dest))
    except OSError as exc :
        if exc.errno == EEXIST : pass
        else : raise
    with open(dest, "wb+") as fd :
        curl.setopt(curl.URL, RootPackURL)
        curl.setopt(curl.FOLLOWLOCATION, True)
        curl.setopt(curl.WRITEDATA, fd)
        curl.perform()

def cache_everything(silent=False) :
    url_queue = Queue()
    urls = [pdsc.get('url') + pdsc.get('name') for pdsc in cache_and_parse(RootPackURL,silent).find_all("pdsc")]
    threads = [Cacher(url_queue, silent) for each in range(10)]
    for each in threads :
        each.setDaemon(True)
        each.start()
    for url in urls:
        url_queue.put(url)
    url_queue.join()


def pull_from_cache(url) :
    dest = join(save_data_path('arm-pack-manager'), url)
    with open(dest, "r") as fd :
        return BeautifulSoup(fd, "html.parser")

def cache_and_parse(url, silent=False) :
    cache_file(Curl(), url, silent)
    return pull_from_cache(url)

if __name__ == "__main__" :
    cache_everything()


