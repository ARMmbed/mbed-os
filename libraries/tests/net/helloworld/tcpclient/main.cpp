#include <algorithm>
#include "mbed.h"
#include "EthernetInterface.h"
#include "test_env.h"

namespace {
    const char *HTTP_SERVER_NAME = "http://developer.mbed.org";
    const int HTTP_SERVER_PORT = 80;
    const int RECV_BUFFER_SIZE = 512;

    // Test related data
    const char *HTTP_OK_STR = "200 OK";
    const char *HTTP_HELLO_STR = "Hello world!";
}

bool find_substring(const char *first, const char *last, const char *s_first, const char *s_last) {
    const char *f = std::search(first, last, s_first, s_last);
    return (f != last);
}

int main() {
    EthernetInterface eth;
    eth.init(); //Use DHCP
    eth.connect();
    printf("TCP client IP Address is %s\n", eth.getIPAddress());

    TCPSocketConnection sock;
    sock.connect(HTTP_SERVER_NAME, HTTP_SERVER_PORT);

    char http_cmd[] = "GET /media/uploads/mbed_official/hello.txt HTTP/1.0\n\n";
    sock.send_all(http_cmd, sizeof(http_cmd));

    char buffer[RECV_BUFFER_SIZE] = {0};
    bool result = true;
    while (true) {
        const int ret = sock.receive(buffer, sizeof(buffer) - 1);
        if (ret <= 0)
            break;
        buffer[ret] = '\0';

        // Find 200 OK HTTP status in reply
        bool found_200_ok = find_substring(buffer, buffer + ret, HTTP_OK_STR, HTTP_OK_STR + strlen(HTTP_OK_STR));
        result = result && found_200_ok;

        // Find Hello World! in reply
        bool found_hello = find_substring(buffer, buffer + ret, HTTP_HELLO_STR, HTTP_HELLO_STR + strlen(HTTP_HELLO_STR));
        result = result && found_hello;

        // Print results
        printf("HTTP: Received %d chars from server\r\n", ret);
        printf("HTTP: Received 200 OK status ... %s\r\n", found_200_ok ? "[OK]" : "[FAIL]");
        printf("HTTP: Received '%s' status ... %s\r\n", HTTP_HELLO_STR, found_hello ? "[OK]" : "[FAIL]");
        printf("HTTP: Received massage:\r\n\r\n");
        printf("%s", buffer);
    }

    sock.close();
    eth.disconnect();
    notify_completion(result);
    return 0;
}
