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

// Global variable for this example
extern Uint32 ErrorCount;
extern Uint32 PassCount;
extern Uint32 MessageReceivedCount;

extern Uint32 TestMbox1;
extern Uint32 TestMbox2;
extern Uint32 TestMbox3;
extern struct ECAN_REGS ECanbShadow;

void
configureEcanB (void);

// Prototype statements for functions found within this file.
void
send_data (int16 MBXnbr, Uint32 low, Uint32 high);
static void
mailbox_check (int32 T1, int32 T2, int32 T3);
static void
mailbox_read (int16 i);
void
init_interrupt ();
void
UserSetInt ();

#endif /* CANBUS_H_ */
