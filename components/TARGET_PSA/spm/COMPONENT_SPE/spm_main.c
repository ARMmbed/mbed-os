#if defined(COMPONENT_SPM_MAILBOX)

#include "ipc_queue.h"
extern ipc_consumer_queue_t *cons_queue;
// this is the dispatcher thread for dual-chip systems
int main(void)
{
    while (true) {
        ipc_queue_drain(cons_queue);
    }
}

#else

#error "Non dual-chip platforms are not yet supported"

#endif // defined(COMPONENT_SPM_MAILBOX)
