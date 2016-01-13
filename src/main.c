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

		DELAY_US(100000);

		//send data begin
		struct CAN_DATA can_data_to_send_1;
		can_data_to_send_1.data.f = LED_STATUS();
		send_data(BIC_ID_INDEX, LED_INDEX, can_data_to_send_1);
		//send end

		//receive data begin
		if (new_data) { // check if new data come
			if (can_msg.MSGID.all == BIC_ID) { //

				can_data.data.c2[0] = can_msg.MDL.word.LOW_WORD;
				can_data.data.c2[1] = can_msg.MDL.word.HI_WORD;

				switch (can_msg.MDH.byte.BYTE4) {
				case LED_INDEX:
					if (can_data.data.f >= 1)
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

