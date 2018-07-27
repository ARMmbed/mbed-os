
/** \addtogroup frameworks */
/** @{*/
#ifndef GREENTEA_METRICS_H
#define GREENTEA_METRICS_H

#ifdef MBED_TEST

/**
 *  Setup platform specific metrics
 */
void greentea_metrics_setup(void);

/**
 *  Report and cleanup platform specifc metrics
 */
void greentea_metrics_report(void);

#endif
#endif

/** @}*/
