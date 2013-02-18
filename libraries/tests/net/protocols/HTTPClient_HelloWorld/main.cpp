#include "mbed.h"
#include "EthernetInterface.h"
#include "HTTPClient.h"

EthernetInterface eth;
HTTPClient http;
char str[512];

int main() 
{
    eth.init(); //Use DHCP

    eth.connect();
    
    //GET data
    printf("Trying to fetch page...\n");
    int ret = http.get("http://mbed.org/media/uploads/donatien/hello.txt", str, 128);
    if (!ret)
    {
      printf("Page fetched successfully - read %d characters\n", strlen(str));
      printf("Result: %s\n", str);
    }
    else
    {
      printf("Error - ret = %d - HTTP return code = %d\n", ret, http.getHTTPResponseCode());
    }
    
    //POST data
    HTTPMap map;
    HTTPText text(str, 512);
    map.put("Hello", "World");
    map.put("test", "1234");
    printf("Trying to post data...\n");
    ret = http.post("http://httpbin.org/post", map, &text);
    if (!ret)
    {
      printf("Executed POST successfully - read %d characters\n", strlen(str));
      printf("Result: %s\n", str);
    }
    else
    {
      printf("Error - ret = %d - HTTP return code = %d\n", ret, http.getHTTPResponseCode());
    }
    
    eth.disconnect();  

    while(1) {
    }
}
