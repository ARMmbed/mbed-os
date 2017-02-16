#ifndef _RTW_INTFS_H_
#define _RTW_INTFS_H_

extern u8 rtw_init_default_value(_adapter *padapter);
#ifdef CONFIG_WOWLAN
void rtw_cancel_dynamic_chk_timer(_adapter *padapter);
#endif
extern void rtw_cancel_all_timer(_adapter *padapter);

extern u8 rtw_init_drv_sw(_adapter *padapter);
extern u8 rtw_free_drv_sw(_adapter *padapter);
extern u8 rtw_reset_drv_sw(_adapter *padapter);

extern int rtw_drv_init(ADAPTER *padapter);
extern void rtw_drv_deinit(ADAPTER *Adapter);

extern u32 rtw_start_drv_threads(_adapter *padapter);
extern void rtw_stop_drv_threads (_adapter *padapter);

#endif //_RTW_INTFS_H_
