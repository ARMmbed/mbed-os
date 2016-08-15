/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include "sn_nsdl.h"
#include "sn_coap_header.h"
#include "sn_coap_protocol.h"
#include "sn_nsdl_lib.h"
#include "ns_list.h"
#include "arguments.h"

#define NUMTHREADS 10

extern int register_endpoint(int port, sn_nsdl_ep_parameters_s *endpoint_ptr, int thread_id);
void stop_pgm(char *s)
{
	perror(s);
	exit(1);
}

void *own_alloc(uint16_t size)
{
    if(size) {
        return malloc(size);
    }
    else {
        return 0;
    }
}

void own_free(void *ptr)
{
    free(ptr);
}

void arg_init(void)
{
    memcpy(arg_dst,"::1",32); 	//default localhost
    arg_sport=5683;
    arg_dport=5683;
}


void usage_show(void)
{
    printf("Usage:\n\n"

            "multithread-linux-test [-d 127.0.0.1] \n"
            "-d	NSP IPv4 address (default = ::1)\n"
            "-dp NSP port number (default = 5683)\n");
}

/* function to be executed by the new thread */
void* create_endpoint(void *arg)
{    
    int index = *((int *) arg);
    int port = 8100 + index;
    sn_nsdl_ep_parameters_s *endpoint_ptr;
    uint8_t endpoint_type[] = {"type"};
    uint8_t lifetime_ptr[] = {"120"};
    char str[10];
    sprintf(str, "THREAD_%d", index);
    endpoint_ptr = own_alloc(sizeof(sn_nsdl_ep_parameters_s));
    if(endpoint_ptr)
    {
        memset(endpoint_ptr, 0, sizeof(sn_nsdl_ep_parameters_s));
        endpoint_ptr->endpoint_name_ptr = str;
        endpoint_ptr->endpoint_name_len = strlen(str);
        endpoint_ptr->type_ptr = endpoint_type;
        endpoint_ptr->type_len =  sizeof(endpoint_type)-1;
        endpoint_ptr->lifetime_ptr = lifetime_ptr;
        endpoint_ptr->lifetime_len =  sizeof(lifetime_ptr)-1;
    }
    register_endpoint(port, endpoint_ptr, index);
    if(endpoint_ptr) {
        own_free(endpoint_ptr);
        endpoint_ptr = 0;
    }    
}

int main(int argc, char **argv)
{    
    uint8_t i;
    arg_init();

    if (argc<1)
    {
        usage_show();
    }
    else
    {
        i=1; //argv[0] is the command itself

        argc--; //get the real number of arguments
        while (i<=argc)
        {
            //check arguments
            if (!(strcmp("-h",argv[i])))
            {
                usage_show();
                stop_pgm("");
            }
            else if (!(strcmp("-d",argv[i])))
            {
                if (i++==argc) stop_pgm("Argument missed for option -d\n");
                memcpy(arg_dst,argv[i],strlen((const char*)argv[i])+1);
                i++;
                continue;
            }
            else if (!(strcmp("-p",argv[i])))
            {
                if (i++==argc) stop_pgm("Argument missed for option -p\n");
                arg_port=atoi(argv[i]);
                i++;
                continue;
            }
            else if (!(strcmp("-dp",argv[i])))
            {
                if (i++==argc) stop_pgm("Argument missed for option -dp\n");
                arg_dport=atoi(argv[i]);
                i++;
                continue;
            }
            else
            {
                usage_show();
                stop_pgm("\n--- Argument error ---\n");
            }

        }
    }

    pthread_t threads[NUMTHREADS];
    for (int index = 0; index < NUMTHREADS; index++) {        
        pthread_create(&threads[index], NULL, create_endpoint, (void *) &index);        
        sleep(1);
    }

    for (int i = 0; i < NUMTHREADS; i) {
        pthread_join(threads[i], NULL);
    }
    exit(0);
}



