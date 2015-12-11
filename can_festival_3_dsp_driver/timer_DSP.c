/*
 This file is part of CanFestival, a library implementing CanOpen Stack.

 Copyright (C): Edouard TISSERANT and Francis DUPIN
 AT91 Port: Peter CHRISTEN

 See COPYING file for copyrights details.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

// Includes for the Canfestival driver
#include "canfestival.h"
#include "timer.h"


#include "DSP2833x_CpuTimers.h"
#include "DSP2833x_Examples.h"
#include "DSP2833x_Device.h"


// Define the timer registers
#define TimerAlarm      CpuTimer0.PeriodInUSec
#define TimerCounter    CpuTimer0.InterruptCount

int timer0_interrupt = FALSE;

void timer_can_irq_handler(void);

/************************** Modul variables **********************************/
// Store the last timer value to calculate the elapsed time
static TIMEVAL last_time_set = TIMEVAL_MAX;

void initTimer(void)
/******************************************************************************
 Initializes the timer, turn on the interrupt and put the interrupt time to zero
 INPUT	void
 OUTPUT	void
 ******************************************************************************/{
	DINT;
	// Disable CPU interrupts

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;
	PieVectTable.TINT0 = &timer_can_irq_handler;			//Timer 0
	EDIS;

	InitCpuTimers();

#if (CPU_FRQ_150MHZ)
// Configure CPU-Timer 0 to interrupt every millisecond:
// 150MHz CPU Freq, 1 millisecond Period (in uSeconds)

	ConfigCpuTimer(&CpuTimer0, 150, 1000);

#endif

	// To ensure precise timing, use write-only instructions to write to the entire register. Therefore, if any
	// of the configuration bits are changed in ConfigCpuTimer and InitCpuTimers (in DSP2833x_CpuTimers.h), the
	// below settings must also be updated.

	CpuTimer0Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0

	// Enable CPU int1 which is connected to CPU-Timer 0,
	IER |= M_INT1;

	// Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

	// Enable global Interrupts and higher priority real-time debug events:
	EINT;
	// Enable Global interrupt INTM
	ERTM;
	// Enable Global realtime interrupt DBGM
	StartCpuTimer0();
}

void setTimer(TIMEVAL value)
/******************************************************************************
 Set the timer for the next alarm.
 INPUT	value TIMEVAL (unsigned long)
 OUTPUT	void
 ******************************************************************************/{
	TimerAlarm += value; // Add the desired time to timer interrupt time
}

TIMEVAL getElapsedTime(void)
/******************************************************************************
 Return the elapsed time to tell the stack how much time is spent since last call.
 INPUT	void
 OUTPUT	value TIMEVAL (unsigned long) the elapsed time
 ******************************************************************************/{
	unsigned int timer = TimerCounter; // Copy the value of the running timer
	// Calculate the time difference
	return timer > last_time_set ? timer - last_time_set : last_time_set - timer;
}

//*----------------------------------------------------------------------------
//* Function Name       : timer_can_irq_handler
//* Object              : C handler interrupt function by the interrupts
//*                       assembling routine
//* Output Parameters   : calls TimeDispatch
//*----------------------------------------------------------------------------

void timer_can_irq_handler(void) {

	timer0_interrupt = TRUE;
	CpuTimer0.InterruptCount++;

	// Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
	TimeDispatch(); // Call the time handler of the stack to adapt the elapsed time
}

