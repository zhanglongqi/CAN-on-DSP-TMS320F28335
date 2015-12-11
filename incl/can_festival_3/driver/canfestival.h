#ifndef DSP_H_
#define DSP_H_

#include "timerscfg.h"
#include "can_driver.h"
#include "data.h"
#include "timers_driver.h"
#include "objdict.h"

UNS8 canSend(CAN_PORT port, Message *m);
CAN_PORT canOpen(s_BOARD *board, CO_Data * d);
int canClose(CO_Data * d);
UNS8 canChangeBaudRate(CAN_PORT port, char* baud);

#endif /*DSP_H_*/
