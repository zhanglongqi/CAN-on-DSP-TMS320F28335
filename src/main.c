/*
 * main.c
 *
 *  Created on: Sep 9, 2015
 *      Author: LongQi
 */

#include "DSP2833x_Device.h"     // Device Header Include File
#include "DSP2833x_Examples.h"
#include "GPIO_Driver.h"
#include "usDelay.h"

#include "CanBus.h"
#include "timer.h"

void main(void) {

	//0. init Sys
	InitSysCtrl();
	InitPieCtrl();

	//1. =========================interrupt=============================================
	// Disable CPU interrupts and clear all CPU interrupt flags:
	DINT;
	IER = 0x0000;
	IFR = 0x0000;

	//ram Funcs

	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

	InitFlash();
	// Initialize the PIE vector table with pointers to the shell Interrupt
	// Service Routines (ISR).
	// This will populate the entire table, even if the interrupt
	// is not used in this example.  This is useful for debug purposes.
	// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
	// This function is found in DSP2833x_PieVect.c.c0
	InitPieVectTable();

	EINT;
	// Enable Global interrupt INTM
	ERTM;
	// Enable Global realtime interrupt DBGM

	//===============user module=======================

	configureLED();

	configureTimer0();

	configureEcanB();

	for (;;) {

		DELAY_US(1000000);

		//send data begin
		struct CAN_DATA can_data_to_send_1;
		can_data_to_send_1.data0 = LED_STATUS();
		can_data_to_send_1.data1 = LED_STATUS() + 1;
		can_data_to_send_1.data2 = LED_STATUS() + 2;
		can_data_to_send_1.index = LED_GROUP_INDEX;
		send_data(BIC_ID_INDEX, can_data_to_send_1);

		//receive data begin
		if (new_data) { // check if new data come
			if (can_data.id == BIC_ID) { //

				switch (can_data.index) {
				case LED_GROUP_INDEX:
					if (can_data.data0 % 2 == 0)
						LED_ON();
					else
						LED_OFF();
				}
			}
			new_data = FALSE;
		}
		//receive data end
	}
}

