

void am_ctimer_isr(void){
  am_hal_ctimer_int_service(am_hal_ctimer_int_status_get(true)); // get enabled interrupt status and then service those only
}