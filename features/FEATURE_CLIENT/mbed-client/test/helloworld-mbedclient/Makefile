PLATFORM= 
OS = LINUX
TARGET	= helloworld_mbedclient
OBJECTS = main.o mbedclient.o
CFLAGS	= -std=c++11 -Wall -D_REENTRANT -D$(OS) -I ../../lwm2m-client \
	  -I ../../source/include -I ../../../../libService/libService \
  	  -I ../../ -I ../../../ -DTARGET_LIKE_LINUX -DHAVE_DEBUG

LDFLAGS = -D_REENTRANT -L../../ -lmbedclient_gcc -L ../../../../nsdl-c -lnsdl_gcc \
			-L ../../../../libService -lservice_gcc -lpthread\
			-L../../../../mbedtls/library -lmbedtls -lmbedcrypto -lmbedx509

all: $(TARGET) 

$(TARGET): $(OBJECTS)
	$(PLATFORM)g++ -g -o $(TARGET) $(OBJECTS) $(LDFLAGS)
	
.cpp.o:
	$(PLATFORM)g++ -c -g -O2 $(CFLAGS) $< 
	
clean:
	rm -f $(TARGET) $(OBJECTS)

