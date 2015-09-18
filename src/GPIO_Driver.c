/*
 * EvMgr.c
 *
 *  Created on: Feb 3, 2012
 *      Author: Mike
 */
#include "DSP2833x_Device.h"     // Device Header Include File

//IO initialize.
void InitGpio(void) {
	EALLOW;

	// Each GPIO pin can be:
	// a) a GPIO input/output
	// b) peripheral function 1
	// c) peripheral function 2
	// d) peripheral function 3

	// By default, all are GPIO Inputs
	GpioCtrlRegs.GPAMUX1.all = 0x0000;     // GPIO functionality GPIO0-GPIO15
	GpioCtrlRegs.GPAMUX2.all = 0x0000;     // GPIO functionality GPIO16-GPIO31
	GpioCtrlRegs.GPBMUX1.all = 0x0000;     // GPIO functionality GPIO32-GPIO39
	GpioCtrlRegs.GPBMUX2.all = 0x0000;     // GPIO functionality GPIO48-GPIO63
	GpioCtrlRegs.GPCMUX1.all = 0x0000;     // GPIO functionality GPIO64-GPIO79
	GpioCtrlRegs.GPCMUX2.all = 0x0000;     // GPIO functionality GPIO80-GPIO95

	GpioCtrlRegs.GPADIR.all = 0x0000;      // GPIO0-GPIO31 are inputs
	GpioCtrlRegs.GPBDIR.all = 0x0000;      // GPIO32-GPIO63 are inputs
	GpioCtrlRegs.GPCDIR.all = 0x0000;      // GPI064-GPIO95 are inputs

	// Each input can have different qualification
	// a) input synchronized to SYSCLKOUT
	// b) input qualified by a sampling window
	// c) input sent asynchronously (valid for peripheral inputs only)
	GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15 Synch to SYSCLKOUT
	GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31 Synch to SYSCLKOUT
	GpioCtrlRegs.GPBQSEL1.all = 0x0000;    // GPIO32-GPIO39 Synch to SYSCLKOUT
	GpioCtrlRegs.GPBQSEL2.all = 0x0000;    // GPIO48-GPIO63 Synch to SYSCLKOUT

	// Pull-ups can be enabled or disabled.
	// GpioCtrlRegs.GPAPUD.all = 0x0000;      // Pullup's enabled GPIO0-GPIO31
	// GpioCtrlRegs.GPBPUD.all = 0x0000;      // Pullup's enabled GPIO32-GPIO63
	// GpioCtrlRegs.GPCPUD.all = 0x0000;      // Pullup's enabled GPIO64-GPIO79

	GpioCtrlRegs.GPAPUD.all = 0xFFFFFFFF;    // Pullup's disabled GPIO0-GPIO31
	GpioCtrlRegs.GPBPUD.all = 0xFFFFFFFF;    // Pullup's disabled GPIO32-GPIO34
	GpioCtrlRegs.GPCPUD.all = 0xFFFFFFFF;     // Pullup's disabled GPIO64-GPIO79

	//test for GPIO57

	//===============GPIO for Test============
//   GpioCtrlRegs.GPADIR.bit.GPIO5 = 1 ; //output
	GpioCtrlRegs.GPBDIR.bit.GPIO62 = 1; //output
	GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1; //output
	GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1; //output
//   GpioCtrlRegs.GPADIR.bit.GPIO6 = 1 ;	//output

	//set LED
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;  //GPIO26
	GpioCtrlRegs.GPADIR.bit.GPIO26 = 1;  //Output
	GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0; // Enable pull-up
	//
	EDIS;
}
