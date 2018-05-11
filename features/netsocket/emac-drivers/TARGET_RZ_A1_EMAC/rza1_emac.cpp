#include "cmsis_os.h"
#include "netsocket/nsapi_types.h"
#include "mbed_shared_queues.h"
#include "ethernet_api.h"
#include "ethernetext_api.h"
#include "rza1_emac.h"

#define RZ_A1_ETH_IF_NAME    "en"

// Weak so a module can override
MBED_WEAK EMAC &EMAC::get_default_instance() {
    return RZ_A1_EMAC::get_instance();
}

RZ_A1_EMAC &RZ_A1_EMAC::get_instance() {
    static RZ_A1_EMAC emac;
    return emac;
}

RZ_A1_EMAC::RZ_A1_EMAC() : hwaddr(), hwaddr_set(false), power_on(false), connect_sts(false),
     link_mode_last(NEGO_FAIL), recvThread(osPriorityNormal, 896)
{
}

uint32_t RZ_A1_EMAC::get_mtu_size() const
{
    return 1500;
}

uint32_t RZ_A1_EMAC::get_align_preference() const
{
    return 0;
}

void RZ_A1_EMAC::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, RZ_A1_ETH_IF_NAME, (size < sizeof(RZ_A1_ETH_IF_NAME)) ? size : sizeof(RZ_A1_ETH_IF_NAME));
}

uint8_t RZ_A1_EMAC::get_hwaddr_size() const
{
    return 6;
}

bool RZ_A1_EMAC::get_hwaddr(uint8_t *addr) const
{
    return false;
}

void RZ_A1_EMAC::set_hwaddr(const uint8_t *addr)
{
    memcpy(hwaddr, addr, sizeof(hwaddr));
    hwaddr_set = true;

    /* Reconnect */
    if (power_on != false) {
        ethernet_cfg_t ethcfg;
        ethcfg.int_priority = 6;
        ethcfg.recv_cb      = &_recv_callback;
        ethcfg.ether_mac    = NULL;
        ethcfg.ether_mac = (char *)hwaddr;
        ethernetext_init(&ethcfg);
    }
}

bool RZ_A1_EMAC::link_out(emac_mem_buf_t *buf)
{
    emac_mem_buf_t *copy_buf = buf;
    uint32_t       retry_cnt;
    bool           result   = false;
    int            write_size;
    int            total_write_size = 0;

    while ((copy_buf != NULL) && (memory_manager->get_ptr(copy_buf) != NULL) && (memory_manager->get_len(copy_buf) != 0)) {
        for (retry_cnt = 0; retry_cnt < 100; retry_cnt++) {
            write_size = ethernet_write((char *)memory_manager->get_ptr(copy_buf), memory_manager->get_len(copy_buf));
            if (write_size != 0) {
                total_write_size += write_size;
                break;
            }
            osDelay(1);
        }
        copy_buf = memory_manager->get_next(copy_buf);
    }
    memory_manager->free(buf);

    if (total_write_size > 0) {
        if (ethernet_send() == 1) {
            result = true;
        }
    }

    return result;
}

bool RZ_A1_EMAC::power_up()
{
    if (power_on != false) {
        return true;
    }

    ethernet_cfg_t ethcfg;
    ethcfg.int_priority = 6;
    ethcfg.recv_cb      = &_recv_callback;
    ethcfg.ether_mac    = NULL;
    if (hwaddr_set) {
        ethcfg.ether_mac = (char *)hwaddr;
    }
    ethernetext_init(&ethcfg);

    /* task */
    recvThread.start(mbed::callback(this, &RZ_A1_EMAC::recv_task));
    phy_task_handle = mbed::mbed_event_queue()->call_every(200, mbed::callback(this, &RZ_A1_EMAC::phy_task));

    power_on = true;
    return true;
}

void RZ_A1_EMAC::power_down()
{
    power_on = false;
}

void RZ_A1_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    emac_link_input_cb = input_cb;
}

void RZ_A1_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    emac_link_state_cb = state_cb;
}

void RZ_A1_EMAC::add_multicast_group(const uint8_t *addr)
{
    ethernetext_add_multicast_group(addr);
}

void RZ_A1_EMAC::remove_multicast_group(const uint8_t *addr)
{
    ethernetext_remove_multicast_group(addr);
}

void RZ_A1_EMAC::set_all_multicast(bool all)
{
    ethernetext_set_all_multicast(all);
}

void RZ_A1_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}


void RZ_A1_EMAC::_recv_callback(void) {
    get_instance().recv_callback();
}

void RZ_A1_EMAC::recv_callback(void) {
    recvThread.signal_set(1);
}

void RZ_A1_EMAC::recv_task(void) {
    uint16_t       recv_size;
    emac_mem_buf_t *buf;
    int            cnt;

    while (1) {
        rtos::Thread::signal_wait(1);
        for (cnt = 0; cnt < 16; cnt++) {
            recv_size = ethernet_receive();
            if (recv_size == 0) {
                break;
            }
            buf = memory_manager->alloc_heap(recv_size, 0);
            if (buf != NULL) {
                (void)ethernet_read((char *)memory_manager->get_ptr(buf), memory_manager->get_len(buf));
                emac_link_input_cb(buf);
            }
        }
    }
}

void RZ_A1_EMAC::phy_task(void)
{
    if (ethernet_link() == 1) {
        int link_mode = ethernetext_chk_link_mode();
        if (link_mode != link_mode_last) {
            if (connect_sts != false) {
                emac_link_state_cb(false);
            }
            if (link_mode != NEGO_FAIL) {
                ethernetext_set_link_mode(link_mode);
                emac_link_state_cb(true);
                connect_sts = true;
            }
            link_mode_last = link_mode;
        }
    } else {
        if (connect_sts != false) {
            emac_link_state_cb(false);
            link_mode_last = NEGO_FAIL;
            connect_sts   = false;
        }
    }
}

