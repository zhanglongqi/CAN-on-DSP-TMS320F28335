/*
 * CanBus.h
 *
 *  Created on: Sep 9, 2013
 *      Author: Mike
 */

#ifndef CANTEST_H_
#define CANBUS_H_

#include "DSP2833x_Device.h"
#include "usDelay.h"
#include "DSP2833x_ECan.h"

// Global variable for this example
extern Uint32 ErrorCount;
extern Uint32 PassCount;
extern Uint32 MessageReceivedCount;

extern Uint32 TestMbox1 = 0;
extern Uint32 TestMbox2 = 0;
extern Uint32 TestMbox3 = 0;
extern struct ECAN_REGS ECanaShadow;

void
configureEcanB (void);

// Prototype statements for functions found within this file.
static void
mailbox_check (int32 T1, int32 T2, int32 T3);
static void
mailbox_read (int16 i);
void
init_interrupt ();
void
UserSetInt ();
#endif /* CANTEST_H_ */
