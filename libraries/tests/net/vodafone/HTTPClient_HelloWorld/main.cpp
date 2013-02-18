#include "mbed.h"
#include "VodafoneUSBModem.h"
#include "HTTPClient.h"
#include "test_env.h"

int main() 
{    
    printf("Connecting...\n");
    
    VodafoneUSBModem modem;
    HTTPClient http;
    char str[512];

    int ret = modem.connect("internet", "web", "web");
    if(ret)
    {
      printf("Could not connect\n");
      notify_completion(false); //Blocks indefinitely
    }
    
    //GET data
    printf("Trying to fetch page...\n");
    ret = http.get("http://mbed.org/media/uploads/donatien/hello.txt", str, 128);
    if (!ret)
    {
      printf("Page fetched successfully - read %d characters\n", strlen(str));
      printf("Result: %s\n", str);
    }
    else
    {
      printf("Error - ret = %d - HTTP return code = %d\n", ret, http.getHTTPResponseCode());
      modem.disconnect();
      notify_completion(false); //Blocks indefinitely
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
      modem.disconnect();
      notify_completion(false); //Blocks indefinitely
    }
    
    modem.disconnect();  
    
    notify_completion(true); //Blocks indefinitely
}
