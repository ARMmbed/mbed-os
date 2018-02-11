
#include "mbed_poll.h"
#include "mbed_poll_stub.h"

int mbed_poll_stub::revents_value = POLLOUT;
int mbed_poll_stub::int_value = 0;

namespace mbed {

int poll(pollfh fhs[], unsigned nfhs, int timeout)
{
    fhs->revents = mbed_poll_stub::revents_value;
    return mbed_poll_stub::int_value;
}

}
