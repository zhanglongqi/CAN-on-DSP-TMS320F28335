#include "DSP2833x_Device.h"     // Device Header Include File
#include "CanTest.h"
#include "usDelay.h"
#include "DSP2833x_ECan.h"
#define BLINK_LED() GpioDataRegs.GPATOGGLE.bit.GPIO26 = 1

// Global variable for this example
Uint32 ErrorCount;
Uint32 PassCount;
Uint32 MessageReceivedCount;

Uint32 TestMbox1;
Uint32 TestMbox2;
Uint32 TestMbox3;

struct ECAN_REGS ECanbShadow;

// Prototype statements for functions found within this file.
static void
mailbox_check (int32 T1, int32 T2, int32 T3);
static void
mailbox_read (int16 i);
void
init_interrupt ();
void
UserSetInt ();

void
configureEcanB (void)
{
  MessageReceivedCount = 0;
  ErrorCount = 0;
  PassCount = 0;

  TestMbox1 = 0;
  TestMbox2 = 0;
  TestMbox3 = 0;

  UserSetInt (); // Initialize interrupt and enable interrupt

  InitECanb ();
  // Mailboxs can be written to 16-bits or 32-bits at a time
  // Write to the MSGID field of TRANSMIT mailboxes MBOX0 - 15
  ECanbMboxes.MBOX0.MSGID.all = 0x9555AAA0;
  ECanbMboxes.MBOX1.MSGID.all = 0x9555AAA1;
  ECanbMboxes.MBOX2.MSGID.all = 0x9555AAA2;
  ECanbMboxes.MBOX3.MSGID.all = 0x9555AAA3;
  ECanbMboxes.MBOX4.MSGID.all = 0x9555AAA4;
  ECanbMboxes.MBOX5.MSGID.all = 0x9555AAA5;
  ECanbMboxes.MBOX6.MSGID.all = 0x9555AAA6;
  ECanbMboxes.MBOX7.MSGID.all = 0x9555AAA7;
  ECanbMboxes.MBOX8.MSGID.all = 0x9555AAA8;
  ECanbMboxes.MBOX9.MSGID.all = 0x9555AAA9;
  ECanbMboxes.MBOX10.MSGID.all = 0x9555AAAA;
  ECanbMboxes.MBOX11.MSGID.all = 0x9555AAAB;
  ECanbMboxes.MBOX12.MSGID.all = 0x9555AAAC;
  ECanbMboxes.MBOX13.MSGID.all = 0x9555AAAD;
  ECanbMboxes.MBOX14.MSGID.all = 0x9555AAAE;
  ECanbMboxes.MBOX15.MSGID.all = 0x9555AAAF;

  // Write to the MSGID field of RECEIVE mailboxes MBOX16 - 31
  ECanbMboxes.MBOX16.MSGID.all = 0x9555AAA0;
  ECanbMboxes.MBOX17.MSGID.all = 0x9555AAA1;
  ECanbMboxes.MBOX18.MSGID.all = 0x9555AAA2;
  ECanbMboxes.MBOX19.MSGID.all = 0x9555AAA3;
  ECanbMboxes.MBOX20.MSGID.all = 0x9555AAA4;
  ECanbMboxes.MBOX21.MSGID.all = 0x9555AAA5;
  ECanbMboxes.MBOX22.MSGID.all = 0x9555AAA6;
  ECanbMboxes.MBOX23.MSGID.all = 0x9555AAA7;
  ECanbMboxes.MBOX24.MSGID.all = 0x9555AAA8;
  ECanbMboxes.MBOX25.MSGID.all = 0x9555AAA9;
  ECanbMboxes.MBOX26.MSGID.all = 0x9555AAAA;
  ECanbMboxes.MBOX27.MSGID.all = 0x9555AAAB;
  ECanbMboxes.MBOX28.MSGID.all = 0x9555AAAC;
  ECanbMboxes.MBOX29.MSGID.all = 0x9555AAAD;
  ECanbMboxes.MBOX30.MSGID.all = 0x9555AAAE;
  ECanbMboxes.MBOX31.MSGID.all = 0x9555AAAF;

  // Configure Mailboxes 0-15 as Tx, 16-31 as Rx
  // Since this write is to the entire register (instead of a bit
  // field) a shadow register is not required.
  ECanbRegs.CANMD.all = 0xFFFF0000;

  // Enable all Mailboxes */
  // Since this write is to the entire register (instead of a bit
  // field) a shadow register is not required.
  ECanbRegs.CANME.all = 0xFFFFFFFF;

  // Specify that 8 bits will be sent/received
  ECanbMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX1.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX2.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX3.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX4.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX5.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX6.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX7.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX8.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX9.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX10.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX11.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX12.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX13.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX14.MSGCTRL.bit.DLC = 8;
  ECanbMboxes.MBOX15.MSGCTRL.bit.DLC = 8;

  // Write to the mailbox RAM field of MBOX0 - 15
  ECanbMboxes.MBOX0.MDL.all = 0x0000AAA0;
  ECanbMboxes.MBOX0.MDH.all = 0x89ABCDEF;

  ECanbMboxes.MBOX1.MDL.all = 0x9555AAA1;
  ECanbMboxes.MBOX1.MDH.all = 0x89ABCDEF;

//	ECanbMboxes.MBOX2.MDL.all = 0x9555AAA2;
//	ECanbMboxes.MBOX2.MDH.all = 0x89ABCDAA;
//
//	ECanbMboxes.MBOX3.MDL.all = 0x9555AAA3;
//	ECanbMboxes.MBOX3.MDH.all = 0x89ABCDEF;
//
//	ECanbMboxes.MBOX4.MDL.all = 0x9555AAA4;
//	ECanbMboxes.MBOX4.MDH.all = 0x89ABCDEF;
//
//	ECanbMboxes.MBOX5.MDL.all = 0x9555AAA5;
//	ECanbMboxes.MBOX5.MDH.all = 0x89ABCDEF;
//
//	ECanbMboxes.MBOX6.MDL.all = 0x9555AAA6;
//	ECanbMboxes.MBOX6.MDH.all = 0x89ABCDEF;
//
//	ECanbMboxes.MBOX7.MDL.all = 0x9555AAA7;
//	ECanbMboxes.MBOX7.MDH.all = 0x89ABCDEF;
//
//	ECanbMboxes.MBOX8.MDL.all = 0x9555AAA8;
//	ECanbMboxes.MBOX8.MDH.all = 0x89ABCDEF;
//
//	ECanbMboxes.MBOX9.MDL.all = 0x9555AAA9;
//	ECanbMboxes.MBOX9.MDH.all = 0x89ABCDEF;
//
//	ECanbMboxes.MBOX10.MDL.all = 0x9555AAAA;
//	ECanbMboxes.MBOX10.MDH.all = 0x89ABCDEF;
//
//	ECanbMboxes.MBOX11.MDL.all = 0x9555AAAB;
//	ECanbMboxes.MBOX11.MDH.all = 0x89ABCDEF;
//
//	ECanbMboxes.MBOX12.MDL.all = 0x9555AAAC;
//	ECanbMboxes.MBOX12.MDH.all = 0x89ABCDEF;
//
//	ECanbMboxes.MBOX13.MDL.all = 0x9555AAAD;
//	ECanbMboxes.MBOX13.MDH.all = 0x89ABCDEF;
//
//	ECanbMboxes.MBOX14.MDL.all = 0x9555AAAE;
//	ECanbMboxes.MBOX14.MDH.all = 0x89ABCDEF;
//
//	ECanbMboxes.MBOX15.MDL.all = 0x9555AAAF;
//	ECanbMboxes.MBOX15.MDH.all = 0x89ABCDEF;

  // Since this write is to the entire register (instead of a bit
  // field) a shadow register is not required.
  EALLOW;
  ECanbRegs.CANMIM.all = 0xFFFFFFFF;

  // Configure the eCAN for self test mode
  // Enable the enhanced features of the eCAN.
  EALLOW;
  ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
  ECanbShadow.CANMC.bit.STM = 1;    // Configure CAN for self-test mode
  ECanbShadow.CANMC.bit.STM = 0;    // Configure CAN for normal mode
  ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;
  EDIS;

  //Initial for interupt
  EALLOW;
//  ECanbRegs.CANMIM.all = 0x04;
  ECanbRegs.CANMIM.all = 0xFFFFFFFF;
  EDIS;

  EALLOW;
//  ECanbRegs.CANMIL.all = 0x04;
  ECanbRegs.CANMIL.all = 0xFFFFFFFF;
  EDIS;

  EALLOW;
  ECanbShadow.CANGIF1.all = ECanbRegs.CANGIF1.all;
  ECanbShadow.CANGIF1.bit.GMIF1 = 1;
  ECanbRegs.CANGIF1.all = ECanbShadow.CANGIF1.all;
  EDIS;

  EALLOW;
  ECanbShadow.CANGIM.all = ECanbRegs.CANGIM.all;
  ECanbShadow.CANGIM.bit.GIL = 1;
  ECanbShadow.CANGIM.bit.I1EN = 1;
  ECanbRegs.CANGIM.all = ECanbShadow.CANGIM.all;
  EDIS;
}

