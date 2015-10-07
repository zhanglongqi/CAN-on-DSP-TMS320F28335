/*
 * main.c
 */

#include "DSP2833x_Device.h"     // Device Header Include File
#include "DSP2833x_Examples.h"
#include "GPIO_Driver.h"

#include <string.h>

#define READY 1
#define SOFTSTART 2
#define RUN	3
#define SOFTOFF 4
#define FAULT 5
#define DEBUG 6

void
main (void)
{

  //0. init Sys
  //0. init Sys
  InitSysCtrl ();
  InitPieCtrl ();

  //1. =========================interrupt=============================================
  // Disable CPU interrupts and clear all CPU interrupt flags:
  DINT;
  IER = 0x0000;
  IFR = 0x0000;

  //ram Funcs

  MemCopy (&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

  InitFlash ();
  // Initialize the PIE vector table with pointers to the shell Interrupt
  // Service Routines (ISR).
  // This will populate the entire table, even if the interrupt
  // is not used in this example.  This is useful for debug purposes.
  // The shell ISR routines are found in DSP2833x_DefaultIsr.c.
  // This function is found in DSP2833x_PieVect.c.c0
  InitPieVectTable ();

  // Interrupts that are used in this example are re-mapped to
  // ISR functions found within this file.
  EALLOW;
  // This is needed to write to EALLOW protected registers
//	PieVectTable.SEQ1INT = &ADC_SEQ1_EOC_isr;
  EDIS;

  // Enable CPU INT1 which is connected to SEQ INT:
//	IER |= M_INT1;
  // Enable SEQ1 INT in the PIE: Group 1 interrupt 1-1
//	PieCtrlRegs.PIEIER1.bit.INTx1 = 1 ;
//	PieCtrlRegs.PIEIER1.bit.INTx7 = 1 ;

  EINT;
  // Enable Global interrupt INTM
  ERTM;
  // Enable Global realtime interrupt DBGM

  //=========================Core Module=============

  //====================Tmr============================
//	initUserTmr1(I2C_TMR) ; //in ms
//	EvMgrPrt->addEvent(EvMgrPrt,&UserTmr1) ;
//	UserTmr1.addEvCallback(&UserTmr1,Response_UserTmr1) ;
  //===============user module=======================
  //initialize GPIO
  InitGpio ();
  //eCan
  InitECanbGpio ();

  configureEcanB ();

  for (;;)
    {
      //      send_data (int16 MBXnbr, Uint32 low, Uint32 high, Uint32 id);
      send_data (0, 0xABABABAB, 0xCDCDCDCD, 0x10101010);
      send_data (1, 0x12345678, 0x87654321, 0x11111111);

    }
}

