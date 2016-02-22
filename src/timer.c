/*
 * timer.c
 *
 *  Created on: Dec 7, 2015
 *      Author: longqi
 */
#include "timer.h"
#include "CanBus.h"

float32 heartbeat = 0;

void configureTimer0() {
	DINT;
	// Disable CPU interrupts

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;
	PieVectTable.TINT0 = &cpu_timer0_isr;			//Timer 0
	EDIS;

	InitCpuTimers();

#if (CPU_FRQ_150MHZ)
// Configure CPU-Timer 0 to interrupt every second:
// 150MHz CPU Freq, 1 second Period (in uSeconds)

	ConfigCpuTimer(&CpuTimer0, 150, 1000000);

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

interrupt void cpu_timer0_isr(void) {

	CpuTimer0.InterruptCount++;

	// Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

	heartbeat += 1;
	struct CAN_DATA data_to_send;

	data_to_send.data0 = heartbeat + 0;
	data_to_send.data1 = heartbeat + 1;
	data_to_send.data2 = heartbeat + 2;

	for (data_to_send.index = 1; data_to_send.index <= 12;
			data_to_send.index++) {
		send_data(Rectifier_1_ID_Index, data_to_send);
		send_data(Inverter_1_ID_Index, data_to_send);

		send_data(Rectifier_2_ID_Index, data_to_send);
		send_data(Inverter_2_ID_Index, data_to_send);

//		send_data(Rectifier_3_ID_Index, data_to_send);
//		send_data(Inverter_3_ID_Index, data_to_send);
//
//		send_data(Rectifier_4_ID_Index, data_to_send);
//		send_data(Inverter_4_ID_Index, data_to_send);
	}
}