void
testCan ()
{
//  Uint16 j;
//  ECanbRegs.CANTRS.all = 0x00000003;  // Set TRS for all transmit mailboxes
//  DELAY_US(100000);
//  do
//    {
//      ECanbShadow.CANTA.all = ECanbRegs.CANTA.all;
//    }
//  while (ECanbShadow.CANTA.all == 0);   // Wait for TA5 bit to be set..
//
////	while (ECanbRegs.CANTA.all != 0x0000FFFF) {
////	}  // Wait for all TAn bits to be set..
//  ECanbRegs.CANTA.all = 0x0000FFFF;   // Clear all TAn

  BLINK_LED();

}

// This function reads out the contents of the indicated
// by the Mailbox number (MBXnbr).
static void
mailbox_read (int16 MBXnbr)
{
  volatile struct MBOX *Mailbox;
  Mailbox = &ECanbMboxes.MBOX0 + MBXnbr;
  TestMbox1 = Mailbox->MDL.all; // = 0x9555AAAn (n is the MBX number)
  TestMbox2 = Mailbox->MDH.all; // = 0x89ABCDEF (a constant)
  TestMbox3 = Mailbox->MSGID.all; // = 0x9555AAAn (n is the MBX number)

} // MSGID of a rcv MBX is transmitted as the MDL data.

