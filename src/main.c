/*
 * main.c
 */

#include "DSP2833x_Device.h"     // Device Header Include File
#include "DSP2833x_Examples.h"
#include "GPIO_Driver.h"
#include "usDelay.h"

/************************* CANopen includes **********************************/
#include "canfestival.h"

#define READY 1
#define SOFTSTART 2
#define RUN	3
#define SOFTOFF 4
#define FAULT 5
#define DEBUG 6

extern int timer0_interrupt;
extern void scic_msg(char *msg);
extern void configure_rs232(void);
extern void configureEcanB(void);
extern void initTimer();

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

	initTimer();

	configureEcanB();

	configure_rs232();

	// can bus testing
//	Uint32 i = 0;
//	for (i = 0;; i++) {
//		//      send_data (int16 MBXnbr, Uint32 low, Uint32 high, Uint32 id);
//		send_data(MBX_BIC, i, i + 1);
//		DELAY_US(1000000);
//	}
	unsigned int leds = 0;
	unsigned int keys, keys_old, keys_edge;

	unsigned char string_down[] = "1234567890x";
	unsigned char string_up[20];
	unsigned char val_down = 1;
	unsigned char val_up;
	unsigned char val_pdo = 1;

	setState(&ObjDict_Data, Initialisation); // Init the state
	setNodeId(&ObjDict_Data, 0x7F);
	setState(&ObjDict_Data, Operational); // Put the master in operational mode

	for (;;) {
		if (TRUE == timer0_interrupt) {

			timer0_interrupt = FALSE;

			//      static Message m = Message_Initializer;		// contain a CAN
			//      message

			//      if (canReceive(&m))			// a message received
			//        canDispatch(&ObjDict_Data, &m);         // process it

			if (keys_edge == 0) // edge on key 0
					{
				masterSendNMTstateChange(&ObjDict_Data, 0x00, NMT_Start_Node);
				//	startSYNC(&ObjDict_Data);
			}

			if (keys_edge == 1) // edge on key 1
					{
				masterSendNMTstateChange(&ObjDict_Data, 0x00, NMT_Reset_Node);
				//	stopSYNC(&ObjDict_Data);
			}

			if (keys == 2) // edge on key 2
					{
				if (!(DO1 <<= 1)) // generate running light
					DO1 = 1;
				sendPDOevent(&ObjDict_Data);
			}

			leds = DI1;

		}
	}

}

