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

#define Rectifier_1_ID 0x80000001
#define Inverter_1_ID 0x80000002

#define Rectifier_2_ID 0x80000003
#define Inverter_2_ID 0x80000004

#define Rectifier_3_ID 0x80000005
#define Inverter_3_ID 0x80000006

#define Rectifier_4_ID 0x80000007
#define Inverter_4_ID 0x80000008


#define Rectifier_1_ID_Index 0
#define Inverter_1_ID_Index 1

#define Rectifier_2_ID_Index 2
#define Inverter_2_ID_Index 3

#define Rectifier_3_ID_Index 4
#define Inverter_3_ID_Index 5

#define Rectifier_4_ID_Index 6
#define Inverter_4_ID_Index 7


#define HEART_BEAT_INDEX 0
#define LED_GROUP_INDEX 1
#define ON_OFF_GROUP_INDEX 2


struct CAN_DATA {
	Uint32 id;
	Uint16 data0;
	Uint16 data1;
	Uint16 data2;
	Uint16 index;
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
