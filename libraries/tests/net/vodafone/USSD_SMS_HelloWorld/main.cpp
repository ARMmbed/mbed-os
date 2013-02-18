#include "mbed.h"
#include "VodafoneUSBModem.h"
#include "test_env.h"

bool compare_msisdn(char* remote_msisdn, char* local_msisdn)
{
  if( !memcmp(remote_msisdn, "+44", 3) ) //Conver to local number
  {
    remote_msisdn += 2;
    remote_msisdn[0] = '0';
  }
  if( !memcmp(local_msisdn, "+44", 3) ) //Conver to local number
  {
    local_msisdn += 2;
    local_msisdn[0] = '0';
  }
  return !strcmp(remote_msisdn, local_msisdn);
}

bool run(VodafoneUSBModem& modem)
{
    char local_msisdn[32];
    char remote_msisdn[32];
    char local_msg[192];
    char remote_msg[192];

    int ret;
    //Clear SMS inbox
    size_t count;
    do
    {
      ret = modem.getSMCount(&count);
      if(ret)
      {
        return false;
      }
      if(count)
      {
        //Fetch SMS
        ret = modem.getSM(remote_msisdn, remote_msg, 192);
        if(ret)
        {
          return false;
        }
      }
    } while(count);

    //Now get MSISDN using USSD
    ret = modem.sendUSSD("*#100#", local_msisdn, 32);
    if(ret)
    {
      return false;
    }
    
    printf("Local MSISDN is %s\n", local_msisdn);
    
    //Makeup a random text message (32 uppper case letters)
    for(int i = 0; i < 32; i++)
    {
      local_msg[i] = 'A' + (rand() % 26); //This is pseudo-random only, but we don't really care anyway
    } 
    local_msg[32] = '\0'; //Terminate string
    
    printf("Sending '%s'\n", local_msg);
    
    //Send SM
    ret = modem.sendSM(local_msisdn, local_msg);
    if(ret)
    {
      return false;
    }
    
    //Now wait for corresponding message for 15s max
    Timer t;
    t.start();
    do
    {
      ret = modem.getSMCount(&count);
      if(ret)
      {
        return false;
      }
      if(count)
      {
        //Fetch SM
        ret = modem.getSM(remote_msisdn, remote_msg, 192);
        if(ret)
        {
          return false;
        }
        
        printf("Received '%s' from %s\n", remote_msg, remote_msisdn);
      
        if( compare_msisdn(remote_msisdn, local_msisdn) && !strcmp(remote_msg, local_msg) )
        {
          break;
        }
      }
      if(t.read_ms() > 15000)
      {
        return false;
      }
      Thread::wait(500);
    } while(true);
    
    //Success :)
    return true;    
}

void test(void const*)  
{    
    VodafoneUSBModem modem;
    bool test = run(modem);
    if(test)
    {
      printf("Test successful\n");
      notify_completion(true);
    }
    else
    {
      printf("Test failed\n");
      notify_completion(false);
    }
    //notify_completion() blocks indefinitely
}

int main()
{
  Thread testTask(test, NULL, osPriorityNormal, 1024 * 5);
  DigitalOut led(LED1);
  while(1)
  {
    led=!led;
    Thread::wait(1000);  
  }

  return 0;
}
