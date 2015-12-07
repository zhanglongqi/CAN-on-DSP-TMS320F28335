/*
 * timer.h
 *
 *  Created on: Dec 7, 2015
 *      Author: longqi
 */

#ifndef INCL_TIMER_H_
#define INCL_TIMER_H_
#include <DSP2833x_CpuTimers.h>
#include "DSP2833x_Examples.h"
#include "DSP2833x_Device.h"

void configureTimer0();
interrupt void cpu_timer0_isr(void);

#endif /* INCL_TIMER_H_ */
