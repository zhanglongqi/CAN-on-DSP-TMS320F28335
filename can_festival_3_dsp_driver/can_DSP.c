/*
 This file is part of CanFestival, a library implementing CanOpen Stack.

 Copyright (C): Edouard TISSERANT and Francis DUPIN
 AT91 Port: Peter CHRISTEN

 See COPYING file for copyrights details.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

//#define DEBUG_WAR_CONSOLE_ON
//#define DEBUG_ERR_CONSOLE_ON
#include "canBus.h"
#include "can_DSP.h"
#include "canfestival.h"

// eCAN control registers require read/write access using 32-bits.  Thus we
// will create a set of shadow registers for this example.  These shadow
// registers will be used to make sure the access is 32-bits and not 16.
struct ECAN_REGS ECanbShadow;
union CANMSGCTRL_REG ECanMSGCTRLSahdow;
int n_receive_MB;


void can_irq_handler(void);

unsigned char canInit(unsigned int bitrate)
/******************************************************************************
 Initialize the hardware to receive CAN messages and start the timer for the
 CANopen stack.
 INPUT
 OUTPUT
 ******************************************************************************/{
	configureEcanB();
	return 1;
}

unsigned char canSend(CAN_PORT notused, Message *m)
/******************************************************************************
 The driver send a CAN message passed from the CANopen stack
 INPUT	CAN_PORT is not used (only 1 avaiable)
 Message *m pointer to message to send
 OUTPUT	1 if  hardware -> CAN frame
 ******************************************************************************/{

	Uint16 n_free_MB;
	Uint32 free_MB = 0;	//相应位表示对应邮箱是否空闲
	volatile struct MBOX *mb_ptr;
	struct MBOX mb_shadow;

	mb_ptr = &ECanbMboxes.MBOX0;
	// Search the first free MB	//置位 相应位表示发送请求，中断写1 清除 0: ready
	//从大号的邮箱开始写入，从而保证先写入的有较高优先级
	//保存使用的最小邮箱号码，如果其为START_TX_MB，等待发送完成以后继续寻找空的邮箱；
	//维持一个可用的发送邮箱的数组
	free_MB = ECanbRegs.CANTRS.all;
	for (n_free_MB = START_TX_MB; n_free_MB < NB_MB; n_free_MB++) {
		if ((free_MB & ((Uint32) 0x01 << n_free_MB))
				== ((Uint32) 0x01 << n_free_MB))
			break;
	}
	n_free_MB--;
	if (n_free_MB == START_TX_MB) {
//		LOG_printf(&trace, "no free mailboxes!waiting...");
		while (ECanbRegs.CANTRS.all != 0)
			;
	}

//	LOG_printf(&trace, "n_free_MB  = %u", n_free_MB);
	mb_ptr += n_free_MB;
	// disable Mailboxes */
	ECanbRegs.CANME.all = (~((Uint32) 1 << n_free_MB));

	// Set cob id
//	mb_ptr->MSGID.bit.STDMSGID = 0;/*m->cob_id*/;	//?16 BIT? 本版本中，只是设定高位
	mb_shadow.MSGID.bit.STDMSGID = m->cob_id;
	mb_shadow.MSGID.bit.EXTMSGID_L = 0;
	mb_shadow.MSGID.bit.EXTMSGID_H = 0;
	mb_shadow.MSGID.bit.IDE = 1;	//extension id
	mb_shadow.MSGID.bit.AAM = 0;  //对于发送邮箱有效，无自动应答0
	mb_shadow.MSGID.bit.AME = 1;	//对于接收邮箱有效，使能接收屏蔽位1
	mb_ptr->MSGID.all = mb_shadow.MSGID.all;

	ECanMSGCTRLSahdow.all = 0;
	ECanMSGCTRLSahdow.bit.RTR = m->rtr;
	ECanMSGCTRLSahdow.bit.DLC = m->len;
	mb_ptr->MSGCTRL.all = ECanMSGCTRLSahdow.all;

//direction: readble 0:output
	ECanbShadow.CANMD.all = ECanbRegs.CANMD.all;
	ECanbShadow.CANMD.all &= ~((Uint32) 1 << n_free_MB);
	ECanbRegs.CANMD.all = ECanbShadow.CANMD.all;

// enable it
	ECanbRegs.CANME.all = (~(Uint32) 0);

	/*============================
	 send message
	 =============================*/

//	mb_ptr->MDL.all = *(Uint32*)(&m->data[0]);
//	mb_ptr->MDH.all = *(Uint32*)(&m->data[4]);
	mb_ptr->MDL.byte.BYTE0 = m->data[0];
	mb_ptr->MDL.byte.BYTE1 = m->data[1];
	mb_ptr->MDL.byte.BYTE2 = m->data[2];
	mb_ptr->MDL.byte.BYTE3 = m->data[3];
	mb_ptr->MDH.byte.BYTE4 = m->data[4];
	mb_ptr->MDH.byte.BYTE5 = m->data[5];
	mb_ptr->MDH.byte.BYTE6 = m->data[6];
	mb_ptr->MDH.byte.BYTE7 = m->data[7];

	// Start sending by writing the MB configuration register to transmit
	ECanbRegs.CANTRS.all = ((Uint32) 1 << n_free_MB);

	return 1;	// successful
}