static void
mailbox_check (int32 T1, int32 T2, int32 T3)
{
  if ((T1 != T3) || (T2 != 0x89ABCDEF))
    {
      ErrorCount++;
    }
  else
    {
      PassCount++;
    }
}

interrupt void
ecan1_intb_isr (void)
{
  int j;
  for (j = 16; j < 32; j++)         // Read & check 16 mailboxes
    {
      mailbox_read (j);         // This func reads the indicated mailbox data
      mailbox_check (TestMbox1, TestMbox2, TestMbox3); // Checks the received data
    }

  ECanbRegs.CANRMP.bit.RMP2 = 1;   // Clear all TAn

  PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

void
UserSetInt ()
{

  InitPieCtrl ();           //Disable PIE interrupt, Clear interrupt flag

  IER = 0x0000;           //Disable CPU interrupt, Clear interrupt flag
  IFR = 0x0000;

  InitPieVectTable (); //// This function initializes the PIE vector table to a known state.

  EALLOW;
//  PieVectTable.EPWM5_INT = &User_EPWM1_isr;	//PWM5 interrupt	//New
//  PieVectTable.TINT0 = &cpu_timer0_isr;			//Timer 0
//  PieVectTable.SCIRXINTC = &scic_rx_isr;			//SCI-C
//  PieVectTable.XINT13 = &cpu_timer1_isr;
//  PieVectTable.TINT2 = &cpu_timer2_isr;
  PieVectTable.ECAN1INTB = &ecan1_intb_isr;		//ECAN interrupt

  EDIS;

  IER |= M_INT1;
  IER |= M_INT3;
  IER |= M_INT8;	//RXD-C
  IER |= M_INT13;
  IER |= M_INT14;
  IER |= M_INT9;

  PieCtrlRegs.PIEIER1.bit.INTx7 = 1;	//timer0
  PieCtrlRegs.PIEIER3.bit.INTx5 = 1;  	//enable ePWM5_INT		//New
  PieCtrlRegs.PIEIER8.bit.INTx5 = 1;  // RXD-C
  PieCtrlRegs.PIEIER9.bit.INTx8 = 1;  // ECAN-B

  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

  // Enable global Interrupts and higher priority real-time debug events:
  EINT;
  // Enable Global interrupt INTM
  ERTM;
  // Enable Global realtime interrupt DBGM
}

