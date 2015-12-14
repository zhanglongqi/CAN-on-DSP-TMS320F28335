/*
 * CanBus.h
 *
 *  Created on: Sep 9, 2013
 *      Author: Mike
 */

#ifndef CANBUS_H_
#define CANBUS_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_GlobalPrototypes.h"
#include "DSP2833x_Examples.h"
#include <stdio.h>

//mailbox number
//
#define BBB_ID 0x9555AAA0
#define MBX_RES 0 // resource manager
#define MBX_BIC 1 // bidirectional converter
#define MBX_PV 2 // PV converter
#define MBX_BAT 3 // Battery converter
#define MBX_WIN 4 // wind turbine converter
#define LED_INDEX 1

typedef union {
    float32 f;
    Uint16 c2[2];
} CAN_DATA_UNION;

// Global variable for this example
extern struct MBOX can_msg;
extern struct ECAN_REGS ECanbShadow;
extern int16 new_data;

#define TRUE 1
#define FALSE 0

void
configureEcanB(void);

// Prototype statements for functions found within this file.
void
send_data(int16 MBXnbr, char index, CAN_DATA_UNION data);
static void
mailbox_check(int32 T1, int32 T2, int32 T3);
static void
mailbox_read(int16 i);
void
init_interrupt();

interrupt void ecan1_intb_isr(void);

#endif /* CANBUS_H_ */
