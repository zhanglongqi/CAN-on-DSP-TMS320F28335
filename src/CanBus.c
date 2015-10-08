#include <CanBus.h>

#define BLINK_LED() GpioDataRegs.GPATOGGLE.bit.GPIO26 = 1

// Global variable for this example
Uint32 ErrorCount;
Uint32 PassCount;
Uint32 MessageReceivedCount;
Uint32 TestMbox1 = 0;
Uint32 TestMbox2 = 0;
Uint32 TestMbox3 = 0;
struct ECAN_REGS ECanbShadow;

void
configureEcanB (void)
{
  MessageReceivedCount = 0;
  ErrorCount = 0;
  PassCount = 0;

  TestMbox1 = 0;
  TestMbox2 = 0;
  TestMbox3 = 0;

  InitECanbGpio ();

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
  ECanbRegs.CANME.all = 0xFFFFFFFF; // after this message id can not be changed

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

  // enable sys clock for ecanb
  EALLOW;
  SysCtrlRegs.PCLKCR0.bit.ECANBENCLK = 1;    // eCAN-B
  EDIS;

  // Configure the eCAN for self test mode
  // Enable the enhanced features of the eCAN.
  EALLOW;
  ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
  ECanbShadow.CANMC.bit.STM = 1;    // Configure CAN for self-test mode
  ECanbShadow.CANMC.bit.STM = 0;    // Configure CAN for normal mode
  ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;
  EDIS;

  //Initial for interupt for receiving event only
  EALLOW;
  ECanbRegs.CANMIM.all = 0xFFFF0000;
  EDIS;

  EALLOW;
  ECanbRegs.CANMIL.all = 0xFFFF0000;
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
send_data (int16 MBXnbr, Uint32 low, Uint32 high)
{

  volatile struct MBOX *Mailbox;
//  for(MBXnbr)
//    {
  Mailbox = &ECanbMboxes.MBOX0 + MBXnbr;
  Mailbox->MDL.all = low; // = 0x9555AAAn (n is the MBX number)
  Mailbox->MDH.all = high; // = 0x89ABCDEF (a constant)
//    }
//******************used for transmit begin*****************
  ECanbRegs.CANTRS.all = 0x00000001;  // Set TRS for all transmit mailboxes

  do
    {
      ECanbShadow.CANTA.all = ECanbRegs.CANTA.all;
    }
  while (ECanbShadow.CANTA.all == 0);   // Wait for TA5 bit to be set..

//	while (ECanbRegs.CANTA.all != 0x0000FFFF) {
//	}  // Wait for all TAn bits to be set..
  ECanbRegs.CANTA.all = 0x0000FFFF;   // Clear all TAn
//******************used for transmit end*****************

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

}

interrupt void
ecan1_intb_isr (void)
{
  int j;
  for (j = 16; j < 32; j++)         // Read 16 mailboxes
    {
      if ((ECanbRegs.CANRMP.all >> j) && 0x1) // only read the mail box which received message
	{
	  mailbox_read (j);        // This func reads the indicated mailbox data
	  ECanbRegs.CANRMP.all = ECanbRegs.CANRMP.all || (0x1 << j); // clear the Received-Message-Pending Register
	}
    }

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
  PieVectTable.ECAN1INTB = &ecan1_intb_isr;		//ECAN interrupt
  EDIS;

  IER |= M_INT9;
  PieCtrlRegs.PIEIER9.bit.INTx8 = 1;  // ECAN-B
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

  // Enable global Interrupts and higher priority real-time debug events:
  EINT;
  // Enable Global interrupt INTM
  ERTM;
  // Enable Global realtime interrupt DBGM
}

