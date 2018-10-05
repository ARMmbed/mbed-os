
#ifndef ETHERNETEXT_H
#define ETHERNETEXT_H

#ifdef __cplusplus
extern "C" {
#endif

/* PHY link mode */
#define NEGO_FAIL               (0)
#define HALF_10M                (1)
#define FULL_10M                (2)
#define HALF_TX                 (3)
#define FULL_TX                 (4)

typedef void (ethernetext_cb_fnc)(void);

typedef struct tag_ethernet_cfg {
    int                  int_priority;
    ethernetext_cb_fnc   *recv_cb;
    char                 *ether_mac;
} ethernet_cfg_t;

extern int ethernetext_init(ethernet_cfg_t *p_ethcfg);
extern void ethernetext_start_stop(int32_t mode);
extern int ethernetext_chk_link_mode(void);
extern void ethernetext_set_link_mode(int32_t link);
extern void ethernetext_add_multicast_group(const uint8_t *addr);
extern void ethernetext_remove_multicast_group(const uint8_t *addr);
extern void ethernetext_set_all_multicast(int all);
#ifdef __cplusplus
}
#endif

#endif
