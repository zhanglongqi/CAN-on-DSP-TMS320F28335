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

#define RES_HB_ID 0x9555AAA6 // resource manager
#define BIC_HB_ID 0x9555AAA7 // bidirectional converter
#define PV_HB_ID 0x9555AAA8 // PV converter
#define BAT_HB_ID 0x9555AAA9 // Battery converter
#define WIN_HB_ID 0x9555AAAA // wind turbine converter

#define BBB_ID_INDEX 0 // beaglebone black can bus id
#define RES_ID_INDEX 1 // resource manager
#define BIC_ID_INDEX 2 // bidirectional converter
#define PV_ID_INDEX 3 // PV converter
#define BAT_ID_INDEX 4 // Battery converter
#define WIN_ID_INDEX 5 // wind turbine converter

#define RES_HB_ID_INDEX 6 // resource manager
#define BIC_HB_ID_INDEX 7 // bidirectional converter
#define PV_HB_ID_INDEX 8 // PV converter
#define BAT_HB_ID_INDEX 9 // Battery converter
#define WIN_HB_ID_INDEX 10 // wind turbine converter

#define HEART_BEAT_INDEX 0
#define LED_INDEX 1
#define ON_OFF_INDEX 2

typedef union CAN_DATA_UNION {
	float32 f;
	Uint16 c2[2];
} CAN_DATA_UNION;
struct CAN_DATA {
	CAN_DATA_UNION data;
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
send_data(int16 MBXnbr, char index, struct CAN_DATA data);
static void
mailbox_check(int32 T1, int32 T2, int32 T3);
static void
mailbox_read(int16 i);
void
init_interrupt();

interrupt void ecan1_intb_isr(void);

#endif /* CANBUS_H_ */