unsigned char canReceive(Message *m)
/******************************************************************************
 The driver passes a received CAN message to the stack
 INPUT	Message *m pointer to received CAN message
 OUTPUT	1 if a message received
 ******************************************************************************/{
	volatile struct MBOX *mb_ptr = &ECanbMboxes.MBOX0;
	// n_receive_MB, the mailbox to be read
//	LOG_printf(&trace, "n_receive_MB  = %u", n_receive_MB);
	mb_ptr += n_receive_MB;

	m->cob_id = mb_ptr->MSGID.bit.STDMSGID;	//

	// Mailbox Control Register, set remote transmission request and data lenght code
	m->rtr = mb_ptr->MSGCTRL.bit.RTR;
	m->len = mb_ptr->MSGCTRL.bit.DLC;

	m->data[0] = mb_ptr->MDL.byte.BYTE0;
	m->data[1] = mb_ptr->MDL.byte.BYTE1;
	m->data[2] = mb_ptr->MDL.byte.BYTE2;
	m->data[3] = mb_ptr->MDL.byte.BYTE3;
	m->data[4] = mb_ptr->MDH.byte.BYTE4;
	m->data[5] = mb_ptr->MDH.byte.BYTE5;
	m->data[6] = mb_ptr->MDH.byte.BYTE6;
	m->data[7] = mb_ptr->MDH.byte.BYTE7;

	return 1;		// message received
}

/******************************************************************************

 ******************************* CAN INTERRUPT  *******************************/

void can_irq_handler(void)
/******************************************************************************
 CAN Interrupt
 ******************************************************************************/{

//	volatile unsigned int status;
//	static Message m = Message_Initializer; // contain a CAN message
//	UNS8 nodeId = 0;
//	asm(" NOP");
//	MIV = ECanbRegs.CANGIF1.bit.MIV1;
//
//	// Mailbox time-out
//	if (ECanbRegs.CANGIF1.bit.MTOF1 == 1)
//		LOG_printf(&trace, "Mailbox :%u Mailbox time-out\n", MIV_CanGIF0);
//
//	//读取接收邮箱；
//	if (MIV < NB_RX_MB) {
//		if (ECanbRegs.CANGIF1.bit.GMIF1 == 1)
//			LOG_printf(&trace, "Mailbox :%u received a message successfully\n",
//					MIV);
//		ECanbShadow.CANRMP.all = ((Uint32) 1 << MIV);
//		ECanbRegs.CANRMP.all = ECanbShadow.CANRMP.all;
//		MessageReceivedCount++;
//		n_receive_MB = MIV;
//		if (canReceive(&m)) {
//#ifdef CAN_STM
//			nodeId = (UNS8)GET_NODE_ID((m));
//			if (nodeId != *TestSlave_Data.bDeviceNodeId)
//			canDispatch(&TestSlave_Data, &m); // process it
//			else if (nodeId != *TestMaster_Data.bDeviceNodeId)
//			canDispatch(&TestMaster_Data, &m);// process it
//#endif
//		}
//	}
//	//清除发送邮箱的标志；
//	else if (MIV < NB_MB) {
//		if (ECanbRegs.CANGIF1.bit.GMIF1 == 1) {
//			volatile struct MBOX *mb_ptr = &ECanbMboxes.MBOX0;
//			mb_ptr += MIV;
//			LOG_printf(&trace,
//					"Mailbox :%u with cob-id:0x%x transmitted a message successfully\n",
//					MIV, mb_ptr->MSGID.bit.STDMSGID);
//		}
//		ECanbShadow.CANTA.all = ((Uint32) 1 << MIV);
//		ECanbRegs.CANTA.all = ECanbShadow.CANTA.all;
//	}
//	PieCtrlRegs.PIEACK.bit.ACK9 = 1; // Enables PIE to drive a pulse into the CPU
//	IER |= 0x0100;                   // Enable INT9
//	EINT;
//
//	static Message m = Message_Initializer; // contain a CAN message
//
//	if (canReceive(&m)) // a message received
//		canDispatch(&ObjDict_Data, &m); // process it

}
