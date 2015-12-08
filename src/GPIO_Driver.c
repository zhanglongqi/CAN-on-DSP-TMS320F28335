/*
 * EvMgr.c
 *
 *  Created on: Feb 3, 2012
 *      Author: Mike
 */
#include "DSP2833x_Device.h"     // Device Header Include File

//IO initialize.
void configureLED(void) {
	EALLOW;
	//set LED
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;  //GPIO26
	GpioCtrlRegs.GPADIR.bit.GPIO26 = 1;  //Output
	GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0; // Enable pull-up
	//
	EDIS;
}
