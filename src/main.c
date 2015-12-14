/*
 * main.c
 */

#include "DSP2833x_Device.h"     // Device Header Include File
#include "DSP2833x_Examples.h"
#include "GPIO_Driver.h"
#include "usDelay.h"

#include <CanBus.h>
#include <timer.h>

#define READY 1
#define SOFTSTART 2
#define RUN	3
#define SOFTOFF 4
#define FAULT 5
#define DEBUG 6

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

	//	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

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

		if (new_data) {
			if (can_msg.MSGID.all == BBB_ID) {
				CAN_DATA_UNION data;
				data.f=39978.3;

				data.c2[0] = can_msg.MDL.word.LOW_WORD;
				data.c2[1] = can_msg.MDL.word.HI_WORD;

				if (can_msg.MDH.byte.BYTE4 == LED_INDEX) {
					if (data.f >= 1)
						LED_ON();
					else
						LED_OFF();
				}
			}
			new_data = FALSE;
		}
	}
}

