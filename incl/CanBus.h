/*
 * CanBus.h
 *
 *  Created on: Sep 9, 2015
 *      Author: LongQi
 */

#ifndef CANBUS_H_
#define CANBUS_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_GlobalPrototypes.h"
#include "DSP2833x_Examples.h"

//mailbox number
//
#define BBB_ID 0x9555AAA0 // beaglebone black can bus id
#define RES_ID 0x9555AAA1 // resource manager
#define BIC_ID 0x9555AAA2 // bidirectional converter
#define PV_ID 0x9555AAA3 // PV converter
#define BAT_ID 0x9555AAA4 // Battery converter
#define WIN_ID 0x9555AAA5 // wind turbine converter

#define RES_FAULT_ID 0x95550006 // resource manager
#define BIC_FAULT_ID 0x95550007 // bidirectional converter
#define PV_FAULT_ID 0x95550008 // PV converter
#define BAT_FAULT_ID 0x95550009 // Battery converter
#define WIN_FAULT_ID 0x9555000A // wind turbine converter

#define BBB_ID_INDEX 0 // beaglebone black can bus id
#define RES_ID_INDEX 1 // resource manager
#define BIC_ID_INDEX 2 // bidirectional converter
#define PV_ID_INDEX 3 // PV converter
#define BAT_ID_INDEX 4 // Battery converter
#define WIN_ID_INDEX 5 // wind turbine converter

#define RES_FAULT_ID_INDEX 6 // resource manager
#define BIC_FAULT_ID_INDEX 7 // bidirectional converter
#define PV_FAULT_ID_INDEX 8 // PV converter
#define BAT_FAULT_ID_INDEX 9 // Battery converter
#define WIN_FAULT_ID_INDEX 10 // wind turbine converter

#define HEART_BEAT_INDEX 0
#define LED_GROUP_INDEX 1
#define ON_OFF_GROUP_INDEX 2


struct CAN_DATA {
	Uint32 id;
	int16 data0;
	int16 data1;
	int16 data2;
	int16 index;
};

// Global variable for this example
extern struct MBOX can_msg;
extern struct ECAN_REGS ECanbShadow;
extern int16 new_data;
extern struct CAN_DATA can_data;

#define TRUE 1
#define FALSE 0

#define BLINK_LED() GpioDataRegs.GPATOGGLE.bit.GPIO26 = 1
#define LED_ON() GpioDataRegs.GPASET.bit.GPIO26 = 1
#define LED_OFF() GpioDataRegs.GPACLEAR.bit.GPIO26 = 1
#define LED_STATUS() GpioDataRegs.GPADAT.bit.GPIO26

void
configureEcanB(void);

// Prototype statements for functions found within this file.
void
send_data(int16 MBXnbr, struct CAN_DATA data);
static void
mailbox_check(int32 T1, int32 T2, int32 T3);
static void
mailbox_read(int16 i);
void
init_interrupt();

interrupt void ecan1_intb_isr(void);

#endif /* CANBUS_H_ */
