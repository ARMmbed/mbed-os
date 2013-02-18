#include "mbed.h"
#include "EthernetInterface.h"
#include "NTPClient.h"

EthernetInterface eth;
NTPClient ntp;

int main() 
{
    eth.init(); //Use DHCP

    eth.connect();
   
    printf("Trying to update time...\r\n");
    if (ntp.setTime("0.pool.ntp.org") == 0)
    {
      printf("Set time successfully\r\n");
      time_t ctTime;
      ctTime = time(NULL);
      printf("Time is set to (UTC): %s\r\n", ctime(&ctTime));
    }
    else
    {
      printf("Error\r\n");
    } 
   
    eth.disconnect();  

    while(1) {
    }
}
