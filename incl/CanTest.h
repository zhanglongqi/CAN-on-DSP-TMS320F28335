/*
 * CanTest.h
 *
 *  Created on: Sep 9, 2013
 *      Author: Mike
 */


#ifndef CANTEST_H_
#define CANTEST_H_


// Global variable for this example
extern Uint32  ErrorCount;
extern Uint32  PassCount;
extern Uint32  MessageReceivedCount;

extern Uint32  TestMbox1 = 0;
extern Uint32  TestMbox2 = 0;
extern Uint32  TestMbox3 = 0;
extern struct ECAN_REGS ECanaShadow;

void InitCanTest(void);
void testCan() ;

#endif /* CANTEST_H_ */
